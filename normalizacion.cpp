#include <iostream>
#include <stdio.h> //fread, fwrite, fopen, fclose
#include <cstring> // strcmp y strcpy
#include <cstdlib> //exit

using namespace std;

struct Mozo
{
    int idMozo;
    char nombre[50];
    char password[20];
    float totalComision;
};

struct Nodo
{
    Mozo datos;      // Datos del mozo
    Nodo *siguiente; // Puntero al próximo nodo
};

struct MozoId
{
    int idMozo;
    char nombre[50];
};
struct NodoId
{
    MozoId datos;
    NodoId *siguiente;
};

struct ComandaHistorica
{
    char fecha[11]; // "DD-MM-AAAA"
    char nombreMozo[50];
    int codigoProducto;
    int cantidad;
    float comision;
};

struct NodoComanda
{
    ComandaHistorica datos;
    NodoComanda *siguiente;
};

struct ComandaDiaria
{
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

struct Producto
{
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};

struct NodoProducto
{
    Producto datos;
    NodoProducto *siguiente;
};

struct StockHistorico
{
    int codigoProducto;
    int cantidad;
};

struct NodoStockHistorico
{
    StockHistorico datos;
    NodoStockHistorico *siguiente;
};

// DECLARO FUNCIONES MOZOS
void crearMozos(const char *archivoExistente, Nodo *&pila, NodoId *&pilaMozoId);
void pushMozo(Mozo *&pila, Mozo *nuevoMozo);
void pushMozoId(NodoId *&pilaMozoId, MozoId nuevoMozoId);
Nodo *buscarMozoEnPila(Nodo *pila, const char *nombreMozo);
void crearArchivoMozos(const char *nombreNuevo, Nodo *pila);

// DECLARO FUNCIONES COMANDAS DIARIAS
long long transformarFechaAEntero(const char *fecha);
int buscarMozoIdEnPila(NodoId *pila, const char *nombreMozo);
NodoComanda *crearNodo(ComandaHistorica registro);
NodoComanda *insertarOrdenado(NodoComanda *listaComanda, ComandaHistorica registro);
void crearListaOrdenada(const char *archivoExistente, NodoComanda *&listaComanda);
void crearArchivoComandaPorDia(NodoComanda *&listaComanda, NodoId *&pilaMozoId);
void liberarPilaMozoId(NodoId *&pila);

// DECLARO FUNCIONES DE INVENTARIO
NodoStockHistorico *buscarProducto(NodoStockHistorico *pilaStockHistorico, const char *descripcion, const char *codigo);
void pushStock(NodoStockHistorico *&pila, StockHistorico nuevoStock);
void listarStock(const char *archivoExistente, NodoStockHistorico *&pilaStockHistorico);
void actualizarStock(const char *archivoInventario, NodoStockHistorico *&pilaStockHistorico);

int main()
{
    // MOZOS
    Nodo *pila = NULL;
    NodoId *pilaMozoId = NULL;
    crearMozos("data/comandas_historicas.dat", pila, pilaMozoId);
    crearArchivoMozos("mozos.dat", pila);

    // COMANDA DIARIA
    NodoComanda *listaComanda = NULL;
    crearListaOrdenada("data/comandas_historicas.dat", listaComanda);
    crearArchivoComandaPorDia(listaComanda, pilaMozoId);
    liberarPilaMozoId(pilaMozoId);

    // INVENTARIO
    NodoStockHistorico *pilaStockHistorico = NULL;
    listarStock("data/comandas_historicas.dat", pilaStockHistorico);
    actualizarStock("data/inventario.dat", pilaStockHistorico);

    return 0;
};

// DEFINO FUNCIONES MOZOS

Nodo *buscarMozoEnPila(Nodo *pila, const char *nombreMozo)
{
    Nodo *actual = pila;

    while (actual != NULL)
    {

        if (strcmp(actual->datos.nombre, nombreMozo) == 0)
        {
            return actual; // si lo encuentra, devuelve el nodo
        }
        actual = actual->siguiente; // y avanza al siguiente nodo
    }
    return NULL; // si no lo encuentra, devuelve NULL
};

void pushMozo(Nodo *&pila, Mozo nuevoMozo)
{
    Nodo *nuevoNodo = new Nodo();
    nuevoNodo->datos = nuevoMozo;
    nuevoNodo->siguiente = pila;
    pila = nuevoNodo;
};
void pushMozoId(NodoId *&pilaMozoId, MozoId nuevoMozoId)
{
    NodoId *nuevoNodo = new NodoId();
    nuevoNodo->datos = nuevoMozoId;
    nuevoNodo->siguiente = pilaMozoId;
    pilaMozoId = nuevoNodo;
};

void crearMozos(const char *archivoExistente, Nodo *&pila, NodoId *&pilaMozoId)
{
    cout << "----- RECORRIENDO COMANDA HISTORICA (MOZOS) -----" << endl;

    FILE *f = fopen(archivoExistente, "rb");
    if (f == NULL)
    {
        cout << "-> ERROR: no se pudo leer el archivo. Saliendo del programa..." << endl;
        exit(EXIT_FAILURE);
    }

    int idMaximo = 1;
    ComandaHistorica registro;

    int leido = fread(&registro, sizeof(ComandaHistorica), 1, f);
    while (leido == 1)
    {
        Nodo *mozoEncontrado = buscarMozoEnPila(pila, registro.nombreMozo);

        if (mozoEncontrado != NULL)
        {
            mozoEncontrado->datos.totalComision += registro.comision;
            cout << "COMISION ACTUALIZADA: OK" << mozoEncontrado->datos.nombre << endl;
        }
        else
        {

            MozoId nuevoMozoId;
            nuevoMozoId.idMozo = idMaximo;
            strcpy(nuevoMozoId.nombre, registro.nombreMozo);

            Mozo nuevoMozo;
            nuevoMozo.idMozo = idMaximo;
            strcpy(nuevoMozo.nombre, registro.nombreMozo);
            strcpy(nuevoMozo.password, "1234");
            nuevoMozo.totalComision = registro.comision;

            pushMozo(pila, nuevoMozo);
            pushMozoId(pilaMozoId, nuevoMozoId);
            idMaximo++;

            cout << "MOZO NUEVO: " << registro.nombreMozo << endl;
        }

        leido = fread(&registro, sizeof(ComandaHistorica), 1, f);
    }

    fclose(f);
};

void crearArchivoMozos(const char *nombreNuevo, Nodo *pila)
{

    cout << "----- AGUARDE: CREANDO ARCHIVO MOZOS.DAT ... ----- " << endl;

    FILE *f = fopen(nombreNuevo, "wb");
    if (f == NULL)
    {
        cout << "--> ERROR: no se pudo crear el archivo. Saliendo del programa..." << endl;
        exit(EXIT_FAILURE);
    };

    while (pila != NULL)
    {

        Nodo *aux = pila;
        Mozo mozoAGuardar = aux->datos;

        fwrite(&mozoAGuardar, sizeof(Mozo), 1, f);
        cout << "MOZO " << aux->datos.nombre << ", guardado exitosamente" << endl;

        pila = pila->siguiente;

        delete aux;
    }

    fclose(f);

    cout << "-> OK: Archivo mozos.dat creado y memoria RAM liberada" << endl;
};

// DEFINO FUNCIONES COMANDAS DIARIAS
long long transformarFechaAEntero(const char *fecha)
{
    // fecha tiene el formato: D D - M M - A A A A
    // posiciones:             0 1 2 3 4 5 6 7 8 9
    long long dia = (fecha[0] - '0') * 10 + (fecha[1] - '0');
    long long mes = (fecha[3] - '0') * 10 + (fecha[4] - '0');
    long long anio = (fecha[6] - '0') * 1000 + (fecha[7] - '0') * 100 + (fecha[8] - '0') * 10 + (fecha[9] - '0');

    return (anio * 10000) + (mes * 100) + dia; // Devuelve un número como 20260201
};

int buscarMozoIdEnPila(NodoId *pila, const char *nombreMozo)
{
    NodoId *actual = pila;

    while (actual != NULL)
    {

        if (strcmp(actual->datos.nombre, nombreMozo) == 0)
        {
            return actual->datos.idMozo;
        }
        actual = actual->siguiente;
    }
    return -1;
};

NodoComanda *crearNodo(ComandaHistorica registro)
{
    NodoComanda *nuevoNodoComanda = new NodoComanda();
    nuevoNodoComanda->datos = registro;
    nuevoNodoComanda->siguiente = NULL;
    return nuevoNodoComanda;
};

NodoComanda *insertarOrdenado(NodoComanda *listaComanda, ComandaHistorica registro)
{
    NodoComanda *nuevoNodo = crearNodo(registro);

    if (listaComanda == NULL || transformarFechaAEntero(registro.fecha) < transformarFechaAEntero(listaComanda->datos.fecha))
    {
        nuevoNodo->siguiente = listaComanda;
        listaComanda = nuevoNodo;
        return nuevoNodo;
    };

    NodoComanda *actual = listaComanda;

    while (actual->siguiente != NULL &&
           transformarFechaAEntero(actual->siguiente->datos.fecha) < transformarFechaAEntero(registro.fecha))
    {
        actual = actual->siguiente;
    }

    nuevoNodo->siguiente = actual->siguiente;
    actual->siguiente = nuevoNodo;
    return listaComanda;
};

void crearListaOrdenada(const char *archivoExistente, NodoComanda *&listaComanda)
{
    cout << "----- RECORRIENDO COMANDA HISTORICA (FECHA) -----" << endl;

    FILE *f = fopen(archivoExistente, "rb");

    if (f == NULL)
    {
        cout << "-> ERROR: no se pudo leer el archivo. Saliendo del programa..." << endl;
        exit(EXIT_FAILURE);
    }

    ComandaHistorica registro;

    int leido = fread(&registro, sizeof(ComandaHistorica), 1, f);

    while (leido == 1)
    {
        listaComanda = insertarOrdenado(listaComanda, registro);

        leido = fread(&registro, sizeof(ComandaHistorica), 1, f);
    }
    fclose(f);
    cout << "--> OK: Lista Ordenada por Fecha" << endl;
};

void crearArchivoComandaPorDia(NodoComanda *&listaComanda, NodoId *&pilaMozoId)
{
    cout << "----- CREANDO ARCHIVOS COMANDAS POR DIA -----" << endl;

    while (listaComanda != NULL)
    {

        long long fechaIntActual = transformarFechaAEntero(listaComanda->datos.fecha);   // corrección
        string nombreArchivo = "comandas_" + string(listaComanda->datos.fecha) + ".dat"; // corrección

        FILE *f = fopen(nombreArchivo.c_str(), "wb");
        if (f == NULL)
        {
            cout << "--> ERROR: no se pudo crear el archivo" << nombreArchivo << endl;
            exit(EXIT_FAILURE);
        };

        while (listaComanda != NULL && fechaIntActual == transformarFechaAEntero(listaComanda->datos.fecha))
        {

            NodoComanda *aux = listaComanda;

            ComandaDiaria comandaDiaria;
            comandaDiaria.idMozo = buscarMozoIdEnPila(pilaMozoId, aux->datos.nombreMozo);
            comandaDiaria.codigoProducto = aux->datos.codigoProducto;
            comandaDiaria.cantidad = aux->datos.cantidad;
            comandaDiaria.comision = aux->datos.comision;

            fwrite(&comandaDiaria, sizeof(ComandaDiaria), 1, f);
            cout << "Registro agregado a " << nombreArchivo << " exitosamente." << endl;

            listaComanda = listaComanda->siguiente;

            delete aux;
        };

        fclose(f);

        cout << "-> OK: Archivo " << nombreArchivo << " creado." << endl;
    };
};

void liberarPilaMozoId(NodoId *&pila)
{
    while (pila != NULL)
    {
        NodoId *aux = pila;
        pila = pila->siguiente;
        delete aux;
    }
}

// DEFINO FUNCIONES DE INVENTARIO
NodoStockHistorico *buscarProducto(NodoStockHistorico *pilaStockHistorico, const int *codigoProducto)
{
    NodoStockHistorico *actual = pilaStockHistorico;

    while (actual != NULL)
    {

        if (actual->datos.codigoProducto == *codigoProducto)
        {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
};

void pushStock(NodoStockHistorico *&pila, StockHistorico nuevoStock)
{
    NodoStockHistorico *nuevoNodo = new NodoStockHistorico();
    nuevoNodo->datos = nuevoStock;
    nuevoNodo->siguiente = pila;
    pila = nuevoNodo;
};

void listarStock(const char *archivoExistente, NodoStockHistorico *&pilaStockHistorico)
{
    cout << "----- RECORRIENDO COMANDA HISTORICA (STOCK) -----" << endl;

    FILE *f = fopen(archivoExistente, "rb");
    if (f == NULL)
    {
        cout << "-> ERROR: no se pudo leer el archivo. Saliendo del programa..." << endl;
        exit(EXIT_FAILURE);
    }

    ComandaHistorica registro;

    int leido = fread(&registro, sizeof(ComandaHistorica), 1, f);
    while (leido == 1)
    {
        NodoStockHistorico *productoEncontrado = buscarProducto(pilaStockHistorico, &registro.codigoProducto);

        if (productoEncontrado != NULL)
        {
            productoEncontrado->datos.cantidad += registro.cantidad;
        }
        else
        {

            StockHistorico nuevoStock;
            nuevoStock.codigoProducto = registro.codigoProducto;
            nuevoStock.cantidad = registro.cantidad;

            pushStock(pilaStockHistorico, nuevoStock);
        }

        leido = fread(&registro, sizeof(ComandaHistorica), 1, f);
    }

    fclose(f);
    cout << "-> OK:  Lista de Stock creada" << endl;
};

void actualizarStock(const char *archivoInventario, NodoStockHistorico *&pilaStockHistorico)
{
    cout << "----- ACTUALIZANDO INVENTARIO (STOCK) -----" << endl;

    FILE *f = fopen(archivoInventario, "rb+");
    if (f == NULL)
    {
        cout << "-> ERROR: no se pudo leer el archivo " << archivoInventario << ". Saliendo del programa..." << endl;
        exit(EXIT_FAILURE);
    };

    while (pilaStockHistorico != NULL)
    {
        NodoStockHistorico *aux = pilaStockHistorico;

        int codigoBuscado = aux->datos.codigoProducto;
        int cantidadResta = aux->datos.cantidad;

        fseek(f, 0, SEEK_SET);

        Producto producto;
        bool encontrado = false;

        while (fread(&producto, sizeof(Producto), 1, f) == 1)
        {
            if (producto.codigo == codigoBuscado)
            {
                producto.stockActual -= cantidadResta;

                fseek(f, -(long)sizeof(Producto), SEEK_CUR);
                fwrite(&producto, sizeof(Producto), 1, f);

                cout << "STOCK ACTUALIZADO: " << producto.descripcion << " (Nuevo stock: " << producto.stockActual << ")" << endl;
                encontrado = true;
                break;
            }
        }

        if (!encontrado)
        {
            cout << "-> ADVERTENCIA: No se encontró el código " << codigoBuscado << " en el inventario." << endl;
        }

        pilaStockHistorico = pilaStockHistorico->siguiente;
        delete aux;
    }

    fclose(f);
};
