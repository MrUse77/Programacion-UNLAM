
#include "includes/generator.h"
#include "includes/variables.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
// Datos para generar registros aleatorios
static const char* names[] = { "Juan", "Maria",	 "Carlos", "Ana",
						 "Luis", "Carmen", "Pedro",  "Laura" };
static const char* cities[] = { "Madrid",  "Barcelona", "Valencia",
				"Sevilla", "Bilbao",	"Malaga" };
static const char* departments[] = { "IT",	  "HR",	     "Sales",
						 "Marketing", "Finance", "Operations" };

int getNextIds(int* ids, int maxCount, sharedData* data);
void generateRecord(int id, char* record);
int sendToBuffer(char* record, sharedData* data);
void generator_process(int genId, sharedData* data)
{

	printf("Generador %d iniciado (PID: %d)\n", genId, getpid());
	int ids[IDS_PER_REQUEST];
	char reg[RECORD_SIZE];

	while (!data->shutdown_flag) {
		int count = getNextIds(ids, IDS_PER_REQUEST, data);
		if (count == 0) {
			break; // No more IDs to generate
		}
		for (int i = 0; i < count; i++) {
			if (data->shutdown_flag)
				break;
			generateRecord(ids[i], reg);
			int attemps = 0;
			printf("Generador %d: Intentando enviar registro %d al buffer\n",
						 genId, ids[i]);
			while (attemps < 100 && !sendToBuffer(reg, data)) {
				printf("Intento %d: Buffer lleno, generador %d esperando para enviar registro %d\n",
							 attemps, genId, ids[i]);
				attemps++;
				sleep(0.1); // Esperar 100ms antes de reintentar
			}
			if (attemps >= 100) {
				printf("Generador %d: No se pudo enviar el registro %d después de múltiples intentos. Saliendo...\n",
							 genId, ids[i]);
				break;
			}
			printf("Generador %d: Registro %d generado y enviado al buffer\n",
						 genId, ids[i]);
			sleep(DELAY);
		}
	}
	sem_operation(SEM_IDS, 1); // Liberar semáforo de IDs
	data->genActivos--;
	sem_operation(SEM_IDS, 1); // Liberar semáforo de IDs
	printf("Generador %d finalizado\n", genId);
}

void generateRecord(int id, char* record)
{
	srand(time(NULL) + getpid() + id);
	int age = 18 + rand() % 48;
	int salario = 30000 + rand() % 70000;
	int experiencia = rand() % (age - 18 + 1);
	snprintf(record, RECORD_SIZE, "%d,%s,%d,%s,%s,%d,%d\n", id,
					 names[rand() % 8], age, cities[rand() % 6],
					 departments[rand() % 6], salario, experiencia);
	printf("Registro generado: %s", record);
}
int sendToBuffer(char* record, sharedData* data)
{
	sem_operation(SEM_BUFFER, -1); // Esperar espacio en buffer
	printf("Buffer actual: %d registros\n", data->bufferCount);
	int len = strlen(record);
	if (strlen(data->buffer) + len < SHM_SIZE - 1) {
		strcat(data->buffer, record);
		data->bufferCount++;

		printf("Registro enviado al buffer. Buffer ahora tiene %d registros\n",
					 data->bufferCount);
		sem_operation(SEM_BUFFER, 1); // Liberar espacio en buffer
		return 1;
	}
	printf("Buffer lleno, no se pudo enviar registro\n");
	sem_operation(SEM_BUFFER, 1); // Liberar espacio en buffer
	return 0;
}
int getNextIds(int* ids, int maxCount, sharedData* data)
{
	sem_operation(SEM_IDS, -1); // Esperar semáforo de IDs
	if (data->nextId > data->total || data->shutdown_flag) {
		sem_operation(SEM_IDS, 1); // Liberar semáforo de IDs
		return 0; // No more IDs to generate
	}
	int assigned = 0;
	for (int i = 0; i < maxCount && data->nextId <= data->total; i++) {
		ids[i] = data->nextId++;
		assigned++;
	}
	sem_operation(SEM_IDS, 1); // Liberar semáforo de IDs
	return assigned;
}
