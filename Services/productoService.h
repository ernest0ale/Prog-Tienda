#pragma once
#include <stdbool.h>
#include <vector>

class ProductoService {
public:
    void registrarInventario();
    void listarProductos();
    void listarLotes();
    void eliminarProducto();
    vector<Producto> obtenerProductos();
    bool existenProductos();
    void cambiarPrecio();
    void guardarHistorialPrecio(int producto_id,double compra,double venta);
};
