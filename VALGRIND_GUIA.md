# 📘 Guía Completa de Valgrind

## ¿Qué es Valgrind?

Valgrind es una herramienta que detecta problemas de memoria en programas C/C++:
- Fugas de memoria (memory leaks)
- Acceso a memoria no inicializada
- Escritura/lectura fuera de límites (buffer overflow)
- Uso de memoria después de liberarla (use after free)
- Y muchos otros errores

---

## 🚀 Instalación

### Ubuntu/Debian:
```bash
sudo apt install valgrind
```

### Arch Linux:
```bash
sudo pacman -S valgrind
```

### Fedora:
```bash
sudo dnf install valgrind
```

---

## 🔧 Compilación para Valgrind

**IMPORTANTE:** Siempre compila con la flag `-g` para obtener números de línea:

```bash
gcc -g -O0 programa.c -o programa
```

- `-g`: Incluye símbolos de debug
- `-O0`: Desactiva optimizaciones (opcional pero recomendado para debug)

---

## 📖 Comando Básico

```bash
valgrind ./programa
```

### Comando Completo (Recomendado):
```bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./programa
```

**Flags explicadas:**
- `--leak-check=full`: Muestra detalles completos de memory leaks
- `--track-origins=yes`: Te dice DÓNDE se crearon los valores no inicializados
- `--show-reachable=yes`: Muestra memoria que todavía es alcanzable al salir

---

## 📚 Cómo Leer la Salida de Valgrind

### 1. **Errores de Memoria**

#### Ejemplo: Valor no inicializado
```
==404100== Conditional jump or move depends on uninitialised value(s)
==404100==    at 0x4001586: vectorOrdInsertar (main.c:55)
==404100==    by 0x40011F4: main (main.c:21)
==404100==  Uninitialised value was created by a stack allocation
==404100==    at 0x4001199: main (main.c:14)
```

**Cómo leerlo:**
1. **Primera línea:** `Conditional jump...` → Describe el problema (salto condicional usa valor no inicializado)
2. **`at 0x4001586:`** → Dirección de memoria donde ocurrió
3. **`vectorOrdInsertar (main.c:55)`** → Función y línea donde está el error
4. **`by 0x40011F4: main`** → Stack trace: quién llamó a esa función (main línea 21)
5. **`Uninitialised value was created`** → Te dice DÓNDE se creó el valor problemático (main línea 14)

**Interpretación:** En la línea 55 de `main.c`, dentro de `vectorOrdInsertar`, se está usando un valor que nunca se inicializó. Ese valor se creó en la línea 14 de `main`.

---

#### Ejemplo: Lectura inválida
```
==404100== Invalid read of size 8
==404100==    at 0x4001311: cmpEj (main.c:47)
==404100==    by 0x400159B: vectorOrdInsertar (main.c:55)
==404100==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
```

**Qué significa:**
- **`Invalid read of size 8`**: Intentaste leer 8 bytes de memoria inválida
- **`at 0x4001311: cmpEj (main.c:47)`**: El error está en la línea 47 de `main.c`, función `cmpEj`
- **`Address 0x0`**: Intentaste acceder a la dirección NULL (0x0)
- **`not stack'd, malloc'd or free'd`**: Esa memoria NO pertenece a tu programa

**Causa común:** Estás desreferenciando un puntero NULL:
```c
int *p = NULL;
printf("%d", *p);  // ← Esto causa "Invalid read"
```

---

#### Ejemplo: Escritura fuera de límites
```
==391626== Invalid write of size 8
==391626==    at 0x4859893: memmove (vg_replace_strmem.c:1415)
==391626==    by 0x40015F0: vectorOrdInsertar (main.c:58)
==391626==  Address 0x4a6e090 is 0 bytes after a block of size 80 alloc'd
==391626==    at 0x484B7A8: malloc (vg_replace_malloc.c:446)
==391626==    by 0x4001438: crearVector (main.c:22)
```

