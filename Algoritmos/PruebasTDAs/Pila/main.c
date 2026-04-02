#include <stdio.h>
#include <stdlib.h>
#include "include/Pila.h"

int main()
{
	t_Pila p;
	int num = 20;
	crearPila(&p);

	int cod = apilar(&p,&num,sizeof(num));
	num++;
	 cod = apilar(&p,&num,sizeof(num));
	int num2;
	if(cod == OK){
		desapilar(&p,&num2,sizeof(num2));
	}
	printf("%d", num2);


    return cod;
}
