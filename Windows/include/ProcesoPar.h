#ifndef PROCESOPAR_H
#define PROCESOPAR_H

#include <windows.h>

// Tipo de dato para el estado de las operaciones
typedef unsigned int Estado_t;

// Estructura que representa un proceso par
typedef struct ProcesoPar {
    HANDLE hProceso;                    // Handle del proceso hijo
    HANDLE hHilo;                       // Handle del hilo principal del proceso hijo
    HANDLE hTuberiaEntradaLeer;        // Handle para leer mensajes del proceso hijo
    HANDLE hTuberiaEntradaEscribir;    // Handle para que el hijo escriba (no usado directamente por el padre)
    HANDLE hTuberiaSalidaLeer;         // Handle para que el hijo lea (no usado directamente por el padre)
    HANDLE hTuberiaSalidaEscribir;     // Handle para escribir mensajes al proceso hijo
    HANDLE hHiloEscucha;               // Handle del hilo que escucha mensajes del hijo
    BOOL activo;                       // Indica si el proceso está activo
    Estado_t (*funcionEscucha)(const char*, int); // Puntero a función callback
    BOOL escuchaActiva;                // Indica si el hilo de escucha está activo
} ProcesoPar_t;

// Códigos de estado según la especificación
#define E_OK                    0   // La función se comportó bien
#define E_PAR_INC               1   // Parámetro incorrecto
#define E_NO_MEMORIA            2   // No hay memoria suficiente
#define E_CREAR_TUBERIA         3   // Error al crear tubería
#define E_CREAR_PROCESO         4   // Error al crear proceso
#define E_PROCESO_NO_ACTIVO     5   // El proceso no está activo
#define E_ESCRIBIR_TUBERIA      6   // Error al escribir en tubería
#define E_LEER_TUBERIA          7   // Error al leer de tubería
#define E_PROCESO_YA_DESTRUIDO  8   // El proceso ya fue destruido
#define E_CREAR_HILO            9   // Error al crear hilo de escucha
#define E_NO_FUNCION_ESCUCHA    10  // No se ha establecido función de escucha

// Prototipos de funciones según la especificación
Estado_t lanzarProcesoPar(
    const char* nombreArchivoEjecutable,
    const char** listaLineaComando,
    ProcesoPar_t** procesoPar
);

Estado_t destruirProcesoPar(
    ProcesoPar_t* procesoPar
);

Estado_t enviarMensajeProcesoPar(
    ProcesoPar_t* procesoPar,
    const char* mensaje,
    int longitud
);

Estado_t establecerFuncionDeEscucha(
    ProcesoPar_t* procesoPar,
    Estado_t (*f)(const char*, int)
);

#endif // PROCESOPAR_H
