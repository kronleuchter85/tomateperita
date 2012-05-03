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


	void serializeDescriptorId( char * descriptorId , char * serialization , int * offset);
	void serializePayloadDescriptor(char * payDesc , char * serialization , int * offset);
	void serializePayloadLength(int payLength , char * serialization , int * offset);
	void serializePayloadOperationName(char * operationName , char * serialization , int * offset);
	void serializePayloadSeparator(char * serialization , int * offset);
	void serializePayloadValuesList(char * values , int valsCount , char * serialization , int * offset);

	char * serializeValueList(List values);
	int calculatePayloadLength(char * serializedValues);




	/*
	 * Funciones de mensajeria
	 */

	void sendMpsResponse(MpsResponse * resp , ListenSocket ls , RuntimeErrorValidator * v){
		MpsStream * stream = serializeMpsResponse(resp ,v);

		if(isDebugEnabled())
			logDebug( "linux-commons" , "Enviando una respuesta MPS");

		sendBytes(ls , stream->data , stream->length);
	}


	MpsRequest * receiveMpsRequest(ListenSocket ls , RuntimeErrorValidator * v){

		if(isDebugEnabled())
			logDebug("linux-commons" , "Reciviendo una peticion MPS");

		char * header = (char *) malloc(MPS_FIELD_LENTH_HEADER + 1);						//TODO: NEW!!
		bzero(header , MPS_FIELD_LENTH_HEADER + 1);											//TODO: NEW!!
		receiveBytes(ls , header , MPS_FIELD_LENTH_HEADER);

		char * descriptorId = getMpsDescriptorId(header, v);

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(3, "Mensjae con descriptorId: '" , descriptorId , "'") );

		int payloadSize = getMpsPayloadLength(header , v);

		char * payload = (char *) malloc(payloadSize + 1);									//TODO: NEW!
		bzero(payload , payloadSize + 1);													//TODO: NEW!

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(3, "Esperando por un payload de " , itoa(payloadSize) , " bytes por MPS") );

		receiveBytes(ls , payload , payloadSize);

		int argsCount = getMpsArgsCount(payload , v);

		List args = NULL;
		if(argsCount > 0){
			args = getMpsArguments(payload , argsCount , v);
		}

		char * operationName = getMpsOperationName(payload , v);

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(5, "Ejecutando la operacion remota " , operationName , " con " , itoa(argsCount) , " argumentos MPS") );

		return buildMpsRequest(descriptorId , operationName , args);
	}


	MpsResponse * sendAndReceiveMpsMessages(ListenSocket ls , MpsRequest * req ,RuntimeErrorValidator * v){
		MpsStream * outStream = serializeMpsRequest(req , v);

		if(isDebugEnabled())
			logDebug( "linux-commons" , "Enviando una respuesta MPS");

		sendBytes(ls , outStream->data , outStream->length);

		char * header = (char *) malloc(MPS_FIELD_LENTH_HEADER + 1);					//TODO: NEW!
		bzero(header , MPS_FIELD_LENTH_HEADER + 1);										//TODO: NEW!

		receiveBytes(ls , header , MPS_FIELD_LENTH_HEADER);

		if(isDebugEnabled())
			logDebug("linux-commons" , "Reciviendo una peticion MPS");

		char * descriptorId = getMpsDescriptorId(header , v);

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(3, "Mensjae con descriptorId: '" , descriptorId , "'") );

		int payloadSize = getMpsPayloadLength(header , v);

		char * payload = (char *) malloc(payloadSize + 1);							//TODO: NEW!
		bzero(payload , payloadSize + 1);											//TODO: NEW!

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(3, "Esperando por un payload de " , itoa(payloadSize) , " bytes por MPS") );

		receiveBytes(ls , payload , payloadSize);

		int argsCount = getMpsArgsCount(payload , v);
		char * operationName = getMpsOperationName(payload , v);

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(5, "Ejecutando la operacion remota " , operationName , " con " , itoa(argsCount) , " argumentos MPS") );

		List retValues = NULL;
		if(argsCount > 0){
			retValues = getMpsArguments(payload , argsCount , v);
		}

		char * statusCode = getMpsStatusCode(header , v);

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(2, "Recibiendo codigo de resultado: " , statusCode ) );

		return buildMpsResponse(descriptorId , statusCode , operationName , retValues);
	}

	MpsResponse * receiveMpsMessages(ListenSocket ls , MpsRequest * req ,RuntimeErrorValidator * v){

		char * header = (char *) malloc(MPS_FIELD_LENTH_HEADER + 1);
		bzero(header , MPS_FIELD_LENTH_HEADER + 1);

		receiveBytes(ls , header , MPS_FIELD_LENTH_HEADER);

		if(isDebugEnabled())
			logDebug("linux-commons" , "Reciviendo una peticion MPS");

		char * descriptorId = getMpsDescriptorId(header , v);

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(3, "Mensjae con descriptorId: '" , descriptorId , "'") );

		int payloadSize = getMpsPayloadLength(header , v);

		char * payload = (char *) malloc(payloadSize + 1);
		bzero(payload , payloadSize + 1);

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(3, "Esperando por un payload de " , itoa(payloadSize) , " bytes por MPS") );

		receiveBytes(ls , payload , payloadSize);

		int argsCount = getMpsArgsCount(payload , v);
		char * operationName = getMpsOperationName(payload , v);

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(5, "Ejecutando la operacion remota " , operationName , " con " , itoa(argsCount) , " argumentos MPS") );

		List retValues = NULL;
		if(argsCount > 0){
			retValues = getMpsArguments(payload , argsCount , v);
		}

		char * statusCode = getMpsStatusCode(header , v);

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(2, "Recibiendo codigo de resultado: " , statusCode ) );

		return buildMpsResponse(descriptorId , statusCode , operationName , retValues);
	}

	/*
	 * Funciones serializadoras
	 */

	MpsStream * serializeMpsRequest(MpsRequest * req , RuntimeErrorValidator * v){

		if(isDebugEnabled())
			logDebug("linux-commons" , "Serializando Request MPS");

		char * args = serializeValueList(req->arguments);
		int payloadLength = calculatePayloadLength(args);
		char * data = malloc( MPS_FIELD_LENTH_HEADER + payloadLength +1);						//TODO: new !!
		bzero(data , MPS_FIELD_LENTH_HEADER + payloadLength +1);								//TODO: new !!

		int offset = 0;

		serializeDescriptorId(req->descriptorId , data , &offset);
		serializePayloadDescriptor(MPS_RESPONSE_STATUS_CODE_SUCCESS , data , &offset);
		serializePayloadLength(payloadLength , data ,  &offset);
		serializePayloadOperationName(req->operationName , data , &offset);
		serializePayloadSeparator(data , &offset);

		if(req->arguments != NULL)
			serializePayloadValuesList(args , req->arguments->size , data , &offset);
		else
			serializePayloadValuesList(args , 0 , data , &offset);

		return buildMpsStream(data);
	}



	MpsStream * serializeMpsResponse(MpsResponse * resp , RuntimeErrorValidator * v){

		if(isDebugEnabled())
			logDebug("linux-commons" , "Serializando Response MPS");

		char * justRetVals = serializeValueList(resp->returnValues);
		int payloadLength = calculatePayloadLength(justRetVals);
		char * data = malloc( MPS_FIELD_LENTH_HEADER + payloadLength + 1);					//TODO: NEW!
		bzero(data , MPS_FIELD_LENTH_HEADER + payloadLength + 1);							//TODO: NEW!

		int offset = 0;

		serializeDescriptorId(resp->descriptorId , data , &offset);
		serializePayloadDescriptor(resp->statusCode , data , &offset);
		serializePayloadLength(payloadLength , data ,  &offset);
		serializePayloadOperationName(resp->operationName , data , &offset);
		serializePayloadSeparator(data , &offset);

		if(resp->returnValues != NULL)
			serializePayloadValuesList(justRetVals , resp->returnValues->size , data , &offset);
		else
			serializePayloadValuesList(justRetVals , 0 , data , &offset);

		return buildMpsStream(data);
	}



	/*
	 * ***************************************** Builders *********************************
	 */

	MpsStream * buildMpsStream(char * data){
		MpsStream * stream = malloc(sizeof(MpsStream));
		stream->data = data;
		stream->length = strlen(data);
		return stream;
	}

	MpsRequest * buildMpsRequest(char * descriptorId , char * operationName , List arguments){
		MpsRequest * req = malloc(sizeof(MpsRequest));

		bzero(req->descriptorId , sizeof(req->descriptorId));
		strcpy(req->descriptorId , descriptorId);
		req->operationName = operationName;
		req->arguments = arguments;

		return req;
	}

	MpsResponse * buildMpsResponse(char * descriptionId , char * statusCode ,
			char * operationName , List returnValues){

		MpsResponse * respo = (MpsResponse *)malloc(sizeof(MpsResponse));

		bzero(respo->descriptorId , sizeof(respo->descriptorId));
		strcpy(respo->descriptorId , descriptionId);
		respo->operationName = operationName;
		respo->statusCode = statusCode;
		respo->returnValues = returnValues;

		return respo;
	}


	/**
	 * Funciones de obtencion de datos y utilidad
	 */

	int getMpsPayloadLength(char * header, RuntimeErrorValidator * v){

		char payLength[MPS_FIELD_LENGTH_PAYLOAD_LENGTH];
		bzero(payLength , MPS_FIELD_LENGTH_PAYLOAD_LENGTH);
		memcpy(payLength ,
				header + MPS_FIELD_LENGTH_DESCRIPTOR_ID + MPS_FIELD_LENGTH_PAYLOAD_DESCRIPTOR ,
				MPS_FIELD_LENGTH_PAYLOAD_LENGTH);
		int paySize = atoi(payLength);
		return paySize;
	}

	char * getMpsOperationName(char * payload, RuntimeErrorValidator * v){
		return strndup(payload , MPS_FIELD_LENGTH_OPERATION_NAME);
	}

	List getMpsArguments(char * payload , int argsCount, RuntimeErrorValidator * v){

		char * searchingString = payload + MPS_FIELD_LENGTH_STANDARD_PAYLOAD + MPS_FIELD_LENGTH_OPERATION_SEPARATOR;

		if(argsCount > 0){
			List args = buildSringsList();

			if(argsCount == 1){

				char * value = decode(strdup(searchingString));
				addNode(args , value);

			}else{

				List tokenized = tokenize(searchingString , ' ');
				Iterator * ite = buildIterator(tokenized);
				while(hasMoreElements(ite)){
					char * arg = decode(next(ite));
					addNode(args , arg);
				}
			}
			return args;
		}

		return NULL;
	}

	char * getMpsDescriptorId(char * header, RuntimeErrorValidator * v){
		return strndup(header , MPS_FIELD_LENGTH_DESCRIPTOR_ID);
	}

	char * getMpsStatusCode(char * header, RuntimeErrorValidator * v){
		return strndup(header + MPS_FIELD_LENGTH_DESCRIPTOR_ID , MPS_FIELD_LENGTH_PAYLOAD_DESCRIPTOR);
	}

	int getMpsArgsCount(char * payload, RuntimeErrorValidator * v){
		char * count = strndup(payload + MPS_FIELD_LENTTH_STANDARD_PAYLOAD_FIRST_PART , MPS_FIELD_LENGTH_ARGUMENTS_COUNT);
		return atoi(count);
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

	int calculatePayloadLength(char * serializedValues){
		int payloadLength = MPS_FIELD_LENGTH_STANDARD_PAYLOAD ;
		if( !equalsStrings(trim(serializedValues) , "")){
			payloadLength += MPS_FIELD_LENGTH_OPERATION_SEPARATOR + strlen(serializedValues);
		}

		return payloadLength;
	}




	/**
	 * *********************************** Serializacion de campos MPS *******************************
	 */



	void serializeDescriptorId( char * descriptorId , char * serialization , int * offset){
		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(3 ,"Serialzando DescriptorId: '" , strndup(descriptorId , 16) , "'"));

		memcpy(serialization , descriptorId , MPS_FIELD_LENGTH_DESCRIPTOR_ID);
		*offset += MPS_FIELD_LENGTH_DESCRIPTOR_ID;
	}

	void serializePayloadDescriptor(char * payDesc , char * serialization , int * offset){
		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(2 ,"Serialzando PayloadDescriptor: " , payDesc));

		memcpy(serialization + *offset , payDesc , MPS_FIELD_LENGTH_PAYLOAD_DESCRIPTOR );
		*offset += MPS_FIELD_LENGTH_PAYLOAD_DESCRIPTOR;
	}

	void serializePayloadLength(int payLength , char * serialization , int * offset){
		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(2 ,"Serialzando PayloadLength: " , serializeInt(payLength , MPS_FIELD_LENGTH_PAYLOAD_LENGTH)));

		memcpy(serialization + *offset , serializeInt(payLength , MPS_FIELD_LENGTH_PAYLOAD_LENGTH) , MPS_FIELD_LENGTH_PAYLOAD_LENGTH);
		*offset += MPS_FIELD_LENGTH_PAYLOAD_LENGTH;
	}

	void serializePayloadOperationName(char * operationName , char * serialization , int * offset){
		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(2 ,"Serialzando Payload.OperationName (nombre de la operacion invocada): " , operationName));

		memcpy(serialization + *offset , operationName ,MPS_FIELD_LENGTH_OPERATION_NAME);
		*offset += MPS_FIELD_LENGTH_OPERATION_NAME;
	}

	void serializePayloadSeparator(char * serialization , int * offset){
		memcpy(serialization + *offset , MPS_FIELD_OPERATION_SEPARATOR , MPS_FIELD_LENGTH_OPERATION_SEPARATOR);
			*offset += MPS_FIELD_LENGTH_OPERATION_SEPARATOR;
	}

	void serializePayloadValuesList(char * values , int valsCount , char * serialization , int * offset){
		if(valsCount > 0){
			if(isDebugEnabled())
				logDebug("linux-commons" , concatAll(2 ,"Serialzando Payload.ValuesListCount (cantidad de valores) : " , itoa(valsCount)));

			memcpy(serialization + *offset , itoa(valsCount) ,MPS_FIELD_LENGTH_ARGUMENTS_COUNT);
			*offset += MPS_FIELD_LENGTH_ARGUMENTS_COUNT;

			serializePayloadSeparator(serialization , offset);

			if(isDebugEnabled())
				logDebug("linux-commons" , concatAll(2 , "Serialzando Payload.ValuesList (valores) : " , values));
			memcpy(serialization + *offset , values , strlen(values));

		}else{

			if(isDebugEnabled())
				logDebug("linux-commons" ,"Serialzando Payload.ArgsCount (cantidad de argumentos) : 0" );

			memcpy(serialization + *offset , "0" , MPS_FIELD_LENGTH_ARGUMENTS_COUNT);
			*offset += MPS_FIELD_LENGTH_ARGUMENTS_COUNT;
		}
	}



	void replyValidationError(ListenSocket ls , MpsRequest * request , RuntimeErrorValidator * validator){
		MpsResponse * resp = buildMpsResponse(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_ERROR,
				request->operationName ,
				buildStringsListWithValues(1, validator->errorDescription));
		sendMpsResponse(resp , ls , validator);
	}


