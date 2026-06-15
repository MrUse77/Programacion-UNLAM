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

typedef int (*cmp_t)(const void *ctx, const void *d);
typedef void (*accion_t)(void *param, const void *dato);
typedef void (*prnt_t)(const void *dato);
typedef int (*acum_t)(void **, unsigned *, const void *, unsigned);
typedef unsigned (*read_t)(void **, void *, unsigned, void *params);

#endif
