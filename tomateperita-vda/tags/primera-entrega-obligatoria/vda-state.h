#include "socket-commons.h"
#include "memory-commons.h"


#ifndef _W_VDA_STATE_H_
#define _W_VDA_STATE_H_

	typedef HANDLE ThreadHandler;

	/**
	 * Heap Global
	 */
	void setGlobalHeap(HeapHandler h);
	HeapHandler getGlobalHeap();
	
	/**
	 * Socket kss
	 */
	ListenSocket getKssConnection();
	void setKssConnection(ListenSocket l);
	BOOL isKssConnectionOpen();


	/**
	 * Thread kss
	 */
	BOOL isContinueKssThread();
	void setContinueKssThread(BOOL b);
	void setKssServiceThread(ThreadHandler);
	ThreadHandler getKssServiceThread();

#endif