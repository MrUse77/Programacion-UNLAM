# Testing de TDAs

Guía para escribir tests completos de los Tipos de Datos Abstractos.

## Filosofía: TDD (Test-Driven Development)

### ¿Qué es TDD?

**Test-Driven Development** es escribir el **test ANTES** que el código.

**Flujo de trabajo**:

```
1. RED   → Escribís un test que falla (la función no existe)
2. GREEN → Escribís el código MÍNIMO para que pase
3. REFACTOR → Mejorás el código sin romper el test
4. REPETIR
```

**Ejemplo práctico**:

```c
// 1. RED - Escribís el test primero
TEST(pila_vacia_retorna_true) {
    t_Pila p;
    crearPila(&p);
    ASSERT_TRUE(pilaVacia(&p), "Pila recién creada debe estar vacía");
    TEST_PASSED("Pila vacía funciona");
}
// ❌ Esto FALLA porque pilaVacia() no existe aún

// 2. GREEN - Implementás lo mínimo
int pilaVacia(t_Pila* p) {
    return p->tope == 0;
}
// ✅ Ahora el test PASA

// 3. REFACTOR - Mejorás si es necesario
// (en este caso, el código ya está bien)

// 4. REPETIR con la siguiente funcionalidad
```

### ¿Por qué TDD?

- **Especificación clara**: El test ES la especificación de lo que debe hacer la función
- **Confianza**: Si los tests pasan, sabés que el código funciona
- **Regresión**: Si cambiás algo y rompés un test, lo ves INMEDIATAMENTE
- **Diseño**: Te obliga a pensar en **cómo se usa** la función antes de implementarla

---

## Casos Fronteras (Edge Cases)

Son las **situaciones límite** donde las cosas se rompen. Para cada TDA, identificá:

### Para una **Pila**:

| Caso | Descripción | Test |
|------|-------------|------|
| **Vacía** | Pila recién creada | `pilaVacia()` debe retornar TRUE |
| **Desapilar vacía** | No hay elementos | Debe retornar `ERR` y no crashear |
| **Un elemento** | Borde entre vacía/no vacía | Apilar 1, desapilar 1 → queda vacía |
| **Llena** | Capacidad máxima (pila estática) | `apilar()` debe retornar `ERR` |
| **Overflow** | Apilar cuando está llena | No debe corromper memoria |
| **Tamaños variables** | `int`, `struct`, etc. | Debe manejar cualquier `sizeof()` |
| **LIFO** | Last In, First Out | Apilar 1,2,3 → Desapilar 3,2,1 |
| **verTope()** | Ver sin modificar | La pila NO debe cambiar |

### Para una **Cola**:

| Caso | Descripción | Test |
|------|-------------|------|
| **Vacía** | Cola recién creada | `colaVacia()` == TRUE |
| **Desencolar vacía** | No hay elementos | Retorna `ERR` |
| **FIFO** | First In, First Out | Encolar 1,2,3 → Desencolar 1,2,3 |
| **Circular** | Cola circular llena que da la vuelta | Debe manejar wrap-around |

### Para una **Lista**:

| Caso | Descripción | Test |
|------|-------------|------|
| **Vacía** | Lista recién creada | `listaVacia()` == TRUE |
| **Insertar al inicio** | Primer nodo | `inicio->dato` == valor insertado |
| **Insertar al final** | Último nodo | Recorrer hasta el final |
| **Eliminar único** | Lista con 1 elemento | Queda vacía después |
| **Buscar inexistente** | Elemento no está | Retorna NULL o ERR |
| **Memoria** | Múltiples inserciones/eliminaciones | No debe haber leaks |

---

## Framework de Testing

Usamos un mini-framework casero (ver `TDAs/test_framework.h`) que provee:

### Macros principales:

```c
TEST(nombre)                    // Define un test
RUN_TEST(nombre)                // Ejecuta un test
ASSERT_TRUE(cond, msg)          // Verifica que sea TRUE
ASSERT_FALSE(cond, msg)         // Verifica que sea FALSE
ASSERT_EQUAL(expected, actual, msg)  // Verifica igualdad
TEST_PASSED(msg)                // Marca test como exitoso
TEST_SUMMARY()                  // Muestra resumen final
```

### Estructura de un test:

