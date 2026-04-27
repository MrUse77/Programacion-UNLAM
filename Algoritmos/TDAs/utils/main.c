#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

// Función que detecta el SO y limpia la terminal
void limpiar_terminal()
{
#ifdef _WIN32
	system("cls");
#elif __linux__ || __APPLE__
	// Para Linux o macOS (usan 'clear')
	system("clear");
#else
	// Fallback si no se detecta nada conocido
	printf("\n\n=========================================\n");
#endif
}

// Función para esperar segundos, adaptada al SO
void esperar_segundos(int segundos)
{
#ifdef _WIN32
// --- PARA WINDOWS (usa Sleep de <windows.h>) ---
#include <windows.h>
	printf("Esperando %d segundos en Windows...\n", segundos);
	Sleep(segundos * 1000); // Sleep espera milisegundos
#elif __linux__ || __APPLE__
// --- PARA LINUX/MACOS (usa sleep de <unistd.h>) ---
#include <unistd.h>
	sleep(segundos); // sleep espera segundos enteros
#else
	// Fallback si no se detecta el SO
	printf("\n\n=========================================\n");
	printf("Advertencia: No hay función de pausa para este sistema operativo.\n");
#endif
}

void scanfInt(int *n, int li, int ls, int c)
{
	do {
		scanf("%d", n);
	} while ((*n < li || *n > ls) && (*n != c));
}

void flush()
{
	int c;
	do {
		c = getchar();
	} while (c != EOF && c != '\n');
}

void scanfFloat(float *n, float li, float ls, float c)
{
	do {
		scanf("%f", n);
	} while ((*n < li || *n > ls) && (*n != c));
}

void fsgets(char t[], int n)
{
	int i = 0;
	fgets(t, n, stdin);
	while (t[i] != '\0') {
		if (t[i] == '\n') {
			t[i] = '\0';
		} else {
			i++;
		}
	}
}

void Err(char t[])
{
	puts(t);
	exit(1);
}
