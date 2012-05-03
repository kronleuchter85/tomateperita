/**
 * file-commons.h
 * Funciones para el manejo de archivos externos
 */

#ifndef _W_FILE_COMMONS_H_
#define _W_FILE_COMMONS_H_

#include "memory-commons.h"
#include <windows.h>

char * getConfigurationStrValue (HeapHandler hh , char *filename, char *section, char *key);
int getConfigurationIntValue (char *filename, char *section, char *key);
HANDLE getFileHandle(char *);

	char * getValue(char * line , HeapHandler h);
	char * getKey(char * line , HeapHandler h);
	void loadConfiguration(char * configFile , HeapHandler , void (*processKeysAndValues)(char * , char *));



	FILE * getFile(char * file);
#endif

