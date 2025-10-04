#ifndef DATABASE_H
#define DATABASE_H

#include "microdb.h"

// Funciones de inicialización y limpieza
database_t *init_database(void);
int load_database_from_csv(database_t *db, const char *filename);
int save_database_to_csv(database_t *db, const char *filename);
void cleanup_database(database_t *db);

// Operaciones CRUD
int db_select(database_t *db, const char *conditions, const char *fields,
	      char *result, int max_len);
int db_insert(database_t *db, char values[][MAX_FIELD_SIZE], int value_count);
int db_update(database_t *db, const char *set_clause, const char *conditions);
int db_delete(database_t *db, const char *conditions);
int db_describe(database_t *db, char *result, int max_len);

// Funciones de búsqueda y filtrado
int find_records_by_condition(database_t *db, const char *conditions,
			      int *matching_indices, int max_matches);
int evaluate_condition(db_record_t *record, database_t *db,
		       const char *condition);
int compare_field_value(const char *field_value, const char *operator,
			const char * compare_value);

// Funciones de transacciones
int begin_transaction(database_t *db, int client_id, pthread_t thread_id);
int commit_transaction(database_t *db, int client_id);
int rollback_transaction(database_t *db, int client_id);
int is_transaction_active(database_t *db);
int is_client_in_transaction(database_t *db, int client_id);

// Funciones de bloqueo
int acquire_db_lock(database_t *db, int client_id);
int release_db_lock(database_t *db, int client_id);
int check_db_lock(database_t *db, int client_id);

// Funciones utilitarias
int get_field_index(database_t *db, const char *field_name);
int get_next_id(database_t *db);
int validate_record_data(database_t *db, char values[][MAX_FIELD_SIZE],
			 int value_count);
int count_deleted_records(database_t *db);
void trim_whitespace(char *str);

// Funciones de parsing de condiciones
typedef struct {
	char field[MAX_FIELD_SIZE];
	char operator[10];
	char value[MAX_FIELD_SIZE];
} condition_t;

int parse_condition(const char *condition_str, condition_t *condition);
int parse_set_clause(const char *set_str, char fields[][MAX_FIELD_SIZE],
		     char values[][MAX_FIELD_SIZE], int *count);

// Funciones de backup y recovery
int create_backup(database_t *db, const char *backup_filename);
int restore_from_backup(database_t *db, const char *backup_filename);

// Estadísticas y monitoreo
typedef struct {
	int total_records;
	int active_records;
	int deleted_records;
	int select_count;
	int insert_count;
	int update_count;
	int delete_count;
	int transaction_count;
} db_stats_t;

int get_database_stats(database_t *db, db_stats_t *stats);
void print_database_info(database_t *db);

#endif // DATABASE_H
