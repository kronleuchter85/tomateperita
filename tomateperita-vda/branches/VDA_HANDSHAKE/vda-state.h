#include "windows-commons-socket.h"
#include "windows-commons-memory.h"

#include "vda-cache.h"
#include "vda-disk-functions.h"

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

	/**
	 * VDA Status
	 */	
	void setVdaRunningStatus(BOOL r);
	BOOL isVdaRunningStatus();
	void setVdaRunningStatus(BOOL r);

	/**
	 * Cache
	 */	
	void setVdaCache();
	cacheMem * getVdaCache();

	/**
	 * Parametros del disco
	 */	
	tDiskParm * getDiskParms();

#endif