#ifndef STRING_H
#define STRING_H
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct {
  char *str;
  size_t length;
  size_t size;

} String;
// Function prototypes
size_t StringSetLen(const char *str);
size_t StringGetLen(String *str);
void StringNew(String *str);
void StringSet(String *str, const char *newStr);
const char *StringGet(String *str);
void StringRemove(String *str);
String StringConcat(String *str1, const char *str2);
char StringCharAt(String *str, size_t index);
bool StringIncludes(String *str, const char *text);
bool StringEndsWith(String *str, const char *text);
bool StringStartsWith(String *str, const char *text);
int StringCompare(String *str1, String *str2);
int StringIndexOf(String *str, char c);
#endif // STRING_H_INCLUDED
