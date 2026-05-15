#include <iostream>
#include <stdbool.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <iomanip>
#include <Data/database.h>
#include <Interface/entradaDatos.h>
#include <Interface/consola.h>
#include <Services/productoService.h>
#include <Services/ventaService.h>
#include <Global/definition.h>
#include <Global/colors.h>
#include <Utils/SQLite/sqlite3.h>

using namespace std;

vector<Producto> ProductoService::obtenerProductos(){
    vector<Producto> lista;

    sqlite3* db = Database::getDB();

    const char* sql = "SELECT id, codigo, nombre, marca, precio_venta FROM productos;";

    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK){
        while(sqlite3_step(stmt) == SQLITE_ROW){
            Producto p;

            p.id = sqlite3_column_int(stmt, 0);
            p.codigo = (char*)sqlite3_column_text(stmt, 1);
            p.nombre = (char*)sqlite3_column_text(stmt, 2);
            p.marca = (char*)sqlite3_column_text(stmt, 3);
            p.precioVenta = sqlite3_column_double(stmt, 4);

            lista.push_back(p);
        }
    }

    sqlite3_finalize(stmt);
    return lista;
}


void ProductoService::listarProductos() {
    system("cls");

    sqlite3* db = Database::getDB();

    const char* sql =
    "SELECT p.id, p.codigo, p.nombre, p.marca, p.precio_venta, SUM(l.stock) "
    "FROM productos p LEFT JOIN lotes l ON p.id = l.producto_id "
    "GROUP BY p.id, p.codigo, p.nombre, p.marca "
    "ORDER BY p.id ASC;"; // Asegúrate de que el ORDER BY esté en la misma cadena

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        cout << red << "Error al preparar la consulta: " << sqlite3_errmsg(db) << reset;
        return;
    }

    cout << white << "=== Productos registrados ===" << reset;
    saltoLinea;
    saltoLinea;

    cout << left
         << setw(5) << "ID"
         << setw(15) << "C\242digo"
         << setw(20) << "Nombre"
         << setw(15) << "Marca"
         << setw(10) << "Precio"
         << setw(10) << "Stock";
    saltoLinea;
    lineaDivision;
    saltoLinea;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << setw(5) << sqlite3_column_int(stmt, 0)
             << setw(15) << (const char*)sqlite3_column_text(stmt, 1)
             << setw(20) << (const char*)sqlite3_column_text(stmt, 2)
             << setw(15) << (const char*)sqlite3_column_text(stmt, 3)
             << "$" << setw(9) << sqlite3_column_double(stmt, 4)
             << setw(10) << (sqlite3_column_type(stmt, 5) == SQLITE_NULL ? 0 : sqlite3_column_int(stmt, 5));
        saltoLinea;
    }

    sqlite3_finalize(stmt);
}


void ProductoService::listarLotes() {
    system("cls");

    sqlite3* db = Database::getDB();

    const char* sql =
    "SELECT p.codigo, p.nombre, p.marca, l.lote, l.fecha_caducidad, l.stock "
    "FROM lotes l JOIN productos p ON l.producto_id = p.id "
    "ORDER BY p.nombre, p.marca;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        cout << red << "Error al preparar la consulta: " << sqlite3_errmsg(db) << reset;
        return;
    }

    string actual = "";

    cout << white << "=== Lotes de productos ===" << reset;
    saltoLinea;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string codigo = (const char*)sqlite3_column_text(stmt, 0);
        string nombre = (const char*)sqlite3_column_text(stmt, 1);
        string marca = (const char*)sqlite3_column_text(stmt, 2);

        string clave = nombre + marca;

        if (clave != actual) {
            saltoLinea;
            cout << lightBlue << nombre << " - " << marca
                 << " (Cod: " << codigo << ")" << reset;
            saltoLinea;
            cout << gray;
            lineaDivision;
            cout << reset;
            saltoLinea;
            actual = clave;
        }        cout << "  Lote: " << (const char*)sqlite3_column_text(stmt, 3)
                      << " | Vence: " << (const char*)sqlite3_column_text(stmt, 4)
                      << " | Stock: " << sqlite3_column_int(stmt, 5);
                 saltoLinea;
             }

             sqlite3_finalize(stmt);
         }

