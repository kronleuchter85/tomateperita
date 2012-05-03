/*
 * kss-ftps-entrypoint.c
 *
 *  Created on: 22/07/2011
 *      Author: gonzalo
 */

#include <stdlib.h>

#include "linux-commons.h"
#include "linux-commons-mps.h"
#include "linux-commons-list.h"
#include "linux-commons-file.h"
#include "linux-commons-errors.h"
#include "linux-commons-logging.h"

#include "kss-core-services.h"
#include "kss-fss-endpoint.h"
#include "kss-tdd.h"
#include "kss-utils.h"
#include "kss-vda-endpoint.h"
#include "kss-tdd.h"
#include "kss-vda-state.h"





	/*
	 * Apertura del archivo
	 *
	 */
	void executeOperationOpen(ListenSocket ls , MpsMessage * aReq,
			RuntimeErrorValidator * validator){
		Iterator * ite = buildIterator(aReq->commands);

		char * openingMode = hasMoreElements(ite) ? next(ite) : NULL;
		char * path = hasMoreElements(ite) ? next(ite) : NULL;

		char * handleId = coreOperationOpenFile(path , openingMode , validator);

		if(hasError(validator)){
			replyValidationError(ls , aReq , validator);
			return;
		}

		MpsMessage * response = buildMpsMessage(aReq->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS ,
				aReq->operationName ,
				buildStringsListWithValues(1, handleId));

		sendMpsMessage(response , ls , validator);
	}






	/*
	 * Operacion Close
	 */
	void executeOperationClose(ListenSocket ls , MpsMessage * aReq,
			RuntimeErrorValidator * validator){

		Iterator * ite = buildIterator(aReq->commands);
		char * fhId = hasMoreElements(ite) ? next(ite) : NULL;

		coreOperationCloseFile(fhId , validator);

		if(hasError(validator)){
			replyValidationError(ls , aReq , validator);
			return;
		}

		MpsMessage * response = buildMpsMessage(aReq->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , aReq->operationName , NULL);

		sendMpsMessage(response , ls , validator);
	}







	void executeOperationWrite(ListenSocket ls , MpsMessage * msg,
			RuntimeErrorValidator * validator){

		if(msg == NULL || msg->block == NULL){
			setError(validator, "Ha ocurrido un error en la syscall SysWrite, el bloque enviado es null");
			return ;
		}

		TddRecord * record = getTddRecordByFileHandleId(msg->block->fileHandleid);
		VdaComponent * vda = getMountedVda(record->vdaName);

		List sectors = callFssOperationAssignAndGetSectors(record->vdaName , record->fileName , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			replyValidationError(ls , msg , validator);
			return;
		}

		Iterator * iteSectors = buildIterator(sectors);

		int sector1 = hasMoreElements(iteSectors) ? atoi(next(iteSectors)) : -1;
		int sector2 = hasMoreElements(iteSectors) ? atoi(next(iteSectors)) : -1;

		msg->block->sector1.sector = sector1;
		msg->block->sector2.sector = sector2;

		callVdaOperationPutSectores(vda , msg->block , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			replyValidationError(ls , msg , validator);
			return;
		}

		int bytesToWrite = msg->block->sector1.dataLength
				+ msg->block->sector2.dataLength;

		record->fileSize += bytesToWrite;

		callFssOperationUpdateFileSize(record->vdaName , record->fileName , record->fileSize , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			replyValidationError(ls , msg , validator);
			return;
		}

		MpsMessage * response = buildMpsMessage(msg->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , msg->operationName , NULL);

		sendMpsMessage(response , ls , validator);
	}








	void executeOperationRead(ListenSocket ls , MpsMessage * aReq , RuntimeErrorValidator * validator){
		Iterator * ite = buildIterator(aReq->commands);
		char * fileId = hasMoreElements(ite) ? next(ite) : NULL;


		if(fileId == NULL){
			setError(validator , "El FileId no puede estar en null");
			return;
		}

		TddRecord * record = getTddRecordByFileHandleId(atoi(fileId));

		if(record == NULL){
			setError(validator , "File Id no corresponde a un registro activo en la TDD");
			return;
		}


		ite = buildIterator(record->fileSectors);

		VdaComponent * comp = getMountedVda(record->vdaName);

		FileBlock * block = NULL;
		MpsMessage * msg = NULL;

		int bytesToSend = record->fileSize;

		while(hasMoreElements(ite)){
			char * sector1 = hasMoreElements(ite) ? next(ite) : NULL;
			char * sector2 = hasMoreElements(ite) ? next(ite) : NULL;

			// Esto no deberia ocurrir nunca, pero por las dudas se valida
			if(sector1 == NULL && sector2 == NULL){

				setError(validator , concatAll( 3 , "Ocurrio un error, ambos sectores cargados en el TDD para el archivo " ,
						record->fileName , "son nulos. "));
				break;
			}

			block = callVdaOperationGetSectores(comp , atoi(sector1) , atoi(sector2) , validator);
			if(hasError(validator)){
				replyValidationError(ls , aReq , validator);
				return;
			}

			/*
			 * Se actualiza el tamaño de los bytes enviados
			 * en los bloques
			 *
			 */
			block->sector1.dataLength = (bytesToSend > sizeof(block->sector1.data)) ? sizeof(block->sector1.data) : bytesToSend;
			bytesToSend -= block->sector1.dataLength;
			block->sector2.dataLength = (bytesToSend > sizeof(block->sector2.data)) ? sizeof(block->sector1.data) : bytesToSend;
			bytesToSend -= block->sector1.dataLength;

			msg = buildMpsMessage(aReq->descriptorId , MPS_RESPONSE_STATUS_CODE_SUCCESS , aReq->operationName , NULL);
			msg->block = block;

			sendMpsBlockMessage(ls , msg , validator);
		}

		msg = buildMpsMessage(aReq->descriptorId , MPS_RESPONSE_STATUS_CODE_SUCCESS , aReq->operationName , NULL);

		sendMpsMessage(msg , ls , validator);
	}


