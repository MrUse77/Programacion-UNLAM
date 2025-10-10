#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 4096
#define MAX_LINE 2048
#define DB_FILE "database.csv"
#define TEMP_FILE "database.tmp"

// Estructura para datos del cliente
typedef struct {
	int socket;
	int client_id;
	struct sockaddr_in address;
} client_data_t;

// Variables globales
static int server_socket = -1;
static int max_clients = 5;
static int max_queue = 10;
static int client_count = 0;
static int transaction_owner = -1; // ID del cliente con transacción activa
static pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
static volatile sig_atomic_t server_running = 1;

// Prototipos
void cleanup_resources();
void signal_handler(int sig);
void* handle_client(void* arg);
int process_command(int client_id, char* command, char* response);
int execute_query(char* command, char* response);
int execute_insert(char* command, char* response);
int execute_update(char* command, char* response);
int execute_delete(char* command, char* response);
void print_usage(const char* program);

// Limpieza de recursos
void cleanup_resources()
{
	printf("\nCerrando servidor...\n");
	server_running = 0;

	if (server_socket != -1) {
		close(server_socket);
		server_socket = -1;
	}

	pthread_mutex_destroy(&db_mutex);
	pthread_mutex_destroy(&client_mutex);

	// Eliminar archivos temporales
	remove(TEMP_FILE);

	printf("Recursos liberados correctamente.\n");
}

// Manejador de señales
void signal_handler(int sig)
{
	if (sig == SIGINT || sig == SIGTERM) {
		printf("\nSeñal recibida, cerrando servidor...\n");
		cleanup_resources();
		exit(0);
	}
}

// Mostrar ayuda
void print_usage(const char* program)
{
	printf("Uso: %s -p <puerto> [-n <max_clientes>] [-m <max_cola>]\n",
				 program);
	printf("Opciones:\n");
	printf("  -p <puerto>         Puerto de escucha (requerido)\n");
	printf("  -n <max_clientes>   Máximo de clientes concurrentes (default: 5)\n");
	printf("  -m <max_cola>       Máximo de clientes en espera (default: 10)\n");
	printf("  -h                  Mostrar esta ayuda\n");
}

