#include "myShortCuts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TAM6 4
#define CANT_CARRERAS 3

typedef struct {
	char code[6];
	float price;
	char description[30];
	int unidadesVendidas;
	float total;
} product;

void ej1();
int Buscar1(product[], char[], int);

typedef struct {
	int code;
	float price;
	int stock;
} medicamentos;

void ej2();
void INGRESO2(medicamentos[]);
void BUSQUEDA_MEDI(medicamentos[]);
void INFORME(medicamentos[]);
int BuscarMedicamentos2(medicamentos[], int, int);

typedef struct {
	int dni;
	char name[81];
	int nota1;
	int nota2;
	float promedio;
} alumno;
void ej3();
int INGRESO3(alumno[]);
void RESULTADO3(alumno[], int);
void INFORME_PROM(alumno[], int);
int BuscarAlumno(alumno[], int, int);

typedef struct {
	int cliente;
	int importe;
	int vendedor;
} venta4;
typedef struct {
	int code;
	char NyA[51];

} cliente4;
void ej4();
int CARGA_CLIENTE(int, char[], cliente4[]);
int EsCorrelativo4(int, int);
typedef struct {
	int legajo;
	char apellido[16];
	char nombre[16];
	float altura;
	char Sexo;
	int DiaNacimiento;
	int MesNacimiento;
	int AnioNacimiento;
} persona5;
typedef struct {
	int DiaNacimiento;
	int MesNacimiento;
	int AnioNacimiento;
} FechaNacimiento;
typedef struct {
	char Nombre[16];
	char Apellido[16];
} NomCompleto;
typedef struct {
	NomCompleto NyA;
	FechaNacimiento fecha;
	int legajo;
	float altura;
	char Sexo;
	int code;
} persona51;
typedef struct {
	NomCompleto NyA;
	FechaNacimiento fecha;
	int legajo;
	float altura;
	char Sexo;
	int code;
	int MateriasAprobadas[36];

} persona52;
void ej5();
persona51 CargaPersona2(persona5);
void CargaPersona3(persona51, persona52);
void MostrarPersona3(persona52, int);
typedef struct {
	char nombre[30];
	char escuderia[30];
	int posicion[CANT_CARRERAS];
} piloto;
void ej6();
void MostrarCarrera(piloto[], int);

int main() {
	int opcion;
	printf("EJERCICIOS DE STRUCT\n");
	printf("Elija el ejercicio que desea realizar: \n");
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
	}
	return 0;
}
/*
 * */
