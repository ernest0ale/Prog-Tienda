#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <Data/database.h>
#include <Global/definition.h>
#include <Global/colors.h>
#include <Interface/consola.h>
#include <Interface/entradaDatos.h>
#include <Services/reporteService.h>

using namespace std;

void ReporteService::productosBajoStock(){

    sqlite3* db = Database::getDB();
    system("cls");

    const char* sql =
    "SELECT p.id,p.codigo,p.nombre,p.marca,"
    "IFNULL(SUM(l.stock),0),"
    "IFNULL(SUM(l.stock_minimo),0),"
    "CASE "
    "WHEN SUM(l.stock)*1.0/SUM(l.stock_minimo)<=0.5 THEN 'Critico' "
    "WHEN SUM(l.stock)*1.0/SUM(l.stock_minimo)<=0.7 THEN 'Bajo' "
    "END,"
    "(SUM(l.stock_minimo)-SUM(l.stock)) "
    "FROM productos p "
    "LEFT JOIN lotes l ON l.producto_id=p.id "
    "GROUP BY p.id "
    "HAVING SUM(l.stock_minimo)>0 "
    "AND SUM(l.stock)*1.0/SUM(l.stock_minimo)<=0.7 "
    "ORDER BY p.id;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    bool encontrado=false;

    while(sqlite3_step(stmt)==SQLITE_ROW){

        if(!encontrado){
            cout<<"Productos con bajo stock"; saltoLinea saltoLinea;
            cout<<left<<setw(5)<<"ID"<<setw(12)<<"Codigo"<<setw(20)<<"Producto"
                <<setw(15)<<"Marca"<<setw(10)<<"Stock"<<setw(10)<<"Min"
                <<setw(10)<<"Estado"<<setw(10)<<"Reponer";
            saltoLinea; lineaDivision saltoLinea;
            encontrado=true;
        }

        const char* estado=(const char*)sqlite3_column_text(stmt,6);
        string color=reset;
        if(strcmp(estado,"Critico")==0) color=red;
        else if(strcmp(estado,"Bajo")==0) color=orange;

        cout<<setw(5)<<sqlite3_column_int(stmt,0)
            <<setw(12)<<sqlite3_column_text(stmt,1)
            <<setw(20)<<sqlite3_column_text(stmt,2)
            <<setw(15)<<sqlite3_column_text(stmt,3)
            <<setw(10)<<sqlite3_column_int(stmt,4)
            <<setw(10)<<sqlite3_column_int(stmt,5)
            <<color<<setw(10)<<estado<<reset
            <<setw(10)<<sqlite3_column_int(stmt,7);
        saltoLinea;
    }

    if(!encontrado)
        cout<<yellow<<"No hay productos con bajo stock."<<reset;

    sqlite3_finalize(stmt);
}

void ReporteService::productoMasVendido() {
    sqlite3* db = Database::getDB();
    system("cls");

    const char* sql =
    "SELECT p.id, p.nombre, p.marca, SUM(v.cantidad) "
    "FROM ventas v "
    "JOIN lotes l ON l.id = v.lote_id "
    "JOIN productos p ON p.id = l.producto_id "
    "GROUP BY p.id "
    "ORDER BY SUM(v.cantidad) DESC "
    "LIMIT 5;"; // Limitar a los 5 productos más vendidos

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    cout << "Productos m\240s vendidos";
    saltoLinea;
    saltoLinea;

    cout << left
         << setw(5) << "ID"
         << setw(20) << "Producto"
         << setw(15) << "Marca"
         << setw(10) << "Vendidos";
    saltoLinea;
    lineaDivision;
    saltoLinea;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << setw(5) << sqlite3_column_int(stmt, 0)
             << setw(20) << (const char*)sqlite3_column_text(stmt, 1)
             << setw(15) << (const char*)sqlite3_column_text(stmt, 2)
             << setw(10) << sqlite3_column_int(stmt, 3);
        saltoLinea;
    }

    sqlite3_finalize(stmt);
}