void ProductoService::registrarInventario() {
    sqlite3* db = Database::getDB();
    Validaciones v;
    VentaService vs;
    int id;

    system("cls");

    // Pedir código de barras
    string codigo = v.leerCadenaTextNum("C\242digo de barras: ");

    const char* sqlCheck = "SELECT id, nombre, marca FROM productos WHERE codigo=? LIMIT 1;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sqlCheck, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, codigo.c_str(), -1, SQLITE_TRANSIENT);

    bool existe = false;
    string nombre, marca;

    // Verificar si el producto ya existe
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        existe = true;
        id = sqlite3_column_int(stmt, 0); // Obtener el ID del producto existente
        nombre = (const char*)sqlite3_column_text(stmt, 1);
        marca = (const char*)sqlite3_column_text(stmt, 2);
    }
    sqlite3_finalize(stmt);

    if (!existe) {
        // Si no existe, solicitar detalles del nuevo producto
        nombre = v.leerCadenaTexto("Nombre: ");
        marca = v.leerCadenaTexto("Marca: ");
        float compra = v.validarFloat("Precio de compra: ", "", "0", "100000");
        float venta = v.validarFloat("Precio de venta: ", "", "0", "100000");

        // Insertar en la tabla productos
        sqlite3_stmt* stmtInsProd;
        sqlite3_prepare_v2(db,
                           "INSERT INTO productos(codigo, nombre, marca, precio_compra, precio_venta) "
                           "VALUES(?,?,?,?,?);", -1, &stmtInsProd, NULL);

        sqlite3_bind_text(stmtInsProd, 1, codigo.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmtInsProd, 2, nombre.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmtInsProd, 3, marca.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmtInsProd, 4, compra);
        sqlite3_bind_double(stmtInsProd, 5, venta);

        if (sqlite3_step(stmtInsProd) != SQLITE_DONE) {
            cout<<red<<"Error al insertar producto: "<<sqlite3_errmsg(db)<<reset;
            saltoLinea;
            sqlite3_finalize(stmtInsProd);
            return; // Salir si hay error al insertar el producto
        }

        // Obtener el ID del producto recién insertado
        id = sqlite3_last_insert_rowid(db);
        sqlite3_finalize(stmtInsProd);

        cout<<lightGreen<<"Producto registrado exitosamente"<<reset;
        saltoLinea;

    } else {
        cout<<lightBlue<<"Producto existente: "<<nombre<<" ("<<marca<<")"<<reset;
        saltoLinea;
    }

    // Preguntar si desea ingresar un lote para este producto
    bool ingresarLote = v.leerCadenaBool("\250Desea ingresar un lote para este producto? (Y/N): ");

    if (ingresarLote) {
        string lote = v.leerCadenaTextNum("Lote: ");
        int stock = v.leerEntero("Stock: ");
        int stockMin = v.leerEntero("Stock m\241nimo: ");
        string fecha = vs.obtenerFechaActual();
        bool tieneCaducidad = v.leerCadenaBool("\250Tiene fecha de caducidad? (Y/N): ");
        string fechaCaducidad = "";

        if (tieneCaducidad) {
            fechaCaducidad=v.leerFechaFlexible("Ingrese la fecha de caducidad (aaaa-mm-dd): ");
        }

        // Insertar en la tabla lotes
        sqlite3_stmt* stmtInsLote;
        sqlite3_prepare_v2(db,
                           "INSERT INTO lotes(producto_id,lote, stock, stock_minimo, fecha_recibido, tiene_caducidad, fecha_caducidad) "
                           "VALUES(?,?,?,?,?,?,?);", -1, &stmtInsLote, NULL);
        sqlite3_bind_int(stmtInsLote, 1, id); // Usar el ID obtenido (ya sea nuevo o existente)
        sqlite3_bind_text(stmtInsLote, 2, lote.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmtInsLote, 3, stock);
        sqlite3_bind_int(stmtInsLote, 4, stockMin);
        sqlite3_bind_text(stmtInsLote, 5, fecha.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmtInsLote, 6, tieneCaducidad ? 1 : 0); // Convertir bool a int
        sqlite3_bind_text(stmtInsLote, 7, fechaCaducidad.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmtInsLote) != SQLITE_DONE) {
            cout << red << "Error al insertar lote: " << sqlite3_errmsg(db) << reset;
            saltoLinea;
        } else {
            cout << lightGreen << "Lote registrado exitosamente." << reset;
            saltoLinea;
        }

        sqlite3_finalize(stmtInsLote);
    } else {
        cout<<lightBlue<<"No se ingres\242 lote para el producto." << reset;
        saltoLinea;
    }
}

