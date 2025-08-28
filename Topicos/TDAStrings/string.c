#include "strings.h"
#include <stdio.h>
#include <stdlib.h>

bool isMayus(char c);
bool isMinus(char c);
char aMinus(char c);
char aMayus(char c);
/***** TDA *****/

void stringNew(String *str)
{
	str->size = 0;
	str->length = 0;
	str->str = (char *)malloc(1);
}

size_t stringGetLen(String *str)
{
	return str->length;
}

size_t stringSetLen(const char *str)
{
	size_t length = 0;
	while (str[length] != '\0') {
		length++;
	}
	return length;
}

void stringSet(String *str, const char *newStr)
{
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

const char *stringGet(String *str)
{
	if (str->str != NULL) {
		str->str[str->length] = '\0';
	}
	return str->str;
}

String stringConcat(String *str1, const char *str2)
{
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

char stringCharAt(String *str, size_t index)
{
	if (index < str->length) {
		if (str->str[index] != ' ') {
			return str->str[index + 1];
		}
		return str->str[index];
	}
	return '\0'; // Return null character if index is out of bounds
}

bool stringIncludes(String *str, const char *text)
{
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

bool stringEndsWith(String *str, const char *text)
{
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

bool stringStartsWith(String *str, const char *text)
{
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

int stringCompare(String *str1, String *str2)
{
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

int stringIndexOf(String *str, char c)
{
	for (size_t i = 0; i < str->length; i++) {
		if (str->str[i] == c) {
			return i;
		}
	}
	return -1;
}

void stringRemove(String *str)
{
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

String stringSubstring(String *str, size_t start, size_t end)
{
	String *subStr = NULL;
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

String stringToUpperCase(String *str)
{
	char *letra = str->str;
	while (*letra != '\0') {
		*letra = aMayus(*letra);
		letra++;
	}
	return *str;
}

String stringToLoweCase(String *str)
{
	char *letra = str->str;
	while (*letra != '\0') {
		*letra = aMinus(*letra);
		letra++;
	}
	return *str;
}

/***** SIN TDA *****/

size_t strLen(const char *str)
{
	size_t length = 0;
	while (*str != '\0') {
		length++;
		str++;
	}
	return length;
}

int strCompare(const char *str1, const char *str2)
{
	size_t len1 = strLen(str1);
	size_t len2 = strLen(str2);
	size_t minLen = (len1 < len2) ? len1 : len2;
	for (size_t i = 0; i < minLen; i++) {
		if (str1[i] != str2[i]) {
			return 0;
		}
	}
	return 1;
}
bool strIncludes(const char *str, const char *text)
{
	size_t len = strLen(text);
	for (size_t i = 0; i < strLen(str); i++) {
		if (str[i] == text[0]) {
			for (size_t j = 1; j < len; j++) {
				if (str[i + j] != text[j]) {
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
bool strEndsWith(const char *str, const char *text)
{
	size_t len = strLen(text);
	char *ult = (char *)str + strLen(str);
	if (len > strLen(str)) {
		return false;
	}
	for (size_t i = len; i > -1; i--) {
		if (*(ult - i) != *(text + len - i)) {
			return false;
		}
	}
	return true;
}
bool strStartsWith(const char *str, const char *text)
{
	size_t len = strLen(text);
	if (len > strLen(str)) {
		return false;
	}
	for (size_t i = 0; i < len; i++) {
		if (*(str + i) != *(text + i)) {
			return false;
		}
	}
	return true;
}
int strIndexOf(const char *str, char c)
{
	int index = 0;
	while (*str != '\0') {
		if (*str == c) {
			return index;
		}
		str++;
		index++;
	}
	return -1;
}
//Solo el primer caracter que encuentre
int strRemoveChar(char *str, char c)
{
	while (*str != '\0') {
		if (*str == c) {
			char *next = str + 1;
			while (*next != '\0') {
				*str = *next;
				str++;
				next++;
			}
			*str = '\0'; // Terminate the string
			return 1; // Character removed
		}
		str++;
	}
	return -1;
}

int strParseInt(const char *str)
{
	int result = 0;

	while (*str != '\0') {
		if (*str >= '0' && *str <= '9') {
			result = result * 10 + (*str - '0');
		} else {
			return -1; // Invalid character for integer parsing
		}
		str++;
	}
	return result;
}

float strParseFloat(const char *str)
{
	float result = 0, decimalFactor = 1.0;

	while (*str != ',' && *str != '.') {
		if (*str >= '0' && *str <= '9') {
			result = result * 10 + (*str - '0');
		} else {
			return -1; // Invalid character for integer parsing
		}
		str++;
	}
	str++;
	while (*str != '\0') {
		if (*str >= '0' && *str <= '9') {
			result = result + (*str - '0') / (decimalFactor *= 10);
		} else {
			return -1; // Invalid character for float parsing
		}

		str++;
	}
	return result;
}

/***** AUXILIARES *****/
bool isMayus(char c)
{
	if (c >= 'A' && c <= 'Z') {
		return true;
	}
	return false;
}
bool isMinus(char c)
{
	if (c >= 'a' && c <= 'z') {
		return true;
	}
	return false;
}
char aMayus(char c)
{
	if (isMinus(c)) {
		return c - ('a' - 'A');
	}
	return c;
}
char aMinus(char c)
{
	if (isMayus(c)) {
		return c + ('a' - 'A');
	}
	return c;
}
