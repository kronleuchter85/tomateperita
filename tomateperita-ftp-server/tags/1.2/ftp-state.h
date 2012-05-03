/**
 * ftp-state.h
 *
 * Header para las funciones del 
 * Heap global y funciones para el KSS.
 *
 */

#include "socket-commons.h"
#include "memory-commons.h"
#include "ftp-constants.h"

	HeapHandler getGlobalHeap();
	void setGlobalHeap( HeapHandler );

	void setKssConnection( ListenSocket );
	ListenSocket getKssConnection();

	int getKernelStatus();
	void setKernelStatus( int );
	int isKernelStatusContinue();
	void startKernel();