```c
TEST(nombre_descriptivo_del_test) {
    // ARRANGE (preparar)
    t_Pila p;
    int valor = 42;
    
    // ACT (actuar)
    crearPila(&p);
    int resultado = apilar(&p, &valor, sizeof(valor));
    
    // ASSERT (verificar)
    ASSERT_EQUAL(OK, resultado, "apilar() debe retornar OK");
    ASSERT_FALSE(pilaVacia(&p), "Pila NO debe estar vacía");
    
    TEST_PASSED("Apilar funciona correctamente");
}
```

**Patrón AAA**:
- **Arrange**: Preparás los datos
- **Act**: Ejecutás la operación
- **Assert**: Verificás el resultado

---

## Ejemplo completo: Test de Pila

Ver `TDAs/Pila/Test/test_pila.c` para un ejemplo completo con:

✅ Test de pila vacía  
✅ Test de apilar/desapilar  
✅ Test de orden LIFO  
✅ Test de pila llena  
✅ Test con estructuras complejas  
✅ Test de `verTope()` sin modificar  
✅ Test de `vaciarPila()`  

### Estructura del proyecto

Cada TDA tiene los tests **en el mismo `.cbp`** usando **targets separados**:

```
TDAs/Pila/
├── Pila.h              # Header del TDA
├── Pila.c              # Implementación
├── Pila.cbp            # Proyecto con 3 targets:
│   ├── Debug           → Static library
│   ├── Release         → Static library
│   └── Test            → Console application (ejecuta los tests)
└── Test/
    └── test_pila.c     # Archivo de tests
```

### Compilar y ejecutar los tests

**En Code::Blocks**:
1. Abrí `Pila.cbp`
2. Cambiá el target de `Debug` a `Test` (dropdown en la barra de herramientas)
3. Build & Run (F9)

**Por terminal**:
```bash
cd TDAs/Pila
gcc -o Test/TestPila Test/test_pila.c Pila.c -I. -I.. -Wall
./Test/Testila
```

**Output esperado**:

```
╔════════════════════════════════════════╗
║   TEST SUITE: Pila (TDA)              ║
╔════════════════════════════════════════╝

[TEST] pila_recien_creada_esta_vacia...
  ✓ PASSED: Pila vacía después de crearPila()

[TEST] apilar_un_elemento_pila_no_vacia...
  ✓ PASSED: Apilar un elemento funciona correctamente

...

========================================
TEST SUMMARY
========================================
Total:  9
Passed: 9
Failed: 0
========================================
```

---

## Checklist: ¿Qué testear en un TDA?

Para **CADA función** del TDA, preguntate:

- [ ] ¿Qué pasa si la estructura está **vacía**?
- [ ] ¿Qué pasa si está **llena** (si es estática)?
- [ ] ¿Qué pasa con **un solo elemento**?
- [ ] ¿Funciona con **múltiples elementos**?
- [ ] ¿Maneja correctamente **distintos tamaños** (`sizeof`)?
- [ ] ¿Retorna los **códigos de error** correctos?
- [ ] ¿Respeta el **invariante** del TDA? (LIFO para Pila, FIFO para Cola, etc.)
- [ ] ¿Hay **fugas de memoria** (memory leaks)?

---

## Template para nuevos tests

```c
#include <stdio.h>
#include "../NombreTDA.h"
#include "../test_framework.h"

// ============================================
// TESTS
// ============================================

TEST(estructura_vacia_al_crear) {
    // ARRANGE
    t_NombreTDA estructura;
    
    // ACT
    crear(&estructura);
    
    // ASSERT
    ASSERT_TRUE(estaVacia(&estructura), "Debe estar vacía al crear");
    TEST_PASSED("Creación correcta");
}

// ... más tests

// ============================================
// MAIN
// ============================================

int main() {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║   TEST SUITE: NombreTDA               ║\n");
    printf("╔════════════════════════════════════════╝\n");
    
    RUN_TEST(estructura_vacia_al_crear);
    // ... ejecutar más tests
    
    TEST_SUMMARY();
}
```

---

## Recursos

- **Test de Pila**: `TDAs/Pila/Test/test_pila.c`
- **Framework**: `TDAs/test_framework.h`
- **Guía de TDD**: [Test-Driven Development](https://en.wikipedia.org/wiki/Test-driven_development)

**Regla de oro**: Si no tiene tests, no funciona. Si tiene tests y pasan, probablemente funciona.
