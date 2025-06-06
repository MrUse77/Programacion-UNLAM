#include "String.h"
#include <stdio.h>
#include <stdlib.h>

void stringNew(String *str) {
  str->size = 0;
  str->length = 0;
  str->str = (char *)malloc(1);
}

size_t stringGetLen(String *str) { return str->length; }

size_t stringSetLen(const char *str) {
  size_t length = 0;
  while (str[length] != '\0') {
    length++;
  }
  return length;
}

void stringSet(String *str, const char *newStr) {
  size_t newLen = stringSetLen(newStr);
  size_t newSize = str->length + 1;
  char *temp = NULL;
  if (newLen + 1 != str->length) {
    temp = (char *)realloc(str->str, (newLen + 1) * sizeof(char));
    if (temp == NULL) {
      return;
    }
    str->str = temp;
    str->size = newSize;
  }

  for (size_t i = 0; i < newLen; i++) {
    str->str[i] = newStr[i];
  }
  str->length = newLen;
}

const char *stringGet(String *str) {
  if (str->str != NULL) {
    str->str[str->length] = '\0';
  }
  return str->str;
}

String stringConcat(String *str1, const char *str2) {
  size_t len2 = stringSetLen(str2);
  size_t newLen = str1->length + len2;
  if (newLen + 1 > str1->size) {
    size_t newSize = str1->size * 2;
    do {
      newSize *= 2;
    } while (str1->size < newLen + 1);
    char *temp = (char *)realloc(str1->str, newSize * sizeof(char));
    if (temp == NULL) {
      return *str1;
    }
    str1->str = temp;
    str1->size = newSize;
    str1->length = newLen;
  }
  for (size_t i = 0; i < len2; i++) {
    str1->str[str1->length + i] = str2[i];
  }
  return *str1;
}

char stringCharAt(String *str, size_t index) {
  if (index < str->length) {
    if (str->str[index] != ' ') {
      return str->str[index + 1];
    }
    return str->str[index];
  }
  return '\0'; // Return null character if index is out of bounds
}

bool stringIncludes(String *str, const char *text) {
  size_t len = stringSetLen(text);
  for (size_t i = 0; i < str->length; i++) {
    if (str->str[i] == text[0]) {
      for (size_t j = 1; j < len; j++) {
        if (str->str[i + j] != text[j]) {
          break;
        }
        if (j == len - 1) {
          return true;
        }
      }
    }
  }
  return false;
}

bool stringEndsWith(String *str, const char *text) {
  size_t len = stringSetLen(text);
  if (len > str->length) {
    return false;
  }
  for (size_t i = 0; i < len; i++) {
    if (str->str[str->length - len + i] != text[i]) {
      return false;
    }
  }
  return true;
}

bool stringStartsWith(String *str, const char *text) {
  size_t len = stringSetLen(text);
  if (len > str->length) {
    return false;
  }
  for (size_t i = 0; i < len; i++) {
    if (str->str[i] != text[i]) {
      return false;
    }
  }
  return true;
}

int stringCompare(String *str1, String *str2) {
  size_t len1 = str1->length;
  size_t len2 = str2->length;
  size_t minLen = (len1 < len2) ? len1 : len2;
  for (size_t i = 0; i < minLen; i++) {
    if (str1->str[i] != str2->str[i]) {
      return 0;
    }
  }
  return 1;
}

int stringIndexOf(String *str, char c) {

  for (size_t i = 0; i < str->length; i++) {
    if (str->str[i] == c) {
      return i;
    }
  }
  return -1;
}

void stringRemove(String *str) {
  if (str->str != NULL) {
    free(str->str);
    str->str = NULL;
  }
}

/* String stringReplace(String *str, const char *oldStr, const char *newStr) {
  size_t oldLen = stringSetLen(oldStr);
  size_t newLen = stringSetLen(newStr);
  str->length += newLen - oldLen;
  if (stringIncludes(str, oldStr)) {
    size_t index = stringIndexOf(str, oldStr[0]);
      for (size_t i = 0; i < oldLen; i++) {
        str->str[index + newLen + i] = newStr[i];
      }
    if (index != -1) {
      char *temp = (char *)realloc(str->str, (str->length + 1) * sizeof(char));
      str->str = temp;
    }
  }
  return *str;
}*/

String stringSubstring(String *str, size_t start, size_t end) {
  String *subStr;
  stringNew(subStr);
  if (start < str->length && end <= str->length && start < end) {
    size_t len = end - start;
    subStr->str = (char *)malloc((len + 1) * sizeof(char));
    for (size_t i = 0; i < len; i++) {
      subStr->str[i] = str->str[start + i];
    }
    subStr->str[len] = '\0';
    subStr->length = len;
  }
  return *subStr;
}

String stringToUpperCase(String *str) {
  char *letra = str->str;
  while (*letra != '\0') {
    *letra = aMayus(*letra);
    letra++;
  }
  return *str;
}

String stringToLoweCase(String *str) {
  char *letra = str->str;
  while (*letra != '\0') {
    *letra = aMinus(*letra);
    letra++;
  }
  return *str;
}
bool isMayus(char c) {
  if (c >= 'A' && c <= 'Z') {
    return true;
  }
  return false;
}
bool isMinus(char c) {
  if (c >= 'a' && c <= 'z') {
    return true;
  }
  return false;
}
char aMayus(char c) {
  if (isMinus(c)) {
    return c - ('a' - 'A');
  }
  return c;
}
char aMinus(char c) {
  if (isMayus(c)) {
    return c + ('a' - 'A');
  }
  return c;
}
