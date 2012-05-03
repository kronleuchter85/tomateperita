/*
 *  file-config.c
 *  
 *
 *  Created by Fernando Nino on 4/11/11.
 *
 *  
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <dirent.h>

#include "linux-commons-file.h"
#include "linux-commons-logging.h"


	Bool isPropertiesCommentedOrBlanckLine(char * line);

	void loadConfiguration(File * file , void (*processKeysAndValues)(char * , char * , void *)){
		parseKeyAndValueFile(file , isPropertiesCommentedOrBlanckLine , processKeysAndValues , NULL);
	}

	char * getKey(char * line ){
		char * formated = trim(line);
		int indexEqual = getIndexOf(formated , '=');
		if(indexEqual > 0){
			return getSubString(formated , 0 , indexEqual -1);
		}else{
			return NULL;
		}
	}

	char * getValue(char * line){
		char * formated = trim(line);
		int indexEqual = getIndexOf(formated , '=');
		if(indexEqual > 0){
			return getSubString(formated , indexEqual +1 , strlen(formated)-1);
		}else{
			return NULL;
		}
	}

	Bool existsFile(char * name){
		return (fopen(name , "r") != NULL);
	}

	Bool createDirectory(char * name){
		return !mkdir(name , S_IRWXU);
	}



	void parseKeyAndValueFile(File * file ,
			Bool (*anIgnoreLineCriteria)(char *),
			void (*processKeysAndValuesCriteria)(char * , char * , void *),
			void * storageObject){


		char lineBuffer[SYSTEM_FILE_LINE_LENGTH];
		cleanMemory(lineBuffer,SYSTEM_FILE_LINE_LENGTH);
		while(fgets(lineBuffer , SYSTEM_FILE_LINE_LENGTH , file)){
			char * line = trim(lineBuffer);

			if(anIgnoreLineCriteria == NULL || !anIgnoreLineCriteria(line)){
				char * key = getKey(line);
				char * value = getValue(line);

				if(key != NULL && value != NULL)
					processKeysAndValuesCriteria(key , value , storageObject);
			}
		}
		cleanMemory(lineBuffer, SYSTEM_FILE_LINE_LENGTH);
	}


	void parseFileAndModifyValueForKey(File * file , char * aKey , char * newValue ,
			Bool (*anIgnoreLineCriteria)(char *),
			void (*modifyKeysAndValuesCriteria)(char * , char * , void * , File * ),
			void * storageObject){


		char lineBuffer[SYSTEM_FILE_LINE_LENGTH];
		cleanMemory(lineBuffer,SYSTEM_FILE_LINE_LENGTH);
		while(fgets(lineBuffer , SYSTEM_FILE_LINE_LENGTH , file)){
			char * line = trim(lineBuffer);

			if(anIgnoreLineCriteria == NULL || !anIgnoreLineCriteria(line)){
				char * key = getKey(line);
				char * value = getValue(line);

				if(key != NULL && value != NULL){

					if(equalsStrings(key , aKey)){

						/*
						 * Inicializamos la posicion donde fue encontrada la linea en cuestion
						 * mediante la posicion actual del archivo menos la suma del largo de la linea mas
						 * un caracter '\n' de fin de linea.
						 */
						long int currentPossition = ftell(file);
						long int fileStartLinePossition = currentPossition - (strlen(value) + 1);

						/*
						 * Pocisionamos el cursor del archivo en la posicion correspondiente
						 * para realizar la escritura
						 */
						fseek(file , fileStartLinePossition , SEEK_SET);

						//escritura...
						modifyKeysAndValuesCriteria(value , newValue, storageObject , file);

						/*
						 * volvemos a posicionarnos donde estabamos
						 */
						fseek(file , currentPossition , SEEK_SET);
					}
				}
			}
		}
		cleanMemory(lineBuffer, SYSTEM_FILE_LINE_LENGTH);
	}



	Bool isPropertiesCommentedOrBlanckLine(char * line){
		return startsWith( line, "#") && !equalsStrings(line , "");
	}

	File * openFile(const char * filename){
		return fopen(filename , "r+");
	}

	File * openOrCreateFile(const char * filename){
		return fopen(filename , "a");
	}

	void removeFile(const char * filename){
		remove(filename);
	}

	Bool removeDirectory(char * dirname){
		//return !rmdir(dirname);

		char * cmd = concatAll(2 , "rm -r " , dirname);
		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(3 , "Ejecutando comando nativo del sistema: '" , cmd , "'"));
		return !system(cmd);
	}

	void closeFile(File * file){
		fclose(file);
	}

	void insertEntry(char * key , char * value , File * file){
		const char * content = concatAll(3 , key , "=" , value);
		insertLine(content, file);
	}

	void insertLine(const char * line , File * file){
		int inserted = fwrite(line , sizeof(char) , strlen(line) , file);
		if(isDebugEnabled()) logDebug("linux-commons" , concatAll(3 , "Se han insertado " , itoa(inserted) , " bytes en el archivo"));
		fputc('\n' , file);
	}


	void updateEntry(char * key , char * value ,
			void (*modifyKeysAndValuesCriteria)(char * , char * , void * , File * ) ,
			File * file){

		parseFileAndModifyValueForKey(file, key , value ,
				isPropertiesCommentedOrBlanckLine ,
				modifyKeysAndValuesCriteria , NULL);
	}



	File * createOrTruncateFile(const char * filepath){
		return fopen(filepath , "w+");
	}



	List getDirectoryFiles(char * path , Bool (*anIncludeCriteria)(char *)){

		Bool isSameFile(char * f1 , char * f2){
			return equalsStrings(f1 , f2);
		}

		Bool sort(char * f1 , char * f2){
			return ( strcmp(f1 , f2) < 0 );
		}

		void listing(char * e){
			if(e != NULL) logInfo( "linux-commons" , e);
		}

		FilesIterator * dp;
		List fileNames = createList(
				(void (*)(void*)) listing ,
				(Bool (*)(void *, void *))isSameFile ,
				(Bool (*)(void *, void *))sort);

		DIR * myDir = opendir(path);

		while( (dp = readdir(myDir)) != NULL ) {
			char * fileName = dp->d_name;
			if(anIncludeCriteria == NULL || anIncludeCriteria(fileName)){
				if(isDebugEnabled())
					logDebug("linux commons" , concatAll(2 , "hay que incluir: " , fileName));
				addNode(fileNames , cloneString(fileName));
			}
		}

		closedir(myDir);

		return fileNames;
	}

	char * getStartNameMatchingFile(char * path , char * pattern){
		FilesIterator * dp;
		Directory * myDir = opendir(path);

		while( (dp = readdir(myDir)) != NULL ) {
			if(startsWith(dp->d_name , pattern)){
				closedir(myDir);
				return cloneString(dp->d_name);
			}
		}

		closedir(myDir);
		return NULL;
	}

	FileInformation * buildFileInformation(char * v , char * f){
		FileInformation * info = (FileInformation*) malloc(sizeof(FileInformation));
		info->name = f;
		info->vda = v;
		return info;
	}

