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
TEST_BIN=test-tomateperita-linux-commons 
REPORTS_DIR=reports

#
#       Compilation
#

all: objdir *.o
	ar rcs $(TARGET) build/obj/*.o > /dev/null 2> /dev/null
	rm -rf build/obj > /dev/null 2> /dev/null

objdir:
	if [ -d build ]; then rm -rf build; fi > /dev/null 2> /dev/null
	mkdir build > /dev/null 2> /dev/null
	mkdir build/obj > /dev/null 2> /dev/null

*.o:
	gcc $(INCLUDES) src/misc/* -w -g -c > /dev/null 2> /dev/null
	gcc $(INCLUDES) src/mps/* -w -g -c > /dev/null 2> /dev/null
	mv *.o build/obj > /dev/null 2> /dev/null

clean:
	rm -rf build > /dev/null 2> /dev/null

tests:
	echo "Compilando tests"
	
	gcc test/* -o test-tomateperita-linux-commons -I. -I/usr/local/include/CUnit -L/usr/local/lib -lcunit -static
	./test-tomateperita-linux-commons

reports:
	xsltproc --stringparam suitename test-tomateperita-commons -o Hudson_Reports.xml cunit-to-junit.xsl test-tomateperita-commons-Results.xml


