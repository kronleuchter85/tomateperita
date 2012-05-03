/*
 * linux-commons-mps-blocks.c
 *
 *  Created on: 29/07/2011
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
#include "linux-commons-file.h"


	/*
	 * Sender
	 */


	void sendMpsBlockMessage(ListenSocket ls , MpsMessage * res, RuntimeErrorValidator * v){

		MpsStream * stream = serializeBlock(res->descriptorId , res->operationName , res->statusCode , res->block);
		int sended = sendBytes(ls , stream->data , stream->length);
		if(sended < stream->length){
			setError(v , "Ocurrio un error al enviar el bloque, los bytes enviados son menos que los del tamaño calculado");
			return;
		}
	}


	/*
	 * Receiver
	 */

	MpsMessage * receiveMpsBlockMessage(ListenSocket ls , RuntimeErrorValidator * validator){

		MpsHeader header;

		int received = receiveBytes(ls , &header , sizeof(MpsHeader));
		if(received != sizeof(MpsHeader)){
			setError(validator , "Ha habido un error en la recepcion, se recibieron menos bytes de los esperados");
			return NULL;
		}

		char * operationName = (char *) malloc(MPS_FIELD_LENGTH_OPERATION_NAME);
		bzero(operationName , MPS_FIELD_LENGTH_OPERATION_NAME);

		received = receiveBytes(ls , operationName , MPS_FIELD_LENGTH_OPERATION_NAME);
		if(received != MPS_FIELD_LENGTH_OPERATION_NAME){
			setError(validator , "Ha habido un error en la recepcion, se recibieron menos bytes de los esperados");
			return NULL;
		}

		// sacamos el suplemento
		char * dummyBytes = (char *) malloc(2 * MPS_FIELD_LENGTH_OPERATION_SEPARATOR);
		received = receiveBytes(ls , dummyBytes , 2 * MPS_FIELD_LENGTH_OPERATION_SEPARATOR);
		// fin de saque de suplemento


		MpsMessage * mpsResponse = buildMpsMessage(header.descriptorId ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , operationName , NULL);

		/*
		 *  Si la respuesta contiene un bloque
		 *  lo agrego de lo contrario queda en null
		 */
		if(header.payloadSize - (MPS_FIELD_LENGTH_OPERATION_NAME + 2 * MPS_FIELD_LENGTH_OPERATION_SEPARATOR) == sizeof(FileBlock)){

			FileBlock * block = (FileBlock *)malloc(sizeof(FileBlock));

			received = receiveBytes(ls , block , sizeof(FileBlock));
			if(received != sizeof(FileBlock)){
				setError(validator , "Ha habido un error en la recepcion, se recibieron menos bytes de los esperados");
				return NULL;
			}
			mpsResponse->block = block;
		}

		return mpsResponse;
	}


	/*
	 * Send and receivers
	 */

	MpsMessage * sendBlockAndReceiveMpsMessage(ListenSocket ls , MpsMessage * msg,  RuntimeErrorValidator * v){

		if(msg->block == NULL){
			setError(v , "El bloque que se esta enviando es null");
			return NULL;
		}

		sendMpsBlockMessage(ls , msg  , v);

		if(hasError(v)){
			setError(v , "Se produjo un error en el envio del Block");
			return NULL;
		}

		MpsMessage * response = receiveMpsMessage(ls , v);

		if(response == NULL || hasError(v)){
			setError(v , "Se produjo un error en la recepcion de la respuesta al enviar el Block");
			return NULL;
		}

		return response;
	}





	MpsMessage * sendMpsMessageAndReceiveBlock(ListenSocket ls , MpsMessage * req, RuntimeErrorValidator * validator){

		sendMpsMessage(req , ls , validator);

		if(hasError(validator)){
			return NULL;
		}

		MpsMessage * response = receiveMpsBlockMessage(ls , validator);

		if(hasError(validator)){
			return NULL;
		}

		return response;

	}