void ReporteService::productoMenosVendido() {
    sqlite3* db = Database::getDB();
    system("cls");

    const char* sql =
    "SELECT p.id, p.nombre, p.marca, SUM(v.cantidad) "
    "FROM ventas v "
    "JOIN lotes l ON l.id = v.lote_id "
    "JOIN productos p ON p.id = l.producto_id "
    "GROUP BY p.id "
    "ORDER BY SUM(v.cantidad) ASC "
    "LIMIT 5;"; // Limitar a los 5 productos menos vendidos

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    cout << "Productos menos vendidos";
    saltoLinea;
    saltoLinea;

    cout << left
         << setw(5) << "ID"
         << setw(20) << "Producto"
         << setw(15) << "Marca"
         << setw(10) << "Vendidos";
    saltoLinea;
    lineaDivision;
    saltoLinea;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << setw(5) << sqlite3_column_int(stmt, 0)
             << setw(20) << (const char*)sqlite3_column_text(stmt, 1)
             << setw(15) << (const char*)sqlite3_column_text(stmt, 2)
             << setw(10) << sqlite3_column_int(stmt, 3);
        saltoLinea;
    }

    sqlite3_finalize(stmt);
}


void ReporteService::productosNuncaVendidos(){

    sqlite3* db = Database::getDB();
    system("cls");

    const char* sql =
    "SELECT p.id,p.codigo,p.nombre,p.marca "
    "FROM productos p "
    "WHERE NOT EXISTS ("
    "SELECT 1 FROM lotes l "
    "JOIN ventas v ON v.lote_id=l.id "
    "WHERE l.producto_id=p.id"
    ") ORDER BY p.id;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    bool encontrado=false;

    while(sqlite3_step(stmt)==SQLITE_ROW){

        if(!encontrado){
            cout<<"Productos nunca vendidos"; saltoLinea saltoLinea;
            cout<<left<<setw(5)<<"ID"<<setw(12)<<"Codigo"<<setw(20)<<"Producto"<<setw(15)<<"Marca";
            saltoLinea; lineaDivision saltoLinea;
            encontrado=true;
        }

        cout<<setw(5)<<sqlite3_column_int(stmt,0)
            <<setw(12)<<sqlite3_column_text(stmt,1)
            <<setw(20)<<sqlite3_column_text(stmt,2)
            <<setw(15)<<sqlite3_column_text(stmt,3);
        saltoLinea;
    }

    if(!encontrado)
        cout<<yellow<<"Todos los productos han sido vendidos."<<reset;

    sqlite3_finalize(stmt);
}

void ReporteService::productosCaducados(){

    sqlite3* db=Database::getDB();
    system("cls");

    const char* sql=
    "SELECT l.id,p.nombre,p.marca,l.lote,l.fecha_caducidad "
    "FROM lotes l "
    "JOIN productos p ON p.id=l.producto_id "
    "WHERE l.tiene_caducidad=1 "
    "AND julianday(l.fecha_caducidad)<julianday('now');";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);

    bool encontrado=false;

    while(sqlite3_step(stmt)==SQLITE_ROW){

        if(!encontrado){
            cout<<"Lotes caducados"; saltoLinea saltoLinea;

            cout<<left
                <<setw(5)<<"ID"
                <<setw(20)<<"Producto"
                <<setw(15)<<"Marca"
                <<setw(12)<<"Lote"
                <<setw(12)<<"Caducidad";
            saltoLinea; lineaDivision saltoLinea;

            encontrado=true;
        }

        cout<<setw(5)<<sqlite3_column_int(stmt,0)
            <<setw(20)<<sqlite3_column_text(stmt,1)
            <<setw(15)<<sqlite3_column_text(stmt,2)
            <<setw(12)<<sqlite3_column_text(stmt,3)
            <<red<<setw(12)<<sqlite3_column_text(stmt,4)<<reset;
        saltoLinea;
    }

    if(!encontrado){
        cout<<lightGreen<<"No hay lotes caducados."<<reset; saltoLinea;
    }

    sqlite3_finalize(stmt);
}

