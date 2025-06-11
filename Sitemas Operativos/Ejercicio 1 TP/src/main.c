#include "includes/venta.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_HIJOS 4

MC mem;
int shmid = -1, shmid_ventas = -1, semid = -1;
pid_t hijos[NUM_HIJOS] = {0};
// Funciones de semáforo
// Semáforo para esperar
int sem_wait(int semid) {
  struct sembuf op = {0, -1, 0};
  return semop(semid, &op, 1);
}
// Semáforo para señalizar
int sem_signal(int semid) {
  struct sembuf op = {0, 1, 0};
  return semop(semid, &op, 1);
}
void aplicar_recargo(Dia *mes, int semid, float rec) {
  sem_wait(semid);
  sleep(10);
  for (int i = 0; i < DIAS; i++) {
    for (int j = 0; j < mes[i].numVentas; j++) {
      mes[i].ventas[j].montoModificado *= rec;
    }
  }
  sem_signal(semid);
}
void aplicar_iva(Dia *mes, int semid) {
  sem_wait(semid);
  sleep(10);
  for (int i = 0; i < DIAS; i++) {
    for (int j = 0; j < mes[i].numVentas; j++) {
      mes[i].ventas[j].montoModificado *= 1.21;
    }
  }
  sem_signal(semid);
}
void aplicar_descuento(Dia *mes, int semid) {
  sem_wait(semid);
  sleep(10);
  for (int i = 0; i < DIAS; i++) {
    for (int j = 0; j < mes[i].numVentas; j++) {
      mes[i].ventas[j].montoModificado *= 0.75;
    }
  }
  sem_signal(semid);
}
void aplicar_descuento_porcentual(Dia *mes, int semid, float desc) {
  sem_wait(semid);
  sleep(10);
  for (int i = 0; i < DIAS; i++) {
    for (int j = 0; j < mes[i].numVentas; j++) {
      mes[i].ventas[j].montoModificado *= (1 - desc);
    }
  }
  sem_signal(semid);
};
void clean() {
  if (shmid != -1)
    shmctl(shmid, IPC_RMID, NULL);
  if (shmid_ventas != -1)
    shmctl(shmid_ventas, IPC_RMID, NULL);
  if (semid != -1)
    semctl(semid, 0, IPC_RMID);
}
void signal_handler(int sign) {
  clean();
  exit(EXIT_SUCCESS);
}

void asignar_signals() {
  int signals[] = {SIGINT, SIGTERM, SIGQUIT, SIGUSR1, SIGUSR2};
  for (int i = 0; i < sizeof(signals) / sizeof(int); i++) {
    if (signal(signals[i], signal_handler) == SIG_ERR) {
      clean();
      perror("Error al asignar signal");
      exit(EXIT_FAILURE);
    }
  }
}
int main() {
  asignar_signals();
  shmid = shmget(IPC_PRIVATE, DIAS * sizeof(Dia), IPC_CREAT | 0666);
  if (shmid < 0) {
    clean();
    perror("shmget");
    exit(1);
  }
  shmid_ventas =
      shmget(IPC_PRIVATE, sizeof(Venta) * DIAS * MAX_VENTAS, IPC_CREAT | 0666);
  if (shmid_ventas < 0) {
    clean();
    perror("shmget ventas");
    exit(1);
  }

  semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

  if (semid < 0) {
    clean();
    perror("semget");
    exit(1);
  }
  if (semctl(semid, 0, SETVAL, 1) < 0) {
    clean();
    perror("semctl");
    exit(1);
  }

  mem = crearMesYVentas(shmid, shmid_ventas);
  for (int i = 0; i < NUM_HIJOS; i++) {
    *(hijos + i) = fork();
    if (*(hijos + i) == -1) {
      clean();
      perror("fork");
      exit(1);
    } else if (*(hijos + i) == 0) {
      sleep(10);
      switch (i) {
      case 0:
        aplicar_recargo(mem.mes, semid, 1.10);
        break;
      case 1:
        aplicar_iva(mem.mes, semid);
        break;
      case 2:
        aplicar_descuento(mem.mes, semid);
        break;
      case 3:
        aplicar_descuento_porcentual(mem.mes, semid, 0.15);
        break;
      }
      exit(EXIT_SUCCESS);
    }
  }
  for (int i = 0; i < NUM_HIJOS; i++) {
    int status;
    waitpid(hijos[i], &status, 0); // espera a cada hijo específico
  }
  printf("\n=== RESUMEN FINAL ===\n");
  for (int d = 0; d < DIAS; d++) {
    double nuevo_total = 0;
    for (int j = 0; j < mem.mes[d].numVentas; j++) {
      nuevo_total += mem.mes[d].ventas[j].montoModificado;
    }
    printf("Día %2d: Original: $%8.2f | Modificado: $%8.2f\n", d + 1,
           (mem.mes)[d].total, nuevo_total);
  }
  clean();
  return 0;
}
