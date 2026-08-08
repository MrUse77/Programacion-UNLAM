#include "../includes/database.h"
#include "../includes/protocol.h"
#include <ctype.h>

// Funciones de inicialización
database_t *init_database(void)
{
	database_t *db = malloc(sizeof(database_t));
	if (!db) {
		return NULL;
	}

	memset(db, 0, sizeof(database_t));

	// Inicializar mutex
	if (pthread_mutex_init(&db->db_mutex, NULL) != 0) {
		free(db);
		return NULL;
	}

	db->transaction_active = 0;
	db->transaction_client_id = -1;

	return db;
}

// Función para cargar base de datos desde CSV
int load_database_from_csv(database_t *db, const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file) {
		return -1;
	}

	char line[MAX_BUFFER_SIZE];
	int line_number = 0;

	pthread_mutex_lock(&db->db_mutex);

	// Leer encabezados
	if (fgets(line, sizeof(line), file)) {
		line_number++;

		// Remover salto de línea
		line[strcspn(line, "\n")] = 0;

		// Parsear encabezados
		char *token = strtok(line, ",");
		db->num_fields = 0;

		while (token && db->num_fields < MAX_FIELDS) {
			trim_whitespace(token);
			strncpy(db->headers[db->num_fields], token,
				MAX_FIELD_SIZE - 1);
			db->headers[db->num_fields][MAX_FIELD_SIZE - 1] = '\0';
			db->num_fields++;
			token = strtok(NULL, ",");
		}
	}

	// Leer registros
	db->num_records = 0;
	while (fgets(line, sizeof(line), file) &&
	       db->num_records < MAX_RECORDS) {
		line_number++;
		line[strcspn(line, "\n")] = 0;

		if (strlen(line) == 0)
			continue;

		char *token = strtok(line, ",");
		int field_count = 0;

		while (token && field_count < db->num_fields) {
			trim_whitespace(token);
			strncpy(db->records[db->num_records].fields[field_count],
				token, MAX_FIELD_SIZE - 1);
			db->records[db->num_records]
				.fields[field_count][MAX_FIELD_SIZE - 1] = '\0';
			field_count++;
			token = strtok(NULL, ",");
		}

		if (field_count == db->num_fields) {
			db->records[db->num_records].id =
				atoi(db->records[db->num_records].fields[0]);
			db->records[db->num_records].is_deleted = 0;
			db->num_records++;
		}
	}

	pthread_mutex_unlock(&db->db_mutex);
	fclose(file);

	return db->num_records;
}

// Función para guardar base de datos a CSV
int save_database_to_csv(database_t *db, const char *filename)
{
	FILE *file = fopen(filename, "w");
	if (!file) {
		return -1;
	}

	pthread_mutex_lock(&db->db_mutex);

	// Escribir encabezados
	for (int i = 0; i < db->num_fields; i++) {
		fprintf(file, "%s", db->headers[i]);
		if (i < db->num_fields - 1) {
			fprintf(file, ",");
		}
	}
	fprintf(file, "\n");

	// Escribir registros activos
	for (int i = 0; i < db->num_records; i++) {
		if (!db->records[i].is_deleted) {
			for (int j = 0; j < db->num_fields; j++) {
				fprintf(file, "%s", db->records[i].fields[j]);
				if (j < db->num_fields - 1) {
					fprintf(file, ",");
				}
			}
			fprintf(file, "\n");
		}
	}

	pthread_mutex_unlock(&db->db_mutex);
	fclose(file);

	return 0;
}

// Función para limpiar base de datos
void cleanup_database(database_t *db)
{
	if (db) {
		pthread_mutex_destroy(&db->db_mutex);
		free(db);
	}
}

// Operación SELECT
int db_select(database_t *db, const char *conditions, const char *fields,
	      char *result, int max_len)
{
	if (!db || !result)
		return -1;

	pthread_mutex_lock(&db->db_mutex);

	char output[MAX_BUFFER_SIZE * 2];
	int offset = 0;
	int record_count = 0;

	// Agregar encabezado
	format_table_header(db, output, sizeof(output));
	offset = strlen(output);

	// Buscar registros que coincidan
	for (int i = 0; i < db->num_records && offset < sizeof(output) - 200;
	     i++) {
		if (db->records[i].is_deleted)
			continue;
		// Evaluar condición si existe
		if (conditions && strlen(conditions) > 0) {
			if (!evaluate_condition(&db->records[i], db,
						conditions)) {
				continue;
			}
		}

		// Formatear registro
		char record_output[500];
		format_record_output(&db->records[i], db, record_output,
				     sizeof(record_output));

		if (offset + strlen(record_output) < sizeof(output)) {
			strcpy(output + offset, record_output);
			offset += strlen(record_output);
			record_count++;
		}
	}

	// Agregar resumen
	if (offset < sizeof(output) - 50) {
		snprintf(output + offset, sizeof(output) - offset,
			 "\n(%d registros encontrados)", record_count);
	}
	puts(output);

	strncpy(result, output, max_len - 1);
	result[max_len - 1] = '\0';
	puts(result);

	pthread_mutex_unlock(&db->db_mutex);

	return record_count;
}

