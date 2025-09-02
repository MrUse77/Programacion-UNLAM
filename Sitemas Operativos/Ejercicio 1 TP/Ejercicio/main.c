#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MAX_GENERATORS 20
#define MAX_RECORDS 100000
#define RECORD_SIZE 256
#define SHM_SIZE (RECORD_SIZE * 100)
#define IDS_PER_REQUEST 10
#define DELAY 5

// Estructura para memoria compartida
typedef struct {
  int next_id;
  int total_records;
  int records_written;
  int active_generators;
  int shutdown_flag;
  char buffer[SHM_SIZE];
  int buffer_count;
} shared_data_t;

// Variables globales para limpieza
static int shm_id = -1;
static int sem_id = -1;
static shared_data_t *shared_data = NULL;
static pid_t *generator_pids = NULL;
static int num_generators = 0;

// Semáforos: 0=mutex_ids, 1=mutex_buffer, 2=mutex_write
enum { SEM_IDS, SEM_BUFFER, SEM_WRITE, SEM_COUNT };

// Datos para generar registros aleatorios
static const char *names[] = {"Juan", "Maria",  "Carlos", "Ana",
                              "Luis", "Carmen", "Pedro",  "Laura"};
static const char *cities[] = {"Madrid",  "Barcelona", "Valencia",
                               "Sevilla", "Bilbao",    "Malaga"};
static const char *departments[] = {"IT",        "HR",      "Sales",
                                    "Marketing", "Finance", "Operations"};

// Operaciones de semáforos
struct sembuf sem_lock = {0, -1, SEM_UNDO};
struct sembuf sem_unlock = {0, 1, SEM_UNDO};

void cleanup_resources() {
  printf("Limpiando recursos del sistema...\n");

  if (shared_data) {
    shared_data->shutdown_flag = 1;
    shmdt(shared_data);
  }

  if (shm_id != -1) {
    shmctl(shm_id, IPC_RMID, NULL);
  }

  if (sem_id != -1) {
    semctl(sem_id, 0, IPC_RMID);
  }

  if (generator_pids) {
    free(generator_pids);
  }
}

void signal_handler(int sig) {
  printf("\nSeñal %d recibida. Iniciando terminación controlada...\n", sig);
  cleanup_resources();
  exit(0);
}

// Manejador de SIGCHLD: solo dispara la limpieza si un hijo fue terminado por señal SIGKILL
void sigchld_handler(int sig) {
  int status;
  pid_t pid;
  // Reap any children that have changed state without blocking
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    if (WIFSIGNALED(status)) {
      int term_sig = WTERMSIG(status);
      // Solo si el hijo fue asesinado con SIGKILL ejecutamos la terminación controlada
      if (term_sig == SIGKILL) {
        printf("\nHijo %d terminado por señal %d. Iniciando terminación controlada...\n", pid, term_sig);	
        cleanup_resources();
        exit(0);
      }else{
				exit(0);
			}
    }
  }
}

int sem_operation(int sem_num, int op) {
  struct sembuf sop = {sem_num, op, SEM_UNDO};
  return semop(sem_id, &sop, 1);
}

int get_next_ids(int *ids, int count) {
  sem_operation(SEM_IDS, -1); // Lock

  if (shared_data->next_id > shared_data->total_records ||
      shared_data->shutdown_flag) {
    sem_operation(SEM_IDS, 1); // Unlock
    return 0;
  }

  int assigned = 0;
  for (int i = 0;
       i < count && shared_data->next_id <= shared_data->total_records; i++) {
    ids[i] = shared_data->next_id++;
    assigned++;
  }

  sem_operation(SEM_IDS, 1); // Unlock
  return assigned;
}

void generate_record(int id, char *record) {
  srand(time(NULL) + getpid() + id);
  int age = 18 + rand() % 48;
  int salario = 30000 + rand() % 70000;
	int experiencia = rand() % (age - 18 + 1);

  snprintf(record, RECORD_SIZE, "%d,%s,%d,%s,%s,%d,%d\n", id, names[rand() % 8],
           age, cities[rand() % 6], departments[rand() % 6], salario,
           experiencia);
}

