#include <iostream>
#include <App/app.h>
#include <Global/colors.h>
#include <Global/definition.h>
#include <Data/database.h>
#include <Interface/menu.h>
#include <Interface/consola.h>
#include <Services/ventaService.h>
#include <Interface/entradaDatos.h>

using namespace std;

/*void App::run() {

    bool dbOK = Database::inicializar();

    if(dbOK){
        Menu menu;
        Consola c;

        c.mostrarBienvenida();
        c.pausa();
        menu.menuPrincipal();


    }else{
        cout<<"Error inicializando la base de datos";
        saltoLinea
    }

}*/

void App::run(){

    bool dbOK = Database::inicializar();
    Menu menu;
    VentaService vs;
    Consola c;
    Validaciones v;

    system("cls");
    if(!dbOK){
        cout<<orange<<"Error inicializando la base de datos"<<reset; saltoLinea
    }else{

        int op;
        bool salir=false;

        c.mostrarBienvenida();
        c.pausa();
        do{
            system("cls");
            cout<<"===== M\242dulos ====="; saltoLinea
            cout<<gray<<"1. Administrador"<<reset; saltoLinea
            cout<<gray<<"2. Cajero"<<reset; saltoLinea
            cout<<gray<<"3. Almacenero"<<reset; saltoLinea
            cout<<gray<<"4. Salir"<<reset; saltoLinea

            op = v.leerCadenaOpcion("Seleccione el m\242dulo al que pertenece: ");

            switch(op){
                case 1:
                    menu.menuPrincipal();
                    break;
                case 2:
                    menu.menuCajero();
                    break;
                case 3:
                    menu.menuAlmacen();
                    break;
                case 4:
                    salir=true;
                    break;
                default:
                    cout<<orange<<"Opci\242n inv\240lida."<<reset;
                    saltoLinea
                    cout<<yellow<<"Intente nuevamente."<<reset;
                    saltoLinea
                    c.pausa();
                    break;
            }

        }while(!salir);
    }
}
