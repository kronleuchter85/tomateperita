#include "socket-commons.h"
#include "memory-commons.h"
#include "vda-commons.h"
#include "vda-state.h"

	ListenSocket kssConnection;
	HeapHandler	globalHeap;
	BOOL vdaRunningStatus;
	ThreadHandler kssServiceThread;	
	BOOL continueKssThread;
	cacheMem cache;
	tDiskParm diskParameters;

	void setKssConnection(ListenSocket l){
		kssConnection = l;
	}
	void setGlobalHeap(HeapHandler h){
		globalHeap = h;
	}
	void setVdaRunningStatus(BOOL r){
		vdaRunningStatus = r;
	}
	void setKssServiceThread(ThreadHandler t){
		kssServiceThread = t;
	}
	void setContinueKssThread(BOOL b){
		continueKssThread = b;
	}

	ThreadHandler getKssServiceThread(){
		return kssServiceThread;
	}
	HeapHandler getGlobalHeap(){
		return globalHeap;
	}
	ListenSocket getKssConnection(){
		return kssConnection;
	}
	BOOL isKssConnectionOpen(){
		return (kssConnection != INVALID_SOCKET);
	}
	BOOL isContinueKssThread(){
		return continueKssThread;
	}
	BOOL isVdaRunningStatus(){
		return vdaRunningStatus;
	}

	void setVdaCache() {
			cache_Create(&cache);
	}
	cacheMem * getVdaCache(){
		return &cache;
	}

	tDiskParm * getDiskParms(){
		return &diskParameters;
	}