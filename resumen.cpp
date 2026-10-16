#include <iostream>
#include <stdio.h>
#include <cstring>
#include <fstream>
using namespace std;

struct Comanda{
  int idMozo;
  int codigoProducto;
  int cantidad;
  float comision;
};

int main() {
  char archivoSemanal[50];
  cout<< "Ingresar nombre de archivo semanal: ";
  cin>> archivoSemanal;

  FILE *f = fopen(archivoSemanal, "rb");
  if (f == NULL){
    cout<< "No se pudo abrir el archivo."<< endl;
    return 1;
}

Comanda r;
int totalProductosBuffet = 0;

int leido = fread(&r, sizeof(Comanda), 1, f); //lectura anticipada

if (leido != 1){
  cout<< "El archivo " << archivoSemanal << "no pudo leerse." << endl;
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

    leido = fread(&r, sizeof(Comanda), 1, f);
  }

  cout<< "INFORMACIÓN SEMANAL: " << endl; 
  cout<< "Mozo ID= "<< mozoActual << endl;
  cout<< "Productos vendidos= " << totalProductosMozo << endl;
  cout<< "Comision a pagar= " << totalComisionMozo << endl;
  cout<< "////////////////////////////////////////////////" << endl;

  totalProductosBuffet += totalProductosMozo;
}

cout<< "---------------------------------------------------------------------" << endl;
cout<< "TOTAL DE PRODUCTOS VENDIDOS EN EL BUFFET= " << totalProductosBuffet << endl;

fclose(f);
return 0;
}

