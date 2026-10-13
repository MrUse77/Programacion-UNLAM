# ============================================================================
#  Unidad 4 - Makefile unico de la unidad
#
#  Compila y corre TODOS los ejercicios de la unidad sin tener que nombrarlos:
#  cada subcarpeta que tenga al menos un .c se detecta automaticamente, asi
#  que agregar "Ejercicio 10", "Ejercicio 11", ... no requiere tocar nada.
#
#  Uso rapido (parado en esta carpeta):
#
#    make                    compila todos los ejercicios
#    make run                compila y corre todos, en orden
#    make EJ=2               compila solo el ejercicio 2  (tambien EJ=ej2)
#    make EJ=2 run           compila y corre solo ese
#    make run-ej2            compila y corre el ejercicio 2
#    make list               lista los ejercicios detectados
#    make clean              borra los artefactos de este Makefile
#    make -k                 sigue con el resto aunque uno falle
#    make help               ayuda completa
#
#  Como funciona (dos modos en un solo archivo):
#
#    - MODO RAIZ (sin MODE=inner): descubre las carpetas de ejercicios, les
#      asigna un nombre corto sin espacios ("Ejercicio 2" -> ej2) y por cada
#      una invoca a este mismo Makefile en modo interno.
#    - MODO INTERNO (MODE=inner): se ejecuta con `make -C <carpeta>` y compila
#      UN ejercicio. Todas sus rutas son relativas a la carpeta (sin espacios),
#      que es justamente el motivo de la recursion: make y los nombres de
#      archivo con espacios se llevan mal.
#
#  Las librerias del repo (libs/Linux/lib*.a) se eligen solas a partir de los
#  #include <...> de cada ejercicio. Ej: `#include <pila_estatica.h>` enlaza
#  -lPilaEstatica. Si queres forzar algo extra: make LIBS_EXTRA=-lLoQueSea
#
#  Los binarios van a <ejercicio>/bin/make/ y los objetos a
#  <ejercicio>/obj/make/, para no pisar la salida de Code::Blocks, que usa
#  bin/Debug/ y obj/Debug/.
# ============================================================================

# ---------------------------------------------------------------------------
#  Datos compartidos por los dos modos
# ---------------------------------------------------------------------------
# Rutas del repo, relativas a CADA carpeta de ejercicio (sube 3 niveles)
INCLUDES_REL := ../../../includes
LIBS_REL     := ../../../libs/Linux

# Subcarpetas de artefactos dentro de cada ejercicio
OBJ_SUB := obj/make
BIN_SUB := bin/make

# Este mismo archivo, en absoluto: se reusa para compilar cada ejercicio
MK := $(abspath $(lastword $(MAKEFILE_LIST)))

# Un salto de linea real. $(shell) devuelve todo en una sola linea, asi que el
# descubrimiento de ejercicios separa con '|' y esto lo vuelve a partir.
define NL


endef

# Convencion de nombres del repo: lista_simple -> ListaSimple (libListaSimple.a)
CAMEL := awk -F_ '{ s=""; for (i=1; i<=NF; i++) s = s toupper(substr($$i,1,1)) substr($$i,2); printf "%s", s }'

ifeq ($(MODE),inner)

# ===========================================================================
#  MODO INTERNO - compila y corre UN ejercicio
#  Invocado siempre como:  make -C "<carpeta>" -f <este archivo> MODE=inner
# ===========================================================================

CC         ?= gcc
CFLAGS     ?= -Wall -g
LDFLAGS    ?=
LDLIBS     ?= -lm
ALL_CPPFLAGS := -I$(INCLUDES_REL) $(CPPFLAGS)

# -MMD -MP deja un .d por objeto: si toco un header, recompila lo que depende.
DEPFLAGS := -MMD -MP

SRCS := $(sort $(wildcard *.c))
OBJS := $(addprefix $(OBJ_SUB)/,$(SRCS:.c=.o))
DEPS := $(OBJS:.o=.d)
BIN  := $(BIN_SUB)/$(NAME)

