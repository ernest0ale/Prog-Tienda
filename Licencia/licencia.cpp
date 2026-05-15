#include <iostream>
#include <stdbool.h>
#include <string>
#include <fstream>
#include <cstring>
#include <windows.h>
#include <shellapi.h>
#include <Global/definition.h>
#include <Global/colors.h>
#include <Licencia/licencia.h>
#include <Services/ventaService.h>

void ejecutarAutodestruccion() {
    char szExePath[MAX_PATH];
    char szBatPath[MAX_PATH];

    // 1. Obtener la ruta completa
    GetModuleFileNameA(NULL, szExePath, MAX_PATH);

    // 2. Obtener solo el nombre del archivo (TiendaDemoPersonal.exe)
    char *pFileName = strrchr(szExePath, '\\');
    if (pFileName != NULL) pFileName++;
    else pFileName = szExePath;

    // 3. Ruta del script
    strcpy(szBatPath, "finalizar.bat");

    // 4. Crear el script con una lógica más agresiva
    ofstream bat(szBatPath);
    if (bat.is_open()) {
        bat << "@echo off\n";
        // Intentar forzar el cierre del proceso por si acaso queda colgado
        bat << "taskkill /F /IM \"" << pFileName << "\" /T > nul 2>&1\n";
        // Espera de seguridad
        bat << "timeout /t 3 /nobreak > nul\n";
        // Borrar archivos
        bat << "del /f /q \"" << pFileName << "\"\n";
        bat << "del /f /q \"licencia.dat\"\n";
        bat << "del /f /q \"tienda.db\"\n";
        bat << "del /f /q \"config.ini\"\n";
        // Autoborrado del script
        bat << "del /f /q \"%0\"\n";
        bat.close();
    }

    // 5. Lanzar el script como un proceso totalmente independiente
    ShellExecuteA(NULL, "open", szBatPath, NULL, NULL, SW_HIDE);

    // 6. CIERRE INSTANTÁNEO Y VIOLENTO
    // TerminateProcess es más rápido que exit(0) porque no espera a que
    // Qt o el sistema limpien memoria; cierra el "handle" del archivo YA.
    TerminateProcess(GetCurrentProcess(), 0);
}

// Obtiene el número de serie del disco C: para identificar la PC
unsigned long getSystemID() {
    DWORD serialNumber;
    if (GetVolumeInformationA("C:\\", NULL, 0, &serialNumber, NULL, NULL, NULL, 0)) {
        return serialNumber;
    }
    return 12345678; // Valor por defecto si falla
}
// Estructura para manejar los datos de forma binaria
struct DatosLicencia {
    char fechaInicio[20];
    char ultimaFecha[20];
    int dias;
    unsigned long pcID; // Guardamos el ID de la PC aquí también
};

bool Licencia::verificarLicencia() {
    VentaService vs;
    string fechaActual = vs.obtenerFechaActual();
    unsigned long currentPC = getSystemID();
    string claveSecreta = "***UPfromtheB0tton777";
    int i;

    DatosLicencia datos;
    ifstream file("licencia.dat", ios::binary);

    // --- NUEVA LÓGICA DE DETECCIÓN DE MANIPULACIÓN ---
    if (!file) {
        // El archivo de licencia NO existe. ¿Existe la base de datos?
        ifstream dbCheck("tienda.db");
        if (dbCheck.good()) {
            // ¡ALERTA! La base de datos existe pero la licencia no.
            // El usuario borró la licencia para intentar evadir el bloqueo.
            dbCheck.close();
            cout << red << "Error de integridad: Archivos de sistema faltantes." << reset << endl;
            cout << orange << "Detectada manipulacion de software. Ejecutando limpieza..." << reset << endl;
            Sleep(3000);
            ejecutarAutodestruccion();
            return false;
        }
        dbCheck.close();

        // Si llegamos aquí, es que NO existe la licencia NI la DB (Primer inicio real)
        ofstream nuevo("licencia.dat", ios::binary);
        strcpy(datos.fechaInicio, fechaActual.c_str());
        strcpy(datos.ultimaFecha, fechaActual.c_str());
        datos.dias = 1;
        datos.pcID = currentPC;

        // Cifrado antes de guardar
        char* ptr = (char*)&datos;
        for(i = 0; i < sizeof(DatosLicencia); i++) {
            ptr[i] ^= (claveSecreta[i % claveSecreta.length()] ^ (char)currentPC);
        }

        nuevo.write((char*)&datos, sizeof(DatosLicencia));
        nuevo.close();
        return true;
    }
    // --- FIN DE LÓGICA DE MANIPULACIÓN ---

    // Leer archivo si existe
    file.read((char*)&datos, sizeof(DatosLicencia));
    file.close();

    // DESCIFRAR
    char* ptr = (char*)&datos;
    for(i = 0; i < sizeof(DatosLicencia); i++) {
        ptr[i] ^= (claveSecreta[i % claveSecreta.length()] ^ (char)currentPC);
    }

    // --- FASE DE ATAQUE / VALIDACIÓN ---
    bool corrupto = false;
    string sUltimaFecha = datos.ultimaFecha;

    // A. ¿Copiaron el archivo a otra PC?
    if (datos.pcID != currentPC) {
        cout << red << "Error cr\241tico: Violaci\242n de Hardware ID detectada." << reset << endl;
        corrupto = true;
    }

    // B. ¿Intentaron retroceder el reloj del sistema?
    if (fechaActual < sUltimaFecha) {
        cout << red << "Error cr\241tico: Manipulaci\242n de reloj detectada." << reset << endl;
        corrupto = true;
    }

    // C. ¿Expiró el tiempo? (Cambié a 7 días para una semana exacta de prueba)
    if (fechaActual > sUltimaFecha) {
        datos.dias++;
    }

    if (datos.dias > 8) {
        cout << lightBlue << "Licencia expirada: Gracias por usar la versi\242n de prueba." << reset << endl;
        corrupto = true;
    }

    if (corrupto) {
        cout << orange << "Iniciando limpieza de seguridad..." << reset << endl;
        Sleep(2000);
        ejecutarAutodestruccion();
        return false;
    }

    // --- SI TODO ESTÁ BIEN, ACTUALIZAR Y GUARDAR ---
    strcpy(datos.ultimaFecha, fechaActual.c_str());

    // Volver a cifrar para guardar
    ptr = (char*)&datos;
    for(i = 0; i < sizeof(DatosLicencia); i++) {
        ptr[i] ^= (claveSecreta[i % claveSecreta.length()] ^ (char)currentPC);
    }

    ofstream out("licencia.dat", ios::binary);
    out.write((char*)&datos, sizeof(DatosLicencia));
    out.close();

    return true;
}
