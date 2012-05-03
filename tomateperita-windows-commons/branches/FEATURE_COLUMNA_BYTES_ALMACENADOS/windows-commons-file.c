/**
 * file-commons.h
 * Funciones para el manejo de archivos externos
 */

#include "windows-commons-file.h"
#include "windows-commons.h"

#define	STR_ITEM_VACIO NULL
#define	STR_ITEM_LEN 20
#define	INT_ITEM_VACIO 0

#define FILE_LINE_LENGTH 100

	/*
	 * Leen la propiedad dada por key desde un archivo de configuracion
	 */
	
	char * getConfigurationStrValue (HeapHandler hh , char *filename, char *section, char *key) {
		
		char * value = allocateMemory( hh , STR_ITEM_LEN * sizeof(char) );

		if (value == NULL){
			return NULL;
		}

		GetPrivateProfileStringA (section, key, STR_ITEM_VACIO, value, STR_ITEM_LEN, filename);
		/*GetPrivateProfileStringA ((LPCSTR)section ,(LPCSTR)key, STR_ITEM_VACIO, value, STR_ITEM_LEN, (LPCSTR)filename);*/
		return value;
	}

	int getConfigurationIntValue(char *filename, char *section, char *key) {
		return GetPrivateProfileIntA((LPCSTR)section, (LPCSTR)key, INT_ITEM_VACIO, filename);
	}

	HANDLE getFileHandle(char * filename){
		HANDLE newFile;
		
		newFile = CreateFileA(filename,
			GENERIC_ALL,
			FILE_SHARE_READ,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		return newFile;
	}

	HANDLE getReadFileHandle(char * filename){
		return CreateFileA(filename,   // file to open
            GENERIC_READ,          // open for reading
            FILE_SHARE_READ,       // share for reading
            NULL,                  // default security
            OPEN_EXISTING,         // existing file only
            FILE_ATTRIBUTE_NORMAL, // normal file
            NULL);                 // no attr. template
	}

	HANDLE getWriteFileHandle(char * filename){
		return CreateFileA(filename,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	}

	BOOL readLine (HANDLE hFile, char * buf) {
		char *p;
		char x;
		BOOL io;
		DWORD byteWritten = 0;
		ZeroMemory(buf, FILE_LINE_LENGTH);
		p = buf;
		io = TRUE;
		while ((p-buf < FILE_LINE_LENGTH) && io){
			io = ReadFile(hFile, p, 1, &byteWritten, NULL);
			if (io && (*p!=13) && (*p!=10) && (*p!=EOF)) p++;
			else if (*buf==EOF) return FALSE;
			else {
				*p='\0';
				io = ReadFile(hFile, &x, 1, &byteWritten, NULL); //leo el caracter \n del fin de linea para avanza puntero
				return TRUE;
			}
		}
		return FALSE;
	}

	void loadConfigurationW(char * filename , HeapHandler h , void (*processKeysAndValues)(char * , char *)){
		char lineBuffer[FILE_LINE_LENGTH];

		HANDLE hFile = getReadFileHandle(filename);
		if (hFile == INVALID_HANDLE_VALUE) return;
		while(readLine(hFile, lineBuffer)){
			if( !startsWith(lineBuffer , "#") ){
				char * key = getKey(lineBuffer , h);
				char * value = getValue(lineBuffer , h);
				if(key != NULL && value != NULL)
					processKeysAndValues(key , value);
			}
		}
		CloseHandle(hFile);
	}
	
	/*
	void loadConfiguration(char * configFile , HeapHandler h , void (*processKeysAndValues)(char * , char *)){
		FILE * file = getFile(configFile);
		char lineBuffer[FILE_LINE_LENGTH];
		if(file == NULL)
			return;

		ZeroMemory(lineBuffer , FILE_LINE_LENGTH);
		while(fgets(lineBuffer , FILE_LINE_LENGTH , file)){
			
			if( !startsWith(lineBuffer , "#") ){
				char * key = getKey(lineBuffer , h);
				char * value = getValue(lineBuffer , h);
				
				if(key != NULL && value != NULL)
					processKeysAndValues(key , value);
			}
			ZeroMemory(lineBuffer , FILE_LINE_LENGTH);
		}
	}
	*/

	char * getKey(char * line , HeapHandler h){
		char * formated = trim(line , h);
		int indexEqual = indexOfChar(formated , '=');
		if(indexEqual > 0){
			return subString(formated , 0 , indexEqual -1 , h);
		}else{
			return NULL;
		}
	}

	char * getValue(char * line , HeapHandler h){
		char * formated = trim(line , h);
		int indexEqual = indexOfChar(formated , '=');
		if(indexEqual > 0)
			return subString(formated , indexEqual +1 , (int) strlen(formated)-1 , h);
		else
			return NULL;
	}

	/*
	FILE * getFile(char * file){
		return fopen(file , "r");
	}
	*/