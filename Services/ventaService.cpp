#include <iostream>
#include <stdbool.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <Data/database.h>
#include <Global/colors.h>
#include <Global/definition.h>
#include <Interface/entradaDatos.h>
#include <Interface/consola.h>
#include <Services/productoService.h>
#include <Services/ventaService.h>
#include <Services/monedaService.h>

using namespace std;

//Funciones nuevas
void VentaService::iniciarVenta(){

    carrito.clear();
    Validaciones v;
    MonedaService ms;
    ProductoService ps;

    bool continuar = true;
    system("cls");
    ps.listarProductos();
    saltoLinea
    do{
        int id = v.leerCadenaEnteros("ID del producto: ");
        int cantidad = v.leerCadenaEnteros("Cantidad: ");

        agregarProducto(id, cantidad);
        continuar = v.leerCadenaBool("\250Agregar otro producto? (Y/N): ");
    }while(continuar);

    double total = calcularTotal();

    cout<<"Total en CUP: "<<total;
    saltoLinea

    string moneda;
    moneda=v.leerCadenaMoneda("Moneda de pago (CUP/USD/EUR): ");

    double totalConvertido = ms.convertir(total, "CUP", moneda);

    cout<<"Total en "<<moneda<<": "<<totalConvertido; saltoLinea

    float montoPagar=v.validarFloat("Dinero recibido: ",monedaMin,monedaMax);
    double pago =(double)montoPagar;

    if(pago < totalConvertido){
        cout<<yellow<<"Pago insuficiente"<<reset;
        saltoLinea
    }else{
        cout<<"Cambio: "<<(pago - totalConvertido);
        saltoLinea
        finalizarVenta();
    }
}

void VentaService::agregarProducto(int productoId, int cantidad){
    ItemVenta item;
    item.productoId = productoId;
    item.cantidad = cantidad;

    carrito.push_back(item);
}

double VentaService::obtenerPrecioProducto(int productoId){
    sqlite3* db = Database::getDB();
    sqlite3_stmt* stmt;

    double precio = 0;

    string sql = "SELECT precio_venta FROM productos WHERE id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, productoId);

    if(sqlite3_step(stmt) == SQLITE_ROW){
        precio = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return precio;
}

double VentaService::calcularTotal(){
    double total = 0;
    size_t i;
    double precio;

    for(i=0; i<carrito.size(); ++i){
        const ItemVenta& item=carrito[i];
        precio = obtenerPrecioProducto(item.productoId);
        total += precio * item.cantidad;
    }

    return total;
}

void VentaService::finalizarVenta(){

    sqlite3* db = Database::getDB();
    sqlite3_stmt* stmt;
    size_t i;

    string sql = "INSERT INTO ventas (lote_id, cantidad, fecha) VALUES (?, ?, datetime('now'));";

    for(i=0; i<carrito.size(); ++i){

        const ItemVenta& item=carrito[i];
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

        // aquí asumimos lote_id = productoId (puedes mejorar luego)
        sqlite3_bind_int(stmt, 1, item.productoId);
        sqlite3_bind_int(stmt, 2, item.cantidad);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    cout<<lightGreen<<"Venta registrada correctamente"<<reset; saltoLinea
}



//Funciones de Beta 1
void VentaService::registrarVenta() {
    sqlite3* db = Database::getDB();
    Validaciones v;
    ProductoService ps;
    string entrada;

    system("cls");
    cout << "Registrar venta"; saltoLinea; saltoLinea;

    ps.listarProductos();
    // Pedir el ID del producto
    int idProducto = v.leerCadenaEnteros("ID del producto: ");

    // Verificar si el producto existe
    const char* sqlCheck = "SELECT codigo FROM productos WHERE id=? LIMIT 1;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sqlCheck, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, idProducto);

    string codigo;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Obtener el código del producto existente
        codigo = (const char*)sqlite3_column_text(stmt, 0);
    } else {
        cout << red << "Producto no encontrado con ID: " << idProducto << reset;
        saltoLinea
        sqlite3_finalize(stmt);
        return;
    }
    sqlite3_finalize(stmt);

    // Obtener lotes disponibles para el código del producto
    vector<LoteVenta> lotes = obtenerLotesFIFO(idProducto);

    if (lotes.empty()) {
        cout << red << "Sin stock disponible para el producto: " << codigo << reset;
        saltoLinea;
        return;
    }

    // Calcular el stock total disponible
    int stockTotal = 0;
    for (size_t i = 0; i < lotes.size(); i++) {
        stockTotal += lotes[i].stock;
    }

    // Pedir cantidad a vender
    int cantidad = v.leerCadenaEnteros("Cantidad: ");

    // Validar si hay suficiente stock
    if (cantidad > stockTotal) {
        cout << red << "Stock insuficiente. Disponible: " << stockTotal << reset;
        saltoLinea;
        return;
    }

    int restante = cantidad;

    // Obtener la fecha actual
    string fecha = obtenerFechaActual();

    // Iniciar transacción
    sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, 0);

    for (size_t i = 0; i < lotes.size(); i++) {
        LoteVenta& l = lotes[i];

        if (restante <= 0) break;

        int usar = (restante < l.stock) ? restante : l.stock;
        int nuevoStock = l.stock - usar;

        // Insertar en la tabla ventas
        sqlite3_stmt* stmtVenta;
        sqlite3_prepare_v2(db,
            "INSERT INTO ventas(lote_id,cantidad,fecha) VALUES(?,?,?);",
            -1, &stmtVenta, NULL);

        sqlite3_bind_int(stmtVenta, 1, l.id);
        sqlite3_bind_int(stmtVenta, 2, usar);
        sqlite3_bind_text(stmtVenta, 3, fecha.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmtVenta) != SQLITE_DONE) {
            cout << red << "Error al registrar la venta: " << sqlite3_errmsg(db) << reset;
            saltoLinea
            sqlite3_finalize(stmtVenta);
            break; // Salir si hay error al insertar la venta
        }

        sqlite3_finalize(stmtVenta);

        // Actualizar stock del lote
        stringstream ss;
        ss << "UPDATE lotes SET stock=" << nuevoStock << " WHERE id=" << l.id;
        sqlite3_exec(db, ss.str().c_str(), 0, 0, 0);

        restante -= usar;
    }

    // Confirmar transacción
    sqlite3_exec(db, "COMMIT;", 0, 0, 0);

    cout << lightGreen << "Venta registrada correctamente" << reset;
    saltoLinea;
}