/*void ProductoService::registrarInventario() {
    sqlite3* db = Database::getDB();
    Validaciones v;
    VentaService vs;
    int id;

    system("cls");

    string codigo = v.leerCadenaTextNum("C\242digo de barras: ");

    const char* sqlCheck = "SELECT id, nombre, marca FROM productos WHERE codigo=? LIMIT 1;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sqlCheck, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, codigo.c_str(), -1, SQLITE_TRANSIENT);

    bool existe = false;
    string nombre, marca;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        existe = true;
        id = sqlite3_column_int(stmt, 0); // Obtener el ID del producto existente
        nombre = (const char*)sqlite3_column_text(stmt, 1);
        marca = (const char*)sqlite3_column_text(stmt, 2);
    }
    sqlite3_finalize(stmt);

    if (!existe) {
        nombre = v.leerCadenaTexto("Nombre: ");
        marca = v.leerCadenaTexto("Marca: ");
        float compra = v.validarFloat("Precio de compra ", "", "0", "100000");
        float venta = v.validarFloat("Precio de venta ", "", "0", "100000");

        // Insertar en la tabla productos
        sqlite3_stmt* stmtInsProd;
        sqlite3_prepare_v2(db,
                           "INSERT INTO productos(codigo, nombre, marca, precio_compra, precio_venta) "
                           "VALUES(?,?,?,?,?);", -1, &stmtInsProd, NULL);

        sqlite3_bind_text(stmtInsProd, 1, codigo.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmtInsProd, 2, nombre.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmtInsProd, 3, marca.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmtInsProd, 4, compra);
        sqlite3_bind_double(stmtInsProd, 5, venta);

        if (sqlite3_step(stmtInsProd) != SQLITE_DONE) {
            cout << red << "Error al insertar producto: " << sqlite3_errmsg(db) << reset;
            saltoLinea;
            sqlite3_finalize(stmtInsProd);
            return; // Salir si hay error al insertar el producto
        }

        // Obtener el ID del producto recién insertado
        id = sqlite3_last_insert_rowid(db);
        sqlite3_finalize(stmtInsProd);

    } else {
        cout << lightBlue << "Producto existente: " << nombre << " " << marca << reset;
        saltoLinea;
    }

    // Aquí ya tienes el ID correcto del producto (ya sea nuevo o existente)
    string lote = v.leerCadenaTextNum("Lote: ");
    int stock = v.leerEntero("Stock: ");
    int stockMin = v.leerEntero("Stock m\241nimo: ");
    string fecha = vs.obtenerFechaActual();
    bool tieneCaducidad = v.leerCadenaBool("Tiene fecha de caducidad? (Y/N): ");
    string fechaCaducidad = "";

    if (tieneCaducidad) {
        cout << "Ingrese fecha de caducidad (aaaa-mm-dd): ";
        getline(cin, fechaCaducidad);
    }

    // Insertar en la tabla lotes
    sqlite3_stmt* stmtInsLote;
    sqlite3_prepare_v2(db,
                       "INSERT INTO lotes(producto_id,lote, stock, stock_minimo, fecha_recibido, tiene_caducidad, fecha_caducidad) "
                       "VALUES(?,?,?,?,?,?,?);", -1, &stmtInsLote, NULL);

    sqlite3_bind_int(stmtInsLote, 1, id); // Usar el ID obtenido (ya sea nuevo o existente)
    sqlite3_bind_text(stmtInsLote, 2, lote.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmtInsLote, 3, stock);
    sqlite3_bind_int(stmtInsLote, 4, stockMin);
    sqlite3_bind_text(stmtInsLote, 5, fecha.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmtInsLote, 6, tieneCaducidad ? 1 : 0); // Convertir bool a int
    sqlite3_bind_text(stmtInsLote, 7, fechaCaducidad.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmtInsLote) != SQLITE_DONE) {
        cout << red << "Error al insertar lote: " << sqlite3_errmsg(db) << reset;
        saltoLinea;
    } else {
        cout << lightGreen << "Inventario actualizado" << reset;
        saltoLinea;
    }

    sqlite3_finalize(stmtInsLote);
}*/

