/*
 * fss-management.c
 *
 *  Created on: 23/06/2011
 *      Author: gonzalo
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linux-commons.h"
#include "linux-commons-file.h"
#include "linux-commons-logging.h"
#include "linux-commons-errors.h"

#include "fss-fs-management.h"
#include "fss-configuration.h"
#include "fss-utils.h"




	void addSectorToFile(char * vdaName , char * fileName , int sector , RuntimeErrorValidator * v);
	char * getAbsolutePathToFile(char * vdaName , char * fileName);
	char * getPhysicalFileFromSectors(int sectorR , int sectorA);
	char * getPhysicalFileFromSectorInPath(char * path , int absoluteSector);
	long getNewFileSize(long size);
	int findSectorInFlag(char flag , File * f);
	Bool isValidSectorNumber(int sector);
	void addSectorAndUpdateSize(char * vdaName , char * fileName , int sector1 , RuntimeErrorValidator *  validator);
	long getNewAddingSectorFileSize(long currentSize);
	long getNewReleasingSectorFileSize(long currentSize);
	void setFreeSector(char * vdaName , RuntimeErrorValidator * validator);
	int getAbsoluteSectorFromPhysicalFile(char * pFile);
	List getFileSectors(char * pathToFile);






	Bool hasFormatedStorage(char * vda){
		Bool existsVdaSpace = existsFile(getAbsolutePathToFile(vda , ""));
		return (existsVdaSpace);
	}


	/*
	 * Creacion del archivo
	 */
	void createFile(char * vdaName , char * fileName , RuntimeErrorValidator * validator){
		char * filePath = getAbsolutePathToFile(vdaName , fileName);

		if(!hasFormatedStorage(vdaName)){
			setError(validator , "No existe unidad formateada");
			return;
		}

		if(!isExistingFile(vdaName , fileName)){

			if(isDebugEnabled())
				debug(concatAll(2 , " Creando archivo " , filePath));

			createDirectory(filePath);

			if(isDebugEnabled())
				debug(concatAll(2 , " Seteando tamaño al archivo " , filePath));

			updateFileSize(vdaName , fileName , 0 , validator);

		}else{
			setError(validator , concatAll(2 , "Se esta tratando de crear un archivo ya existente: " , filePath));
		}
	}




	/*
	 * Existencia del archivo
	 */
	Bool isExistingFile(char * vdaName , char * fileName ){

		if(!hasFormatedStorage(vdaName)){
			return FALSE;
		}

		char * filePath = getAbsolutePathToFile(vdaName , fileName);

		if(isDebugEnabled())
			debug(concatAll(2, "Comprobando existencia del archivo " , filePath));

		Bool existence = existsFile(filePath);

		if(isDebugEnabled()){
			debug( (existence)? concatAll(2, "El archivo existe: ", filePath) : concatAll(2 , "El archivo no existe " , filePath));
		}
		return existence;
	}






	FileInformation * getFileInformation(char * vdaName, char * fileName, RuntimeErrorValidator * validator){

		if(!hasFormatedStorage(vdaName)){
			setError(validator , "No existe unidad formateada");
			return NULL;
		}

		char * filePath = getAbsolutePathToFile(vdaName , fileName);

		if(!isExistingFile(vdaName , fileName)){
			setError(validator , concatAll(2, "Se esta tratando de obtener informacion de un archivo que no existe: " , filePath));
			return NULL;
		}


		FileInformation * fileInformation = buildFileInformation(vdaName , fileName);
		char * fileSizeName = getStartNameMatchingFile(filePath , FILE_NAME_SIZE);

		char * fileSize = getSubString(fileSizeName , getIndexOf(fileSizeName , '-') +1, strlen(fileSizeName));
		if(isDebugEnabled())
			debug(concatAll(2 , "Tamaño descubierto del archivo: " , fileSize));

		List sectorFiles = getFileSectors(filePath );
		List effectiveSectors = buildSringsList();
		Iterator * sectorFilesIterator = buildIterator(sectorFiles);
		while(hasMoreElements(sectorFilesIterator)){
			char * physicalFile = next(sectorFilesIterator);
			int sector = getAbsoluteSectorFromPhysicalFile(physicalFile);
			if( sector >= 0)
			addNode(effectiveSectors , itoa(sector));

			free(physicalFile);
		}

		fileInformation->sectors = effectiveSectors;
		fileInformation->size = atol(fileSize);

		if(isDebugEnabled()){
			debug(concatAll(5, "Sectores de ", vdaName , "/" , fileName ,": "  ));
			debug(concatAll(6, "Tamano de ", vdaName , "/" , fileName ,": "  , ltoa( fileInformation->size) ));
		}


		free(filePath);
		free(fileSizeName);
		free(fileSize);
		free(sectorFilesIterator);
		free(sectorFiles);

		return fileInformation;
	}




	void freeSector(char * vdaName , int sector , RuntimeErrorValidator * v){

		if(!hasFormatedStorage(vdaName)){
			setError(v , "No existe unidad formateada");
			return;
		}

		List files = getAllFiles(vdaName , v);
		Iterator * filesIterator = buildIterator(files);

		Iterator * sectorsIterator = NULL;
		FileInformation * info = NULL;

		while( hasMoreElements(filesIterator)){
			char * fileName = next(filesIterator);

			info = getFileInformation(vdaName , fileName , v);
			sectorsIterator = buildIterator(info->sectors);

			while( hasMoreElements(sectorsIterator)){
				char * sectorNumber = next(sectorsIterator);
				if( equalsStrings(itoa(sector) , sectorNumber)){

					char * fileToDelete = getAbsolutePathToFile(vdaName , fileName);
					char * fileSectorName = getPhysicalFileFromSectorInPath(fileToDelete , sector);

					if(fileSectorName != NULL){
						fileToDelete = concatAll(3 , fileToDelete , "/" , fileSectorName);
						removeFile(fileToDelete);
					}

					if(isDebugEnabled())
						debug(concatAll(2 , "Eliminando archivo: " , fileToDelete));

					long currentSize = getCurrentFileSize(vdaName , fileName , v);
					setFreeSector(vdaName , v);
					updateFileSize(vdaName , fileName , getNewReleasingSectorFileSize(currentSize) , v);

					return;
				}
			}
		}

		/*
		 * Si llego a este punto no pudo eliminar el archivo
		 * del sector porq no existe
		 */
		setError(v , concatAll(3 , "No se ha podido eliminar el sector '" ,	itoa(sector) , "'"));
	}



	void freeSectors(char * vdaName, List sectors, RuntimeErrorValidator * validator){

		if(!hasFormatedStorage(vdaName)){
			setError(validator , "No existe unidad formateada");
			return;
		}

		Iterator * iteSectors = buildIterator(sectors);

		while(hasMoreElements(iteSectors)){
			int * sector = next(iteSectors);
			freeSector(vdaName , *sector , validator);
		}
	}



	void addSectorsToFile(char * vdaName , char * fileName ,
			int sector1 , int sector2, RuntimeErrorValidator * validator){

		if(!hasFormatedStorage(vdaName)){
			setError(validator , "No existe unidad formateada");
			return;
		}

		if(!isExistingFile(vdaName , fileName)){
			char * path = getAbsolutePathToFile(vdaName , fileName);
			setError(validator , concatAll(2, "Tratando de agregar sectores a un archivo inexistente: " , path));
			return;
		}

		if(isValidSectorNumber(sector1)){
			addSectorToFile(vdaName , fileName, sector1, validator);
//			addSectorAndUpdateSize(vdaName , fileName, sector1, validator);
		}
		if(isValidSectorNumber(sector2)){
			addSectorToFile(vdaName , fileName, sector2, validator);
//			addSectorAndUpdateSize(vdaName , fileName, sector2, validator);
		}
	}


	void addSectorAndUpdateSize(char * vdaName , char * fileName , int sector1 , RuntimeErrorValidator *  validator){

		if(!hasFormatedStorage(vdaName)){
			setError(validator , "No existe unidad formateada");
			return;
		}

		if(!isExistingFile(vdaName , fileName)){
			char * path = getAbsolutePathToFile(vdaName , fileName);
			setError(validator , concatAll(2, "Tratando de agregar sectores a un archivo inexistente: " , path));
			return;
		}

		addSectorToFile(vdaName , fileName , sector1 , validator);
		long currentSize = getCurrentFileSize(vdaName, fileName , validator);
		if(isDebugEnabled())
			debug(concatAll(2 , "Current Size: " , ltoa(currentSize)));
		long newSize = getNewAddingSectorFileSize(currentSize);
		if(isDebugEnabled())
			debug(concatAll(2 , "New Size: " , ltoa(newSize)));
		updateFileSize(vdaName , fileName , newSize , validator);
	}


	void format(char * vdaName , int sectorsCount , RuntimeErrorValidator * validator){
		char * pathToVda = getAbsolutePathToFile(vdaName , "");
		if(existsFile(pathToVda)){

			if(isDebugEnabled())
				debug(concatAll(2, "Elininando ruta VDA: " , pathToVda));

			Bool success = removeDirectory(pathToVda);
			if(!success){
				setError(validator , concatAll(2 , "Ocurrio un error al intentar eliminar el directorio del VDA: " , pathToVda));
				return ;
			}
		}

		/*
		 * Procedemos a crear el directorio del VDA
		 * y crear la tabla de sectores libres.
		 */
		Bool success = createDirectory(pathToVda);
		if(!success){
			setError(validator , concatAll(2 , "Ocurrio un error al intentar crear el directorio del VDA: " , pathToVda));
			return;
		}

		createFreeBitVector(vdaName , sectorsCount , validator);
	}


	/*
	 * Eliminacion del archivo
	 */
	void deleteFile(char * vdaName , char* fileName , RuntimeErrorValidator * validator ){

		if(!hasFormatedStorage(vdaName)){
			setError(validator , "No existe unidad formateada");
			return;
		}

		char * path = getAbsolutePathToFile(vdaName , fileName);

		if(existsFile(path)){

			if(isDebugEnabled())
				debug(concatAll(4 , "Eliminando archivo " , vdaName , "/" , fileName));


			FileInformation * info = getFileInformation(vdaName , fileName , validator);
			if(hasError(validator)){
				return;
			}

			Iterator * sectorsIterator = buildIterator(info->sectors);
			while(hasMoreElements(sectorsIterator)){
				int sector = atoi(next(sectorsIterator));
				freeSector(vdaName , sector , validator);

			}

			if(hasError(validator)){

				char * errormessage = concatAll(2,
						"Posible archivo corrupto. Ocurrio un error al eliminar los sectores del archivo " ,
						getAbsolutePathToFile(vdaName , fileName));
				setError(validator , errormessage);
				return;
			}

			Bool deletionSuccess = removeDirectory(path);

			if(!deletionSuccess)
				setError(validator , concatAll(2 , "No se pudo eliminar el archivo (directorio): " , path));
		}else{
			setError(validator , concatAll(2 ,"Se esta tratando de eliminar un archivo que no existe: " , path));
		}
	}



	void updateFileSize(char * vdaName , char * fileName , long fileSize , RuntimeErrorValidator * validator){

		if(!hasFormatedStorage(vdaName)){
			setError(validator , "No existe unidad formateada");
			return;
		}

		char * filePath = getAbsolutePathToFile(vdaName , fileName) ;
		char * fileSizeName = concatAll(2 , FILE_NAME_SIZE, ltoa(fileSize));
		char * fileToCreate = concatAll(3 , filePath , "/" , fileSizeName);

		char * currentSizeFile = getStartNameMatchingFile(filePath , FILE_NAME_SIZE);

		if(currentSizeFile != NULL){

			char * fileToDelete = concatAll(3, filePath , "/" , currentSizeFile);

			if(isDebugEnabled())
				debug(concatAll(2 , "Eliminando el archivo size anterior: " , fileToDelete));

			removeFile(fileToDelete);
		}

		if(isDebugEnabled())
			debug(concatAll(2, "Creando el archivo  size: " , fileToCreate));

		File * file = createOrTruncateFile(fileToCreate);
		closeFile(file);
	}



	void createFreeBitVector(char * vdaName , int sectorsCount, RuntimeErrorValidator * validator){

		if(!hasFormatedStorage(vdaName)){
			setError(validator , "No existe unidad formateada");
			return;
		}

		char * fileBitsPath = getAbsolutePathToFile(vdaName , FILE_NAME_FREE_SECTORS);

		File * file = createOrTruncateFile(fileBitsPath);

		if(file == NULL){
			setError(validator , "No se pudo crear (o truncar) el archivo de sectores libres");
			return;
		}

		int i; for( i=0 ; i<sectorsCount ; i++){
			int written = fwrite("1" , sizeof(char) , strlen("1") , file);
			if( written <= 0 && i<sectorsCount){
				setError(validator , "no se pudo terminar de inicializar el contenido del bitvector");
				break;
			}
		}
		closeFile(file);
	}






	int getFreeSector(char * vdaName , RuntimeErrorValidator * validator){

		if(!hasFormatedStorage(vdaName)){
			setError(validator , "No existe unidad formateada");
			return -1;
		}

		char * file = getAbsolutePathToFile(vdaName , FILE_NAME_FREE_SECTORS);

		File * f = openFile(file);
		if(file == NULL){
			setError(validator , "No se pudo abrir el archivo de sectores libres");
			return -1;
		}

		int sectorId = findSectorInFlag('1' , f);

		if(sectorId == -1){
			setError(validator , "no existen mas sectores libres para asignar");
		}else{
			fseek(f , sectorId , SEEK_SET);
			putc('0' , f);
		}

		closeFile(f);
		return sectorId+1;
	}


	void setFreeSector(char * vdaName , RuntimeErrorValidator * validator){

		if(!hasFormatedStorage(vdaName)){
			setError(validator , "No existe unidad formateada");
			return;
		}

		char * file = getAbsolutePathToFile(vdaName , FILE_NAME_FREE_SECTORS);

		File * f = openFile(file);
		if(file == NULL){
			setError(validator , "No se pudo abrir el archivo de sectores libres");
			return ;
		}

		int sectorId = findSectorInFlag('0' , f);

		if(sectorId == -1){
			setError(validator , "no existen mas sectores libres para asignar");
		}else{
			fseek(f , sectorId , SEEK_SET);
			putc('1' , f);
		}

		closeFile(f);
	}


	void addSectorToFile(char * vdaName , char * fileName , int sector , RuntimeErrorValidator * v){

		if(!hasFormatedStorage(vdaName)){
			setError(v , "No existe unidad formateada");
			return;
		}

		char * logicalFilePath = getAbsolutePathToFile(vdaName , fileName);

		if(!isExistingFile(vdaName , fileName)){
			setError(v , concatAll(2, "Tratando de agregar sectores a un archivo inexistente: " , logicalFilePath));
			return;
		}

		int nextRelativeSectorIndex = 0;

		List allFiles = getFileSectors(logicalFilePath);
		if(allFiles != NULL )
			nextRelativeSectorIndex = allFiles->size + 1;

		char * sector1File = concatAll(3 , logicalFilePath , "/" , getPhysicalFileFromSectors(nextRelativeSectorIndex , sector));

		File * f = createOrTruncateFile(sector1File);
		if(f == NULL){
			setError(v , concatAll(2 , "No se pudo crear el archivo " , sector1File));
			return;
		}else {
			if(isDebugEnabled())
				debug( concatAll( 2 , "Asignando sector " , itoa(sector)));
			closeFile(f);
		}
	}



	List getAllFiles(char * vdaName, RuntimeErrorValidator * validator){

		if(!hasFormatedStorage(vdaName)){
			setError(validator , "No existe unidad formateada");
			return NULL;
		}

		Bool includeCriteria(char * n){
			debug(concatAll(3 , "evaluando: '" , n , "'"));
			return !equalsStrings( trim(n) , ".")
					&& !equalsStrings( trim(n) , "..")
					&& !equalsStrings( trim(n) , FILE_NAME_FREE_SECTORS)
					&& !equalsStrings(trim(n) , "");
		}

		char * path = getAbsolutePathToFile(vdaName , "");

		if(!existsFile(path)){
			setError(validator , concatAll(2, "El directorio no existe: " , path));
			return NULL;
		}

		debug(concatAll(2, "obteniendo todos los archivos de: ", path));

		return getDirectoryFiles(path , includeCriteria, NULL);
	}




	/**
	 * ****************************** Funciones de soporte ******************************************
	 */



	long getCurrentFileSize(char * vdaName , char * fileName , RuntimeErrorValidator * v){

		if(!hasFormatedStorage(vdaName)){
			setError(v , "No existe unidad formateada");
			return -1;
		}

		char * filePath = getAbsolutePathToFile(vdaName , fileName) ;
		char * fileSize = getStartNameMatchingFile(filePath , FILE_NAME_SIZE);
		char * strSize = getSubString(fileSize , getIndexOf(fileSize , '-')+1 , strlen(fileSize));

		if(isDebugEnabled())
			debug(concatAll(2 , "Tamaño actual del archivo: " , strSize));

		return atol(strSize);
	}


	Bool isValidSectorNumber(int sector){
		return (sector >= 0);
	}


	int findSectorInFlag(char flag , File * f){
		char c;
		int i; for( i=0 ; (c = getc(f)) != EOF ; i++){
			if(c == flag){
				return i;
			}
		}
		return -1;
	}

	long getNewReleasingSectorFileSize(long currentSize){
		return currentSize - SECTOR_SIZE;
	}

	long getNewAddingSectorFileSize(long currentSize){
		return currentSize + SECTOR_SIZE;
	}

	char * getAbsolutePathToFile(char * vdaName , char * fileName){
		return concatAll(5, getDataDirectory() , "/" , vdaName , "/" , fileName);
	}


	/**
	 * Va a recibir un numero de sector x y otro z tiene q devolver
	 * el nombre del archivo correspondiente. P/Ej. 0000x-sectorz.
	 */
	char * getPhysicalFileFromSectors(int relativeSector , int absoluteSector){
		return concatAll(3, serializeInt(relativeSector , 5) , FILE_NAME_SECTOR , itoa(absoluteSector));
	}

	/**
	 * Va a recibir un nombre tipo '0000Y-sectorx'
	 * donde x es el numero que me interesa.
	 */
	int getAbsoluteSectorFromPhysicalFile(char * pFile){
		if(pFile == NULL)
			return -1;

		int indexOf = getIndexOf(pFile , '-');
		indexOf += getIndexOf(pFile + indexOf + 1 , '-');
		char * sectorNumber = strdup(pFile + indexOf +2);
		return atoi(sectorNumber);
	}


	List getFileSectors(char * pathToFile){
		Bool includeCriteria(char * n){
			return !equalsStrings(trim(n) , ".")
					&& !equalsStrings(trim(n) , "..")
					&& !startsWith(trim(n) , FILE_NAME_SIZE);
		}

		/*
		 * El criterio de ordenacion es el orden relativo
		 */
		Bool sort(char * f1 , char * f2){
			char * nbr1 = getSubString(f1 , 0 , getIndexOf(f1 , '-') -1);
			char * nbr2 = getSubString(f2 , 0 , getIndexOf(f2 , '-') -1);

			return atoi(nbr1)<atoi(nbr2);
		}

		return getDirectoryFiles(pathToFile , includeCriteria , sort);
	}

	char * getPhysicalFileFromSectorInPath(char * path , int absoluteSector){

		Iterator * ite = buildIterator(getFileSectors(path));
		while(hasMoreElements(ite)){
			char * fileName = next(ite);
			if(endsWith(fileName , concat(FILE_NAME_SECTOR , itoa(absoluteSector))) ){
				return fileName;
			}
		}

		return NULL;
	}
