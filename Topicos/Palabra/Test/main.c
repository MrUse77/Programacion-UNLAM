#include <stdio.h>
#include "../TDA/strings.h"
#define TAM 201

char *normalizar(const char *src, char *dest);
int main(int argc, char *argv[])
{
	char normalize[TAM];
	puts(argv[1]);
	normalizar(argv[1], normalize);
	puts(normalize);
	return 0;
}
char *normalizar(const char *src, char *dest)
{
	SecString secLect, secEscr;
	secStringCrear(&secLect, (char *)src);
	secStringCrear(&secEscr, dest);
	String str;
	secStringLeer(&secLect, &str);
	while (!secStringFin(&secLect)) {
		stringATitulo(&str);
		secStringEscribir(&secEscr, &str);
		secStringEscribirCaracter(&secEscr, ' ');
		secStringLeer(&secLect, &str);
	}
	secStringCerrar(&secEscr);
	return dest;
}
