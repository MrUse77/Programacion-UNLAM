#ifndef FLUSH_H
#define FLUSH_H
#include <stdio.h>

void flush(FILE *stream);
void scanfInt(int *n, int li, int ls, int c);
void scanfFloat(float *n, float li, float ls, int c);
void fsgets(char s[], int n);
void Err(char *msg);

#endif
