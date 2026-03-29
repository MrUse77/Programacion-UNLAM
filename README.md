# Ejercicios de Universidad 🧠📚

Este repositorio contiene una colección de ejercicios prácticos realizados durante mi carrera de Ingeniería Informática. Están organizados por materia y cuatrimestre, y abarcan diferentes áreas de la informática, desde programación hasta sistemas operativos.

## 📁 Estructura del Repositorio

```bash
.
├── BasesDeDatos/
├── SistemasOperativos/
├── TopicosDeProgramacion/
├── Algoritmos/
├── ProgramacionEstructuradaBasica/
├── ProgramacionInicial/
├── flake.nix
├── flake.lock
├── .envrc
└── README.md
```
## Submodulos
El repositorio cuenta con algunos submodulos (por algunos TPs o diferentes ejercicios que necesiten su propio Repositorio)
Para traer todo junto:

```bash
git clone --recursive https://github.com/MrUse77/Programacion-UNLAM
```

Si ya tenes el repositorio bastara con hacer:

```bash
git submodule update --init --recursive
```
Tambien debe hacerse luego de un git pull o usar:

```bash
git config --global submodule.recurse true
```



## 🛠️ Entorno de Desarrollo con Nix

Este repositorio utiliza **Nix Flakes** para proporcionar un entorno de desarrollo reproducible y consistente para C/C++, ideal para las materias de programación de la UNLaM.



### ¿Qué incluye el entorno?

- **gcc**: Compilador de C/C++
- **gdb**: Depurador de GNU
- **gnumake**: Herramienta de automatización de compilación
- **binutils**: Utilidades binarias (ld, as, etc.)
- **codeblocks**: IDE utilizado en la universidad
- **valgrind**: Herramienta para detectar fugas de memoria

### Requisitos previos

1. **Instalar Nix** (si aún no lo tienes):
   ```bash
   sh <(curl -L https://nixos.org/nix/install) --daemon
   ```

2. **Habilitar Flakes** (añade esto a `~/.config/nix/nix.conf`):
   ```
   experimental-features = nix-command flakes
   ```

3. **Instalar direnv** (opcional pero recomendado):
   ```bash
   # En NixOS o con Nix
   nix-env -iA nixpkgs.direnv
   
   # O con tu gestor de paquetes
   # Ubuntu/Debian: sudo apt install direnv
   # macOS: brew install direnv
   ```

4. **Configurar direnv en tu shell** (añade a `~/.bashrc` o `~/.zshrc`):
   ```bash
   eval "$(direnv hook bash)"  # Para bash
   # o
   eval "$(direnv hook zsh)"   # Para zsh
   ```

### Uso

#### Opción 1: Con direnv (recomendado - automático)

Si instalaste direnv, el entorno se activará automáticamente al entrar al directorio:

```bash
cd /ruta/al/repositorio
# direnv carga automáticamente el entorno
```

La primera vez necesitarás permitir el archivo `.envrc`:

```bash
direnv allow
```

A partir de ese momento, cada vez que entres al directorio, el entorno se activará automáticamente, y se desactivará al salir. ¡No más `nix develop` manual!

#### Opción 2: Manualmente con nix develop

Para activar el entorno con todas las herramientas:

```bash
nix develop
```

Una vez dentro del entorno, tendrás acceso a:
- Compilar: `gcc archivo.c -o programa`
- Depurar: `gdb ./programa`
- Verificar memoria: `valgrind ./programa`
- Abrir IDE: `codeblocks`

#### Salir del entorno (solo si usas nix develop manual)

```bash
exit
```

### Ventajas de usar Nix + direnv

- **Reproducibilidad**: El mismo entorno en cualquier máquina
- **Aislamiento**: No interfiere con el resto del sistema
- **Sin instalación global**: No necesitas instalar gcc, gdb, etc. en tu sistema
- **Versiones específicas**: Siempre usarás las mismas versiones de las herramientas
- **Activación automática** (con direnv): El entorno se carga al entrar al directorio
- **Sin contaminar el shell**: Al salir del directorio, vuelves a tu entorno normal
