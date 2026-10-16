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

int main(){
    cout<<"MODULO DE CIERRE DE SEMANA"<<endl;
    
    int numeroSemana,mes;
    cout<<"Ingrese el numero de semana: ";
    cin>>numeroSemana;
    cout<<"Ingrese el mes: ";
    cin>>mes;

    char archivoSemanal[30];
    sprintf(archivoSemanal,"comandas_semana_s%02d-%02d.dat",numeroSemana,mes);
    FILE *fsemanal = fopen(archivoSemanal, "wb+");
    if(!fsemanal){
        cout<<"Error al abrir el archivo"<<archivoSemanal<<endl;
        return 1;
    }
    char continuar= 's';
    int diasProcesados=0;

    while(continuar=='s'|| continuar == 'S'){
        char fechaDia[11];
        cout<<"Ingrese la fecha del dia (DD-MM-AAAA): ";
        cin>>fechaDia;

        char archivoDiario[50];
        strcpy(archivoDiario,"comandas_");
        strcat(archivoDiario,fechaDia);
        strcat(archivoDiario,".dat");


        FILE *fdiario = fopen(archivoDiario, "rb");
        if(fdiario==NULL){
             cout<<"Error al abrir el archivo. "<<archivoDiario<< endl;
        } else {
         comanda c;
         int contador=0; 
         while(fread(&c,sizeof(comanda),1,fdiario)==1){
            fwrite(&c,sizeof(comanda),1,fsemanal);
            contador++; 
        }
            fclose(fdiario);
            diasProcesados++;
            cout<<"Se procesaron "<<contador<<" comandas del dia "<<fechaDia<<endl;
        }
        cout<<"Desea procesar otro dia? (s/n): ";
        cin>>continuar;
    }

    if(diasProcesados==0){
        cout<<"No se procesaron dias, no se puede generar el archivo semanal."<<endl;
        fclose(fsemanal);
        return 0;
    }

    cout<<"Ordenar el archivo semanal por id de mozo..."<<endl;
    ordenarArchivoSemanal(fsemanal);

    fclose(fsemanal);
    cout<<"Cierre de semana completado."<<endl;
    cout<<"Archivo semanal generado: "<<archivoSemanal<<endl;

    return 0;
}