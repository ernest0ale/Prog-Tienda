#include <iostream>
#include <App/app.h>
#include <Global/definition.h>
#include <Data/database.h>
#include <Interface/menu.h>
#include <Interface/consola.h>


using namespace std;

void App::run() {

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

}
