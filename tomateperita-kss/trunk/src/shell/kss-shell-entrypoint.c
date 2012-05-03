/*
 * kss-shell-commands.c
 *
 *  Created on: 05/06/2011
 *      Author: gonzalo
 */
#include <stdlib.h>
#include <stdio.h>

#include "linux-commons.h"
#include "linux-commons-mps.h"
#include "linux-commons-logging.h"
#include "linux-commons-errors.h"

#include "kss-core-services.h"
#include "kss-utils.h"
#include "kss-shell-service.h"
#include "kss-state.h"
#include "kss-tdd.h"
#include "kss-sys-calls.h"

#include "kss-vda-state.h"
#include "kss-fss-endpoint.h"
#include "kss-vda-endpoint.h"



	void executeCommandMount(ListenSocket l , MpsMessage * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		Iterator * ite = buildIterator(req->commands);

		char * vda = next(ite);

		if(isDebugEnabled())
			debug(concatAll(2 , "Montando dispositivo: " , vda));

		mountVda(vda , validator);

		MpsMessage * response = NULL;

		if(hasError(validator)){
			response = buildMpsMessage(req->descriptorId , MPS_RESPONSE_STATUS_CODE_ERROR ,
					req->operationName , buildStringsListWithValues(1 , validator->errorDescription));
		}else{
			response = buildMpsMessage(req->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName , NULL);
		}
		sendMpsMessage(response , l , validator);
	}



	void executeCommandUmount(ListenSocket l , MpsMessage * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		Iterator * ite = buildIterator(req->commands);
		char * vda = next(ite);

		if(isDebugEnabled())
			debug(concatAll(2 , "Desmontando dispositivo: " , vda));

		umountVda(vda , validator);

		MpsMessage * response = NULL;

		if(hasError(validator)){
			response = buildMpsMessage(req->descriptorId , MPS_RESPONSE_STATUS_CODE_ERROR ,
					req->operationName ,
					buildStringsListWithValues(1 , validator->errorDescription));
		}else{
			response = buildMpsMessage(req->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName , NULL);
		}

		sendMpsMessage(response , l , validator);
	}



	void executeCommandFormat(ListenSocket l , MpsMessage * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		Iterator * ite = buildIterator(req->commands);
		char * vda = next(ite);

		if(isDebugEnabled())
			debug(concatAll(2 , "Formateando dispositivo: " , vda));

		format(vda , validator);
		MpsMessage * response = NULL;
		if(hasError(validator)){
			response = buildMpsMessage(req->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_ERROR , req->operationName ,
				buildStringsListWithValues(1 , validator->errorDescription));
		}else{
			response = buildMpsMessage(req->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName , NULL);
		}

		sendMpsMessage(response , l , validator);
	}



	void executeCommandList(ListenSocket l , MpsMessage * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		Iterator * ite = buildIterator(req->commands);
		char * pattern = next(ite);

		if(isDebugEnabled())
			debug(concatAll(2 , "Listando Ruta: " , pattern));

		List elements = getPathContentElements(pattern  , validator);
		char * serializedElements = serializeList(elements , ",");

		MpsMessage * response = NULL;
		if(hasError(validator)){
			response = buildMpsMessage(req->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_ERROR , req->operationName ,
					buildStringsListWithValues(1 , validator->errorDescription));
		}else{
			response = buildMpsMessage(req->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName ,
					buildStringsListWithValues(1 , serializedElements));
		}

		sendMpsMessage(response , l , validator);
	}



	void executeCommandTddDump(ListenSocket l , MpsMessage * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		Iterator * ite = buildIterator( getTdd() );
		char * sectors = NULL;
		MpsMessage * response;

		info( "Realizando un dump de la TDD" );

		while( hasMoreElements(ite) ){
			TddRecord * record = next(ite);

			char * fileHandleId = itoa( record->fileHandleId );
			char * openingMode = itoa( record->openingMode );
			char * fileSize = itoa( record->fileSize );

			if ( record->fileSectors != NULL ){
				sectors = serializeList( record->fileSectors , "," );
			}

			List tddRecord = buildStringsListWithValues( 6 , fileHandleId ,
					record->fileName ,
					record->vdaName ,
					openingMode ,
					fileSize ,
					sectors );

			response = buildMpsMessage( req->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName ,
					tddRecord );
			sendMpsMessage(response , l , validator);
		}

		if(hasError(validator)){
			replyValidationError(l , response , validator);
		} else {
			response = buildMpsMessage(req->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName , NULL);
		}

		sendMpsMessage(response , l , validator);
	}



	void executeCommandDisconnect(ListenSocket l , MpsMessage * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		Iterator * ite = buildIterator(req->commands);

		char * vda = next(ite);

		if(isDebugEnabled())
			debug(concatAll(2 , "Desconectando dispositivo: " , vda));

		disconnectVda(vda , validator);

		MpsMessage * response = NULL;

		if(hasError(validator)){
			response = buildMpsMessage(req->descriptorId , MPS_RESPONSE_STATUS_CODE_ERROR ,
					req->operationName , buildStringsListWithValues(1 , validator->errorDescription));
		}else{
			response = buildMpsMessage(req->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName , NULL);
		}
		sendMpsMessage(response , l , validator);
	}



	void executeCommandShowDevices(ListenSocket l , MpsMessage * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		Iterator * ite = buildIterator( getConnectedVdas());
		MpsMessage *	response;

		if ( hasMoreElements(ite) ){
			List devicesNames = buildSringsList();

			while(hasMoreElements(ite)){
				VdaComponent * component = next(ite);
				addNode(devicesNames , component->vdaName);
			}

			response = buildMpsMessage(req->descriptorId , MPS_RESPONSE_STATUS_CODE_SUCCESS ,
					req->operationName , devicesNames);
		} else {
			response = buildMpsMessage(req->descriptorId , MPS_RESPONSE_STATUS_CODE_SUCCESS ,
					req->operationName , NULL);
		}

		sendMpsMessage(response , l , validator);
	}



	void executeCommandQuit(ListenSocket l , MpsMessage * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		if(isDebugEnabled())
					debug("Cerrando sesion de cliente.");

		MpsMessage * response = buildMpsMessage(req->descriptorId ,
						MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName , NULL);

		sendMpsMessage(response , l , validator);
	}
