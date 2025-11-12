#include "../include/ProcesoPar.h"
#include <stdlib.h>

Estado_t destruirProcesoPar(ProcesoPar_t* procesoPar) {
    // Validación de parámetros
    if (!procesoPar) {
        return E_PAR_INC;
    }

    if (!procesoPar->activo) {
        return E_PROCESO_YA_DESTRUIDO;
    }

    // Detener el hilo de escucha si está activo
    if (procesoPar->escuchaActiva && procesoPar->hHiloEscucha) {
        procesoPar->escuchaActiva = FALSE;
        WaitForSingleObject(procesoPar->hHiloEscucha, 2000);
        CloseHandle(procesoPar->hHiloEscucha);
    }

    // Cerrar handles de las tuberías
    if (procesoPar->hTuberiaSalidaEscribir) {
        CloseHandle(procesoPar->hTuberiaSalidaEscribir);
    }
    if (procesoPar->hTuberiaEntradaLeer) {
        CloseHandle(procesoPar->hTuberiaEntradaLeer);
    }

    // Terminar el proceso hijo
    if (procesoPar->hProceso) {
        TerminateProcess(procesoPar->hProceso, 0);
        WaitForSingleObject(procesoPar->hProceso, 2000);
        CloseHandle(procesoPar->hProceso);
    }

    // Cerrar handle del hilo principal del proceso hijo
    if (procesoPar->hHilo) {
        CloseHandle(procesoPar->hHilo);
    }

    // Marcar como inactivo
    procesoPar->activo = FALSE;

    // Liberar memoria
    free(procesoPar);

    return E_OK;
}
