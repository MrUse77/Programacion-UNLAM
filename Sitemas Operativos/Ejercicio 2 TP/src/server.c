#include "includes/microdb.h"
#include "includes/protocol.h"
#include "includes/database.h"
#include <stdarg.h>

// Variables globales
server_t *global_server = NULL;
database_t *global_database = NULL;

// Función para leer configuración
int load_config(server_config_t *config, const char *config_file)
{
	FILE *file = fopen(config_file, "r");
	if (!file) {
		printf("No se pudo abrir archivo de configuración %s, usando valores por defecto\n",
		       config_file);
		if (config != NULL) {
			log_message(
				"WARN",
				"No se detecto configuración, usando valores por defecto");
			strcpy(config->host, DEFAULT_HOST);
			config->port = DEFAULT_PORT;
			config->max_clients = DEFAULT_MAX_CLIENTS;
			config->queue_size = DEFAULT_QUEUE_SIZE;
			strcpy(config->database_file, "database.csv");
			config->log_level = 1;
		}
		return 0;
	}

	char line[256];
	while (fgets(line, sizeof(line), file)) {
		if (line[0] == '#' || line[0] == '\n')
			continue;

		char key[128], value[128];
		if (sscanf(line, "%[^=]=%s", key, value) == 2) {
			if (strcmp(key, "HOST") == 0) {
				strcpy(config->host, value);
			} else if (strcmp(key, "PORT") == 0) {
				config->port = atoi(value);
			} else if (strcmp(key, "MAX_CLIENTS") == 0) {
				config->max_clients = atoi(value);
			} else if (strcmp(key, "QUEUE_SIZE") == 0) {
				config->queue_size = atoi(value);
			} else if (strcmp(key, "DATABASE_FILE") == 0) {
				strcpy(config->database_file, value);
			} else if (strcmp(key, "LOG_LEVEL") == 0) {
				config->log_level = atoi(value);
			}
		}
	}

	fclose(file);
	log_message("INFO", "Configuración cargada desde %s", config_file);
	return 1;
}

// Función de logging
void log_message(const char *level, const char *format, ...)
{
	time_t now = time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S",
		 localtime(&now));

	printf("[%s] [%s] ", timestamp, level);

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	printf("\n");
	fflush(stdout);
}

