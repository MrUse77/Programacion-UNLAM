#ifndef SOLUCIONMATRIZDEPOSITOS_SOLUCIONMATRIZDEPOSITOS_H
#define SOLUCIONMATRIZDEPOSITOS_SOLUCIONMATRIZDEPOSITOS_H
#include "../Vector/Vector.h"

typedef struct {
	char codDeposito[11];
	int stock;
} Stock;

typedef struct {
	char depOrigen[11];
	char depDestino[11];
	int cantidad;
} Movimiento;

typedef struct {
	char codDeposito[11];
} CodigoDeposito;

void generarStocks(const char *nombreStocks);
void generarMovimientos(const char *nombreMovimientos);
void mostrarStocks(const char *nombreStocks);
void mostrarMovimientos(const char *nombreMovimientos);
int actualizarDepositos(const char *nombreArchivoStocks,
			const char *nombreArchivoMovimientos);
int crearVectorCodigosDepositos(const char *nombreStocks,
				Vector *codigosDepositos);
void inicializarMatrizEnCero(int **matMovDeps, int filas, int columnas);
int cargarMatrizMovimientosDepositos(const char *nombreMovimientos,
				     int **matMovDeps,
				     Vector *codigosDepositos);
int procesarMatrizMovimientosDepositosActualizandoStocks(
	int **matMovDeps, const Vector *codigosDepositos,
	const char *nombreStocks);
int cmpCodDep(const void *e1, const void *e2);

#endif //SOLUCIONMATRIZDEPOSITOS_SOLUCIONMATRIZDEPOSITOS_H
