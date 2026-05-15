#include <Data/database.h>

using namespace std;

sqlite3* Database::db = 0;

bool Database::inicializar() {

    int rc = sqlite3_open("tienda.db", &db);
    if(rc != SQLITE_OK) return false;

    // Activar claves foráneas correctamente
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);

    const char* sqlProductos =
        "CREATE TABLE IF NOT EXISTS productos ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "codigo TEXT UNIQUE NOT NULL,"
        "nombre TEXT NOT NULL,"
        "marca TEXT NOT NULL,"
        "precio_compra REAL NOT NULL,"
        "precio_venta REAL NOT NULL"
        ");";

    sqlite3_exec(db, sqlProductos, 0, 0, 0);

    const char* sqlLotes =
        "CREATE TABLE IF NOT EXISTS lotes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "producto_id INTEGER NOT NULL,"
        "lote TEXT NOT NULL,"
        "stock INTEGER NOT NULL,"
        "stock_minimo INTEGER NOT NULL,"
        "fecha_recibido TEXT,"
        "tiene_caducidad INTEGER,"
        "fecha_caducidad TEXT,"
        "FOREIGN KEY(producto_id) REFERENCES productos(id) ON DELETE CASCADE"
        ");";

    sqlite3_exec(db, sqlLotes, 0, 0, 0);

    const char* sqlVentas =
        "CREATE TABLE IF NOT EXISTS ventas ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "lote_id INTEGER NOT NULL,"
        "cantidad INTEGER NOT NULL,"
        "fecha TEXT,"
        "FOREIGN KEY(lote_id) REFERENCES lotes(id) ON DELETE CASCADE"
        ");";

    sqlite3_exec(db, sqlVentas, 0, 0, 0);

    const char* sqlHistorial =
        "CREATE TABLE IF NOT EXISTS historial_precios ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "producto_id INTEGER,"
        "precio_compra REAL,"
        "precio_venta REAL,"
        "fecha TEXT,"
        "FOREIGN KEY(producto_id) REFERENCES productos(id) ON DELETE CASCADE"
        ");";

    sqlite3_exec(db, sqlHistorial, 0, 0, 0);

    return true;
}

sqlite3* Database::getDB() {
    return db;
}