// Operación INSERT
int db_insert(database_t *db, char values[][MAX_FIELD_SIZE], int value_count)
{
	printf("Insertando...\n");
	printf("Cantidad de valores: %d, %d campos esperados\n", value_count,
	       db->num_fields);

	if (!db || value_count != db->num_fields) {
		return -1;
	}

	pthread_mutex_lock(&db->db_mutex);

	printf("Insertando registro con %d valores\n", value_count);
	printf("%d registros actuales\n", db->num_records);
	if (db->num_records >= MAX_RECORDS) {
		pthread_mutex_unlock(&db->db_mutex);
		return -1;
	}
	// Buscar slot libre o usar el siguiente
	int slot = db->num_records;

	int next_id = get_next_id(db);
	// Asignar ID automáticamente si el primer campo es 'id'
	if (strcmp(db->headers[0], "id") == 0) {
		int next_id = get_next_id(db);
		snprintf(values[0], MAX_FIELD_SIZE, "%d", next_id);
	}

	// Copiar valores
	for (int i = 0; i < value_count; i++) {
		strncpy(db->records[slot].fields[i], values[i],
			MAX_FIELD_SIZE - 1);
		db->records[slot].fields[i][MAX_FIELD_SIZE - 1] = '\0';
	}

	db->records[slot].id = atoi(db->records[slot].fields[0]);
	db->records[slot].is_deleted = 0;
	db->num_records++;

	pthread_mutex_unlock(&db->db_mutex);

	return 1;
}

// Operación UPDATE
int db_update(database_t *db, const char *set_clause, const char *conditions)
{
	if (!db || !set_clause)
		return -1;

	pthread_mutex_lock(&db->db_mutex);

	// Parsear cláusula SET
	char fields[10][MAX_FIELD_SIZE];
	char values[10][MAX_FIELD_SIZE];
	int set_count = 0;

	if (parse_set_clause(set_clause, fields, values, &set_count) < 0) {
		pthread_mutex_unlock(&db->db_mutex);
		return -1;
	}

	int updated_count = 0;

	// Buscar y actualizar registros
	for (int i = 0; i < db->num_records; i++) {
		if (db->records[i].is_deleted)
			continue;

		// Evaluar condición
		if (conditions && strlen(conditions) > 0) {
			if (!evaluate_condition(&db->records[i], db,
						conditions)) {
				continue;
			}
		}

		// Actualizar campos
		for (int j = 0; j < set_count; j++) {
			int field_index = get_field_index(db, fields[j]);
			if (field_index >= 0) {
				strncpy(db->records[i].fields[field_index],
					values[j], MAX_FIELD_SIZE - 1);
				db->records[i].fields[field_index]
						     [MAX_FIELD_SIZE - 1] =
					'\0';
			}
		}

		updated_count++;
	}

	pthread_mutex_unlock(&db->db_mutex);

	return updated_count;
}

// Operación DELETE
int db_delete(database_t *db, const char *conditions)
{
	if (!db)
		return -1;

	pthread_mutex_lock(&db->db_mutex);

	int deleted_count = 0;

	for (int i = 0; i < db->num_records; i++) {
		if (db->records[i].is_deleted)
			continue;

		// Evaluar condición
		if (conditions && strlen(conditions) > 0) {
			if (!evaluate_condition(&db->records[i], db,
						conditions)) {
				continue;
			}
		}

		// Marcar como eliminado
		db->records[i].is_deleted = 1;
		deleted_count++;
	}

	pthread_mutex_unlock(&db->db_mutex);

	return deleted_count;
}

// Operación DESCRIBE
int db_describe(database_t *db, char *result, int max_len)
{
	if (!db || !result)
		return -1;

	pthread_mutex_lock(&db->db_mutex);

	char output[MAX_BUFFER_SIZE];
	int offset = 0;

	offset += snprintf(output + offset, sizeof(output) - offset,
			   "Estructura de la tabla:\n\n");

	offset += snprintf(output + offset, sizeof(output) - offset,
			   "%-20s | %-10s | %-30s\n", "Campo", "Tipo",
			   "Descripción");

	offset += snprintf(
		output + offset, sizeof(output) - offset,
		"---------------------+------------+--------------------------------\n");

	for (int i = 0; i < db->num_fields; i++) {
		offset += snprintf(output + offset, sizeof(output) - offset,
				   "%-20s | %-10s | %-30s\n", db->headers[i],
				   "VARCHAR", "Campo de texto");
	}

	offset += snprintf(output + offset, sizeof(output) - offset,
			   "\nTotal de campos: %d\n", db->num_fields);
	offset += snprintf(output + offset, sizeof(output) - offset,
			   "Registros activos: %d\n",
			   db->num_records - count_deleted_records(db));

	strncpy(result, output, max_len - 1);
	result[max_len - 1] = '\0';

	pthread_mutex_unlock(&db->db_mutex);

	return 0;
}

