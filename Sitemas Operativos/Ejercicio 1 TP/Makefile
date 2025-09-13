OBJDIR = obj
SRCDIR = src
INCDIR = include
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -I$(INCDIR)

TARGET = ./bin/DataGenerator
SOURCES = main.c coordinator.c generators.c 
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

REGISTERS = 321
CHILDS = 4
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Regla patrón: cómo compilar cada .c en obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Crear directorio si no existe
$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: clean

init: $(TARGET)
	$(TARGET) $(CHILDS) $(REGISTERS)