int send_record_to_coordinator(const char *record) {
  sem_operation(SEM_BUFFER, -1); // Lock buffer

  int len = strlen(record);
  if (strlen(shared_data->buffer) + len < SHM_SIZE - 1) {
    strcat(shared_data->buffer, record);
    shared_data->buffer_count++;
    sem_operation(SEM_BUFFER, 1); // Unlock
    return 1;
  }

  sem_operation(SEM_BUFFER, 1); // Unlock
  return 0;                     // Buffer lleno
}

void generator_process(int generator_id) {
  printf("Generador %d iniciado (PID: %d)\n", generator_id, getpid());

  int ids[IDS_PER_REQUEST];
  char record[RECORD_SIZE];

  while (!shared_data->shutdown_flag) {
    int count = get_next_ids(ids, IDS_PER_REQUEST);
    if (count == 0)
      break;

    for (int i = 0; i < count; i++) {
      if (shared_data->shutdown_flag)
        break;

      generate_record(ids[i], record);

      // Intentar enviar registro con reintentos
      int attempts = 0;
      while (attempts < 100 && !send_record_to_coordinator(record)) {
        usleep(1000); // 1ms
        attempts++;
      }

      if (attempts >= 100) {
        printf("Generador %d: Buffer saturado, terminando\n", generator_id);
        break;
      }
			printf("Generador %d: Registro %d generado y enviado\n", generator_id, ids[i]);
      sleep(DELAY); // Simular tiempo de procesamiento
    }
  }

  // Decrementar contador de generadores activos
  sem_operation(SEM_IDS, -1);
  shared_data->active_generators--;
  sem_operation(SEM_IDS, 1);

  printf("Generador %d finalizado\n", generator_id);
}

void coordinator_process() {
  printf("Coordinador iniciado (PID: %d)\n", getpid());

  FILE *csv_file = fopen("generated_data.csv", "w");
  if (!csv_file) {
    perror("Error creando archivo CSV");
    return;
  }

  // Escribir encabezados
  fprintf(csv_file, "ID,Name,Age,City,Department,Salary,Experience\n");
  fflush(csv_file);

  char local_buffer[SHM_SIZE];
  int processed_ids[MAX_RECORDS] = {0}; // Para evitar duplicados

  while (
      shared_data->records_written < shared_data->total_records &&
      (shared_data->active_generators > 0 || strlen(shared_data->buffer) > 0)) {

    sem_operation(SEM_BUFFER, -1); // Lock buffer

    if (strlen(shared_data->buffer) > 0) {
      strcpy(local_buffer, shared_data->buffer);
      memset(shared_data->buffer, 0, SHM_SIZE);
      shared_data->buffer_count = 0;
      sem_operation(SEM_BUFFER, 1); // Unlock

      // Procesar registros
      char *line = strtok(local_buffer, "\n");
      while (line != NULL) {
        int id;
        if (sscanf(line, "%d,", &id) == 1) {
          if (id > 0 && id <= shared_data->total_records &&
              !processed_ids[id - 1]) {
            processed_ids[id - 1] = 1;

            sem_operation(SEM_WRITE, -1); // Lock escritura
            fprintf(csv_file, "%s\n", line);
            fflush(csv_file);
            shared_data->records_written++;
            sem_operation(SEM_WRITE, 1); // Unlock
          }
        }
        line = strtok(NULL, "\n");
      }
    } else {
      sem_operation(SEM_BUFFER, 1); // Unlock
    }

    sleep(DELAY); // 10ms
  }

  fclose(csv_file);
  printf("Coordinador finalizado. Registros escritos: %d\n",
         shared_data->records_written);
}

