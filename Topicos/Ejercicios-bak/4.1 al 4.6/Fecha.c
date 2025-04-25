#include "Fecha.h"
#include <stdbool.h>
#include <stdio.h>

// primitiva
Fecha ingresarFecha() {
  Fecha f;
  int d, m, a;
  printf("Ingrese la fecha (dd mm aaaa): ");
  scanf("%d / %d / %d", &d, &m, &a);
  setFecha(&f, d, m, a);
  return f;
}
// primitiva
bool setFecha(Fecha *f, int d, int m, int a) {
  if (!esFechaValida(d, m, a)) {
    return false;
  }
  f->dia = d;
  f->mes = m;
  f->anio = a;
  return true;
}
// no primitiva
bool esFechaValida(int d, int m, int a) {
  if (a < 1601) {
    return false;
  }
  if (m < 1 || m > 12) {
    return false;
  }
  if (d < 1 || d > cDiaMes(m, a)) {
    return false;
  }
  return true;
}
// no primitiva
int cDiaMes(int m, int a) {
  if (m == 2) {
    return esBisiesto(a) ? 29 : 28;
  }
  if (m == 4 || m == 6 || m == 9 || m == 11) {
    return 30;
  }
  return 31;
}
// no primitiva
bool esBisiesto(int a) { return a % 4 == 0 && (a % 100 != 0 || a % 400 == 0); }
// primitiva
Fecha sumarDias(const Fecha *f, int d) {
  int cdm;
  Fecha fSig = *f;
  fSig.dia += d;

  while (fSig.dia > (cdm = cDiaMes(fSig.mes, fSig.anio))) {
    printf("%d\n", fSig.dia);
    fSig.dia -= cdm;
    fSig.mes++;
    if (fSig.mes > 12) {
      fSig.mes = 1;
      fSig.anio++;
    }
  }
  return fSig;
}

Fecha restarDias(const Fecha *f, int d) {
  int cdm;
  Fecha fSig = *f;
  fSig.dia += d;
  while (fSig.dia > (cdm = cDiaMes(fSig.mes, fSig.anio))) {
    fSig.dia -= cdm;
    fSig.mes--;
    if (fSig.mes < 1) {
      fSig.mes = 12;
      fSig.anio--;
    }
  }
  return fSig;
}

int getDiasDif(const Fecha *f1, const Fecha *f2) {
  int dif;
  if (f1->anio == f2->anio) {
    dif = (f1->mes - f2->mes) * 30;
    dif += f1->dia - f2->dia;
  } else {
    dif = (f1->anio - f2->anio) * 365;
    dif += (f1->mes - f2->mes) * 30;
    dif += (f1->dia - f2->dia);
  }
  return dif;
}
int getDiaSemana(const Fecha *f) {
  int a = f->anio;
  int m = f->mes;
  int k = a % 100;
  int j = a / 100;
  if (m < 3) {
    m += 12;
    a--;
  }
  int d = (f->dia + (13 * (m + 1)) / 5 + k + (k / 4) + (j / 4) - 2 * j);
  printf("%d %d %d", d, j, k);
  printf("\n%d+(13*(%d+1)/5)+%d+(%d/4)+(%d/4)-2*%d\n", f->dia, m, k, k, j, j);
  d = (d + 6) % 7;
  return d;
}
