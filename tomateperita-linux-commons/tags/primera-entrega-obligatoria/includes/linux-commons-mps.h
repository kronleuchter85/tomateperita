/*
 * linux-syscall-protocol.h
 *
 *  Created on: 12/06/2011
 *      Author: gonzalo
 */

#include "linux-commons-socket.h"


#ifndef LINUX_SYSCALL_PROTOCOL_H_
#define LINUX_SYSCALL_PROTOCOL_H_


	typedef struct {

		char idDescriptor[16];
		char payloadDescriptor;
		long payloadLenght;
		char payload[1030];

	} MpsMessage;

	enum SysCall {
		SYS_READ, SYS_WRITE, SYS_OPEN, SYS_CLOSE, SYS_FLUSH, SYS_FLIST
	};

	enum SysCallResult {
		SUCCESS_RESULT,
		FAULT_RESULT
	};

	int remoteCallSysOpen(ListenSocket l, char mode , char * path );
	char * remoteCallSysRead(ListenSocket l, int fd );
	void remoteCallSysWrite(ListenSocket l, int fd , char * buffer , int * status);
	void remoteCallSysClose(ListenSocket l, int fd , int * status);
	void remoteCallSysFlush(ListenSocket l, int fd , int * status);

	MpsMessage buildMessage(long idDescriptor, int payloadDescriptor,
			char * payload);


#endif /* LINUX_SYSCALL_PROTOCOL_H_ */
