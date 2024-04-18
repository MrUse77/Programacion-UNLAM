//
// Created by agusd on 29/3/2024.
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define TAM 20

int ej1();

int ej2();

int ej3();

int ej4();

void ej25();

void ej29();

void ej210();

int Min(const int[], int);

int Max(const int[], int);

int Buscar(int[], int, int);

void IngresarCodigos(int[], int);

int IngresarCantidades(int[], int);

void BuscarMayorCantidad(int[], int[], int);

void BuscarMenorCantidad(int[], int[], int);

int BuscarRepetido(int[], int, int);

void LlenarSala(char [12][9]);

void MostrarSala(char [12][9]);

int HacerReserva(int, int, char [12][9]);

void FilasVacias(char [12][9], int);

void MaxSpec(int [2][9], char [12][9]);

int Verificacion(int, int);

int mapButaca(int);

void IngresoVotos(int, int, int, int[11][16], int[10], int[15], int);

void MostrarVotos(int[11][16]);

int BuscarMatrizRepetidas(int[11][16], int, int);

void MostrarTablero(char [3][3]);

int BusquedaGanador(char [3][3]);

int main() {
    int ej;
    printf("EJERCICIOS:\n");
    printf("Elija el ejercicio sin puntos (ej: ejercicio 2.5 = 25): ");
    scanf("%d", &ej);
    switch (ej) {
        case 1:
            printf("EJ 1:\n");
            ej1();
            break;
        case 2:
            printf("EJ 2:\n");
            ej2();
            break;
        case 3:
            printf("EJ 3:\n");
            ej3();
            break;
        case 4:
            printf("EJ 4:\n");
            ej4();
            break;
        case 25:
            printf("EJ 2.5:\n");
            ej25();
            break;
        case 29:
            printf("EJ 2.9:\n");
            ej29();
            break;
        case 210:
            printf("EJ 2.10:\n");
            ej210();
            break;
    }
    return 0;
}

//Ejercicio 1.1: Cargar de forma aleatoria un vector de 20 elementos con números de 3 cifras.
//a. Mostrar el menor valor e indicar cuantas veces se repite
//b. Mostrar el mayor valor e indicar en que posición/posiciones se encuentra
int ej1() {
    int ce = 20;
    int v[20];
    int min, max;
    srand(time(NULL));
    int i;


    for (i = 0; i < ce; i++) {
        v[i] = rand() % 1000;
    }

    min = Min(v, 20);
    max = Max(v, 20);
    printf("El menor valor es %d\n", min);
    printf("El mayor valor es %d\n", max);
    return 0;
}

int Min(const int v[], int N) {
    int i, min;
    min = v[0];
    for (i = 1; i < N; i++) {
        if (v[i] < min) {
            min = v[i];
        }
    }
    return min;
}

int Max(const int v[], int N) {
    int i, max;
    max = v[0];
    for (i = 1; i < N; i++) {
        if (v[i] > max) {
            max = v[i];
        }
    }
    return max;
}
//Ejercicio 1.2: Una empresa debe registrar los pedidos recibidos de cada uno sus 10 productos a lo largo del día. Cada
//producto está identificado por un código de 4 cifras. Los códigos deben ingresarse al inicio del programa
//mediante la función IngresaCódigos y no pueden repetirse.
//Por cada pedido se recibe:
//• Código de producto
//• Cantidad de unidades solicitadas
//Se puede recibir más de un pedido por producto.
// La carga de pedidos finaliza cuando se ingresa un producto igual a 0
//Al finalizar se debe:
//a) Emitir un listado con código y cantidad de unidades solicitadas de cada producto.
//b) El / los productos del cual se solicitaron mayor cantidad de unidades.
//c) El / los productos del cual se solicitaron menos cantidad de unidades.

int ej2() {
    int cant = 3;
    int products[10];
    int cants[10] = {0};

    int codMax, codMin;

    for (int i = 0; i < cant; i++) {
        IngresarCodigos(products, i);
        IngresarCantidades(cants, i);
    }

    for (int i = 0; i < cant; i++) {
        printf("Codigo: %d, Cantidad: %d\n", products[i], cants[i]);
    }

    BuscarMayorCantidad(products, cants, cant);
    BuscarMenorCantidad(products, cants, cant);

    return 0;
}

