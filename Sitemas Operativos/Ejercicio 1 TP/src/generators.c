
#include "includes/generator.h"
#include "includes/variables.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
// Datos para generar registros aleatorios
static const char *names[] = { "Juan", "Maria",	 "Carlos", "Ana",
			       "Luis", "Carmen", "Pedro",  "Laura" };
static const char *cities[] = { "Madrid",  "Barcelona", "Valencia",
				"Sevilla", "Bilbao",	"Malaga" };
static const char *departments[] = { "IT",	  "HR",	     "Sales",
				     "Marketing", "Finance", "Operations" };

int getNextIds(int *ids, int maxCount, sharedData *data);
void generateRecord(int id, Record *record);
int sendToBuffer(Record *record, sharedData *data);
void generator_process(int genId, sharedData *data)
{
	printf("Generador %d iniciado (PID: %d)\n", genId, getpid());
	int ids[IDS_PER_REQUEST];
	Record reg;

	while (!data->shutdown_flag) {
		int count = getNextIds(ids, IDS_PER_REQUEST, data);
		if (count == 0) {
			break; // No more IDs to generate
		}
		for (int i = 0; i < count; i++) {
			generateRecord(ids[i], &reg);
			while (!sendToBuffer(&reg, data)) {
				//sleep(1); // Esperar 100ms antes de reintentar
			}
			//	printf("Generador %d: Registro %d generado y enviado al buffer\n",
			//       genId, ids[i]);
			//sleep(DELAY);
		}
	}
	sem_operation(SEM_IDS, 1); // Liberar semáforo de IDs
	data->genActivos--;
	sem_operation(SEM_IDS, 1); // Liberar semáforo de IDs
	printf("Generador %d finalizado\n", genId);
}

void generateRecord(int id, Record *record)
{
	srand(time(NULL) + getpid() + id);
	int age = 18 + rand() % 48;
	int salario = 30000 + rand() % 70000;
	int experiencia = rand() % (age - 18 + 1);
	record->id = id;
	record->age = age;
	record->experience = experiencia;
	record->salary = salario;
	strcpy(record->name, names[rand() % 8]);
	strcpy(record->city, cities[rand() % 6]);
	strcpy(record->department, departments[rand() % 6]);
}
int sendToBuffer(Record *record, sharedData *data)
{
	sem_operation(SEM_BUFFER, -1); // Esperar espacio en buffer
	if (data->bufferCount >= 1) {
		sem_operation(SEM_BUFFER, 1); // Liberar espacio en buffer
		return 0; // Buffer full or shutdown
	}
	data->buffer = *record;
	data->bufferCount++;
	sem_operation(SEM_BUFFER, 1); // Liberar espacio en buffer
	return 1;
}
int getNextIds(int *ids, int maxCount, sharedData *data)
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