void ReporteService::perdidasPorCaducidad(){

    sqlite3* db = Database::getDB();
    system("cls");

    const char* sql =
    "SELECT l.id,p.codigo,p.nombre,p.marca,l.lote,l.stock,p.precio_compra "
    "FROM lotes l "
    "JOIN productos p ON p.id=l.producto_id "
    "WHERE l.tiene_caducidad=1 "
    "AND l.stock>0 "
    "AND julianday(l.fecha_caducidad)<julianday('now') "
    "ORDER BY p.id;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    bool encontrado=false;
    double total=0;

    while(sqlite3_step(stmt)==SQLITE_ROW){

        if(!encontrado){
            cout<<"Perdidas por caducidad"; saltoLinea saltoLinea;
            cout<<left<<setw(5)<<"ID"<<setw(12)<<"Codigo"<<setw(20)<<"Producto"
                <<setw(15)<<"Marca"<<setw(12)<<"Lote"<<setw(8)<<"Stock"
                <<setw(10)<<"Costo"<<setw(10)<<"Perdida";
            saltoLinea; lineaDivision saltoLinea;
            encontrado=true;
        }

        int stock=sqlite3_column_int(stmt,5);
        double costo=sqlite3_column_double(stmt,6);
        double perdida=stock*costo;
        total+=perdida;

        cout<<setw(5)<<sqlite3_column_int(stmt,0)
            <<setw(12)<<sqlite3_column_text(stmt,1)
            <<setw(20)<<sqlite3_column_text(stmt,2)
            <<setw(15)<<sqlite3_column_text(stmt,3)
            <<setw(12)<<sqlite3_column_text(stmt,4)
            <<setw(8)<<stock
            <<"$"<<setw(9)<<costo
            <<"$"<<setw(9)<<perdida;
        saltoLinea;
    }

    if(encontrado){
        saltoLinea; lineaDivision saltoLinea;
        cout<<red<<"Perdida total: $"<<total<<reset;
    }else{
        cout<<yellow<<"No hay perdidas por caducidad."<<reset;
    }

    sqlite3_finalize(stmt);
}
void ReporteService::ingresosTotales(){

    sqlite3* db = Database::getDB();
    system("cls");

    const char* sql =
    "SELECT IFNULL(SUM(v.cantidad*p.precio_venta),0) "
    "FROM ventas v "
    "JOIN lotes l ON l.id=v.lote_id "
    "JOIN productos p ON p.id=l.producto_id;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    double total=0;
    if(sqlite3_step(stmt)==SQLITE_ROW)
        total = sqlite3_column_double(stmt,0);

    sqlite3_finalize(stmt);

    cout<<"Ingresos totales"; saltoLinea;
    cout<<"$ "<<total; saltoLinea;
}
void ReporteService::productosMayorGanancia(){

    sqlite3* db=Database::getDB();

    system("cls");
    const char* sql=
    "SELECT id,nombre,marca,precio_compra,precio_venta,"
    "(precio_venta-precio_compra) AS ganancia,"
    "((precio_venta-precio_compra)/precio_compra)*100 AS porcentaje "
    "FROM productos "
    "ORDER BY porcentaje DESC;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db,sql,-1,&stmt, NULL);

    cout<<"Productos con mayor ganancia";
    saltoLinea
    saltoLinea

    cout<<left
        <<setw(5)<<"ID"
        <<setw(20)<<"Producto"
        <<setw(15)<<"Marca"
        <<setw(10)<<"Compra"
        <<setw(10)<<"Venta"
        <<setw(10)<<"Ganancia"
        <<setw(10)<<"%";
    saltoLinea

    lineaDivision
    saltoLinea

    while(sqlite3_step(stmt)==SQLITE_ROW){

        cout<<setw(5)<<sqlite3_column_int(stmt,0)
            <<setw(20)<<sqlite3_column_text(stmt,1)
            <<setw(15)<<sqlite3_column_text(stmt,2)
            <<"$"<<setw(10)<<sqlite3_column_double(stmt,3)
            <<"$"<<setw(10)<<sqlite3_column_double(stmt,4)
            <<"$"<<setw(10)<<sqlite3_column_double(stmt,5)
            <<setw(10)<<sqlite3_column_double(stmt,6);

        saltoLinea
    }

    sqlite3_finalize(stmt);
}

void ReporteService::ventasTotalesPorMes(){

    system("cls");
    sqlite3* db = Database::getDB();
    Validaciones v;

    bool opcion;
    int year=0;

    opcion=v.leerCadenaBool("Desea ver las ventas de un a\244o espec\241fico? (Y/N): ");

    if(opcion){
        year=v.leerCadenaEnteros("Ingrese el a\244o deseado: ");
        mostrarVentasPorYear(year);
    }else{
        const char* sql =
        "SELECT DISTINCT strftime('%Y',fecha) "
        "FROM ventas "
        "ORDER BY strftime('%Y',fecha) DESC;";

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db,sql,-1,&stmt,0);

        bool hayDatos=false;

        while(sqlite3_step(stmt)==SQLITE_ROW){
            hayDatos=true;
            year=atoi(reinterpret_cast<const char*>(sqlite3_column_text(stmt,0)));
            mostrarVentasPorYear(year);
            saltoLinea; saltoLinea;
        }

        if(!hayDatos){
            cout<<yellow<<"No existen ventas registradas."<<reset;
            saltoLinea
        }
        sqlite3_finalize(stmt);
    }
}

