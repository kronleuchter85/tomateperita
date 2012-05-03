CFLAGS=-I./includes -I../../../tomateperita-linux-commons/tags/primera-entrega-obligatoria/includes
LDFLAGS=-L../../../tomateperita-linux-commons/tags/primera-entrega-obligatoria
_OBJS=kss-microkernel.o kss-configuration.o kss-internal-state.o kss-state.o kss-tdd.o kss-utils.o kss-fss-service.o kss-ftps-service.o kss-ftps-worker-thread.o kss-mps-protocol.o kss-shell-commands.o kss-shell-service.o kss-shell-worker-thread.o kss-vda-component.o kss-vda-service.o
OBJS=$(patsubst %, build/obj/%, $(_OBJS))
FUENTES=./src/core/kss-microkernel.c ./src/core/kss-configuration.c ./src/core/kss-internal-state.c ./src/core/kss-state.c ./src/core/kss-tdd.c ./src/core/kss-utils.c ./src/fss/kss-fss-service.c ./src/ftps/kss-ftps-service.c ./src/ftps/kss-ftps-worker-thread.c ./src/mps/kss-mps-protocol.c ./src/shell/kss-shell-commands.c ./src/shell/kss-shell-service.c ./src/shell/kss-shell-worker-thread.c ./src/vda/kss-vda-component.c ./src/vda/kss-vda-service.c
MAIN=./build/bin/tomateperita-kss

all: objdir lib $(MAIN)

objdir:
	mkdir build
	mkdir ./build/obj
	mkdir ./build/conf
	mkdir ./build/bin
	mkdir ./build/logs

lib:
	make -C ../../../tomateperita-linux-commons/tags/primera-entrega-obligatoria

#obj/%.o: $(FUENTES) $(DEPS)
#	gcc -c -o $(OBJS) $(FUENTES) $(CFLAGS) $(LDFLAGS) -llinux-commons

$(MAIN): $(OBJS)
	gcc -o $(MAIN) $(CFLAGS) $(OBJS) $(LDFLAGS) -llinux-commons -lpthread
	rm -rf ./build/obj
	cp conf/*.* build/conf/

./build/obj/kss-microkernel.o:
	gcc -c -o ./build/obj/kss-microkernel.o ./src/core/kss-microkernel.c $(CFLAGS) $(LDFLAGS) -llinux-commons

./build/obj/kss-configuration.o:
	gcc -c -o ./build/obj/kss-configuration.o ./src/core/kss-configuration.c $(CFLAGS) $(LDFLAGS) -llinux-commons

./build/obj/kss-internal-state.o:
	gcc -c -o ./build/obj/kss-internal-state.o ./src/core/kss-internal-state.c $(CFLAGS) $(LDFLAGS) -llinux-commons

./build/obj/kss-state.o:
	gcc -c -o ./build/obj/kss-state.o ./src/core/kss-state.c $(CFLAGS) $(LDFLAGS) -llinux-commons

./build/obj/kss-tdd.o:
	gcc -c -o ./build/obj/kss-tdd.o ./src/core/kss-tdd.c $(CFLAGS) $(LDFLAGS) -llinux-commons

./build/obj/kss-utils.o:
	gcc -c -o ./build/obj/kss-utils.o ./src/core/kss-utils.c $(CFLAGS) $(LDFLAGS) -llinux-commons

./build/obj/kss-fss-service.o:
	gcc -c -o ./build/obj/kss-fss-service.o ./src/fss/kss-fss-service.c $(CFLAGS) $(LDFLAGS) -llinux-commons

./build/obj/kss-ftps-service.o:
	gcc -c -o ./build/obj/kss-ftps-service.o ./src/ftps/kss-ftps-service.c $(CFLAGS) $(LDFLAGS) -llinux-commons -lpthread

./build/obj/kss-ftps-worker-thread.o:
	gcc -c -o ./build/obj/kss-ftps-worker-thread.o ./src/ftps/kss-ftps-worker-thread.c $(CFLAGS) $(LDFLAGS) -llinux-commons

./build/obj/kss-mps-protocol.o:
	gcc -c -o ./build/obj/kss-mps-protocol.o ./src/mps/kss-mps-protocol.c $(CFLAGS) $(LDFLAGS) -llinux-commons

./build/obj/kss-shell-commands.o:
	gcc -c -o ./build/obj/kss-shell-commands.o ./src/shell/kss-shell-commands.c $(CFLAGS) $(LDFLAGS) -llinux-commons

./build/obj/kss-shell-service.o:
	gcc -c -o ./build/obj/kss-shell-service.o ./src/shell/kss-shell-service.c $(CFLAGS) $(LDFLAGS) -llinux-commons -lpthread

./build/obj/kss-shell-worker-thread.o:
	gcc -c -o ./build/obj/kss-shell-worker-thread.o ./src/shell/kss-shell-worker-thread.c $(CFLAGS) $(LDFLAGS) -llinux-commons

./build/obj/kss-vda-component.o:
	gcc -c -o ./build/obj/kss-vda-component.o ./src/vda/kss-vda-component.c $(CFLAGS) $(LDFLAGS) -llinux-commons

./build/obj/kss-vda-service.o:
	gcc -c -o ./build/obj/kss-vda-service.o ./src/vda/kss-vda-service.c $(CFLAGS) $(LDFLAGS) -llinux-commons

clean:
	rm -rf build
