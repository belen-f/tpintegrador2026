#include <iostream>
#include <cstdio>  //FILE, fopen, fread, fwrite, fseek, ftell, fclose
#include <cctype>  //isdgit
#include <cstring> // srtlen, strcpy
using namespace std;

struct Mozo
{
    int idMozo;
    char nombre[50];
    char password[20];
    float totalComision;
};

struct Producto
{
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};

struct Comanda
{
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

bool nombreDelArchivoDelDia(char archivoDelDia[])
{
    char fecha[11];
    do
    {
        cout << "Ingrese la fecha del día en formato DD-MM-AAAA" << endl;

        if (!(cin >> fecha))
        {
            cout << "\nEntrada cancelada." << endl;
            return 0;
        }

    } while (strlen(fecha) != 10 ||
             fecha[2] != '-' ||
             fecha[5] != '-' ||
             !isdigit(fecha[0]) ||
             !isdigit(fecha[1]) ||
             !isdigit(fecha[3]) ||
             !isdigit(fecha[4]) ||
             !isdigit(fecha[6]) ||
             !isdigit(fecha[7]) ||
             !isdigit(fecha[8]) ||
             !isdigit(fecha[9]) ||
             fecha[0] > '3' || (fecha[0] == '3' && fecha[1] > '1') ||
             fecha[3] > '1' || (fecha[3] == '1' && fecha[4] > '2') ||
             fecha[6] != '2' || fecha[7] != '0' || fecha[8] < '2' ||
             (fecha[0] == '0' && fecha[1] == '0') || (fecha[3] == '0' && fecha[4] == '0'));

    strcpy(archivoDelDia, "comandas_"); // copia una cadena dentro de otra
    strcat(archivoDelDia, fecha);
    strcat(archivoDelDia, ".dat");
    return true;
};

void encriptarPassword(char password[])
{
    const int K = 5;

    for (int i = 0; password[i] != '\0'; i++)
    {
        password[i] = password[i] + K;
    }
}

Mozo buscarMozo(int idMozo)
{

    FILE *fMozos = fopen("mozos.dat", "rb");

    Mozo mozoInvalido;

    mozoInvalido.idMozo = -1;
    strcpy(mozoInvalido.nombre, "-1");
    strcpy(mozoInvalido.password, "-1");
    mozoInvalido.totalComision = -1;

    if (fMozos == NULL)
    {
        cout << "Archivo dañado o inexistente" << endl;
        return mozoInvalido;
    }

    Mozo m = mozoInvalido; // para que no quede sin inicializar si no encuentra mozo y no quede con valores "basura" no controlados para cuando entra al condicional del while

    while (fread(&m, sizeof(Mozo), 1, fMozos) == 1 && m.idMozo != idMozo)
    {
        // No hace falta completar este while, porque sólo necesito iterar en el archivo hasta que quede en la variable m el mozo buscado
    }

    if (m.idMozo != idMozo)
    {
        cout << "Mozo inválido" << endl;
        fclose(fMozos);
        return mozoInvalido;
    }

    char password[20];
    cout << "Ingrese la contraseña" << endl;
    cin >> password;

    encriptarPassword(password);

    if (strcmp(m.password, password) != 0)
    { // strcmp para comparar arrays
        cout << "Contraseña incorrecta" << endl;
        fclose(fMozos);
        return mozoInvalido;
    }

    fclose(fMozos);
    return m;
};

Producto buscarProducto(int codigo)
{
    FILE *fInventario = fopen("data/inventario.dat", "rb");

    Producto productoInvalido;

    productoInvalido.codigo = -1;
    strcpy(productoInvalido.descripcion, "-1");
    productoInvalido.precio = -1;
    productoInvalido.stockActual = -1;

    if (fInventario == NULL)
    {
        cout << "Archivo dañado o inexistente" << endl;
        return productoInvalido;
    }

    Producto p = productoInvalido;

    while (fread(&p, sizeof(Producto), 1, fInventario) == 1 && p.codigo != codigo)
    {
        // No hace falta completar este while, porque sólo necesito iterar en el archivo hasta que quede en la variable p el producto buscado
    }

    if (p.codigo != codigo)
    {
        cout << "Producto inválido" << endl;
        fclose(fInventario);
        return productoInvalido;
    }

    fclose(fInventario);
    return p;
}

bool validarCantidad(Producto p, int cantidadVendida)
{
    if (cantidadVendida < 1)
    {
        cout << "Cantidad inválida" << endl;
        return 0;
    }
    else if (p.stockActual < cantidadVendida)
    {
        cout << "El stock disponible es menor a la cantidad ingresada" << endl;
        return 0;
    }
    else
        return 1;
}

float calcularComision(Producto p, int cantidad)
{
    const float TASA_COMISION = 0.10f;

    return p.precio * cantidad * TASA_COMISION;
}

void modificarArchivoDeComandasDelDia(FILE *archivoDelDiaAbierto, int idMozo, int codigoProducto, int cantidadVendida, float comision)
{

    Comanda c;
    c.idMozo = idMozo;
    c.codigoProducto = codigoProducto;
    c.cantidad = cantidadVendida;
    c.comision = comision;

    fwrite(&c, sizeof(Comanda), 1, archivoDelDiaAbierto);
}

bool modificarInventario(int codigoProducto, int cantidadVendida)
{
    FILE *fInventario = fopen("data/inventario.dat", "rb+");
    if (fInventario == NULL)
    {
        cout << "Archivo inexistente o dañado" << endl;
        return false;
    };

    Producto p;
    bool encontrado = false;
    while (!encontrado && fread(&p, sizeof(Producto), 1, fInventario) == 1)
    {
        if (p.codigo == codigoProducto)
        {
            encontrado = true;
            p.stockActual = p.stockActual - cantidadVendida;

            // el puntero quedó en el SIGUIENTE registro -> retrocede uno
            fseek(fInventario, -(long)sizeof(Producto), SEEK_CUR); // SEEK_CUR significa “tomá como referencia la posición actual del cursor del archivo”
            fwrite(&p, sizeof(Producto), 1, fInventario);
        }
    }

    fclose(fInventario);
    if (!encontrado)
    {
        cout << "No se encontró el producto." << endl;
        return false;
    }

    return true;
}

void registrarVenta(FILE *archivoDelDiaAbierto)
{
    int idMozo;
    int codigoProducto;
    int cantidadVendida;
    float comision;

    cout << "Ingrese el número identificador del mozo" << endl;
    cin >> idMozo;

    if (cin.fail())
    {
        cin.clear();            // quita el estado de error de cin
        cin.ignore(1000, '\n'); // cin.ignore(1000, '\n');

        cout << "Debe ingresar un número" << endl;
        return;
    }

    Mozo m = buscarMozo(idMozo);

    if (m.idMozo == -1)
    {
        return;
    }

    // cout << "ID: " << m.idMozo << '\n'
    //      << "Nombre: " << m.nombre << '\n'
    //      << "Password: " << m.password << '\n'
    //      << "Total comisión: " << m.totalComision << "\n\n";

    cout << "Ingrese el código del producto" << endl;
    cin >> codigoProducto;

    if (cin.fail())
    {
        cin.clear();            // quita el estado de error de cin
        cin.ignore(1000, '\n'); // descarta lo que quedó mal ingresado en el buffer.

        cout << "Debe ingresar el código numérico" << endl;
        return;
    }

    Producto p = buscarProducto(codigoProducto);

    if (p.codigo == -1)
    {
        return;
    }

    // cout << "Codigo: " << p.codigo << '\n'
    //      << "Descripción: " << p.descripcion << '\n'
    //      << "Precio: " << p.precio << '\n'
    //      << "Stock: " << p.stockActual << "\n\n";

    cout << "Ingrese cantidad vendida" << endl;
    cin >> cantidadVendida;

    if (cin.fail())
    {
        cin.clear();            // quita el estado de error de cin
        cin.ignore(1000, '\n'); // descarta lo que quedó mal ingresado en el buffer.

        cout << "Debe ingresar un número" << endl;
        return;
    }

    bool cantidadValida = validarCantidad(p, cantidadVendida);

    if (!cantidadValida)
        return;

    comision = calcularComision(p, cantidadVendida);

    if (!modificarInventario(p.codigo, cantidadVendida))
    {
        return;
    }

    modificarArchivoDeComandasDelDia(archivoDelDiaAbierto, idMozo, p.codigo, cantidadVendida, comision);
};

void ordenarArchivoDelDiaPorSeleccion(FILE *archivoDelDiaAbierto)
{
    // Calculo cuántas comandas hay
    fseek(archivoDelDiaAbierto, 0, SEEK_END);
    long bytes = ftell(archivoDelDiaAbierto);
    int cantidadComandas = bytes / sizeof(Comanda);

    for (int i = 0; i < cantidadComandas - 1; i++)
    {
        int posicionMenor = i;

        Comanda menor;

        // Leo la comanda que está en la posición i
        fseek(archivoDelDiaAbierto, i * sizeof(Comanda), SEEK_SET);
        fread(&menor, sizeof(Comanda), 1, archivoDelDiaAbierto);

        // Busco una comanda con idMozo menor
        for (int j = i + 1; j < cantidadComandas; j++)
        {
            Comanda actual;

            fseek(archivoDelDiaAbierto, j * sizeof(Comanda), SEEK_SET);
            fread(&actual, sizeof(Comanda), 1, archivoDelDiaAbierto);

            if (actual.idMozo < menor.idMozo)
            {
                menor = actual;
                posicionMenor = j;
            }
        }

        // Si encontré una menor, intercambio
        if (posicionMenor != i)
        {
            Comanda original;

            // Guardo la comanda que estaba en i
            fseek(archivoDelDiaAbierto, i * sizeof(Comanda), SEEK_SET);
            fread(&original, sizeof(Comanda), 1, archivoDelDiaAbierto);

            // Escribo la menor en i
            fseek(archivoDelDiaAbierto, i * sizeof(Comanda), SEEK_SET);
            fwrite(&menor, sizeof(Comanda), 1, archivoDelDiaAbierto);

            // Escribo la original donde estaba la menor
            fseek(archivoDelDiaAbierto, posicionMenor * sizeof(Comanda), SEEK_SET);
            fwrite(&original, sizeof(Comanda), 1, archivoDelDiaAbierto);
        }
    }
}

void leerArchivoDeVentas(const char nombreArchivo[])
{
    FILE *f = fopen(nombreArchivo, "rb");

    if (f == NULL)
    {
        cout << "No se pudo abrir el archivo" << endl;
        return;
    }

    Comanda c;

    while (fread(&c, sizeof(Comanda), 1, f) == 1)
    {
        cout << "ID Mozo: " << c.idMozo << '\n'
             << "Código producto: " << c.codigoProducto << '\n'
             << "Cantidad: " << c.cantidad << '\n'
             << "Comisión: " << c.comision << "\n\n";
    }

    fclose(f);
}

int main()
{

    // Nombre del archivo del día
    //! Me quedo con el nombre guardado, porque primero tengo que abrirlo con ab+ para modificarlo, y después tengo que cerrarlo y abrirlo con rb+ para ordenarlo
    char archivoDelDia[50];
    if (!nombreDelArchivoDelDia(archivoDelDia))
    {
        return 0;
    }

    // Crear o abrir archivo del archivo del día
    FILE *fArchivoDelDia = fopen(archivoDelDia, "ab+");

    if (fArchivoDelDia == NULL)
    {
        cout << "Archivo inexistente o dañado" << endl;
        return -1;
    };

    // cout << "LECTURA DEL ARCHIVO ANTES DE GRABAR VENTAS" << endl;
    leerArchivoDeVentas(archivoDelDia);

    // Ingresar ventas
    int continuar = 1;

    do
    {
        registrarVenta(fArchivoDelDia);
        cout << "¿Desea insertar otra venta?" << '\n'
             << "Ingrese 0 (cero) para NO" << '\n'
             << "Ingrese 1 (uno) para SÍ" << endl;

        cin >> continuar;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Debe ingresar 0 (cero) o 1 (uno)" << endl;
            return -1;
        }
        else if (continuar != 0 && continuar != 1)
        {
            cout << "Valor inválido. Debe ingresar 0 (cero) o 1 (uno)" << endl;
            return -1;
        }

    } while (continuar);

