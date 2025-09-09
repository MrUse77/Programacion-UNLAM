#ifndef STRING_H
#define STRING_H
/***** Includes *****/
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/***** Macros *****/


/***** Estructuras *****/

//String
typedef struct {
	char str[51];
} String;

//SecString
typedef struct {
	char *str;
	bool finSec;
} SecString;

/***** Primitivas *****/

//String
void stringATitulo(String *str);

//SecString
void secStringCrear(SecString *sec, char *src);
bool secStringLeer(SecString *sec, String *str);
bool secStringEscribir(SecString *sec, const String *str);
bool secStringEscribirCaracter(SecString *sec, const char c);
bool secStringFin(const SecString *sec);
void secStringCerrar(SecString* sec);
/***** No Primitivas *****/
//String

//SecString

#endif // STRING_H_INCLUDED
