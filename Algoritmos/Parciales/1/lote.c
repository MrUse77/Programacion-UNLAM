#include "lote.h"
#include "files.h"
#define TAM_COL 4

// 3 COLUMNAS * 4 CONTENEDER x COL = 12 REGISTROS de entrada y
void generarLote()
{
	lote_t lote_pruebas[12] = { { "IN", "10", "E1" },
				    { "EG", "10", "E1" },
				    { "IN", "1101", "BinGroup" },
				    { "EG", "1101", "BinGroup" },
				    { "IN", "ABC1234567", "TransArg" },
				    { "EG", "999", "ShippingCo" },
				    { "IN", "ID-MAX-10C", "MegaLogist" },
				    { "EG", "SHORT", "A" },
				    { "IN", "0000000001", "1234567890" },
				    { "EG", "0000000001", "1234567890" },
				    { "IN", "A1B2C3D4E5", "F6G7H8I9J0" },
				    { "EG", "L-99-Z", "T-Global" } };
	escribirLote("movimientos.txt", lote_pruebas, 12);
}
