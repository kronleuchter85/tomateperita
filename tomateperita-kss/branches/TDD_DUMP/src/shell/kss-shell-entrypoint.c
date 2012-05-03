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




	void executeCommandMount(ListenSocket l , MpsRequest * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		Iterator * ite = buildIterator(req->arguments);

		char * vda = next(ite);

		if(isDebugEnabled())
			debug(concatAll(2 , "Montando dispositivo: " , vda));

		mountVda(vda , validator);

		MpsResponse * response = NULL;

		if(hasError(validator)){
			response = buildMpsResponse(req->descriptorId , MPS_RESPONSE_STATUS_CODE_ERROR ,
					req->operationName , buildStringsListWithValues(1 , validator->errorDescription));
		}else{
			response = buildMpsResponse(req->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName , NULL);
		}
		sendMpsResponse(response , l , validator);
	}



	void executeCommandUmount(ListenSocket l , MpsRequest * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		Iterator * ite = buildIterator(req->arguments);
		char * vda = next(ite);

		if(isDebugEnabled())
			debug(concatAll(2 , "Desmontando dispositivo: " , vda));

		umountVda(vda , validator);

		MpsResponse * response = NULL;

		if(hasError(validator)){
			response = buildMpsResponse(req->descriptorId , MPS_RESPONSE_STATUS_CODE_ERROR ,
					req->operationName ,
					buildStringsListWithValues(1 , validator->errorDescription));
		}else{
			response = buildMpsResponse(req->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName , NULL);
		}

		sendMpsResponse(response , l , validator);
	}



	void executeCommandFormat(ListenSocket l , MpsRequest * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		Iterator * ite = buildIterator(req->arguments);
		char * vda = next(ite);

		if(isDebugEnabled())
			debug(concatAll(2 , "Formateando dispositivo: " , vda));

		format(vda , validator);
		MpsResponse * response = NULL;
		if(hasError(validator)){
			response = buildMpsResponse(req->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_ERROR , req->operationName ,
				buildStringsListWithValues(1 , validator->errorDescription));
		}else{
			response = buildMpsResponse(req->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName , NULL);
		}

		sendMpsResponse(response , l , validator);
	}



	void executeCommandList(ListenSocket l , MpsRequest * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		Iterator * ite = buildIterator(req->arguments);
		char * pattern = next(ite);

		if(isDebugEnabled())
			debug(concatAll(2 , "Listando Ruta: " , pattern));

		List elements = getPathContentElements(pattern  , validator);
		char * serializedElements = serializeList(elements , ",");

		MpsResponse * response = NULL;
		if(hasError(validator)){
			response = buildMpsResponse(req->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_ERROR , req->operationName ,
					buildStringsListWithValues(1 , validator->errorDescription));
		}else{
			response = buildMpsResponse(req->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName ,
					buildStringsListWithValues(1 , serializedElements));
		}

		sendMpsResponse(response , l , validator);
	}



	void executeCommandTddDump(ListenSocket l , MpsRequest * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		Iterator * ite = buildIterator( getTdd() );
		char * sectors = NULL;
		MpsResponse * response;

		info( "Realizando un dump de la TDD" );

		while( hasMoreElements(ite) ){
			TddRecord * record = next(ite);

			//Pasar a string los campos
			char * fileHandleId = itoa( record->fileHandleId );
			char * openingMode = itoa( record->openingMode );
			char * fileSize = itoa( record->fileSize );

			//Serializar la lista de sectores
			if ( !(record->fileSectors == NULL) ){
				sectors = serializeList( record->fileSectors , "," );
			}

			//buildStringListWithValues(string campos , lista de sectores)
			List tddRecord = buildStringsListWithValues( 6 , fileHandleId ,
					record->fileName ,
					record->vdaName ,
					openingMode ,
					fileSize ,
					sectors );

			//Mandar por mps el record
			response = buildMpsResponse( req->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName ,
					tddRecord );
			sendMpsResponse(response , l , validator);
		}

		/* Este es el ulitmo send, el que dice que se terminaron los datos a enviar*/
		if(hasError(validator)){
			response = buildMpsResponse(req->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_ERROR , req->operationName , NULL);
		} else {
			response = buildMpsResponse(req->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName , NULL);
		}

		sendMpsResponse(response , l , validator);
	}



	void executeCommandMd5Sum(ListenSocket l , MpsRequest * req){

		/*
		 * Fernando..
		 */

	}
