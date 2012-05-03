/**
 * ftp-client-thread-resources.h
 *
 * utilidades para el manejo de los recursos de 
 * cada thread.
 */

#include "memory-commons.h"
#include "socket-commons.h"
#include "list-commons.h"
#include "ftp-client-thread-resources.h"
#include "ftp-constants.h"

	ClientThreadResource * buildClientThreadResource(ListenSocket ls , HeapHandler h){
		ClientThreadResource * ctr = allocateMemory(h , sizeof(ClientThreadResource));
		ctr->controlSocket = ls;
		ctr->heapHandler = h;
		ctr->mode = FTP_DATA_MODE_INACTIVE;
		ctr->dataMutex = CreateMutex(NULL , FALSE , NULL);
		ctr->controlMutex = CreateMutex(NULL , FALSE , NULL);
		return ctr;
	}

	void disposeClientThreadResource( ClientThreadResource * ctr){
		HeapHandler hh = ctr->heapHandler;
		closeSocket(ctr->controlSocket);
		closeSocket(ctr->dataSocket);
		CloseHandle(ctr->controlThreadHandler);		
		CloseHandle(ctr->dataThreadHandler);
		CloseHandle(ctr->controlMutex);
		CloseHandle(ctr->dataMutex);
		freeMemory( hh , ctr);			
	}

	SortedList createClientThreadResourcesList(){
		return createSortedList();
	}

	HeapHandler getDisposerHandler(ClientThreadResource * ln){
		return ln->heapHandler;
	}

	void disposeClientThreadResourcesList(SortedList * list){
		deleteSortedList(list , &getDisposerHandler , &disposeClientThreadResource);
	}

