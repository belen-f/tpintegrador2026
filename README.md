# TP Integrador - El buffet de Alberto

### Integrantes

- Belén Fernández - GitHub: belen-f
- Sabrina Rapa - GitHub: Sabri-Rapa
- Gabriela Melo Duran - GitHub: gmeloduran
- Laura Camila Otero - GitHub: LauraOtero20

---

## Descripción

El trabajo consiste en desarrollar un sistema para administrar las ventas del buffet a partir de archivos binarios.

La solución se divide en cuatro programas independientes:

- `normalizacion.cpp`
- `ventas.cpp`
- `cierre.cpp`
- `resumen.cpp`

Cada programa tiene su propio `main` y se ejecuta por separado.

---

## Orden de ejecución

Los programas deben ejecutarse en el siguiente orden:

1. `normalizacion.cpp`
2. `ventas.cpp`
3. `cierre.cpp`
4. `resumen.cpp`

`normalizacion.cpp` procesa los datos históricos y genera los archivos necesarios para que los programas siguientes puedan trabajar.

---

# normalizacion.cpp

Responsable: Belén Fernández

Este programa procesa la información inicial contenida en `comandas_historicas.dat` , para normalizar los datos, generar reportes ordenados y actualizar el stock del inventario.

Sus principales tareas son:

- Generar el archivo `mozos.dat`.
- Asignar un identificador a cada mozo.
- Calcular la comisión acumulada de cada mozo.
- Separar las ventas históricas por día.
- Generar los archivos `comandas_DD-MM-AAAA.dat`.
- Reemplazar el nombre del mozo por su identificador en las comandas.
- Ordenar las comandas por mozo.
- Actualizar el stock en `inventario.dat`.
- Guardar las contraseñas de los mozos de forma transformada, evitando almacenarlas en texto plano.

### Archivos utilizados

Lee:

- `comandas_historicas.dat`
- `inventario.dat`

Genera o modifica:

- `mozos.dat`
- `comandas_DD-MM-AAAA.dat`
- `inventario.dat`

## Flujo Funcional (Arquitectura del Proceso)

El programa se ejecuta de forma secuencial en el `main()` dividiéndose en tres grandes etapas independientes:

1. **Gestión de Mozos:** Extrae el personal del historial, genera una estructura en memoria y exporta un archivo maestro limpio (`mozos.dat`).
2. **Procesamiento de Comandas:** Organiza cronológicamente las transacciones y genera un reporte diario cruzando información de los mozos.
3. **Actualización de Inventario:** Calcula el impacto de los consumos históricos y actualiza el stock real en el archivo de inventario (`inventario.dat`).

---

## Lógica de Implementación (Detalle Técnico)

El sistema basa su eficiencia en el uso de memoria dinámica (punteros) y el manejo de archivos binarios (`.dat`). A continuación se detalla la lógica interna de cada componente:

### 1. Estructuras de Datos Utilizadas

- `Nodo` y `NodoId`: Estructuras de tipo **Pila (LIFO)**. Se eligieron para apilar los mozos detectados y procesar sus IDs de forma inversa.
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

## Compilación

Desde la carpeta del proyecto

```bash
g++ -o normalizacion.exe normalizacion.cpp
```

## Ejecución

En PowerShell:

```powershell
.\normalizacion.exe
```

## Casos contemplados

- Fallo en la apertura/lectura de archivo;
- Formato de fechas para comparativas;

---

# ventas.cpp

Responsable: Sabrina Rapa

Este programa permite registrar nuevas ventas correspondientes a un día determinado.

Al iniciar, solicita la fecha en formato:

`DD-MM-AAAA`

A partir de esa fecha abre o crea el archivo:

`comandas_DD-MM-AAAA.dat`

## Registro de una venta

Para cada venta el programa:

1. Solicita el ID del mozo.
2. Busca el mozo en `mozos.dat`.
3. Valida la contraseña ingresada.
4. Solicita el código del producto.
5. Busca el producto en `inventario.dat`.
6. Solicita la cantidad vendida.
7. Valida que la cantidad sea mayor a cero.
8. Verifica que haya stock suficiente.
9. Calcula la comisión correspondiente al 10% del importe vendido.
10. Actualiza el stock del producto en `inventario.dat`.
11. Guarda la venta en el archivo de comandas correspondiente al día.

Cada venta se almacena utilizando la siguiente estructura:

```cpp
struct Comanda
{
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};
```

## Búsqueda y validación del mozo

La búsqueda del mozo se realiza de forma secuencial sobre el archivo `mozos.dat`.
Se utiliza el `idMozo` ingresado para localizar el registro correspondiente. Si el mozo no existe, la venta no se registra.
Una vez encontrado el mozo, se solicita su contraseña y se valida contra la almacenada en el archivo.

## Búsqueda del producto

El producto se busca en `inventario.dat` a partir del código ingresado.
Si el producto no existe, la venta no se registra.
Una vez encontrado, se utilizan su precio y stock actual para continuar con la operación.

## Validación de stock

La cantidad ingresada debe ser mayor a cero y no puede superar el stock disponible del producto.
Si la cantidad no es válida o no hay stock suficiente, la venta se cancela.