void ReporteService::mostrarVentasPorYear(int year) {
    sqlite3* db = Database::getDB();
    int i;

    const char* sql =
    "SELECT "
    "p.id, "
    "p.codigo, "
    "p.nombre, "
    "p.marca, "
    "IFNULL(SUM(CASE WHEN strftime('%m',v.fecha)='01' THEN v.cantidad END),0),"
    "IFNULL(SUM(CASE WHEN strftime('%m',v.fecha)='02' THEN v.cantidad END),0),"
    "IFNULL(SUM(CASE WHEN strftime('%m',v.fecha)='03' THEN v.cantidad END),0),"
    "IFNULL(SUM(CASE WHEN strftime('%m',v.fecha)='04' THEN v.cantidad END),0),"
    "IFNULL(SUM(CASE WHEN strftime('%m',v.fecha)='05' THEN v.cantidad END),0),"
    "IFNULL(SUM(CASE WHEN strftime('%m',v.fecha)='06' THEN v.cantidad END),0),"
    "IFNULL(SUM(CASE WHEN strftime('%m',v.fecha)='07' THEN v.cantidad END),0),"
    "IFNULL(SUM(CASE WHEN strftime('%m',v.fecha)='08' THEN v.cantidad END),0),"
    "IFNULL(SUM(CASE WHEN strftime('%m',v.fecha)='09' THEN v.cantidad END),0),"
    "IFNULL(SUM(CASE WHEN strftime('%m',v.fecha)='10' THEN v.cantidad END),0),"
    "IFNULL(SUM(CASE WHEN strftime('%m',v.fecha)='11' THEN v.cantidad END),0),"
    "IFNULL(SUM(CASE WHEN strftime('%m',v.fecha)='12' THEN v.cantidad END),0),"
    "IFNULL(SUM(v.cantidad),0) "
    "FROM ventas v "
    "JOIN lotes l ON l.id=v.lote_id "
    "JOIN productos p ON p.id=l.producto_id "
    "WHERE strftime('%Y',v.fecha)=? "
    "GROUP BY p.id,p.codigo,p.nombre,p.marca "
    "ORDER BY p.id;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    // Usar stringstream para convertir el año a string
    std::stringstream yearStream;
    yearStream << year;
    std::string yearStr = yearStream.str();

    sqlite3_bind_text(stmt, 1, yearStr.c_str(), -1, SQLITE_TRANSIENT);

    cout << "Ventas totales por producto";
    saltoLinea;
    cout << "A\244o: " << year;
    saltoLinea;
    saltoLinea;

    cout << left
         << setw(5) << "ID"
         << setw(12) << "C\242digo"
         << setw(20) << "Producto"
         << setw(15) << "Marca"
         << setw(5) << "Ene"
         << setw(5) << "Feb"
         << setw(5) << "Mar"
         << setw(5) << "Abr"
         << setw(5) << "May"
         << setw(5) << "Jun"
         << setw(5) << "Jul"
         << setw(5) << "Ago"
         << setw(5) << "Sep"
         << setw(5) << "Oct"
         << setw(5) << "Nov"
         << setw(5) << "Dic"
         << setw(7) << "Total";

    saltoLinea;
    lineaDivision;
    saltoLinea;

    bool hayDatos = false;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hayDatos = true;

        cout << setw(5) << sqlite3_column_int(stmt, 0)
             << setw(12) << (const char*)sqlite3_column_text(stmt, 1)
             << setw(20) << (const char*)sqlite3_column_text(stmt, 2)
             << setw(15) << (const char*)sqlite3_column_text(stmt, 3);

        for (i = 4; i <= 15; i++)
            cout << setw(5) << sqlite3_column_int(stmt, i);

        cout << setw(7) << sqlite3_column_int(stmt, 16);
        saltoLinea;
    }

    if (!hayDatos){
        cout << yellow << "No hubo ventas en este año." << reset;
        saltoLinea
    }
    sqlite3_finalize(stmt);
}

