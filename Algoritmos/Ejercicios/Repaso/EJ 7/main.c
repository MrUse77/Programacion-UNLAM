#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long long factorial(unsigned int n)
{
	if (n == 0)
		return 1;
	else
		return n * factorial(n - 1);
}
void print(char *c)
{
	if (*c == '\0') {
		return;
	}
	printf("'%c' ", *c);
	print(c + 1);
	return;
}
void printInverso(char *c)
{
	if (*c == '\0') {
		return;
	}
	printInverso(c + 1);
	printf("'%c' ", *c);
	return;
}
void printArray(char *c)
{
	if (*c == '\0') {
		return;
	}
	puts(c);
	printArray(c + 1);
	return;
}
void printArrayInverso(char *c)
{
	if (*c == '\0') {
		return;
	}
	printArrayInverso(c + 1);
	puts(c);
	return;
}
void printArrayCompleto(char *c)
{
	if (*c == '\0') {
		return;
	}
	static int cont = 0;
	char *init = c - cont;
	char *a = c;
	while (a >= init) {
		printf("%c", *init);
		init++;
	}
	printf("\n");
	cont++;
	printArrayCompleto(c + 1);
	return;
}
void printArrayCompletoInverso(char *c)
{
	if (*c == '\0') {
		return;
	}
	static int cont = 0;
	char *init = c - cont;
	char *a = c;
	cont++;
	printArrayCompletoInverso(c + 1);
	while (a >= init) {
		printf("%c", *init);
		init++;
	}
	printf("\n");
	return;
}
int main()
{
	printf("Factorial of 20 is: %llu\n", factorial(20));
	puts(" ");
	print("Hola");

	puts(" \n");
	printInverso("Hola");

	puts(" \n");
	printArray("Hola");

	puts(" \n");
	printArrayInverso("Hola");
	puts(" \n");
	printArrayCompleto("Hola");
	puts(" \n");
	printArrayCompletoInverso("Hola");
	puts(" \n");

	return 0;
}
