#include "includes/venta.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <time.h>

int crearVentas(Venta *venta) {
  int nVentas = rand() % MAX_VENTAS;
  srand(time(NULL));
  for (int i = 0; i < nVentas; i++) {
    venta[i].id = rand() % 1000;
    venta[i].monto = ((double)(rand() % 1000) / 10);
    venta[i].montoModificado = venta[i].monto;
  }
  return nVentas;
}

MC crearMesYVentas(int shmid, int shmid_ventas) {
  Dia *mesCompartido = (Dia *)shmat(shmid, NULL, 0);
  Venta *ventaTemp = (Venta *)shmat(shmid_ventas, NULL, 0);

  if (mesCompartido == (Dia *)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  for (int d = 0; d < DIAS; d++) {

    Venta *venta = &ventaTemp[d * MAX_VENTAS];
    int numVentas = crearVentas(venta);
    mesCompartido[d].ventas = venta;
    mesCompartido[d].numVentas = numVentas;
    mesCompartido[d].total = 0.0;

    // Calcular total inicial
    for (int v = 0; v < numVentas; v++) {
      mesCompartido[d].total += venta[v].monto;
    }
  }
  MC mc = {mesCompartido, ventaTemp};
  return mc;
}