// Ejecutar consulta SELECT
int execute_query(char* command, char* response)
{
	FILE* file = fopen(DB_FILE, "r");
	if (!file) {
		snprintf(response, BUFFER_SIZE,
						 "ERROR: No se pudo abrir la base de datos\n");
		return -1;
	}

	char line[MAX_LINE];
	char* result = malloc(BUFFER_SIZE);
	int count = 0;
	int first_line = 1;

	// Parsear comando SELECT
	// Formato: SELECT [campo] [valor] o SELECT ALL
	char* token = strtok(command, " ");
	token = strtok(NULL, " "); // siguiente token después de SELECT

	if (token && strcmp(token, "ALL") == 0) {
		// SELECT ALL - devolver todos los registros
		size_t result_capacity = BUFFER_SIZE;
		while (fgets(line, sizeof(line), file)) {
			size_t needed_size = strlen(result) + strlen(line) + 1;

			// Si no hay espacio suficiente, hacer realloc
			if (needed_size > result_capacity) {
				size_t new_capacity = result_capacity * 2;
				while (new_capacity < needed_size) {
					new_capacity *= 2;
				}

				char* new_result = realloc(result, new_capacity);
				if (!new_result) {
					free(result);
					fclose(file);
					snprintf(response, BUFFER_SIZE,
									 "ERROR: Memoria insuficiente\n");
					return -1;
				}
				result = new_result;
				result_capacity = new_capacity;
			}

			strcat(result, line);
			count++;
		}
	}
	else if (token) {
		// SELECT campo valor
		char field[64], value[64];
		strncpy(field, token, sizeof(field) - 1);
		token = strtok(NULL, " ");
		if (token) {
			strncpy(value, token, sizeof(value) - 1);

			// Leer encabezado
			if (fgets(line, sizeof(line), file)) {
				strncat(result, line,
								BUFFER_SIZE - strlen(result) - 1);

				// Buscar índice del campo
				int field_index = -1;
				char* header = strdup(line);
				char* h_token = strtok(header, ",");
				int idx = 0;

				while (h_token) {
					// Eliminar espacios y saltos de línea
					char clean_token[64];
					sscanf(h_token, "%s", clean_token);
					if (strcmp(clean_token, field) == 0) {
						field_index = idx;
						break;
					}
					h_token = strtok(NULL, ",");
					idx++;
				}
				free(header);

				// Filtrar registros
				if (field_index >= 0) {
					while (fgets(line, sizeof(line),
											 file)) {
						char* line_copy = strdup(line);
						char* l_token =
							strtok(line_copy, ",");
						idx = 0;
						int match = 0;

						while (l_token &&
									 idx <= field_index) {
							if (idx ==
									field_index) {
								char clean_value
									[64];
								sscanf(l_token,
											 "%s",
											 clean_value);
								if (strcmp(clean_value,
													 value) ==
										0) {
									match = 1;
								}
								break;
							}
							l_token = strtok(NULL,
															 ",");
							idx++;
						}
						free(line_copy);

						if (match) {
							strncat(result, line,
											BUFFER_SIZE -
											strlen(result) -
											1);
							count++;
						}
					}
				}
			}
		}
	}

	fclose(file);

	if (strlen(result) > 0) {
		size_t result_len = strlen(result);
		size_t prefix_len = strlen("OK\n");
		size_t suffix_len = snprintf(NULL, 0, "\nRegistros encontrados: %d\n", count);
		size_t total_needed = prefix_len + result_len + suffix_len + 1;


		// Realloc response to fit the entire result
		char* new_response = realloc(response, total_needed);
		if (!new_response) {
			free(result);
			snprintf(response, BUFFER_SIZE,
							 "ERROR: Memoria insuficiente\n");
			return -1;
		}
		response = new_response;
		snprintf(response, total_needed,
						 "OK\n%s\nRegistros encontrados: %d\n", result, count);

	}
	else {
		snprintf(response, BUFFER_SIZE,
						 "OK\nNo se encontraron registros\n");
	}
	printf("Respuesta del servidor: %s\n", response);

	free(result);
	return 0;
}

// Ejecutar INSERT
int execute_insert(char* command, char* response)
{
	// Formato: INSERT id,name,age,city,department,salary,experience
	char* data = strchr(command, ' ');
	if (!data) {
		snprintf(
			response, BUFFER_SIZE,
			"ERROR: Formato incorrecto. Uso: INSERT campo1,campo2,...\n");
		return -1;
	}
	data++; // Saltar el espacio

	FILE* file = fopen(DB_FILE, "a");
	if (!file) {
		snprintf(response, BUFFER_SIZE,
						 "ERROR: No se pudo abrir la base de datos\n");
		return -1;
	}

	fprintf(file, "%s\n", data);
	fclose(file);

	snprintf(response, BUFFER_SIZE,
					 "OK\nRegistro insertado correctamente\n");
	return 0;
}

