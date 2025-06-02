// server.c
// Los comentarios tienen que estar en español
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080        // puerto del servidor
#define BACKLOG 10       // Cantidad mexima de conexiones pendientes
#define BUF_SIZE 1024    // Tamaño del buffer para recibir datos
#define MAX_CLIENTES 100 // Máximo número de clientes permitidos
static int clientes[MAX_CLIENTES];
static int listen_fd;          // socket
static int active_clients = 0; // Contador de clientes activos
// Mutex para proteger el contador de clientes activos (condicion de carrera)
static pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Maneja la conexión de un cliente.
 * Receive datos del cliente, cuenta los caracteres y envía la respuesta.
 * @param arg puntero al socket del cliente
 *
 * */
void *client_handler(void *arg) {
  int client_fd = *(int *)arg;
  free(arg);

  char buf[BUF_SIZE];
  ssize_t n;

  while ((n = recv(client_fd, buf, sizeof(buf), 0)) > 0) {
    // Si el cliente envía un mensaje vacío, lo ignoramos
    int count = n;
    // Si el último carácter es un salto de línea, no lo contamos
    char resp[32];
    int len = snprintf(resp, sizeof(resp), "%d\n", count);
    if (send(client_fd, resp, len, 0) < 0) {
      perror("send");
      break;
    }
  }

  // Desconexión del cliente o error
  close(client_fd);

  // decrementar el contador de clientes activos
  pthread_mutex_lock(&count_mutex);
  active_clients--;
  pthread_mutex_unlock(&count_mutex);

  return NULL;
}
// Función para hacer un socket no bloqueante
int make_non_blocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0)
    return -1;
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);

  // Ignorar señales de tubería rota (SIGPIPE)
  signal(SIGPIPE, SIG_IGN);

  // Crear socket
  if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // Configurar socket para reutilizar la dirección
  int yes = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);

  if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(listen_fd, BACKLOG) < 0) {
    perror("listen");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  // Hacer el socket no bloqueante
  if (make_non_blocking(listen_fd) < 0) {
    perror("fcntl");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);

  // Aceptar conexiones entrantes
  while (1) {
    int *pclient = malloc(sizeof(int));
    if (!pclient) {
      perror("malloc");
      continue;
    }
    *pclient = accept(listen_fd, (struct sockaddr *)&addr, &addrlen);
    if (*pclient < 0) {
      free(pclient);
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // no hay nuevos clientes ahora
        usleep(100000);
        continue;
      } else {
        perror("accept");
        break;
      }
    }

    // Hacer el socket del cliente no bloqueante
    pthread_mutex_lock(&count_mutex);
    active_clients++;
    pthread_mutex_unlock(&count_mutex);

    pthread_t tid;
    pthread_create(&tid, NULL, client_handler, pclient);
    pthread_detach(tid);
    break;
  }

  // Bucle principal para aceptar conexiones
  while (1) {
    // Verificar si hay clientes activos
    pthread_mutex_lock(&count_mutex);
    if (active_clients == 0) {
      pthread_mutex_unlock(&count_mutex);
      printf("No more clients. Shutting down.\n");
      break;
    }
    pthread_mutex_unlock(&count_mutex);

    int *pclient = malloc(sizeof(int));
    if (!pclient) {
      perror("malloc");
      continue;
    }
    *pclient = accept(listen_fd, (struct sockaddr *)&addr, &addrlen);
    if (*pclient < 0) {
      free(pclient);
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // no hay nuevos clientes ahora
        usleep(100000);
        continue;
      } else {
        perror("accept");
        break;
      }
    }

    // Hacer el socket del cliente no bloqueante
    pthread_mutex_lock(&count_mutex);
    active_clients++;
    pthread_mutex_unlock(&count_mutex);

    pthread_t tid;
    pthread_create(&tid, NULL, client_handler, pclient);
    pthread_detach(tid);
  }

  close(listen_fd);
  return 0;
}
