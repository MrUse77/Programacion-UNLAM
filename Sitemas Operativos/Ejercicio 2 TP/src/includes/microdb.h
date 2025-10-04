#ifndef MICRODB_H
#define MICRODB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

// Constantes
#define MAX_BUFFER_SIZE 4096
#define MAX_FIELD_SIZE 256
#define MAX_FIELDS 10
#define MAX_RECORDS 1000
#define CONFIG_FILE "config.txt"
#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 8080
#define DEFAULT_MAX_CLIENTS 5
#define DEFAULT_QUEUE_SIZE 10

// Estados de transacción
typedef enum {
	TRANSACTION_NONE,
	TRANSACTION_ACTIVE,
	TRANSACTION_COMMITTED,
	TRANSACTION_ABORTED
} transaction_state_t;

// Estructura de configuración del servidor
typedef struct {
	char host[256];
	int port;
	int max_clients;
	int queue_size;
	char database_file[256];
	int log_level;
} server_config_t;

// Estructura de un registro de la base de datos
typedef struct {
	int id;
	char fields[MAX_FIELDS][MAX_FIELD_SIZE];
	int is_deleted;
} db_record_t;

// Estructura de la base de datos
typedef struct {
	db_record_t records[MAX_RECORDS];
	char headers[MAX_FIELDS][MAX_FIELD_SIZE];
	int num_records;
	int num_fields;
	pthread_mutex_t db_mutex;
	int transaction_active;
	int transaction_client_id;
	pthread_t transaction_thread_id;
} database_t;

// Estructura del cliente conectado
typedef struct {
	int socket_fd;
	int client_id;
	struct sockaddr_in address;
	pthread_t thread_id;
	transaction_state_t transaction_state;
	time_t connect_time;
	int is_active;
} client_info_t;

// Estructura del servidor
typedef struct {
	int server_socket;
	server_config_t config;
	database_t *db;
	client_info_t clients[50];
	int active_clients;
	pthread_mutex_t clients_mutex;
	int should_stop;
} server_t;

// Variables globales
extern server_t *global_server;
extern database_t *global_database;

// Funciones de utilidad
void log_message(const char *level, const char *format, ...);
void cleanup_resources(void);
void signal_handler(int sig);

#endif // MICRODB_H
