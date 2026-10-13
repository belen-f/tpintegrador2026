#include <iostream>
#include <stdio.h>
#include <cstring>
#include <fstream>
using namespace std;

Struct Comanda{
  int idMozo;
  int codigoProducto;
  int cantidad;
  float comision;
}

int main() {
  char nombreArchivo[50]
  cout<< "Ingresar nombre de archivo semanal"
    cin<< nombreArchivo

  FILE *archivo = fopen(nombreArchivo, "rb");
  if (archivo == NULL){
    cout<< "No se pudo abrir el archivo."<< endl;
    return 1;
}

Comanda venta;
int totalProductosBuffet = 0;


