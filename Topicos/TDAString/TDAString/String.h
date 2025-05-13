#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct {
  char *str;
  size_t length;
  size_t size;

} String;
// Function prototypes
size_t setLen(const char *str);
size_t getLen(String *str);
void newString(String *str);
void setString(String *str, const char *newStr);
const char *getString(String *str);
void deleteString(String *str);
String concat(String *str1, const char *str2);
char charAt(String *str, size_t index);
bool includes(String *str, const char *text);
bool endsWhith(String *str, const char *text);
bool startsWith(String *str, const char *text);

#endif // STRING_H_INCLUDED
