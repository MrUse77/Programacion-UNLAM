#ifndef STRING_H
#define STRING_H
#include <stddef.h>

#include <stdbool.h>
/*
* Esta libreria usara como base algunas funciones hechas desde 0 de string.h
* asi como algunas modificaciones, mejoras y nuevas funciones.
 * */

size_t lenString(const char *str);
int cmpString(const char *str1, const char *str2);
int cmpNString(const char *str1, const char *str2, size_t n);
char *cpyNString(char *dest, const char *src, size_t lim);
char *cpyString(char *dest, const char *src);
char *removerCharEnString(char *str, char c);
char *normarlizarString(char *str);
char *reemplazarCharEnString(char *str, const char buscar,
			     const char reemplazar);
char *reemplazarCharEnStringEnPos(char *str, const char reemplazar, size_t pos);
char *buscarCharEnStringEnReversa(const char *str, char c);
char *buscarStringEnReversa(const char *str, const char *sub);
char *buscarSubString(const char *str, const char *sub);
char *buscarChar(const char *str, char c);
char *normarlizarPrimerChar(char *str);
bool includeString(const char *str, const char **arr, size_t size);

#endif // !STRING_H
