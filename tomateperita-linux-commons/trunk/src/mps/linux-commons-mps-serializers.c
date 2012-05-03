/*
 * linux-commons-mps-serializers.c
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


	void serializeDescriptorId( char * descriptorId , char * serialization , int * offset);
	void serializePayloadDescriptor(char * payDesc , char * serialization , int * offset);
	void serializePayloadLength(int payLength , char * serialization , int * offset);
	int serializePayloadOperationName(char * operationName , char * serialization , int offset);
	int serializePayloadSeparator(char * serialization , int offset);
	int serializePayloadValuesList(char * values , int valsCount , char * serialization , int offset);

	char * serializeValueList(List values);






	/*
	 * Funciones serializadoras
	 */

	MpsStream * serializeMpsMessage(MpsMessage * msg , RuntimeErrorValidator * v){

		if(isDebugEnabled())
			logDebug("linux-commons" , "Serializando Response MPS");

		char * justRetVals = serializeValueList(msg->commands);
		int payloadLength = calculatePayloadLength(justRetVals);


		char * data = malloc( MPS_FIELD_LENTH_HEADER + payloadLength );
		bzero(data , MPS_FIELD_LENTH_HEADER + payloadLength);

		MpsHeader header;
		header.payloadSize = payloadLength;
		header.payloadDescriptor = msg->statusCode;

		memcpy(header.descriptorId , msg->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID);
		memcpy(data , &header , MPS_FIELD_LENTH_HEADER);

		int offset = MPS_FIELD_LENTH_HEADER;
		offset = serializePayloadOperationName(msg->operationName , data , offset);
		offset = serializePayloadSeparator(data , offset);

		if(msg->commands != NULL){
			offset = serializePayloadValuesList(justRetVals , msg->commands->size , data , offset);
		}else{
			offset = serializePayloadValuesList(justRetVals , 0 , data , offset);
		}

		return buildMpsStreamWithOffset(data, offset);
	}






	/*
	 * Genera una serializacion de la lista
	 */
	char * serializeValueList(List values){

		char arguments[2048];
		bzero(arguments , 2048);

		Iterator * iteArgs = buildIterator(values);

		int position = 0;
		while(hasMoreElements(iteArgs)){
			char * argument = next(iteArgs);
			argument = encode(argument);
			strcpy(arguments + position, argument);
			position += strlen(argument);

			if(hasMoreElements(iteArgs)){
				strcpy(arguments + position , MPS_FIELD_OPERATION_SEPARATOR);
				position++;
			}
		}

		return strdup(arguments);
	}






	/**
	 * *********************************** Serializacion de campos MPS *******************************
	 */


	int serializePayloadOperationName(char * operationName , char * serialization , int offset){
		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(2 ,"Serialzando Payload.OperationName (nombre de la operacion invocada): " , operationName));

		memcpy(serialization + offset , operationName ,MPS_FIELD_LENGTH_OPERATION_NAME);
		return offset + MPS_FIELD_LENGTH_OPERATION_NAME;
	}

	int serializePayloadSeparator(char * serialization , int offset){
		memcpy(serialization + offset , MPS_FIELD_OPERATION_SEPARATOR , MPS_FIELD_LENGTH_OPERATION_SEPARATOR);
		return offset + MPS_FIELD_LENGTH_OPERATION_SEPARATOR;
	}

	int serializePayloadValuesList(char * values , int valsCount , char * serialization , int offset){
		if(valsCount > 0){
			if(isDebugEnabled())
				logDebug("linux-commons" , concatAll(2 ,"Serialzando Payload.ValuesListCount (cantidad de valores) : " , itoa(valsCount)));

			memcpy(serialization + offset , itoa(valsCount) ,MPS_FIELD_LENGTH_ARGUMENTS_COUNT);
			offset += MPS_FIELD_LENGTH_ARGUMENTS_COUNT;

			offset = serializePayloadSeparator(serialization , offset);

			if(isDebugEnabled())
				logDebug("linux-commons" , concatAll(2 , "Serialzando Payload.ValuesList (valores) : " , values));
			memcpy(serialization + offset , values , strlen(values));
			offset += strlen(values);
		}else{

			if(isDebugEnabled())
				logDebug("linux-commons" ,"Serialzando Payload.ArgsCount (cantidad de argumentos) : 0" );

			memcpy(serialization + offset , "0" , MPS_FIELD_LENGTH_ARGUMENTS_COUNT);
			offset += MPS_FIELD_LENGTH_ARGUMENTS_COUNT;
		}
		return offset;
	}




	MpsStream * serializeBlock(char * descriptorId , char * operationName , char payDescriptor , FileBlock * block){

		MpsHeader header;

		memcpy(header.descriptorId , descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID);
		header.payloadSize = strlen(operationName) + (2 * MPS_FIELD_LENGTH_OPERATION_SEPARATOR) + sizeof(FileBlock);
		header.payloadDescriptor = payDescriptor;

		char * buffer = malloc(sizeof(MpsHeader) + sizeof(FileBlock) + strlen(operationName) + 2 * MPS_FIELD_LENGTH_OPERATION_SEPARATOR);
		bzero(buffer, sizeof(MpsHeader) + sizeof(FileBlock) + strlen(operationName) + 2 * MPS_FIELD_LENGTH_OPERATION_SEPARATOR);

		int offset = 0;
		memcpy(buffer , &header , sizeof(MpsHeader));
		offset += sizeof(MpsHeader);
		memcpy(buffer + offset ,operationName , strlen(operationName));
		offset += strlen(operationName);

		// -> Suplemento para mantener el mismo tamaño inicial que cuando envio solo comandos
		memcpy(buffer + offset , MPS_FIELD_OPERATION_SEPARATOR , MPS_FIELD_LENGTH_OPERATION_SEPARATOR);
		offset += MPS_FIELD_LENGTH_OPERATION_SEPARATOR;
		memcpy(buffer + offset , MPS_FIELD_OPERATION_SEPARATOR , MPS_FIELD_LENGTH_OPERATION_SEPARATOR);
		offset += MPS_FIELD_LENGTH_OPERATION_SEPARATOR;
		// -> fin de suplemento...

		memcpy(buffer + offset , block  , sizeof(FileBlock));
		offset += sizeof(FileBlock);

		return buildMpsStreamWithOffset(buffer , offset);
	}