## Cálculo de comisión

La comisión correspondiente a cada venta se calcula como el 10% del importe total vendido.
La fórmula utilizada es: `precio × cantidad × 0.10`

## Actualización del inventario

Luego de validar la venta, se actualiza el registro correspondiente al producto en `inventario.dat`.
Se descuenta la cantidad vendida del stock actual y se sobrescribe el registro directamente en el archivo binario.

## Registro de la comanda

La venta se agrega al archivo correspondiente al día: `comandas_DD-MM-AAAA.dat`
El archivo se abre en modo de agregado para conservar las ventas previamente registradas.

## Ordenamiento de las comandas

Una vez finalizada la carga de ventas del día, el archivo se vuelve a abrir en modo lectura y escritura.
Las comandas se ordenan por `idMozo` utilizando el método de ordenamiento por selección.
El ordenamiento se realiza directamente sobre el archivo binario utilizando `fseek`, `fread` y `fwrite`.

## Archivos utilizados

El programa lee:

- `mozos.dat`
- `data/inventario.dat`

El programa crea o modifica:

- `comandas_DD-MM-AAAA.dat`
- `data/inventario.dat`

## Compilación

Desde la carpeta del proyecto:

```bash
g++ ventas.cpp -o ventas.exe
```

## Ejecución

En PowerShell:

```powershell
.\ventas.exe
```

## Casos contemplados

El programa contempla los siguientes casos:

- mozo inexistente;
- contraseña incorrecta;
- producto inexistente;
- cantidad menor o igual a cero;
- stock insuficiente;
- archivo de mozos inexistente o dañado;
- archivo de inventario inexistente o dañado;
- archivo diario inexistente, que se crea al comenzar la carga.

---

# Cierre.cpp

Responsable: Gabriela Melo Duran

Este programa permite juntar todas las planillas de ventas diarias de una semana en un unico archivo semanal binario, ordenando automaticamente las comandas de menor a mayor segun el codigo de mozo (`idMozo`).

Al iniciar, solicita el número de semana y el mes:

`numeroSemana` y `mes`

A partir de estos datos, abre o crea el archivo:

`comandas_semana_sX-mm.dat`

## Consolidación y cierre de semana

Para realizar el cierre de la semana el programa:

1. Solicita el número de semana.
2. Solicita el mes.
3. Crea y abre el archivo semanal en modo de escritura y lectura binaria (`wb+`).
4. Mediante el bucle interativo (`while`), solicita la fecha de cada día a procesar en formato `DD-MM-AAAA`.
5. Construye el nombre del archivo diario correspondiente (`comandas_DD-MM-AAAA.dat`) y lo abre en modo lectura binaria (`rb`).
6. Valida la existencia del archivo diario (`fdiario == NULL`); si no existe, avisa por consola pero permite continuar con otros días.
7. Lee secuencialmente todas las comandas almacenadas en el archivo diario mediante `fread`.
8. Escribe cada comanda de manera acumulativa en el archivo semanal mediante `fwrite`.
9. Controla la cantidad de días procesados (`diasProcesados`) e informa la cantidad de comandas leídas por día.
10. Finalizada la carga de datos, si se procesaron días válidos, invoca la función encargada de ordenar el archivo.

Cada comanda se almacena utilizando la siguiente estructura:

```cpp
struct comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};
```

## Archivos utilizados

El programa lee:
comandas_DD-MM-AAAA.dat

El programa crea o modifica:
comandas_semana_sX-mm.dat

## Casos contemplados

- Archivo diario inexistente;
- Cero días procesados;
- Semana con cero o una comanda.



---

# resumen.cpp

Responsable: Laura Camila Otero

Este programa permite generar un listado a partir de un archivo binario con datos semanales. Se busca procesar la información unificada para sí obtener un detalle sobre las ventas y comisiones de cada mozo y de las ventas totales del buffet.

# Procesamiento
1. Se le solicita al usuario el nombre del archivo semanal a procesar, "Ingresar nombre de archivo semanal: ".

2. Si el archivo es correcto, este se abre como lectuta binaria (rb), en cambio, si el no pudo abrirse se muestra el mensaje "No se pudo abrir el archivo." y finaliza.

3. A partir de este archivo se utiliza fread para hacer una lectura anticipada.

4. Se realiza un primer bucle principal para detectar cada cambio de mozo, se recorre el archivo mientra haya registros válidos.

5. Se crea un bucle secundario donde se recorren las comandas de un mismo mozo, acumulandose los productos vendidos y su comisión.

6. Se muestra en pantalla, por cada mozo: su ID, la cantidad de productos vendidos y comisión a pagar.

7. Se suman los productos vendidos por cada mozo, calculando así el total de productos vendidos en el buffet.

8. Al finalizar, se imprime en pantalla el total general de productos vendidos en el buffet.

(Cada comanda se procesa siguiendo el formato del archivo generado en el cierre)=
struct comandaSemanal {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

## Archivos utilizados

El programa lee:
(comandas_semana_sX-mm.dat) archivo semanal de comandas, nombre ingresado por el usuario.

## Casos contemplados

- Archivo semanal inexistente o no pudo abrirse.
- Archivo semanal no pudo leerse.

