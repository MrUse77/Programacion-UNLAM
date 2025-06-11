// client.c
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 2
#define ADDRESS 1
#define BUF_SIZE 1024

char *fsgets(char t[], int n) {
  int i = 0;
  fgets(t, n, stdin);
  while (t[i] != '\0') {
    if (t[i] == '\n') {
      t[i] = '\0';
    } else {
      i++;
    }
  }
  return t;
}
int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Uso: %s <ip_servidor> <puerto>\n", argv[0]);
    return 1;
  }

  // Obtener información de la dirección del servidor
  struct addrinfo hints = {.ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM};
  struct addrinfo *res;
  if (getaddrinfo(argv[ADDRESS], argv[PORT], &hints, &res) != 0) {
    perror("getaddrinfo");
    return 1;
  }

  // Crear socket y conectarse al servidor
  int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sock < 0) {
    perror("socket");
    freeaddrinfo(res);
    return 1;
  }

  if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
    perror("connect");
    close(sock);
    freeaddrinfo(res);
    return 1;
  }
  freeaddrinfo(res);
  printf(
      "Conectado a %s:%s. Escribe un mensaje. Ctrl+C o \"exit\" para salir.\n",
      argv[ADDRESS], argv[PORT]);

  char sendbuf[BUF_SIZE];
  char recvbuf[BUF_SIZE];

  while (1) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds); // Entrada estándar
    FD_SET(sock, &readfds);         // Socket del servidor
    int maxfd = sock > STDIN_FILENO ? sock : STDIN_FILENO;
    int ready = select(maxfd + 1, &readfds, NULL, NULL, NULL);
    if (ready < 0) {
      perror("select");
      close(sock);
      return 1;
    }
    if (FD_ISSET(STDIN_FILENO, &readfds)) {
      // Leer del stdin

      if (fsgets(sendbuf, BUF_SIZE) == NULL || strcmp(sendbuf, "exit") == 0) {
        break; // Ctrl+D o error
      }
      size_t len = strlen(sendbuf);
      if (send(sock, sendbuf, len, 0) < 0) {
        perror("send");
        break;
      }
    }
    // Si el servidor envió algo
    if (FD_ISSET(sock, &readfds)) {
      ssize_t n = recv(sock, recvbuf, sizeof(recvbuf) - 1, 0);
      if (n <= 0) {
        printf("El servidor cerró la conexión.\n");
        break;
      }

      recvbuf[n] = '\0';
      printf("%s", recvbuf); // Muestra directamente lo que envió el servidor
    }
  }

  close(sock);
  printf("Saliendo...\n");
  return 0;
}
