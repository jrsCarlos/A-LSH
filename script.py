import json

# Ejemplo de JSON con una lista en la clave "key"
with open('deps/stopwords.json', 'r', encoding='utf-8') as file:
    data = json.load(file)

# Contar los elementos en la lista asociada a la clave "key"
num_elementos = len(data["ca"])
print(num_elementos)  # Salida: 3
