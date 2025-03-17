# Compilador
CXX = g++

# Opciones del compilador
CXXFLAGS = -std=c++17 -Wall -Ideps

# Directorios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .



# Archivos objeto
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Nombre del ejecutable
TARGET = $(BIN_DIR)/main

# Regla para compilar el ejecutable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# Regla para compilar los archivos .cpp a .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para ejecutar el programa
run: $(TARGET)
	$(TARGET)

# Limpiar archivos generados
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: clean