    fclose(fArchivoDelDia);

    FILE *fArchivoDelDiaOrdenar = fopen(archivoDelDia, "rb+");

    if (fArchivoDelDiaOrdenar == NULL)
    {
        cout << "Archivo inexistente o dañado" << endl;
        return -1;
    };

    ordenarArchivoDelDiaPorSeleccion(fArchivoDelDiaOrdenar);

    fclose(fArchivoDelDiaOrdenar);

    // cout << "LECTURA DEL ARCHIVO DESPUÉS DE GRABAR VENTAS" << endl;
    leerArchivoDeVentas(archivoDelDia);

    return 0;
}

//! Lectura de Mozos
void leerMozos()
{
    FILE *archivo = fopen("mozos.dat", "rb");

    if (!archivo)
    {
        cout << "No se pudo abrir mozos.dat\n";
        return;
    }

    Mozo mozo;
    while (fread(&mozo, sizeof(Mozo), 1, archivo))
    {
        cout << "ID: " << mozo.idMozo << '\n'
             << "Nombre: " << mozo.nombre << '\n'
             << "Password: " << mozo.password << '\n'
             << "Total comisión: " << mozo.totalComision << "\n\n";
    }
    fclose(archivo);
}

//! Lectura de Inventario
void leerInventario()
{

    FILE *archivo = fopen("data/inventario.dat", "rb");

    if (!archivo)
    {
        cout << "No se pudo abrir inventario.dat\n";
        return;
    }

    Producto producto;
    while (fread(&producto, sizeof(Producto), 1, archivo))
    {
        cout << "Código: " << producto.codigo << '\n'
             << "Descripción: " << producto.descripcion << '\n'
             << "Precio: " << producto.precio << '\n'
             << "Stock: " << producto.stockActual << "\n\n";
    }
    fclose(archivo);
}

// FLUJO
//  fecha
//  → abrir comanda del día
//  → validar mozo
//  → validar contraseña
//  → buscar producto
//  → pedir cantidad
//  → validar stock
//  → calcular comisión
//  → guardar comanda
//  → descontar inventario
//  → repetir
//  → cerrar archivo
//  → ordenar