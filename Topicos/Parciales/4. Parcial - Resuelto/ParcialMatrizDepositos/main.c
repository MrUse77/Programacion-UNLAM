/**********************************************************************************************************************
******************************* Complete sus datos ********************************************************************
***********************************************************************************************************************
* Apellido, Nombres: Panasiuk, Fabio
*
* DNI:
*
***********************************************************************************************************************
 * Correcci�n
***************

***********************************************************************************************************************
 * Nota:
**********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Matriz/Matriz.h"
#include "../Vector/Vector.h"
#include "../SolucionMatrizDepositos/SolucionMatrizDepositos.h"

#define ARG_STOCKS 1
#define ARG_MOVIMIENTOS 2
#define FILAS 4
#define COLUMNAS 4

#define TAM_CRITICO 0.7
#define CRECIMIENTO 1.3
#define CODES "ABCD"

void actualizarStocks(int **matriz, char *nombreStocks);
void guardarEnArchivo(void *elem, void *extra);
void modificarCant(void *elem, void *extra);
int indexOf(const char *c, const char *src);
// Prototipos de réplicas de SolucionMatrizDepositos.h
int actualizarDepositos_ALU(const char *nombreStocks,
			    const char *nombreMovimientos);
int cargarMatrizMovimientosDepositos_ALU(const char *nombreMovimientos,
					 int **matMovDeps,
					 Vector *codigosDepositos);
int procesarMatrizMovimientosDepositosActualizandoStocks_ALU(
	int **matMovDeps, const Vector *codigosDepositos,
	const char *nombreStocks);
void inicializarMatrizEnCero_ALU(int **matMovDeps, int filas, int columnas);
int crearVectorCodigosDepositos_ALU(const char *nombreStocks,
				    Vector *codigosDepositos);
int cmpCodDep_ALU(const void *e1, const void *e2);

// Prototipos de réplicas de Vector.h
int vectorCantidadElementos_ALU(const Vector *v);
int vectorCargarDeArchivo_ALU(const char *nombreArch, Vector *vector,
			      size_t tamElem);
int vectorOrdBuscar_ALU(const Vector *v, void *elem, Cmp cmp);
int vectorCrear_ALU(Vector *vec, size_t tamElem);
int vectorInsertarAlFinal_ALU(Vector *vector, const void *elem);
void vectorDestruir_ALU(Vector *vec);
int vectorOrdInsertarCDup_ALU(Vector *vector, const void *elem, Cmp cmp);

// Prototipos de réplicas de Matriz.h
void matrizMostrar_ALU(int fil, int col, int **mat);
void **matrizCrear_ALU(int fil, int col, size_t tamElem);
void matrizDestruir_ALU(void **mat, int fil);
int MatrizCargarDeTxt_ALU(char *codes, int **matriz,
			  const char *nombreMovimientos);

// Prototipos de funciones extra (no presentes en los .h dados)
void *vectorObtenerElem_ALU(const Vector *v, int indice);

// Entrada del programa

int main(int argc, char *argv[])
{
	generarStocks(argv[ARG_STOCKS]);
	generarMovimientos(argv[ARG_MOVIMIENTOS]);

	puts("Stocks antes de actualizar:");
	mostrarStocks(argv[ARG_STOCKS]);

	/*********************************************************************************************************************/
	//int cod = actualizarDepositos_ALU(argv[ARG_STOCKS],argv[ARG_MOVIMIENTOS]);
	//printf("codigo: %d\n", cod);
	/************* Descomente la funci�n de arriba y comente la de abajo para ejecutar su c�digo. ************************/
	actualizarDepositos(argv[ARG_STOCKS], argv[ARG_MOVIMIENTOS]);
	/*********************************************************************************************************************/

	puts("\nStocks despues de actualizar:");
	mostrarStocks(argv[ARG_STOCKS]);

	return 0;
}

// Réplicas de SolucionMatrizDepositos.h

int actualizarDepositos_ALU(const char *nombreStocks,
			    const char *nombreMovimientos)
{
	int cod = TODO_OK;
	printf("Actualizando stocks a partir de los movimientos...\n");

	int **matriz = (int **)matrizCrear(FILAS, COLUMNAS, sizeof(int));
	inicializarMatrizEnCero_ALU(matriz, FILAS, COLUMNAS);
	cod = MatrizCargarDeTxt_ALU(CODES, matriz, nombreMovimientos);

	if (cod != TODO_OK)
		return cod;
	actualizarStocks(matriz, (char *)nombreStocks);

	if (cod != TODO_OK)
		return cod;

	/// Resolver. Todas las funciones que desarrolle deber�n tener el sufijo _ALU.

	return cod;
}

void inicializarMatrizEnCero_ALU(int **matMovDeps, int filas, int columnas)
{
	for (int i = 0; i < filas; i++) {
		for (int j = 0; j < columnas; j++) {
			*(*(matMovDeps + i) + j) = 0;
		}
	}
}