// Ejecutar UPDATE
int execute_update(char* command, char* response)
{
	// Formato: UPDATE ID campo nuevo_valor
	char id[64], field[64], value[256];

	if (sscanf(command, "UPDATE %s %s %[^\n]", id, field, value) != 3) {
		snprintf(
			response, BUFFER_SIZE,
			"ERROR: Formato incorrecto. Uso: UPDATE ID campo nuevo_valor\n");
		return -1;
	}

	FILE* file = fopen(DB_FILE, "r");
	FILE* temp = fopen(TEMP_FILE, "w");

	if (!file || !temp) {
		if (file)
			fclose(file);
		if (temp)
			fclose(temp);
		snprintf(response, BUFFER_SIZE,
						 "ERROR: No se pudo abrir la base de datos\n");
		return -1;
	}

	char line[MAX_LINE];
	int updated = 0;
	int field_index = -1;

	// Copiar encabezado y encontrar índice del campo
	if (fgets(line, sizeof(line), file)) {
		fprintf(temp, "%s", line);

		char* header = strdup(line);
		char* token = strtok(header, ",");
		int idx = 0;

		while (token) {
			char clean_token[64];
			sscanf(token, "%s", clean_token);
			if (strcmp(clean_token, field) == 0) {
				field_index = idx;
				break;
			}
			token = strtok(NULL, ",");
			idx++;
		}
		free(header);
	}

	// Procesar registros
	while (fgets(line, sizeof(line), file)) {
		char line_copy[MAX_LINE];
		strncpy(line_copy, line, sizeof(line_copy));

		char* token = strtok(line_copy, ",");
		if (token && strcmp(token, id) == 0 && field_index >= 0) {
			// Reconstruir línea con el nuevo valor
			char new_line[MAX_LINE] = "";
			char* original_token = strtok(line, ",");
			int idx = 0;

			while (original_token) {
				if (idx > 0)
					strcat(new_line, ",");

				if (idx == field_index) {
					strcat(new_line, value);
				}
				else {
					// Eliminar salto de línea si es el último campo
					char clean_token[256];
					int len = strlen(original_token);
					if (original_token[len - 1] == '\n') {
						strncpy(clean_token,
										original_token,
										len - 1);
						clean_token[len - 1] = '\0';
					}
					else {
						strcpy(clean_token,
									 original_token);
					}
					strcat(new_line, clean_token);
				}

				original_token = strtok(NULL, ",");
				idx++;
			}

			fprintf(temp, "%s\n", new_line);
			updated = 1;
		}
		else {
			fprintf(temp, "%s", line);
		}
	}

	fclose(file);
	fclose(temp);

	if (updated) {
		remove(DB_FILE);
		rename(TEMP_FILE, DB_FILE);
		snprintf(response, BUFFER_SIZE,
						 "OK\nRegistro actualizado correctamente\n");
	}
	else {
		remove(TEMP_FILE);
		snprintf(response, BUFFER_SIZE,
						 "ERROR: No se encontró el registro con ID %s\n", id);
	}

	return updated ? 0 : -1;
}

// Ejecutar DELETE
int execute_delete(char* command, char* response)
{
	// Formato: DELETE ID
	char id[64];

	if (sscanf(command, "DELETE %s", id) != 1) {
		snprintf(response, BUFFER_SIZE,
						 "ERROR: Formato incorrecto. Uso: DELETE ID\n");
		return -1;
	}

	FILE* file = fopen(DB_FILE, "r");
	FILE* temp = fopen(TEMP_FILE, "w");

	if (!file || !temp) {
		if (file)
			fclose(file);
		if (temp)
			fclose(temp);
		snprintf(response, BUFFER_SIZE,
						 "ERROR: No se pudo abrir la base de datos\n");
		return -1;
	}

	char line[MAX_LINE];
	int deleted = 0;

	// Copiar encabezado
	if (fgets(line, sizeof(line), file)) {
		fprintf(temp, "%s", line);
	}

	// Procesar registros
	while (fgets(line, sizeof(line), file)) {
		char line_copy[MAX_LINE];
		strncpy(line_copy, line, sizeof(line_copy));

		char* token = strtok(line_copy, ",");
		if (token && strcmp(token, id) == 0) {
			deleted = 1;
			continue; // Saltar este registro
		}
		fprintf(temp, "%s", line);
	}

	fclose(file);
	fclose(temp);

	if (deleted) {
		remove(DB_FILE);
		rename(TEMP_FILE, DB_FILE);
		snprintf(response, BUFFER_SIZE,
						 "OK\nRegistro eliminado correctamente\n");
	}
	else {
		remove(TEMP_FILE);
		snprintf(response, BUFFER_SIZE,
						 "ERROR: No se encontró el registro con ID %s\n", id);
	}

	return deleted ? 0 : -1;
}