void IngresarCodigos(int v[], int i) {
    int pos;
    int aux;
    do {
        printf("Ingrese el codigo del producto %d: ", i + 1);
        scanf("%d", &aux);
        pos = Buscar(v, aux, i);
        if (pos != -1) printf("Codigo incorrecto\n");
    } while (pos != -1);
    v[i] = aux;
}

int Buscar(int v[], int datoABuscar, int cantElem) {
    int i = 0, pos = -1;
    if (datoABuscar < 1000 || datoABuscar > 9999) return 1;
    while (pos == -1 && i < cantElem) {
        if (v[i] == datoABuscar) {
            pos = i;
        } else i++;
    }
    return pos;
}

int IngresarCantidades(int v[], int c) {
    int aux = 0;
    do {
        printf("Ingrese la cantidad de unidades solicitadas(con 0 terminan las solicitudes del producto): ");
        scanf("%d", &aux);
        v[c] += aux;

    } while (aux != 0);

    return 0;
}

void BuscarMayorCantidad(int v[], int c[], int N) {
    int i, max[2];
    max[0] = v[0];
    max[1] = c[0];
    for (i = 0; i < N; i++) {
        if (c[i] > max[1]) {
            max[0] = v[i];
            max[1] = c[i];
        }
    }
    printf("La mayor cantidad de unidades, con codigo %d, solicitadas es: %d\n", max[0], max[1]);


}

void BuscarMenorCantidad(int v[], int c[], int N) {
    int i, min[2];
    min[0] = v[0];
    min[1] = c[0];
    for (i = 0; i < N; i++) {
        if (c[i] < min[1]) {
            min[0] = v[i];
            min[1] = c[i];
        }
    }
    printf("La menor cantidad de unidades, con codigo %d, solicitadas es: %d\n", min[0], min[1]);

}

//Ejercicio 1.3  En un curso de la maestría en informática los estudiantes deben exponer en forma grupal sus investigaciones
//de un tema dado. El curso está formado por 5 grupos. Se pide realizar un programa para sortear el orden de
//exposición de dichos grupos. Cada vez que se ejecute el programa deberá mostrar un orden de exposición
//distinto para cada uno de los 5 grupos.

int ej3() {
    int grupos[5] = {1, 2, 3, 4, 5};
    int j, aux, num;
    srand(time(NULL));
    for (j = 0; j < 5; j++) {
        num = rand() % 5;
        aux = grupos[j];
        grupos[j] = grupos[num];
        grupos[num] = aux;
    }
    printf("Orden de exposicion de los grupos: \n");
    for (j = 0; j < 5; j++) {
        printf("%d ", grupos[j]);
        printf("\n");
    }
    return 0;
}
//Ejercicio 1.4: Se ingresan DNI y nota de un parcial de los alumnos de un curso. El ingreso de datos finaliza con un DNI
//negativo. Se sabe que como máximo pueden presentarse a rendir 60 alumnos. Tenga en cuenta que no
//pueden existir 2 o más alumnos con el mismo DNI.
//Mostrar el Listado de alumnos con su correspondiente DNI y la nota obtenida (en forma de listado), ordenado
//de mayor a menor por nota.

int ej4() {
    int DNI[6] = {123, 124, 125, 126, 127, 128};
    int nota[6] = {2, 1, 7, 10, 9, 1};
    int i;
    int aux;
    int aux2;
    int cant = 6;
    do {
        do {
            printf("Ingrese el DNI del alumno: ");
            scanf("%d", &aux);

            if (BuscarRepetido(DNI, aux, cant) != -1) printf("DNI incorrecto\n");
        } while (BuscarRepetido(DNI, aux, cant) != -1);
        if (aux > 0) {
            DNI[cant] = aux;
            do {
                printf("Ingrese la nota del alumno: ");
                scanf("%d", &aux2);
                if (aux2 < 0 || aux2 > 10) printf("Nota incorrecta\n");
            } while (aux2 < 0 || aux2 > 10);
            nota[cant] = aux2;
            cant++;
        }
    } while (aux > 0 && cant < 6);

    for (i = 0; i < cant; i++) {

        for (int j = 0; j < cant - 1; j++) {
            if (nota[j] < nota[j + 1]) {
                aux = nota[j];
                nota[j] = nota[j + 1];
                nota[j + 1] = aux;
                aux = DNI[j];
                DNI[j] = DNI[j + 1];
                DNI[j + 1] = aux;
            }
        }

    }
    for (i = 0; i < cant; i++) {
        printf("DNI: %d, Nota: %d\n", DNI[i], nota[i]);
    }
    return 0;
}

