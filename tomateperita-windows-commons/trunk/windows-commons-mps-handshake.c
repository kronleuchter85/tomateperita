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





	void sendMpsHandshake(MpsHandshake * hand , ListenSocket l , 
		RuntimeErrorValidator * v , HeapHandler hh){

		MpsHeader header;
		int bytesSended ;

		memcpy(header.descriptorId , hand->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID);
		header.payloadDescriptor = hand->statusCode;
		header.payloadSize = 0;

		bytesSended = sendBytes(l , &header , sizeof(MpsHeader));

		if( bytesSended <= 0 ){
			setError(v , "No se han podido enviar bytes en el handshake");
		}
	}




	MpsHandshake * receiveMpsHandshake(ListenSocket l , RuntimeErrorValidator * v , HeapHandler hh){
		MpsHeader header;
		int receivedBytes = receiveBytes(l , &header , sizeof(MpsHeader) );

		if(receivedBytes <= 0){
			setError(v, "No se han recivido bytes en el handshake");
		}

		return buildMpsHandhake( header.descriptorId , header.payloadDescriptor , hh );
	}



	MpsHandshake * sendAndReceiveMpsHandshake(MpsHandshake * h, ListenSocket l , 
		RuntimeErrorValidator * validator , HeapHandler hh){
		sendMpsHandshake(h , l , validator , hh);

		if(hasError(validator)){
			return NULL;
		}else{
			 return receiveMpsHandshake(l , validator , hh);
		}
	}
