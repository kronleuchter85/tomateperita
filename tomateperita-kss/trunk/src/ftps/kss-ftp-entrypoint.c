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





	char * coreOperationListFiles(char * pattern , RuntimeErrorValidator * validator){
		if(isDebugEnabled())
			debug(concatAll(2 , "Listando Ruta: " , pattern));

		List elements = getPathContentElements(pattern  , validator);
		return serializeList(elements , ",");
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




