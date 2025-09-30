// server.c
#include <arpa/inet.h>
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

#define PORT 1           // puerto del servidor
#define BACKLOG 10       // Cantidad mexima de conexiones pendientes
#define BUF_SIZE 1024    // Tamaño del buffer para recibir datos
#define MAX_CLIENTES 100 // Máximo número de clientes permitidos

static int clientes[MAX_CLIENTES];
static int listen_fd;          // socket
static int active_clients = 0; // Contador de clientes activos
// Mutex para proteger el contador de clientes activos (condicion de carrera)
static pthread_mutex_t count_mutex =
    PTHREAD_MUTEX_INITIALIZER; // Mutex para contar clientes
static pthread_mutex_t clientes_mutex =
    PTHREAD_MUTEX_INITIALIZER; // Mutex para lista de clientes

// Agrega un cliente al arreglo
void agregar_cliente(int fd) {
  pthread_mutex_lock(&clientes_mutex);
  for (int i = 0; i < MAX_CLIENTES; i++) {
    if (clientes[i] == -1) {
      clientes[i] = fd;
      break;
    }
  }
  pthread_mutex_unlock(&clientes_mutex);
}

// Elimina un cliente del arreglo
void eliminar_cliente(int fd) {
  pthread_mutex_lock(&clientes_mutex);
  for (int i = 0; i < active_clients; i++) {
    if (clientes[i] == fd) {
      clientes[i] = -1;
      break;
    }
  }
  pthread_mutex_unlock(&clientes_mutex);
}
void enviar_a_todos(int emisor_fd, const char *mensaje, size_t len) {
  pthread_mutex_lock(&clientes_mutex);
  for (int i = 0; i < active_clients; i++) {
    int fd = clientes[i];

    if (fd > 0 && fd != emisor_fd) {
      printf("Enviando mensaje a cliente %d\n", fd);
      printf("Mensaje: %.*s\n", (int)len, mensaje);
      printf("Emisor: %d\n", emisor_fd);
      if (send(fd, mensaje, len, 0) < 0) {
        perror("Error al enviar mensaje a cliente");
      }
    }
  }
  pthread_mutex_unlock(&clientes_mutex);
}

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

  while ((n = recv(client_fd, buf, sizeof(buf) - 1, 0)) > 0) {
    buf[n] = '\0'; // Convertimos el mensaje recibido en un string válido

    // Formatear la respuesta que contiene el mensaje original y el conteo de
    // caracteres
    char mensaje_formateado[BUF_SIZE + 100];
    int count = strlen(buf);
    snprintf(mensaje_formateado, sizeof(mensaje_formateado),
             "Mensaje recibido: %s - Letras: %d\n", buf, count);

    // Enviar el mensaje a todos los clientes conectados (incluido el emisor)
    pthread_mutex_lock(&count_mutex);
    enviar_a_todos(client_fd, mensaje_formateado, strlen(mensaje_formateado));
    pthread_mutex_unlock(&count_mutex);
  }

  // Desconexión del cliente
  close(client_fd);

  // Eliminar al cliente del array y restar el contador
  pthread_mutex_lock(&count_mutex);
  for (int i = 0; i < MAX_CLIENTES; i++) {
    if (clientes[i] == client_fd) {
      clientes[i] = 0;
      break;
    }
  }
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

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  int b = 0;
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
  addr.sin_port = htons(atoi(argv[PORT]));

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

  printf("Servidor escuchando en el puerto: %d\n", atoi(argv[PORT]));

  // Bucle principal para aceptar conexiones
  while (1) {
    // Verificar si hay clientes activos
    pthread_mutex_lock(&count_mutex);
    if (active_clients == 0 && b == 1) {
      pthread_mutex_unlock(&count_mutex);
      printf("No hay mas clientes. Apagando...\n");
      break;
    }
    pthread_mutex_unlock(&count_mutex);

    int *pclient = malloc(sizeof(int));
    if (!pclient) {
      perror("malloc");
      continue;
    }
    *pclient = accept(listen_fd, (struct sockaddr *)&addr, &addrlen);
    char *ip = inet_ntoa(addr.sin_addr);
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
    pthread_mutex_lock(&clientes_mutex);
    int added = 0;
    for (int i = 0; i < MAX_CLIENTES; i++) {
      if (clientes[i] == 0) {
        printf("Cliente %d conectado desde: %s\n", *pclient, ip);
        clientes[i] = *pclient;
        added = 1;
        break;
      }
    }
    if (!added) {
      fprintf(stderr, "Máximo de clientes alcanzado. Cerrando conexión.\n");
      close(*pclient);
      free(pclient);
      continue;
    }
    pthread_mutex_unlock(&clientes_mutex);
    // Hacer el socket del cliente no bloqueante
    pthread_mutex_lock(&count_mutex);
    active_clients++;
    pthread_mutex_unlock(&count_mutex);

    pthread_t tid;
    pthread_create(&tid, NULL, client_handler, pclient);
    pthread_detach(tid);
    b = 1;
  }

  close(listen_fd);
  return 0;
}
