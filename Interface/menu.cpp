#include <iostream>
#include <stdlib.h>
#include <vector>
#include <windows.h>
#include <Interface/menu.h>
#include <Global/definition.h>
#include <Global/colors.h>
#include <Services/productoService.h>
#include <Services/ventaService.h>
#include <Services/reporteService.h>
#include <Interface/consola.h>
#include <Interface/entradaDatos.h>
#include <Modules/cajeroModule.h>
#include <Services/monedaService.h>

using namespace std;

void Menu::menuPrincipal(){

    int op=0;
    bool salir=false;

    ProductoService ps;
    VentaService vs;
    Menu menu;
    Validaciones v;
    Consola c;
    CajeroModule cj;

    do{
        system("cls");
        cout<<white<<"===== Men\243 Principal ====="<<reset; saltoLinea
        cout<<gray<<"1. Inventario"<<reset; saltoLinea
        cout<<gray<<"2. Registrar venta"<<reset; saltoLinea
        cout<<gray<<"3. Listados"<<reset; saltoLinea
        cout<<gray<<"4. Reportes"<<reset; saltoLinea
        cout<<gray<<"5. Monedas"<<reset; saltoLinea
        cout<<gray<<"6. Salir"<<reset; saltoLinea

        op=v.leerCadenaOpcion("Selecciona una opci\242n: ");

        switch(op){
            case 1:
                menu.menuInventario();
                break;
            case 2:
                if(ps.existenProductos()){
                    system("cls");
                    cj.iniciar();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay productos registrados."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, ingrese un producto."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 3:
                menu.menuListados();
                break;
            case 4:
                menu.menuReportes();
                break;
            case 5:
                menu.configurarMonedas();
                break;
            case 6:
                Sleep(100);
                TerminateProcess(GetCurrentProcess(), 0);
                break;
            default:
                cout<<orange<<"Opci\242n inv\240lida."<<reset;
                saltoLinea
                cout<<yellow<<"Intente nuevamente."<<reset;
                saltoLinea
                c.pausa();
                break;
        }
    } while(!salir);
}

void Menu::menuInventario(){

    int op=0;
    bool salir=false;

    ProductoService ps;
    Validaciones v;
    Consola c;

    do{
        system("cls");
        cout<<white<<"===== Men\243 de inventario ====="<<reset; saltoLinea
        cout<<gray<<"1. Agregar producto"<<reset; saltoLinea
        cout<<gray<<"2. Eliminar producto"<<reset; saltoLinea
        cout<<gray<<"3. Cambiar precio de venta"<<reset; saltoLinea
        cout<<gray<<"4. Volver al men\243 principal"<<reset; saltoLinea

        op=v.leerCadenaOpcion("Selecciona una opci\242n: ");

        switch(op){
            case 1:
                ps.registrarInventario();
                c.pausa();
                break;
            case 2:
                if(ps.existenProductos()){
                    system("cls");
                    ps.eliminarProducto();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay productos registrados."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, ingrese un producto."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 3:
                if(ps.existenProductos()){
                    system("cls");
                    ps.cambiarPrecio();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay productos registrados."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, ingrese un producto."<<reset;
                    saltoLinea
                }
                c.pausa();
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
    } while(!salir);
}

void Menu::menuListados(){

    int op=0;
    bool salir=false;

    ProductoService ps;
    VentaService vs;
    Validaciones v;
    Consola c;

    do{
        system("cls");
        cout<<white<<"===== Men\243 de listados ====="<<reset; saltoLinea
        cout<<gray<<"1. Lista de productos"<<reset; saltoLinea
        cout<<gray<<"2. Lista de lotes"<<reset; saltoLinea
        cout<<gray<<"3. Lista de ventas"<<reset; saltoLinea
        cout<<gray<<"4. Volver al men\243 principal"<<reset; saltoLinea

        op=v.leerCadenaOpcion("Selecciona una opci\242n: ");

        switch(op){
            case 1:
                if(ps.existenProductos()){
                    system("cls");
                    ps.listarProductos();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay productos registrados."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, ingrese un producto."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 2:
                if(ps.existenProductos()){
                    system("cls");
                    ps.listarLotes();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay productos registrados."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, ingrese un producto."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 3:
                if(vs.existenVentas()){
                    system("cls");
                    vs.listarVentas();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay ventas registradas."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, registre una venta."<<reset;
                    saltoLinea
                }
                c.pausa();
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
    } while(!salir);
}

void Menu::menuReportes() {

    int op=0;
    bool salir=false;
    Validaciones v;
    Consola c;
    Menu menu;

    do{
        system("cls");
        cout<<white<<"====Men\243 de reportes===="<<reset; saltoLinea
        cout<<gray<<"1. Reportes de productos"<<reset; saltoLinea
        cout<<gray<<"2. Reportes de ventas"<<reset; saltoLinea
        cout<<gray<<"3. Volver al men\243 principal"<<reset; saltoLinea
        op=v.leerCadenaOpcion("Selecciona una opci\242n: ");

        switch(op){
            case 1:
                menu.menuReportesProductos();
                break;
            case 2:
                menu.menuReportesVentas();
                break;
            case 3:
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

void Menu::menuReportesProductos(){
    ProductoService ps;
    VentaService vs;
    ReporteService rs;
    Validaciones v;
    Consola c;
    bool salir=false;
    int op=0;

    do{
        system("cls");
        cout<<"==== Reportes de productos ===="; saltoLinea
        cout<<gray<<"1. Bajo stock"<<reset; saltoLinea
        cout<<gray<<"2. Cerca de caducidad"<<reset; saltoLinea
        cout<<gray<<"3. Caducados"<<reset; saltoLinea
        cout<<gray<<"4. Nunca vendidos"<<reset; saltoLinea
        cout<<gray<<"5. Mayor ganancia"<<reset; saltoLinea
        cout<<gray<<"6. Historial de precios"<<reset; saltoLinea
        cout<<gray<<"7. P\202rdidas por caducidad"<<reset; saltoLinea
        cout<<gray<<"8. Volver al men\243 de reportes"<<reset; saltoLinea
        op=v.leerCadenaOpcion("Selecciona una opci\242n: ");

        switch(op){
            case 1:
                if(ps.existenProductos()){
                    system("cls");
                    rs.productosBajoStock();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay productos registrados."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, registre un producto."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 2:
                if(ps.existenProductos()){
                    system("cls");
                    rs.productosCercaCaducidad();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay productos registrados."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, ingrese un producto."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 3:
                if(ps.existenProductos()){
                    system("cls");
                    rs.productosCaducados();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay productos registrados."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, ingrese un producto."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 4:
                if(ps.existenProductos() && vs.existenVentas()){
                    system("cls");
                    rs.productosNuncaVendidos();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay ventas registradas."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, registre una venta."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 5:
                if(ps.existenProductos()){
                    system("cls");
                    rs.productosMayorGanancia();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay ventas registradas."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, registre una venta."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 6:
                if(ps.existenProductos()){
                    rs.historialPrecios();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay ventas registradas."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, registre una venta."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 7:
                if(ps.existenProductos()){
                    rs.perdidasPorCaducidad();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay ventas registradas."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, registre una venta."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 8:
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

void Menu::menuReportesVentas(){
    VentaService vs;
    ReporteService rs;
    Validaciones v;
    Consola c;
    bool salir=false;
    int op=0;

    do{
        system("cls");
        cout<<"====Reportes de ventas===="; saltoLinea
        cout<<gray<<"1. Productos m\240s vendidos"<<reset; saltoLinea
        cout<<gray<<"2. Productos menos vendidos"<<reset; saltoLinea
        cout<<gray<<"3. Ingresos totales"<<reset; saltoLinea
        cout<<gray<<"4. Ventas totales por mes"<<reset; saltoLinea
        cout<<gray<<"5. Volver al men\243 de reportes"<<reset; saltoLinea
        op=v.leerCadenaOpcion("Selecciona una opci\242n: ");

        switch(op){
            case 1:
                if(vs.existenVentas()){
                    system("cls");
                    rs.productoMasVendido();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay ventas registradas."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, registre una venta."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 2:
                if(vs.existenVentas()){
                    system("cls");
                    rs.productoMenosVendido();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay ventas registradas."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, registre una venta."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 3:
                if(vs.existenVentas()){
                    system("cls");
                    rs.ingresosTotales();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay ventas registradas."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, registre una venta."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 4:
                if(vs.existenVentas()){
                    system("cls");
                    rs.ventasTotalesPorMes();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay ventas registradas."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, registre una venta."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 5:
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

void Menu::configurarMonedas(){
    MonedaService ms;
    Validaciones v;
    Consola c;
    bool actualizar;

    system("cls");
    ms.listarMonedas();
    actualizar = v.leerCadenaBool("\250Desea actualizar una tasa de cambio? (Y/N): ");

    if(actualizar){
        string moneda;
        float nuevaTasa;
        double doubleNuevaTasa;

        moneda=v.leerMonedaExtranjera("Ingrese la moneda (USD/EUR): ");
        nuevaTasa=v.validarFloat("Nueva tasa ", monedaMin, monedaMax);

        doubleNuevaTasa=(double)nuevaTasa;
        ms.actualizarTasa(moneda, doubleNuevaTasa);
        cout<<lightGreen<<"Tasa actualizada correctamente"<<reset; saltoLinea
        c.pausa();
    }
}

void Menu::menuCajero(){
    int op = 0;
    bool salir = false;

    VentaService vs;
    ProductoService ps;
    Validaciones v;
    Consola c;

    do{
        system("cls");

        cout<<"===== M\242dulo de cajero ====="; saltoLinea
        cout<<gray<<"1. Registrar venta"<<reset; saltoLinea
        cout<<gray<<"2. Listar productos"<<reset; saltoLinea
        cout<<gray<<"3. Listar lotes"<<reset; saltoLinea
        cout<<gray<<"4. Listar ventas"<<reset; saltoLinea
        cout<<gray<<"5. Salir"<<reset; saltoLinea

        op = v.leerCadenaOpcion("Seleccione una opci\242n: ");

        switch(op){
            case 1:
                if(ps.existenProductos()){
                    vs.iniciarVenta();
                }else{
                    cout<<orange<<"Error, no hay productos registrados"<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, ingrese un producto."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;

            case 2:
                if(ps.existenProductos()){
                    ps.listarProductos();
                }else{
                    cout<<orange<<"Error, no hay productos registrados"<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, ingrese un producto."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;

            case 3:
                if(ps.existenProductos()){
                    ps.listarLotes();
                }else{
                    cout<<orange<<"Error, no hay productos registrados"<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, ingrese un producto."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;

            case 4:
                if(vs.existenVentas()){
                    vs.listarVentas();
                }else{
                    cout<<orange<<"Error, no hay ventas registradas"<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, registre una venta."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 5:
                Sleep(100);
                TerminateProcess(GetCurrentProcess(), 0);
                break;
            default:
                cout<<orange<<"Opci\242n inv\240lida."<<reset;
                saltoLinea
                cout<<yellow<<"Intente nuevamente."<<reset;
                saltoLinea
                break;
        }

    }while(!salir);
}

void Menu::menuAlmacen(){
    int op = 0;
    bool salir = false;

    ProductoService ps;
    Validaciones v;
    Consola c;

    do{
        system("cls");
        cout<<"===== M\242dulo almac\202n====="; saltoLinea
        cout<<gray<<"1. Agregar producto"<<reset; saltoLinea
        cout<<gray<<"2. Eliminar producto"<<reset; saltoLinea
        cout<<gray<<"3. Listar productos"<<reset; saltoLinea
        cout<<gray<<"4. Listar lotes"<<reset; saltoLinea
        cout<<gray<<"5. Salir"<<reset; saltoLinea

        op = v.leerCadenaOpcion("Seleccione una opci\242: ");

        switch(op){
            case 1:
                ps.registrarInventario();
                break;
            case 2:
                if(ps.existenProductos()){
                    system("cls");
                    ps.eliminarProducto();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay productos registrados."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, ingrese un producto."<<reset;
                    saltoLinea
                }
            case 3:
                if(ps.existenProductos()){
                    system("cls");
                    ps.listarProductos();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay productos registrados."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, ingrese un producto."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 4:
                if(ps.existenProductos()){
                    system("cls");
                    ps.listarLotes();
                }else{
                    saltoLinea
                    cout<<orange<<"Error: No hay productos registrados."<<reset;
                    saltoLinea
                    cout<<yellow<<"Por favor, ingrese un producto."<<reset;
                    saltoLinea
                }
                c.pausa();
                break;
            case 5:
                Sleep(100);
                TerminateProcess(GetCurrentProcess(), 0);
                break;
            default:
                cout<<orange<<"Opci\242n inv\240lida."<<reset;
                saltoLinea
                cout<<yellow<<"Intente nuevamente."<<reset;
                saltoLinea
                break;
        }
        c.pausa();
    }while(!salir);
}
