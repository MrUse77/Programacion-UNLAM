#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define PUERTO 8080
#define TAM_BUFFER 1024

// Estructura para pasar datos a los hilos
typedef struct {
    int socket_cliente;
    struct sockaddr_in direccion;
} datos_cliente;

// Función manejadora de cada cliente en un hilo separado
void *manejar_cliente(void *datos) {
    datos_cliente *datos_cl = (datos_cliente *)datos;
    char buffer[TAM_BUFFER];
    int bytes_recibidos;

    printf("Cliente conectado: %s:%d\n",
           inet_ntoa(datos_cl->direccion.sin_addr),
           ntohs(datos_cl->direccion.sin_port));

    while ((bytes_recibidos = recv(datos_cl->socket_cliente, buffer, TAM_BUFFER - 1, 0)) > 0) {
        buffer[bytes_recibidos] = '\0'; // Terminar cadena
        int longitud = strlen(buffer);
        printf("Recibido de %s:%d: '%s' (%d caracteres)\n",
               inet_ntoa(datos_cl->direccion.sin_addr),
               ntohs(datos_cl->direccion.sin_port),
               buffer, longitud);

        // Enviar respuesta (longitud del mensaje)
        char respuesta[20];
        snprintf(respuesta, sizeof(respuesta), "%d", longitud);
        send(datos_cl->socket_cliente, respuesta, strlen(respuesta), 0);
    }

    if (bytes_recibidos == 0) {
        printf("Cliente desconectado: %s:%d\n",
               inet_ntoa(datos_cl->direccion.sin_addr),
               ntohs(datos_cl->direccion.sin_port));
    } else {
        perror("Error en recv");
    }

    close(datos_cl->socket_cliente);
    free(datos_cl); // Liberar memoria de la estructura
    pthread_exit(NULL);
}

int main() {
    int servidor_fd, cliente_fd;
    struct sockaddr_in direccion_servidor, direccion_cliente;
    socklen_t addr_len = sizeof(direccion_cliente);
    pthread_t hilo_id;

    // Crear socket del servidor
    if ((servidor_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error al crear socket");
        exit(EXIT_FAILURE);
    }

    // Configurar dirección del servidor
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_addr.s_addr = INADDR_ANY;
    direccion_servidor.sin_port = htons(PUERTO);

    // Enlazar socket al puerto
    if (bind(servidor_fd, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) < 0) {
        perror("Error en bind");
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones
    if (listen(servidor_fd, 5) < 0) {
        perror("Error en listen");
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en puerto %d...\n", PUERTO);

    while (1) {
        // Aceptar nueva conexión
        if ((cliente_fd = accept(servidor_fd, (struct sockaddr *)&direccion_cliente, &addr_len)) < 0) {
            perror("Error en accept");
            continue;
        }

        // Preparar datos para el hilo
        datos_cliente *datos_cl = malloc(sizeof(datos_cliente));
        datos_cl->socket_cliente = cliente_fd;
        memcpy(&datos_cl->direccion, &direccion_cliente, sizeof(direccion_cliente));

        // Crear hilo para el cliente
        if (pthread_create(&hilo_id, NULL, manejar_cliente, (void *)datos_cl) < 0) {
            perror("Error al crear hilo");
            close(cliente_fd);
            free(datos_cl);
            continue;
        }

        // Liberar recursos del hilo al terminar
        pthread_detach(hilo_id);
    }

    close(servidor_fd);
    return 0;
}
