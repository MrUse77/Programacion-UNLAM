#ifndef VARIABLES_H
#define VARIABLES_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#define MAX_GENERATORS 20
#define MAX_RECORDS 100000
#define RECORD_SIZE 256
#define SHM_SIZE (RECORD_SIZE * 100)
#define IDS_PER_REQUEST 10
#define DELAY 1
typedef struct {
	int nextId;
	int total;
	int regEscritos;
	int shutdown_flag;
	int genActivos;
	char buffer[SHM_SIZE];
	int bufferCount;
} sharedData;

// Semáforos: 0=mutex_ids, 1=mutex_buffer, 2=mutex_write
enum { SEM_IDS, SEM_BUFFER, SEM_WRITE, SEM_COUNT };

void cleanup_resources();
int sem_operation(int sem_num, int op);
void signal_handler(int sig);
void sigchld_handler();
#endif // VARIABLES_H
