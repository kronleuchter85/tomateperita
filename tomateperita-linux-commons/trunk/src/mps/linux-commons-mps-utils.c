/*
 * linux-commons-mps-utils.c
 *
 *  Created on: 19/07/2011
 *      Author: gonzalo
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "linux-commons-list.h"
#include "linux-commons-logging.h"
#include "linux-commons-mps.h"
#include "linux-commons-socket.h"
#include "linux-commons-mps-encoder.h"
#include "linux-commons-errors.h"


	int calculatePayloadLength(char * serializedValues);


	/**
	 * Funciones de obtencion de datos y utilidad
	 */

	char * getMpsOperationName(char * payload, RuntimeErrorValidator * v){
		return strndup(payload , MPS_FIELD_LENGTH_OPERATION_NAME);
	}

	List getMpsArguments(char * payload , int argsCount, RuntimeErrorValidator * v){

		char * searchingString = payload + MPS_FIELD_LENGTH_STANDARD_PAYLOAD + MPS_FIELD_LENGTH_OPERATION_SEPARATOR;

		if(argsCount > 0){
			List args = buildSringsList();

			if(argsCount == 1){

				char * value = decode(strdup(searchingString));
				addNode(args , value);

			}else{

				List tokenized = tokenize(searchingString , ' ');
				Iterator * ite = buildIterator(tokenized);
				while(hasMoreElements(ite)){
					char * arg = decode(next(ite));
					addNode(args , arg);
				}
			}
			return args;
		}

		return NULL;
	}

	int getMpsArgsCount(char * payload, RuntimeErrorValidator * v){
		char * count = strndup(payload + MPS_FIELD_LENTTH_STANDARD_PAYLOAD_FIRST_PART , MPS_FIELD_LENGTH_ARGUMENTS_COUNT);
		return atoi(count);
	}


	int calculatePayloadLength(char * serializedValues){
		int payloadLength = MPS_FIELD_LENGTH_STANDARD_PAYLOAD ;
		if( !equalsStrings(trim(serializedValues) , "")){
			payloadLength += MPS_FIELD_LENGTH_OPERATION_SEPARATOR + strlen(serializedValues);
		}

		return payloadLength;
	}


	void replyValidationError(ListenSocket ls , MpsMessage * request , RuntimeErrorValidator * validator){
		MpsMessage * resp = buildMpsMessage(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_ERROR,
				request->operationName ,
				buildStringsListWithValues(1, validator->errorDescription));
		sendMpsMessage(resp , ls , validator);
	}




