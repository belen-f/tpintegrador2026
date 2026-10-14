# TP Integrador - El buffet de Alberto


### Integrantes

- [Nombre integrante 1] - GitHub: Sabri-Rapa
- Sabrina Rapa - GitHub: [COMPLETAR]
- [Nombre integrante 3] - GitHub: [COMPLETAR]
- [Nombre integrante 4] - GitHub: [COMPLETAR]

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

Responsable: [COMPLETAR]

Este programa procesa la información inicial contenida en `comandas_historicas.dat`.

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