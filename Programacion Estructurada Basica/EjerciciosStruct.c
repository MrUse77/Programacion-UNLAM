#include<stdio.h>
#include<string.h>


void flush(){
  int c;
  do{
    c=getchar();
  }while(c != EOF && c!='\n');
}
  typedef struct{
    int code;
    float price;
    char description[30];
    int unidadesVendidas;
    float total;
  }product;
void ej1(){
  int i=0,j=50,codigo,unidades,venta[50]={0};
  float total[50]={0};
  product productos[j];
  char fin[]="fin";
  do{
    printf("Ingrese el codigo del producto: "); scanf("%d", &productos[i].code);
    printf("Ingrese el precio del producto: "); scanf("%f", &productos[i].price);
    printf("Ingrese la descripcion del producto: "); scanf("%s", productos[i].description);
    printf("Ingrese la cantidad de unidades vendidas: "); scanf("%d", &productos[i].unidadesVendidas);
    productos[i].total=productos[i].price*productos[i].unidadesVendidas;
    i++;
    flush();
  }while(i!=j|| strcmp(productos[i].description, fin)==0);
  do{
    printf("Ingrese Codigo de producto");
    scanf("%d", &codigo);
    int codigoProducto = Buscar(productos, codigo, j);
    printf("Ingrese cantidad de unidades vendidas");
    scanf("%d", &unidades);
    venta[codigoProducto]+=unidades;
    total[codigoProducto]=productos[codigoProducto].price*venta[codigoProducto];
  }while(unidades!=0);
    for(i=0;i<j;i++){
      productos[i].unidadesVendidas+=venta[i];
      productos[i].total=total[i];
    }
  printf("DESCRIPCION \t CANTIDAD DE UNIDADES VENDIDAS \t IMPORTE TOTAL VENDIDO\n");
  for(i=0;i<j;i++){
    printf("%s \t %d \t %f\n", productos[i].description, productos[i].unidadesVendidas, productos[i].total);
  }
}
int Buscar(product productos[], int code, int j){
  int i=0;
  while(i<j){
    if(productos[i].code==code){
      return i;
    }
    i++;
  }
}
int main(){
  int opcion;
	printf("EJERCICIOS DE STRUCT\n");
  printf("Elija el ejercicio que desea realizar: \n"); scanf("%d", &opcion);
	switch(opcion){
    case 1:
      ej1();
    break;
  }
  
  return 0;
}
