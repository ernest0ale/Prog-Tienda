#pragma once
#include <string>
#include <vector>
using namespace std;

struct LoteVenta {
    int id;
    string lote;
    int stock;
};

class VentaService {
public:
    void registrarVenta();
    bool existenVentas();
    void listarVentas();
    string obtenerFechaActual();
    void verificarStockCritico(int producto_id);
    vector<LoteVenta> obtenerLotesFIFO(int id);
};


