#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define dir "../archivosParaEjercicios/"
void fsgets(char t[], int n) {
  int i;
  fgets(t, n, stdin);
  while (t[i] != '\0') {
    if (t[i] == '\n') {
      t[i] = '\0';
      break;
    }
  }
}
void Err(char t[]) {
  puts(t);
  exit(1);
}
/****************************************************/
typedef struct {
  int canal;
  char programa[36];
  float rating;
} Programa;
typedef struct {
  int canal;
  float promedio;
} Promedio;
void ej1();
/****************************************************/
typedef struct {
  char sucursal[16];
  int code;
  int cant;
} Empresa;
typedef struct {
  int code;
  char desc[21];
  float price;
  int stock;
  int puntoPedido;
  int cantPedido;
} Producto;
typedef struct {
  int code;
  int cantPedido;
} Pedido;
int Buscar2(FILE *, int, int);
void ActualizarStock(FILE *, float[]);
void ej2();
/****************************************************/
typedef struct {
  char sector[16];
  int duracion;
  int tipo;
} Llamada;
typedef struct {
  float ll;
  float ld;
  float li;
} Costos;
typedef struct {
  char sector[16];
  float costo;
} gastos;
typedef struct {
  int duracion;
  int tipo;
  float costo;
} LlamadaSector;
void ContabilizarGastos(int[], char[], Costos, FILE *);
void VerGastos(FILE *);
void CrearArchivos(Llamada, Costos);
void ej3();
/****************************************************/
int main() {
  int opcion;
  printf("Elija una opcion: \n");
  scanf("%d", &opcion);
  switch (opcion) {
  case 1:
    ej1();
    break;
  case 2:
    ej2();
    break;
  case 3:
    ej3();
    break;
  default:
    printf("Opcion no valida\n");
    break;
  }
  return 0;
}
void ej1() {
  Programa prog, *p;
  Promedio prom, *pm;
  FILE *f = fopen(dir "7.1/RATING.dat", "r+b"),
       *f2 = fopen(dir "7.1/PROM_RATING.dat", "wb"); // ordenado por canal
  int canalAnt = 0, maxCanal = 0, i;
  float n = 0, max = 0;
  p = &prog;
  pm = &prom;
  if (!f)
    Err("Error al abrir el archivo");
  printf("Canales con menos de 15 puntos de rating en total\n");
  fread(p, sizeof(Programa), 1, f);

  while (!feof(f)) {
    i = 0;
    n = 0;
    canalAnt = p->canal;
    do {
      n += p->rating;
      i++;
      fread(p, sizeof(Programa), 1, f);

    } while (canalAnt == p->canal && !feof(f));
    if (n < 15) {
      printf("Canal\tRating\n");

      printf("%d\t%.2f\n", p->canal, n);
    }
    if (n > max) {
      max = n;
      maxCanal = canalAnt;
    }
    pm->promedio = n / i;
    pm->canal = canalAnt;
    fwrite(pm, sizeof(Promedio), 1, f2);
    fread(p, sizeof(Programa), 1, f);
  }
  printf("Canal con mayor rating: %d\n", maxCanal);
  fclose(f);
  fclose(f2);
}
void ej2() {
  Empresa emp, *e;
  Producto prod, *p;
  FILE *f = fopen(dir "7.2/ventas.dat", "rb"),
       *f2 = fopen(dir "7.2/productos.dat", "r+b");
  e = &emp;
  p = &prod;
  int pos;
  char maxSuc[16], sucAnt[16];
  float prodPrice, totalVenta[200] = {0}, totalEmpresa = 0, maxVenta, totalSuc;
  if (!f || !f2)
    Err("Error al abrir el archivo");
  fread(e, sizeof(Empresa), 1, f);
  strcpy(maxSuc, e->sucursal);
  maxVenta = e->cant;
  while (!feof(f)) {
    strcpy(sucAnt, e->sucursal);
    do {
      pos = Buscar2(f2, e->code, e->cant);
      if (prodPrice != -1) {
        fseek(f2, sizeof(Producto) * (pos - 1), SEEK_SET);
        fread(p, sizeof(Producto), 1, f2);
        totalVenta[pos] += p->price * e->cant;
        totalSuc += p->price * e->cant;
        fread(e, sizeof(Empresa), 1, f);
      }
    } while (strcmp(sucAnt, e->sucursal) == 0 && !feof(f));
    if (totalSuc > maxVenta) {
      maxVenta = totalSuc;
      strcpy(maxSuc, sucAnt);
    }
    printf("Total de ventas de la sucursal %s: %.2f\n", sucAnt,
           totalVenta[pos]);
    totalEmpresa += totalVenta[pos];
    fread(e, sizeof(Empresa), 1, f);
  }
  printf("Sucursal con mayor venta: %s\n", maxSuc);
  printf("Total recaudado: %.2f\n", totalEmpresa);
  ActualizarStock(f2, totalVenta);
  fclose(f);
  fclose(f2);
}
int Buscar2(FILE *f, int code, int cant) {
  Producto prod, *p;
  p = &prod;
  int pos = -1;
  rewind(f);
  fread(p, sizeof(Producto), 1, f);
  while (!feof(f) && pos == -1) {
    if (p->code == code) {
      pos = ftell(f) / sizeof(Producto);
    }
    fread(p, sizeof(Producto), 1, f);
  }
  return pos;
}
void ActualizarStock(FILE *f, float totalVenta[]) {
  Producto prod, *p;
  FILE *f3 = fopen(dir "7.2/pedidos.dat", "wb");
  Pedido ped, *pd;
  pd = &ped;
  p = &prod;
  int i = 0;
  float *v = &totalVenta[0];
  rewind(f);
  fread(p, sizeof(Producto), 1, f);
  while (!feof(f)) {
    p->stock -= *v;
    if (p->stock < p->puntoPedido) {
      pd->code = p->code;
      pd->cantPedido = p->puntoPedido - p->stock;
      fwrite(pd, sizeof(Pedido), 1, f3);
    }
    fread(p, sizeof(Producto), 1, f);
    v++;
  }
}
void ej3() {
  Llamada LLam, *l;
  Costos costos, *cos;
  FILE *f = fopen(dir "7.3/LLAMADAS.dat", "rb"),
       *f2 = fopen(dir "7.3/COSTOS.dat", "rb"),
       *f3 = fopen(dir "7.3/GASTOS.dat", "w+b");
  char sectorAnt[16], sectorCharlatan[16];
  int lo = 0, ld = 0, c = 0, maxTemp = 0, temp, tl[3];
  l = &LLam;
  if (!f || !f2 || !f3)
    Err("Error al abrir el archivo");
  fread(cos, sizeof(Costos), 1, f2);
  fread(l, sizeof(Llamada), 1, f);
  strcpy(sectorCharlatan, l->sector);
  printf("hola");
  char dire[500];
  strcpy(dire, dir);
  strcat(dire, "7.3/");
  strcat(dire, l->sector);
  strcat(dire, ".dat");
  printf("%s\n", dire);
  while (!feof(f)) {
    strcpy(sectorAnt, l->sector);
    lo = 0;
    ld = 0;
    c = 0;
    temp = 0;
    do {
      switch (l->tipo) {
      case 1:
        lo++;
        tl[0] = l->duracion;
        break;
      case 2:
        ld++;
        tl[1] = l->duracion;
        break;
      case 3:
        c++;
        tl[2] = l->duracion;
        break;
      }
      temp += l->duracion;
      CrearArchivos(LLam, costos);
      fread(l, sizeof(Llamada), 1, f);
    } while (strcmp(sectorAnt, l->sector) == 0 && !feof(f));
    printf("Sector: %s\n", sectorAnt);
    printf("llamadas locales: %d\n", lo);
    printf("llamadas de larga distancia: %d\n", ld);
    printf("llamadas internacionales: %d\n", c);
    if (temp > maxTemp) {
      maxTemp = temp;
      strcpy(sectorCharlatan, sectorAnt);
    }
    ContabilizarGastos(tl, sectorAnt, costos, f3);
    fread(l, sizeof(Llamada), 1, f);
  }
  VerGastos(f3);
  fclose(f);
  fclose(f2);
  fclose(f3);
  printf("Sector con mayor duracion de llamadas: %s\n", sectorCharlatan);
}
void ContabilizarGastos(int tl[], char sector[], Costos costos, FILE *f) {
  gastos gas, *g;
  g = &gas;
  strcpy(g->sector, sector);
  float total = 0;
  for (int i = 0; i < 3; i++) {
    switch (i) {
    case 0:
      total += tl[i] * costos.ll;
      break;
    case 1:
      total += tl[i] * costos.ld;
      break;
    case 2:
      total += tl[i] * costos.li;
      break;
    }
  }
  g->costo = total;
  fwrite(g, sizeof(gastos), 1, f);
}
void VerGastos(FILE *f) {
  gastos gas, *g;
  g = &gas;
  rewind(f);
  fread(g, sizeof(gastos), 1, f);
  printf("Sector\tCosto\n");
  while (!feof(f)) {
    printf("%s\t%.2f", g->sector, g->costo);
    fread(g, sizeof(gastos), 1, f);
  }
}
void CrearArchivos(Llamada l, Costos cos) {
  char sector[16];
  char dire[100];
  char arch[50];
  strcpy(dire, dir);
  strcat(dire, "7.3/");
  strcat(dire, l.sector);
  strcat(dire, ".dat");
  printf("%s\n", dire);
  LlamadaSector lls, *ls;
  FILE *f2 = fopen(dire, "a+b");
  if (!f2)
    Err("Error al abrir el archivo");
  switch (l.tipo) {
  case 1:
    ls->costo = l.duracion * cos.ll;
    break;
  case 2:
    ls->costo = l.duracion * cos.ld;
    break;
  case 3:
    ls->costo = l.duracion * cos.li;
    break;
  }
  ls->duracion = l.duracion;
  ls->tipo = l.tipo;
  fwrite(&ls, sizeof(Costos), 1, f2);
  fclose(f2);
}
