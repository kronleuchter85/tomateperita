/**
 * ftp-state.h
 *
 * Header para las funciones del 
 * Heap global y funciones para el KSS.
 *
 */

#include "windows-commons-socket.h"
#include "windows-commons-memory.h"



#ifndef _FTP_KERNEL_STATE_CONSTANTS_
#define _FTP_KERNEL_STATE_CONSTANTS_

#define KERNEL_STATUS_CONTINUE 1
#define KERNEL_STATUS_INTERRUPT_BY_EXTERNAL_REQUEST 2
#define KERNEL_STATUS_INTERRUPT_BY_INTERNAL_REQUEST 3

#define HEAP_SIZE_PER_THREAD 1024

#endif



	HeapHandler getGlobalHeap();
	void setGlobalHeap( HeapHandler );

	void setKssConnection( ListenSocket );
	ListenSocket getKssConnection();

	int getKernelStatus();
	void setKernelStatus( int );
	int isKernelStatusContinue();
	void startKernel();