void ej1() {
	int i = 0, j = 2, unidades, venta[2] = { 0 };
	float total[2] = { 0 };
	product productos[2] = { 0 };
	char fin[] = "fin", code[6];
	do {
		do {
			printf("Ingrese el codigo del producto: ");
			flush(stdin);
			fsgets(code, 6);
		} while (Buscar1(productos, code, j) != i);
		strcpy(productos[i].code, code);
		printf("Ingrese el precio del producto: ");
		scanf("%f", &productos[i].price);
		do {
			printf("Ingrese la descripcion del producto: ");
			flush(stdin);
			fsgets(productos[i].description, 30);

		} while (strlen(productos[i].description) > 30);
		printf("Ingrese la cantidad de unidades vendidas: ");
		scanf("%d", &productos[i].unidadesVendidas);
		// convertir int en float para que no haya problemas con la multiplicacion
		productos[i].total =
			productos[i].price * (float)productos[i].unidadesVendidas;
		i++;

	} while (i != j && strcmp(productos[i - 1].description, fin) != 0);
	do {
		int codigoProducto = 0;
		do {
			printf("Ingrese Codigo de producto");
			flush(stdin);
			fsgets(code, 6);
			codigoProducto = Buscar1(productos, code, j);
		} while (codigoProducto == -1);
		printf("Ingrese cantidad de unidades vendidas");
		scanf("%d", &unidades);
		venta[codigoProducto] += unidades;
		total[codigoProducto] +=
			productos[codigoProducto].price * venta[codigoProducto];
	} while (unidades != 0);
	for (i = 0; i < j; i++) {
		productos[i].unidadesVendidas = venta[i];
		productos[i].total = total[i];
	}
	printf("DESCRIPCION \t CANTIDAD DE UNIDADES VENDIDAS \t IMPORTE TOTAL "
				 "VENDIDO\n");
	for (i = 0; i < j; i++) {
		printf("%s \t %d \t %f\n", productos[i].description,
					 productos[i].unidadesVendidas, productos[i].total);
	}
}
int Buscar1(product productos[], char code[6], int j) {
	int i = 0;
	while (i < j) {
		if (strcmp(productos[i].code, code) == 0) {
			return i;
		}
		i++;
	}
	return -1;
}
void ej2() {
	medicamentos medicamento[50];
	srand((float)time(NULL));
	INGRESO2(medicamento);
	// system("cls");
	BUSQUEDA_MEDI(medicamento);
	INFORME(medicamento);
}
void INGRESO2(medicamentos medicamento[]) {
	int i = 0, code, stock;
	float price;
	for (i = 0; i < 50; i++) {
		// nuemro random, positivo y menor a 100
		code = rand() % 100;
		// numero random, positivo, real y menor a 350
		price = (float)rand() / (float)350;
		stock = rand() % 100;
		printf("Ingrese el codigo del medicamento: \n");
		medicamento[i].code = code; // scanf("%d", &medicamento[i].code);
		printf("%d\n", medicamento[i].code);
		printf("Ingrese el precio del medicamento: \n");
		medicamento[i].price = price; // scanf("%f", &medicamento[i].price);
		printf("%.2f\n", medicamento[i].price);
		printf("Ingrese el stock del medicamento: \n");
		medicamento[i].stock = stock; // scanf("%d", &medicamento[i].stock);
		printf("%d\n", medicamento[i].stock);
	}
}
void BUSQUEDA_MEDI(medicamentos medicamento[]) {
	int i = 0, codigo;
	do {
		printf("Ingrese el codigo del medicamento que desea buscar: ");
		scanf("%d", &codigo);
		i = BuscarMedicamentos2(medicamento, codigo, 50);
		if (i == -1) {
			printf("No se encontro el medicamento\n");
		}
		else {
			printf("CODIGO \t PRECIO \t STOCK\n");
			printf("%d \t %f \t %d\n", medicamento[i].code, medicamento[i].price,
						 medicamento[i].stock);
		}

	} while (codigo > 0 || i == -1);
}
void INFORME(medicamentos medicamento[]) {
	int i;
	printf("CODIGO \t PRECIO \t STOCK\n");
	for (i = 0; i < 50; i++) {
		if (medicamento[i].stock < 10) {
			printf("%d \t %.2f \t %d\n", medicamento[i].code, medicamento[i].price,
						 medicamento[i].stock);
		}
	}
}
int BuscarMedicamentos2(medicamentos medicamento[], int codigo, int j) {
	int i = 0;
	while (i < j) {
		if (medicamento[i].code == codigo) {
			return i;
		}
		i++;
	}
	return -1;
}
void ej3() {
	alumno alumnos[81];
	int i = 0;
	i = INGRESO3(alumnos);
	system("cls");
	RESULTADO3(alumnos, i);
	INFORME_PROM(alumnos, i);
}
int INGRESO3(alumno alumnos[]) {
	int i = 0;
	do {
		do {
			printf("Ingrese el DNI del alumno: ");
			scanf("%d", &alumnos[i].dni);
			// con 0 termina
		} while (BuscarAlumno(alumnos, alumnos[i].dni, i) != -1);

		printf("Ingrese el nombre del alumno: ");
		fsgets(alumnos[i].name, 80);
		do {
			printf("Ingrese la nota 1 del alumno: ");
			scanf("%d", &alumnos[i].nota1);
		} while (alumnos[i].nota1 < 0 || alumnos[i].nota1 > 10);
		do {
			printf("Ingrese la nota 2 del alumno: ");
			scanf("%d", &alumnos[i].nota2);
		} while (alumnos[i].nota2 < 0 || alumnos[i].nota2 > 10);
		alumnos[i].promedio = (float)(alumnos[i].nota1 + alumnos[i].nota2) / 2;
		i++;
	} while (alumnos[i - 1].dni > 0 && i < 80);
	return i;
}
void RESULTADO3(alumno alumnos[], int cant) {
	int i, j = 0, k = 0;
	alumno aprobados[cant], reprobados[cant];
	for (i = 0; i < cant; i++) {
		if (alumnos[i].nota1 >= 4 && alumnos[i].nota2 >= 4) {
			aprobados[j] = alumnos[i];
			j++;
		}
		else {
			reprobados[k] = alumnos[i];
			k++;
		}
	}
	printf("APROBADOS\n");
	printf("\t DNI \t NOMBRE \t NOTA 1 \t NOTA 2 \t PROMEDIO\n");
	for (i = 0; i < j; i++) {
		printf("%d \t %s \t %d \t %d \t %.2f\n", aprobados[i].dni,
					 aprobados[i].name, aprobados[i].nota1, aprobados[i].nota2,
					 aprobados[i].promedio);
	}
	printf("REPROBADOS\n");
	printf("\t DNI \t NOMBRE \t NOTA 1 \t NOTA 2 \t PROMEDIO\n");
	for (i = 0; i < k; i++) {
		printf("%d \t %s \t %d \t %d \t %.2f\n", reprobados[i].dni,
					 reprobados[i].name, reprobados[i].nota1, reprobados[i].nota2,
					 reprobados[i].promedio);
	}
}
void INFORME_PROM(alumno alumnos[], int cant) {
	int i = 0, j = 0;
	alumno promocionado[cant];
	for (i = 0; i < cant; i++) {
		if (alumnos[i].nota1 >= 7 && alumnos[i].nota2 >= 7) {
			promocionado[j] = alumnos[i];
			j++;
		}
	}
	printf("PROMOCIONADOS\n");
	printf("DNI \t NOMBRE \t NOTA 1 \t NOTA 2 \t PROMEDIO\n");
	for (i = 0; i < j; i++) {
		printf("%d \t %s \t %d \t %d \t %.2f\n", promocionado[i].dni,
					 promocionado[i].name, promocionado[i].nota1, promocionado[i].nota2,
					 promocionado[i].promedio);
	}
}
int BuscarAlumno(alumno alumnos[], int dni, int j) {
	int i = 0;
	while (i < j) {
		if (alumnos[i].dni == dni) {
			return i;
		}
		i++;
	}
	return -1;
}
void ej4() {
	cliente4 clientes[100];
	int i, cliente, importe, vendedor, code;
	char NyA[51];
	i = CARGA_CLIENTE(code, NyA, clientes);

	venta4 ventas[i];
	int j = 0;
	do {
		printf("Ingrese Numero de cliente: ");
		scanf("%d", &cliente);
	} while (EsCorrelativo4(cliente, i) && cliente != 999);
	while (cliente != 999 && i < j) {
		// numero de cliente debe ser no correlativo
		ventas[j].cliente = cliente;
		do {

			printf("Ingrese Importe: ");
			scanf("%d", &importe);
		} while (importe < 0);
		ventas[j].importe = importe;
		j++;
		do {
			printf("Ingrese Numero de vendedor: ");
			scanf("%d", &vendedor);
		} while (vendedor < 1 && vendedor > 10);
		ventas[j].vendedor = vendedor;
	}
}
int CARGA_CLIENTE(int code, char NyA[51], cliente4 clientes[100]) {
	int i = 0;
	do {
		printf("Ingrese el codigo del cliente: ");
		scanf("%d", &code);
	} while (EsCorrelativo4(code, i));
	while (code != 999 && i < 100) {
		clientes[i].code = code;
		printf("Ingrese el nombre y apellido del cliente: ");
		flush(stdin);
		fsgets(NyA, 51);
		strcpy(clientes[i].NyA, NyA);
		i++;
		do {
			printf("Ingrese el codigo del cliente: ");
			scanf("%d", &code);
		} while (EsCorrelativo4(code, i));
	}

	return i;
}
int EsCorrelativo4(int code, int i) {
	int j = 1;
	int anterior = code % 10, actual;
	code /= 10;
	// saber si un numero es correlativo o no
	while (code > 0) {
		actual = code % 10;
		if (actual + 1 != anterior && anterior != actual + 1) {
			j = 0;
		}
		anterior = actual;
		code /= 10;
	}
	return j;
}

