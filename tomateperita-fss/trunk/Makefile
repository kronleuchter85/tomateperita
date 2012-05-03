CFLAGS=-I./includes -I../tomateperita-linux-commons/includes
LDFLAGS=-L../tomateperita-linux-commons/build
MAIN=./build/bin/tomateperita-fss

all: objdir $(MAIN)

objdir:
	@echo "-------------------------"
	@echo "Verificando existencia de carpeta build"
	if [ -d build ]; then rm -rf build; fi > /dev/null 2> /dev/null
	@echo "-------------------------"
	@echo "Creando carpeta build y sus subcarpetas"
	mkdir build
	mkdir ./build/conf
	mkdir ./build/bin
	mkdir ./build/logs
	mkdir ./build/obj

lib:
	@echo "-------------------------"
	@echo "Generando libreria tomateperita-linux-commons"
	make -C ../tomateperita-linux-commons > /dev/null 2> /dev/null

$(MAIN):
	@echo "-------------------------"
	@echo "Generando ejecutable tomateperita-fss en la carpeta build/bin"
	gcc $(CFLAGS) src/core/* src/fsm/* src/mps/* test/* $(LDFLAGS) -ltomateperita-linux-commons -lpthread -o $(MAIN) > /dev/null 2> /dev/null
	@echo "-------------------------"
	cp conf/*.* build/conf
	@echo "Se ha movido el archivo de configuracion"
	@echo "-------------------------"
	@echo "El modulo tomateperita-fss se ha creado con exito"
	@echo "-------------------------"

clean:
	@echo "-------------------------"
	rm -rf build
	@echo "Se ha eliminado la carpeta build"

