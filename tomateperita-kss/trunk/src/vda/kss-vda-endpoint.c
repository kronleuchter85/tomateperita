/*
 * kss-vda-endpoint.c
 *
 *  Created on: 16/07/2011
 *      Author: gonzalo
 */

#include <stdlib.h>

#include "linux-commons-mps.h"
#include "linux-commons-remote-services.h"
#include "linux-commons-file.h"

#include "kss-utils.h"
#include "kss-tdd.h"
#include "kss-vda-service.h"
#include "kss-vda-state.h"
#include "kss-core-services.h"




	void callVdaOperationPutSectores(VdaComponent * vdaComp ,
			FileBlock * fileBlock ,
			RuntimeErrorValidator * validator){

		MpsMessage * message = buildMpsMessage(generateRandomKey(16) ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , VDA_SECTORS_PUTTING , NULL);
		sendMpsMessage(message , vdaComp->listenSocket , validator);

		message = buildMpsMessage(generateRandomKey(16) ,
						MPS_RESPONSE_STATUS_CODE_SUCCESS , VDA_SECTORS_PUTTING , NULL);
		message->block = fileBlock;
		message = sendBlockAndReceiveMpsMessage(vdaComp->listenSocket , message , validator);

		if(hasError(validator)){
			error(validator->errorDescription);
			return;
		}else if(message->statusCode == MPS_RESPONSE_STATUS_CODE_ERROR){
			setError(validator , "Se ha producido un error en la operacion PutSectores");
			error(validator->errorDescription);
			return;
		} else{
			debug("La operacion PutSectores se ejecuto correctamente");
		}
	}






	FileBlock * callVdaOperationGetSectores(VdaComponent * vdaComp ,
			int sector1 , int sector2 , RuntimeErrorValidator * validator){

		MpsMessage * message = buildMpsMessage(generateRandomKey(16) , MPS_RESPONSE_STATUS_CODE_SUCCESS ,
				VDA_SECTORS_GETTING , buildStringsListWithValues(2 , itoa(sector1) , itoa(sector2)));

		message = sendMpsMessageAndReceiveBlock(vdaComp->listenSocket , message , validator);

		if(hasError(validator)){
			error(validator->errorDescription);
			return NULL;
		}else if ( message == NULL || message->statusCode == MPS_RESPONSE_STATUS_CODE_ERROR ){
			setError(validator , "Se ha producido un error en la operacion GetSectores del VDA");
			error(validator->errorDescription);
			return NULL;
		}else if(message->block == NULL){
			setError(validator , "Se ha producido un error en getSectores, el block esta en null");
			return NULL;
		}else{
			return message->block;
		}
	}



	VdaInformation * callVdaOperationGetChs(VdaComponent * vdaComp , RuntimeErrorValidator * validator){
		MpsMessage * req = buildMpsMessage(generateRandomKey(16) ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS, VDA_CHS_GETTING , NULL);
		MpsMessage * response = sendAndReceiveMpsMessages(vdaComp->listenSocket , req , validator);
		if( response->statusCode != MPS_RESPONSE_STATUS_CODE_SUCCESS ){
			Iterator * ite = buildIterator(response->commands);
			char * errorMessage = hasMoreElements(ite)?next(ite):NULL;
			setError(validator , errorMessage);
			return NULL;
		}else{
			return buildVdaInformation(vdaComp->vdaName , response->commands);
		}
	}


