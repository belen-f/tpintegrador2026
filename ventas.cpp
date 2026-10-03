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

void crearArchivoDelDia()
{
    char fecha[11];
    do
    {
        cout << "Ingrese la fecha del día en formato DD-MM-AAAA";

        cin >> fecha;

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
             (fecha[0] == '0' && fecha[1] == '0') || (fecha[3] == '0' && fecha[4] == '0') );

    char nombreArchivo[50];
    strcpy(nombreArchivo, "comandas_"); // copia una cadena dentro de otra
    strcat(nombreArchivo, fecha);
    strcat(nombreArchivo, ".dat");
    FILE *f = fopen(nombreArchivo, "ab");
};

int main()
 {

    //Crear el archivo del día
    crearArchivoDelDia();

//! Prueba de lectura de Mozos
//     FILE *archivo = fopen("Mozos.dat", "rb");

//     if (!archivo)
//     {
//         cout << "No se pudo abrir Mozos.dat\n";
//         return 1;
//     }

//     Mozo mozo;
//     while (fread(&mozo, sizeof(Mozo), 1, archivo))
//     {
//         cout << "ID: " << mozo.idMozo << '\n'
//              << "Nombre: " << mozo.nombre << '\n'
//              << "Password: " << mozo.password << '\n'
//              << "Total comisión: " << mozo.totalComision << "\n\n";
//     }
//     fclose(archivo);
}