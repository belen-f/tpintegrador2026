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

void apareoArchivos(FILE *a, FILE * b, FILE *c){
    comanda ra, rb;
    int la= fread(&ra, sizeof(comanda), 1, a);
    int lb= fread(&rb, sizeof(comanda), 1, b);

    while(la==1 && lb==1){
        if(ra.idMozo<=rb.idMozo){
            fwrite(&ra, sizeof(comanda), 1, c);
            la= fread(&ra, sizeof(comanda), 1, a);
        } else {
            fwrite(&rb, sizeof(comanda), 1, c);
            lb= fread(&rb, sizeof(comanda), 1, b);
        }
    }
    while(la==1){
        fwrite(&ra, sizeof(comanda), 1, c);
        la= fread(&ra, sizeof(comanda), 1, a);
    }
    while(lb==1){
        fwrite(&rb, sizeof(comanda), 1, c);
        lb= fread(&rb, sizeof(comanda), 1, b);
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
    char continuar= 's';
    int diasProcesados=0;

    while(continuar == 's' || continuar == 'S'){
        char fechaDia[11];
        cout<<"Ingrese la fecha del dia (DD-MM-AAAA): ";
        cin>>fechaDia;

        char archivoDiario[50];
        sprintf(archivoDiario,"comandas_%s.dat",fechaDia);

        FILE *fdiario = fopen(archivoDiario, "rb");
        if(fdiario==NULL){
            cout<<"No se puede abrir el archivo. "<<archivoDiario<< endl;
        } else {
            if(diasProcesados==0){
                FILE *fsemanal = fopen(archivoSemanal, "wb");
                if(fsemanal==NULL){
                    cout<<"No se pudo crear el archivo semanal."<<archivoSemanal<<endl;
                    fclose(fdiario);
                    return 1;
                }
                comanda venta;
                int contador=0;

                while(fread(&venta,sizeof(comanda),1,fdiario)==1){
                    fwrite(&venta,sizeof(comanda),1,fsemanal);
                    contador++;
                }
                cout<<"Se procesaron "<<contador<<" comandas del dia "<<fechaDia<<endl;
                fclose(fsemanal);
            } else{
                FILE *fsemanal = fopen(archivoSemanal, "rb");
                
                FILE *ftemp = fopen("temp.dat", "wb");
                if(fsemanal==NULL || ftemp==NULL){
                    cout<<"No se pudo abrir el archivo."<<endl;

                    if(fsemanal!=NULL)
                        fclose(fsemanal);
                    if(ftemp!=NULL)
                        fclose(ftemp);

                    fclose(fdiario);
                    return 1;
                        
                    apareoArchivos (fsemanal, fdiario, ftemp);
                    fclose(fsemanal);
                    fclose(ftemp);
                }
                remove(archivoSemanal);
                rename("temp.dat",archivoSemanal);
                cout<<"Las comandas del dia "<<fechaDia<<" fueron agregadas al cierre semanal."<<endl;
            }
            fclose(fdiario);
            diasProcesados++;
        }
        cout<<"Desea procesar otro dia? (s/n): ";
        cin>>continuar;
    }
    if (diasProcesados==0){
    cout<<"No se procesaron dias."<<endl;
    return 0;
    }
    cout<<"Cierre de semana completado."<<endl;
    cout<<"Archivo semanal generado: "<<archivoSemanal<<endl;
    return 0;
}