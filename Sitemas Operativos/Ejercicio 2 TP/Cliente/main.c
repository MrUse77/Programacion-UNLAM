#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVIDOR_IP "127.0.0.1"
#define PUERTO 8080
#define TAM_BUFFER 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[TAM_BUFFER];

    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error al crear socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PUERTO);

    // Convertir dirección IP
    if (inet_pton(AF_INET, SERVIDOR_IP, &serv_addr.sin_addr) <= 0) {
        perror("Dirección inválida");
        exit(EXIT_FAILURE);
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error en connect");
        exit(EXIT_FAILURE);
    }

    printf("Conectado al servidor. Escriba mensajes (o 'exit' para salir):\n");

    while (1) {
        printf("> ");
        fgets(buffer, TAM_BUFFER, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Eliminar salto de línea

        if (strcmp(buffer, "exit") == 0) break;

        // Enviar mensaje al servidor
        send(sock, buffer, strlen(buffer), 0);

        // Recibir respuesta
        int bytes_recibidos = recv(sock, buffer, TAM_BUFFER, 0);
        if (bytes_recibidos <= 0) {
            perror("Conexión cerrada o error");
            break;
        }
        buffer[bytes_recibidos] = '\0';
        printf("Respuesta del servidor: %s caracteres\n", buffer);
    }

    close(sock);
    return 0;
}
