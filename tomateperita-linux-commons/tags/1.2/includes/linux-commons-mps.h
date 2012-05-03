/*
 * linux-syscall-protocol.h
 *
 *  Created on: 12/06/2011
 *      Author: gonzalo
 */

#include "linux-commons-list.h"
#include "linux-commons-socket.h"
#include "linux-errors.h"

#ifndef LINUX_SYSCALL_PROTOCOL_H_
#define LINUX_SYSCALL_PROTOCOL_H_

#define MPS_FIELD_OPERATION_SEPARATOR						" "
#define MPS_FIELD_LENGTH_ARGUMENTS_COUNT					1
#define MPS_FIELD_ARGUMENT_NULL								"###"
#define MPS_FIELD_LENGTH_DESCRIPTOR_ID						16
#define MPS_FIELD_LENGTH_PAYLOAD_DESCRIPTOR					1
#define MPS_FIELD_LENGTH_PAYLOAD_LENGTH						4

#define MPS_FIELD_LENGTH_OPERATION_NAME						8
#define MPS_FIELD_LENGTH_OPERATION_SEPARATOR				1

#define MPS_FIELD_LENTTH_STANDARD_PAYLOAD_FIRST_PART		(MPS_FIELD_LENGTH_OPERATION_NAME + MPS_FIELD_LENGTH_OPERATION_SEPARATOR )
#define MPS_FIELD_LENGTH_STANDARD_PAYLOAD					(MPS_FIELD_LENTTH_STANDARD_PAYLOAD_FIRST_PART + MPS_FIELD_LENGTH_ARGUMENTS_COUNT)

#define MPS_FIELD_LENTH_HEADER								(MPS_FIELD_LENGTH_DESCRIPTOR_ID + MPS_FIELD_LENGTH_PAYLOAD_LENGTH + MPS_FIELD_LENGTH_PAYLOAD_DESCRIPTOR)

#define MPS_RESPONSE_STATUS_CODE_ERROR						"1"
#define MPS_RESPONSE_STATUS_CODE_SUCCESS					"0"

#define MPS_ENCODED_CHARACTER_SPACE							"{%S%}"
#define MPS_ENCODING_CHARACTER_SPACE						" "




	typedef struct {
		char descriptorId[16];
		char * operationName;
		List arguments;
	} MpsRequest;

	typedef struct {
		char descriptorId[16];
		char * operationName;
		char * statusCode;
		List returnValues;
	} MpsResponse;

	typedef struct {
		char * data;
		int length;
	} MpsStream;

	typedef struct {
		char descriptorId[16];
		char payloadDescriptor;
		int payloadLength;
		char * payload;
	} MpsMessage;

	 void sendMpsResponse(MpsResponse * resp , ListenSocket ls , RuntimeErrorValidator * v);
	 MpsRequest * receiveMpsRequest(ListenSocket ls , RuntimeErrorValidator * v);
	 MpsResponse * sendAndReceiveMpsMessages(ListenSocket ls , MpsRequest * req, RuntimeErrorValidator * v);
	 void sendMpsStream(ListenSocket l , MpsStream * s);

	 MpsRequest * buildMpsRequest(char * descriptorId , char * operationName , List arguments);
	 MpsResponse * buildMpsResponse(char * descriptionId , char * statusCode , char * operationName , List returnValues);
	 MpsStream * buildMpsStream(char * data);

	 MpsStream * serializeMpsRequest(MpsRequest * req, RuntimeErrorValidator * v);
	 MpsStream * serializeMpsResponse(MpsResponse * resp, RuntimeErrorValidator * v);


	/*
	 * Operaciones para obtener valores
	 * de los campos MPS
	 */

	 int getMpsPayloadLength(char * header, RuntimeErrorValidator * v);
	 char * getMpsOperationName(char * payload, RuntimeErrorValidator * v);
	 List getMpsArguments(char * payload , int args, RuntimeErrorValidator * v);
	 char * getMpsDescriptorId(char * header, RuntimeErrorValidator * v);
	 char * getMpsStatusCode(char * header, RuntimeErrorValidator * v);

	 int getMpsArgsCount(char * payload, RuntimeErrorValidator * v);


	 void replyValidationError(ListenSocket ls , MpsRequest * request ,  RuntimeErrorValidator * validator);

#endif /* LINUX_SYSCALL_PROTOCOL_H_ */
