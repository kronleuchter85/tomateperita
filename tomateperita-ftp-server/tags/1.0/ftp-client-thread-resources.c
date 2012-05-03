/**
 * ftp-client-thread-resources.h
 *
 * utilidades para el manejo de los recursos de 
 * cada thread.
 */

#include "ftp-client-thread-resources.h"
#include "socket-commons.h"
#include "list-commons.h"

	ClientThreadResource * buildClientThreadResource(ListenSocket ls , HeapHandler h){
		ClientThreadResource * ctr = allocateMemory(h , sizeof(ClientThreadResource));
		ctr->socket = ls;
		ctr->heapHandler = h;
		return ctr;
	}

	void disposeClientThreadResource( ClientThreadResource * ctr){
		HeapHandler hh = ctr->heapHandler;
		closeSocket(ctr->socket);
		CloseHandle(ctr->threadHandler);		
		freeMemory( hh , ctr);
		
		//FIXED: BUG de la liberacion de recursos 3/5/2011
		//destroyHeap(hh);		
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

