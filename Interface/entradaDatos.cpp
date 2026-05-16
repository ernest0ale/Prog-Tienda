#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdbool.h>
#include <string>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <Global/definition.h>
#include <Global/colors.h>
#include <Interface/entradaDatos.h>
#include <Interface/consola.h>

using namespace std;

bool Validaciones::soloEspacios(string mensaje){
    int i;
    bool valido=true;

    for(i=0;i<mensaje.length() && valido;i++){

        if(mensaje[i] != ' ')
            valido=false;
    }

    return valido;
}

int Validaciones::leerEntero(string mensaje) {

    string entrada;
    Consola c;
    int valor=-1;
    bool valido=false;
    int i;

    do {
        cout<<mensaje;
        getline(cin, entrada);
        bool esNumero=true;

        if(entrada.length()==0){
            soloEspacios(entrada);{
                esNumero=false;
            }
        }
        for (i=0; i<entrada.length() && esNumero; i++) {
            if (!isdigit(entrada[i])){
                esNumero=false;
            }
        }

        if(esNumero){
            valor=atoi(entrada.c_str());
            valido=true;

        }else{
            cout<<orange<<"Entrada inv\240lida."<<reset;
            saltoLinea
            cout<<yellow<<"Intente nuevamente."<<reset;
            saltoLinea
        }

    }while(!valido);

    return valor;
}

float Validaciones::leerFloat(string mensaje) {

    string entrada;
    Consola c;
    double valor=0;
    bool valido=false;

    do {
        cout<<mensaje;
        getline(cin, entrada);
        bool formatoValido=true;
        bool tieneDigito=false;
        int puntos=0;
        int i;

        if(entrada.length()==0){
            soloEspacios(entrada);{
                formatoValido=false;
            }
        }

        for (i=0; i<entrada.length() && formatoValido; i++) {
            if(isdigit(entrada[i])){
                tieneDigito=true;
            }else if(entrada[i] == '.'){
                puntos++;
                if(puntos>1){
                    formatoValido=false;
                }
            }else{
                formatoValido=false;
            }
        }

        if(formatoValido && tieneDigito){
            valor=atof(entrada.c_str());
            valido=true;

        }else{
            cout<<orange<<"Entrada inv\240lida."<<reset;
            saltoLinea
            cout<<yellow<<"Intente nuevamente."<<reset;
            saltoLinea
        }

    }while(!valido);

    return valor;
}

double Validaciones::leerDouble(string mensaje) {

    string entrada;
    Consola c;
    double valor=0;
    bool valido=false;

    do {
        cout<<mensaje;
        getline(cin, entrada);
        bool formatoValido=true;
        bool tieneDigito=false;
        int puntos=0;
        int i;

        if(entrada.length()==0){
            soloEspacios(entrada);{
                formatoValido=false;
            }
        }

        for (i=0; i<entrada.length() && formatoValido; i++) {
            if(isdigit(entrada[i])){
                tieneDigito = true;
            }else if(entrada[i] == '.'){
                puntos++;
                if(puntos > 1){
                    formatoValido=false;
                }
            }else{
                formatoValido=false;
            }
        }

        if(formatoValido && tieneDigito){
            valor=atof(entrada.c_str());
            valido=true;
        }else{
            cout<<orange<<"Entrada inv\240lida."<<reset;
            saltoLinea
            cout<<yellow<<"Intente nuevamente."<<reset;
            saltoLinea
        }

    }while(!valido);

    return valor;
}

string Validaciones::leerTexto(string mensaje) {

    string texto;
    Consola c;
    bool valido=false;
    int i;

    do {
        cout<<mensaje;
        getline(cin, texto);
        bool tieneLetra=false;

        for(i=0; i<texto.length() && !tieneLetra; i++) {
            if(isalpha(texto[i])) {
                tieneLetra=true;
            }
        }

        if(tieneLetra){
            valido=true;
        }else{
            cout<<orange<<"Debe ingresar texto v\240lido."<<reset;
            saltoLinea
            cout<<yellow<<"Intente nuevamente."<<reset;
            saltoLinea
        }


    }while(!valido);

    return texto;
}


