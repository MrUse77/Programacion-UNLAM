#include "myShortCuts.h"
#include <stdio.h>
void ordenar(int a[]) {
  int i, j, aux;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9 - i; j++) {
      if (a[j] > a[j + 1]) {
        aux = a[j];
        a[j] = a[j + 1];
        a[j + 1] = aux;
      }
    }
  }
}
int main() {
  int a[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0}, c;

  char b[10];
  printf("escribir un numero\n");
  scanf("%d", &c);
  printf(" %d ", c);
  printf("escribir una palabra\n");
  flush(stdin);
  fsgets(b, 10);
  printf(" %s ", b);
  ordenar(a);
  for (int i = 0; i < 10; i++) {
    printf("%d", a[i]);
  }
  return 0;
}
