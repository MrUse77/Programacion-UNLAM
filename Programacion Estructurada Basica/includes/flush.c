#include "flush.h"
#include <stdlib.h>

void flush(FILE *stream)
{
	int c;
	do
	{
		c = getchar();
	} while (c != EOF && c != '\n');
}
