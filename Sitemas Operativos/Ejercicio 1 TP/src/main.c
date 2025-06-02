#include "includes/venta.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_HIJOS 4

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
  for (int i = 0; i < DIAS; i++) {
    for (int j = 0; j < mes[i].numVentas; j++) {
      mes[i].ventas[j].montoModificado *= rec;
    }
  }
  sem_signal(semid);
}
void aplicar_iva(Dia *mes, int semid) {
  sem_wait(semid);
  for (int i = 0; i < DIAS; i++) {
    for (int j = 0; j < mes[i].numVentas; j++) {
      mes[i].ventas[j].montoModificado *= 1.21;
    }
  }
  sem_signal(semid);
}
void aplicar_descuento(Dia *mes, int semid) {
  sem_wait(semid);
  for (int i = 0; i < DIAS; i++) {
    for (int j = 0; j < mes[i].numVentas; j++) {
      mes[i].ventas[j].montoModificado *= 0.75;
    }
  }
  sem_signal(semid);
}
void aplicar_descuento_porcentual(Dia *mes, int semid, float desc) {
  sem_wait(semid);
  for (int i = 0; i < DIAS; i++) {
    for (int j = 0; j < mes[i].numVentas; j++) {
      mes[i].ventas[j].montoModificado *= (1 - desc);
    }
  }
  sem_signal(semid);
}

int main() {
  int shmid = shmget(IPC_PRIVATE, DIAS * sizeof(Dia), IPC_CREAT | 0666);
  int shmid_ventas =
      shmget(IPC_PRIVATE, sizeof(Venta) * DIAS * MAX_VENTAS, IPC_CREAT | 0666);
  if (shmid < 0) {
    perror("shmget");
    exit(1);
  }
  MC mem = crearMesYVentas(shmid, shmid_ventas);

  int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

  if (semid < 0) {
    perror("semget");
    exit(1);
  }
  if (semctl(semid, 0, SETVAL, 1) < 0) {
    perror("semctl");
    exit(1);
  }

  for (int i = 0; i < NUM_HIJOS; i++) {
    pid_t pid = fork();
    if (pid == -1) {
      perror("fork");
      exit(1);
    } else if (pid == 0) {
      switch (i) {
      case 0: // Aplicar recargo fijo
        aplicar_recargo(mem.mes, semid, 1.10);
        exit(EXIT_SUCCESS);
        break;
      case 1:
        aplicar_iva(mem.mes, semid);
        exit(EXIT_SUCCESS);
        break;
      case 2: // Aplicar descuento fijo
        aplicar_descuento(mem.mes, semid);
        exit(EXIT_SUCCESS);
        break;
      case 3: // Aplicar descuento porcentual
        aplicar_descuento_porcentual(mem.mes, semid, 0.15);
        exit(EXIT_SUCCESS);
        break;
      }
    }
  }
  for (int i = 0; i < NUM_HIJOS; i++) {
    wait(NULL);
  }
  printf("\n=== RESUMEN FINAL ===\n");
  // getchar();
  for (int d = 0; d < DIAS; d++) {
    double nuevo_total = 0;
    for (int j = 0; j < mem.mes[d].numVentas; j++) {
      nuevo_total += mem.mes[d].ventas[j].montoModificado;
    }
    printf("Día %2d: Original: $%8.2f | Modificado: $%8.2f\n", d + 1,
           (mem.mes)[d].total, nuevo_total);
  }
  shmdt(mem.mes);
  shmdt(mem.ventaBase);
  shmctl(shmid, IPC_RMID, NULL);
  shmctl(shmid_ventas, IPC_RMID, NULL);
  semctl(semid, 0, IPC_RMID, 0);
  return 0;
}
