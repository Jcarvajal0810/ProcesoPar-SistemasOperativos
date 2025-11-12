#include "../include/ProcesoPar.h"
#include <stdio.h>

// Función que ejecutará el hilo de escucha
DWORD WINAPI hiloEscucha(LPVOID param) {
    ProcesoPar_t* procesoPar = (ProcesoPar_t*)param;
    char buffer[4096];
    DWORD bytesLeidos;

    while (procesoPar->escuchaActiva) {
        // Leer de la tubería
        if (ReadFile(
            procesoPar->hTuberiaEntradaLeer,
            buffer,
            sizeof(buffer) - 1,
            &bytesLeidos,
            NULL
        ) && bytesLeidos > 0) {
            buffer[bytesLeidos] = '\0';

            // Llamar a la función de callback si está establecida
            if (procesoPar->funcionEscucha) {
                procesoPar->funcionEscucha(buffer, bytesLeidos);
            }
        } else {
            // Si falla la lectura, es posible que el proceso hijo haya terminado
            DWORD exitCode;
            if (GetExitCodeProcess(procesoPar->hProceso, &exitCode) && 
                exitCode != STILL_ACTIVE) {
                break;
            }
        }
    }

    return 0;
}

Estado_t establecerFuncionDeEscucha(
    ProcesoPar_t* procesoPar,
    Estado_t (*f)(const char*, int)
) {
    // Validación de parámetros
    if (!procesoPar || !f) {
        return E_PAR_INC;
    }

    if (!procesoPar->activo) {
        return E_PROCESO_NO_ACTIVO;
    }

    // Establecer la función de callback
    procesoPar->funcionEscucha = f;
    procesoPar->escuchaActiva = TRUE;

    // Crear el hilo de escucha
    procesoPar->hHiloEscucha = CreateThread(
        NULL,           // Atributos de seguridad por defecto
        0,              // Tamaño de pila por defecto
        hiloEscucha,    // Función del hilo
        procesoPar,     // Parámetro para el hilo
        0,              // Flags de creación
        NULL            // ID del hilo (no necesitamos)
    );

    if (!procesoPar->hHiloEscucha) {
        procesoPar->escuchaActiva = FALSE;
        return E_CREAR_HILO;
    }

    return E_OK;
}
