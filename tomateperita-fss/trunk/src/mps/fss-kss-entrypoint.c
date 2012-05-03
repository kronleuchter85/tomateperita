/*
 * fss-mps-services.c
 *
 *  Created on: 10/07/2011
 *      Author: gonzalo
 */
#include <stdlib.h>
#include <string.h>

#include "linux-commons.h"
#include "linux-commons-list.h"
#include "linux-commons-mps.h"
#include "linux-commons-file.h"
#include "linux-commons-logging.h"

#include "fss-kss-entrypoint.h"
#include "fss-fs-management.h"
#include "fss-utils.h"
#include "fss-kss-entrypoint-core.h"



	void executeOperationIsExistingFile(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator){

		Iterator * iterator = buildIterator(request->commands);
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

		MpsMessage * response = buildMpsMessage(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , retValues);

		sendMpsMessage(response , ls , validator);
	}





	void executeOperationGetFileInformation(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator){
		Iterator * iterator = buildIterator(request->commands);
		char * vdaName = hasMoreElements(iterator)? next(iterator) : NULL;
		char * fileName = hasMoreElements(iterator)? next(iterator) : NULL;

		if(vdaName == NULL || fileName == NULL){
			char * msg = concatAll(3, "Ejecutando operacion 'GetFileInformation' con argumentos MPS nulos: VdaName o FileName. DescriptorId: '" ,
					strndup(request->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID) , "'");
			setError(validator , msg);
			replyValidationError(ls ,request, validator);

			return;
		}

		List retValues = coreOperationFileInformation(vdaName , fileName , validator);

		if(hasError(validator)){
			replyValidationError(ls , request, validator);
			return;
		}

		MpsMessage * resp = buildMpsMessage(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , retValues);

		sendMpsMessage(resp , ls , validator);

	}




	void executeOperationDeleteFile(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator){
		Iterator * iterator = buildIterator(request->commands);
		char * vdaName = hasMoreElements(iterator)? next(iterator) : NULL;
		char * fileName = hasMoreElements(iterator)? next(iterator) : NULL;

		if(vdaName == NULL || fileName == NULL){
			char * msg = concatAll(3, "Ejecutando operacion 'DeleteFile' con argumentos MPS nulos: VdaName o FileName. DescriptorId: '" ,
					strndup(request->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID) , "'");
			setError(validator , msg);
			return;
		}

		deleteFile(vdaName , fileName , validator);

		if(hasError(validator)){
			replyValidationError(ls ,request, validator);
			return;
		}

		MpsMessage * resp = buildMpsMessage(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , NULL);

		sendMpsMessage(resp , ls , validator);

	}




	void executeOperationCreateFile(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator){
		Iterator * iterator = buildIterator(request->commands);
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

		if(hasError(validator)){
			replyValidationError(ls ,request, validator);
			return;
		}

		MpsMessage * resp = buildMpsMessage(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , NULL);

		sendMpsMessage(resp , ls , validator);
	}




	void executeOperationFormat(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator){
		Iterator * iterator = buildIterator(request->commands);
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

		if(hasError(validator)){
			replyValidationError(ls , request, validator);
			return ;
		}

		MpsMessage * resp = buildMpsMessage(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , NULL);

		sendMpsMessage(resp , ls , validator);
	}







	void executeOperationAssignAndGetSectors(ListenSocket ls , MpsMessage * request , RuntimeErrorValidator * validator){

		Iterator * iterator = buildIterator(request->commands);
		char * vdaName = hasMoreElements(iterator)? next(iterator) : NULL;
		char * fileName = hasMoreElements(iterator)? next(iterator) : NULL;

		if(vdaName == NULL || fileName == NULL){
			char * msg = concatAll(3, "Ejecutando operacion 'AssignAndGetSectors' con argumentos MPS nulos: VdaName. DescriptorId: '" ,
					strndup(request->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID) , "'");
			setError(validator , msg);
			replyValidationError(ls ,request, validator);
			return;
		}

		List freeSectors = coreOperationAssignAndGetSectors(vdaName , fileName , validator);

		if(hasError(validator)){
			replyValidationError(ls , request , validator);
			return;
		}

		MpsMessage * response = buildMpsMessage(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , freeSectors);

		sendMpsMessage(response , ls , validator);
	}






	void executeOperationListDirectory(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator){
		Iterator * iterator = buildIterator(request->commands);
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
		MpsMessage * response = buildMpsMessage(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , returnValues);

		sendMpsMessage(response , ls , validator);
	}




	void executeInvalidOperation(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator){}





	void executeOperationHasVdaFormat(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator){
		Iterator * iterator = buildIterator(request->commands);
		char * vdaName = hasMoreElements(iterator)? next(iterator) : NULL;

		if(vdaName == NULL){
			char * msg = concatAll(3, "Ejecutando operacion 'HasVdaFormat' con argumentos MPS nulos: VdaName. DescriptorId: '" ,
					strndup(request->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID) , "'");
			setError(validator , msg);
			replyValidationError(ls ,request, validator);
			return;
		}

		Bool formated = hasFormatedStorage(vdaName);
		formated = formated && existsFile(getAbsolutePathToFile(vdaName , FILE_NAME_FREE_SECTORS));

		if(formated){
			sendMpsMessage(buildMpsMessage(request->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , NULL) , ls , validator);
		}else{
			sendMpsMessage(buildMpsMessage(request->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_ERROR, request->operationName , NULL), ls , validator);
		}

	}




	void executeOperationUpdateFileSize(ListenSocket ls , MpsMessage * message , RuntimeErrorValidator * validator){
		Iterator * iterator = buildIterator(message->commands);

		char * vdaName = hasMoreElements(iterator)? next(iterator) : NULL;
		char * fileName = hasMoreElements(iterator)? next(iterator) : NULL;
		char * newSize = hasMoreElements(iterator)? next(iterator) : NULL;

		if(vdaName == NULL || fileName == NULL || newSize == NULL){
			char * msg = concatAll(3, "Ejecutando operacion 'AssignAndGetSectors' con argumentos MPS nulos: VdaName. DescriptorId: '" ,
					strndup(message->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID) , "'");
			setError(validator , msg);
			replyValidationError(ls ,message, validator);
			return;
		}

		long currentSize = getCurrentFileSize(vdaName, fileName , validator);
		if(isDebugEnabled())
			debug(concatAll(2 , "Current Size: " , ltoa(currentSize)));
		long newSizeLong = atol(newSize);//getNewAddingSectorFileSize(currentSize);

		if(isDebugEnabled())
			debug(concatAll(2 , "New Size: " , newSize));
		updateFileSize(vdaName , fileName , newSizeLong , validator);

		if(hasError(validator)){
			replyValidationError(ls , message , validator);
			return;
		}

		MpsMessage * resp = buildMpsMessage(message->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , message->operationName , NULL);

		sendMpsMessage(resp , ls , validator);
	}
