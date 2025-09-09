#include "includes/coordinator.h"
#include "includes/variables.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void coordinator_process(sharedData* data)
{
	printf("Coordinador iniciado. PID: %d\n", getpid());
	FILE* csv = fopen("generated_data.csv", "w");
	if (!csv) {
		perror("Error abriendo archivo CSV");
		cleanup_resources();
		exit(1);
	}
	fprintf(csv, "ID,Name,Age,City,Department,Salary,Experience\n");
	fflush(csv);

	char localBuffer[SHM_SIZE];
	int* regProcesados = malloc(sizeof(int) * data->total);
	memset(regProcesados, 0, sizeof(int) * data->total);

	while (data->regEscritos < data->total &&
				 (data->genActivos > 0 || strlen(data->buffer) > 0) && !data->shutdown_flag) {
		sem_operation(SEM_BUFFER, -1); // Wait for buffer access
		if (strlen(data->buffer) > 0) {
			strcpy(localBuffer, data->buffer);
			memset(data->buffer, 0, SHM_SIZE);
			data->bufferCount = 0;

			// Release buffer mutex immediately after copying data so
			// generators can write while the coordinator processes the
			// local copy. Previously the coordinator held the mutex
			// during processing, causing generators to block indefinitely.
			sem_operation(SEM_BUFFER, 1); // Release buffer access
			char* line = strtok(localBuffer, "\n");
			while (line != NULL) {
				int id;
				if (sscanf(line, "%d,", &id) == 1) {
					if (id >= 0 && id < data->total &&
							!regProcesados[id]) {
						sem_operation(
							SEM_WRITE,
							-1); // Wait for write access
						fprintf(csv, "%s\n", line);
						fflush(csv);
						data->regEscritos++;
						printf("Coordinador: Registro %d escrito en CSV. Total escritos: %d\n",
									 id, data->regEscritos);
						sem_operation(
							SEM_WRITE,
							1); // Release write access
					}
					line = strtok(NULL, "\n");
				}

			}
		}
		else {
			sem_operation(SEM_BUFFER, 1); // Release buffer access
		}
		sleep(DELAY);
	}
	fclose(csv);
	printf("Coordinador finalizado. PID: %d\n", getpid());
	printf("Total de registros procesados: %d\n", data->regEscritos);
}
