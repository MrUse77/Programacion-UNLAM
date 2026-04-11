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

1. **Generar `compile_commands.json`** (necesario para que clangd encuentre los headers):

   ```bash
   cd Algoritmos/
   ./gen_compile_commands.sh
   ```

   Esto crea un archivo `compile_commands.json` con **paths absolutos** adaptados a tu máquina.

2. **Verificar que funciona**:

   ```bash
   nvim Ejercicios/Repaso/EJ\ 7/main.c
   ```

   El LSP (clangd) debería reconocer los `#include "Pila.h"` sin errores.

### ¿Por qué este script?

- **Problema**: Los TDAs están en `TDAs/Pila/`, `TDAs/Vector/`, etc., pero cada ejercicio está en subcarpetas profundas como `Ejercicios/Repaso/EJ1/`.
- **Sin configuración**: clangd no encuentra los headers (`'Pila.h' file not found`).
- **Solución**: `compile_commands.json` le dice a clangd **desde dónde** buscar los includes.
- **Portable**: El script genera paths absolutos basados en **tu** ubicación del repo, funciona en desktop y notebook.

### Archivos importantes

- **`.clangd`**: Configuración de clangd (versionada en Git).
- **`compile_commands.json`**: Generado localmente, **NO versionado** (cada máquina genera el suyo).
- **`gen_compile_commands.sh`**: Script para generar `compile_commands.json` portable.

### Uso de TDAs en tus ejercicios

En tus archivos `.c`:

```c
#include "Pila.h"      // ✅ Correcto
#include "vector.h"    // ✅ Correcto
```

**NO** uses:
```c
#include "../../TDAs/Pila/Pila.h"  // ❌ Paths relativos feos
#include "include/Pila.h"           // ❌ Subcarpeta inexistente
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

2. Agregar el path en `gen_compile_commands.sh`:
   ```bash
   -ITDAs/NuevoTDA
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
