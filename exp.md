# Explicación de los parámetros b, r y t:
b: Número de bandas en las que se divide la firma MinHash.
r: Número de filas por banda. La longitud total de la firma (n) es n=b×r.
t: Umbral de similitud de Jaccard. Pares con similitud ≥t se consideran similares.
Según el análisis teórico, el umbral aproximado para LSH es (1/b)^1/r.

# Experimentos a realizar:

## Generación de datos controlados:
Crear un documento base (con al menos 50 palabras no stopwords).

## Generar documentos derivados:
Bloque 1: 20+ documentos mediante permutaciones aleatorias del documento base.
Bloque 2: 20+ "documentos virtuales" extrayendo k-shingles aleatorios del documento base (con similitud controlada según p_i).
Eliminar stopwords antes de generar shingles.

## Variación de parámetros:
k (tamaño de shingles): Probar valores como k=3,5,7.
t (longitud de la firma MinHash): Probar t=100,200,500.
b y r (bandas y filas): Para cada t, probar combinaciones de b×r=t (ej: t=100⇒b=20,r=5; b=10, r=10).

# Métricas a medir:

## Precisión:
Recall: Porcentaje de pares verdaderamente similares detectados.
Precisión: Porcentaje de pares candidatos que son realmente similares.
Falsos positivos/negativos.

## Rendimiento:
Tiempo de cómputo total (shingling, MinHash, LSH, verificación).
Número de pares candidatos generados.
Umbral teórico vs. real: Comparar (1/b)^1/r con el umbral empírico donde la probabilidad de detección es 50%.

# Análisis por experimento:

Experimento 1 (Impacto de k): Fijar t, b, r. Variar k.
Resultado esperado: k grande captura más contexto pero reduce similitud en textos con pequeñas variaciones.

Experimento 2 (Impacto de t): Fijar k. Variar t (longitud de firma).
Resultado esperado: Mayor t mejora la aproximación de Jaccard pero aumenta el tiempo.

Experimento 3 (Impacto de b y r): Fijar t. Variar b y r.
Resultado esperado: Más bandas (b grande) reducen el umbral, detectando pares menos similares pero aumentando falsos positivos.

# Recolección de datos:
Registrar para cada configuración: Valores de k,t,b,r.
Tiempos de cada etapa.
Número de pares verdaderos, candidatos, falsos positivos/negativos.
Recall y precisión.
Similitud promedio de pares detectados vs. no detectados.

# Explicación de resultados:

Gráficos: Curvas S (probabilidad de detección vs. similitud) para diferentes b y r.
Tiempo de cómputo vs. t o k.
Recall y precisión vs. umbral teórico.
Tablas comparativas:
- Efecto de b y r en falsos positivos/negativos.
- Impacto de k en la similitud real vs. estimada.

# Conclusiones:

Ej: "Con b=20,r=5, se logra un equilibrio entre recall (85%) y tiempo (2 segundos)".

Limitaciones: Elección de funciones de hash, tamaño de documentos, etc.

# Parámetros clave a seguir:
Sí, b, r, t (umbral), k, y el número de funciones de hash (t en MinHash) son parámetros críticos. Su interacción determina la efectividad y eficiencia de LSH.