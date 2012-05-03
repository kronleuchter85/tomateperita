#
#
# Linux Commons Makefile
#
#

#
#       Macros
#

TARGET = build/libtomateperita-linux-commons.a
INCLUDES = -I./includes
FLAGS = -w -g -c -o
 
#
#       Compilation
#

all: objdir *.o
	@echo "------------------"
	@echo "Generando libreria"
	ar rcs $(TARGET) build/obj/*.o > /dev/null 2> /dev/null
	@echo "------------------"
	@echo "Eliminando carpeta obj"
	rm -rf build/obj > /dev/null 2> /dev/null
	@echo "------------------"
	@echo "Finalizada la creacion de la libreria tomateperita-linux-commons"
	@echo "------------------"

objdir:
	@echo "------------------"
	@echo "Eliminando carpeta build si existe"
	if [ -d build ]; then rm -rf build; fi > /dev/null 2> /dev/null
	@echo "------------------"
	@echo "Creando carpeta build"
	mkdir build > /dev/null 2> /dev/null
	@echo "------------------"
	@echo "Creando subcarpeta object"
	mkdir build/obj > /dev/null 2> /dev/null

*.o:
	@echo "------------------"
	@echo "Generando archivos objetos"
	gcc $(INCLUDES) src/* -w -g -c > /dev/null 2> /dev/null
	@echo "------------------"
	@echo "Moviendo objetos a carpeta build/obj"
	mv *.o build/obj > /dev/null 2> /dev/null

clean:
	@echo "------------------"
	rm -rf build > /dev/null 2> /dev/null
	@echo "Se elimino la carpeta build"
