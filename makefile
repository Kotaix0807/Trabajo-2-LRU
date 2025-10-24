# Nombre del ejecutable
TARGET = lru

# Archivos fuente
SRC = src/main.c

# Flags de compilación
CFLAGS = -Wall -std=c11

# Regla principal
all: $(TARGET)

# Cómo compilar el ejecutable
$(TARGET): $(SRC)
	gcc $(CFLAGS) $(SRC) -o $(TARGET)

# Limpiar archivos compilados
clean:
	rm -f $(TARGET)
