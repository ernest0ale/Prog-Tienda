#include <iostream>
#include <Data/database.h>
#include <Services/monedaService.h>

using namespace std;

double MonedaService::obtenerTasa(string nombre){
    sqlite3* db = Database::getDB();
    sqlite3_stmt* stmt;
    double tasa = 1;

    string sql = "SELECT tasa FROM monedas WHERE nombre = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) == SQLITE_ROW){
        tasa = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return tasa;
}

double MonedaService::convertir(double monto, string origen, string destino){
    double tasaOrigen = obtenerTasa(origen);
    double tasaDestino = obtenerTasa(destino);

    double enBase = monto * tasaOrigen; // a CUP
    return enBase / tasaDestino;
}

void MonedaService::listarMonedas(){
    sqlite3* db = Database::getDB();
    sqlite3_stmt* stmt;

    string sql = "SELECT nombre, tasa FROM monedas;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    cout<<"--- Monedas ---"<<endl;

    while(sqlite3_step(stmt) == SQLITE_ROW){
        string nombre = (char*)sqlite3_column_text(stmt, 0);
        double tasa = sqlite3_column_double(stmt, 1);

        cout<<nombre<<" -> "<<tasa<<endl;
    }

    sqlite3_finalize(stmt);
}

void MonedaService::actualizarTasa(string nombre, double nuevaTasa){
    sqlite3* db = Database::getDB();
    sqlite3_stmt* stmt;

    string sql = "UPDATE monedas SET tasa = ? WHERE nombre = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    sqlite3_bind_double(stmt, 1, nuevaTasa);
    sqlite3_bind_text(stmt, 2, nombre.c_str(), -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}
