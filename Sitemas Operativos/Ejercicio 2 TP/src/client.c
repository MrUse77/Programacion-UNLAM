#include "includes/microdb.h"
#include "includes/protocol.h"
#include <readline/readline.h>
#include <readline/history.h>

typedef struct {
	int socket_fd;
	char host[256];
	int port;
	int connected;
	int in_transaction;
} client_state_t;

// Variables globales del cliente
client_state_t client_state = { 0 };

// Función para mostrar ayuda del cliente
void show_client_help()
{
	printf("\n=== Cliente de Micro Base de Datos ===\n");
	printf("Comandos SQL disponibles:\n");
	printf("  SELECT [campos] WHERE [condición]  - Consultar registros\n");
	printf("  INSERT VALUES (val1,val2,...)      - Insertar nuevo registro\n");
	printf("  UPDATE SET campo=valor WHERE cond  - Actualizar registros\n");
	printf("  DELETE WHERE [condición]           - Eliminar registros\n");
	printf("  DESCRIBE                           - Mostrar estructura de tabla\n");
	printf("\nComandos de transacción:\n");
	printf("  BEGIN TRANSACTION                  - Iniciar transacción\n");
	printf("  COMMIT TRANSACTION                 - Confirmar transacción\n");
	printf("\nComandos del cliente:\n");
	printf("  HELP                              - Mostrar esta ayuda\n");
	printf("  QUIT                              - Salir del cliente\n");
	printf("  .status                           - Estado de la conexión\n");
	printf("  .clear                            - Limpiar pantalla\n");
	printf("=====================================\n\n");
}

// Función para conectar al servidor
int connect_to_server(const char *host, int port)
{
	struct sockaddr_in server_addr;

	client_state.socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_state.socket_fd < 0) {
		printf("Error: No se pudo crear socket\n");
		return -1;
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
		printf("Error: Dirección IP inválida: %s\n", host);
		close(client_state.socket_fd);
		return -1;
	}

	if (connect(client_state.socket_fd, (struct sockaddr *)&server_addr,
		    sizeof(server_addr)) < 0) {
		printf("Error: No se pudo conectar al servidor %s:%d\n", host,
		       port);
		printf("Verifique que el servidor esté ejecutándose\n");
		close(client_state.socket_fd);
		return -1;
	}

	strcpy(client_state.host, host);
	client_state.port = port;
	client_state.connected = 1;
	client_state.in_transaction = 0;

	printf("Conectado al servidor %s:%d\n", host, port);

	// Recibir mensaje de bienvenida
	response_message_t welcome_resp;
	if (receive_response(client_state.socket_fd, &welcome_resp) > 0) {
		printf("\nServidor: %s\n", welcome_resp.message);
		if (strlen(welcome_resp.data) > 0) {
			printf("%s\n", welcome_resp.data);
		}
	}

	return 0;
}

// Función para desconectar del servidor
void disconnect_from_server()
{
	if (client_state.connected) {
		close(client_state.socket_fd);
		client_state.connected = 0;
		printf("Desconectado del servidor\n");
	}
}