//Funciones de lectura de datos
int Validaciones::leerCadenaOpcion(string mensaje){
    char entrada[5];
    int opcion = -1;
    int i;
    bool esNumeroValido = true;

    cout<<mensaje;
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = '\0';

    // Validar que todos los caracteres sean dígitos y no haya punto
    esNumeroValido = true;
    for(i=0; entrada[i] != '\0' && esNumeroValido; i++){
        if(!isdigit(entrada[i])) {
            esNumeroValido = false;
        }
    }

    // También validar que no esté vacío
    if(entrada[0] != '\0' && esNumeroValido){
        opcion = atoi(entrada);
    }else{
        opcion = -1; // Valor inválido
    }

    return opcion;
}

int Validaciones::leerCadenaEnteros(string mensaje){
    char entrada[100];
    int opcion = -1;
    int i;
    bool esNumeroValido = true;

    cout<<mensaje;
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = '\0';

    // Validar que todos los caracteres sean dígitos y no haya punto
    esNumeroValido = true;
    for(i=0; entrada[i] != '\0' && esNumeroValido; i++){
        if(!isdigit(entrada[i])) {
            esNumeroValido = false;
        }
    }

    // También validar que no esté vacío
    if(entrada[0] != '\0' && esNumeroValido){
        opcion = atoi(entrada);
    }else{
        opcion = -1; // Valor inválido
    }

    return opcion;
}

bool Validaciones::leerCadenaBool(string mensaje){

    char entrada[5];
    bool entradaValida=false;
    bool opcionYN;

    do{
        cout<<mensaje;
        fgets(entrada,sizeof(entrada),stdin);
        entrada[strcspn(entrada,"\n")]='\0';

        if(strcmp(entrada,"Y")==0 || strcmp(entrada,"y")==0){
            opcionYN=true;
            entradaValida=true;

        }else if(strcmp(entrada,"N")==0 || strcmp(entrada,"n")==0){
                opcionYN=false;
                entradaValida=true;

        }else{
            cout<<orange<<"Entrada inv\240lida."<<reset;
            saltoLinea
            cout<<yellow<<"Introduzca Y o N."<<reset;
            saltoLinea;
        }

    }while(!entradaValida);

    return opcionYN;
}

string Validaciones::leerFechaFlexible(string mensaje){
    bool valida=false;
    string entrada;
    char buffer[50];
    Consola c;

    do{
        cout<<mensaje;
        fgets(buffer,sizeof(buffer),stdin);
        buffer[strcspn(buffer,"\n")]='\0';
        entrada=string(buffer);

        int len=entrada.length();

        // FORMATO 1: aaaammdd
        if(len==8){
            bool soloNumeros=true;
            int i;

            for(i=0; i<len && soloNumeros; i++){
                if(!isdigit(entrada[i])){
                    soloNumeros=false;
                }
            }

            if(soloNumeros){
                string year = entrada.substr(0,4);
                string mes  = entrada.substr(4,2);
                string dia  = entrada.substr(6,2);

                int mesInt = atoi(mes.c_str());
                int diaInt = atoi(dia.c_str());

                if(mesInt>=1 && mesInt<=12 && diaInt>=1 && diaInt<=31){
                    entrada = year + "-" + mes + "-" + dia;
                    valida=true;
                }
            }
        }

        // FORMATO 2: aaaa-mm-dd
        else if(len==10){
            bool formatoCorrecto=true;

            if(entrada[4]!='-' || entrada[7]!='-')
                formatoCorrecto=false;

            int i;
            bool detenerBucle=false;
            for(i=0;i<len && !detenerBucle;i++){
                if(i==4 || i==7) continue;
                if(!isdigit(entrada[i])){
                    formatoCorrecto=false;
                    detenerBucle=true;
                }
            }

            if(formatoCorrecto){
                string mes  = entrada.substr(5,2);
                string dia  = entrada.substr(8,2);

                int mesInt = atoi(mes.c_str());
                int diaInt = atoi(dia.c_str());

                if(mesInt>=1 && mesInt<=12 && diaInt>=1 && diaInt<=31){
                    valida=true;
                }
            }
        }

        if(!valida){
            cout<<orange<<"Formato de fecha inv\240lido."<<reset; saltoLinea
            cout<<"Use: aaaa-mm-dd o aaaammdd"; saltoLinea
            c.pausa();
        }

    }while(!valida);

    return entrada;
}