int MatrizCargarDeTxt_ALU(char *codes, int **matriz,
			  const char *nombreMovimientos)
{
	FILE *movs = fopen(nombreMovimientos, "rt");
	if (!movs) {
		perror("Error al abrir el archivo de movimientos");
		return ERR_ARCHIVO;
	};
	char buffer[28];
	Movimiento mov;

	while (fgets(buffer, 28, movs)) {
		sscanf(buffer, "%11s%11s%d", mov.depOrigen, mov.depDestino,
		       &mov.cantidad);
		int indexX = indexOf(mov.depDestino, codes);
		int indexY = indexOf(mov.depOrigen, codes);
		int *dest = *(matriz + indexY) + indexX;
		*dest += mov.cantidad;
	}
	matrizMostrar(FILAS, COLUMNAS, matriz);
	return 0;
}

int indexOf(const char *c, const char *src)
{
	int index = 0;
	while (*src != '\0') {
		if (*src == *c) {
			return index;
		}
		index++;
		src++;
	}
	return -1;
}

int procesarMatrizMovimientosDepositosActualizandoStocks_ALU(
	int **matMovDeps, const Vector *codigosDepositos,
	const char *nombreStocks)
{
	return TODO_OK;
}

int crearVectorCodigosDepositos_ALU(const char *nombreStocks,
				    Vector *codigosDepositos)
{
    	int error = 0;
	FILE* f = fopen(nombreStocks, "rb");
	Stock stock;
	if(!f){
		printf("\nERROR: No se ha podido abrir el archivo \"%s\".", nombreStocks);
		return ERR_ARCHIVO;
	}

	error = vectorCrear_ALU(codigosDepositos, sizeof(stock.codDeposito));
	if(error != TODO_OK){
		printf("\nERROR: No se ha podido crear el vector [crearVectorCodigosDepositos_ALU].");
		return error;
	}

	fread(&stock, sizeof(stock), 1, f);
	while(!feof(f)){
		vectorOrdInsertarCDup(codigosDepositos, stock.codDeposito, cmpCodDep_ALU);
		fread(&stock, sizeof(stock), 1, f);
	}

	return TODO_OK;
}

int cmpCodDep_ALU(const void *e1, const void *e2)
{
	return 0;
}

// Réplicas de Vector.h

int vectorCantidadElementos_ALU(const Vector *v)
{
	return 0;
}

int vectorCargarDeArchivo_ALU(const char *nombreArch, Vector *vector,
			      size_t tamElem)
{
	return TODO_OK;
}

int vectorOrdBuscar_ALU(const Vector *v, void *elem, Cmp cmp)
{
	return 0;
}

int vectorCrear_ALU(Vector *vec, size_t tamElem)
{
	return TODO_OK;
}

int vectorInsertarAlFinal_ALU(Vector *vector, const void *elem)
{
	return TODO_OK;
}

void vectorDestruir_ALU(Vector *vec)
{
}

int vectorOrdInsertarCDup_ALU(Vector *vector, const void *elem, Cmp cmp)
{
	return TODO_OK;
}

// Réplicas de Matriz.h
void matrizMostrar_ALU(int fil, int col, int **mat)
{
}

void **matrizCrear_ALU(int fil, int col, size_t tamElem)
{
	return 0;
}

void matrizDestruir_ALU(void **mat, int fil)
{
}

// Funciones extra (no presentes en los .h dados)

void actualizarStocks(int **matriz, char *nombreStocks)
{
	Vector stocks;
	Stock stock;
	vectorCrear(&stocks, sizeof(Stock));
	vectorCargarDeArchivo(nombreStocks,&stocks,sizeof(Stock));
	vectorRecorrer(&stocks, modificarCant, (void *)matriz);
	for (int i = 0; i < 4; i++) {
		Stock *s = &((Stock *)stocks.vec)[i];
		printf("%11s %d\n", s->codDeposito, s->stock);
	}
}

void guardarEnArchivo(void *elem, void *extra)
{
	Stock *s = (Stock *)elem;
	FILE *f = (FILE *)extra;
	if (f) {
		fwrite(s, sizeof(Stock), 1, f);
	} else {
		printf("%11s %d\n", s->codDeposito, s->stock);
	};
}
void modificarCant(void *elem, void *extra)
{
	Stock *s = (Stock *)elem;
	int **m = (int **)extra; //matriz
	int index = indexOf(s->codDeposito, CODES);
	int *vFila = *(m + index);

	//sumar
	for (int i = 0; i < 4; i++) {
		s->stock += *(*(m + i) + index) -*(vFila + i) ;

	}
}

void *vectorObtenerElem_ALU(const Vector *v, int indice)
{
	return 0;
}
