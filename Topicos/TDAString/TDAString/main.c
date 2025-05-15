#include "String.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
  String str;

	StringNew(&str);

	StringSet(&str,"hola");

	int a = StringIndexOf(&str,'h');



	printf("Hello World %d",a);
    return 0;
}
