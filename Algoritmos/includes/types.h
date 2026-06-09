#ifndef COMUN_H_INCLUDED
#define COMUN_H_INCLUDED

/**
 * @def MIN
 * @brief verifica cual es el menor entre 2 elementos
 */
#define MIN(a, b) (a > b ? b : a)
#define MAX(a, b) (a < b ? b : a)

/**
 * @def TRUE
 * @brief Valor verdadero.
 */
#define TRUE 1

/**
 * @def FALSE
 * @brief Valor falso.
 */
#define FALSE 0

typedef int (*t_Cmp)(const void *a, const void *b);
typedef void (*t_Accion)(void *param, const void *dato);
typedef void (*t_Prnt)(const void *dato);
typedef int (*t_Acum)(void **, unsigned *, const void *, unsigned);

#endif