string Validaciones::leerCadenaTexto(string mensaje){
    bool cadenaValida=false;
    int i;
    bool tieneCaracter=false;
    string texto;
    char cadena[50];
    Consola c;

    do {
        cout<<mensaje;
        fgets(cadena, sizeof(cadena), stdin);
        cadena[strcspn(cadena, "\n")] = '\0';

        // Verificar si tiene al menos un carácter que no sea espacio
        for(i=0; cadena[i] != '\0'; i++){
            if(isalpha(cadena[i])){
                tieneCaracter=true;
            }
        }

        if(!tieneCaracter){
            cout<<orange<<"\41Error! Debe ingresar alg\243n texto v\240lido (no solo espacios)."<<reset;
            saltoLinea
            c.pausa();
        }else{
            cadenaValida=true;
            texto=string(cadena);
        }

    } while(!cadenaValida);

    return texto;
}

string Validaciones::leerCadenaTextNum(string mensaje){
    bool cadenaValida=false;
    int i;
    bool entradaValida=false;
    string texto;
    char cadena[10];
    Consola c;

    do {
        cout<<mensaje;
        fgets(cadena, sizeof(cadena), stdin);
        cadena[strcspn(cadena, "\n")] = '\0';

        // Verificar si tiene al menos un carácter que no sea espacio
        for(i=0; cadena[i] != '\0'; i++){
            if(isalpha(cadena[i]) || isdigit(cadena[i])){
                entradaValida=true;
            }
        }

        if(!entradaValida){
            cout<<orange<<"\41Error! Debe ingresar alg\243n texto v\240lido (no solo espacios)."<<reset;
            saltoLinea
            c.pausa();
        }else{
            cadenaValida=true;
            texto=string(cadena);
        }

    } while(!cadenaValida);

    return texto;
}

string Validaciones::leerCadenaDigitos(string mensaje){
    bool cadenaValida=false;
    int i;
    bool entradaValida=false;
    string texto;
    char cadena[50];
    Consola c;

    do {
        cout<<mensaje;
        fgets(cadena, sizeof(cadena), stdin);
        cadena[strcspn(cadena, "\n")] = '\0';

        // Verificar si tiene al menos un carácter que no sea espacio
        for(i=0; cadena[i] != '\0'; i++){
            if(isdigit(cadena[i])){
                entradaValida=true;
            }
        }

        if(!entradaValida){
            cout<<orange<<"\41Error! Debe ingresar alg\243n texto v\240lido (no solo espacios)."<<reset;
            saltoLinea
            c.pausa();
        }else{
            cadenaValida=true;
            texto=string(cadena);
        }

    } while(!cadenaValida);

    return texto;
}

float Validaciones::leerCadenaFloat(string mensaje, string min, string max){
    char entrada[10];
    float valor=-1.0;
    int i;
    int cantPuntos = 0;
    bool formatoValido = true;
    bool tieneDigito = false;
    bool tieneEspacio = false;
    bool recibioEntrada=false;

    cout<<mensaje<<"("<<min<<"-"<<max<<")"<<": ";
    do{
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        // Procesar cadena

        if(strcmp(entrada,monedaMin)!=0 && strcmp(entrada,monedaMax)!=0){
            for(i = 0; entrada[i] != '\0'; i++) {
                if(entrada[i] != ' ') {
                    tieneEspacio=true;
                }

                if(formatoValido){
                    if(isdigit(entrada[i])) {
                        tieneDigito = true;
                        recibioEntrada=true;
                    }else if(entrada[i] == '.') {
                        cantPuntos++;
                        if(cantPuntos>1){
                            formatoValido = false;
                            recibioEntrada=true;
                        }
                    }else if(!(i == 0 && entrada[i] == '-')) {
                        formatoValido = false;
                        recibioEntrada=true;
                    }
                }
            }
        }

        if(entrada[0] != '\0' && recibioEntrada){
            if(!tieneEspacio){
                valor=-1.0;
            }else if(!formatoValido || !tieneDigito){
                valor=-2.0f;
            }else{
                valor=atof(entrada);
            }
        }
    }while(!recibioEntrada);

    return valor;
}

