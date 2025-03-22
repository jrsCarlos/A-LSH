# Compilador y flags
CXX := g++
CXXFLAGS := -Wall -std=c++17 -Ideps

# Directorios
SRC_DIR := src
OBJ_DIR := obj
EXP_DIR := exp-docs

# Archivos fuente específicos
SRCS_MAIN := $(SRC_DIR)/main.cpp $(SRC_DIR)/hashing_tools.cpp
SRCS_DOCS := $(SRC_DIR)/document_creator.cpp $(SRC_DIR)/preprocessing.cpp

# Archivos objeto
OBJS_MAIN := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS_MAIN))
OBJS_DOCS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS_DOCS))

# Binaries
MAIN := main
DOCS := docsCreator

# Regla por defecto
all: $(MAIN) $(DOCS)

# Compilar main con optimización
$(MAIN): $(OBJS_MAIN) | $(OBJ_DIR)
	$(CXX) -o $@ $^ -O2

# Compilar docs con depuración
$(DOCS): $(OBJS_DOCS) | $(OBJ_DIR)
	$(CXX) -o $@ $^ -g

# Regla para compilar objetos de MAIN
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/hashing_tools.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para compilar objetos de DOCS
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/preprocessing.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear la carpeta obj si no existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpiar archivos generados
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(EXP_DIR)/*.txt $(MAIN) $(DOCS)
