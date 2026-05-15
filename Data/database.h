#pragma once
#include <Utils/SQLite/sqlite3.h>

class Database {
public:
    static bool inicializar();
    static sqlite3* getDB();

private:
    static sqlite3* db;
};