void ProductoService::eliminarProducto() {

    sqlite3* db = Database::getDB();
    listarProductos();
    Validaciones v;

    saltoLinea;
    int id = v.leerCadenaEnteros("ID del producto a eliminar: ");

    stringstream ss;
    ss<<"DELETE FROM productos WHERE id="<<id<<";";

    char* error = 0;

    if (sqlite3_exec(db, ss.str().c_str(), 0, 0, &error) != SQLITE_OK) {
        cout << red << "Error eliminando producto."<<reset;
        saltoLinea;
        sqlite3_free(error);
    }else{
        cout<<lightGreen<<"Producto eliminado correctamente."<<reset;
        saltoLinea;
        cout<<gray<<"Sus lotes y ventas asociadas fueron eliminados automaticamente."<<reset;
        saltoLinea;
    }
}


bool ProductoService::existenProductos(){

    sqlite3* db = Database::getDB();
    const char* sql = "SELECT COUNT(*) FROM productos;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    int total=0;

    if(sqlite3_step(stmt) == SQLITE_ROW){
        total = sqlite3_column_int(stmt,0);
    }

    sqlite3_finalize(stmt);

    return total>0;
}

void ProductoService::cambiarPrecio(){
    sqlite3* db = Database::getDB();
    listarProductos();
    Validaciones v;
    int id;
    float compra;
    float venta;
    double precioCompra;
    double precioVenta;
    string min="0.0";
    string max="1000000.0";

    saltoLinea;
    id = v.leerCadenaEnteros("ID del producto: ");
    saltoLinea;
    compra=v.validarFloat("Introduzca el nuevo precio de ", "compra ", min, max);
    venta=v.validarFloat("Introduzca el nuevo precio de ", "venta ", min, max);

    //Convertir a double
    precioCompra=(double)compra;
    precioVenta=(double)venta;

    stringstream sqlStream;
    sqlStream << "UPDATE productos "
              << "SET precio_compra = " << precioCompra<<", "
              << "precio_venta = " << precioVenta
              << " WHERE id = " << id << ";";

    string sql = sqlStream.str();
    char* error = 0;

    guardarHistorialPrecio(id,compra,venta);

    if (sqlite3_exec(db, sql.c_str(), 0, 0, &error) != SQLITE_OK) {
        cout<<red<<"Error actualizando los precios."<<reset;
        saltoLinea;
        cout<<orange<<"Intente nuevamente."<<reset;
        saltoLinea;
        sqlite3_free(error);
    } else {
        cout<<lightGreen<<"Precios actualizados correctamente"<<reset;
        saltoLinea;
    }
}

void ProductoService::guardarHistorialPrecio(int producto_id,double compra,double venta){

    sqlite3* db = Database::getDB();

    const char* sql =
    "INSERT INTO historial_precios "
    "(producto_id,precio_compra,precio_venta,fecha) "
    "VALUES (?,?,?,date('now'));";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db,sql,-1,&stmt,0);

    sqlite3_bind_int(stmt,1,producto_id);
    sqlite3_bind_double(stmt,2,compra);
    sqlite3_bind_double(stmt,3,venta);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
}


