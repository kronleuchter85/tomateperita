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




	/*
	 * ***************************************** Builders *********************************
	 */



	MpsHandshake * buildMpsHandhake(char * key , char code , HeapHandler hh){
		MpsHandshake * hand = allocateMemory(hh ,sizeof(MpsHandshake));
		hand->descriptorId = key;
		hand->statusCode = code;
		return hand;
	}



	MpsMessage * buildMpsMessage(char * descriptionId , char statusCode ,
			char * operationName , List returnValues , HeapHandler hh){

		MpsMessage * respo = (MpsMessage *)allocateMemory(hh ,sizeof(MpsMessage));

		ZeroMemory(respo->descriptorId , sizeof(respo->descriptorId));
		strcpy(respo->descriptorId , descriptionId);
		respo->operationName = operationName;
		respo->statusCode = statusCode;
		respo->commands = returnValues;
		respo->block = NULL;
		return respo;
	}

	
	MpsStream * buildMpsStreamWithOffset(char * data , int length, HeapHandler hh){
		MpsStream * stream = allocateMemory(hh ,sizeof(MpsStream));
		stream->data = data;
		stream->length = length;
		
		return stream;
	}