vector<LoteVenta> VentaService::obtenerLotesFIFO(int id) {
    sqlite3* db = Database::getDB();

    const char* sql =
    "SELECT id, lote, stock FROM lotes "
    "WHERE producto_id=? AND stock > 0 "
    "ORDER BY fecha_recibido ASC;"; // Asegúrate de que esta columna exista en tu tabla

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);

    vector<LoteVenta> lotes;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        LoteVenta l;
        l.id = sqlite3_column_int(stmt, 0);
        l.lote = (const char*)sqlite3_column_text(stmt, 1);
        l.stock = sqlite3_column_int(stmt, 2);
        lotes.push_back(l);
    }

    sqlite3_finalize(stmt);
    return lotes;
}


bool VentaService::existenVentas(){

    sqlite3* db=Database::getDB();
    const char* sql="SELECT COUNT(*) FROM ventas;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
    int total=0;

    if(sqlite3_step(stmt)==SQLITE_ROW){
        total=sqlite3_column_int(stmt,0);
    }

    sqlite3_finalize(stmt);

    return total>0;
}

void VentaService::listarVentas() {
    system("cls");

    sqlite3* db = Database::getDB();

    const char* sql =
    "SELECT v.id, v.fecha, p.codigo, p.nombre, p.marca, l.lote, v.cantidad "
    "FROM ventas v "
    "JOIN lotes l ON v.lote_id = l.id "
    "JOIN productos p ON l.producto_id = p.id "
    "ORDER BY v.fecha DESC;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    bool encontrado = false;

    cout << "Listado de ventas";
    saltoLinea
    saltoLinea

    cout << left
        << setw(5) << "ID"
        << setw(20) << "Fecha"
        << setw(10) << "C\242digo"
        << setw(20) << "Producto"
        << setw(15) << "Marca"
        << setw(12) << "Lote"
        << setw(10) << "Cantidad";
    saltoLinea

    lineaDivision
    saltoLinea

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        encontrado = true;

        cout << setw(5) << sqlite3_column_int(stmt, 0)
            << setw(20) << (const char*)sqlite3_column_text(stmt, 1)
            << setw(10) << (const char*)sqlite3_column_text(stmt, 2) // Código del producto
            << setw(20) << (const char*)sqlite3_column_text(stmt, 3)
            << setw(15) << (const char*)sqlite3_column_text(stmt, 4)
            << setw(12) << (const char*)sqlite3_column_text(stmt, 5)
            << setw(10) << sqlite3_column_int(stmt, 6);

        saltoLinea
    }

    if (!encontrado) {
        saltoLinea
        cout << yellow << "No hay ventas registradas." << reset;
        saltoLinea
    }

    sqlite3_finalize(stmt);
}



string VentaService::obtenerFechaActual(){

    time_t ahora=time(0);
    tm *tiempo=localtime(&ahora);

    char fecha[11];

    strftime(fecha,sizeof(fecha),"%Y-%m-%d",tiempo);

    return string(fecha);
}

//Funciones auxiliares
/*int VentaService::obtenerLoteMasAntiguo(string nombre,string marca){

    sqlite3* db = Database::getDB();

    const char* sql =
    "SELECT id "
    "FROM productos "
    "WHERE LOWER(nombre)=LOWER(?) "
    "AND LOWER(marca)=LOWER(?) "
    "AND stock>0 "
    "ORDER BY id ASC "
    "LIMIT 1;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,sql,-1,&stmt,0);

    sqlite3_bind_text(stmt,1,nombre.c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,2,marca.c_str(),-1,SQLITE_TRANSIENT);

    int id=-1;

    if(sqlite3_step(stmt)==SQLITE_ROW)
        id=sqlite3_column_int(stmt,0);

    sqlite3_finalize(stmt);

    return id;
}*/

void VentaService::verificarStockCritico(int producto_id){

    sqlite3* db = Database::getDB();

    const char* sql =
    "SELECT IFNULL(SUM(stock),0), IFNULL(SUM(stock_minimo),0) "
    "FROM lotes WHERE producto_id=?;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db,sql,-1,&stmt,0);

    sqlite3_bind_int(stmt,1,producto_id);

    if(sqlite3_step(stmt)==SQLITE_ROW){

        int stock = sqlite3_column_int(stmt,0);
        int minimo = sqlite3_column_int(stmt,1);

        if(stock<=minimo){
            cout<<red<<"Producto en stock cr\241tico"<<reset;
            saltoLinea;
        }
    }

    sqlite3_finalize(stmt);
}
