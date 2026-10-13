# ============================================================================
#  ListaSimple - TDA Lista Simple
#  Espejo por linea de comandos del proyecto Code::Blocks (ListaSimple.cbp)
#
#  Correr siempre desde este directorio:  cd TDAs/ListaSimple && make
#
#  Target en Code::Blocks     Tipo          Salida aca
#  ---------------------------------------------------------------------------
#  Linux        (activo)      lib estatica  ../../libs/Linux/libListaSimple.a
#  TestLinux                  ejecutable    bin/Test/Linux/ListaSimple
#  Windows                    lib estatica  ../../libs/Windows/libListaSimple.a
#  TestWindows                ejecutable    bin/Test/Windows/ListaSimple.exe
#  WyL          (virtual)     Linux;Windows -> make wyl
#
#  Atajos: make | make test | make run | make wyl | make clean
# ============================================================================

# --- Herramientas -----------------------------------------------------------
# El target "Windows" del .cbp compila con mingw-w64 (gcc cruzado).
CC      ?= gcc
AR      ?= ar
ARFLAGS := rcs

CROSS  ?= x86_64-w64-mingw32-
WIN_CC := $(CROSS)gcc
WIN_AR := $(CROSS)ar

# --- Rutas compartidas del repo (identicas a las del .cbp) ------------------
INCLUDES := ../../includes

# --- Flags: espejo de las opciones de compilador de cada target del .cbp ----
LIBC_CFLAGS := -Wall -g          # target "Linux"
TEST_CFLAGS := -g                # target "TestLinux"
WIN_CFLAGS  := -Wextra -g        # targets "Windows"/"TestWindows"
# Nota: el .cbp agrega "-nostdinc -I/usr/x86_64-w64-mingw32/include
# -I/usr/lib/gcc/x86_64-w64-mingw32/16.1.0/include" en los targets Windows.
# Eso es un parche de la shell de Nix (hardcodea la version de GCC), no se
# replica: con mingw-w64 bien instalado el compilador cruzado ya busca sus
# propios headers. Si hiciera falta, podes sobreescribir WIN_CFLAGS.

# -MMD -MP deja un .d al lado de cada .o: equivalente al ListaSimple.depend
# que mantiene Code::Blocks (si toco un header, recompila lo que depende).
DEPFLAGS := -MMD -MP
CPPFLAGS := -I$(INCLUDES)

# --- Target Linux: biblioteca estatica --------------------------------------
LIB_DIR := ../../libs/Linux
LIB     := $(LIB_DIR)/libListaSimple.a
OBJ_DIR := obj/Debug
OBJS    := $(OBJ_DIR)/main.o
DEPS    := $(OBJS:.o=.d)

# --- Target TestLinux: ejecutable de la suite de tests ----------------------
TEST_OBJ_DIR := obj/Test/Linux
TEST_OBJ     := $(TEST_OBJ_DIR)/test_lista_simple.o
TEST_DEPS    := $(TEST_OBJ:.o=.d)
TEST_BIN_DIR := bin/Test/Linux
TEST_BIN     := $(TEST_BIN_DIR)/ListaSimple

# --- Targets Windows y TestWindows: compilacion cruzada ---------------------
WIN_LIB_DIR := ../../libs/Windows
WIN_LIB     := $(WIN_LIB_DIR)/libListaSimple.a
WIN_OBJ_DIR := obj/Debug-windows
WIN_OBJS    := $(WIN_OBJ_DIR)/main.o
WIN_DEPS    := $(WIN_OBJS:.o=.d)

WIN_TEST_OBJ_DIR := obj/Test/Windows
WIN_TEST_OBJ     := $(WIN_TEST_OBJ_DIR)/test_lista_simple.o
WIN_TEST_DEPS    := $(WIN_TEST_OBJ:.o=.d)
WIN_TEST_BIN_DIR := bin/Test/Windows
WIN_TEST_BIN     := $(WIN_TEST_BIN_DIR)/ListaSimple.exe
# El .cbp linkea TestWindows con -L../../libs (sin /Windows); aca se usa
# ../../libs/Windows, que es donde el propio target "Windows" deja el .a.