void ej5() {
	persona5 persona;
	persona51 persona2;
	persona52 persona3;
	printf("Ingrese los datos de la persona\n");
	do {
		printf("Legajo: ");
		scanf("%d", &persona.legajo);
	} while (persona.legajo < 1 && persona.legajo > 10000);
	printf("Apellido: ");
	flush(stdin);
	fsgets(persona.apellido, 16);
	printf("Nombre: ");
	fsgets(persona.nombre, 16);
	do {
		printf("Altura: ");
		scanf("%f", &persona.altura);
	} while (persona.altura < 0);
	do {
		printf("Sexo: ");
		scanf(" %c", &persona.Sexo);
	} while (persona.Sexo != 'F' && persona.Sexo != 'M');
	do {
		printf("Dia de nacimiento: ");
		scanf("%d", &persona.DiaNacimiento);
	} while (persona.DiaNacimiento < 1 || persona.DiaNacimiento > 31);
	do {
		printf("Mes de nacimiento: ");
		scanf("%d", &persona.MesNacimiento);
	} while (persona.MesNacimiento < 1 || persona.MesNacimiento > 12);
	do {
		printf("Anio de nacimiento: ");
		scanf("%d", &persona.AnioNacimiento);
	} while (persona.AnioNacimiento < 1950 || persona.AnioNacimiento > 2019);
	printf("Persona:\n");
	printf("Legajo: %d\n", persona.legajo);
	printf("Apellido: %s\n", persona.apellido);
	printf("Nombre: %s\n", persona.nombre);
	printf("Altura: %.2f\n", persona.altura);
	printf("Sexo: %c\n", persona.Sexo);
	printf("Fecha de nacimiento: %d/%d/%d\n", persona.DiaNacimiento,
				 persona.MesNacimiento, persona.AnioNacimiento);
	persona2 = CargaPersona2(persona);
	printf("Persona:\n");
	printf("Legajo: %d\n", persona2.legajo);
	printf("Apellido: %s\n", persona2.NyA.Apellido);
	printf("Nombre: %s\n", persona2.NyA.Nombre);
	printf("Altura: %.2f\n", persona2.altura);
	printf("Sexo: %c\n", persona2.Sexo);
	printf("Fecha de nacimiento: %d/%d/%d\n", persona2.fecha.DiaNacimiento,
				 persona2.fecha.MesNacimiento, persona2.fecha.AnioNacimiento);

	CargaPersona3(persona2, persona3);
}
persona51 CargaPersona2(persona5 persona) {
	persona51 persona2;
	printf("Modificacion de datos\n");
	printf("Ingrese los datos de la persona\n");
	printf("Nombre: ");
	flush(stdin);
	fsgets(persona2.NyA.Nombre, 16);
	printf("Apellido: ");
	fsgets(persona2.NyA.Apellido, 16);
	do {
		printf("Dia de nacimiento: ");
		scanf("%d", &persona2.fecha.DiaNacimiento);
	} while (persona2.fecha.DiaNacimiento < 1 ||
					 persona2.fecha.DiaNacimiento > 31);
	do {
		printf("Mes de nacimiento: ");
		scanf("%d", &persona2.fecha.MesNacimiento);
	} while (persona2.fecha.MesNacimiento < 1 ||
					 persona2.fecha.MesNacimiento > 12);
	do {
		printf("Anio de nacimiento: ");
		scanf("%d", &persona2.fecha.AnioNacimiento);
	} while (persona2.fecha.AnioNacimiento < 1950 ||
					 persona2.fecha.AnioNacimiento > 2019);
	persona2.legajo = persona.legajo;
	persona2.altura = persona.altura;
	persona2.Sexo = persona.Sexo;

	return persona2;
}
void CargaPersona3(persona51 persona2, persona52 persona3) {
	int i = 0;
	do {
		printf("Ingrese el codigo de la materia aprobada: ");
		scanf("%d", &persona3.MateriasAprobadas[i]);
		i++;
	} while (persona3.MateriasAprobadas[i - 1] != 0 && i < 36);
	persona3.code = persona2.code;
	persona3.legajo = persona2.legajo;
	persona3.altura = persona2.altura;
	persona3.Sexo = persona2.Sexo;
	persona3.fecha = persona2.fecha;
	strcpy(persona3.NyA.Apellido, persona2.NyA.Apellido);
	strcpy(persona3.NyA.Nombre, persona2.NyA.Nombre);
	MostrarPersona3(persona3, i);
}
void MostrarPersona3(persona52 persona3, int j) {
	int i;
	printf("Persona:\n");
	printf("Legajo: %d\n", persona3.legajo);
	printf("Apellido: %s\n", persona3.NyA.Apellido);
	printf("Nombre: %s\n", persona3.NyA.Nombre);
	printf("Altura: %.2f\n", persona3.altura);
	printf("Sexo: %c\n", persona3.Sexo);
	printf("Fecha de nacimiento: %d/%d/%d\n", persona3.fecha.DiaNacimiento,
				 persona3.fecha.MesNacimiento, persona3.fecha.AnioNacimiento);
	printf("Materias aprobadas: ");
	for (i = 0; i < j; i++) {
		printf("%d ", persona3.MateriasAprobadas[i]);
	}
	printf("\n");
}
void ej6() {
	piloto pilotos[TAM6];
	int i, j;
	for (i = 0; i < TAM6; i++) {
		flush(stdin);
		printf("Ingrese Nombre de piloto: ");
		fsgets(pilotos[i].nombre, 30);
		printf("Ingrese escuderia: ");
		fsgets(pilotos[i].escuderia, 30);
		printf("Ingrese la posicion del piloto de cada carrera: \n");
		for (j = 0; j < CANT_CARRERAS; j++) {
			printf("Carrera %d: ", j);
			scanf("%d", &pilotos[i].posicion[j]);
		}
	}
	int carrera;
	do {
		printf("Ingrese Numero de carrera (0 para terminar): ");
		scanf("%d", &carrera);
		carrera--;
		MostrarCarrera(pilotos, carrera);
	} while (carrera < 0 || carrera < CANT_CARRERAS);
}
void MostrarCarrera(piloto pilotos[], int pos) {
	int i, j;
	piloto top10[TAM6], aux;
	for (i = 0; i < TAM6; i++) {
		if (pilotos[i].posicion[pos] <= 10) {
			top10[i] = pilotos[i];
		}
	}
	for (i = 0; i < TAM6 - 1; i++) {
		for (j = 0; j < TAM6 - 1 - i; j++) {
			if (top10[j].posicion[pos] > top10[j + 1].posicion[pos]) {
				aux = top10[j];
				top10[j] = top10[j + 1];
				top10[j + 1] = aux;
			}
		}
	}
	for (i = 0; i < TAM6; i++) {
		printf("Nombre: %s \t Escuderia: %s \t Posicion: %d\n", top10[i].nombre,
					 top10[i].escuderia, top10[i].posicion[pos]);
	}
}
