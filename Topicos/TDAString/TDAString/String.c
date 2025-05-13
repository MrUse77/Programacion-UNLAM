#include "String.h"
#include <stdio.h>
#include <stdlib.h>

void newString(String *str) {
  str->size = 0;
  str->length = 0;
  str->str = (char *)malloc(1);
}

size_t getLen(String *str) { return str->length; }

size_t setLen(const char *str) {
  size_t length = 0;
  while (str[length] != '\0') {
    length++;
  }
  return length;
}

void setString(String *str, const char *newStr) {
  size_t newLen = setLen(newStr);
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

const char *getString(String *str) {

  if (str->str != NULL) {
    str->str[str->length] = '\0';
  }
  return str->str;
}

String concat(String *str1, const char *str2) {
  size_t len2 = setLen(str2);
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

char charAt(String *str, size_t index) {
  if (index < str->length) {
    if (str->str[index] != ' ') {
      return str->str[index + 1];
    }
    return str->str[index];
  }
  return '\0'; // Return null character if index is out of bounds
}

bool includes(String *str, const char *text) {
  size_t len = setLen(text);
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

bool endsWith(String *str, const char *text) {
  size_t len = setLen(text);
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

bool startsWith(String *str, const char *text) {
  size_t len = setLen(text);
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

/*
 * @brief Compares two strings.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return 1 if the strings are equal, 0 otherwise.
 * */
int strCompare(String *str1, String *str2) {
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

void deleteString(String *str) {
  if (str->str != NULL) {
    free(str->str);
    str->str = NULL;
  }
}
