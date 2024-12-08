#include "myShortCuts.h"
#include <stdio.h>
#include <string.h>
#define dir "../archivosParaEjercicios/"
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
int Buscar2(FILE*, int, int);
void ActualizarStock(FILE*, float[]);
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
void ContabilizarGastos(int[], char[], Costos, FILE*);
void VerGastos(FILE*);
void CrearArchivos(Llamada, Costos);
void ej3();
/****************************************************/
typedef struct {
	int numInt;
	int numTarjeta;
	char code[11];
	int dia;
	int hora;
} SUBE;
typedef struct {
	int mes;
	char code[11];
	float price;
} TARIFA;
typedef struct {
	int numInt;
	int viajes;
	float importe;
} RECAUDACION;
TARIFA BuscarTarifa(char[], FILE*, TARIFA*);
int IngresoTarifa(TARIFA*, FILE*, float*, int, char[8][11]);
void GenerarArchivo4(RECAUDACION*, FILE*);
void ej4();
/****************************************************/
typedef struct {
	char code[11]; // ordenado
	int puerta;
	int cEsp;
} eventos;
typedef struct {
	int puerta;
	int cEsp;
} puertas;
void ej5();
void grabarArchivo5(FILE*, int[]);
/****************************************************/
typedef struct {
	int dia;
	int mes;
	int anio;
} Fecha;
typedef struct {
	char NyA[36];
	int dni;
	char localidad[41];
	char email[21];
	Fecha fecha;
} visitante;
void ej6();
void IngresarVisitante(FILE*);
void OrdenarPorLocalidad(visitante[], int);
void visitasRecibidas(FILE*);
/****************************************************/
typedef struct {
	int dia;//ordenado
	int hora;//ordenado
	int numBarrera;
}Barreras;
typedef struct {
	int dia;
	float dinero;
}Recaudacion;
void ej7();
void GuardarRecaudacion6(FILE*, int, int, int[], int);
/****************************************************/
typedef struct {
	int code;
	float price;
}ARTICULO;
typedef struct {
	char nombre[31];
	int numProv;
}PROVEEDOR;
typedef struct {
	char region[4];//ordenado
	char nombre[31];
	int mes;
	int code;
	int cant;
}VENTAS;
void ej8();
int buscarProvi8(FILE*, char[], int*);
float buscarPrecio8(FILE*, char[]);
void VentasConsecutivas(int[][12]);
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
	case 4:
		ej4();
		break;
	case 5:
		ej5();
		break;
	case 6:
		ej6();
		break;
	case 7:
		ej7();
		break;
	case 8:
		ej8();
		break;

	default:
		printf("Opcion no valida\n");
		break;
	}
	return 0;
}
void ej1() {
	Programa prog, * p;
	Promedio prom, * pm;
	FILE* f = fopen(dir "7.1/RATING.dat", "r+b"),
		* f2 = fopen(dir "7.1/PROM_RATING.dat", "wb"); // ordenado por canal
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
	Empresa emp, * e;
	Producto prod, * p;
	FILE* f = fopen(dir "7.2/ventas.dat", "rb"),
		* f2 = fopen(dir "7.2/productos.dat", "r+b");
	e = &emp;
	p = &prod;
	int pos;
	char maxSuc[16], sucAnt[16];
	float prodPrice, totalVenta[200] = { 0 }, totalEmpresa = 0, maxVenta, totalSuc;
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
int Buscar2(FILE* f, int code, int cant) {
	Producto prod, * p;
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
void ActualizarStock(FILE* f, float totalVenta[]) {
	Producto prod, * p;
	FILE* f3 = fopen(dir "7.2/pedidos.dat", "wb");
	Pedido ped, * pd;
	pd = &ped;
	p = &prod;
	int i = 0;
	float* v = &totalVenta[0];
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
	Llamada LLam, * l;
	Costos costos, * cos;
	FILE* f = fopen(dir "7.3/LLAMADAS.dat", "rb"),
		* f2 = fopen(dir "7.3/COSTOS.dat", "rb"),
		* f3 = fopen(dir "7.3/GASTOS.dat", "w+b");
	char sectorAnt[16], sectorCharlatan[16];
	int lo = 0, ld = 0, c = 0, maxTemp = 0, temp, tl[3];
	l = &LLam;
	cos = &costos;
	if (!f || !f2 || !f3)
		Err("Error al abrir el archivo");
	fread(cos, sizeof(Costos), 1, f2);
	fread(l, sizeof(Llamada), 1, f);
	strcpy(sectorCharlatan, l->sector);
	printf("hola");
	char dire[500];
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
void ContabilizarGastos(int tl[], char sector[], Costos costos, FILE* f) {
	gastos gas, * g;
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
void VerGastos(FILE* f) {
	gastos gas, * g;
	g = &gas;
	rewind(f);
	fread(g, sizeof(gastos), 1, f);
	printf("Sector\tCosto\n");
	while (!feof(f)) {
		printf("%s\t%.2f\n", g->sector, g->costo);
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
	LlamadaSector lls, * ls;
	ls = &lls;
	FILE* f2 = fopen(dire, "a+b");
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
void ej4() {
	int cont, i, numIntAnt = 0, mes, min = 0, b = 0, intMin = 0;
	char codes[8][11];
	SUBE viaje, * v;
	TARIFA est_tar, * est_pt, new_tar;
	RECAUDACION recaudacion, * rec;
	float tarifa[8] = { 0 }, * t, importeFinal = 0;
	FILE* f = fopen(dir "7.4/viajes.dat", "r+b"),
		* f2 = fopen(dir "7.4/tarifas.dat", "r+b"),
		* f3 = fopen(dir "7.4/recaudacion.dat", "wb");

	if (!f || !f2 || !f3) {
		Err("Error al abrir el archivo");
	}
	v = &viaje;
	rec = &recaudacion;
	est_pt = &est_tar;
	t = &tarifa[0];
	do {
		printf("Ingese mes a consultar: ");
		scanf("%d", &mes);
	} while (mes < 1 || mes > 12);

	cont = IngresoTarifa(est_pt, f2, t, mes, codes);
	if (cont == 0) {
		Err("No hay tarifas");
	}
	else {
		for (i = 0; i < cont; i++) {
			printf("Tarifa %s del mes: %.2f\n", codes[i], tarifa[i]);
		}
	}
	fread(v, sizeof(SUBE), 1, f);
	while (!feof(f)) {
		numIntAnt = v->numInt;
		rec->numInt = numIntAnt;
		new_tar = BuscarTarifa(v->code, f2, est_pt);
		do {
			rec->viajes++;
			rec->importe += new_tar.price;
			fread(v, sizeof(SUBE), 1, f);

		} while (v->numInt == numIntAnt && !feof(f));
		if (b == 0) {
			min = rec->viajes;
			intMin = numIntAnt;
			b = 1;
		}
		else {
			if (rec->viajes < min) {
				min = rec->viajes;
				intMin = numIntAnt;
			}
		}

		GenerarArchivo4(rec, f3);
		printf("Importe final cobrado por %d: %.2f\n", numIntAnt, importeFinal);
		fread(v, sizeof(SUBE), 1, f);
	}
	printf("Numero de intelectual con menos viajes: %d\n", intMin);
	fclose(f3);
	fclose(f2);
	fclose(f);
}
int IngresoTarifa(TARIFA* tar, FILE* F, float t[], int mes, char c[8][11]) {
	int i = 0;
	fread(tar, sizeof(TARIFA), 1, F);
	while (!feof(F) && i < 8) {
		if (tar->mes == mes) {
			*t = tar->price;
			strcpy(c[i], tar->code);
			t++;
			i++;
		}
		fread(tar, sizeof(TARIFA), 1, F);
	}
	return i;
}
TARIFA BuscarTarifa(char code[], FILE* f, TARIFA* tar) {
	TARIFA tari;
	tari.price = 0;
	fread(tar, sizeof(TARIFA), 1, f);
	while (!feof(f) && tari.price == 0) {
		if (strcmp(tar->code, code) == 0) {
			tari = *tar;
		}
		fread(tar, sizeof(TARIFA), 1, f);
	}
	return tari;
}
void GenerarArchivo4(RECAUDACION* rec, FILE* f) {
	fwrite(rec, sizeof(RECAUDACION), 1, f);
}
/****************************************************/
void ej5() {
	FILE* f = fopen(dir "7.5/eventos.dat", "rb"),
		* f2 = fopen(dir "7.5/puertas.dat", "wb");
	if (!f || !f2)
		Err("Error al abrir el archivo");
	eventos ev;
	char codeAnt[11], code[11];
	int cEsp[15] = { 0 }, i, cEspxEv, min, b = 0;
	fread(&ev, sizeof(eventos), 1, f);
	min = ev.cEsp;
	while (!feof(f)) {
		cEspxEv = 0;
		strcpy(codeAnt, ev.code);
		do {
			cEsp[ev.puerta - 1] += ev.cEsp;
			cEspxEv += ev.cEsp;
			printf("puerta: %d\n", ev.puerta);
			printf("cEsp: %d\n", cEsp[0]);
			fread(&ev, sizeof(eventos), 1, f);
		} while (!feof(f) && strcmp(codeAnt, ev.code) == 0);
		printf("Cantidad de espectadores en el evento %s: %d\n", codeAnt, cEspxEv);
		if (b == 0) {
			min = cEspxEv;
			strcpy(code, ev.code);
			b = 1;
		}
		else {
			if (cEspxEv < min) {
				min = cEspxEv;
				strcpy(code, ev.code);
			}
		}
	}
	printf("Evento con menor cantidad de expectadores: %s\n", code);
	grabarArchivo5(f2, cEsp);
	fclose(f);
	fclose(f2);
}
void grabarArchivo5(FILE* f, int esp[]) {
	puertas pu;
	int i;
	for (i = 1; i <= 15; i++) {
		pu.puerta = i;
		pu.cEsp = esp[i - 1];
		printf("Puerta %d: %d\n", pu.puerta, pu.cEsp);
		fwrite(&pu, sizeof(puertas), 1, f);
	}
}
/****************************************************/
void ej6() {
	visitante vis;
	FILE* f = fopen(dir "7.6/visitantes.dat", "w+b");
	if (!f)
		Err("Error al abrir el archivo");
	IngresarVisitante(f);
	visitasRecibidas(f);
	fclose(f);
}
void IngresarVisitante(FILE* f) {
	visitante vis[500];
	char NyA[36], localidad[41], email[21];
	int i = 0;
	printf("Ingrese Nombre y Apellido del visitante: ");
	fsgets(NyA, 36);
	while (strcmp(NyA, "FIN") != 0 && i < 500) {
		strcpy(vis[i].NyA, NyA);
		printf("Ingrese DNI del visitante: ");
		scanfInt(&vis[i].dni, 1000000, 99999999, 99999999);
		printf("Ingrese localidad del visitante: ");
		fsgets(localidad, 41);
		strcpy(vis[i].localidad, localidad);
		printf("Ingrese email del visitante: ");
		fsgets(email, 21);
		strcpy(vis[i].email, email);
		printf("Ingrese dia de la visita: ");
		scanfInt(&vis[i].fecha.dia, 1, 31, 31);
		scanfInt(&vis[i].fecha.mes, 1, 12, 12);
		scanfInt(&vis[i].fecha.anio, 1900, 2024, 2024);
		i++;
	}
	OrdenarPorLocalidad(vis, i);
	fwrite(vis, sizeof(visitante), i, f);
}
void OrdenarPorLocalidad(visitante vis[], int i) {
	visitante aux;
	int j, k;
	for (j = 0; j < i; j++) {
	}
	for (k = 0; k < i; k++) {
		if (strcmp(vis[j].localidad, vis[k].localidad) < 0) {
			aux = vis[j];
			vis[j] = vis[k];
			vis[k] = aux;
		}
	}
}
void visitasRecibidas(FILE* f) {
	FILE* f2;
	visitante vis;
	int c;
	char loAnt[41], dire[1000];
	strcpy(dire, dir);
	strcat(dire, "7.6/");
	fread(&vis, sizeof(visitante), 1, f);
	while (!feof(f)) {
		strcpy(loAnt, vis.localidad);
		c = 0;
		do {
			c++;
			fread(&vis, sizeof(visitante), 1, f);
		} while (!feof(f) && strcmp(loAnt, vis.localidad) == 0);
		printf("Localidad: %s\t", loAnt);
		printf("Cantidad de visitas: %d\n", c);
		strcat(dire, loAnt);
		strcat(dire, ".dat");
		f2 = fopen(dire, "wb");
		fwrite(&c, sizeof(int), 1, f2);
		fclose(f2);
	}
}
/****************************************************/
void ej7() {
	FILE* f = fopen(dir "7.7/barreras.dat", "rb"), * f2 = fopen(dir "7.7/recaudacion.dat", "wb");
	if (!f) Err("Error al abrir el archivo");
	Barreras bar;
	int dAnt, hAnt, cDia, cHora[24] = { 0 }, Hpico, Hnormal;
	fread(&bar, sizeof(Barreras), 1, f);
	printf("Ingrese precios de horario normal y pico:");
	scanfInt(&Hnormal, 0, 999999, 999999);
	scanfInt(&Hpico, 0, 999999, 999999);
	while (!feof(f)) {
		dAnt = bar.dia;
		cDia = 0;
		for (int i = 0; i < 24; i++) {
			cHora[i] = 0;
		}
		do {
			hAnt = bar.hora;
			cDia++;
			do {
				printf("Dia: %d\tHora: %d\n", bar.dia, bar.hora);
				cHora[bar.hora]++;
				fread(&bar, sizeof(Barreras), 1, f);
			} while (!feof(f) && bar.hora == hAnt);
			fread(&bar, sizeof(Barreras), 1, f);
		} while (!feof(f) && bar.dia == dAnt);
		printf("Cantidad de autos por hora\n");
		for (int i = 0; i < 24; i++) {
			printf("Hora: %d\tCantidad: %d\n", i, cHora[i]);
		}
		printf("Cantidad de autos por dia: %d\n", cDia);
		GuardarRecaudacion6(f2, Hpico, Hnormal, cHora, dAnt);
		fread(&bar, sizeof(Barreras), 1, f);
	}
}
void GuardarRecaudacion6(FILE* f, int Hpico, int Hnormal, int cHora[], int dAnt) {
	Recaudacion rec;
	float total = 0;
	for (int i = 0; i < 24; i++) {
		if (i >= 7 && i <= 11) {
			total += cHora[i] * Hnormal;
		}
		else if (i >= 17 && i <= 19) {
			total += cHora[i] * Hpico;
		}
	}
	rec.dia = dAnt;
	rec.dinero = total;
	fwrite(&rec, sizeof(Recaudacion), 1, f);
}
/****************************************************/
void ej8() {
	FILE* f = fopen(dir "7.8/articulos.dat", "rb"),
		* f2 = fopen(dir "7.8/proveedores.dat", "r+b"),
		* f3 = fopen(dir "7.8/ventas.dat", "r+b");
	if (!f || !f2 || !f3) Err("Error al abrir el archivo");
	ARTICULO art;
	PROVEEDOR prov;
	VENTAS ven;
	int provi, a = 0, pos, vVendedor[30][12] = { 0 };
	float ventas[24][12] = { 0 }, price, cVen, vMin;
	char regAnt[4], regMin[4];
	fread(&ven, sizeof(VENTAS), 1, f3);
	while (!feof(f3)) {
		strcpy(regAnt, ven.code);
		cVen = 0;
		do {
			provi = bucarProvi8(f2, ven.nombre, &pos);
			price = buscarPrecio8(f, ven.code);
			if (price != -1 && provi != -1) {
				ventas[provi - 1][ven.mes - 1] += ven.cant * price;
				cVen += ven.cant * price;
			}
			vVendedor[pos - 1][ven.mes - 1] = 1;
			fread(&ven, sizeof(VENTAS), 1, f3);
		} while (!feof(f3) && strcmp(regAnt, ven.code) == 0);
		if (a == 0) {
			vMin = cVen;
			strcpy(regMin, regAnt);
			a = 1;
		}
		else {
			if (vMin < cVen) {
				vMin = cVen;
				strcpy(regMin, regAnt);
			}
		}
		fread(&ven, sizeof(VENTAS), 1, f3);
	}
	printf("Region con menor venta: %s\n", regMin);
	VentasConsecutivas(vVendedor);
	Informar(ventas);
	fclose(f);
	fclose(f2);
	fclose(f3);
}
int buscarProvi8(FILE* f, char nom[], int* pos) {
	int provi = -1;
	rewind(f);
	PROVEEDOR prov;
	fread(&prov, sizeof(PROVEEDOR), 1, f);
	while (!feof(f) && provi == -1) {
		if (strcmp(prov.nombre, nom) == 0) {
			provi = prov.numProv;
			*pos = ftell(f) / sizeof(PROVEEDOR);
		}
		fread(&prov, sizeof(PROVEEDOR), 1, f);
	}
	return provi;
}
float buscarPrecio8(FILE* f, char code[]) {
	float price = -1;
	rewind(f);
	ARTICULO art;
	fread(&art, sizeof(ARTICULO), 1, f);
	while (!feof(f) && price == -1) {
		if (strcmp(art.code, code) == 0) {
			price = art.price;
		}
		fread(&art, sizeof(ARTICULO), 1, f);
	}
	return price;
}
void VentasConsecutivas(int v[][12]) {//3 meses consecutivos
	int i, j, c;
	for (i = 0; i < 30; i++) {
		c = 0;
		for (j = 0; j < 12; j++) {
			if (v[i][j] == 1 && v[i][j + 1] == 1 && v[i][j + 2] == 1) {
				c++;
			}
		}
		printf("Vendedor %d: %d\n", i + 1, c);
	}
}