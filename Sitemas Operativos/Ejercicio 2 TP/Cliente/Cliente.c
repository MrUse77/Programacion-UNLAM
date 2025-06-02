// client.c
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT "8080"
#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]);
    return 1;
  }

  // Obtener información de la dirección del servidor
  struct addrinfo hints = {.ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM};
  struct addrinfo *res;
  if (getaddrinfo(argv[1], PORT, &hints, &res) != 0) {
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
  printf("Connected to %s:%s. Type messages, one per line. Ctrl+D to quit.\n",
         argv[1], PORT);

  char sendbuf[BUF_SIZE];
  char recvbuf[BUF_SIZE];

  while (fgets(sendbuf, sizeof(sendbuf), stdin)) {
    size_t len = strlen(sendbuf);
    if (send(sock, sendbuf, len, 0) < 0) {
      perror("send");
      break;
    }
    // Esperar respuesta del servidor
    ssize_t n = recv(sock, recvbuf, sizeof(recvbuf) - 1, 0);
    if (n <= 0) {
      printf("Server closed connection.\n");
      break;
    }
    recvbuf[n] = '\0';
    printf("Char count: %s", recvbuf);
  }

  close(sock);
  printf("Client exiting.\n");
  return 0;
}
