# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -I../include
AR = ar
ARFLAGS = rcs

# Directorios
LIBDIR = ../lib
INCDIR = ../include

# Archivos fuente de la biblioteca
LIB_SOURCES = lanzarProcesoPar.cpp destruirProcesoPar.cpp enviarMensajeProcesoPar.cpp establecerFuncionDeEscucha.cpp
LIB_OBJECTS = $(addprefix $(LIBDIR)/, $(LIB_SOURCES:.cpp=.o))

# Nombre de la biblioteca
LIBRARY = $(LIBDIR)/libproceso_par.a

# Archivos de prueba
TEST_MAIN = test_main.exe
TEST_HIJO = test_proceso_hijo.exe

# Regla principal
all: $(LIBRARY) $(TEST_MAIN) $(TEST_HIJO)
	@echo ""
	@echo "Compilacion completada exitosamente"
	@echo "Biblioteca creada: $(LIBRARY)"
	@echo "Programas de prueba: $(TEST_MAIN) y $(TEST_HIJO)"

# Crear directorio lib si no existe
$(LIBDIR):
	mkdir -p $(LIBDIR)

# Compilar archivos objeto de la biblioteca
$(LIBDIR)/%.o: %.cpp | $(LIBDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear la biblioteca estática
$(LIBRARY): $(LIB_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Biblioteca creada: $@"

# Compilar programa de prueba principal
$(TEST_MAIN): test_main.cpp $(LIBRARY)
	$(CXX) $(CXXFLAGS) -o $@ test_main.cpp -L$(LIBDIR) -lproceso_par

# Compilar programa proceso hijo
$(TEST_HIJO): test_proceso_hijo.cpp
	$(CXX) $(CXXFLAGS) -o $@ test_proceso_hijo.cpp

# Ejecutar pruebas
test: all
	@echo ""
	@echo "Ejecutando pruebas..."
	@echo ""
	./$(TEST_MAIN)

# Limpiar archivos compilados
clean:
	rm -f $(LIBDIR)/*.o $(LIBRARY) $(TEST_MAIN) $(TEST_HIJO)
	@echo "Archivos limpiados"

# Ayuda
help:
	@echo "Comandos disponibles:"
	@echo "  make          - Compila la biblioteca y los programas de prueba"
	@echo "  make test     - Compila y ejecuta las pruebas"
	@echo "  make clean    - Elimina archivos compilados"
	@echo "  make help     - Muestra esta ayuda"

.PHONY: all test clean help
