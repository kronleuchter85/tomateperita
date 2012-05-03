/*
 * linux-syscall-protocol.c
 *
 *  Created on: 12/06/2011
 *      Author: gonzalo
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "windows-commons-list.h"
#include "windows-commons-logging.h"
#include "windows-commons-mps.h"
#include "windows-commons-socket.h"
#include "windows-commons-mps-encoder.h"
#include "windows-commons.h"


	


	void sendMpsMessage(MpsMessage * resp , ListenSocket ls , 
		RuntimeErrorValidator * v , HeapHandler hh){

		MpsStream * stream = serializeMpsMessage(resp ,v , hh);
		int sended ;

		if(isDebugEnabled())
			logDebug( processName , "Enviando una respuesta MPS" , hh);

		sended = sendBytes(ls , stream->data , stream->length);

		if(sended != stream->length){
			setError(v , concat(hh , 5, "Se ha enviado una cantidad distinta de bytes con respecto a la cantidad esperada para enviar: ( enviados:"
					, intToString(sended , hh) , " vs planificados:" , intToString(stream->length , hh) , ")"));
		}
	}





	MpsMessage * receiveMpsMessage(ListenSocket ls , RuntimeErrorValidator * validator , HeapHandler hh){
		MpsHeader header;
		
		char buffer[2048];
		
		char * payload = NULL;

		int received;
		int argsCount;
		char * operationName;
		List retValues = NULL;


		if(isDebugEnabled())
			logDebug( processName , "Esperando recibir Header" , hh );

		received = receiveBytes(ls , &header , sizeof(MpsHeader));
		if(received != sizeof(MpsHeader)){
			setError(validator, "Se recibio una cantidad distinta de bytes de los solicitados para el header");
			return NULL;
		}

		if(isDebugEnabled())
			logDebug( processName , "Reciviendo una peticion MPS" , hh);

		if( header.descriptorId == NULL ){
			setError(validator, "Se recibio una cantidad distinta de bytes de los solicitados para el descriptorId");
			return NULL;
		}

		if(isDebugEnabled())
			logDebug( processName , concat(hh , 3, "Mensjae con descriptorId: '" , strdup(header.descriptorId) , "'") , hh );
		
		//payload = (char *) allocateMemory(hh ,header.payloadSize );

		ZeroMemory(buffer , 2048);

		if(isDebugEnabled())
			logDebug( processName , concat(hh , 3, "Esperando por un payload de " , intToString(header.payloadSize , hh) , " bytes por MPS") , hh);
		
		received = receiveBytes(ls , buffer , header.payloadSize);
		if(received != header.payloadSize){
			setError(validator, "Se recibio una cantidad distinta de bytes de los solicitados para el payload");
			return NULL;
		}

		payload = subString(buffer , 0 , header.payloadSize - 1 , hh);
		argsCount = getMpsArgsCount(payload , validator, hh);
		operationName = getMpsOperationName(payload , validator , hh);

		if(isDebugEnabled())
			logDebug( processName , concat(hh , 5, "Ejecutando la operacion remota " , 
			operationName , " con " , intToString(argsCount , hh) , " argumentos MPS") , hh);


		if(argsCount > 0){
			retValues = getMpsArguments(payload , argsCount , validator , hh);
		}
			
		if(isDebugEnabled())
			//logDebug( processName , concat(hh , 2, "Reciviendo codigo de resultado: " , charToString(header.payloadDescriptor , hh) ) , hh);

		return buildMpsMessage(header.descriptorId , header.payloadDescriptor , 
			operationName , retValues , hh);
	}




	MpsMessage * sendAndReceiveMpsMessages(ListenSocket ls ,
		MpsMessage * msg ,RuntimeErrorValidator * v , HeapHandler hh){
		MpsMessage * response;
		sendMpsMessage(msg , ls , v , hh);

		if(hasError(v)){
			setError(v , "Se produjo un error en el envio del Request");
			return NULL;
		}

		response = receiveMpsMessage(ls , v , hh);

		if(hasError(v)){
			setError(v , "Se produjo un error en la recepcion del Response");
			return NULL;
		}

		return response;
	}




	

