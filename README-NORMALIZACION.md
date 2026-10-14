# Sistema de Normalización de Comandas e Inventario (normalizacion.cpp)

**Responsable: Belén Fernández**

Este programa procesa un archivo histórico de comandas para normalizar los datos, generar reportes ordenados y actualizar el stock del inventario. El sistema está desarrollado en C++ utilizando estructuras de datos dinámicas.

## 📋 Flujo Funcional (Arquitectura del Proceso)

El programa se ejecuta de forma secuencial en el `main()` dividiéndose en tres grandes etapas independientes:

1. **Gestión de Mozos:** Extrae el personal del historial, genera una estructura en memoria y exporta un archivo maestro limpio (`mozos.dat`).
2. **Procesamiento de Comandas:** Organiza cronológicamente las transacciones y genera un reporte diario cruzando información de los mozos.
3. **Actualización de Inventario:** Calcula el impacto de los consumos históricos y actualiza el stock real en el archivo de inventario (`inventario.dat`).

---

## 🛠️ Lógica de Implementación (Detalle Técnico)

El sistema basa su eficiencia en el uso de memoria dinámica (punteros) y el manejo de archivos binarios (`.dat`). A continuación se detalla la lógica interna de cada componente:

### 1. Estructuras de Datos Utilizadas

- `Nodo` y `NodoId`: Estructuras de tipo **Pila (LIFO)**. Se eligieron para apilar los mozos detectados y procesar sus IDs de forma inversa o rápida sin necesidad de recorrer accesos aleatorios.
- `NodoComanda`: **Lista Enlazada Simplemente Ordenada**. Permite insertar elementos manteniendo el orden cronológico en tiempo de ejecución.
- `NodoStockHistorico`: **Pila**. Acumula los movimientos de stock leídos del histórico para su posterior volcado.

### 2. Lógica de las Funciones Clave

#### Módulo de Mozos

- `crearMozos`: Abre `comandas_historicas.dat` en modo lectura binaria. Lee registro por registro mediante un bucle de control de fin de archivo. Aplica una lógica de **control de duplicados** antes de hacer el `push` en las pilas (`pila` y `pilaMozoId`) para asegurar que cada mozo se registre una sola vez.
- `crearArchivoMozos`: Recorre la `pila` desapilando o transitando los nodos, escribe los datos estructurados en `mozos.dat` y libera la memoria dinámica de cada nodo procesado.

#### Módulo de Comandas

- `crearListaOrdenada`: Al leer el histórico, por cada registro se busca su posición correcta en la lista `listaComanda`. La lógica de inserción compara el criterio de ordenamiento (Fecha) y reasigna los punteros (`anterior->sig = nuevo` y `nuevo->sig = actual`) para mantener la lista ordenada de forma nativa.
- `crearArchivoComandaPorDia`: Recorre la lista ordenada secuencialmente. Utiliza la `pilaMozoId` para realizar búsquedas/cruces de datos (Lookup) y escribe el resultado final en el archivo de reporte diario.

#### Módulo de Inventario

- `listarStock`: Agrupa los consumos del archivo histórico y genera los balances en la pila `pilaStockHistorico`.
- `actualizarStock`: Abre `inventario.dat` en modo lectura/escritura binaria (`rb+` o similar). Por cada nodo de la pila de stock, busca el registro correspondiente en el archivo y sobreescribe directamente la posición de memoria física utilizando `fseek` y `fwrite`, evitando tener que cargar todo el inventario en la memoria RAM.

---

## 🚀 Cómo Ejecutar el Programa

### Requisitos Previos

- Compilador de C++ (GCC/MinGW, Clang o MSVC).
- Contar con la carpeta `data/` y el archivo `comandas_historicas.dat` en la raíz del proyecto.

### Compilación y Ejecución (Consola)

```bash
# Compila el código fuente y genera el ejecutable "normalizacion"
g++ -o normalizacion normalizacion.cpp

# Ejecuta el programa para iniciar el proceso de normalización
./normalizacion
```
