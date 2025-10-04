#include "includes/coordinator.h"
#include "includes/variables.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void coordinator_process(sharedData *data)
{
	FILE *csv = fopen("generated_data.csv", "w");
	if (!csv) {
		perror("Error abriendo archivo CSV");
		cleanup_resources();
		exit(1);
	}
	fprintf(csv, "ID,Name,Age,City,Department,Salary,Experience\n");
	fflush(csv);

	Record reg;
	int *regProcesados = malloc(sizeof(int) * data->total);
	memset(regProcesados, 0, sizeof(int) * data->total);

	while (data->regEscritos < data->total &&
	       (data->genActivos > 0 || data->bufferCount > 0) &&
	       !data->shutdown_flag) {
		sem_operation(SEM_BUFFER, -1); // Wait for buffer access
		if (data->bufferCount > 0) {
			reg = data->buffer;
			data->bufferCount = 0;
			sem_operation(SEM_BUFFER, 1); // Release buffer access
			sem_operation(SEM_WRITE,
				      -1); // Wait for write access
			fprintf(csv, "%d,%s,%d,%s,%s,%d,%d\n", reg.id, reg.name,
				reg.age, reg.city, reg.department, reg.salary,
				reg.experience);
			fflush(csv);
			data->regEscritos++;
			//printf("Coordinador: Registro %d escrito en CSV. Total escritos: %d\n",id, data->regEscritos);
			sem_operation(SEM_WRITE,
				      1); // Release write access
		} else {
			sem_operation(SEM_BUFFER, 1); // Release buffer access
		}
		//sleep(DELAY);
	}
	fclose(csv);
	printf("Total de registros procesados: %d\n", data->regEscritos);
}
