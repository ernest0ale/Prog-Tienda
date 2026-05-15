#pragma once
#include <string>
using namespace std;

struct Producto {
    int id;
    string codigo;
    string nombre;
    string marca;
    double precio_compra;
    double precio_venta;
};

struct Lote {
    int id;
    int producto_id;
    string lote;
    int stock;
    int stock_minimo;
    string fecha_recibido;
    bool tiene_caducidad;
    string fecha_caducidad;
};
