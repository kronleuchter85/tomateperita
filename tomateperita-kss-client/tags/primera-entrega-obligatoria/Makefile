CFLAGS=-I./includes -I../../../tomateperita-linux-commons/tags/primera-entrega-obligatoria/includes
LDFLAGS=-L../../../tomateperita-linux-commons/tags/primera-entrega-obligatoria
OBJS=./build/obj/tomateperita-kss-client.o ./build/obj/kss-client-configuration.o
FUENTES=./src/kss-client-configuration.c ./src/tomateperita-kss-client.c
MAIN=./build/bin/tomateperita-kss-client

all: objdir lib $(MAIN)

objdir:
	mkdir build
	mkdir ./build/obj
	mkdir ./build/bin
	mkdir ./build/conf
	mkdir ./build/logs

lib:
	make -C ../../../tomateperita-linux-commons/tags/primera-entrega-obligatoria

$(MAIN): $(OBJS)
	gcc -o $(MAIN) $(CFLAGS) $(OBJS) $(LDFLAGS) -llinux-commons
	rm -rf ./build/obj
	cp conf/*.* build/conf/

./build/obj/tomateperita-kss-client.o:
	gcc -c -o ./build/obj/tomateperita-kss-client.o ./src/tomateperita-kss-client.c $(CFLAGS) $(LDFLAGS) -llinux-commons

./build/obj/kss-client-configuration.o:
	gcc -c -o ./build/obj/kss-client-configuration.o ./src/kss-client-configuration.c $(CFLAGS) $(LDFLAGS) -llinux-commons

clean:
	rm -rf build
