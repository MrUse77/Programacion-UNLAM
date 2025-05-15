#include "myShortCuts.h"
#include <stdio.h>
#include <stdlib.h>

void scanfInt(int *n, int li, int ls, int c) {
  do {
    scanf("%d", n);
  } while ((*n < li || *n > ls) && (*n != c));
}

void flush(FILE *stream) {
  int c;
  do {
    c = getchar();
  } while (c != EOF && c != '\n');
}
void scanfFloat(float *n, float li, float ls, float c) {
  do {
    scanf("%f", n);
  } while ((*n < li || *n > ls) && (*n != c));
}
void fsgets(char t[], int n) {
  int i = 0;
  fgets(t, n, stdin);
  while (t[i] != '\0') {
    if (t[i] == '\n') {
      t[i] = '\0';
    } else {
      i++;
    }
  }
}
void Err(char t[]) {
  puts(t);
  exit(1);
}
