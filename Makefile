# Compilador
CXX = g++

# Opciones del compilador
CXXFLAGS = -std=c++17 -Wall -Ideps

# Directorios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

# Archivos objeto
OBJ = $(OBJ_DIR)/main.o

# Nombre del ejecutable
TARGET = $(BIN_DIR)/main

# Regla para compilar el ejecutable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# Regla para compilar los archivos .cpp a .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: clean