/*
 * linux-syscall-protocol.c
 *
 *  Created on: 12/06/2011
 *      Author: gonzalo
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "commons-list-2.h"
#include "logging.h"
#include "commons-mps.h"
#include "socket-commons.h"
#include "windows-commons-mps-encoder.h"

	void serializeDescriptorId( char * descriptorId , char * serialization , int * offset);
	void serializePayloadDescriptor(char * payDesc , char * serialization , int * offset);
	void serializePayloadLength(int payLength , char * serialization , int * offset , HeapHandler hh);
	void serializePayloadOperationName(char * operationName , char * serialization , int * offset);
	void serializePayloadSeparator(char * serialization , int * offset);
	void serializePayloadValuesList(char * values , int valsCount , char * serialization , int * offset, HeapHandler hh);
	char * serializeValueList(List values , HeapHandler hh);
	int calculatePayloadLength(char * serializedValues , HeapHandler hh);

	
	/*
	 * *************************** Funciones de mensajeria ********************************
	 */


	void sendMpsResponse(MpsResponse * resp , ListenSocket ls , RuntimeErrorValidator * v , HeapHandler hh){
		MpsStream * stream = serializeMpsResponse(resp ,v , hh);	
		sendBytes(ls , stream->data , stream->length);
	}

	MpsRequest * receiveMpsRequest(ListenSocket ls , RuntimeErrorValidator * v , HeapHandler hh){
	
		char * header;
		char * descriptorId ;
		int payloadSize;
		char * payload;
		List args = NULL;
		int argsCount ;
		char * operationName;

		//todo: new!!
		//header = (char *) allocateMemory(hh , MPS_FIELD_LENTH_HEADER);
		header = (char *) allocateMemory(hh , MPS_FIELD_LENTH_HEADER + 1);
		ZeroMemory(header , MPS_FIELD_LENTH_HEADER + 1);
		
		
		receiveBytes(ls , header , MPS_FIELD_LENTH_HEADER);
		descriptorId = getMpsDescriptorId(header, v , hh);
		payloadSize = getMpsPayloadLength(header , v );
		
		//todo: new!!
		//payload = (char *) allocateMemory(hh ,payloadSize);
		payload = (char *) allocateMemory(hh ,payloadSize + 1);
		ZeroMemory(payload , payloadSize + 1);


		receiveBytes(ls , payload , payloadSize);
		argsCount = getMpsArgsCount(payload , v, hh);
		
		if(argsCount > 0){
			args = getMpsArguments(payload , argsCount , v , hh);
		}
		operationName = getMpsOperationName(payload , v , hh);

		return buildMpsRequest(descriptorId , operationName , args , hh);
	}

	MpsResponse * sendAndReceiveMpsMessages(ListenSocket ls , MpsRequest * req ,RuntimeErrorValidator * v , HeapHandler hh){
		MpsStream * outStream = serializeMpsRequest(req , v , hh);

		char * header ;
		char * descriptorId ;
		int payloadSize;
		char * payload ;
		int argsCount;
		char * operationName;
		char * statusCode ;
		List retValues = NULL;

		sendBytes(ls , outStream->data , outStream->length);
		
		//todo: new!!
		//header = (char *) allocateMemory(hh ,MPS_FIELD_LENTH_HEADER);
		header = (char *) allocateMemory(hh ,MPS_FIELD_LENTH_HEADER+1);
		ZeroMemory( header , MPS_FIELD_LENTH_HEADER + 1);


		receiveBytes(ls , header , MPS_FIELD_LENTH_HEADER);
		descriptorId = getMpsDescriptorId(header , v , hh);
		payloadSize = getMpsPayloadLength(header , v);

		//todo: new!!
		//payload = (char *) allocateMemory(hh ,payloadSize);
		payload = (char *) allocateMemory(hh ,payloadSize+1);
		ZeroMemory(payload , payloadSize + 1);
		
		receiveBytes(ls , payload , payloadSize);
		argsCount = getMpsArgsCount(payload , v , hh);
		operationName = getMpsOperationName(payload , v , hh);
		
		if(argsCount > 0){
			retValues = getMpsArguments(payload , argsCount , v , hh);
		}
		statusCode = getMpsStatusCode(header , v , hh);

		return buildMpsResponse(descriptorId , statusCode , operationName , retValues ,hh);
	}




	/*
	 *  **************************** Funciones serializadoras ****************************
	 */

	MpsStream * serializeMpsRequest(MpsRequest * req , RuntimeErrorValidator * v , HeapHandler hh){

		char * args = serializeValueList(req->arguments , hh);
		int payloadLength = calculatePayloadLength(args , hh);
		int offset = 0;
		//todo: new!!
		//char * data = allocateMemory(hh , MPS_FIELD_LENTH_HEADER + payloadLength);
		char * data = allocateMemory(hh , MPS_FIELD_LENTH_HEADER + payloadLength + 1);
		ZeroMemory(data , MPS_FIELD_LENTH_HEADER + payloadLength +1);

		

		serializeDescriptorId(req->descriptorId , data , &offset);
		serializePayloadDescriptor(MPS_RESPONSE_STATUS_CODE_SUCCESS , data , &offset);
		serializePayloadLength(payloadLength , data ,  &offset , hh);
		serializePayloadOperationName(req->operationName , data , &offset);
		serializePayloadSeparator(data , &offset);

		if(req->arguments != NULL)
			serializePayloadValuesList(args , req->arguments->size , data , &offset , hh);
		else
			serializePayloadValuesList(args , 0 , data , &offset , hh);

		return buildMpsStream(data , hh);
	}

	MpsStream * serializeMpsResponse(MpsResponse * resp , RuntimeErrorValidator * v , HeapHandler hh){

		char * justRetVals = serializeValueList(resp->returnValues , hh);
		int payloadLength = calculatePayloadLength(justRetVals , hh);
		
		int offset = 0;

		//todo: new!
		//char * data = allocateMemory(hh , MPS_FIELD_LENTH_HEADER + payloadLength);
		char * data = allocateMemory(hh , MPS_FIELD_LENTH_HEADER + payloadLength + 1);
		ZeroMemory(data , MPS_FIELD_LENTH_HEADER + payloadLength +1);
		
	
		serializeDescriptorId(resp->descriptorId , data , &offset);
		serializePayloadDescriptor(resp->statusCode , data , &offset);
		serializePayloadLength(payloadLength , data ,  &offset , hh);
		serializePayloadOperationName(resp->operationName , data , &offset);
		serializePayloadSeparator(data , &offset);

		if(resp->returnValues != NULL)
			serializePayloadValuesList(justRetVals , resp->returnValues->size , data , &offset , hh);
		else
			serializePayloadValuesList(justRetVals , 0 , data , &offset , hh);

		return buildMpsStream(data , hh);
	}





	/*
	 * ***************************************** Builders *********************************
	 */

	MpsStream * buildMpsStream(char * data , HeapHandler hh){
		MpsStream * stream = allocateMemory(hh ,sizeof(MpsStream));
		stream->data = data;
		stream->length = strlen(data);
		return stream;
	}

	MpsRequest * buildMpsRequest(char * descriptorId , char * operationName , List arguments , HeapHandler hh){
		MpsRequest * req = allocateMemory(hh ,sizeof(MpsRequest));
		req->descriptorId = descriptorId;
		req->operationName = operationName;
		req->arguments = arguments;

		return req;
	}

	MpsResponse * buildMpsResponse(char * descriptionId , char * statusCode ,
			char * operationName , List returnValues , HeapHandler hh){

		MpsResponse * respo = (MpsResponse *)allocateMemory(hh ,sizeof(MpsResponse));
		respo->descriptorId = descriptionId;
		respo->operationName = operationName;
		respo->statusCode = statusCode;
		respo->returnValues = returnValues;

		return respo;
	}





	/**
	 *  ********************** Funciones de obtencion de datos MPS y utilidad ***********************
	 */

	int getMpsPayloadLength(char * header, RuntimeErrorValidator * v){
		int paySize;
		char payLength[MPS_FIELD_LENGTH_PAYLOAD_LENGTH];
		ZeroMemory(payLength , MPS_FIELD_LENGTH_PAYLOAD_LENGTH);
		memcpy(payLength ,
				header + MPS_FIELD_LENGTH_DESCRIPTOR_ID + MPS_FIELD_LENGTH_PAYLOAD_DESCRIPTOR ,
				MPS_FIELD_LENGTH_PAYLOAD_LENGTH);
		paySize = atoi(payLength);
		return paySize;
	}

	char * getMpsOperationName(char * payload, RuntimeErrorValidator * v , HeapHandler hh){
		return subString(payload , 0 , MPS_FIELD_LENGTH_OPERATION_NAME -1 , hh);
	}

	List getMpsArguments(char * payload , int argsCount, RuntimeErrorValidator * v , HeapHandler hh){

		char * searchingString = payload + MPS_FIELD_LENGTH_STANDARD_PAYLOAD + MPS_FIELD_LENGTH_OPERATION_SEPARATOR;

		if(argsCount > 0){
			List args = buildSringsList(hh);

			if(argsCount == 1){
				char * value = decode(strdup(searchingString) );
				addNode(args , value , hh);

			} else{
				List tokenized = tokenize(searchingString , ' ' , hh);
				Iterator * ite = buildIterator(tokenized, hh);
				while(hasMoreElements(ite)){
					char * arg = decode(next(ite));
					addNode(args , arg, hh);
				}
			}
			return args;
		}
		return NULL;
	}

	char * getMpsDescriptorId(char * header, RuntimeErrorValidator * v , HeapHandler hh){
		return subString(header , 0 , MPS_FIELD_LENGTH_DESCRIPTOR_ID-1 , hh);
	}

	char * getMpsStatusCode(char * header, RuntimeErrorValidator * v , HeapHandler hh){
		return subString(header + MPS_FIELD_LENGTH_DESCRIPTOR_ID ,0 , MPS_FIELD_LENGTH_PAYLOAD_DESCRIPTOR-1 , hh);
	}

	int getMpsArgsCount(char * payload, RuntimeErrorValidator * v , HeapHandler hh){
		char * count = subString(payload + MPS_FIELD_LENTTH_STANDARD_PAYLOAD_FIRST_PART , 0 , MPS_FIELD_LENGTH_ARGUMENTS_COUNT-1 , hh);
		return atoi(count);
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
			position += strlen(argument);

			if(hasMoreElements(iteArgs)){
				strcpy(arguments + position , MPS_FIELD_OPERATION_SEPARATOR);
				position++;
			}
		}	
		return strdup(arguments);
	}
	
	int calculatePayloadLength(char * serializedValues , HeapHandler hh){
		int payloadLength = MPS_FIELD_LENGTH_STANDARD_PAYLOAD ;
		if( !equalsStrings(trim(serializedValues , hh) , "")){
			payloadLength += MPS_FIELD_LENGTH_OPERATION_SEPARATOR + strlen(serializedValues);
		}

		return payloadLength;
	}





	/**
	 * ************************* Funciones de Serializacion de campos MPS ****************************
	 */

	void serializeDescriptorId( char * descriptorId , char * serialization , int * offset){
		memcpy(serialization , descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID);
		*offset += MPS_FIELD_LENGTH_DESCRIPTOR_ID;
	}

	void serializePayloadDescriptor(char * payDesc , char * serialization , int * offset){
		memcpy(serialization + *offset , payDesc , MPS_FIELD_LENGTH_PAYLOAD_DESCRIPTOR );
		*offset += MPS_FIELD_LENGTH_PAYLOAD_DESCRIPTOR;
	}

	void serializePayloadLength(int payLength , char * serialization , int * offset , HeapHandler hh){
		memcpy(serialization + *offset , serializeInt(payLength , MPS_FIELD_LENGTH_PAYLOAD_LENGTH , hh) , MPS_FIELD_LENGTH_PAYLOAD_LENGTH);
		*offset += MPS_FIELD_LENGTH_PAYLOAD_LENGTH;
	}

	void serializePayloadOperationName(char * operationName , char * serialization , int * offset){
		memcpy(serialization + *offset , operationName ,MPS_FIELD_LENGTH_OPERATION_NAME);
		*offset += MPS_FIELD_LENGTH_OPERATION_NAME;
	}

	void serializePayloadSeparator(char * serialization , int * offset){
		memcpy(serialization + *offset , MPS_FIELD_OPERATION_SEPARATOR , MPS_FIELD_LENGTH_OPERATION_SEPARATOR);
			*offset += MPS_FIELD_LENGTH_OPERATION_SEPARATOR;
	}

	void serializePayloadValuesList(char * values , int valsCount , char * serialization , int * offset , HeapHandler hh){
		if(valsCount > 0){
			memcpy(serialization + *offset , intToString(valsCount , hh) ,MPS_FIELD_LENGTH_ARGUMENTS_COUNT);
			*offset += MPS_FIELD_LENGTH_ARGUMENTS_COUNT;
			serializePayloadSeparator(serialization , offset);
			strcpy(serialization + *offset , values);
		}else{
			memcpy(serialization + *offset , "0" , MPS_FIELD_LENGTH_ARGUMENTS_COUNT);
			*offset += MPS_FIELD_LENGTH_ARGUMENTS_COUNT;
		}
	}


	void replyValidationError(ListenSocket ls , MpsRequest * request , RuntimeErrorValidator * validator , HeapHandler hh){
		MpsResponse * resp = buildMpsResponse(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_ERROR,
				request->operationName ,
				buildStringsListWithValues(hh, 1, validator->errorDescription),  hh);
		sendMpsResponse(resp , ls , validator , hh);
	}