int validate_parameters(int argc, char *argv[], int *num_gen, int *total_rec) {
  if (argc != 3) {
    printf("Uso: %s <num_generadores> <total_registros>\n", argv[0]);
    printf("Ejemplo: %s 4 1000\n", argv[0]);
    printf("\nParámetros:\n");
    printf("  num_generadores: 1-%d\n", MAX_GENERATORS);
    printf("  total_registros: 1-%d\n", MAX_RECORDS);
    return 0;
  }

  *num_gen = atoi(argv[1]);
  *total_rec = atoi(argv[2]);

  if (*num_gen <= 0 || *num_gen > MAX_GENERATORS) {
    printf("Error: Número de generadores debe estar entre 1 y %d\n",
           MAX_GENERATORS);
    return 0;
  }

  if (*total_rec <= 0 || *total_rec > MAX_RECORDS) {
    printf("Error: Total de registros debe estar entre 1 y %d\n", MAX_RECORDS);
    return 0;
  }

  return 1;
}

int main(int argc, char *argv[]) {
  int total_records;

  if (!validate_parameters(argc, argv, &num_generators, &total_records)) {
    return 1;
  }

  printf("Iniciando sistema con %d generadores y %d registros\n",
         num_generators, total_records);
  printf("PID del proceso principal: %d\n", getpid());

  // Configurar manejadores de señales
  signal(SIGINT, signal_handler);
	signal(SIGCHLD, sigchld_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGTERM, signal_handler);

  // Crear memoria compartida
  key_t shm_key = ftok(".", 'S');
  shm_id = shmget(shm_key, sizeof(shared_data_t), IPC_CREAT | 0666);
  if (shm_id == -1) {
    perror("Error creando memoria compartida");
    return 1;
  }

  shared_data = (shared_data_t *)shmat(shm_id, NULL, 0);
  if (shared_data == (void *)-1) {
    perror("Error adjuntando memoria compartida");
    cleanup_resources();
    return 1;
  }

  // Inicializar memoria compartida
  memset(shared_data, 0, sizeof(shared_data_t));
  shared_data->next_id = 1;
  shared_data->total_records = total_records;
  shared_data->active_generators = num_generators;

  // Crear semáforos
  sem_id = semget(IPC_PRIVATE, SEM_COUNT, IPC_CREAT | 0666);
  if (sem_id == -1) {
    perror("Error creando semáforos");
    cleanup_resources();
    return 1;
  }

  // Inicializar semáforos
  for (int i = 0; i < SEM_COUNT; i++) {
    if (semctl(sem_id, i, SETVAL, 1) == -1) {
      perror("Error inicializando semáforos");
      cleanup_resources();
      return 1;
    }
  }

  // Crear array para PIDs de generadores
  generator_pids = malloc(num_generators * sizeof(pid_t));

  // Crear proceso coordinador
  pid_t coordinator_pid = fork();
  if (coordinator_pid == 0) {
    coordinator_process();
    exit(0);
  } else if (coordinator_pid == -1) {
    perror("Error creando coordinador");
    cleanup_resources();
    return 1;
  }

  sleep(1); // Dar tiempo al coordinador para inicializar

  // Crear procesos generadores
  for (int i = 0; i < num_generators; i++) {
    generator_pids[i] = fork();
    if (generator_pids[i] == 0) {
      generator_process(i + 1);
      exit(0);
    } else if (generator_pids[i] == -1) {
      perror("Error creando generador");
      shared_data->shutdown_flag = 1;
      break;
    }
  }

  // Esperar a que terminen los generadores
  for (int i = 0; i < num_generators; i++) {
    if (generator_pids[i] > 0) {
      int status;
      waitpid(generator_pids[i], &status, 0);
    }
  }

  // Esperar al coordinador
  sleep(2); // Dar tiempo para procesar registros restantes
  shared_data->shutdown_flag = 1;

  int status;
  waitpid(coordinator_pid, &status, 0);

  printf("\nSistema finalizado exitosamente\n");
  printf("Total de registros generados: %d\n", shared_data->records_written);

  cleanup_resources();
  return 0;
}