int BuscarRepetido(int v[], int datoABuscar, int cantElem) {
    int i = 0, pos = -1;
    while (pos == -1 && i < cantElem) {
        if (v[i] == datoABuscar) {
            pos = i;
        } else i++;
    }
    return pos;
}

/*Ej 2.5:
Se desea desarrollar un sistema de reservas de entradas para un cine. La sala consta de 12 filas numeradas de
la 1 a la 12 y cada fila tiene 9 butacas numeradas a partir de la columna central, con las butacas impares a la
y las pares a la izquierda, como en el siguiente esquema:
        8 6 4 2 1 3 5 7 9
Para la carga, se debe mostrar al usuario un esquema con las butacas disponibles y reservadas, marcando
con la letra D las disponibles y con la letra R las reservadas.
Por cada reserva se debe solicitar la fila y número de butaca a reservar. Cada vez que se realice una
reserva se deberá actualizar el esquema que muestra las butacas. Si la butaca seleccionada ya estaba
ocupada se debe informar al usuario para que seleccione otra. El proceso de reserva finaliza con una fila
con un número negativo. Al finalizar mostrar:
a. la cantidad de asientos disponibles y la cantidad de asientos reservados.
b. los números de filas que quedaron vacías.
c. la o las filas con mayor cantidad de espectadores.
d. un listado con la cantidad de personas que se sentaron en los mismos números de butacas en todo el cine ordenado de mayor a menor.
*/
void ej25() {
    char sala[12][9] = {0};
    int i, fila, butaca, cantReservadas = 0, cantDisponibles = 12 * 9, cantVacias = 0, cant[2][9] = {0};
    LlenarSala(sala);
    do {
        do {
            printf("\n");
            MostrarSala(sala);
            printf("Ingrese la fila y la butaca a reservar: ");
            scanf("%d", &fila);
            scanf("%d", &butaca);
        } while (HacerReserva(fila, butaca, sala) == -1 && Verificacion(fila, butaca) == -1);
        if (fila > 0) {
            cantReservadas++;
            cantDisponibles--;
        }
    } while (fila > 0);


    printf("Cantidad de asientos disponibles: %d\n", cantDisponibles);
    printf("Cantidad de asientos reservados: %d\n", cantReservadas);
    FilasVacias(sala, cantVacias);
    MaxSpec(cant, sala);
}

void LlenarSala(char sala[12][9]) {
    int i, j;
    for (i = 0; i < 12; i++) {
        for (j = 0; j < 9; j++) {
            sala[i][j] = 'D';
        }
    }
}

void MostrarSala(char sala[12][9]) {
    int i, j;
    printf("      ");
    for (i = 8; i >= 1; i -= 2) {
        printf("  %d  ", i);
    }
    for (i = 1; i <= 9; i += 2) {
        printf("  %d  ", i);
    }
    for (i = 0; i < 13; i++) {
        for (j = 0; j < 10; j++) {
            if (j == 0 && i > 0) {
                if (i < 10) printf("  0%d  ", i);
                else printf("  %d  ", i);
            } else printf("  %c  ", sala[i - 1][j - 1]);

        }
        printf("\n");
    }
}

int Verificacion(int fila, int butaca) {
    if (fila < 0) return 0;
    else if (fila == 0 || fila > 12 || butaca < 1 || butaca > 9) {
        printf("Fila o butaca incorrecta\n");
        return -1;
    }
}

int HacerReserva(int fila, int butaca, char sala[12][9]) {
    int posButaca = mapButaca(butaca);
    if (sala[fila - 1][posButaca] == 'D') {
        sala[fila - 1][posButaca] = 'R';
        return 0;
    } else {
        printf("Butaca ocupada, seleccione otra\n");
        return -1;
    }
}

void FilasVacias(char sala[12][9], int cantVacias) {
    int i, j, vacia = 0;
    for (i = 0; i < 12; i++) {
        vacia = 1;
        for (j = 0; j < 9; j++) {
            if (sala[i][j] == 'R') {
                vacia = 0;
                break;
            }
        }
        if (vacia == 1) {
            cantVacias++;
        }
    }
    printf("Filas vacias: %d\n", cantVacias);
}

