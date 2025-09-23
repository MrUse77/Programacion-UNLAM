#include "strings.h"
#define isMayus(c) ((c) >= 'A' && (c) <= 'Z')
#define isMinus(c) ((c) >= 'a' && (c) <= 'z')
#define aMayus(c) (((c) >= 'a' && (c) <= 'z') ? (c) - ('a' - 'A') : (c))
#define aMinus(c) (((c) >= 'A' && (c) <= 'Z') ? (c) + ('a' - 'A') : (c))
#define esLetra(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
//String

void stringATitulo(String *str)
{
	char *i = str->str;
	*i = aMayus(*i);
	i++;
	while (*i) {
		*i = aMinus(*i);
		i++;
	}
}

//SecString
void secStringCrear(SecString *sec, char *src)
{
	sec->str = src;
	sec->finSec = false;
}
bool secStringLeer(SecString *sec, String *str)
{
	while (*sec->str != '\0' && (!esLetra(*sec->str))) {
		sec->str++;
	}
	if (*sec->str == '\0') {
		sec->finSec = true;
		return false;
	}
	char *i = str->str;
	while (*sec->str != '\0' && (esLetra(*sec->str))) {
		*i = *sec->str;
		i++;
		sec->str++;
	}
	*i = '\0';
	return true;
}
bool secStringEscribir(SecString *sec, const String *str)
{
	const char *i = str->str;
	while (*i) {
		*sec->str = *i;
		sec->str++;
		i++;
	}
	return true;
}
bool secStringEscribirCaracter(SecString *sec, const char c)
{
	*sec->str = c;
	sec->str++;
	return true;
}

bool secStringFin(const SecString *sec)
{
	return sec->finSec;
}

void secStringCerrar(SecString* sec)
{
    *sec->str = '\0';
}
