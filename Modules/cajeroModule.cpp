#include <Services/ventaService.h>
#include <Modules/cajeroModule.h>

void CajeroModule::iniciar(){
    VentaService vs;
    vs.iniciarVenta();
}
