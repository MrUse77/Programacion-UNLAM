#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 4096

static int client_socket = -1;
static volatile sig_atomic_t running = 1;

void cleanup()
{
	if (client_socket != -1) {
		close(client_socket);
		client_socket = -1;
	}
}

void signal_handler(int sig)
{
	if (sig == SIGINT || sig == SIGTERM) {
		printf("\nCerrando cliente...\n");
		cleanup();
		exit(0);
	}
}

void print_usage(const char* program)
{
	printf("Uso: %s -s <servidor> -p <puerto>\n", program);
	printf("Opciones:\n");
	printf("  -s <servidor>  Dirección IP o hostname del servidor (requerido)\n");
	printf("  -p <puerto>    Puerto del servidor (requerido)\n");
	printf("  -h             Mostrar esta ayuda\n");
}

void print_help()
{
	printf("\n=== AYUDA DE COMANDOS ===\n");
	printf("Consultas:\n");
	printf("  SELECT ALL                      - Mostrar todos los registros\n");
	printf("  SELECT campo valor              - Buscar registros por campo\n");
	printf("    Ejemplo: SELECT City Madrid\n");
	printf("    Ejemplo: SELECT Name Carlos\n");
	printf("\n");
	printf("Modificaciones:\n");
	printf("  INSERT id,nombre,edad,ciudad,departamento,salario,experiencia\n");
	printf("    Ejemplo: INSERT 500,Juan,30,Madrid,IT,50000,5\n");
	printf("\n");
	printf("  UPDATE id campo nuevo_valor\n");
	printf("    Ejemplo: UPDATE 500 Salary 55000\n");
	printf("    Ejemplo: UPDATE 500 City Barcelona\n");
	printf("\n");
	printf("  DELETE id\n");
	printf("    Ejemplo: DELETE 500\n");
	printf("\n");
	printf("Transacciones:\n");
	printf("  BEGIN TRANSACTION               - Iniciar transacción (bloqueo exclusivo)\n");
	printf("  COMMIT TRANSACTION              - Confirmar transacción\n");
	printf("\n");
	printf("Otros:\n");
	printf("  HELP                            - Mostrar esta ayuda\n");
	printf("  QUIT                            - Salir del cliente\n");
	printf("========================\n\n");
}

int main(int argc, char* argv[])
{
	char* server_addr = NULL;
	int port = -1;
	int opt;

	// Parsear argumentos
	while ((opt = getopt(argc, argv, "s:p:h")) != -1) {
		switch (opt) {
		case 's':
			server_addr = optarg;
			break;
		case 'p':
			port = atoi(optarg);
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
	if (!server_addr || port < 0 || port > 65535) {
		fprintf(stderr, "Error: Parámetros inválidos\n");
		print_usage(argv[0]);
		return 1;
	}

	// Configurar manejador de señales
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	// Crear socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0) {
		perror("Error al crear socket");
		return 1;
	}

	// Configurar dirección del servidor
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (inet_pton(AF_INET, server_addr, &server.sin_addr) <= 0) {
		fprintf(stderr, "Error: Dirección IP inválida\n");
		close(client_socket);
		return 1;
	}

	// Conectar al servidor
	printf("Conectando a %s:%d...\n", server_addr, port);
	if (connect(client_socket, (struct sockaddr*)&server, sizeof(server)) <
			0) {
		perror("Error al conectar");
		close(client_socket);
		return 1;
	}

	printf("Conectado al servidor!\n");

	// Recibir mensaje de bienvenida
	char* buffer = malloc(BUFFER_SIZE);
	if (!buffer) {
		perror("Error al asignar memoria");
		close(client_socket);
		return 1;
	}
	memset(buffer, 0, BUFFER_SIZE);
	int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
	if (bytes_received > 0) {
		buffer[bytes_received] = '\0';
		printf("\n%s\n", buffer);
	}

	printf("Escriba HELP para ver los comandos disponibles\n");
	printf("Escriba QUIT para salir\n\n");

	// Bucle interactivo
	while (running) {
		printf("db> ");
		fflush(stdout);

		char command[BUFFER_SIZE];
		if (!fgets(command, sizeof(command), stdin)) {
			break;
		}

		// Eliminar salto de línea
		command[strcspn(command, "\n")] = 0;

		// Verificar si está vacío
		if (strlen(command) == 0) {
			continue;
		}

		// Comando local HELP
		if (strcasecmp(command, "HELP") == 0) {
			print_help();
			continue;
		}

		// Enviar comando al servidor
		if (send(client_socket, command, strlen(command), 0) < 0) {
			perror("Error al enviar comando");
			break;
		}

		// Recibir respuesta
		memset(buffer, 0, BUFFER_SIZE);
		bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
		printf("Respuesta del servidor: %d %d\n", bytes_received, BUFFER_SIZE);
		if (bytes_received > 0) {
			buffer[bytes_received] = '\0';

			// Verificar si hay más datos por recibir
			while (bytes_received == BUFFER_SIZE - 1) {
				char* new_buffer = realloc(buffer, strlen(buffer) + BUFFER_SIZE);
				if (!new_buffer) {
					perror("Error al realocar memoria");
					break;
				}
				buffer = new_buffer;

				int more_bytes = recv(client_socket, buffer + strlen(buffer), BUFFER_SIZE - 1, 0);
				if (more_bytes <= 0) {
					break;
				}
				buffer[strlen(buffer) + more_bytes] = '\0';
				bytes_received = more_bytes;
			}
		}

		if (bytes_received <= 0) {
			if (bytes_received == 0) {
				printf("Servidor desconectado\n");
			}
			else {
				perror("Error al recibir respuesta");
			}
			break;
		}

		printf("%s\n", buffer);

		// Verificar si fue comando QUIT


	}
	free(buffer);
	cleanup();
	printf("Cliente cerrado\n");
	return 0;
}