void MaxSpec(int cant[2][9], char sala[12][9]) {
    int i, j, filas[12] = {0}, max[2] = {0}, butacas[9] = {0};
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 12; j++) {
            if (sala[j][i] == 'R') {
                filas[j]++;
            }
        }
    }
    for (i = 0; i < 12; i++) {
        for (j = 0; j < 9; j++) {
            if (sala[i][j] == 'R') {
                butacas[j]++;
            }
        }
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8 - i; j++) {
            if (butacas[j] < butacas[j + 1]) {
                max[0] = butacas[j];
                butacas[j] = butacas[j + 1];
                butacas[j + 1] = max[0];
            }
        }
    }
    for (i = 0; i < 11; i++) {
        for (j = 0; j < 11 - i; j++) {
            if (filas[j] < filas[j + 1]) {
                max[1] = filas[j];
                filas[j] = filas[j + 1];
                filas[j + 1] = max[1];
            }
        }
    }
    max[1] = filas[0];
    i = 0;
    i = 0;
    while (filas[i] == max[1]) {
        cant[0][i] = i + 1;
        i++;
    }
    printf("Filas con mayor cantidad de espectadores: ");
    for (i = 0; i < 9; i++) {
        if (cant[0][i] != 0) {
            printf("%d ", cant[0][i]);
        }
    }
    printf("\n");
    printf("Cantidad de personas que se sentaron en los mismos numeros de butacas en todo el cine ordenado de mayor a menor: ");
    for (i = 0; i < 9; i++) {
        if (butacas[i] != 0)printf("%d ", butacas[i]);
    }
}

int mapButaca(int butaca) {
    switch (butaca) {
        case 8:
            return 0;
        case 6:
            return 1;
        case 4:
            return 2;
        case 2:
            return 3;
        case 1:
            return 4;
        case 3:
            return 5;
        case 5:
            return 6;
        case 7:
            return 7;
        case 9:
            return 8;
    }
}

/*Ej:2.9 Se desea contabilizar los votos recibidos en las elecciones de un club de fútbol. Existen 10 candidatos cada
uno representado por un número de lista diferente (número de 3 cifras no correlativo). La votación se realiza
en 15 sedes distintas codificadas del 1 al 15. Se ingresan los votos registrados en cada una de las sedes
        registrando:
• Número de lista
• Número de sede
• Cantidad de votos
(Solo se informan candidatos que recibieron votos).
La carga de votos finaliza con un número de lista igual a 0
Mostrar:
a. Cantidad de votos recibidos por cada candidato en cada sede.
LISTA SEDE1 SEDE2 SEDE3… SEDE15
873 36 78 99… XX
735 XX XX XXX… XX
b. Listado ordenado por cantidad de votos totales en formar decreciente, con el siguiente formato:
TOTAL DE VOTOS PORCENTAJE LISTA
800 80% 873
200 20% 735
c. Candidatos que NO recibieron votos en la sede 5*/
void ej29() {
    int votos[11][16] = {0};
    int i, j;
    int lista, sede, voto,rep=0;
    int totalVotos[10] = {0};
    int totalVotosSede[15] = {0};
    int totalVotosTotal = 0;
    for (i = 0; i < 16; i++) {
        votos[0][i] = i;
    }
    i=1;
    do {
        do {
            printf("Ingrese el numero de lista: ");
            scanf("%d", &lista);
            rep=BuscarMatrizRepetidas(votos, lista, 11);
        } while (lista != 0 && (lista < 100 || lista > 999));
        if(rep=-1) votos[i][0] =lista;
        if (lista!=0) {
          IngresoVotos(lista, sede, voto, votos, totalVotos, totalVotosSede, totalVotosTotal);
          i++;
        }

    } while (lista != 0);
    MostrarVotos(votos);
    for (i = 1; i < 11; i++) {
        printf("TOTAL DE VOTOS PORCENTAJE LISTA\n");
        printf("     %d            %d       %d\n", totalVotos[i], totalVotos[i] * 100 / totalVotosTotal,
               votos[i][0]);
    }
    printf("Candidatos que NO recibieron votos en la sede 5\n");
    for (i = 1; i < 11; i++) {
        if (votos[i][5] == 0) {
            printf("LISTA: ");
            printf("%d\n", votos[i][0]);
        }
    }
}

