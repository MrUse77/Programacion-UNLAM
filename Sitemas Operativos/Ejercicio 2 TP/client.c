#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

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

// Recibir datos dinámicamente (lee en chunks y los concatena)
char* recv_dynamic(int sock, ssize_t* out_len)
{
	const size_t CHUNK = 8192; // Chunk más grande para reducir llamadas
	char* tmp = malloc(CHUNK);
	if (!tmp) return NULL;

	char* buf = NULL;
	size_t len = 0;
	size_t capacity = 0;

	while (1) {
		// Usar select para ver si hay datos disponibles (timeout de 100ms)
		fd_set readfds;
		struct timeval timeout;
		FD_ZERO(&readfds);
		FD_SET(sock, &readfds);
		timeout.tv_sec = 0;
		timeout.tv_usec = 100000; // 100ms

		int select_result = select(sock + 1, &readfds, NULL, NULL, &timeout);
		if (select_result < 0) {
			if (errno == EINTR) continue;
			free(buf);
			free(tmp);
			return NULL;
		}

		// Si no hay datos disponibles después del timeout, terminamos
		if (select_result == 0) {
			// Solo salir si ya leímos algo
			if (len > 0) break;
			// Si no hemos leído nada, seguir esperando (primera lectura)
			continue;
		}

		ssize_t n = recv(sock, tmp, CHUNK, 0);
		if (n < 0) {
			if (errno == EINTR) continue; // reintentar si fue interrumpido
			free(buf);
			free(tmp);
			return NULL;
		}
		if (n == 0) {
			// conexión cerrada por el otro extremo
			break;
		}

		// Expandir buffer si es necesario
		if (len + n + 1 > capacity) {
			size_t new_capacity = (capacity == 0) ? CHUNK : capacity * 2;
			while (new_capacity < len + n + 1) {
				new_capacity *= 2;
			}
			char* newbuf = realloc(buf, new_capacity);
			if (!newbuf) {
				free(buf);
				free(tmp);
				return NULL;
			}
			buf = newbuf;
			capacity = new_capacity;
		}

		memcpy(buf + len, tmp, n);
		len += n;
	}

	free(tmp);

	if (!buf) {
		buf = malloc(1);
		if (!buf) return NULL;
		buf[0] = '\0';
	}
	else {
		buf[len] = '\0';
	}

	if (out_len) *out_len = (ssize_t)len;
	return buf;
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
	ssize_t bytes_received;
	char* buffer = recv_dynamic(client_socket, &bytes_received);
	if (!buffer) {
		perror("Error al recibir bienvenida");
		close(client_socket);
		return 1;
	}
	if (bytes_received > 0) {
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
		free(buffer); // liberar buffer anterior
		buffer = recv_dynamic(client_socket, &bytes_received);
		if (!buffer) {
			perror("Error al recibir respuesta");
			break;
		}

		if (bytes_received <= 0) {
			if (bytes_received == 0) {
				printf("Servidor desconectado\n");
			}
			break;
		}

		printf("%s\n", buffer);

		// Verificar si fue comando QUIT
		if (strcasecmp(command, "QUIT") == 0) {
			break;
		}
	}
	free(buffer);
	cleanup();
	printf("Cliente cerrado\n");
	return 0;
}