// Función para parsear comando del usuario
command_type_t parse_user_command(const char *input, command_message_t *cmd)
{
	if (!input || !cmd)
		return -1;

	memset(cmd, 0, sizeof(command_message_t));
	strncpy(cmd->query, input, MAX_BUFFER_SIZE - 1);

	// Convertir a mayúsculas para parsing
	char upper_input[MAX_BUFFER_SIZE];
	strncpy(upper_input, input, MAX_BUFFER_SIZE - 1);
	for (int i = 0; upper_input[i]; i++) {
		upper_input[i] = toupper(upper_input[i]);
	}

	// Detectar tipo de comando
	if (strncmp(upper_input, "SELECT", 6) == 0) {
		cmd->type = CMD_SELECT;

		// Parsear SELECT para extraer condiciones y campos
		char table[256], conditions[MAX_BUFFER_SIZE],
			fields[MAX_BUFFER_SIZE];
		if (parse_select_query(input, table, conditions, fields) == 0) {
			if (strlen(conditions) > 0) {
				strcpy(cmd->params[0], conditions);
				cmd->param_count = 1;
			}
			if (strlen(fields) > 0) {
				strcpy(cmd->params[1], fields);
				if (cmd->param_count == 0)
					cmd->param_count = 2;
				else
					cmd->param_count = 2;
			}
		}

	} else if (strncmp(upper_input, "INSERT", 6) == 0) {
		cmd->type = CMD_INSERT;

		// Parsear INSERT para extraer valores
		char table[256];
		char values[MAX_FIELDS][MAX_FIELD_SIZE];
		int value_count;

		if (parse_insert_query(input, table, values, &value_count) ==
		    0) {
			cmd->param_count = value_count;
			for (int i = 0; i < value_count && i < 10; i++) {
				strcpy(cmd->params[i], values[i]);
			}
		}

	} else if (strncmp(upper_input, "UPDATE", 6) == 0) {
		cmd->type = CMD_UPDATE;

		// Parsear UPDATE para extraer SET y WHERE
		char table[256], set_clause[MAX_BUFFER_SIZE],
			conditions[MAX_BUFFER_SIZE];
		if (parse_update_query(input, table, set_clause, conditions) ==
		    0) {
			strcpy(cmd->params[0], set_clause);
			strcpy(cmd->params[1], conditions);
			cmd->param_count = 2;
		}

	} else if (strncmp(upper_input, "DELETE", 6) == 0) {
		cmd->type = CMD_DELETE;

		// Parsear DELETE para extraer WHERE
		char table[256], conditions[MAX_BUFFER_SIZE];
		if (parse_delete_query(input, table, conditions) == 0) {
			if (strlen(conditions) > 0) {
				strcpy(cmd->params[0], conditions);
				cmd->param_count = 1;
			}
		}

	} else if (strncmp(upper_input, "BEGIN TRANSACTION", 17) == 0 ||
		   strncmp(upper_input, "BEGIN", 5) == 0) {
		cmd->type = CMD_BEGIN_TRANSACTION;

	} else if (strncmp(upper_input, "COMMIT TRANSACTION", 18) == 0 ||
		   strncmp(upper_input, "COMMIT", 6) == 0) {
		cmd->type = CMD_COMMIT_TRANSACTION;

	} else if (strncmp(upper_input, "DESCRIBE", 8) == 0) {
		cmd->type = CMD_DESCRIBE;

	} else if (strncmp(upper_input, "HELP", 4) == 0) {
		cmd->type = CMD_HELP;

	} else if (strncmp(upper_input, "QUIT", 4) == 0 ||
		   strncmp(upper_input, "EXIT", 4) == 0) {
		cmd->type = CMD_QUIT;

	} else {
		return -1; // Comando no reconocido
	}

	return cmd->type;
}

// Función para mostrar respuesta del servidor
void display_response(response_message_t *resp)
{
	if (!resp)
		return;

	switch (resp->code) {
	case RESP_OK:
		printf("\n✓ %s\n", resp->message);
		break;

	case RESP_ERROR:
		printf("\n✗ Error: %s\n", resp->message);
		break;

	case RESP_NOT_FOUND:
		printf("\n⚠ No encontrado: %s\n", resp->message);
		break;

	case RESP_CONFLICT:
		printf("\n⚠ Conflicto: %s\n", resp->message);
		break;

	case RESP_LOCKED:
		printf("\n🔒 Bloqueado: %s\n", resp->message);
		break;

	case RESP_SERVER_ERROR:
		printf("\n✗ Error del servidor: %s\n", resp->message);
		puts("Desconectando...");
		break;

	default:
		printf("\nRespuesta: %s\n", resp->message);
		break;
	}

	// Mostrar datos si los hay
	if (strlen(resp->data) > 0) {
		printf("\n%s\n", resp->data);
	}
}

// Función para procesar comandos especiales del cliente
int process_client_command(const char *input)
{
	if (strncmp(input, "status", 6) == 0) {
		printf("\n=== Estado de la Conexión ===\n");
		printf("Servidor: %s:%d\n", client_state.host,
		       client_state.port);
		printf("Conectado: %s\n", client_state.connected ? "Sí" : "No");
		printf("En transacción: %s\n",
		       client_state.in_transaction ? "Sí" : "No");
		printf("Socket FD: %d\n", client_state.socket_fd);
		printf("=============================\n");
		return 1;

	} else if (strncmp(input, "clear", 5) == 0) {
		system("clear");
		printf("Cliente de Micro Base de Datos\n");
		printf("Escriba HELP para ver los comandos disponibles\n\n");
		return 1;

	} else if (strncmp(input, "help", 4) == 0) {
		show_client_help();
		return 1;
	}

	return 0; // No es un comando especial del cliente
}

// Función para crear prompt dinámico
char *create_prompt()
{
	static char prompt[100];

	if (client_state.in_transaction) {
		snprintf(prompt, sizeof(prompt), "microdb [TX]> ");
	} else if (client_state.connected) {
		snprintf(prompt, sizeof(prompt), "microdb> ");
	} else {
		snprintf(prompt, sizeof(prompt), "microdb [disconnected]> ");
	}

	return prompt;
}

