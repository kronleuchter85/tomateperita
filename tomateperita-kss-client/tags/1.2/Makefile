CFLAGS=-I../tomateperita-linux-commons/includes -I./includes
LDFLAGS=-L../tomateperita-linux-commons/build
MAIN=./build/bin/kss-client-launch

all: objdir lib $(MAIN)

objdir:
	@echo "-------------------------"
	@echo "Verificando existencia de carpeta build"
	if [ -d build ]; then rm -rf build; fi > /dev/null 2> /dev/null
	@echo "-------------------------"
	@echo "Creando carpeta build con sus subcarpetas"
	mkdir build
	mkdir ./build/bin
	mkdir ./build/conf
	mkdir ./build/logs

lib:
	@echo "-------------------------"
	@echo "Generando la libreria tomateperita-linux-commons"
	make -C ../tomateperita-linux-commons > /dev/null 2> /dev/null

$(MAIN):
	@echo "-------------------------"
	@echo "Generando ejecutable tomateperita-kss-client"
	gcc $(CFLAGS) src/* $(LDFLAGS) -ltomateperita-linux-commons -o $(MAIN) > /dev/null 2> /dev/null
	@echo "-------------------------"
	cp conf/*.* build/conf/
	@echo "Se ha movido el archivo de configuracion"
	@echo "-------------------------"
	@echo "Se ha creado el modulo tomateperita-kss-client con exito"
	@echo "-------------------------"

clean:
	@echo "-------------------------"
	rm -rf build
	@echo "Se ha eliminado la carpeta build"

