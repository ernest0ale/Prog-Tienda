#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <Interface/consola.h>
#include <Global/colors.h>
#include <Global/definition.h>
#include <Services/reporteService.h>

using namespace std;

void Consola::pausa() {
    system("pause");
    system("cls");
}

void Consola::mostrarBienvenida(){
    ReporteService rs;
    system("cls");

    int porCaducar=rs.contarProductosPorCaducar();
    int caducados=rs.contarProductosCaducados();

    time_t ahora=time(0);

    tm* tiempo=localtime(&ahora);

    char fecha[11];
    char hora[9];

    strftime(fecha,sizeof(fecha),"%d/%m/%Y",tiempo);
    strftime(hora,sizeof(hora),"%H:%M",tiempo);

    cout<<"Sistema de Inventario Alpha v4.3 Pre Beta v1.0";
    saltoLinea;

    cout<<"Fecha: "<<fecha;
    saltoLinea;

    cout<<"Hora: "<<hora;
    saltoLinea;

    if(porCaducar>0 || caducados>0){
        saltoLinea;
        cout<<"Notificaciones";
        saltoLinea;
        cout<<"----------------";
        saltoLinea;

        if(porCaducar>0){
            cout<<"Productos por caducar: ";
            cout<<orange<<porCaducar<<reset;
            saltoLinea;
        }
        if(caducados>0){
            cout<<"Productos caducados: ";
            cout<<red<<caducados<<reset;
            saltoLinea;
        }
    }
}
