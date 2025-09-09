#include "includes/variables.h"
#include "includes/coordinator.h"
#include "includes/generator.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Variables globales para limpieza
static sharedData* data = NULL;
static pid_t* generador = NULL;
static pid_t coordinator_pid = -1;
static int cantGen = 0;
static int total;
static int shm_id = -1;
static int sem_id = -1;

//UTILS
int validate_parameters(int argc, char* argv[], int* num_gen, int* total_rec);
void asignarSignals();
void cleanup_resources();

//SEMAFOROS
int sem_operation(int sem_num, int op);
void crearSemaforos();
void initSemaforos();

//MEMORIA COMPARTIDA
void crearMemoriaCompartida();
void initMemoriaCompartida();

int main(int argc, char* argv[])
{
	if (!validate_parameters(argc, argv, &cantGen, &total)) {
		return 1;
	}

	printf("Iniciando sistema con %d generadores y %d registros\n", cantGen,
				 total);
	printf("PID del proceso principal: %d\n", getpid());

	// Configurar manejadores de señales
	asignarSignals();

	//Memoria Compartida
	crearMemoriaCompartida();
	initMemoriaCompartida();

	//Semáforos
	crearSemaforos();
	initSemaforos();

	// Crear array para PIDs de generadores
	generador = malloc(cantGen * sizeof(pid_t));

	// Crear proceso coordinador
	coordinator_pid = fork();
	if (coordinator_pid == 0) {
		coordinator_process(data);
		exit(0);
	}
	else if (coordinator_pid == -1) {
		perror("Error creando coordinador");
		cleanup_resources();
		return 1;
	}
	sleep(1); // Dar tiempo al coordinador para inicializar

	// Crear procesos generadores
	for (int i = 0; i < cantGen; i++) {
		generador[i] = fork();
		if (generador[i] == 0) {
			generator_process(i + 1, data);
			exit(0);
		}
		else if (generador[i] == -1) {
			perror("Error creando generador");
			break;
		}
	}

	// Esperar a que terminen los generadores
	for (int i = 0; i < cantGen; i++) {
		if (generador[i] > 0) {
			int status;
			waitpid(generador[i], &status, 0);
		}
	}
	sleep(1);

	int status;
	waitpid(coordinator_pid, &status, 0);

	printf("\nSistema finalizado exitosamente\n");
	printf("Total de registros generados: %d\n", data->regEscritos);

	cleanup_resources();
	return 0;
}

//UTILS
int validate_parameters(int argc, char* argv[], int* num_gen, int* total_rec)
{
	if (argc != 3) {
		printf("Uso: %s <num_generadores> <total_registros>\n",
					 argv[0]);
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
		printf("Error: Total de registros debe estar entre 1 y %d\n",
					 MAX_RECORDS);
		return 0;
	}

	return 1;
}
void cleanup_resources()
{
	printf("Limpiando recursos del sistema...\n");

	if (data) {
		printf("generadores activos: %d\n", data->genActivos);
		data->shutdown_flag = 1;
		shmdt(data);
	}
	printf("%d\n", shm_id);
	if (shm_id != -1) {
		printf("Eliminando memoria compartida...\n");
		shmctl(shm_id, IPC_RMID, NULL);
	}

	if (sem_id != -1) {
		semctl(sem_id, 0, IPC_RMID);
	}

	if (generador) {
		free(generador);
	}
}
void signal_handler(int sig)
{
	printf("\nSeñal %d recibida. Iniciando terminación controlada...\n",
				 sig);
	cleanup_resources();
	exit(0);
}
void sigchld_handler()
{
	int status;
	pid_t pid;
	// Reap any children that have changed state without blocking
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
		printf("Proceso hijo %d terminó\n", pid);
		if (WIFSIGNALED(status)) {
			int term_sig = WTERMSIG(status);
			// Solo si el hijo fue asesinado con SIGKILL ejecutamos la terminación
			// controlada
			if (term_sig == SIGKILL) {
				printf("\nHijo %d terminado por señal %d. Iniciando terminación "
							 "controlada...\n",
							 pid, term_sig);
				cleanup_resources();
				exit(0);
			}
			else {
				exit(0);
			}
		}
	}
}
void asignarSignals()
{
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGCHLD, (void (*)(int))sigchld_handler);
	signal(SIGQUIT, signal_handler);
}

//SEMAFOROS
int sem_operation(int sem_num, int op)
{
	struct sembuf sop = { sem_num, op, SEM_UNDO };
	return semop(sem_id, &sop, 1);
}
void crearSemaforos()
{
	sem_id = semget(IPC_PRIVATE, SEM_COUNT, IPC_CREAT | 0666);
	if (sem_id == -1) {
		perror("Error creando semáforos");
		cleanup_resources();
		exit(1);
	}
}
void initSemaforos()
{
	// Inicializar semáforos
	for (int i = 0; i < SEM_COUNT; i++) {
		if (semctl(sem_id, i, SETVAL, 1) == -1) {
			perror("Error inicializando semáforos");
			cleanup_resources();
			exit(1);
		}
	}
}

//MEMORIA COMPARTIDA
void crearMemoriaCompartida()
{
	shm_id = shmget(IPC_PRIVATE, sizeof(sharedData), IPC_CREAT | 0666);
	if (shm_id == -1) {
		perror("Error creando memoria compartida");
		cleanup_resources();
		exit(1);
	}
}
void initMemoriaCompartida()
{
	data = (sharedData*)shmat(shm_id, NULL, 0);
	if (data == (void*)-1) {
		perror("Error adjuntando memoria compartida");
		cleanup_resources();
		exit(1);
	}
	// Inicializar datos compartidos
	data->nextId = 1;
	data->total = total;
	data->genActivos = cantGen;
}
