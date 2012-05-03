/*
 * fss-mps-services.c
 *
 *  Created on: 10/07/2011
 *      Author: gonzalo
 */
#include <stdlib.h>
#include <string.h>

#include "linux-commons.h"
#include "linux-commons-remote-services.h"
#include "linux-commons-list.h"
#include "linux-commons-mps.h"

#include "fss-mps-entrypoint.h"
#include "fss-fs-management.h"
#include "fss-utils.h"




	void executeOperation(ListenSocket ls , MpsRequest * request , RuntimeErrorValidator * validator){

		if(isOperationExistFile(request)){
			executeOperationIsExistingFile(ls , request , validator);
		}else if(isOperationAssignAndGetSectors(request)){
			executeOperationAssignAndGetSectors(ls , request, validator);
		}else if(isOperationCreateFile(request)){
			executeOperationCreateFile(ls , request, validator);
		}else if(isOperationDeleteFile(request)){
			executeOperationDeleteFile(ls , request, validator);
		}else if(isOperationFormat(request)){
			executeOperationFormat(ls , request, validator);
		}else if(isOperationListDirectory(request)){
			executeOperationListDirectory(ls , request, validator);
		}else if(isOperationGetFileInformation(request)){
			executeOperationGetFileInformation(ls , request, validator);
		}else {
			executeInvalidOperation(ls , request, validator);
		}
	}



	void executeOperationIsExistingFile(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator){

		Iterator * iterator = buildIterator(request->arguments);
		char * vdaName = hasMoreElements(iterator)? next(iterator) : NULL;
		char * fileName = hasMoreElements(iterator)? next(iterator) : NULL;

		if(vdaName == NULL || fileName == NULL){
			char * msg = concatAll(3, "Ejecutando operacion 'IsExistFile' con argumentos MPS nulos: VdaName o FileName. DescriptorId: '" ,
					strndup(request->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID) , "'");
			setError(validator , msg);
			replyValidationError(ls ,request, validator);

			return;
		}

		Bool existence = isExistingFile(vdaName , fileName);
		List retValues = buildStringsListWithValues(1, itoa(existence));

		MpsResponse * response = buildMpsResponse(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , retValues);

		sendMpsResponse(response , ls , validator);
	}





	void executeOperationGetFileInformation(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator){
		Iterator * iterator = buildIterator(request->arguments);
		char * vdaName = hasMoreElements(iterator)? next(iterator) : NULL;
		char * fileName = hasMoreElements(iterator)? next(iterator) : NULL;

		if(vdaName == NULL || fileName == NULL){
			char * msg = concatAll(3, "Ejecutando operacion 'GetFileInformation' con argumentos MPS nulos: VdaName o FileName. DescriptorId: '" ,
					strndup(request->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID) , "'");
			setError(validator , msg);
			replyValidationError(ls ,request, validator);

			return;
		}

		FileInformation * info = getFileInformation(vdaName , fileName , validator);

		MpsResponse * resp = NULL;

		if(!hasError(validator)){

			char * fileSize = ltoa(info->size);
			char * fileSectors = serializeList(info->sectors , ",");

			/*
			 *  Si el archivo no tiene sectores mandamos un null.
			 */
			if(fileSectors == NULL)
				fileSectors = MPS_FIELD_ARGUMENT_NULL;

			List retValues = buildStringsListWithValues(2 , fileSize , fileSectors);

			resp = buildMpsResponse(request->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , retValues);

			sendMpsResponse(resp , ls , validator);
		}else{
			replyValidationError(ls , request, validator);
		}

	}




	void executeOperationDeleteFile(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator){
		Iterator * iterator = buildIterator(request->arguments);
		char * vdaName = hasMoreElements(iterator)? next(iterator) : NULL;
		char * fileName = hasMoreElements(iterator)? next(iterator) : NULL;

		if(vdaName == NULL || fileName == NULL){
			char * msg = concatAll(3, "Ejecutando operacion 'DeleteFile' con argumentos MPS nulos: VdaName o FileName. DescriptorId: '" ,
					strndup(request->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID) , "'");
			setError(validator , msg);
			return;
		}

		deleteFile(vdaName , fileName , validator);

		MpsResponse * resp = NULL;
		if(!hasError(validator)){
			resp = buildMpsResponse(request->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , NULL);

			sendMpsResponse(resp , ls , validator);
		}else{
			replyValidationError(ls ,request, validator);
		}

	}




	void executeOperationCreateFile(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator){
		Iterator * iterator = buildIterator(request->arguments);
		char * vdaName = hasMoreElements(iterator)? next(iterator) : NULL;
		char * fileName = hasMoreElements(iterator)? next(iterator) : NULL;

		if(vdaName == NULL || fileName == NULL){
			char * msg = concatAll(3, "Ejecutando operacion 'CreateFile' con argumentos MPS nulos: VdaName o FileName. DescriptorId: '" ,
					strndup(request->descriptorId ,MPS_FIELD_LENGTH_DESCRIPTOR_ID) , "'");
			setError(validator , msg);
			replyValidationError(ls ,request, validator);
			return;
		}

		createFile(vdaName , fileName , validator);

		MpsResponse * resp = NULL;

		if(!hasError(validator)){
			resp = buildMpsResponse(request->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , NULL);

			sendMpsResponse(resp , ls , validator);
		}else{
			replyValidationError(ls ,request, validator);
		}

	}




	void executeOperationFormat(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator){
		Iterator * iterator = buildIterator(request->arguments);
		char * vdaName = hasMoreElements(iterator)? next(iterator) : NULL;
		char * sectorsCount = hasMoreElements(iterator)? next(iterator) : NULL;

		if(vdaName == NULL || sectorsCount == NULL){
			char * msg = concatAll(3, "Ejecutando operacion 'Format' con argumentos MPS nulos: VdaName. DescriptorId: '" ,
					strndup(request->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID) , "'");
			setError(validator , msg);
			replyValidationError(ls ,request, validator);
			return;
		}

		int secCount = atoi(sectorsCount);

		format(vdaName , secCount , validator);

		MpsResponse * resp = NULL;
		if(!hasError(validator)){
			resp = buildMpsResponse(request->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , NULL);

			sendMpsResponse(resp , ls , validator);
		}else {
			replyValidationError(ls , request, validator);
		}

	}





	void executeOperationAssignAndGetSectors(ListenSocket ls , MpsRequest * request , RuntimeErrorValidator * validator){

		Iterator * iterator = buildIterator(request->arguments);
		char * vdaName = hasMoreElements(iterator)? next(iterator) : NULL;
		char * fileName = hasMoreElements(iterator)? next(iterator) : NULL;

		if(vdaName == NULL || fileName == NULL){
			char * msg = concatAll(3, "Ejecutando operacion 'AssignAndGetSectors' con argumentos MPS nulos: VdaName. DescriptorId: '" ,
					strndup(request->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID) , "'");
			setError(validator , msg);
			replyValidationError(ls ,request, validator);
			return;
		}

		int firstSector = getFreeSector(vdaName , validator);
		if(hasError(validator)){
			replyValidationError(ls ,request, validator);
			return;
		}

		int secondSector = getFreeSector(vdaName , validator);
		if(hasError(validator)){
			replyValidationError(ls ,request, validator);
			return;
		}

		addSectorsToFile(vdaName , fileName , firstSector , secondSector , validator);

		if(hasError(validator)){
			replyValidationError(ls , request , validator);
			return;
		}

		List freeSectors = buildStringsListWithValues(1 ,
				serializeList(buildStringsListWithValues(2 , itoa(firstSector) , itoa(secondSector)) , ","));

		MpsResponse * response = buildMpsResponse(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , freeSectors);

		sendMpsResponse(response , ls , validator);
	}






	void executeOperationListDirectory(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator){
		Iterator * iterator = buildIterator(request->arguments);
		char * vdaName = hasMoreElements(iterator)? next(iterator) : NULL;

		if(vdaName == NULL){
			char * msg = concatAll(3, "Ejecutando operacion 'ListDirectory' con argumentos MPS nulos: VdaName. DescriptorId: '" ,
					strndup(request->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID) , "'");
			setError(validator , msg);
			replyValidationError(ls ,request, validator);
			return;
		}

		List files = getAllFiles(vdaName , validator);

		if(hasError(validator)){
			replyValidationError(ls , request, validator);
			return ;
		}

		char * serializedList = serializeList(files , ",");
		List returnValues = buildStringsListWithValues(1, serializedList);
		MpsResponse * response = buildMpsResponse(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , returnValues);

		sendMpsResponse(response , ls , validator);
	}




	void executeInvalidOperation(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator){}




	Bool isOperationExistFile(MpsRequest * req){
		return equalsStrings(req->operationName , FSS_FILE_CHECK_EXISTENCE);
	}
	Bool isOperationGetFileInformation(MpsRequest * req){
		return equalsStrings(req->operationName , FSS_FILE_INFORMATION);
	}
	Bool isOperationDeleteFile(MpsRequest * req){
		return equalsStrings(req->operationName , FSS_FILE_DELETION);
	}
	Bool isOperationCreateFile(MpsRequest * req){
		return equalsStrings(req->operationName , FSS_FILE_CREATION);
	}
	Bool isOperationListDirectory(MpsRequest * req){
		return equalsStrings(req->operationName , FSS_DIRECTORY_LISTING);
	}
	Bool isOperationFormat(MpsRequest * req){
		return equalsStrings(req->operationName , FSS_FORMAT);
	}
	Bool isOperationAssignAndGetSectors(MpsRequest * req){
		return equalsStrings(req->operationName , FSS_SECTORS_ASSIGN_AND_GET);
	}


