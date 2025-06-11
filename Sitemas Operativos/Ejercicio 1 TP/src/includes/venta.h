#ifndef FILE_H
#define FILE_H

#define MAX_VENTAS 1000
#define DIAS 30

typedef struct {
  int id;                 // Identificador de venta
  double monto;           // Monto de la venta
  double montoModificado; // Monto de la venta

} Venta;

typedef struct {
  Venta *ventas;
  int numVentas;
  float total;
} Dia;

typedef struct {
  Dia *mes;
  Venta *ventaBase;
} MC;
int crearVentas(Venta *venta);
MC crearMesYVentas(int shmid, int shmid_ventas);

#endif // !#ifndef FILE_H
