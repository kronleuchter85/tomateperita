/*
 * linux-syscall-protocol.h
 *
 *  Created on: 12/06/2011
 *      Author: gonzalo
 */

#include "linux-commons-list.h"
#include "linux-commons-socket.h"
#include "linux-commons-errors.h"
#include "linux-commons-file.h"

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

#define MPS_RESPONSE_STATUS_CODE_ERROR						'1'
#define MPS_RESPONSE_STATUS_CODE_SUCCESS					'0'

#define MPS_ENCODED_CHARACTER_SPACE							"{%S%}"
#define MPS_ENCODING_CHARACTER_SPACE						" "


#define MPS_PAYLOAD_DESCRIPTOR_BINARY_BINARY				'B'
#define MPS_PAYLOAD_DESCRIPTOR_ASCII_ASCII					'A'
#define MPS_PAYLOAD_DESCRIPTOR_BINARY_ASCII					'b'
#define MPS_PAYLOAD_DESCRIPTOR_ASCII_BINARY					'a'


	/*
	 * Representa los campos del header de
	 * un mensaje MPS.
	 */
	typedef struct  __attribute__((packed)) {
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

	void sendMpsMessage(MpsMessage * msg , ListenSocket ls , RuntimeErrorValidator * v);
	MpsMessage * receiveMpsMessage(ListenSocket ls , RuntimeErrorValidator * v);
	MpsMessage * sendAndReceiveMpsMessages(ListenSocket ls , MpsMessage * msg, RuntimeErrorValidator * v);

	void sendMpsHandshake(MpsHandshake * hand , ListenSocket l , RuntimeErrorValidator * v);
	MpsHandshake * receiveMpsHandshake(ListenSocket l , RuntimeErrorValidator * v);
	MpsHandshake * sendAndReceiveMpsHandshake(MpsHandshake * , ListenSocket l, RuntimeErrorValidator * v);


	/*
	 * Blocks
	 */
	MpsMessage * sendBlockAndReceiveMpsMessage(ListenSocket ls , MpsMessage * msg, RuntimeErrorValidator * v);
	MpsMessage * sendMpsMessageAndReceiveBlock(ListenSocket ls , MpsMessage * req, RuntimeErrorValidator * validator);
	MpsMessage * receiveMpsBlockMessage(ListenSocket ls , RuntimeErrorValidator * validator);
	void sendMpsBlockMessage(ListenSocket ls , MpsMessage * msg, RuntimeErrorValidator * v);

	/*
	 * Builders
	 */

	MpsHandshake * buildMpsHandhake(char * descriptorId , char statusCode);
	MpsMessage * buildMpsMessage(char * descriptionId , char statusCode , char * operationName , List returnValues);
	MpsStream * buildMpsStreamWithOffset(char * data , int length);


	/*
	 * Serializers
	 */

	MpsStream * serializeMpsMessage(MpsMessage * resp, RuntimeErrorValidator * v);
	MpsStream * serializeBlock(char * descriptorId , char * operationName , char payDescriptor , FileBlock * block);



	/*
	 * Utils
	 */

	 char * getMpsOperationName(char * payload, RuntimeErrorValidator * v);
	 List getMpsArguments(char * payload , int args, RuntimeErrorValidator * v);
	 int getMpsArgsCount(char * payload, RuntimeErrorValidator * v);
	 void replyValidationError(ListenSocket ls , MpsMessage * request ,  RuntimeErrorValidator * validator);
	 int calculatePayloadLength(char * serializedValues);


#endif /* LINUX_SYSCALL_PROTOCOL_H_ */
