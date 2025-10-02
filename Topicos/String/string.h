#ifndef STRING_H
#define STRING_H
#include <stddef.h>

/*
* Esta libreria usara como base algunas funciones hechas desde 0 de string.h
* asi como algunas modificaciones, mejoras y nuevas funciones.
 * */

size_t lenString(const char *str);
int cmpString(const char *str1, const char *str2);
char *cpyString(char *dest, const char *src, size_t lim);

#endif // !STRING_H
