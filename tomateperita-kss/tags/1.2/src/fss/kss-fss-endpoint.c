/*
 * kss-fss-operations.c
 *
 *  Created on: 10/07/2011
 *      Author: gonzalo
 */

#include <stdlib.h>

#include "linux-commons.h"
#include "linux-commons-mps.h"
#include "linux-commons-list.h"
#include "linux-commons-remote-services.h"
#include "linux-commons-file.h"

#include "kss-state.h"



	ThreadMutex fileSystemCallingOperationMutex = PTHREAD_MUTEX_INITIALIZER;


	Bool callFssOperationIsExistingFile(char * vdaName , char * fileName , RuntimeErrorValidator * validator){

		List arguments = buildStringsListWithValues(2 , vdaName , fileName);
		MpsRequest * req = buildMpsRequest(generateRandomKey(16) , FSS_FILE_CHECK_EXISTENCE , arguments);

		lockThreadMutex(&fileSystemCallingOperationMutex);

		MpsResponse * resp = sendAndReceiveMpsMessages(getFileSystemConnection() , req , validator);

		unlockThreadMutex(&fileSystemCallingOperationMutex);

		Iterator * ite = buildIterator(resp->returnValues);

		if(equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){

			char * retValue = hasMoreElements(ite)? next(ite) : NULL;
			if(retValue != NULL){
				return atoi(retValue);
			}else{
				setError(validator , "Ocurrio un error, al invocar la operacion 'IsExistingFile'. Los resultados no tienen el formato esperado");
				return FALSE;
			}
		}else{
			char * retValue = hasMoreElements(ite)? next(ite) : NULL;
			setError(validator , retValue);
			return FALSE;
		}
	}





	FileInformation *  callFssOperationGetFileInformation(char * vdaName , char * fileName , RuntimeErrorValidator * v){
		List arguments = buildStringsListWithValues(2 , vdaName , fileName);
		MpsRequest * req = buildMpsRequest(generateRandomKey(16) , FSS_FILE_INFORMATION , arguments);

		lockThreadMutex(&fileSystemCallingOperationMutex);

		MpsResponse * resp = sendAndReceiveMpsMessages(getFileSystemConnection() , req , v);

		unlockThreadMutex(&fileSystemCallingOperationMutex);

		Iterator * ite = buildIterator(resp->returnValues);

		if(equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			char * fileSize = hasMoreElements(ite)? next(ite) : NULL;
			char * fileSectors = hasMoreElements(ite)? next(ite) : NULL;

			if(fileSize == NULL || fileSectors == NULL){
				setError(v , "Valores nulos al llamar a 'GetFileInformation' aunque la operacion finalizo correctamente");
				return NULL;
			}

			FileInformation * fileInfo = buildFileInformation(vdaName , fileName);
			fileInfo->size = atol(fileSize);
			if(equalsStrings(fileSectors , MPS_FIELD_ARGUMENT_NULL )){
				fileInfo->sectors = NULL;
			}else{
				fileInfo->sectors = deserializeList(fileSectors );
			}

			return fileInfo;
		}else{
			char * errorMessage = hasMoreElements(ite)? next(ite):NULL;
			setError(v , errorMessage);
			return NULL;
		}
	}





	void callFssOperationCreateFile(char * vdaName , char * fileName , RuntimeErrorValidator * validator){
		List arguments = buildStringsListWithValues(2 , vdaName , fileName);
		MpsRequest * req = buildMpsRequest(generateRandomKey(16) , FSS_FILE_CREATION , arguments);

		lockThreadMutex(&fileSystemCallingOperationMutex);

		MpsResponse * resp = sendAndReceiveMpsMessages(getFileSystemConnection() , req , validator);

		unlockThreadMutex(&fileSystemCallingOperationMutex);

		if(!equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			Iterator * ite = buildIterator(resp->returnValues);
			char * errorMessage = hasMoreElements(ite) ? next(ite) : NULL;
			setError(validator , errorMessage);
		}
	}





	void callFssOperationDeleteFile(char * vdaName , char * fileName , RuntimeErrorValidator * validator){
		List arguments = buildStringsListWithValues(2 , vdaName , fileName);
		MpsRequest * req = buildMpsRequest(generateRandomKey(16) , FSS_FILE_DELETION, arguments);

		lockThreadMutex(&fileSystemCallingOperationMutex);

		MpsResponse * resp = sendAndReceiveMpsMessages(getFileSystemConnection() , req , validator);

		unlockThreadMutex(&fileSystemCallingOperationMutex);

		if(!equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			Iterator * ite = buildIterator(resp->returnValues);
			char * errorMessage = hasMoreElements(ite) ? next(ite) : NULL;
			setError(validator , errorMessage);
		}
	}





	void callFssOperationFormat(char * vdaName , int sectorsCount ,  RuntimeErrorValidator * validator){

		if(sectorsCount < 0){
			setError(validator , "Se esta intentando formatear un dispositivo con una cantidad negativa de sectores.");
			return;
		}

		List arguments = buildStringsListWithValues(2 , vdaName , itoa(sectorsCount));
		MpsRequest * req = buildMpsRequest(generateRandomKey(16) , FSS_FORMAT, arguments);

		lockThreadMutex(&fileSystemCallingOperationMutex);

		MpsResponse * resp = sendAndReceiveMpsMessages(getFileSystemConnection() , req , validator);

		unlockThreadMutex(&fileSystemCallingOperationMutex);

		if(!equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			Iterator * ite = buildIterator(resp->returnValues);
			char * errorMessage = hasMoreElements(ite) ? next(ite) : NULL;
			setError(validator , errorMessage);
		}
	}






	List callFssOperationAssignAndGetSectors(char * vdaName , char * filename , RuntimeErrorValidator * validator){

		List arguments = buildStringsListWithValues(2 , vdaName , filename);
		MpsRequest * req = buildMpsRequest(generateRandomKey(16) , FSS_SECTORS_ASSIGN_AND_GET, arguments);

		lockThreadMutex(&fileSystemCallingOperationMutex);

		MpsResponse * resp = sendAndReceiveMpsMessages(getFileSystemConnection() , req , validator);

		unlockThreadMutex(&fileSystemCallingOperationMutex);

		if(!equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			Iterator * ite = buildIterator(resp->returnValues);
			char * errorMessage = hasMoreElements(ite) ? next(ite) : NULL;
			setError(validator , errorMessage);
			return NULL;
		}else{
			Iterator * ite = buildIterator(resp->returnValues);
			char * freeSectors = hasMoreElements(ite)? next(ite):NULL;
			return deserializeList(freeSectors);
		}
	}





	List callFssOperationGetDirectoryInformation(char * vdaName , RuntimeErrorValidator * validator){


		void listing(FileInformation * f){}
		Bool sorting(FileInformation * f1 , FileInformation * f2){
			return TRUE;
		}
		Bool equally(FileInformation * f1 , FileInformation * f2){
			return equalsStrings(f1->name , f2->name);
		}


		MpsRequest * req = buildMpsRequest(
				generateRandomKey(16) ,
				FSS_DIRECTORY_LISTING,
				buildStringsListWithValues(1 , vdaName ));

		lockThreadMutex(&fileSystemCallingOperationMutex);

		MpsResponse * resp = sendAndReceiveMpsMessages(getFileSystemConnection() , req , validator);

		unlockThreadMutex(&fileSystemCallingOperationMutex);

		if(!equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			Iterator * ite = buildIterator(resp->returnValues);
			char * errorMessage = hasMoreElements(ite) ? next(ite) : NULL;
			setError(validator , errorMessage);
			return NULL;
		}else{
			Iterator * ite = buildIterator(resp->returnValues);
			char * strfiles = hasMoreElements(ite)? next(ite) : NULL;
			List files = deserializeList(strfiles);

			ite = buildIterator(files);

			List fileInfos = createList(
					(void (*)(void*))listing ,
					(Bool (*)(void*,void*))equally ,
					(Bool (*)(void*,void*))sorting );

			while(hasMoreElements(ite)){
				char * fileName = next(ite);

				/*
				 * Esto genera overhead al pedo pero por un impedimento en
				 * la implementacion actual del protocolo sobre MPS acerca
				 * de la cantidad de argumentos devueltos por una operacion
				 * se deja de este modo hasta mejorar el protocolo de comunicacion
				 * o aprobar la materia. Lo q ocurra primero.
				 */
				FileInformation * fileInfo = callFssOperationGetFileInformation(vdaName , fileName , validator);
				if(!hasError(validator))
					addNode(fileInfos , fileInfo);
			}

			return fileInfos;
		}

	}






