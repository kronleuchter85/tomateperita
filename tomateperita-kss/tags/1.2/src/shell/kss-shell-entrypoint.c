/*
 * kss-shell-commands.c
 *
 *  Created on: 05/06/2011
 *      Author: gonzalo
 */
#include <stdlib.h>
#include <stdio.h>

#include "linux-commons.h"
#include "linux-commons-remote-services.h"
#include "linux-commons-mps.h"
#include "linux-commons-logging.h"
#include "linux-errors.h"

#include "kss-core-services.h"
#include "kss-utils.h"
#include "kss-shell-service.h"
#include "kss-state.h"



	void executeCommand(ListenSocket l , MpsRequest * req){

		if(equalsStrings(SHELL_MOUNT , req->operationName)){

			executeCommandMount(l , req);
		}else if(equalsStrings(SHELL_UMOUNT ,req->operationName)){

			executeCommandUmount(l , req);
		}else if(equalsStrings(SHELL_FORMAT ,req->operationName)){

			executeCommandFormat(l , req);
		}else if(equalsStrings(SHELL_LS,req->operationName)){

			executeCommandList(l , req);
		}else if(equalsStrings(SHELL_MD5_SUM ,req->operationName)){

			executeCommandMd5Sum(l , req);
		}else if(equalsStrings(SHELL_TDD_DUMP, req->operationName)){

			executeCommandTddDump(l , req);
		}
	}



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

		info("Realizando un Dump de la TDD" );

		MpsResponse * response = buildMpsResponse(req->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , req->operationName , NULL);

		sendMpsResponse(response , l , validator);
	}



	void executeCommandMd5Sum(ListenSocket l , MpsRequest * req){
	}
