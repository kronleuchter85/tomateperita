#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "windows-commons-list.h"
#include "windows-commons-logging.h"
#include "windows-commons-mps.h"
#include "windows-commons-socket.h"
#include "windows-commons-mps-encoder.h"
	

	char * serializeValueList(List values , HeapHandler hh);
	
	/**
	 * ************************* Funciones de Serializacion de campos MPS ****************************
	 */



	int serializePayloadOperationName(char * operationName , char * serialization , int offset){
		memcpy(serialization + offset , operationName ,MPS_FIELD_LENGTH_OPERATION_NAME);
		return offset + MPS_FIELD_LENGTH_OPERATION_NAME;
	}

	int serializePayloadSeparator(char * serialization , int offset){
		memcpy(serialization + offset , MPS_FIELD_OPERATION_SEPARATOR , MPS_FIELD_LENGTH_OPERATION_SEPARATOR);
		return offset + MPS_FIELD_LENGTH_OPERATION_SEPARATOR;
	}

	int serializePayloadValuesList(char * values , int valsCount , char * serialization , int offset , HeapHandler hh){
		if(valsCount > 0){	
			memcpy(serialization + offset , intToString(valsCount , hh) ,MPS_FIELD_LENGTH_ARGUMENTS_COUNT);
			offset += MPS_FIELD_LENGTH_ARGUMENTS_COUNT;
			offset = serializePayloadSeparator(serialization , offset);
			memcpy(serialization + offset , values , strlen(values));
			offset += strlen(values);
		}else{
			memcpy(serialization + offset , "0" , MPS_FIELD_LENGTH_ARGUMENTS_COUNT);
			offset += MPS_FIELD_LENGTH_ARGUMENTS_COUNT;
		}
		return offset;
	}




	/*
	 * Funciones serializadoras
	 */

	MpsStream * serializeMpsMessage(MpsMessage * msg , RuntimeErrorValidator * v , HeapHandler hh){
		int offset;
		char * justRetVals ;
		int payloadLength ;
		char * data ;
		MpsHeader header;

		if(isDebugEnabled())
			logDebug("windows-commons" , "Serializando Response MPS" , hh);

		justRetVals = serializeValueList(msg->commands , hh);
		payloadLength = calculatePayloadLength(justRetVals , hh);
		data = allocateMemory(hh , MPS_FIELD_LENTH_HEADER + payloadLength );
		ZeroMemory(data , MPS_FIELD_LENTH_HEADER + payloadLength);
		
		header.payloadSize = payloadLength;
		header.payloadDescriptor = msg->statusCode;

		memcpy(header.descriptorId , msg->descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID);
		memcpy(data , &header , MPS_FIELD_LENTH_HEADER);

		offset = MPS_FIELD_LENTH_HEADER;
		offset = serializePayloadOperationName(msg->operationName , data , offset);
		offset = serializePayloadSeparator(data , offset);

		if(msg->commands != NULL){
			offset = serializePayloadValuesList(justRetVals , msg->commands->size , data , offset , hh);
		}else{
			offset = serializePayloadValuesList(justRetVals , 0 , data , offset , hh);
		}

		return buildMpsStreamWithOffset(data, offset , hh);
	}




	/*
	 * Genera una serializacion de la lista
	 */
	char * serializeValueList(List values , HeapHandler hh){

		char arguments[2048];
		Iterator * iteArgs = buildIterator(values , hh);
		int position = 0;
		ZeroMemory(arguments , 2048);
		
		while(hasMoreElements(iteArgs)){
			char * argument = next(iteArgs);

			argument = encode(argument);

			strcpy(arguments + position, argument);
			position += (int)strlen(argument);

			if(hasMoreElements(iteArgs)){
				strcpy(arguments + position , MPS_FIELD_OPERATION_SEPARATOR);
				position++;
			}
		}	
		return strdup(arguments);
	}
	

	
	MpsStream * serializeBlock(char * descriptorId , char * operationName , char payDescriptor , 
		FileBlock * block , HeapHandler hh){

		MpsHeader header;
		int offset = 0;
		char * buffer;

		memcpy(header.descriptorId , descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID);
		header.payloadSize = strlen(operationName) + sizeof(FileBlock);
		header.payloadDescriptor = payDescriptor;

		buffer = allocateMemory(hh , sizeof(MpsHeader) + sizeof(FileBlock) 
			+ strlen(operationName) + 2 * MPS_FIELD_LENGTH_OPERATION_SEPARATOR);
		ZeroMemory(buffer, sizeof(MpsHeader) + sizeof(FileBlock) 
			+ strlen(operationName)+ 2 * MPS_FIELD_LENGTH_OPERATION_SEPARATOR);

		
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

		return buildMpsStreamWithOffset(buffer , offset , hh);
	}
