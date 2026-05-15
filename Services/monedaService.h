#pragma once
#include <string>

using namespace std;

class MonedaService {
public:
    double obtenerTasa(string nombre);
    double convertir(double monto, string monedaOrigen, string monedaDestino);
    void listarMonedas();
    void actualizarTasa(string nombre, double nuevaTasa);
};
