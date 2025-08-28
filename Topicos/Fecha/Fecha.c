
#include "Fecha.h"
#include <stdio.h>

int cantDiasMes(int m, int a);
bool esFechaValida(int d, int m, int a);
int cantDiasAnio(int a);
bool esBisiesto(int a);
// Primitivas (si pueden setear/acceder a datos de la estructura)
void fechaSet(Fecha *f, int d, int m, int a)
{
	f->dia = d;
	f->mes = m;
	f->anio = a;
}
void fechaGet(const Fecha *f, int *d, int *m, int *a)
{
	*d = f->dia;
	*m = f->mes;
	*a = f->anio;
}
Fecha fechaSumarDias(const Fecha *f, int dias)
{
	Fecha fSuma = *f;

	fSuma.dia += dias;
	int cdm;
	while (fSuma.dia > (cdm = cantDiasMes(fSuma.mes, fSuma.anio))) {
		fSuma.dia -= cdm;
		fSuma.mes++;
		if (fSuma.mes > 12) {
			fSuma.anio++;
			fSuma.mes = 0;
		}
	}
	return fSuma;
}
int fechaDiferencia(const Fecha *f1, const Fecha *f2)
{
	int dif = 0;
	for (int anioAct = f1->anio; anioAct < f2->anio; anioAct++) {
		if (anioAct == f2->anio) {
			dif += f2->dia; // Otra forma (sirve si anioAct <= f2->anio)
		} else {
			dif += cantDiasAnio(anioAct);
		}
	}
	dif -= fechaDiaDelAnio(f1);
	dif += fechaDiaDelAnio(f2);

	/*
	 * Otra forma (en el for debe estar anioAct <= f2->anio) 
	 * (no combinar con el if del for)
	 * (no combinar con la suma de la linea de arriba)
	 * dif -= fechaDiasRestantesDelAnio(f2);
	*/
	return dif;
}
int fechaDiaDelAnio(const Fecha *f)
{
	int diaDelAnio = f->dia;
	for (int mesAct = 1; mesAct < f->mes; mesAct++) {
		diaDelAnio += cantDiasMes(mesAct, f->anio);
	}
	return diaDelAnio;
}
int fechaDiasRestantesDelAnio(const Fecha *f)
{
	int diasDelAnio = cantDiasAnio(f->anio);
	return diasDelAnio - f->dia;
}

// No Primitivas (no se pueden setear/acceder a datos de la estructura)
void ingresarFecha(Fecha *f)
{
	int d, m, a;
	puts("Ingrese una fecha(D/M/A):");
	scanf("%d/%d/%d", &d, &m, &a);
	while (!esFechaValida(d, m, a)) {
		puts("Fecha invalida, ingresela de nueva (D/M/A):");
		scanf("%d/%d/%d", &d, &m, &a);
	}
	fechaSet(f, d, m, a);
}
void mostrarFecha(const Fecha *f)
{
	int d, m, a;
	fechaGet(f, &d, &m, &a);
	printf("%02d/%02d/%04d", d, m, a);
}
int fechaDeTxt(const char *txt, Fecha *f)
{
	sscanf(txt, "%02d/%02d/%04d", &f->dia, &f->mes, &f->anio);

	return 1;
}

bool esFechaValida(int d, int m, int a)
{
	if (a < 1601) {
		return false;
	}

	if (m < 1 || m > 12) {
		return false;
	}

	if (d < 1 || d > cantDiasMes(m, a)) {
		return false;
	}

	return true;
}
int cantDiasMes(int m, int a)
{
	int diasMes[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (m == 2 && esBisiesto(a)) {
		return 29;
	}
	return diasMes[m];
}
bool esBisiesto(int a)
{
	return a % 4 == 0 && (a % 100 != 0 || a % 400 == 0);
}
int cantDiasAnio(int a)
{
	if (esBisiesto(a)) {
		return 366;
	} else {
		return 365;
	}
}
