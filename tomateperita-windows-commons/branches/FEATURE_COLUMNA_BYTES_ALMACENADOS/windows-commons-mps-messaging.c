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


	


	void sendMpsMessage(MpsMessage * resp , ListenSocket ls , 
		RuntimeErrorValidator * v , HeapHandler hh){

		MpsStream * stream = serializeMpsMessage(resp ,v , hh);
		int sended ;

		if(isDebugEnabled())
			logDebug( "windows-commons" , "Enviando una respuesta MPS" , hh);

		sended = sendBytes(ls , stream->data , stream->length);

		if(sended != stream->length){
			setError(v , concat(5, "Se ha enviado una cantidad distinta de bytes con respecto a "
					"la cantidad esperada para enviar: ( enviados:"
					, intToString(sended , hh) , " vs planificados:" , intToString(stream->length , hh) , ")"));
		}
	}





	MpsMessage * receiveMpsMessage(ListenSocket ls , RuntimeErrorValidator * validator , HeapHandler hh){
		MpsHeader header;
		char * payload ;
		int received;
		int argsCount;
		char * operationName;
		List retValues = NULL;

		if(isDebugEnabled())
			logDebug("windows-commons" , "Esperando recibir Header" , hh );

		received = receiveBytes(ls , &header , sizeof(MpsHeader));
		if(received != sizeof(MpsHeader)){
			setError(validator, "Se recibio una cantidad distinta de bytes de los solicitados para el header");
			return NULL;
		}

		if(isDebugEnabled())
			logDebug("windows-commons" , "Reciviendo una peticion MPS" , hh);

		if( header.descriptorId == NULL ){
			setError(validator, "Se recibio una cantidad distinta de bytes de los solicitados para el descriptorId");
			return NULL;
		}

		if(isDebugEnabled())
			logDebug("windows-commons" , concat(3, "Mensjae con descriptorId: '" , header.descriptorId , "'") , hh );
		
		payload = (char *) allocateMemory(hh ,header.payloadSize );
		ZeroMemory(payload , header.payloadSize );

		if(isDebugEnabled())
			logDebug("windows-commons" , concat(3, "Esperando por un payload de " , intToString(header.payloadSize , hh) , " bytes por MPS") , hh);

		received = receiveBytes(ls , payload , header.payloadSize);
		if(received != header.payloadSize){
			setError(validator, "Se recibio una cantidad distinta de bytes de los solicitados para el payload");
			return NULL;
		}

		argsCount = getMpsArgsCount(payload , validator, hh);
		operationName = getMpsOperationName(payload , validator , hh);

		if(isDebugEnabled())
			logDebug("windows-commons" , concat(5, "Ejecutando la operacion remota " , 
			operationName , " con " , intToString(argsCount , hh) , " argumentos MPS") , hh);


		if(argsCount > 0){
			retValues = getMpsArguments(payload , argsCount , validator , hh);
		}
			
		if(isDebugEnabled())
			logDebug("windows-commons" , concat(2, "Reciviendo codigo de resultado: " , header.payloadDescriptor ) , hh);

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




	