void ReporteService::productosCercaCaducidad(){

    sqlite3* db = Database::getDB();
    system("cls");

    const char* sql =
    "SELECT p.id,p.codigo,p.nombre,p.marca,l.lote,"
    "(julianday(l.fecha_caducidad)-julianday('now')) "
    "FROM lotes l "
    "JOIN productos p ON p.id=l.producto_id "
    "WHERE l.tiene_caducidad=1 "
    "AND julianday(l.fecha_caducidad)>julianday('now') "
    "AND (julianday(l.fecha_caducidad)-julianday('now'))<=90 "
    "ORDER BY p.id;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    bool encontrado=false;

    while(sqlite3_step(stmt)==SQLITE_ROW){

        if(!encontrado){
            cout<<"Productos cerca de caducar"; saltoLinea saltoLinea;
            cout<<left<<setw(5)<<"ID"<<setw(12)<<"Codigo"<<setw(20)<<"Producto"
                <<setw(15)<<"Marca"<<setw(12)<<"Lote"<<setw(10)<<"Dias";
            saltoLinea; lineaDivision saltoLinea;
            encontrado=true;
        }

        int dias = sqlite3_column_int(stmt,5);
        string color = (dias<=7)?red:(dias<=15)?orange:(dias<=30)?yellow:reset;

        cout<<setw(5)<<sqlite3_column_int(stmt,0)
            <<setw(12)<<sqlite3_column_text(stmt,1)
            <<setw(20)<<sqlite3_column_text(stmt,2)
            <<setw(15)<<sqlite3_column_text(stmt,3)
            <<setw(12)<<sqlite3_column_text(stmt,4)
            <<color<<setw(10)<<dias<<reset;
        saltoLinea;
    }

    if(!encontrado)
        cout<<yellow<<"No hay productos cercanos a caducar."<<reset;

    sqlite3_finalize(stmt);
}

void ReporteService::historialPrecios(){

    sqlite3* db = Database::getDB();
    system("cls");

    const char* sql =
    "SELECT p.id,p.nombre,p.marca,h.precio_compra,h.precio_venta,h.fecha "
    "FROM historial_precios h "
    "JOIN productos p ON p.id = h.producto_id "
    "ORDER BY h.fecha DESC;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);

    bool encontrado=false;

    while(sqlite3_step(stmt)==SQLITE_ROW){

        if(!encontrado){

            cout<<"Historial de cambios de precios";
            saltoLinea; saltoLinea;

            cout<<left
                <<setw(5)<<"ID"
                <<setw(20)<<"Producto"
                <<setw(15)<<"Marca"
                <<setw(12)<<"Compra"
                <<setw(12)<<"Venta"
                <<setw(12)<<"Fecha";
            saltoLinea;

            lineaDivision
            saltoLinea;

            encontrado=true;
        }

        cout<<setw(5)<<sqlite3_column_int(stmt,0)
            <<setw(20)<<sqlite3_column_text(stmt,1)
            <<setw(15)<<sqlite3_column_text(stmt,2)
            <<setw(12)<<sqlite3_column_double(stmt,3)
            <<setw(12)<<sqlite3_column_double(stmt,4)
            <<setw(12)<<sqlite3_column_text(stmt,5);

        saltoLinea;
    }

    if(!encontrado){

        cout<<yellow<<"No existen cambios de precios registrados."<<reset;
        saltoLinea;
    }

    sqlite3_finalize(stmt);
}



//Funciones auxiliares para la presentación
int ReporteService::contarProductosPorCaducar(){

    sqlite3* db = Database::getDB();

    const char* sql =
    "SELECT COUNT(*) "
    "FROM lotes "
    "WHERE tiene_caducidad=1 "
    "AND (julianday(fecha_caducidad)-julianday('now')) BETWEEN 1 AND 30;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);

    int total = 0;

    if(sqlite3_step(stmt)==SQLITE_ROW)
        total = sqlite3_column_int(stmt,0);

    sqlite3_finalize(stmt);

    return total;
}

int ReporteService::contarProductosCaducados(){

    sqlite3* db = Database::getDB();

    const char* sql =
    "SELECT COUNT(*) "
    "FROM lotes "
    "WHERE tiene_caducidad=1 "
    "AND julianday(fecha_caducidad) < julianday('now');";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);

    int total = 0;

    if(sqlite3_step(stmt)==SQLITE_ROW)
        total = sqlite3_column_int(stmt,0);

    sqlite3_finalize(stmt);

    return total;
}
