/**
 * file-commons.h
 * Funciones para el manejo de archivos externos
 */


#include "windows-commons-memory.h"
#include <windows.h>

#ifndef _W_FILE_COMMONS_H_
#define _W_FILE_COMMONS_H_



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
		int fileHandleId;
	} FileBlock;




char * getConfigurationStrValue (HeapHandler hh , char *filename, char *section, char *key);
int getConfigurationIntValue (char *filename, char *section, char *key);
HANDLE getFileHandle(char *);
HANDLE getReadFileHandle(char *);
HANDLE getWriteFileHandle(char *);

	char * getValue(char * line , HeapHandler h);
	char * getKey(char * line , HeapHandler h);
	void loadConfigurationW(char * configFile , HeapHandler , void (*processKeysAndValues)(char * , char *));

	
	void cleanBlock(FileBlock * block);
	FileBlock * buildEmptyFileBlock(HeapHandler hh);
	void setFileBlockSectors(FileBlock * block, int s1 , int s2 );
	void setFileBlockContent(FileBlock * block , char * buffer );
	void setFileBlockContents(FileBlock * block , char * content1 , char * content2 , 
		int length1 , int length2);

	BOOL hasTwoSectors(FileBlock * fileBlock);




#endif
