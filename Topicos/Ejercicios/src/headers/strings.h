#ifndef STRING_H
#define STRING_H
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct {
	char *str;
	size_t length;
	size_t size;
} String;
/***** CON TDA *****/
// Function prototypes
size_t stringSetLen(const char *str);
size_t stringGetLen(String *str);
void stringNew(String *str);
void stringSet(String *str, const char *newStr);
const char *stringGet(String *str);
void stringRemove(String *str);
String stringConcat(String *str1, const char *str2);
char stringCharAt(String *str, size_t index);
bool stringIncludes(String *str, const char *text);
bool stringEndsWith(String *str, const char *text);
bool stringStartsWith(String *str, const char *text);
int stringCompare(String *str1, String *str2);
int stringIndexOf(String *str, char c);
// String stringReplace(String *str, const char *oldStr, const char *newStr);
String stringSubstring(String *str, size_t start, size_t end);

/***** SIN TDA *****/
size_t strLen(const char *str);
int strCompare(const char *str1, const char *str2);
bool strIncludes(const char *str, const char *text);
bool strEndsWith(const char *str, const char *text);
bool strStartsWith(const char *str, const char *text);
int strIndexOf(const char *str, char c);
int strRemoveChar(char *str, char c);
int strParseInt(const char *str);
float strParseFloat(const char *str);

#endif // STRING_H_INCLUDED