// Función para manejar comandos del cliente
void *handle_client(void *arg)
{
	client_info_t *client = (client_info_t *)arg;
	command_message_t cmd;
	response_message_t resp;

	log_message("INFO", "Cliente %d conectado desde %s", client->client_id,
		    inet_ntoa(client->address.sin_addr));

	// Mensaje de bienvenida
	resp.code = RESP_OK;
	strcpy(resp.message, "Conectado al servidor de Micro Base de Datos");
	strcpy(resp.data, "Escriba HELP para ver los comandos disponibles");
	resp.row_count = 0;
	send_response(client->socket_fd, &resp);

	while (client->is_active) {
		memset(&cmd, 0, sizeof(cmd));
		memset(&resp, 0, sizeof(resp));

		// Recibir comando del cliente
		if (receive_command(client->socket_fd, &cmd) <= 0) {
			log_message("INFO", "Cliente %d desconectado",
				    client->client_id);
			break;
		}

		cmd.client_id = client->client_id;
		printf("Comando recibido del cliente %d: %s\n",
		       client->client_id, cmd.query);
		// Procesar comando
		printf("procesando comando...\n");
		printf("transaccion activa: %d\n",
		       is_transaction_active(global_database));
		printf("cliente en transaccion: %d\n",
		       is_client_in_transaction(global_database,
						client->client_id));
		switch (cmd.type) {
		case CMD_SELECT:
			if (is_transaction_active(global_database) &&
			    !is_client_in_transaction(global_database,
						      client->client_id)) {
				resp.code = RESP_LOCKED;
				strcpy(resp.message,
				       "Base de datos bloqueada por transacción activa");
			} else {
				if (db_select(global_database, cmd.params[0],
					      cmd.params[1], resp.data,
					      MAX_BUFFER_SIZE) >= 0) {
					resp.code = RESP_OK;
					strcpy(resp.message,
					       "Consulta ejecutada");
				} else {
					resp.code = RESP_ERROR;
					strcpy(resp.message,
					       "Error en consulta");
				}
			}
			break;

		case CMD_INSERT:
			if (is_transaction_active(global_database) &&
			    !is_client_in_transaction(global_database,
						      client->client_id)) {
				resp.code = RESP_LOCKED;
				strcpy(resp.message,
				       "Base de datos bloqueada por transacción activa");
			} else {
				char values[MAX_FIELDS][MAX_FIELD_SIZE];
				int value_count = cmd.param_count;
				for (int i = 0; i < value_count; i++) {
					strcpy(values[i], cmd.params[i]);
				}

				if (db_insert(global_database, values,
					      value_count) >= 0) {
					resp.code = RESP_OK;
					strcpy(resp.message,
					       "Registro insertado correctamente");
				} else {
					resp.code = RESP_ERROR;
					strcpy(resp.message,
					       "Error al insertar registro");
				}
			}
			break;

		case CMD_UPDATE:
			if (is_transaction_active(global_database) &&
			    !is_client_in_transaction(global_database,
						      client->client_id)) {
				resp.code = RESP_LOCKED;
				strcpy(resp.message,
				       "Base de datos bloqueada por transacción activa");
			} else {
				int updated = db_update(global_database,
							cmd.params[0],
							cmd.params[1]);
				if (updated >= 0) {
					resp.code = RESP_OK;
					sprintf(resp.message,
						"%d registros actualizados",
						updated);
				} else {
					resp.code = RESP_ERROR;
					strcpy(resp.message,
					       "Error en actualización");
				}
			}
			break;

		case CMD_DELETE:
			if (is_transaction_active(global_database) &&
			    !is_client_in_transaction(global_database,
						      client->client_id)) {
				resp.code = RESP_LOCKED;
				strcpy(resp.message,
				       "Base de datos bloqueada por transacción activa");
			} else {
				int deleted = db_delete(global_database,
							cmd.params[0]);
				if (deleted >= 0) {
					resp.code = RESP_OK;
					sprintf(resp.message,
						"%d registros eliminados",
						deleted);
				} else {
					resp.code = RESP_ERROR;
					strcpy(resp.message,
					       "Error en eliminación");
				}
			}
			break;

		case CMD_BEGIN_TRANSACTION:
			if (begin_transaction(global_database,
					      client->client_id,
					      client->thread_id) == 0) {
				client->transaction_state = TRANSACTION_ACTIVE;
				resp.code = RESP_OK;
				strcpy(resp.message, "Transacción iniciada");
			} else {
				resp.code = RESP_CONFLICT;
				strcpy(resp.message,
				       "Ya existe una transacción activa");
			}
			break;

		case CMD_COMMIT_TRANSACTION:
			if (commit_transaction(global_database,
					       client->client_id) == 0) {
				client->transaction_state =
					TRANSACTION_COMMITTED;
				resp.code = RESP_OK;
				strcpy(resp.message, "Transacción confirmada");
				save_database_to_csv(
					global_database,
					global_server->config.database_file);
			} else {
				resp.code = RESP_ERROR;
				strcpy(resp.message,
				       "Error al confirmar transacción");
			}
			break;

		case CMD_DESCRIBE:
			if (db_describe(global_database, resp.data,
					MAX_BUFFER_SIZE) >= 0) {
				resp.code = RESP_OK;
				strcpy(resp.message, "Estructura de la tabla");
			} else {
				resp.code = RESP_ERROR;
				strcpy(resp.message,
				       "Error al obtener estructura");
			}
			break;

		case CMD_HELP:
			resp.code = RESP_OK;
			strcpy(resp.message, "Comandos disponibles");
			strcpy(resp.data,
			       "SELECT [campos] WHERE [condición] - Consultar registros\n"
			       "INSERT VALUES (val1,val2,...) - Insertar registro\n"
			       "UPDATE SET campo=valor WHERE [condición] - Actualizar registros\n"
			       "DELETE WHERE [condición] - Eliminar registros\n"
			       "BEGIN TRANSACTION - Iniciar transacción\n"
			       "COMMIT TRANSACTION - Confirmar transacción\n"
			       "DESCRIBE - Mostrar estructura de tabla\n"
			       "HELP - Mostrar esta ayuda\n"
			       "QUIT - Desconectar del servidor\n");
			break;

		case CMD_QUIT:
			resp.code = RESP_OK;
			strcpy(resp.message, "Desconectando...");
			send_response(client->socket_fd, &resp);
			client->is_active = 0;
			continue;

		default:
			resp.code = RESP_ERROR;
			strcpy(resp.message, "Comando no reconocido");
			break;
		}

		send_response(client->socket_fd, &resp);
	}

	// Limpiar transacción si estaba activa
	if (client->transaction_state == TRANSACTION_ACTIVE) {
		rollback_transaction(global_database, client->client_id);
		log_message(
			"INFO",
			"Transacción del cliente %d cancelada por desconexión",
			client->client_id);
	}

	// Cerrar socket y marcar cliente como inactivo
	close(client->socket_fd);
	client->is_active = 0;

	pthread_mutex_lock(&global_server->clients_mutex);
	global_server->active_clients--;
	pthread_mutex_unlock(&global_server->clients_mutex);

	log_message("INFO", "Cliente %d desconectado completamente",
		    client->client_id);
	return NULL;
}

