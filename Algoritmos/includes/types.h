#ifndef COMUN_H_INCLUDED
#define COMUN_H_INCLUDED

/**
 * @def MIN
 * @brief verifica cual es el menor entre 2 elementos
 */
#define MIN(a, b) (a > b ? b : a)
#define MAX(a, b) (a < b ? b : a)

typedef enum { TRUE = 1, FALSE = 0 } bool_t;
typedef enum {
  SELECCION = 1,
  BURBUJEO = 2,
  INSERCION = 3,
  QUICK = 4,
  MERGE = 5,
  RADIX = 6
} order_t;

typedef int (*cmp_t)(const void *ctx, const void *d);
typedef void (*accion_t)(const void *param, void *dato);
typedef void (*reduce_t)(const void *param, void *dato, void *user);
typedef void (*map_t)(const void *from, void *to, void *user);
typedef void (*filter_t)(const void *param, void *dato, void *user);
typedef void (*prnt_t)(const void *dato);
typedef int (*acum_t)(void **, unsigned *, const void *, unsigned);
typedef unsigned (*read_t)(void **, void *, unsigned, void *params);

#endif
