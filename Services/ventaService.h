#pragma once
#include <string>
#include <vector>

using namespace std;

struct LoteVenta {
    int id;
    string lote;
    int stock;
};

struct ItemVenta{
    int productoId;
    int cantidad;
};

class VentaService {
private:
    vector<ItemVenta> carrito;
public:
    void iniciarVenta();
    void agregarProducto(int productoId, int cantidad);
    double obtenerPrecioProducto(int productoId);
    double calcularTotal();
    void finalizarVenta();

    //Funciones beta1
    void registrarVenta();
    bool existenVentas();
    void listarVentas();
    string obtenerFechaActual();
    void verificarStockCritico(int producto_id);
    vector<LoteVenta> obtenerLotesFIFO(int id);
};