//Funciones antiguas en desuso:
/*void ProductoService::agregarProducto(){

    sqlite3* db=Database::getDB();

    VentaService vs;
    Validaciones v;
    Consola c;

    system("cls");
    string nombre=v.leerCadenaTexto("Nombre del producto: ");
    string marca=v.leerCadenaTexto("Marca: ");
    string lote=v.leerCadenaTextNum("Lote: ");

    // verificar que el lote no exista
    string sqlCheck="SELECT id FROM productos WHERE lote=?";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db,sqlCheck.c_str(),-1,&stmt,0);
    sqlite3_bind_text(stmt,1,lote.c_str(),-1,SQLITE_TRANSIENT);

    if(sqlite3_step(stmt)==SQLITE_ROW){
        cout<<red<<"Error: el lote ya existe."<<reset;
        saltoLinea;
        sqlite3_finalize(stmt);
        c.pausa();
        return;
    }
    sqlite3_finalize(stmt);

    string fecha_recibido=vs.obtenerFechaActual();

    bool tieneCaducidad=v.leerCadenaBool("Tiene fecha de caducidad? (Y/N): ");

    string fechaCaducidad="";

    if(tieneCaducidad){

        cout<<"Ingrese fecha de caducidad (aaaa-mm-dd): ";
        getline(cin,fechaCaducidad);
    }
    string min="0.0";
    string max="1000000.0";

    float compra=v.validarFloat("Precio de ", "compra ",min,max);
    float venta=v.validarFloat("Precio de ", "venta ",min,max);

    //Convertir a double
    double precioCompra=(double)compra;
    double precioVenta=(double)venta;

    int stock=v.leerEntero("Stock inicial: ");
    int stockMin=v.leerEntero("Stock m\241nimo: ");

    const char* sqlInsert=
            "INSERT INTO productos(nombre,marca,lote,precio_compra,precio_venta,stock,stock_minimo,fecha_recibido,tiene_caducidad,fecha_caducidad)"
            "VALUES(?,?,?,?,?,?,?,?,?,?)";

    sqlite3_stmt* stmtInsert;

    sqlite3_prepare_v2(db,sqlInsert,-1,&stmtInsert,0);

    sqlite3_bind_text(stmtInsert,1,nombre.c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmtInsert,2,marca.c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmtInsert,3,lote.c_str(),-1,SQLITE_TRANSIENT);

    sqlite3_bind_double(stmtInsert,4,precioCompra);
    sqlite3_bind_double(stmtInsert,5,precioVenta);

    sqlite3_bind_int(stmtInsert,6,stock);
    sqlite3_bind_int(stmtInsert,7,stockMin);

    sqlite3_bind_text(stmtInsert,8,fecha_recibido.c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_int(stmtInsert,9,tieneCaducidad);

    sqlite3_bind_text(stmtInsert,10,fechaCaducidad.c_str(),-1,SQLITE_TRANSIENT);

    sqlite3_step(stmtInsert);
    sqlite3_finalize(stmtInsert);

    cout<<lightGreen<<"Producto agregado correctamente."<<reset;
    saltoLinea
}*/

/*void ProductoService::listar(){

    system("cls");

    sqlite3* db=Database::getDB();

    string sql="SELECT id,nombre,marca,lote,precio_venta,stock FROM productos;";

    sqlite3_stmt* stmt;

    int rc=sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,NULL);

    if(rc==SQLITE_OK){

        cout<<white<<"Lista de productos"<<reset;
        saltoLinea
        saltoLinea

        cout<<left
            <<setw(5)<<"ID"
            <<setw(20)<<"Nombre"
            <<setw(15)<<"Marca"
            <<setw(12)<<"Lote"
            <<setw(10)<<"Precio"
            <<setw(8)<<"Stock";
        saltoLinea

        lineaDivision
        saltoLinea

        while(sqlite3_step(stmt)==SQLITE_ROW){

            cout<<setw(5)<<sqlite3_column_int(stmt,0)
                <<setw(20)<<sqlite3_column_text(stmt,1)
                <<setw(15)<<sqlite3_column_text(stmt,2)
                <<setw(12)<<sqlite3_column_text(stmt,3)
                <<"$"<<setw(10)<<sqlite3_column_double(stmt,4)
                <<setw(8)<<sqlite3_column_int(stmt,5);

            saltoLinea
        }
    }

    sqlite3_finalize(stmt);
}*/

