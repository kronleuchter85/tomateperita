#
#
# Linux Commons Makefile
#
#

#
#       Macros
#

TARGET = liblinux-commons.a

SOURCE = src
INCLUDES = -I./includes
FLAGS = -w -g -c -o
OBJECTS = linux-commons.o linux-commons-file.o linux-commons-list.o linux-commons-logging.o linux-commons-mps.o linux-commons-socket.o

#
#       Compilation
#

all: objdir $(OBJECTS)
	ar rcs $(TARGET) obj/linux-commons.o obj/linux-commons-file.o obj/linux-commons-list.o obj/linux-commons-logging.o obj/linux-commons-mps.o obj/linux-commons-socket.o 
	rm -rf obj

objdir:
	mkdir obj

linux-commons.o: 
	gcc $(SOURCE)/linux-commons.c $(FLAGS) obj/linux-commons.o $(INCLUDES)

linux-commons-file.o:
	gcc $(SOURCE)/linux-commons-file.c $(FLAGS) obj/linux-commons-file.o $(INCLUDES)

linux-commons-list.o:
	gcc $(SOURCE)/linux-commons-list.c $(FLAGS) obj/linux-commons-list.o $(INCLUDES)

linux-commons-logging.o:
	gcc $(SOURCE)/linux-commons-logging.c $(FLAGS) obj/linux-commons-logging.o $(INCLUDES)

linux-commons-mps.o:
	gcc $(SOURCE)/linux-commons-mps.c $(FLAGS) obj/linux-commons-mps.o $(INCLUDES)

linux-commons-socket.o:
	gcc $(SOURCE)/linux-commons-socket.c $(FLAGS) obj/linux-commons-socket.o $(INCLUDES)

clean:
	rm *.a
