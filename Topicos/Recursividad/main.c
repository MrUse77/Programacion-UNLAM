#include <stdio.h>
#include <stdlib.h>

unsigned long long factorial(unsigned long long x);
int dcm(int x, int y);
int main(int argc, char *argv[])
{

	int x = atoi(argv[1]);
	int y = atoi(argv[2]);
	printf("DCM(%d,%d): %d\n", x, y, dcm(x, y));
	//printf("Factorial: %llu\n", factorial(x));

	return 0;
}

unsigned long long factorial(unsigned long long x)
{
	if (x <= 1) {
		return 1;
	}
	return x * factorial(x - 1);
}

int dcm(int x, int y)
{
	if (y == 0) {
		return x;
	}
	return dcm(y, x % y);
}
