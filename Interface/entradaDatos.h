#pragma once
#include <stdbool.h>
#include <string>

using namespace std;

class Validaciones {
public:
    static int leerEntero(string mensaje);
    static float leerFloat(string mensaje);
    static double leerDouble(string mensaje);
    static string leerTexto(string mensaje);
    static bool soloEspacios(string mensaje);
    static int leerCadenaOpcion(string mensaje);
    static string leerCadenaTexto(string mensaje);
    static string leerCadenaTextNum(string mensaje);
    static string leerCadenaDigitos(string mensaje);
    static int leerCadenaEnteros(string mensaje);
    static float leerCadenaFloat(string mensaje, string tipoPrecio, string min, string max);
    static float validarFloat(string mensaje, string tipoPrecio, string min, string max);
    static bool leerCadenaBool(string mensaje);
    static string leerFechaFlexible(string mensaje);
    static bool esNumeroSimple(const string& s);
};