/*void ProductoService::reponerStock() {
    sqlite3* db = Database::getDB();
    listar();
    Validaciones v;

    saltoLinea;
    int id = v.leerCadenaEnteros("ID del producto: ");
    saltoLinea;
    int cantidad = v.leerCadenaEnteros("Cantidad recibida: ");

    // Usar stringstream para construir la consulta SQL
    stringstream sqlStream;
    sqlStream << "UPDATE productos "
              << "SET stock = stock + " << cantidad
              << " WHERE id = " << id << ";";

    string sql = sqlStream.str();
    char* error = 0;

    if (sqlite3_exec(db, sql.c_str(), 0, 0, &error) != SQLITE_OK) {
        cout << red << "Error actualizando stock." << reset;
        saltoLinea;
        cout << orange << "Intente nuevamente." << reset;
        saltoLinea;
        sqlite3_free(error);
    } else {
        cout << lightGreen << "Stock actualizado correctamente" << reset;
        saltoLinea;
    }
}*/

/*void ProductoService::registrarInventario(){

    sqlite3* db=Database::getDB();
    Validaciones v;
    Consola c;
    VentaService vs;

    system("cls");

    string codigo=v.leerCadenaTextNum("C\242digo de barras: ");

    const char* sqlCheck="SELECT nombre,marca FROM productos WHERE codigo=? LIMIT 1;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,sqlCheck,-1,&stmt,NULL);
    sqlite3_bind_text(stmt,1,codigo.c_str(),-1,SQLITE_TRANSIENT);

    bool existe=false;
    string nombre,marca;

    if(sqlite3_step(stmt)==SQLITE_ROW){
        existe=true;
        nombre=(const char*)sqlite3_column_text(stmt,0);
        marca=(const char*)sqlite3_column_text(stmt,1);
    }
    sqlite3_finalize(stmt);

    if(!existe){
        nombre=v.leerCadenaTexto("Nombre: ");
        marca=v.leerCadenaTexto("Marca: ");
    }else{
        cout<<lightBlue<<"Producto existente: "<<nombre<<" "<<marca<<reset;
        saltoLinea;
    }

    string lote=v.leerCadenaTextNum("Lote: ");

    float compra=v.validarFloat("Precio de compra ","","0","100000");
    float venta=v.validarFloat("Precio de venta ","","0","100000");

    int stock=v.leerEntero("Stock: ");
    int stockMin=v.leerEntero("Stock m\241nimo: ");

    string fecha=vs.obtenerFechaActual();

    bool tieneCaducidad=v.leerCadenaBool("Tiene fecha de caducidad? (Y/N): ");

    string fechaCaducidad="";

    if(tieneCaducidad){

        cout<<"Ingrese fecha de caducidad (aaaa-mm-dd): ";
        getline(cin,fechaCaducidad);
    }

    sqlite3_stmt* stmtIns;
    sqlite3_prepare_v2(db,
    "INSERT INTO productos(codigo,nombre,marca,lote,precio_compra,precio_venta,stock,stock_minimo,fecha_recibido,tiene_caducidad,fecha_caducidad)"
    "VALUES(?,?,?,?,?,?,?,?,?);",-1,&stmtIns,NULL);

    sqlite3_bind_text(stmtIns,1,codigo.c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmtIns,2,nombre.c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmtIns,3,marca.c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmtIns,4,lote.c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_double(stmtIns,5,compra);
    sqlite3_bind_double(stmtIns,6,venta);
    sqlite3_bind_int(stmtIns,7,stock);
    sqlite3_bind_int(stmtIns,8,stockMin);
    sqlite3_bind_text(stmtIns,9,fecha.c_str(),-1,SQLITE_TRANSIENT);

    sqlite3_step(stmtIns);
    sqlite3_finalize(stmtIns);

    cout<<lightGreen<<"Inventario actualizado"<<reset;
    saltoLinea;
}*/