// Función principal del cliente interactivo
int run_interactive_client()
{
	char *input;
	command_message_t cmd;
	response_message_t resp;

	printf("\nCliente de Micro Base de Datos iniciado\n");
	printf("Escriba HELP para ver los comandos disponibles\n");
	printf("Escriba QUIT para salir\n\n");

	// Configurar readline
	using_history();

	while (1) {
		// Obtener entrada del usuario
		input = readline(create_prompt());

		// Verificar EOF (Ctrl+D)
		if (!input) {
			printf("\nSaliendo...\n");
			break;
		}

		// Ignorar líneas vacías
		if (strlen(input) == 0) {
			free(input);
			continue;
		}

		// Agregar al historial
		add_history(input);

		// Procesar comandos especiales del cliente
		if (process_client_command(input)) {
			free(input);
			continue;
		}

		// Verificar conexión
		if (!client_state.connected) {
			printf("Error: No hay conexión al servidor\n");
			free(input);
			printf("\nSaliendo...\n");
			break;
		}

		// Parsear comando
		if (parse_user_command(input, &cmd) < 0) {
			printf("Error: Comando no reconocido. Escriba HELP para ver los comandos disponibles.\n");
			free(input);
			continue;
		}
		printf("Procesando comando: %s\n", cmd.query);
		printf("Tipo de comando: %d\n", cmd.type);
		printf("Número de parámetros: %d\n", cmd.param_count);

		// Enviar comando al servidor
		if (send_command(client_state.socket_fd, &cmd) <= 0) {
			printf("Error: No se pudo enviar comando al servidor\n");
			client_state.connected = 0;
			free(input);
			continue;
		}

		// Recibir respuesta
		if (receive_response(client_state.socket_fd, &resp) <= 0) {
			printf("Error: No se pudo recibir respuesta del servidor\n");
			client_state.connected = 0;
			free(input);
			break;
		}

		// Procesar respuesta especial para transacciones
		if (cmd.type == CMD_BEGIN_TRANSACTION && resp.code == RESP_OK) {
			client_state.in_transaction = 1;
		} else if (cmd.type == CMD_COMMIT_TRANSACTION &&
			   resp.code == RESP_OK) {
			client_state.in_transaction = 0;
		}

		// Mostrar respuesta
		display_response(&resp);

		// Salir si el comando fue QUIT
		if (cmd.type == CMD_QUIT) {
			free(input);
			break;
		}

		free(input);
	}

	return 0;
}

// Función para mostrar ayuda del programa
void show_usage(const char *program_name)
{
	printf("Uso: %s [opciones]\n", program_name);
	printf("Opciones:\n");
	printf("  -h, --host HOST    Dirección del servidor (por defecto: 127.0.0.1)\n");
	printf("  -p, --port PORT    Puerto del servidor (por defecto: 8080)\n");
	printf("  --help             Mostrar esta ayuda\n");
	printf("\nEjemplos:\n");
	printf("  %s                     # Conectar a localhost:8080\n",
	       program_name);
	printf("  %s -h 192.168.1.100    # Conectar a servidor remoto\n",
	       program_name);
	printf("  %s -h localhost -p 9000 # Conectar a puerto específico\n",
	       program_name);
	printf("\nComandos SQL soportados:\n");
	printf("  SELECT, INSERT, UPDATE, DELETE, BEGIN TRANSACTION, COMMIT TRANSACTION\n");
}

int main(int argc, char *argv[])
{
	char host[256] = DEFAULT_HOST;
	int port = DEFAULT_PORT;

	// Parsear argumentos de línea de comandos
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0) {
			show_usage(argv[0]);
			return 0;
		} else if (strcmp(argv[i], "-h") == 0 ||
			   strcmp(argv[i], "--host") == 0) {
			if (i + 1 < argc) {
				strncpy(host, argv[i + 1], sizeof(host) - 1);
				i++;
			} else {
				printf("Error: La opción %s requiere un argumento\n",
				       argv[i]);
				return 1;
			}
		} else if (strcmp(argv[i], "-p") == 0 ||
			   strcmp(argv[i], "--port") == 0) {
			if (i + 1 < argc) {
				port = atoi(argv[i + 1]);
				if (port <= 0 || port > 65535) {
					printf("Error: Puerto inválido: %s\n",
					       argv[i + 1]);
					return 1;
				}
				i++;
			} else {
				printf("Error: La opción %s requiere un argumento\n",
				       argv[i]);
				return 1;
			}
		} else {
			printf("Error: Opción desconocida: %s\n", argv[i]);
			printf("Use --help para ver las opciones disponibles\n");
			return 1;
		}
	}

	// Configurar manejo de señales
	signal(SIGINT, SIG_IGN); // Ignorar Ctrl+C para permitir salida limpia
	signal(SIGPIPE, SIG_IGN); // Ignorar SIGPIPE en caso de desconexión

	printf("Cliente de Micro Base de Datos v1.0\n");
	printf("Conectando a %s:%d...\n", host, port);

	// Conectar al servidor
	if (connect_to_server(host, port) < 0) {
		return 1;
	}

	// Ejecutar cliente interactivo
	int result = run_interactive_client();

	// Desconectar
	disconnect_from_server();

	printf("Cliente terminado.\n");
	return result;
}
