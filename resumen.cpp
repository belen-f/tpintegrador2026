#include <iostream>
#include <stdio.h>
#include <cstring>
#include <fstream>
using namespace std;

Struct Comanda{
  char fecha[11]
  int idMozo;
  int codigoProducto;
  int cantidad;
  float comision;
}

int main() {
  char nombreArchivo[50]
  cout<< "Ingresar nombre de archivo semanal: "
    cin<< nombreArchivo

  FILE *f = fopen(nombreArchivo, "rb");
  if (f == NULL){
    cout<< "No se pudo abrir el archivo."<< endl;
    return 1;
}

Comanda r;
int totalProductosBuffet = 0;

int leido = fread(&r, sizeof(Comanda), 1, f); //lectura anticipada

if (leido != 1){
  cout<< "El archivo " << nombreArchivo << "no puedo abrirse." << endl;
  fclose(f);
  return 0;
}

while (leido == 1) {
  int mozoActual = r.idMozo;
  int totalProductosMozo = 0;
  float totalComisionMozo = 0;

  while (leido ==1 && r.idMozo == mozoActual){
    totalProductosMozo += r.cantidad;
    totalComisionMozo += r.comision;
    totalProductosBuffet += r.cantidadTotal;

    leido = fread(&r, sizeof(Comanda), 1, f);
  }

  cout<< "INFORMACIÓN SEMANAL: " << "
     " << "Mozo ID= "<< mozoActual <<"
     " << "Productos vendidos= " << totalProductosMozo << "
     " << "Comision a pagar= " << totalComisionMozo
}
