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

bool isMayus(char c);
bool isMinus(char c);
char aMinus(char c);
char aMayus(char c);

#endif // STRING_H_INCLUDED
