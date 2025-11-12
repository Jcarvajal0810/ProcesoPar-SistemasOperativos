
                    BIBLIOTECA DE PROCESO PAR - WINDOWS
                          

DESCRIPCION
-----------
Esta biblioteca implementa el concepto de "Proceso Par" que permite a un
proceso principal (Proceso A) lanzar, controlar y comunicarse de manera
bidireccional (full-duplex) con un proceso secundario (Proceso B) mediante
tuberias anonimas de Windows.

CARACTERISTICAS
---------------
- Comunicacion bidireccional mediante tuberias (pipes)
- Envio y recepcion de mensajes de cualquier longitud
- Sistema de callback para procesar mensajes recibidos
- Manejo robusto de errores con codigos de estado
- Implementacion con API de Windows (x64)

ESTRUCTURA DEL PROYECTO
------------------------
windows/
  |-- src/
  |   |-- lanzarProcesoPar.cpp          - Lanza un nuevo proceso par
  |   |-- destruirProcesoPar.cpp        - Destruye un proceso par
  |   |-- enviarMensajeProcesoPar.cpp   - Envia mensajes al proceso hijo
  |   |-- establecerFuncionDeEscucha.cpp - Configura funcion de callback
  |   |-- test_main.cpp                 - Programa de prueba principal
  |   |-- test_proceso_hijo.cpp         - Programa de prueba (proceso hijo)
  |   |-- Makefile                      - Script de compilacion
  |-- lib/
  |   |-- *.o                           - Archivos objeto
  |   |-- libproceso_par.a              - Biblioteca estatica
  |-- include/
  |   |-- ProcesoPar.h                  - Archivo de cabecera con API
  |-- readme.txt                        - Este archivo

REQUISITOS DEL SISTEMA
-----------------------
- Windows 11 (o Windows 10)
- MSYS2 con MinGW-w64
- Compilador GNU GCC/G++
- Make (incluido en MSYS2)

INSTALACION DE MSYS2
---------------------
1. Descargar MSYS2 desde: https://www.msys2.org/
2. Ejecutar el instalador (msys2-x86_64-*.exe)
3. Seguir las instrucciones de instalacion
4. Abrir "MSYS2 MINGW64" desde el menu inicio
5. Actualizar el sistema:
   $ pacman -Syu
   (Cerrar y reabrir la terminal si es necesario)
6. Instalar herramientas de desarrollo:
   $ pacman -S mingw-w64-x86_64-gcc
   $ pacman -S mingw-w64-x86_64-make
   $ pacman -S make

COMPILACION
-----------
1. Abrir "MSYS2 MINGW64" desde el menu inicio de Windows
2. Navegar al directorio del proyecto:
   $ cd /c/ruta/a/tu/proyecto/windows/src
   
   Ejemplo:
   $ cd /c/Users/TuUsuario/Desktop/proyecto/windows/src