// Procesar comando del cliente
int process_command(int client_id, char* command, char* response)
{
	// Eliminar salto de línea
	command[strcspn(command, "\n")] = 0;

	printf("Cliente %d ejecuta: %s\n", client_id, command);

	// BEGIN TRANSACTION
	if (strcmp(command, "BEGIN TRANSACTION") == 0) {
		pthread_mutex_lock(&db_mutex);

		if (transaction_owner != -1 && transaction_owner != client_id) {
			pthread_mutex_unlock(&db_mutex);
			snprintf(
				response, BUFFER_SIZE,
				"ERROR: Existe una transacción activa. Reintente más tarde.\n");
			return -1;
		}

		transaction_owner = client_id;
		pthread_mutex_unlock(&db_mutex);

		snprintf(response, BUFFER_SIZE, "OK\nTransacción iniciada\n");
		return 0;
	}

	// COMMIT TRANSACTION
	if (strcmp(command, "COMMIT TRANSACTION") == 0) {
		pthread_mutex_lock(&db_mutex);

		if (transaction_owner != client_id) {
			pthread_mutex_unlock(&db_mutex);
			snprintf(response, BUFFER_SIZE,
							 "ERROR: No tiene una transacción activa\n");
			return -1;
		}

		transaction_owner = -1;
		pthread_mutex_unlock(&db_mutex);

		snprintf(response, BUFFER_SIZE, "OK\nTransacción confirmada\n");
		return 0;
	}

	// Verificar si hay transacción activa
	pthread_mutex_lock(&db_mutex);
	if (transaction_owner != -1 && transaction_owner != client_id) {
		pthread_mutex_unlock(&db_mutex);
		snprintf(
			response, BUFFER_SIZE,
			"ERROR: Existe una transacción activa. Reintente más tarde.\n");
		return -1;
	}

	// Si el cliente tiene transacción activa, mantener el lock
	int has_transaction = (transaction_owner == client_id);
	if (!has_transaction) {
		pthread_mutex_unlock(&db_mutex);
	}

	// Procesar otros comandos
	int result = 0;

	if (strncmp(command, "SELECT", 6) == 0) {
		if (!has_transaction)
			pthread_mutex_lock(&db_mutex);
		result = execute_query(command, response);
		if (!has_transaction)
			pthread_mutex_unlock(&db_mutex);
	}
	else if (strncmp(command, "INSERT", 6) == 0) {
		result = execute_insert(command, response);
	}
	else if (strncmp(command, "UPDATE", 6) == 0) {
		result = execute_update(command, response);
	}
	else if (strncmp(command, "DELETE", 6) == 0) {
		result = execute_delete(command, response);
	}
	else if (strcmp(command, "QUIT") == 0) {
		if (has_transaction) {
			transaction_owner = -1;
			pthread_mutex_unlock(&db_mutex);
		}
		snprintf(response, BUFFER_SIZE, "OK\nDesconectando...\n");
		return 1; // Señal de desconexión
	}
	else {
		snprintf(response, BUFFER_SIZE, "ERROR: Comando desconocido\n");
		result = -1;
	}

	if (has_transaction) {
		pthread_mutex_unlock(&db_mutex);
	}

	return result;
}

// Manejar cliente
void* handle_client(void* arg)
{
	client_data_t* client = (client_data_t*)arg;
	char buffer[BUFFER_SIZE];
	char* response = malloc(BUFFER_SIZE);

	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &client->address.sin_addr, client_ip,
						INET_ADDRSTRLEN);

	printf("Cliente %d conectado desde %s:%d\n", client->client_id,
				 client_ip, ntohs(client->address.sin_port));

	// Mensaje de bienvenida
	snprintf(
		response, BUFFER_SIZE,
		"Bienvenido al servidor de base de datos\n"
		"Comandos disponibles:\n"
		"  SELECT ALL\n"
		"  SELECT campo valor\n"
		"  INSERT id,nombre,edad,ciudad,departamento,salario,experiencia\n"
		"  UPDATE id campo nuevo_valor\n"
		"  DELETE id\n"
		"  BEGIN TRANSACTION\n"
		"  COMMIT TRANSACTION\n"
		"  QUIT\n");
	send(client->socket, response, strlen(response), 0);

	// Bucle de comunicación
	while (server_running) {
		memset(buffer, 0, BUFFER_SIZE);
		int bytes_received =
			recv(client->socket, buffer, BUFFER_SIZE - 1, 0);

		if (bytes_received <= 0) {
			if (bytes_received == 0) {
				printf("Cliente %d desconectado\n",
							 client->client_id);
			}
			else {
				perror("Error al recibir datos");
			}
			break;
		}

		buffer[bytes_received] = '\0';

		int cmd_result =
			process_command(client->client_id, buffer, response);
		printf("Respuesta del servidor: %s, %d\n", response, strlen(response));
		send(client->socket, response, strlen(response), 0);

		if (cmd_result == 1) { // QUIT
			break;
		}
	}

	// Liberar transacción si el cliente la tenía
	pthread_mutex_lock(&db_mutex);
	if (transaction_owner == client->client_id) {
		printf("Cliente %d tenía transacción activa, liberando lock\n",
					 client->client_id);
		transaction_owner = -1;
	}
	pthread_mutex_unlock(&db_mutex);

	close(client->socket);

	pthread_mutex_lock(&client_mutex);
	client_count--;
	printf("Clientes activos: %d\n", client_count);
	pthread_mutex_unlock(&client_mutex);
	free(response);
	free(client);
	return NULL;
}

