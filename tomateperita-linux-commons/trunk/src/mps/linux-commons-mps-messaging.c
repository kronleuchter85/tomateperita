/*
 * linux-syscall-protocol.c
 *
 *  Created on: 12/06/2011
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






	void sendMpsMessage(MpsMessage * resp , ListenSocket ls , RuntimeErrorValidator * v){

		MpsStream * stream = serializeMpsMessage(resp ,v);

		if(isDebugEnabled())
			logDebug( "linux-commons" , "Enviando una respuesta MPS");

		int sended = sendBytes(ls , stream->data , stream->length);

		if(sended != stream->length){
			setError(v , concatAll(5, "Se ha enviado una cantidad distinta de bytes con respecto a "
					"la cantidad esperada para enviar: ( enviados:"
					, itoa(sended) , " vs planificados:" , itoa(stream->length) , ")"));
		}
	}






	MpsMessage * receiveMpsMessage(ListenSocket ls , RuntimeErrorValidator * validator){
		MpsHeader header;

		if(isDebugEnabled())
			logDebug("linux-commons" , "Esperando recibir Header" );

		int received = receiveBytes(ls , &header , sizeof(MpsHeader));
		if(received != sizeof(MpsHeader)){
			setError(validator, "Se recibio una cantidad distinta de bytes de los solicitados para el header");
			return NULL;
		}

		if(isDebugEnabled())
			logDebug("linux-commons" , "Reciviendo una peticion MPS");

		if( header.descriptorId == NULL ){
			setError(validator, "Se recibio una cantidad distinta de bytes de los solicitados para el descriptorId");
			return NULL;
		}

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(3, "Mensjae con descriptorId: '" , header.descriptorId , "'") );

		//char * payload = (char *) malloc(header.payloadSize);


		char buffer[2048];
		bzero(buffer, 2048);

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(3, "Esperando por un payload de " , itoa(header.payloadSize) , " bytes por MPS") );

		received = receiveBytes(ls , buffer , header.payloadSize);
		if(received != header.payloadSize){
			setError(validator, "Se recibio una cantidad distinta de bytes de los solicitados para el payload");
			return NULL;
		}

		char * payload = strndup(buffer, header.payloadSize);

		int argsCount = getMpsArgsCount(payload , validator);
		char * operationName = getMpsOperationName(payload ,validator);

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(5, "Ejecutando la operacion remota " , operationName , " con " , itoa(argsCount) , " argumentos MPS") );

		List retValues = NULL;
		if(argsCount > 0){
			retValues = getMpsArguments(payload , argsCount , validator);
		}

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(2, "Reciviendo codigo de resultado: " , header.payloadDescriptor ) );

		return buildMpsMessage(header.descriptorId , header.payloadDescriptor , operationName , retValues);
	}








	MpsMessage * sendAndReceiveMpsMessages(ListenSocket ls , MpsMessage * msg ,RuntimeErrorValidator * v){

		sendMpsMessage(msg , ls , v);

		if(hasError(v)){
			setError(v , "Se produjo un error en el envio del Request");
			return NULL;
		}

		MpsMessage * response = receiveMpsMessage(ls , v);

		if(hasError(v)){
			setError(v , "Se produjo un error en la recepcion del Response");
			return NULL;
		}

		return response;
	}





