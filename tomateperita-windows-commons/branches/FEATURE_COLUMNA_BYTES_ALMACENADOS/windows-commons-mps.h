/*
 * linux-syscall-protocol.h
 *
 *  Created on: 12/06/2011
 *      Author: gonzalo
 */
#include "windows-commons-file.h"
#include "windows-commons-list.h"
#include "windows-commons-socket.h"
#include "windows-commons-errors.h"
#include "windows-commons.h"
#include "windows-commons-memory.h"

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


	
	#pragma pack(1)
	typedef struct {
		char descriptorId[16];
		char payloadDescriptor;
		int payloadSize;
	}MpsHeader ;

	typedef struct {
		char descriptorId[16];
		char * operationName;
		char statusCode;
		List commands;
		FileBlock * block;
	} MpsMessage;

	typedef struct {
		char * data;
		int length;
	} MpsStream;

	typedef struct{
		char * descriptorId;
		char statusCode;
	}MpsHandshake;
	



	/*
	 * Senders and receivers
	 */

	void sendMpsMessage(MpsMessage * msg , ListenSocket ls , RuntimeErrorValidator * v, HeapHandler hh);
	MpsMessage * receiveMpsMessage(ListenSocket ls , RuntimeErrorValidator * v, HeapHandler hh);
	MpsMessage * sendAndReceiveMpsMessages(ListenSocket ls , MpsMessage * msg, RuntimeErrorValidator * v, HeapHandler hh);

	void sendMpsHandshake(MpsHandshake * hand , ListenSocket l , RuntimeErrorValidator * v, HeapHandler hh);
	MpsHandshake * receiveMpsHandshake(ListenSocket l , RuntimeErrorValidator * v, HeapHandler hh);
	MpsHandshake * sendAndReceiveMpsHandshake(MpsHandshake * , ListenSocket l, RuntimeErrorValidator * v, HeapHandler hh);


	/*
	 * Blocks
	 */
	MpsMessage * sendBlockAndReceiveMpsMessage(ListenSocket ls , MpsMessage * msg, RuntimeErrorValidator * v, HeapHandler hh);
	MpsMessage * sendMpsMessageAndReceiveBlock(ListenSocket ls , MpsMessage * req, RuntimeErrorValidator * validator, HeapHandler hh);
	MpsMessage * receiveMpsBlockMessage(ListenSocket ls , RuntimeErrorValidator * validator, HeapHandler hh);
	void sendMpsBlockMessage(ListenSocket ls , MpsMessage * msg, RuntimeErrorValidator * v, HeapHandler hh);

	/*
	 * Builders
	 */

	MpsHandshake * buildMpsHandhake(char * descriptorId , char statusCode, HeapHandler hh);
	MpsMessage * buildMpsMessage(char * descriptionId , char statusCode , char * operationName , List returnValues, HeapHandler hh);
	MpsStream * buildMpsStreamWithOffset(char * data , int length, HeapHandler hh);


	/*
	 * Serializers
	 */

	MpsStream * serializeMpsMessage(MpsMessage * resp, RuntimeErrorValidator * v, HeapHandler hh);
	MpsStream * serializeBlock(char * descriptorId , char * operationName , char payDescriptor , FileBlock * block, HeapHandler hh);


	/*
	 * Utils
	 */
	
	char * getMpsOperationName(char * payload, RuntimeErrorValidator * v, HeapHandler hh);
	List getMpsArguments(char * payload , int args, RuntimeErrorValidator * v, HeapHandler);
	int getMpsArgsCount(char * payload, RuntimeErrorValidator * v, HeapHandler hh);
	void replyValidationError(ListenSocket ls , MpsMessage * request , RuntimeErrorValidator * validator , HeapHandler hh);
	int calculatePayloadLength(char * serializedValues , HeapHandler hh);
	
	
#endif /* LINUX_SYSCALL_PROTOCOL_H_ */
