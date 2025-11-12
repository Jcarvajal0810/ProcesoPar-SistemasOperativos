#include "../include/ProcesoPar.h"

Estado_t enviarMensajeProcesoPar(
    ProcesoPar_t* procesoPar,
    const char* mensaje,
    int longitud
) {
    // Validación de parámetros
    if (!procesoPar || !mensaje || longitud <= 0) {
        return E_PAR_INC;
    }

    if (!procesoPar->activo) {
        return E_PROCESO_NO_ACTIVO;
    }

    DWORD bytesEscritos = 0;

    // Escribir el mensaje en la tubería
    if (!WriteFile(
        procesoPar->hTuberiaSalidaEscribir,
        mensaje,
        longitud,
        &bytesEscritos,
        NULL
    )) {
        return E_ESCRIBIR_TUBERIA;
    }

    // Asegurar que se envíe inmediatamente
    FlushFileBuffers(procesoPar->hTuberiaSalidaEscribir);

    return E_OK;
}