# --- Librerias del repo que este ejercicio necesita -------------------------
# Se deducen de los headers propios que incluye. Solo cuentan los headers que
# existen en $(INCLUDES_REL): los de la biblioteca estandar (stdio.h, ...) no
# estan ahi, asi que quedan afuera solos. types.h, utils.h y test_framework.h
# son compartidos (no tienen libreria), por eso se excluyen a mano.
NO_LIB_HEADERS := types.h utils.h test_framework.h

HEADERS_USED := $(shell grep -ho '#include *<[a-z0-9_]*\.h>' $(SRCS) 2>/dev/null | sed -e 's/.*<//' -e 's/>//' | sort -u)

WANTED_LIBS := $(sort $(shell for h in $(filter-out $(NO_LIB_HEADERS),$(HEADERS_USED)); do \
	[ -f "$(INCLUDES_REL)/$$h" ] || continue; \
	echo "$$h" | sed -e 's/\.h$$//' | $(CAMEL); printf ' '; done))

FOUND_LIBS   := $(foreach l,$(WANTED_LIBS),$(if $(wildcard $(LIBS_REL)/lib$(l).a),$(l)))
MISSING_LIBS := $(filter-out $(FOUND_LIBS),$(WANTED_LIBS))

AUTO_LIBS := $(addprefix -l,$(FOUND_LIBS))
ALL_LIBS  := $(AUTO_LIBS) $(LIBS_EXTRA)

.PHONY: build run clean help

build: $(BIN)

$(BIN): $(OBJS) | $(BIN_SUB)
	@[ -z "$(strip $(MISSING_LIBS))" ] || \
	  echo "  aviso: no existe $(LIBS_REL)/lib<NOMBRE>.a para: $(MISSING_LIBS)"
	@echo "  enlazando $(BIN)  (libs: $(if $(strip $(ALL_LIBS)),$(strip $(ALL_LIBS)),ninguna))"
	$(CC) $(LDFLAGS) -o "$@" $(OBJS) -L$(LIBS_REL) $(ALL_LIBS) $(LDLIBS)

$(OBJ_SUB)/%.o: %.c | $(OBJ_SUB)
	$(CC) $(ALL_CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c "$<" -o "$@"

$(OBJ_SUB) $(BIN_SUB):
	@mkdir -p "$@"

# Corre el binario DESDE la carpeta del ejercicio: asi los fopen("datos1.csv")
# relativos siguen funcionando.
run: $(BIN)
	@echo ""
	@echo "== $(NAME) ===================================================="
	@echo "   carpeta : $(CURDIR)"
	@echo "   binario : $(BIN)"
	@echo "---------------------------------------------------------------"
	@"./$(BIN)"

clean:
	rm -rf $(OBJ_SUB) $(BIN_SUB)

help:
	@echo "Modo interno del Makefile de Unidad 4 (normalmente no se llama a mano)."
	@echo "Se usa desde la carpeta de la unidad:  make EJ=<n> run"

-include $(DEPS)

else

# ===========================================================================
#  MODO RAIZ - descubre los ejercicios y delega en el modo interno
# ===========================================================================

# --- Descubrimiento ---------------------------------------------------------
# Script de UNA linea: $(shell) no conserva los saltos de linea del argumento.
# Imprime las asignaciones que necesita el modo raiz separadas por '|'.
define FIND_EXERCISES
o=''; s=''; for d in */ ; do d=$${d%/}; case "$$d" in .*|bin|obj) continue ;; esac; ls "$$d"/*.c >/dev/null 2>&1 || continue; g=$$(echo "$$d" | sed -e 's/[^0-9]*\([0-9][0-9]*\).*/ej\1/' -e 's/[^A-Za-z0-9_]/_/g' | tr 'A-Z' 'a-z'); o="$$o|EX_DIR_$$g := $$d"; s="$$s $$g"; done; printf '%s|EX_ALL :=%s' "$$o" "$$(printf '%s\n' $$s | sort -V | tr '\n' ' ')"
endef

$(eval $(subst |,$(NL),$(shell $(FIND_EXERCISES))))

