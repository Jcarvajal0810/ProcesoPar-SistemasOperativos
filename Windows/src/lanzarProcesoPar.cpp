#include "../include/ProcesoPar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Estado_t lanzarProcesoPar(
    const char* nombreArchivoEjecutable,
    const char** listaLineaComando,
    ProcesoPar_t** procesoPar
) {
    // Validación de parámetros
    if (!nombreArchivoEjecutable || !procesoPar) {
        return E_PAR_INC;
    }

    // Asignar memoria para la estructura ProcesoPar_t
    *procesoPar = (ProcesoPar_t*)malloc(sizeof(ProcesoPar_t));
    if (!*procesoPar) {
        return E_NO_MEMORIA;
    }

    // Inicializar la estructura
    memset(*procesoPar, 0, sizeof(ProcesoPar_t));
    (*procesoPar)->activo = FALSE;
    (*procesoPar)->escuchaActiva = FALSE;
    (*procesoPar)->funcionEscucha = NULL;

    // Atributos de seguridad para que los handles sean heredables
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    // Crear tubería de salida (padre escribe, hijo lee)
    if (!CreatePipe(&(*procesoPar)->hTuberiaSalidaLeer, 
                    &(*procesoPar)->hTuberiaSalidaEscribir, 
                    &sa, 0)) {
        free(*procesoPar);
        return E_CREAR_TUBERIA;
    }

    // Asegurar que el handle de escritura del padre no sea heredable
    SetHandleInformation((*procesoPar)->hTuberiaSalidaEscribir, HANDLE_FLAG_INHERIT, 0);

    // Crear tubería de entrada (hijo escribe, padre lee)
    if (!CreatePipe(&(*procesoPar)->hTuberiaEntradaLeer, 
                    &(*procesoPar)->hTuberiaEntradaEscribir, 
                    &sa, 0)) {
        CloseHandle((*procesoPar)->hTuberiaSalidaLeer);
        CloseHandle((*procesoPar)->hTuberiaSalidaEscribir);
        free(*procesoPar);
        return E_CREAR_TUBERIA;
    }

    // Asegurar que el handle de lectura del padre no sea heredable
    SetHandleInformation((*procesoPar)->hTuberiaEntradaLeer, HANDLE_FLAG_INHERIT, 0);

    // Construir la línea de comandos
    char cmdLine[4096] = {0};
    strncpy(cmdLine, nombreArchivoEjecutable, sizeof(cmdLine) - 1);

    if (listaLineaComando) {
        for (int i = 0; listaLineaComando[i] != NULL; i++) {
            strncat(cmdLine, " ", sizeof(cmdLine) - strlen(cmdLine) - 1);
            strncat(cmdLine, listaLineaComando[i], sizeof(cmdLine) - strlen(cmdLine) - 1);
        }
    }

    // Configurar STARTUPINFO para redirigir stdin/stdout del proceso hijo
    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdInput = (*procesoPar)->hTuberiaSalidaLeer;   // El hijo lee de aquí
    si.hStdOutput = (*procesoPar)->hTuberiaEntradaEscribir; // El hijo escribe aquí
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.dwFlags |= STARTF_USESTDHANDLES;

    // Información del proceso
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    // Crear el proceso hijo
    if (!CreateProcessA(
        NULL,           // Nombre del módulo (NULL para usar cmdLine)
        cmdLine,        // Línea de comandos
        NULL,           // Atributos de seguridad del proceso
        NULL,           // Atributos de seguridad del hilo
        TRUE,           // Heredar handles
        0,              // Flags de creación
        NULL,           // Usar el ambiente del padre
        NULL,           // Usar el directorio del padre
        &si,            // STARTUPINFO
        &pi             // PROCESS_INFORMATION
    )) {
        CloseHandle((*procesoPar)->hTuberiaEntradaLeer);
        CloseHandle((*procesoPar)->hTuberiaEntradaEscribir);
        CloseHandle((*procesoPar)->hTuberiaSalidaLeer);
        CloseHandle((*procesoPar)->hTuberiaSalidaEscribir);
        free(*procesoPar);
        return E_CREAR_PROCESO;
    }

    // Guardar handles del proceso
    (*procesoPar)->hProceso = pi.hProcess;
    (*procesoPar)->hHilo = pi.hThread;
    (*procesoPar)->activo = TRUE;

    // Cerrar los handles que el padre no necesita
    CloseHandle((*procesoPar)->hTuberiaSalidaLeer);
    CloseHandle((*procesoPar)->hTuberiaEntradaEscribir);

    return E_OK;
}