**Interpretación:**
- **`Invalid write of size 8`**: Escribiste 8 bytes en memoria inválida
- **`at 0x4859893: memmove`**: Ocurrió durante un `memmove`
- **`by 0x40015F0: vectorOrdInsertar`**: Llamado desde `vectorOrdInsertar` línea 58
- **`Address is 0 bytes after a block`**: Escribiste JUSTO después del final del bloque
  - Asignaste 80 bytes con `malloc`
  - Intentaste escribir en la posición 80 (que está fuera)
- **`malloc` en línea 22**: Te dice dónde asignaste esa memoria

**Causa:** Buffer overflow:
```c
int arr[10];
arr[10] = 5;  // ← Fuera de límites (índices válidos: 0-9)
```

---

### 2. **Memory Leaks (Fugas de Memoria)**

```
==404100== LEAK SUMMARY:
==404100==    definitely lost: 8 bytes in 1 blocks
==404100==    indirectly lost: 0 bytes in 0 blocks
==404100==      possibly lost: 0 bytes in 0 blocks
==404100==    still reachable: 0 bytes in 0 blocks
```

**Tipos de leaks:**

| Tipo | Significado | Gravedad |
|------|-------------|----------|
| **definitely lost** | Perdiste el puntero, nunca podrás liberar esta memoria | ❌ MUY MALO |
| **indirectly lost** | Perdiste un puntero padre que apuntaba a esta memoria | ❌ MALO |
| **possibly lost** | Valgrind cree que hay un leak pero no está 100% seguro | ⚠️ REVISAR |
| **still reachable** | Memoria que todavía tienes referenciada al salir del programa | ⚠️ MEJORABLE |

#### Ver dónde está el leak:
```
==404100== 8 bytes in 1 blocks are definitely lost in loss record 1 of 1
==404100==    at 0x484B7A8: malloc (vg_replace_malloc.c:446)
==404100==    by 0x4001416: crearVector (main.c:20)
==404100==    by 0x40011C0: main (main.c:16)
```

**Interpretación:**
- Hiciste `malloc` en `crearVector` (línea 20)
- `crearVector` fue llamado desde `main` (línea 16)
- Nunca hiciste el `free` correspondiente

**Solución:** Agrega `free()` antes de salir:
```c
int main() {
    Vector v;
    crearVector(&v, sizeof(int));
    // ... usar el vector ...
    vectorDestruir(&v);  // ← Asegúrate de llamar esto
    return 0;
}
```

---

### 3. **Resumen Final**

