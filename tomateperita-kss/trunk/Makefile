CFLAGS=-I./includes -I../tomateperita-linux-commons/includes
LDFLAGS=-L../tomateperita-linux-commons/build
MAIN=./build/bin/tomateperita-kss

all: objdir lib $(MAIN)

objdir:
	@echo "------------------------"
	@echo "Verificando existencia de carpeta build"
	if [ -d build ]; then rm -rf build; fi > /dev/null 2> /dev/null
	@echo "------------------------"
	@echo "Creando carpeta build y subcarpetas"
	mkdir build
	mkdir ./build/obj
	mkdir ./build/conf
	mkdir ./build/bin
	mkdir ./build/logs

lib:
	@echo "------------------------"
	@echo "Generando libreria tomateperita-linux-commons"
	make -C ../tomateperita-linux-commons > /dev/null 2> /dev/null

$(MAIN):
	@echo "------------------------"
	@echo "Generando ejecutable"
	gcc $(CFLAGS) src/core/* src/fss/* src/ftps/* src/shell/* src/vda/* tests/* $(LDFLAGS) -ltomateperita-linux-commons -lpthread -o $(MAIN) > /dev/null 2> /dev/null
	@echo "------------------------"
	@echo "Moviendo archivo de configuracion"
	cp conf/*.* build/conf/
	@echo "------------------------"
	@echo "Se ha generado la aplicacion tomateperita-kss con exito"
	@echo "------------------------"

clean:
	@echo "------------------------"
	rm -rf build
	@echo "Se elimino la carpeta build"

