#include <stddef.h>
#include <stdlib.h>
#include "vector.h"
#include "string.h"
#include "archivos.h"

int convTxtABin(const char *bin, const char *txt, size_t tamReg,
		TxtABin TxtABin, EsErrorFatal errorFatal)
{
	void *reg = malloc(1024);
	if (reg == NULL) {
		return ERR_MEM;
	}
	FILE *archTxt = fopen(txt, "rt");
	if (archTxt == NULL) {
		free(reg);
		return ERR_ARCH;
	}
	FILE *archBin = fopen(bin, "wb");
	if (archBin == NULL) {
		free(reg);
		fclose(archTxt);
		return ERR_ARCH;
	}
	void *linea = malloc(tamReg);
	int ret = OK;
	while (!errorFatal(ret) && fgets(linea, tamReg, archTxt) != NULL) {
		ret = TxtABin(linea, reg);
		if (ret == OK) {
			fwrite(reg, tamReg, 1, archBin);
		}
	}
	fclose(archBin);
	fclose(archTxt);
	free(reg);
	return OK;
}
int convBinATexto(const char *bin, const char *txt, size_t tamReg,
		  BinATxt BinATxt)
{
	void *reg = malloc(1024);
	if (reg == NULL) {
		return ERR_MEM;
	}
	FILE *archBin = fopen(bin, "rb");
	if (archBin == NULL) {
		free(reg);
		return ERR_ARCH;
	}
	FILE *archTxt = fopen(txt, "wt");
	if (archTxt == NULL) {
		free(reg);
		fclose(archBin);
		return ERR_ARCH;
	}
	while (fread(reg, tamReg, 1, archBin)) {
		BinATxt(reg, archTxt);
	}
	fclose(archBin);
	fclose(archTxt);
	free(reg);
	return OK;
}
void cambiarExtension(const char *nomArch, const char *ext,
		      char *nomArchCambiado)
{
	cpyString(nomArchCambiado, nomArch);
	char *punto = buscarCharEnStringEnReversa(nomArchCambiado, '.');
	cpyString(punto + 1, ext);
}
int crearIndice(const char *nombArch, size_t tam, size_t tamRegIdx,
		CrearRegIdx crearRegIdx, Cmp cmpRegIdx)
{
	void *reg = malloc(tam);
	void *regIdx = malloc(tamRegIdx);
	FILE *f = fopen(nombArch, "rb");
	if (!f) {
		return ERR_ARCH;
	}
	Vector v;
	if (!vectorCrear(&v, tam)) {
		return ERR_MEM;
	}
	unsigned nroRegAct = 0;
	while (fread(reg, tam, 1, f)) {
		crearRegIdx(reg, nroRegAct, regIdx);
		vectorOrdInsertar(&v, regIdx, cmpRegIdx);
		nroRegAct++;
	}
	fclose(f);
	char nombArchIdx[201];
	cambiarExtension(nombArch, ".idx", nombArchIdx);
	return 1;
}
