# TP Integrador – El buffet de Alberto
**Algoritmos y Estructuras de Datos – UTN FRBA**

**Grupo:** []

## Integrantes

| Integrante | GitHub | Programa |
|---|---|---|
| Belén Fernández | [belen-f](https://github.com/belen-f) | `normalizacion.cpp` |
| Sabrina Rapa | [Sabri-Rapa](https://github.com/Sabri-Rapa) | `ventas.cpp` |
| Gabriela Melo Duran | [gmeloduran](https://github.com/gmeloduran) | `cierre.cpp` |
| Laura Camila Otero | [LauraOtero20](https://github.com/LauraOtero20) | `resumen.cpp` |

## Descripción del proyecto

El proyecto consiste en desarrollar un sistema de administración de ventas para el buffet de Alberto, utilizando archivos binarios en C++.

El sistema permite procesar ventas históricas, administrar mozos e inventario, registrar nuevas ventas, consolidar las comandas semanales y generar un resumen de productos vendidos y comisiones.

La solución está dividida en cuatro programas independientes, cada uno con su propio archivo `.cpp` y su propia función `main()`.

## Programas y orden de ejecución

Los programas deben ejecutarse en el siguiente orden:

| Orden | Programa | Función |
|---|---|---|
| 1 | `normalizacion.cpp` | Procesa las ventas históricas, genera los registros de mozos y las comandas diarias, y actualiza el inventario. |
| 2 | `ventas.cpp` | Registra nuevas ventas, valida mozos y productos, actualiza el stock y ordena las comandas del día. |
| 3 | `cierre.cpp` | Consolida las comandas diarias en un archivo semanal, ordenado por mozo. |
| 4 | `resumen.cpp` | Genera el resumen semanal de productos vendidos y comisiones por mozo. |
**Estado de integración:** `normalizacion.cpp` se encuentra desarrollado en la rama de normalización, pendiente de integrarse a la rama principal.

## Compilación y ejecución

Desde la carpeta raíz del proyecto, compilar cada programa por separado utilizando g++:
```bash
g++ normalizacion.cpp -o normalizacion.exe
g++ ventas.cpp -o ventas.exe
g++ cierre.cpp -o cierre.exe
g++ resumen.cpp -o resumen.exe
```

Ejecutar en PowerShell, respetando el orden indicado:
```powershell
.\normalizacion.exe
.\ventas.exe
.\cierre.exe
.\resumen.exe
```

Cada programa genera o modifica los archivos binarios necesarios para las siguientes etapas.

## 1. Normalización – `normalizacion.cpp`

**Responsable:** Belén Fernández

**Estado:** Código disponible en la rama de normalización; pendiente de integración.

Este programa procesa los registros de `data/comandas_historicas.dat`, identifica a los mozos, genera las comandas diarias y actualiza el stock de `data/inventario.dat` de acuerdo con las ventas históricas.

### Generación y registro de mozos

1. Abre el archivo de comandas históricas en modo de lectura binaria (`rb`).
2. Recorre sus registros mediante `fread` y busca el nombre de cada mozo en una lista enlazada.
3. Si el mozo ya existe, acumula el importe de la comisión en `totalComision`.
4. Si es un mozo nuevo, le asigna un `idMozo` correlativo desde 1 y lo incorpora a la lista. También guarda su nombre y su ID en una lista auxiliar para utilizarla al crear las comandas diarias.
5. Asigna la contraseña inicial `hola1234` y la transforma antes de almacenarla, aplicando un corrimiento de caracteres con `K = 5`.
6. Genera `mozos.dat` en modo escritura binaria (`wb`) con `fwrite` y libera los nodos de la lista utilizados en este procesamiento.

Cada registro de mozo utiliza la siguiente estructura:
```cpp
struct Mozo {
    int idMozo;
    char nombre[50];
    char password[20];
    float totalComision;
};
```

### Generación de comandas diarias

1. Vuelve a leer `data/comandas_historicas.dat`.
2. Inserta cada registro en una lista enlazada ordenada por fecha. Para comparar las fechas, convierte `DD-MM-AAAA` a un valor numérico `AAAAMMDD`.
3. Agrupa las comandas de una misma fecha y crea el archivo `comandas_DD-MM-AAAA.dat` correspondiente.
4. Busca en la lista auxiliar el `idMozo` asociado al nombre que figura en el registro histórico.
5. Guarda las comandas diarias con `fwrite`, utilizando el ID del mozo en lugar de su nombre, y libera los nodos procesados.

Cada comanda diaria se guarda con la siguiente estructura:
```cpp
struct ComandaDiaria {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};
```

### Actualización del inventario

1. Recorre nuevamente las comandas históricas y acumula las cantidades vendidas por `codigoProducto` en una lista enlazada.
2. Abre `data/inventario.dat` en modo lectura y escritura binaria (`rb+`).
3. Busca secuencialmente cada producto por código.
4. Descuenta la cantidad histórica vendida de `stockActual`.
5. Utiliza `fseek` para volver a la posición del registro encontrado y `fwrite` para actualizarlo directamente, sin reconstruir el archivo de inventario.
6. Informa una advertencia si un código de producto no se encuentra en el inventario.

### Técnicas utilizadas

**Listas enlazadas:** se utilizan para identificar mozos únicos, mantener la correspondencia entre nombres e IDs, ordenar los registros históricos por fecha y acumular cantidades vendidas por producto.

**Búsqueda secuencial:** se buscan los mozos por nombre en las listas y los productos por código en el archivo binario de inventario.

**Inserción ordenada:** las comandas históricas se insertan en una lista ordenada cronológicamente, para procesarlas por día.

**Actualización directa:** se modifica el stock de cada producto sobre el archivo binario mediante `fseek` y `fwrite`, conservando los demás registros.

**Liberación de memoria:** se eliminan los nodos de las listas al completar los procesos correspondientes.

### Archivos utilizados

**Lee:**
- `data/comandas_historicas.dat`
- `data/inventario.dat`

**Genera:**
- `mozos.dat`
- `comandas_DD-MM-AAAA.dat`

**Modifica:**
- `data/inventario.dat`

### Casos contemplados

- Imposibilidad de abrir el archivo de comandas históricas.
- Imposibilidad de crear los archivos de mozos o comandas diarias.
- Imposibilidad de abrir el inventario para actualizarlo.
- Código de producto histórico inexistente en el inventario: se informa una advertencia.

**Pendiente de adecuación a la consigna:** la implementación agrupa las comandas por fecha, pero actualmente no garantiza que los registros de cada archivo diario queden ordenados por `idMozo`. Es necesario incorporar ese ordenamiento.

**Precaución al ejecutar:** la normalización abre `mozos.dat` y los archivos diarios con `wb`, por lo que reemplaza su contenido si ya existen. Además, cada nueva ejecución vuelve a descontar las ventas históricas del inventario. Debe ejecutarse sobre el inventario original antes de cargar nuevas ventas.

---

## 2. Registro de ventas – `ventas.cpp`

**Responsable:** Sabrina Rapa

Este programa permite registrar nuevas ventas correspondientes a un día determinado, utilizando los archivos binarios de mozos e inventario.

Al iniciar, solicita una fecha en formato `DD-MM-AAAA`, que se utiliza para abrir o crear el archivo `comandas_DD-MM-AAAA.dat`.

Todas las ventas registradas durante esa ejecución se guardan en el mismo archivo diario.

### Registro y validación de ventas

Para cada venta, el programa:

1. Solicita el ID del mozo y lo busca secuencialmente en `mozos.dat`.
2. Solicita y valida la contraseña del mozo.
3. Solicita el código del producto y lo busca en `inventario.dat`.
4. Solicita la cantidad vendida y verifica que sea mayor a cero.
5. Comprueba que exista stock suficiente.
6. Calcula la comisión correspondiente al 10% del importe vendido.
7. Descuenta la cantidad vendida del stock del producto.
8. Actualiza el registro del producto en el archivo de inventario.
9. Agrega la comanda al archivo correspondiente al día.

### Cálculo de comisión

La comisión de cada venta se calcula mediante la fórmula:

`comision = precio * cantidad * 0.10`

### Estructura de las comandas

Cada venta se almacena como un registro de tamaño fijo:
```cpp
struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};
```

### Técnicas utilizadas

**Búsqueda secuencial de mozos:** se recorre el archivo binario hasta encontrar el registro cuyo `idMozo` coincida con el ingresado. Si no existe, no se permite registrar la venta.

**Validación de contraseña:** la clave ingresada se verifica contra la almacenada en `mozos.dat`, utilizando la transformación reversible acordada para el sistema.

**Actualización directa del inventario:** una vez validada la venta, se utiliza `fseek` para posicionarse sobre el registro correspondiente y `fwrite` para sobrescribirlo con el stock actualizado, sin reconstruir el archivo completo.

**Registro de comandas:** las nuevas ventas se agregan al archivo diario, conservando las comandas registradas anteriormente.

**Ordenamiento por selección:** al finalizar la carga, las comandas se ordenan por `idMozo` directamente sobre el archivo binario mediante `fseek`, `fread` y `fwrite`.

### Archivos utilizados

**Lee:**

- `mozos.dat`
- `inventario.dat`

**Crea o modifica:**

- `comandas_DD-MM-AAAA.dat`
- `inventario.dat`

### Casos contemplados

- ID de mozo inexistente.
- Contraseña incorrecta.
- Código de producto inexistente.
- Cantidad menor o igual a cero.
- Stock insuficiente.
- Errores de apertura o lectura de archivos binarios.
- Archivo diario inexistente, que se crea al comenzar la carga.

---

## 3. Cierre semanal – `cierre.cpp`

**Responsable:** Gabriela Melo Duran

Este programa consolida las comandas de distintos días en un único archivo semanal, ordenado de menor a mayor por `idMozo`.

Al iniciar, solicita el número de semana y el mes, con los que construye el nombre del archivo:

`comandas_semana_sX-mm.dat`

### Consolidación de comandas

1. Solicita el número de semana y el mes.
2. Crea el archivo semanal en modo lectura y escritura binaria (`wb+`).
3. Solicita las fechas de los archivos diarios que se desean procesar.
4. Construye el nombre de cada archivo diario y lo abre en modo lectura binaria (`rb`).
5. Si el archivo no existe, informa el error y permite continuar con otros días.
6. Lee las comandas mediante `fread` y las incorpora al archivo semanal utilizando `fwrite`.
7. Informa la cantidad de comandas procesadas por cada día.
8. Al finalizar, ordena el archivo semanal por `idMozo`.

### Técnicas utilizadas

**Lectura secuencial:** se recorren los archivos diarios para obtener las comandas registradas.

**Consolidación de archivos:** los registros de cada día se incorporan a un único archivo semanal.

**Ordenamiento:** las comandas consolidadas se ordenan por `idMozo` para facilitar su posterior procesamiento.

### Archivos utilizados

**Lee:**

- `comandas_DD-MM-AAAA.dat`

**Genera:**

- `comandas_semana_sX-mm.dat`

### Casos contemplados

- Archivo diario inexistente.
- Ningún día procesado correctamente.
- Archivo semanal con cero o una comanda.

---

## 4. Resumen semanal – `resumen.cpp`

**Responsable:** Laura Camila Otero

Este programa procesa el archivo semanal generado por `cierre.cpp` y presenta un resumen de las ventas (individuales y totales) y comisiones correspondientes a cada mozo.

### Procesamiento

1. Solicita el nombre del archivo semanal que se desea procesar.
2. Abre el archivo en modo lectura binaria (`rb`).
3. Realiza una lectura anticipada utilizando `fread`.
4. Recorre las comandas ordenadas por `idMozo`.
5. Detecta los cambios de mozo mediante un corte de control.
6. Acumula las cantidades vendidas y las comisiones correspondientes a cada mozo.
7. Muestra el ID del mozo, la cantidad total de productos vendidos y su comisión acumulada.
8. Calcula y muestra el total general de productos vendidos por el buffet durante la semana.

### Técnicas utilizadas

**Lectura anticipada:** permite controlar el avance por los registros del archivo binario.

**Corte de control:** agrupa las comandas correspondientes a un mismo mozo, aprovechando que el archivo semanal se encuentra ordenado por `idMozo`.

**Acumuladores:** se utilizan para calcular las cantidades vendidas y las comisiones individuales, además del total general del buffet.

### Archivos utilizados

**Lee:**

- `comandas_semana_sX-mm.dat`

**Salida:**

- Reporte por pantalla.

### Casos contemplados

- Archivo semanal inexistente o que no puede abrirse.
- Errores de lectura del archivo binario.


---

## Seguridad de contraseñas

La consigna requiere que las contraseñas de los mozos no se almacenen en texto plano.

Para cumplir este requisito se utiliza una transformación reversible por corrimiento de caracteres, con un valor fijo `K = 5`.

- **Normalización:** asigna a cada mozo la contraseña inicial `hola1234`, suma 5 al valor de cada carácter y almacena la clave transformada en `mozos.dat`.
- **Ventas:** debe aplicar el mismo corrimiento de `K = 5` a la contraseña ingresada antes de compararla con el valor guardado en el archivo.


## Archivos binarios

| Archivo | Función |
|---|---|
| `comandas_historicas.dat` | Archivo original de ventas, proporcionado por la cátedra. |
| `inventario.dat` | Registro de productos, precios y stock disponible. |
| `mozos.dat` | Datos de los mozos, contraseñas transformadas y comisiones acumuladas. |
| `comandas_DD-MM-AAAA.dat` | Registro de las ventas correspondientes a un día. |
| `comandas_semana_sX-mm.dat` | Consolidación de las comandas correspondientes a una semana. |
El procesamiento de los archivos binarios se realiza mediante las funciones estándar de C: `fopen`, `fread`, `fwrite`, `fseek` y `fclose`.

Las estructuras almacenadas utilizan registros de tamaño fijo.

## Control de versiones

El repositorio incluye un archivo `.gitignore` para excluir los archivos generados durante la compilación y ejecución.

Se excluyen:
```gitignore
*.exe
*.dat
```

Los archivos binarios de datos no se versionan, ya que la cátedra ejecutará los programas utilizando sus propios archivos de prueba.

Cada integrante realiza sus propios commits en GitHub, de acuerdo con la parte del proyecto que tiene asignada.
