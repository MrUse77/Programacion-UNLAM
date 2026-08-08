#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "microdb.h"

// Tipos de comandos del protocolo
typedef enum {
	CMD_SELECT = 1,
	CMD_INSERT,
	CMD_UPDATE,
	CMD_DELETE,
	CMD_BEGIN_TRANSACTION,
	CMD_COMMIT_TRANSACTION,
	CMD_ROLLBACK_TRANSACTION,
	CMD_DESCRIBE,
	CMD_HELP,
	CMD_QUIT,
	CMD_PING
} command_type_t;

// Códigos de respuesta
typedef enum {
	RESP_OK = 200,
	RESP_ERROR = 400,
	RESP_NOT_FOUND = 404,
	RESP_CONFLICT = 409,
	RESP_LOCKED = 423,
	RESP_SERVER_ERROR = 500
} response_code_t;

// Estructura del mensaje de comando
typedef struct {
	command_type_t type;
	char query[MAX_BUFFER_SIZE];
	char params[10][MAX_FIELD_SIZE];
	int param_count;
	int client_id;
} command_message_t;

// Estructura de la respuesta
typedef struct {
	response_code_t code;
	char message[MAX_BUFFER_SIZE];
	char data[MAX_BUFFER_SIZE];
	int row_count;
} response_message_t;
typedef struct {
	int id;
	char name[50];
	int age;
	char city[50];
	char department[50];
	int salary;
	int experience;
} Record;

// Funciones del protocolo
int send_command(int socket_fd, command_message_t *cmd);
int receive_command(int socket_fd, command_message_t *cmd);
int send_response(int socket_fd, response_message_t *resp);
int receive_response(int socket_fd, response_message_t *resp);

// Funciones de parsing
int parse_select_query(const char *query, char *table, char *conditions,
		       char *fields);
int parse_insert_query(const char *query, char *table,
		       char values[][MAX_FIELD_SIZE], int *value_count);
int parse_update_query(const char *query, char *table, char *set_clause,
		       char *conditions);
int parse_delete_query(const char *query, char *table, char *conditions);

// Funciones de validación
int validate_command(command_message_t *cmd);
int validate_field_name(const char *field_name, database_t *db);
int validate_field_value(const char *field_value, int field_index,
			 database_t *db);

// Utilidades del protocolo
void format_record_output(db_record_t *record, database_t *db, char *output,
			  int max_len);
void format_table_header(database_t *db, char *output, int max_len);
const char *get_command_name(command_type_t type);
const char *get_response_message(response_code_t code);

#endif // PROTOCOL_H
