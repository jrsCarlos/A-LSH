# Compilador y opciones
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Archivos
SRCS = main.cpp preprocessing.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = main

# Regla por defecto
all: $(EXEC)

# Cómo crear el ejecutable
$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC)

# Cómo compilar archivos .cpp a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJS) $(EXEC)
