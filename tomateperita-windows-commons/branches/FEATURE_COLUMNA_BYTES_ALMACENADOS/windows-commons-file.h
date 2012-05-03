/**
 * file-commons.h
 * Funciones para el manejo de archivos externos
 */

#ifndef _W_FILE_COMMONS_H_
#define _W_FILE_COMMONS_H_

#include "windows-commons-memory.h"
#include <windows.h>


	#pragma pack(1)
	typedef struct {
		int sector;
		char data[512];
		int dataLength;
	} FileSector;
	
	#pragma pack(1)
	typedef struct {
		FileSector sector1;
		FileSector sector2;
	} FileBlock;




char * getConfigurationStrValue (HeapHandler hh , char *filename, char *section, char *key);
int getConfigurationIntValue (char *filename, char *section, char *key);
HANDLE getFileHandle(char *);
HANDLE getReadFileHandle(char *);
HANDLE getWriteFileHandle(char *);

	char * getValue(char * line , HeapHandler h);
	char * getKey(char * line , HeapHandler h);
	void loadConfigurationW(char * configFile , HeapHandler , void (*processKeysAndValues)(char * , char *));

#endif