// Manejador de señales
void signal_handler(int sig)
{
	log_message("INFO", "Recibida señal %d, cerrando servidor...", sig);
	if (global_server) {
		global_server->should_stop = 1;
	}
	cleanup_resources();
	exit(0);
}

// Función principal del servidor
int start_server(server_config_t *config)
{
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_id = 1;

	// Crear socket del servidor
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		log_message("ERROR", "No se pudo crear socket del servidor");
		return -1;
	}

	// Configurar reutilización de dirección
	int opt = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt,
		       sizeof(opt)) < 0) {
		log_message("WARN", "No se pudo configurar SO_REUSEADDR");
	}

	// Configurar dirección del servidor
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(config->host);
	server_addr.sin_port = htons(config->port);

	// Bind del socket
	if (bind(server_socket, (struct sockaddr *)&server_addr,
		 sizeof(server_addr)) < 0) {
		log_message("ERROR", "No se pudo hacer bind en %s:%d",
			    config->host, config->port);
		close(server_socket);
		return -1;
	}

	// Escuchar conexiones
	if (listen(server_socket, config->queue_size) < 0) {
		log_message("ERROR", "Error al escuchar en socket");
		close(server_socket);
		return -1;
	}

	log_message("INFO", "Servidor iniciado en %s:%d", config->host,
		    config->port);
	log_message("INFO", "Máximo %d clientes concurrentes, cola de %d",
		    config->max_clients, config->queue_size);

	global_server->server_socket = server_socket;

	// Bucle principal del servidor
	while (!global_server->should_stop) {
		client_socket = accept(server_socket,
				       (struct sockaddr *)&client_addr,
				       &client_len);
		if (client_socket < 0) {
			if (errno == EINTR)
				continue; // Interrumpido por señal
			log_message("ERROR", "Error al aceptar conexión");
			continue;
		}

		// Verificar límite de clientes
		pthread_mutex_lock(&global_server->clients_mutex);
		if (global_server->active_clients >= config->max_clients) {
			pthread_mutex_unlock(&global_server->clients_mutex);
			log_message(
				"WARN",
				"Cliente rechazado - límite alcanzado (%d/%d)",
				global_server->active_clients,
				config->max_clients);
			close(client_socket);
			continue;
		}

		// Buscar slot libre para el cliente
		int slot = -1;
		for (int i = 0; i < 50; i++) {
			if (!global_server->clients[i].is_active) {
				slot = i;
				break;
			}
		}

		if (slot == -1) {
			pthread_mutex_unlock(&global_server->clients_mutex);
			log_message("WARN",
				    "No hay slots disponibles para cliente");
			close(client_socket);
			continue;
		}

		// Configurar información del cliente
		global_server->clients[slot].socket_fd = client_socket;
		global_server->clients[slot].client_id = client_id++;
		global_server->clients[slot].address = client_addr;
		global_server->clients[slot].transaction_state =
			TRANSACTION_NONE;
		global_server->clients[slot].connect_time = time(NULL);
		global_server->clients[slot].is_active = 1;
		global_server->active_clients++;

		// Crear thread para manejar el cliente
		if (pthread_create(&global_server->clients[slot].thread_id,
				   NULL, handle_client,
				   &global_server->clients[slot]) != 0) {
			log_message("ERROR",
				    "No se pudo crear thread para cliente");
			close(client_socket);
			global_server->clients[slot].is_active = 0;
			global_server->active_clients--;
		}
		pthread_mutex_unlock(&global_server->clients_mutex);
	}

	return 0;
}