void IngresoVotos(int lista, int sede, int voto, int votos[11][16], int totalVotos[10], int totalVotosSede[15],
                  int totalVotosTotal) {
    int i,j;
    do {
        printf("Ingrese el numero de sede: ");
        scanf("%d", &sede);
    } while (sede < 1 || sede > 15);
    printf("Ingrese la cantidad de votos: ");
    scanf("%d", &voto);
    for(i=1;i<11;i++){
          printf("\n%d %d",votos[i][0],lista);
          if(votos[i][0]==lista){
            votos[i][sede]=voto;
            totalVotos[i]+=voto;
            totalVotosSede[sede]+=voto;
            totalVotosTotal+=voto;
          }
        
    }
    
}

void MostrarVotos(int votos[11][16]) {
    int i, j;
    printf("LISTA ");
    for (i = 1; i < 16; i++) {
        printf("\tSEDE %d", votos[0][i]);
    }
    printf("\n");
    for (i = 1; i < 11; i++) {
        printf("%d ", votos[i][0]);
        for (j = 1; j < 16; j++) {
            printf("\t%d", votos[i][j]);
        }
        printf("\n");
    }
}
int BuscarMatrizRepetidas(int v[11][16], int datoABuscar, int cantElem) {
    int i = 0, pos = -1;
    while (pos == -1 && i < cantElem) {
        if (v[i][0] == datoABuscar) {
            pos = i;
        } else i++;
    }
    return pos;
}

/*Ej: 2.10 Realizar el juego del Tateti. El jugador 1 utilizara la letra X
y el jugador 2 la letra O para maracar sus elecciones. el tablero armarlo con
una matriz de 3x3 que debe ser visualizada en pantalla en cada jugada marcando
la ubicacion elegida por cada jugador. Luego de cada jugada se debe llamar a una funcion
para verificar si el jugador gano o no la partida. Si se llega a la ultima jugada y nadie
gana, se debe informar del empate
*/
void ej210(){
  char tablero[3][3]={0};
  int i,j,x,y,ganador=0;
  for(i=0;i<3;i++){
    for(j=0;j<3;j++){
      tablero[i][j]=' ';
    }
  }
  MostrarTablero(tablero);
  i=0;
  do{
    if(i%2==0){ 
      printf("Jugador 1: ");
      scanf("%d %d",&x,&y);
      tablero[x-1][y-1]='X';
    }else{
      printf("Jugador 2: ");
      scanf("%d %d",&x,&y);
      tablero[x-1][y-1]='O';
    }
    MostrarTablero(tablero);
    ganador=BusquedaGanador(tablero);
    i++;
    printf("ganador: %c\n",ganador);
    printf("i: %d\n",i);
  }while(i<9 && !ganador); 
  if(ganador==1){
    printf("El ganador es: X");
  }else if(ganador==2){
    printf("El ganador es: O");
  }else{
    printf("Empate");
  
  }
}
void MostrarTablero(char tablero[3][3]){
  int i,j;
  for(i=0;i<3;i++){
    for(j=0;j<3;j++){
      printf("| %c |",tablero[i][j]);
    }
    printf("\n");
  }
}
int BusquedaGanador(char tablero[3][3]){
  int i;
  for(i=0;i<3;i++){
    if(tablero[i][0]==tablero[i][1] && tablero[i][1]==tablero[i][2]){
      if(tablero[i][0]=='X'){
        return 1;
      }else if(tablero[i][0]=='O'){
        return 2;
      }
    }else if(tablero[0][i]==tablero[1][i] && tablero[1][i]==tablero[2][i]){
      if(tablero[0][i]=='X'){
        return 1;
      }else if(tablero[0][i]=='O'){
        return 2;
      }
    }
  }
  if(tablero[0][0]==tablero[1][1] && tablero[1][1]==tablero[2][2]){
    if(tablero[0][0]=='X'){
        return 1;
      }else if(tablero[0][0]=='O'){
        return 2;
      }
  }else if(tablero[0][2]==tablero[1][1] && tablero[1][1]==tablero[2][0]){
    if(tablero[0][2]=='X'){
        return 1;
      }else if(tablero[0][2]=='O'){
        return 2;
      }
  }
    return 0;
  

}