// Funciones de transacciones
int begin_transaction(database_t *db, int client_id, pthread_t thread_id)
{
	if (!db)
		return -1;

	pthread_mutex_lock(&db->db_mutex);

	if (db->transaction_active) {
		pthread_mutex_unlock(&db->db_mutex);
		return -1;
	}

	db->transaction_active = 1;
	db->transaction_client_id = client_id;
	db->transaction_thread_id = thread_id;

	pthread_mutex_unlock(&db->db_mutex);

	return 0;
}

int commit_transaction(database_t *db, int client_id)
{
	if (!db)
		return -1;

	pthread_mutex_lock(&db->db_mutex);

	if (!db->transaction_active || db->transaction_client_id != client_id) {
		pthread_mutex_unlock(&db->db_mutex);
		return -1;
	}

	// Los cambios ya están en memoria, solo liberamos el lock
	db->transaction_active = 0;
	db->transaction_client_id = -1;

	pthread_mutex_unlock(&db->db_mutex);

	return 0;
}

int rollback_transaction(database_t *db, int client_id)
{
	if (!db)
		return -1;

	pthread_mutex_lock(&db->db_mutex);

	if (!db->transaction_active || db->transaction_client_id != client_id) {
		pthread_mutex_unlock(&db->db_mutex);
		return -1;
	}

	// Para un rollback completo, deberíamos restaurar desde backup
	// Por simplicidad, solo liberamos el lock
	db->transaction_active = 0;
	db->transaction_client_id = -1;

	pthread_mutex_unlock(&db->db_mutex);

	return 0;
}

int is_transaction_active(database_t *db)
{
	if (!db)
		return 0;
	return db->transaction_active;
}

int is_client_in_transaction(database_t *db, int client_id)
{
	if (!db)
		return 0;
	return db->transaction_active && db->transaction_client_id == client_id;
}

// Funciones utilitarias
int get_field_index(database_t *db, const char *field_name)
{
	if (!db || !field_name)
		return -1;

	for (int i = 0; i < db->num_fields; i++) {
		puts(db->headers[i]);
		puts(field_name);
		if (strcasecmp(db->headers[i], field_name) == 0) {
			puts("Campo encontrado");
			return i;
		}
	}

	return -1;
}

int get_next_id(database_t *db)
{
	if (!db)
		return 1;

	int max_id = 0;
	for (int i = 0; i < db->num_records; i++) {
		if (!db->records[i].is_deleted && db->records[i].id > max_id) {
			max_id = db->records[i].id;
		}
	}

	return max_id + 1;
}

// Función para evaluar condiciones
int evaluate_condition(db_record_t *record, database_t *db,
		       const char *condition)
{
	if (!record || !db || !condition)
		return 1;

	condition_t cond;
	if (parse_condition(condition, &cond) < 0) {
		return 1; // Si no puede parsear, incluye el registro
	}

	int field_index = get_field_index(db, cond.field);
	if (field_index < 0) {
		return 0;
	}

	const char *field_value = record->fields[field_index];

	return compare_field_value(field_value, cond.operator, cond.value);
}

// Función para comparar valores de campo
int compare_field_value(const char *field_value, const char *operator,
			const char *compare_value)
{
	if (!field_value || !operator || !compare_value)
		return 0;

	if (strcmp(operator, "=") == 0 || strcmp(operator, "==") == 0) {
		return strcasecmp(field_value, compare_value) == 0;
	} else if (strcmp(operator, "!=") == 0 || strcmp(operator, "<>") == 0) {
		return strcasecmp(field_value, compare_value) != 0;
	} else if (strcmp(operator, ">") == 0) {
		return atoi(field_value) > atoi(compare_value);
	} else if (strcmp(operator, "<") == 0) {
		return atoi(field_value) < atoi(compare_value);
	} else if (strcmp(operator, ">=") == 0) {
		return atoi(field_value) >= atoi(compare_value);
	} else if (strcmp(operator, "<=") == 0) {
		return atoi(field_value) <= atoi(compare_value);
	} else if (strcmp(operator, "LIKE") == 0) {
		return strstr(field_value, compare_value) != NULL;
	}

	return 0;
}