float Validaciones::validarFloat(string mensaje, string min, string max){
    float valor;
    bool valorValido = false;
    Validaciones v;
    Consola c;

    do{
        valor=v.leerCadenaFloat(mensaje,min,max);
        if(valor==-1.0){
            cout<<red<<"\41Error! Formato inv\240lido."<<reset; saltoLinea
            cout<<orange<<"Los datos no pueden contener espacios."<<reset; saltoLinea
            cout<<yellow<<"Intente nuevamente."<<reset; saltoLinea
            c.pausa();
        }else if(valor==-2.0){
            cout<<red<<"\41Error! Formato inv\240lido."<<reset; saltoLinea
            cout<<orange<<"Los datos no pueden contener letras, ni m\240s de un punto decimal."<<reset; saltoLinea
            cout<<yellow<<"Intente nuevamente."<<reset; saltoLinea
            c.pausa();
        }else if(valor<=0.0 || valor>1000000.0){
            cout<<red<<"\41Error! Formato inv\240lido."<<reset; saltoLinea
            cout<<orange<<"Debe introducir un valor entre"<<min<<" y "<<max<<"."<<reset;
            cout<<yellow<<"Intente nuevamente."<<reset; saltoLinea
            c.pausa();
        }else{
            valorValido = true;
        }
        fflush(stdin);

    } while(!valorValido);

    return valor;
}

bool Validaciones::esNumeroSimple(const string& s){
    bool resultado=true;
    size_t i;
    for(i=0; i<s.length() && resultado; i++){
        if(!isdigit(s[i])){
            resultado=false;
        }
    }
    return resultado;
}

string Validaciones::leerCadenaMoneda(string mensaje){

    char entrada[4];
    bool entradaValida=false;
    bool esTexto=true;
    string moneda;
    int i;


    do{
        cout<<mensaje;
        fgets(entrada,sizeof(entrada),stdin);
        entrada[strcspn(entrada,"\n")]='\0';

        for(i=0; entrada[i] != '\0' && esTexto; i++){
            if(isalpha(entrada[i])){
                entrada[i]=toupper(entrada[i]);
            }else{
                esTexto=false;
            }
        }

        if((strcmp(entrada,"CUP")==0) || (strcmp(entrada,"USD")==0) ||
                (strcmp(entrada,"EUR")==0)){
            moneda=string(entrada);
            entradaValida=true;
        }else{
            cout<<orange<<"Entrada inv\240lida."<<reset;
            saltoLinea
            cout<<yellow<<"Introduzca una moneda correcta (CUP/USD/EUR)"<<reset;
            saltoLinea;
        }


    }while(!entradaValida);

    return moneda;
}

string Validaciones::leerMonedaExtranjera(string mensaje){

    char entrada[4];
    bool entradaValida=false;
    bool esTexto=true;
    string moneda;
    Consola c;
    int i;

    do{
        cout<<mensaje;
        fgets(entrada,sizeof(entrada),stdin);
        entrada[strcspn(entrada,"\n")]='\0';

        for(i=0; entrada[i] != '\0' && esTexto; i++){
            if(isalpha(entrada[i])){
                entrada[i]=toupper(entrada[i]);
            }else{
                esTexto=false;
            }
        }

        if((strcmp(entrada,"USD")==0) || (strcmp(entrada,"EUR")==0)){
            moneda=string(entrada);
            entradaValida=true;
        }else{
            cout<<orange<<"Entrada inv\240lida."<<reset;
            saltoLinea
            cout<<yellow<<"Introduzca una moneda correcta (USD/EUR)"<<reset;
            saltoLinea;
            c.pausa();
        }

    }while(!entradaValida);

    return moneda;
}
