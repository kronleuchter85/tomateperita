/*
 * linux-commons-mps-handshake.c
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




	void sendMpsHandshake(MpsHandshake * hand , ListenSocket l , RuntimeErrorValidator * v){

		MpsHeader header;

		memcpy(header.descriptorId , hand->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID);
		header.payloadDescriptor = hand->statusCode;
		header.payloadSize = 0;

		int bytesSended = sendBytes(l , &header , sizeof(MpsHeader));

		if( bytesSended <= 0 ){
			setError(v , "No se han podido enviar bytes en el handshake");
		}
	}




	MpsHandshake * receiveMpsHandshake(ListenSocket l , RuntimeErrorValidator * v){
		MpsHeader header;
		int receivedBytes = receiveBytes(l , &header , sizeof(MpsHeader) );

		if(receivedBytes <= 0){
			setError(v, "No se han recivido bytes en el handshake");
		}

		return buildMpsHandhake( header.descriptorId , header.payloadDescriptor );
	}



	MpsHandshake * sendAndReceiveMpsHandshake(MpsHandshake * h, ListenSocket l , RuntimeErrorValidator * validator){
		sendMpsHandshake(h , l , validator);

		if(hasError(validator)){
			return NULL;
		}else{
			 return receiveMpsHandshake(l , validator);
		}
	}


