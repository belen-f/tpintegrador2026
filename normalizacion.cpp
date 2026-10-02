#include <iostream>
#include <stdio.h> //fread, fwrite, fopen, fclose
#include <cstring> // strcmp y strcpy
#include <fstream> //para visualizar

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

// DECLARO FUNCIONES MOZOS
void crearMozos(const char *archivoExistente, Nodo *&pila);
void pushMozo(Mozo *&pila, Mozo *nuevoMozo);
Nodo *buscarMozoEnPila(Nodo *pila, const char *nombreMozo);
void crearArchivoMozos(const char *nombreNuevo, Nodo *pila);

// DECLARO FUNCIONES COMANDAS DIARIAS
long long transformarFechaAEntero(const char *fecha);
NodoComanda *crearNodo(ComandaHistorica registro);
NodoComanda *insertarOrdenado(NodoComanda *listaComanda, ComandaHistorica registro);
void crearListaOrdenada(const char *archivoExistente, NodoComanda *&listaComanda);
void crearArchivoComandaPorDia(NodoComanda *&listaComanda);

int main()
{
    // MOZOS
    Nodo *pila = NULL;
    crearMozos("data/comandas_historicas.dat", pila);
    crearArchivoMozos("mozos.dat", pila);

    // COMANDA DIARIA
    NodoComanda *listaComanda = NULL;
    crearListaOrdenada("data/comandas_historicas.dat", listaComanda);
    crearArchivoComandaPorDia(listaComanda);

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

void crearMozos(const char *archivoExistente, Nodo *&pila)
{
    cout << "----- RECORRIENDO COMANDA HISTORICA (MOZOS) -----" << endl;

    FILE *f = fopen(archivoExistente, "rb");
    if (f == NULL)
    {
        cout << "--> ERROR: no se pudo leer el archivo" << endl;
        return;
    }

    int idMaximo = 1; // para ir asignando ids únicos
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
            Mozo nuevoMozo;
            nuevoMozo.idMozo = idMaximo;

            strcpy(nuevoMozo.nombre, registro.nombreMozo);
            strcpy(nuevoMozo.password, "1234");
            nuevoMozo.totalComision = registro.comision;

            pushMozo(pila, nuevoMozo);
            idMaximo++; // se incrementa para el próximo mozo

            cout << "MOZO EXISTENTE: " << registro.nombreMozo << endl;
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
        cout << "--> ERROR: no se pudo crear el archivo";
        return;
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

    cout << "--> OK: Archivo mozos.dat creado y memoria RAM liberada" << endl;
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
        cout << "--> ERROR: no se pudo leer el archivo" << endl;
        return;
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

void crearArchivoComandaPorDia(NodoComanda *&listaComanda)
{
    cout << "----- CREANDO ARCHIVOS COMANDAS POR DIA -----" << endl;

    while (listaComanda != NULL)
    {
        const char *fechaActual = listaComanda->datos.fecha;

        string nombreArchivo = "comandas_" + string(fechaActual) + ".dat";

        FILE *f = fopen(nombreArchivo.c_str(), "wb");
        if (f == NULL)
        {
            cout << "--> ERROR: no se pudo crear el archivo" << nombreArchivo << endl;
            return;
        };

        while (listaComanda != NULL && transformarFechaAEntero(fechaActual) == transformarFechaAEntero(listaComanda->datos.fecha))
        {

            NodoComanda *aux = listaComanda;
            ComandaHistorica registroAGuardar = aux->datos;
            fwrite(&registroAGuardar, sizeof(ComandaHistorica), 1, f);
            cout << "Registro agregado a " << nombreArchivo << " exitosamente" << endl;

            listaComanda = listaComanda->siguiente;

            delete aux;
        };

        fclose(f);

        cout << "--> OK: Archivo " << nombreArchivo << " creado." << endl;
    };
};
