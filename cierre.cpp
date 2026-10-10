#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct comanda{
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

void ordenarArchivoSemanal(FILE *fsemanal){
    fseek(fsemanal, 0, SEEK_END);
    long bytes= ftell(fsemanal);
    int cantidadComandas= bytes/sizeof(comanda);

    if(cantidadComandas<=1){
        return;
    }

    for(int i=0; i<cantidadComandas-1; i++){
        int posicionMenor=i;
        comanda menor;
        fseek(fsemanal, i*sizeof(comanda), SEEK_SET);
        fread(&menor, sizeof(comanda), 1, fsemanal);

        for(int j=i+1; j<cantidadComandas; j++){
            comanda actual;
            fseek(fsemanal, j*sizeof(comanda), SEEK_SET);
            fread(&actual, sizeof(comanda), 1, fsemanal);

            if(actual.idMozo<menor.idMozo){
                posicionMenor=j;
                menor=actual;
            }
        }
        
        if(posicionMenor!=i){
            comanda original;
            fseek(fsemanal, i*sizeof(comanda), SEEK_SET);
            fread(&original, sizeof(comanda), 1, fsemanal);

            fseek(fsemanal, i*sizeof(comanda), SEEK_SET);
            fwrite(&menor, sizeof(comanda), 1, fsemanal);

            fseek(fsemanal, posicionMenor*sizeof(comanda), SEEK_SET);
            fwrite(&original, sizeof(comanda), 1, fsemanal);
        }
    }
}