// Función para parsear condición
int parse_condition(const char *condition_str, condition_t *condition)
{
	if (!condition_str || !condition)
		return -1;

	char cond_copy[MAX_BUFFER_SIZE];
	strncpy(cond_copy, condition_str, MAX_BUFFER_SIZE - 1);
	cond_copy[MAX_BUFFER_SIZE - 1] = '\0';

	// Buscar operadores
	char *operators[] = { ">=", "<=", "!=", "<>", "=", ">", "<", "LIKE" };
	int num_operators = sizeof(operators) / sizeof(operators[0]);

	for (int i = 0; i < num_operators; i++) {
		char *op_pos = strstr(cond_copy, operators[i]);
		if (op_pos) {
			// Extraer campo (antes del operador)
			int field_len = op_pos - cond_copy;
			strncpy(condition->field, cond_copy, field_len);
			condition->field[field_len] = '\0';
			trim_whitespace(condition->field);

			// Copiar operador
			strcpy(condition->operator, operators[i]);

			// Extraer valor (después del operador)
			char *value_start = op_pos + strlen(operators[i]);
			strcpy(condition->value, value_start);
			trim_whitespace(condition->value);

			// Remover comillas del valor
			if ((condition->value[0] == '"' ||
			     condition->value[0] == '\'') &&
			    strlen(condition->value) > 1) {
				int len = strlen(condition->value);
				if (condition->value[len - 1] ==
				    condition->value[0]) {
					condition->value[len - 1] = '\0';
					memmove(condition->value,
						condition->value + 1, len - 1);
				}
			}

			return 0;
		}
	}

	return -1;
}

// Función para parsear cláusula SET
int parse_set_clause(const char *set_str, char fields[][MAX_FIELD_SIZE],
		     char values[][MAX_FIELD_SIZE], int *count)
{
	if (!set_str || !fields || !values || !count)
		return -1;

	char set_copy[MAX_BUFFER_SIZE];
	strncpy(set_copy, set_str, MAX_BUFFER_SIZE - 1);
	set_copy[MAX_BUFFER_SIZE - 1] = '\0';

	*count = 0;

	// Dividir por comas para múltiples asignaciones
	char *assignment = strtok(set_copy, ",");

	while (assignment && *count < 10) {
		trim_whitespace(assignment);

		// Buscar el signo igual
		char *equals_pos = strchr(assignment, '=');
		if (!equals_pos) {
			continue;
		}

		// Extraer campo
		int field_len = equals_pos - assignment;
		strncpy(fields[*count], assignment, field_len);
		fields[*count][field_len] = '\0';
		trim_whitespace(fields[*count]);

		// Extraer valor
		char *value_start = equals_pos + 1;
		strcpy(values[*count], value_start);
		trim_whitespace(values[*count]);

		// Remover comillas del valor
		if ((values[*count][0] == '"' || values[*count][0] == '\'') &&
		    strlen(values[*count]) > 1) {
			int len = strlen(values[*count]);
			if (values[*count][len - 1] == values[*count][0]) {
				values[*count][len - 1] = '\0';
				memmove(values[*count], values[*count] + 1,
					len - 1);
			}
		}

		(*count)++;
		assignment = strtok(NULL, ",");
	}

	return *count > 0 ? 0 : -1;
}

// Función para contar registros eliminados
int count_deleted_records(database_t *db)
{
	if (!db)
		return 0;

	int count = 0;
	for (int i = 0; i < db->num_records; i++) {
		if (db->records[i].is_deleted) {
			count++;
		}
	}

	return count;
}

// Función para crear backup
int create_backup(database_t *db, const char *backup_filename)
{
	if (!db || !backup_filename)
		return -1;

	return save_database_to_csv(db, backup_filename);
}

// Función para obtener estadísticas
int get_database_stats(database_t *db, db_stats_t *stats)
{
	if (!db || !stats)
		return -1;

	pthread_mutex_lock(&db->db_mutex);

	memset(stats, 0, sizeof(db_stats_t));
	stats->total_records = db->num_records;

	for (int i = 0; i < db->num_records; i++) {
		if (db->records[i].is_deleted) {
			stats->deleted_records++;
		} else {
			stats->active_records++;
		}
	}

	pthread_mutex_unlock(&db->db_mutex);

	return 0;
}

// Función para imprimir información de la base de datos
void print_database_info(database_t *db)
{
	if (!db)
		return;

	db_stats_t stats;
	get_database_stats(db, &stats);

	printf("=== Información de la Base de Datos ===\n");
	printf("Campos: %d\n", db->num_fields);
	printf("Registros totales: %d\n", stats.total_records);
	printf("Registros activos: %d\n", stats.active_records);
	printf("Registros eliminados: %d\n", stats.deleted_records);
	printf("Transacción activa: %s\n",
	       db->transaction_active ? "Sí" : "No");
	if (db->transaction_active) {
		printf("Cliente en transacción: %d\n",
		       db->transaction_client_id);
	}
	printf("=======================================\n");
}