.PHONY: all linux windows wyl test test-linux test-windows run check-cross \
        clean help

# --- Meta targets -----------------------------------------------------------
all: linux                       # Code::Blocks arranca en el target "Linux"

linux: $(LIB)

wyl: linux windows               # target virtual "WyL" del .cbp

windows: check-cross $(WIN_LIB)

test: test-linux                 # alias corto
test-linux: $(TEST_BIN)
test-windows: check-cross $(WIN_TEST_BIN)

run: $(TEST_BIN)
	./$(TEST_BIN)

check-cross:
	@command -v $(WIN_CC) >/dev/null 2>&1 || \
	  { echo "ERROR: falta $(WIN_CC); instalá mingw-w64 o usá 'make linux'"; exit 1; }

# --- Reglas: Linux / TestLinux ---------------------------------------------
$(LIB): $(OBJS) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(LIBC_CFLAGS) $(DEPFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_OBJ) $(LIB) | $(TEST_BIN_DIR)
	$(CC) $(TEST_CFLAGS) $< -L$(LIB_DIR) -lListaSimple -o $@

$(TEST_OBJ_DIR)/%.o: Test/%.c | $(TEST_OBJ_DIR)
	$(CC) $(CPPFLAGS) $(TEST_CFLAGS) $(DEPFLAGS) -c $< -o $@

# --- Reglas: Windows / TestWindows -----------------------------------------
$(WIN_LIB): $(WIN_OBJS) | $(WIN_LIB_DIR)
	$(WIN_AR) $(ARFLAGS) $@ $^

$(WIN_OBJ_DIR)/%.o: %.c | $(WIN_OBJ_DIR)
	$(WIN_CC) $(CPPFLAGS) $(WIN_CFLAGS) $(DEPFLAGS) -c $< -o $@

$(WIN_TEST_BIN): $(WIN_TEST_OBJ) $(WIN_LIB) | $(WIN_TEST_BIN_DIR)
	$(WIN_CC) $(WIN_CFLAGS) $< -L$(WIN_LIB_DIR) -lListaSimple -o $@

$(WIN_TEST_OBJ_DIR)/%.o: Test/%.c | $(WIN_TEST_OBJ_DIR)
	$(WIN_CC) $(CPPFLAGS) $(WIN_CFLAGS) $(DEPFLAGS) -c $< -o $@

# --- Directorios de trabajo (no ensucian si ya existen) ---------------------
$(LIB_DIR) $(WIN_LIB_DIR) $(OBJ_DIR) $(WIN_OBJ_DIR) \
$(TEST_OBJ_DIR) $(WIN_TEST_OBJ_DIR) $(TEST_BIN_DIR) $(WIN_TEST_BIN_DIR):
	@mkdir -p $@

# --- Limpieza: solo lo que produce este Makefile ---------------------------
clean:
	rm -rf $(OBJ_DIR) $(WIN_OBJ_DIR) $(TEST_OBJ_DIR) $(WIN_TEST_OBJ_DIR)
	rm -rf $(TEST_BIN_DIR) $(WIN_TEST_BIN_DIR)
	rm -f $(LIB) $(WIN_LIB)

# --- Ayuda ------------------------------------------------------------------
help:
	@echo "ListaSimple - Makefile espejo de ListaSimple.cbp"
	@echo ""
	@echo "  make            compila la lib Linux   -> $(LIB)"
	@echo "  make test       compila la suite       -> $(TEST_BIN)"
	@echo "  make run        compila y corre la suite de tests"
	@echo "  make windows    compila la lib Windows -> $(WIN_LIB)   (necesita mingw-w64)"
	@echo "  make test-windows  compila la suite para Windows"
	@echo "  make wyl        targets Windows + Linux (virtual 'WyL')"
	@echo "  make clean      borra obj/, bin/Test/ y las libs generadas"
	@echo ""
	@echo "Se puede usar -j:  make -j4 test"

-include $(DEPS) $(TEST_DEPS) $(WIN_DEPS) $(WIN_TEST_DEPS)
