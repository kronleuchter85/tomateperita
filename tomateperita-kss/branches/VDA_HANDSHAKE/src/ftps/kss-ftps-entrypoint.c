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
#include "kss-ftps-entrypoint-core.h"
#include "kss-vda-state.h"






	/*
	 * Apertura del archivo
	 * 70 lineas la puta madre!!!!
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
	 * Operacion Close !!
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







	void executeOperationList(ListenSocket ls , MpsMessage * aReq ,
			RuntimeErrorValidator * validator){

		Iterator * ite = buildIterator(aReq->commands);
		char * pattern = next(ite);

		List separatedFilesOrVdas = getPathContentElements(pattern , validator);

		if(hasError(validator)){
			replyValidationError(ls , aReq , validator);
			return;
		}

		MpsMessage * response = buildMpsMessage(aReq->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_SUCCESS , aReq->operationName ,
					separatedFilesOrVdas);

		sendMpsMessage(response , ls , validator);
	}





	void executeOperationWrite(ListenSocket ls ,
			MpsMessage * aReq,
			RuntimeErrorValidator * validator){

		Iterator * ite = buildIterator(aReq->commands);
		char * fileId = hasMoreElements(ite) ? next(ite) : NULL;
		char * contentBlock = hasMoreElements(ite) ? next(ite) : NULL;

		coreOperationWriteFile(fileId , contentBlock , validator);

		if(hasError(validator)){
			replyValidationError(ls , aReq , validator);
			return;
		}

		MpsMessage * response = buildMpsMessage(aReq->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , aReq->operationName , NULL);

		sendMpsMessage(response , ls , validator);
	}








	void executeOperationRead(ListenSocket ls , MpsMessage * aReq , RuntimeErrorValidator * validator){
		Iterator * ite = buildIterator(aReq->commands);
		char * fileId = hasMoreElements(ite) ? next(ite) : NULL;

		/* Se recibe como parametro.
		 * Es una buena practica obtenerlo de esta forma y no mantener un estado aunq
		 * el enunciado no dice nada.
		 * En ultima instancia hay q mantener el estado de las peticiones para este cliente y
		 * de esta manera el ultimo sector consumido.
		 */
		char * lastSectorSended = hasMoreElements(ite) ? next(ite) : NULL;

		FileBlock * block = coreOperationReadFile(fileId , lastSectorSended , validator);

		if(hasError(validator)){
			replyValidationError(ls , aReq , validator);
			return;
		}

		List blockList = NULL;
		if(hasTwoSectors(block)){
			blockList =  buildStringsListWithValues(4 , itoa(block->sector1.sector) ,
					block->sector1.data , itoa(block->sector2.sector) , block->sector2.data );
		} else {
			blockList =  buildStringsListWithValues(2 , itoa(block->sector1.sector) , block->sector1.data);
		}

		MpsMessage  * response = buildMpsMessage(aReq->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , aReq->operationName , blockList);

		sendMpsMessage(response , ls , validator);
	}


