/*
 * linux-commons-mps-builders.c
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



	MpsHandshake * buildMpsHandhake(char * key , char code){
		MpsHandshake * hand = malloc(sizeof(MpsHandshake));
		hand->descriptorId = key;
		hand->statusCode = code;
		return hand;
	}

	MpsStream * buildMpsStreamWithOffset(char * data , int length){
		MpsStream * stream = malloc(sizeof(MpsStream));
		stream->data = data;
		stream->length = length;
		return stream;
	}


	MpsMessage * buildMpsMessage(char * descriptionId , char statusCode ,
			char * operationName , List returnValues){

		MpsMessage * respo = (MpsMessage *)malloc(sizeof(MpsMessage));

		bzero(respo->descriptorId , sizeof(respo->descriptorId));
		strcpy(respo->descriptorId , descriptionId);
		respo->operationName = operationName;
		respo->statusCode = statusCode;
		respo->commands = returnValues;
		respo->block = NULL;
		return respo;
	}