int main(int argc, char* argv[])
{
	int port = -1;
	int opt;

	// Parsear argumentos
	while ((opt = getopt(argc, argv, "p:n:m:h")) != -1) {
		switch (opt) {
		case 'p':
			port = atoi(optarg);
			break;
		case 'n':
			max_clients = atoi(optarg);
			break;
		case 'm':
			max_queue = atoi(optarg);
			break;
		case 'h':
			print_usage(argv[0]);
			return 0;
		default:
			print_usage(argv[0]);
			return 1;
		}
	}

	// Validar parámetros
	if (port < 0 || port > 65535) {
		fprintf(stderr, "Error: Puerto inválido\n");
		print_usage(argv[0]);
		return 1;
	}

	if (max_clients <= 0) {
		fprintf(stderr, "Error: Número de clientes inválido\n");
		return 1;
	}

	// Verificar que existe el archivo de base de datos
	FILE* db = fopen(DB_FILE, "r");
	if (!db) {
		fprintf(stderr, "Error: No se encuentra el archivo %s\n",
						DB_FILE);
		return 1;
	}
	fclose(db);

	// Configurar manejadores de señales
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGPIPE, SIG_IGN); // Ignorar SIGPIPE

	// Crear socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		perror("Error al crear socket");
		return 1;
	}

	// Configurar opción SO_REUSEADDR
	int reuse = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse,
								 sizeof(reuse)) < 0) {
		perror("Error en setsockopt");
		close(server_socket);
		return 1;
	}

	// Configurar dirección
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	// Bind
	if (bind(server_socket, (struct sockaddr*)&server_addr,
					 sizeof(server_addr)) < 0) {
		perror("Error en bind");
		close(server_socket);
		return 1;
	}

	// Listen
	if (listen(server_socket, max_queue) < 0) {
		perror("Error en listen");
		close(server_socket);
		return 1;
	}

	printf("Servidor iniciado en puerto %d\n", port);
	printf("Máximo de clientes concurrentes: %d\n", max_clients);
	printf("Máximo de clientes en espera: %d\n", max_queue);
	printf("Esperando conexiones...\n");

	int next_client_id = 1;

	// Bucle principal
	while (server_running) {
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);

		int client_socket = accept(server_socket,
															 (struct sockaddr*)&client_addr,
															 &client_len);

		if (client_socket < 0) {
			if (errno == EINTR)
				continue;
			perror("Error en accept");
			continue;
		}

		// Verificar límite de clientes
		pthread_mutex_lock(&client_mutex);
		if (client_count >= max_clients) {
			pthread_mutex_unlock(&client_mutex);

			char msg[] =
				"ERROR: Servidor lleno. Intente más tarde.\n";
			send(client_socket, msg, strlen(msg), 0);
			close(client_socket);
			printf("Conexión rechazada: servidor lleno\n");
			continue;
		}

		client_count++;
		pthread_mutex_unlock(&client_mutex);

		// Crear estructura de datos del cliente
		client_data_t* client = malloc(sizeof(client_data_t));
		client->socket = client_socket;
		client->client_id = next_client_id++;
		client->address = client_addr;

		// Crear thread para manejar el cliente
		pthread_t thread;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		if (pthread_create(&thread, &attr, handle_client, client) !=
				0) {
			perror("Error al crear thread");
			close(client_socket);
			free(client);

			pthread_mutex_lock(&client_mutex);
			client_count--;
			pthread_mutex_unlock(&client_mutex);
		}

		pthread_attr_destroy(&attr);
	}

	cleanup_resources();
	return 0;
}
