#include <stdio.h>
#include <stdlib.h>
#include <Cola.h>
#include <time.h>
#include <limits.h>
typedef struct {
	int id;
	int tiempoArribo; // Cuándo llega (T_arribo = T_anterior + TEA)
	int tiempoServicio; // Cuánto dura (TS)
	int inicioServicio; // Max(Arribo, Fin anterior)
	int finServicio; // Inicio + TS
	int tiempoEspera; // Inicio - Arribo
} Cliente;
int generarTEA()
{
	int opciones[] = { 1, 5, 9 };
	return opciones[rand() %
			3]; // Elige uno de los 3 índices con 1/3 de prob.
}

int generarTS()
{
	int opciones[] = { 1, 3, 5 };
	return opciones[rand() % 3];
}
int main()
{
	t_Cola cola;
	crearCola(&cola);
	srand((unsigned)time(NULL));
	int reloj = 0;
	int prox_arribo = 0;
	int fin_servicio = INT_MAX;
	int veces_vacia = 0;
	int cajero = 0;
	int id_cliente = 0;
	int code;
	Cliente cliCajero;
	while (veces_vacia < 5) {
		printf("[t=%d] prox_arribo=%d fin_servicio=%d cajero=%d cola_vacia=%d veces_vacia=%d\n",
		       reloj, prox_arribo, fin_servicio, cajero,
		       colaVacia(&cola), veces_vacia);
		if (prox_arribo <= fin_servicio) {
			Cliente cli;
			if (prox_arribo == reloj) {
				cli.id = id_cliente++;
				cli.tiempoArribo = reloj;
				cli.finServicio = 0;
				cli.tiempoEspera = 0;
				printf("Llega cliente %d en t=%d\n", cli.id,
				       cli.tiempoArribo);
				if (cajero == 0 && colaVacia(&cola)) {
					cliCajero = cli;
					cliCajero.finServicio =
						reloj + generarTS();
					cajero = 1;
					printf(">> Cliente %d entra a servicio en t=%d, TS=%d, fin=%d\n",
					       cliCajero.id, reloj,
					       cliCajero.tiempoServicio,
					       cliCajero.finServicio);
				} else {
					printf(">> Cliente %d va a la cola en t=%d\n",
					       cli.id, reloj);
					code = ponerEnCola(&cola, &cli,
							   sizeof(Cliente));
					if (code != OK) {
						return code;
					}
				}
				prox_arribo = reloj + generarTEA();
			}
			if (cliCajero.finServicio == reloj) {
				printf("<< Termina cliente %d en t=%d\n",
				       cliCajero.id, reloj);
				cajero = 0;
				code = sacarDeCola(&cola, &cliCajero,
						   sizeof(Cliente));
				if (code == OK) {
					cliCajero.tiempoServicio = 1;
					cliCajero.finServicio =
						reloj + generarTS();
					cajero = 1;
					printf(">> Cliente %d entra a servicio en t=%d, TS=%d, fin=%d\n",
					       cliCajero.id, reloj,
					       cliCajero.tiempoServicio,
					       cliCajero.finServicio);
				} else if (code == ERR_COLA_VACIA) {
					printf("** Cola vacia en t=%d (veces_vacia=%d)\n",
					       reloj, veces_vacia);
					veces_vacia++;
				}
			}
			reloj++;
		}
	}
	return 0;
}
