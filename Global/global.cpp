#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Global/colors.h>

void pausa(){
    system("pause");
    system("cls");
}

//Funciones de lectura de datos
int leerCadenaOpcion(){
    char entrada[5];
    int opcion = -1;
    int i;
    bool esNumeroValido = true;

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

void leerCadenaTexto(char cadena[], int max){
    bool cadenaValida=false;
    int i;
    bool tieneCaracter=false;

    do {
        fgets(cadena, max, stdin);
        cadena[strcspn(cadena, "\n")] = '\0';

        // Verificar si tiene al menos un carácter que no sea espacio
        tieneCaracter=false;
        for(i=0; cadena[i] != '\0'; i++){
            if(isalpha(cadena[i])){
                tieneCaracter=true;
            }
        }

        if(!tieneCaracter){
            printf(orange "\41Error! Debe ingresar alg\243n texto v\240lido (no solo espacios).\n" reset);
            pausa();
        }else{
            cadenaValida=true;
        }

    } while(!cadenaValida);
}

/*float leerFlotante(float min, float max){
    char entrada[10];
    float valor=-1.0;
    int i;
    int cantPuntos = 0;
    bool formatoValido = true;
    bool tieneDigito = false;
    bool tieneEspacio = false;

    printf("%s ($%.2f-$%.2f): $", mensaje, min, max);
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = '\0';

    // Procesar cadena
    for(i = 0; entrada[i] != '\0'; i++) {
        if(entrada[i] != ' ') {
            tieneEspacio=true;
        }

        if(formatoValido){
            if(isdigit(entrada[i])) {
                tieneDigito = true;
            }else if(entrada[i] == '.') {
                cantPuntos++;
                if(cantPuntos>1){
                    formatoValido = false;
                }
            }else if(!(i == 0 && entrada[i] == '-')) {
                formatoValido = false;
            }
        }
    }

    if(entrada[0] != '\0'){
        if(!tieneEspacio){
            valor=-1.0;
        }else if(!formatoValido || !tieneDigito){
            valor=-2.0f;
        }else{
            valor=atof(entrada);
        }
    }

    return valor;
}*/
