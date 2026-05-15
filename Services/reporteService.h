#pragma once

class ReporteService {
public:
    void ventasTotalesPorMes();
    void mostrarVentasPorYear(int year);
    void productosBajoStock();
    void productoMasVendido();
    void productoMenosVendido();
    void ingresosTotales();
    void productosMayorGanancia();
    void productosNuncaVendidos();
    void productosCercaCaducidad();
    void productosCaducados();
    void historialPrecios();
    void perdidasPorCaducidad();

    //Funciones para la presentación
    int contarProductosPorCaducar();
    int contarProductosCaducados();
};