# --- Seleccion opcional de un ejercicio (EJ=<n> o EJ=<nombre de carpeta>) ---
define FIND_BY_NAME
for d in */ ; do d=$${d%/}; if [ "$$d" = '$(strip $(EJ))' ]; then echo "$$d" | sed -e 's/[^0-9]*\([0-9][0-9]*\).*/ej\1/' -e 's/[^A-Za-z0-9_]/_/g' | tr 'A-Z' 'a-z'; fi; done
endef

EX_ALL := $(strip $(EX_ALL))

ifneq ($(strip $(EJ)),)
  EX_SEL := $(filter ej$(strip $(EJ)) $(strip $(EJ)),$(EX_ALL))
  ifeq ($(strip $(EX_SEL)),)
    EX_SEL := $(strip $(shell $(FIND_BY_NAME)))
  endif
  ifeq ($(strip $(EX_SEL)),)
    $(error No encontre ningun ejercicio que coincida con EJ='$(EJ)'. Mira `make list`.)
  endif
else
  EX_SEL := $(EX_ALL)
endif

RUN_TARGETS := $(addprefix run-,$(EX_SEL))

.PHONY: all build run list clean help $(EX_SEL) $(RUN_TARGETS)
.DEFAULT_GOAL := all

# --- Meta targets -----------------------------------------------------------
all: $(EX_SEL)
build: all

run: $(RUN_TARGETS)
.NOTPARALLEL: run

# --- Un ejercicio: compilar ------------------------------------------------
$(EX_SEL):
	@echo ""
	@echo "== $@ -> $(EX_DIR_$@)"
	@$(MAKE) --no-print-directory -C "$(EX_DIR_$@)" -f "$(MK)" MODE=inner NAME=$@ build

# --- Un ejercicio: compilar y correr ---------------------------------------
$(RUN_TARGETS):
	@$(MAKE) --no-print-directory -C "$(EX_DIR_$(patsubst run-%,%,$@))" -f "$(MK)" MODE=inner NAME=$(patsubst run-%,%,$@) run

# --- Utilidades -------------------------------------------------------------
list:
	@echo "Ejercicios detectados en la unidad:"
	@printf '  %-8s %s\n' $(foreach s,$(EX_SEL),'$(s)' "'$(EX_DIR_$(s))'")
	@echo ""
	@echo "$(words $(EX_SEL)) ejercicio(s)."

clean:
	@$(foreach s,$(EX_SEL),rm -rf '$(EX_DIR_$(s))/$(OBJ_SUB)' '$(EX_DIR_$(s))/$(BIN_SUB)';) \
	 echo "Listo: borre $(OBJ_SUB)/ y $(BIN_SUB)/ en $(words $(EX_SEL)) ejercicio(s)."

help:
	@echo "Unidad 4 - Makefile unico (un comando para todos los ejercicios)"
	@echo ""
	@echo "  make                  compila todos los ejercicios"
	@echo "  make build            igual que make"
	@echo "  make run              compila y corre todos, en orden"
	@echo ""
	@echo "  make EJ=2             compila solo el ejercicio 2  (tambien EJ=ej2)"
	@echo "  make EJ=2 run         compila y corre solo ese"
	@echo "  make 'EJ=Ejercicio 2' run   lo mismo, por nombre de carpeta"
	@echo "  make run-ej2          compila y corre el ejercicio 2"
	@echo ""
	@echo "  make list             lista los ejercicios detectados"
	@echo "  make clean            borra $(OBJ_SUB)/ y $(BIN_SUB)/ de cada ejercicio"
	@echo "  make -k               no se detiene en el primer error"
	@echo ""
	@echo "Variables utiles:"
	@echo "  CC=clang              compilador (por defecto gcc)"
	@echo "  CFLAGS='-Wall -g -O2' opciones de compilacion"
	@echo "  LIBS_EXTRA=-lFoo      agrega librerias al enlazar"
	@echo "  INCLUDES_REL / LIBS_REL   rutas compartidas ($(INCLUDES_REL), $(LIBS_REL))"
	@echo ""
	@echo "Las librerias del repo se detectan solas segun los #include de cada"
	@echo "ejercicio (<pila_estatica.h> -> -lPilaEstatica). Agregar un ejercicio"
	@echo "nuevo es solo crear la carpeta con su .c: no hay que tocar el Makefile."

endif
