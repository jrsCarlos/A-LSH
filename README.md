# A - Locatity-Sensitive-Hashing

## Introduccón
Este proyecto ...

## Estructura del Proyecto
```
├── exp-docs/                  # Documentos utilizados para los experimentos
│
├── deps/
│   ├── json.hpp               # Dependencia para manipular JSONs
│   │__ xxh64.hpp              # Familia de funciones de hash
│   │__ stopwords.json         # Lista de stopwords
│
├── docs/                      # Documentos originales
│
├── obj/                       # Archivos objetos
│
├── src/                       # Codigo fuente
│
├── .gitattributes             # Configuración de Git
│
├── docsCreator.exe            # Ejecutable que genera los documentos para los experimetos
│
├── main.exe                   # Ejecutable principal
│
│__ Makefile                   # Reglas de compilacion
```

## Instalación y Uso
### Requisitos
- Compilador de C++ (g++ recomendado)
- Make

### Compilación
Ejecutar el siguiente comando en la terminal:
```sh
make
```

### Ejecución
Generar los documentos para los experimentos:
```sh
./docsCreator.exe
```

Ejecutar el programa principal:
```sh
make run
```

Ejecutar para limpiar todos los archivos generados:
```sh
make clean
```

## Autores
- Adrián Salazar
- Montse Peñarrubia 
- Kristian Stoykov
- Carlos Rosales


