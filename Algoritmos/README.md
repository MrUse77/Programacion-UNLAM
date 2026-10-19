# Algoritmos - UNLAM

Ejercicios y TDAs (Tipos de Datos Abstractos) de la materia Algoritmos.

## Estructura del proyecto

```
Algoritmos/
├── TDAs/                         # Tipos de Datos Abstractos
│   ├── Pila/
│   │   ├── Pila.h
│   │   ├── Pila.c
│   │   └── lib/libPila.a
│   └── Vector/
│       ├── include/vector.h
│       ├── vector.c
│       └── lib/libVector.a
│
├── Ejercicios/                   # Ejercicios organizados por unidad
│   ├── Repaso/
│   │   ├── EJ1/
│   │   ├── EJ2/
│   │   └── ...
│   ├── Unidad1/
│   ├── Unidad2/
│   ├── Unidad3/
│   ├── Unidad4/
│   ├── Unidad5/
│   └── Unidad6/
│
├── TP/                           # Trabajos Prácticos
│
├── Algoritmos.workspace          # Workspace maestro (TODO)
├── TDAs.workspace                # Solo TDAs
├── Repaso.workspace              # Solo ejercicios de Repaso
├── Unidad1.workspace             # Ejercicios Unidad 1
├── Unidad2.workspace             # ...
└── ...
```

## Workspaces de Code::Blocks

Los proyectos están organizados en **workspaces temáticos**:

- **`Algoritmos.workspace`**: Workspace maestro con TODO (TDAs + Ejercicios actuales)
- **`TDAs.workspace`**: Solo los TDAs (Pila, Vector, etc.)
- **`Repaso.workspace`**: Ejercicios de repaso
- **`Unidad1.workspace`** a **`Unidad6.workspace`**: Ejercicios por unidad

**Para abrir en Code::Blocks**: Click derecho en el `.workspace` → Abrir con Code::Blocks

## Setup para desarrollo con Neovim/LSP

Este proyecto está configurado para trabajar con **Code::Blocks** (requerido por la universidad) y **Neovim + clangd** (desarrollo local).

### Primera vez (después de clonar)

No hace falta configurar nada: **`compile_flags.txt` está versionado** y es el fallback que clangd usa cuando no hay `compile_commands.json`. Con eso alcanza para que encuentre los headers.

Opcionalmente, podés generar `compile_commands.json` (tiene prioridad sobre `compile_flags.txt` y es el camino recomendado si querés flags distintas por archivo):

```bash
cd Algoritmos/
./gen_compile_commands.sh
```

Esto crea un archivo `compile_commands.json` con **paths absolutos** adaptados a tu máquina.

**Verificar que funciona**:

```bash
clangd --check=TDAs/Arbol/main.c
```

Debe terminar con `All checks completed, 0 errors`. En Neovim, abrí `TDAs/Arbol/main.c`: el LSP (clangd) no debería marcar el `#include <binary_search_tree.h>`.

### ¿Por qué hay dos archivos de configuración?

- **Problema**: los headers viven en `includes/`, pero los `.c` están en subcarpetas profundas como `Ejercicios/Repaso/EJ1/`. Sin `-I`, clangd no los encuentra (`'binary_search_tree.h' file not found`).
- **Solución**: `-I` con la ruta de `includes/`.
- **`compile_flags.txt`**: versionado, portable, y clangd resuelve sus rutas relativas contra el directorio del propio archivo. Es el fallback y funciona desde cualquier cwd.
- **`compile_commands.json`**: opcional, lo genera `gen_compile_commands.sh` con paths absolutos. Si existe, clangd lo usa en lugar de `compile_flags.txt`.

> Ojo: los paths relativos de un `compile_commands.json` se resuelven contra su campo `directory`, no contra el cwd de Neovim. Por eso el script los emite absolutos.

### Archivos importantes

- **`.clangd`**: flags generales de clangd (versionado en Git).
- **`compile_flags.txt`**: fallback con el `-I` de `includes/` (versionado en Git).
- **`compile_commands.json`**: generado localmente, **NO versionado** (cada máquina genera el suyo).
- **`gen_compile_commands.sh`**: script para generar `compile_commands.json` portable.

### Uso de TDAs en tus ejercicios

En tus archivos `.c`:

```c
#include <binary_search_tree.h>  // ✅ Correcto
#include <types.h>               // ✅ Correcto
```

**NO** uses:
```c
#include "../../includes/binary_search_tree.h"  // ❌ Paths relativos feos
#include "includes/types.h"                     // ❌ Subcarpeta inexistente
```

### Code::Blocks

**IMPORTANTE**: Todos los proyectos de ejercicios ya están configurados para encontrar los TDAs automáticamente. Los paths están configurados como:

- **Search directories → Compiler**: `../../../TDAs/Pila`, `../../../TDAs/Vector/include`
- **Search directories → Linker**: `../../../TDAs/Pila/lib`, `../../../TDAs/Vector/lib`

**No necesitás copiar archivos** — los ejercicios referencian los TDAs desde su ubicación central.

### Crear un nuevo ejercicio

1. Crear la carpeta:
   ```bash
   mkdir -p Ejercicios/Unidad1/EJ1
   ```

2. Copiar un `.cbp` existente como template:
   ```bash
   cp Ejercicios/Repaso/EJ1/EJ1.cbp Ejercicios/Unidad1/EJ1/
   ```

3. Editar el `.cbp` y cambiar el nombre del proyecto (línea `<Option title="...">`).

4. Agregar al workspace correspondiente (`Unidad1.workspace`):
   ```xml
   <Project filename="Ejercicios/Unidad1/EJ1/EJ1.cbp" />
   ```

## Agregar un nuevo TDA

1. Crear la carpeta en `TDAs/`:
   ```bash
   mkdir -p TDAs/NuevoTDA
   ```

2. Si el TDA deja headers en `TDAs/NuevoTDA/`, agregá el directorio al array `INCLUDE_DIRS` de `gen_compile_commands.sh` y sumá el `-I` correspondiente en `compile_flags.txt`:
   ```bash
   INCLUDE_DIRS=(
     "$SCRIPT_DIR/includes"
     "$SCRIPT_DIR/TDAs/NuevoTDA"
   )
   ```

3. Regenerar `compile_commands.json`:
   ```bash
   ./gen_compile_commands.sh
   ```

4. Reiniciar el LSP en Neovim:
   ```
   :LspRestart
   ```

## Testing de TDAs

**IMPORTANTE**: Cada TDA debe tener tests que cubran casos fronteras.

Ver la guía completa en **[`TDAs/TESTING.md`](TDAs/TESTING.md)** que incluye:

- ✅ **TDD (Test-Driven Development)**: Escribir tests ANTES que el código
- ✅ **Casos fronteras**: Vacío, lleno, un elemento, overflow, etc.
- ✅ **Framework de testing**: `test_framework.h` con macros útiles
- ✅ **Ejemplo completo**: `TDAs/Pila/Test/test_pila.c`

**Ejecutar tests de Pila**:

```bash
cd TDAs/Pila/Test
gcc -o test_pila test_pila.c ../Pila.c -I../ -I../../ -Wall
./test_pila
```

**Qué testear** (mínimo para cada TDA):
- Estructura vacía
- Agregar un elemento
- Agregar y sacar el mismo elemento
- Sacar de estructura vacía (debe retornar error)
- Estructura llena (si es estática)
- Orden correcto (LIFO para Pila, FIFO para Cola)

---

**Nota**: `compile_commands.json` está en `.gitignore` porque contiene paths absolutos específicos de cada máquina. Siempre corré `gen_compile_commands.sh` después de clonar o cambiar de computadora.