// Función de limpieza
void cleanup_resources(void)
{
	log_message("INFO", "Iniciando limpieza de recursos...");

	if (global_server) {
		// Cerrar socket del servidor
		if (global_server->server_socket > 0) {
			close(global_server->server_socket);
			log_message("INFO", "Socket del servidor cerrado");
		}

		// Esperar a que terminen los threads de clientes
		for (int i = 0; i < 50; i++) {
			if (global_server->clients[i].is_active) {
				global_server->clients[i].is_active = 0;
				close(global_server->clients[i].socket_fd);
				pthread_join(
					global_server->clients[i].thread_id,
					NULL);
			}
		}

		pthread_mutex_destroy(&global_server->clients_mutex);
		free(global_server);
		global_server = NULL;
		log_message("INFO", "Recursos del servidor liberados");
	}

	if (global_database) {
		cleanup_database(global_database);
		global_database = NULL;
		log_message("INFO", "Base de datos cerrada");
	}

	log_message("INFO", "Limpieza completada");
}

int main(int argc, char *argv[])
{
	// Configurar manejadores de señales
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGPIPE, SIG_IGN);
	// Inicializar servidor
	global_server = malloc(sizeof(server_t));
	if (!global_server) {
		fprintf(stderr,
			"Error: no se pudo asignar memoria para el servidor\n");
		return 1;
	}

	memset(global_server, 0, sizeof(server_t));
	pthread_mutex_init(&global_server->clients_mutex, NULL);
	// Verificar argumentos
	const char *config_file = CONFIG_FILE;
	int use_config_file = 1;

	if (argc > 1) {
		if (strcmp(argv[1], "--help") == 0 ||
		    strcmp(argv[1], "-h") == 0) {
			printf("Uso: %s [archivo_configuracion] o %s <host> <puerto> <clientes_concurrentes> <clientes_en_espera> <db_file> <log_level>\n",
			       argv[0], argv[0]);
			printf("  archivo_configuracion: archivo de configuración (por defecto: %s)\n",
			       CONFIG_FILE);
			return 0;
		}
		if (argc == 7) {
			use_config_file = 0;

			global_server->config.max_clients = atoi(argv[3]);
			global_server->config.queue_size = atoi(argv[4]);
			global_server->config.port = atoi(argv[2]);
			strcpy(global_server->config.host, argv[1]);
			strcpy(global_server->config.database_file, argv[5]);
			global_server->config.log_level = atoi(argv[6]);
			puts(global_server->config.database_file);

			log_message(
				"INFO",
				"Configuración desde línea de comandos: %d clientes concurrentes, %d en espera, puerto %d",
				global_server->config.max_clients,
				global_server->config.queue_size,
				global_server->config.port,
				global_server->config.host);
		} else if (argc != 2) {
			fprintf(stderr,
				"Uso incorrecto. Use --help para ayuda.\n");
			return 1;
		}
		config_file = argv[1];
	}

	// Cargar configuración
	if (use_config_file) {
		if (!load_config(&global_server->config, config_file)) {
			log_message("WARN", "Usando configuración por defecto");
		}
	}

	// Inicializar base de datos
	global_database = init_database();
	if (!global_database) {
		log_message("ERROR", "No se pudo inicializar la base de datos");
		cleanup_resources();
		return 1;
	}

	// Cargar datos desde archivo CSV
	if (load_database_from_csv(global_database,
				   global_server->config.database_file) < 0) {
		log_message("ERROR",
			    "No se pudo cargar la base de datos desde %s",
			    global_server->config.database_file);
		cleanup_resources();
		return 1;
	}

	log_message("INFO", "Base de datos cargada: %d registros",
		    global_database->num_records);

	// Iniciar servidor
	int result = start_server(&global_server->config);

	// Limpieza final
	cleanup_resources();

	return result == 0 ? 0 : 1;
}