3. Compilar la biblioteca y programas de prueba:
   $ make

   Esto generara:
   - ../lib/libproceso_par.a  (biblioteca estatica)
   - ../lib/*.o               (archivos objeto)
   - test_proceso_hijo.exe    (proceso hijo de prueba)
   - test_main.exe            (programa principal de prueba)

4. Para limpiar archivos compilados:
   $ make clean

5. Para compilar y ejecutar pruebas automaticamente:
   $ make test

EJECUCION DE PRUEBAS
---------------------
Despues de compilar, ejecutar el programa de prueba:

$ ./test_main.exe

El programa realizara las siguientes pruebas:
1. Lanzar un proceso hijo
2. Establecer funcion de escucha
3. Enviar 3 mensajes al proceso hijo
4. Recibir respuestas del proceso hijo
5. Destruir el proceso hijo
6. Mostrar resumen de resultados

USO DE LA API
-------------
Incluir el archivo de cabecera en tu codigo:
    #include "ProcesoPar.h"

Compilar tu programa con la biblioteca:
    g++ -o mi_programa mi_programa.cpp -I../include -L../lib -lproceso_par

FUNCIONES DE LA API
-------------------
1. lanzarProcesoPar()
   - Crea y lanza un nuevo proceso hijo
   - Establece tuberias de comunicacion bidireccional
   - Parametros: ejecutable, argumentos, puntero a ProcesoPar_t
   
2. destruirProcesoPar()
   - Termina el proceso hijo
   - Libera recursos (handles, memoria)
   - Parametro: puntero a ProcesoPar_t

3. enviarMensajeProcesoPar()
   - Envia un mensaje al proceso hijo
   - Soporta mensajes de cualquier longitud
   - Parametros: ProcesoPar_t, mensaje, longitud

4. establecerFuncionDeEscucha()
   - Configura una funcion callback para recibir mensajes
   - Crea un hilo de escucha automatico
   - Parametros: ProcesoPar_t, funcion callback

CODIGOS DE ESTADO
-----------------
E_OK                    (0)  - Operacion exitosa
E_PAR_INC              (1)  - Parametro incorrecto
E_NO_MEMORIA           (2)  - Memoria insuficiente
E_CREAR_TUBERIA        (3)  - Error al crear tuberia
E_CREAR_PROCESO        (4)  - Error al crear proceso
E_PROCESO_NO_ACTIVO    (5)  - Proceso no esta activo
E_ESCRIBIR_TUBERIA     (6)  - Error al escribir en tuberia
E_LEER_TUBERIA         (7)  - Error al leer de tuberia
E_PROCESO_YA_DESTRUIDO (8)  - Proceso ya destruido
E_CREAR_HILO           (9)  - Error al crear hilo
E_NO_FUNCION_ESCUCHA   (10) - Funcion de escucha no establecida

PROTOCOLO DE COMUNICACION
--------------------------
Los mensajes se envian con el siguiente formato:
1. 4 bytes: Longitud del mensaje (entero)
2. N bytes: Contenido del mensaje

Este protocolo permite mensajes de longitud variable y detectar
correctamente los limites de cada mensaje.

EJEMPLO DE USO
--------------
    #include "ProcesoPar.h"
    
    // Funcion callback
    Estado_t miCallback(const char* msg, int len) {
        printf("Recibido: %s\n", msg);
        return E_OK;
    }
    
    int main() {
        ProcesoPar_t* pp;
        
        // Lanzar proceso
        lanzarProcesoPar("hijo.exe", NULL, &pp);
        
        // Establecer escucha
        establecerFuncionDeEscucha(pp, miCallback);
        
        // Enviar mensaje
        const char* msg = "Hola";
        enviarMensajeProcesoPar(pp, msg, strlen(msg));
        
        // Esperar y destruir
        Sleep(1000);
        destruirProcesoPar(pp);
        
        return 0;
    }

NOTAS IMPORTANTES
-----------------
- La biblioteca utiliza la API nativa de Windows (CreateProcess, CreatePipe)
- Las tuberias son anonimas y no tienen nombre en el sistema de archivos
- El proceso hijo hereda handles de las tuberias configuradas
- La funcion de escucha se ejecuta en un hilo separado
- Todos los handles se cierran automaticamente al destruir el proceso par
- La comunicacion es segura para datos binarios

SOLUCION DE PROBLEMAS
---------------------
1. Error "command not found" al ejecutar make:
   - Verificar que MSYS2 este correctamente instalado
   - Usar "MSYS2 MINGW64" (no "MSYS2 MSYS")
   - Instalar make: pacman -S make mingw-w64-x86_64-make

2. Error al crear proceso hijo:
   - Verificar que el ejecutable hijo exista en la ruta especificada
   - Usar rutas relativas o absolutas correctas
   - Verificar permisos de ejecucion

3. Mensajes no se reciben:
   - Asegurarse de establecer la funcion de escucha antes de enviar
   - Dar tiempo suficiente para que los mensajes se procesen
   - Verificar que el proceso hijo este escribiendo correctamente

4. El proceso hijo no termina:
   - Asegurarse de cerrar las tuberias en el hijo cuando termine
   - El padre usa TerminateProcess si el hijo no termina

Jose Manuel Carvajal Aristizabal
Proyecto de Sistemas Operativos
Implementacion de Procesos Pares en Windows