```
==403333== HEAP SUMMARY:
==403333==     in use at exit: 0 bytes in 0 blocks
==403333==   total heap usage: 2 allocs, 2 frees, 4,136 bytes allocated
==403333== 
==403333== All heap blocks were freed -- no leaks are possible
==403333== 
==403333== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

**Qué buscar:**

✅ **Perfecto (sin errores):**
```
in use at exit: 0 bytes in 0 blocks
total heap usage: 2 allocs, 2 frees
ERROR SUMMARY: 0 errors
```

❌ **Hay problemas:**
```
in use at exit: 80 bytes in 1 blocks  ← Hay memoria sin liberar
total heap usage: 3 allocs, 2 frees   ← Más malloc que free
ERROR SUMMARY: 5 errors               ← Hay 5 errores
```

---

## 🎯 Estrategia para Leer Valgrind

### Paso a paso:

1. **Mira el ERROR SUMMARY primero**
   ```
   ERROR SUMMARY: 0 errors → ✅ Todo bien
   ERROR SUMMARY: 5 errors → ❌ Hay problemas
   ```

2. **Si hay errores, busca el PRIMERO que aparece**
   - Los errores subsiguientes suelen ser consecuencia del primero
   - Arregla de arriba hacia abajo

3. **Lee el stack trace de abajo hacia arriba**
   ```
   at 0x4001311: cmpEj (main.c:47)          ← Aquí está el error
   by 0x400159B: vectorOrdInsertar (main.c:55)  ← Quién lo llamó
   by 0x40011F4: main (main.c:21)           ← Quién llamó a ese
   ```
   
   **Estrategia:** 
   - Ve primero a `main.c:47` (donde está el error real)
   - Si no es obvio, mira el contexto en `main.c:55`

4. **Mira los números de línea** 
   - Ve directo al código fuente
   - Usa un editor que muestre números de línea

5. **Al final, revisa el LEAK SUMMARY**
   - Asegúrate de que `definitely lost: 0 bytes`
   - Idealmente también `still reachable: 0 bytes`

---

## 💡 Ejemplos Comunes de Errores

### Error 1: Puntero NULL
```c
int *p = NULL;
*p = 5;  // ← Invalid write
```

**Valgrind dice:**
```
Invalid write of size 4
Address 0x0 is not stack'd, malloc'd or (recently) free'd
```

**Solución:**
```c
int *p = malloc(sizeof(int));
if (p) {
    *p = 5;
    free(p);
}
```

---

### Error 2: Buffer Overflow
```c
int arr[10];
for (int i = 0; i <= 10; i++) {  // ← i=10 está fuera de límites
    arr[i] = i;
}
```

**Valgrind dice:**
```
Invalid write of size 4
Address 0x... is 0 bytes after a block of size 40 alloc'd
```

**Solución:**
```c
for (int i = 0; i < 10; i++) {  // ← Cambiar <= por <
    arr[i] = i;
}
```

---

### Error 3: Uso de valor no inicializado
```c
int x;
if (x > 0) {  // ← x nunca se inicializó
    printf("Positivo\n");
}
```

**Valgrind dice:**
```
Conditional jump or move depends on uninitialised value(s)
```

**Solución:**
```c
int x = 0;  // ← Inicializar
if (x > 0) {
    printf("Positivo\n");
}
```

---

### Error 4: Memory Leak
```c
void funcion() {
    int *p = malloc(sizeof(int) * 10);
    // ... usar p ...
    // ← Falta free(p)
}
```

**Valgrind dice:**
```
40 bytes in 1 blocks are definitely lost
```

**Solución:**
```c
void funcion() {
    int *p = malloc(sizeof(int) * 10);
    // ... usar p ...
    free(p);  // ← Agregar free
}
```

---

### Error 5: Use After Free
```c
int *p = malloc(sizeof(int));
*p = 5;
free(p);
*p = 10;  // ← Error: usar memoria después de liberarla
```

**Valgrind dice:**
```
Invalid write of size 4
Address 0x... is 0 bytes inside a block of size 4 free'd
```

**Solución:**
```c
int *p = malloc(sizeof(int));
*p = 5;
free(p);
p = NULL;  // ← Buena práctica
// No usar p después de free
```

---

### Error 6: Double Free
```c
int *p = malloc(sizeof(int));
free(p);
free(p);  // ← Error: liberar dos veces
```

**Valgrind dice:**
```
Invalid free() / delete / delete[] / realloc()
Address 0x... is 0 bytes inside a block of size 4 free'd
```

**Solución:**
```c
int *p = malloc(sizeof(int));
free(p);
p = NULL;  // ← Así el segundo free no hace nada
if (p) free(p);  // Seguro
```

---

## 🔍 Flags Útiles de Valgrind

### Básicas:
```bash
valgrind --leak-check=full ./programa
```

### Completas (Recomendado):
```bash
valgrind \
  --leak-check=full \
  --show-leak-kinds=all \
  --track-origins=yes \
  --verbose \
  --log-file=valgrind-out.txt \
  ./programa
```

**Flags explicadas:**

| Flag | Descripción |
|------|-------------|
| `--leak-check=full` | Muestra detalles completos de leaks |
| `--show-leak-kinds=all` | Muestra todos los tipos de leaks |
| `--track-origins=yes` | Rastrea dónde se crearon valores no inicializados |
| `--verbose` | Más información detallada |
| `--log-file=archivo.txt` | Guarda la salida en un archivo |
| `--suppressions=archivo.supp` | Ignora errores conocidos/falsos positivos |

### Para programas con entrada:
```bash
valgrind --leak-check=full ./programa < entrada.txt
```

### Para ver el stack trace completo:
```bash
valgrind --leak-check=full --num-callers=20 ./programa
```

---

## 🧪 Casos Especiales

### Valgrind con argumentos:
```bash
valgrind --leak-check=full ./programa arg1 arg2 arg3
```

### Valgrind con Makefile:
```makefile
valgrind: programa
	valgrind --leak-check=full --track-origins=yes ./programa

