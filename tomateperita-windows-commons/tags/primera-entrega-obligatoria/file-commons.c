/**
 * file-commons.h
 * Funciones para el manejo de archivos externos
 */

#include "file-commons.h"

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
		
		newFile = CreateFileW((LPCSTR)filename,
			GENERIC_ALL,
			FILE_SHARE_READ,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		return newFile;
	}


	
	void loadConfiguration(char * configFile , HeapHandler h , void (*processKeysAndValues)(char * , char *)){
		FILE * file = getFile(configFile);
		char lineBuffer[FILE_LINE_LENGTH];
		if(file == NULL){
			//TODO: sacar este printline
			return;
		}

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
		if(indexEqual > 0){
			return subString(formated , indexEqual +1 , strlen(formated)-1 , h);
		}else{
			return NULL;
		}
	}


	FILE * getFile(char * file){
		return fopen(file , "r");
	}