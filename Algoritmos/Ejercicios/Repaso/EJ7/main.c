#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
void prints_rec1(int i, char *str)
{
	if (i == 0) {
		return;
	}
	printf("%.*s\n", i, str);
	prints_rec1(i - 1, str);
}
void prints_rec2(int i, char *str)
{
	if (i == 0) {
		return;
	}
	prints_rec2(i - 1, str);
	printf("%.*s\n", i, str);
}
void printArrayCompleto(char *c)
{
	int i = strlen(c);
	prints_rec2(i, c);
	prints_rec1(i, c);
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
void print1a1(char *c)
{
	if (*c == '\0') {
		return;
	}
	printf("%c \n", *c);
	print1a1(c + 1);
	return;
}
void print1a1Inverso(char *c)
{
	if (*c == '\0') {
		return;
	}
	print1a1Inverso(c + 1);
	printf("%c \n", *c);
	return;
}
void printi(int n)
{
	if (n == 0) {
		return;
	}
	printi(n / 10);
	printf("%d \n", n % 10);
}
int calcularDivisor(int n)
{
	if (n < 10) {
		return 1;
	}
	return 10 * calcularDivisor(n / 10);
}
void printiArrayCompleto(int n)
{
	if (n == 0) {
		return;
	}
	printf("%d \n", n);
	int divisor = calcularDivisor(n);
	printiArrayCompleto(n % divisor);
}
void printiArrayCompletoInverso(int n)
{
	if (n == 0) {
		return;
	}
	int divisor = calcularDivisor(n);
	printiArrayCompletoInverso(n % divisor);
	printf("%d \n", n);
}
void printiArrayCompletoCompleto(int n)
{
	if (n == 0) {
		return;
	}
	printf("%d \n", n);
	int divisor = calcularDivisor(n);
	printiArrayCompletoCompleto(n % divisor);
	printf("%d \n", n);
}
void printArrrayCompletoCompleto(char *c)
{
	if (*c == '\0') {
		return;
	}
	static int cont = 0;
	char *init = c - cont;
	char *fin = init;
	char *a = c;
	while (a >= init) {
		printf("%c", *init);
		init++;
	}
	printf("\n");
	cont++;
	printArrrayCompletoCompleto(c + 1);
	while (a >= fin) {
		printf("%c", *fin);
		fin++;
	}
	printf("\n");
}
bool chequearPalindromo(const char *inicio, const char *fin)
{
	if (inicio >= fin) {
		return true;
	}
	if (*inicio != *fin) {
		return false;
	}

	return chequearPalindromo(inicio + 1, fin - 1);
}

bool esPalindromo(const char *str)
{
	int longitud = 0;
	while (str[longitud] != '\0') {
		longitud++;
	}
	if (longitud == 0) {
		return true;
	}
	return chequearPalindromo(str, str + longitud - 1);
}

int main()
{
	/*	printf("Factorial of 20 is: %llu\n", factorial(20));
	puts(" ");
	print("Hola");
	puts(" \n");
	printInverso("Hola");
	puts(" \n");
	printArray("Hola");

	puts(" \n");
	printArrayInverso("Hola");
	puts(" \n");
	*/
	printArrayCompleto("Hola");
	puts(" \n");
	printArrayCompletoInverso("Hola");
	puts(" \n");
	printArrrayCompletoCompleto("Hola");
	puts(" \n");
	/*
	print1a1("Hola");
	puts(" \n");
	print1a1Inverso("Hola");
	*/
	printi(1234);
	puts(" \n");
	printiArrayCompleto(1234);
	puts(" \n");
	printiArrayCompletoInverso(1234);
	puts(" \n");
	printiArrayCompletoCompleto(1234);
	bool palindromo = esPalindromo("neuquen");
	if (palindromo) {
		printf("ES palindromo.\n");
	} else {
		printf(" NO es palindromo.\n");
	}

	return 0;
}