clean:
	rm -f programa *.o
```

Luego:
```bash
make valgrind
```

---

## 🐛 Problema Común: Underflow en size_t

### Código problemático:
```c
size_t n = 0;
void *ptr = arr + (n - 1) * sizeof(int);  // ← n-1 = SIZE_MAX (overflow)
```

**Por qué falla:**
- `size_t` es **sin signo** (unsigned)
- `0 - 1` NO da `-1`, da `18446744073709551615` (SIZE_MAX)
- El puntero apunta a memoria inválida

**Valgrind dice:**
```
Invalid write of size 8
Address 0x... is 0 bytes after a block of size 80 alloc'd
```

**Solución:**
```c
// Opción 1: Verificar antes de restar
if (n > 0) {
    void *ptr = arr + (n - 1) * sizeof(int);
}

// Opción 2: Usar el límite directamente
void *ptr = arr + n * sizeof(int);  // Apunta al final, no necesitas -1
```

---

## 📊 Interpretación de Direcciones

### Tipos de direcciones que muestra Valgrind:

```
Address 0x0 → NULL pointer
Address 0x4a6e090 is 0 bytes after a block → Justo después del array
Address 0x4a6e088 is 8 bytes before a block → Antes del inicio
Address 0x4a6e050 is 0 bytes inside a block → Dentro del bloque pero ya fue free'd
```

---

## ✅ Checklist para un Programa Sin Errores

- [ ] Compilé con `-g` flag
- [ ] `ERROR SUMMARY: 0 errors`
- [ ] `definitely lost: 0 bytes`
- [ ] `indirectly lost: 0 bytes`
- [ ] Mismo número de `allocs` que de `frees`
- [ ] `in use at exit: 0 bytes`
- [ ] No hay "Invalid read/write"
- [ ] No hay "Conditional jump depends on uninitialised value"

---

## 🎓 Mejores Prácticas

### 1. Siempre inicializa variables:
```c
int x = 0;  // ✅ Correcto
int x;      // ❌ No inicializado
```

### 2. Verifica malloc antes de usar:
```c
int *p = malloc(sizeof(int));
if (!p) {
    perror("malloc falló");
    return -1;
}
*p = 5;
free(p);
```

### 3. Pon punteros en NULL después de free:
```c
free(p);
p = NULL;
```

### 4. Usa sizeof con variables, no tipos:
```c
Ejemplo *e = malloc(sizeof(*e));  // ✅ Mejor
Ejemplo *e = malloc(sizeof(Ejemplo));  // ✅ También correcto
```

### 5. Verifica límites de arrays:
```c
for (int i = 0; i < n; i++) {  // ✅ Correcto: i < n
    arr[i] = i;
}

for (int i = 0; i <= n; i++) {  // ❌ Fuera de límites cuando i == n
    arr[i] = i;
}
```

---

## 📝 Resumen Rápido

### ¿Programa sin errores?
```bash
valgrind --leak-check=full ./programa
# Busca: ERROR SUMMARY: 0 errors
```

### ¿De dónde vienen los valores no inicializados?
```bash
valgrind --track-origins=yes ./programa
```

### ¿Guardar output en archivo?
```bash
valgrind --leak-check=full --log-file=valgrind.log ./programa
```

### ¿Verificar memoria al final?
Busca en la salida:
```
in use at exit: 0 bytes
All heap blocks were freed
```

---

## 🔗 Referencias

- [Documentación oficial de Valgrind](https://valgrind.org/docs/manual/manual.html)
- [Quick Start Guide](https://valgrind.org/docs/manual/quick-start.html)
- [FAQ de Valgrind](https://valgrind.org/docs/manual/faq.html)

---

**¡Feliz debugging! 🐛🔨**
