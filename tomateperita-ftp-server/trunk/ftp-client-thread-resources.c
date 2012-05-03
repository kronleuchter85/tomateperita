/**
 * ftp-client-thread-resources.h
 *
 * utilidades para el manejo de los recursos de 
 * cada thread.
 */

#include "windows-commons-memory.h"
#include "windows-commons-socket.h"


#include "ftp-response-builder.h"
#include "ftp-client-thread-resources.h"


	ClientThreadResource * buildClientThreadResource(ListenSocket ls , HeapHandler h){
		ClientThreadResource * ctr = allocateMemory(h , sizeof(ClientThreadResource));
		ctr->controlSocket = ls;
		ctr->heapHandler = h;		
		return ctr;
	}


	void disposeClientThreadResource( ClientThreadResource * ctr){
		HeapHandler hh = ctr->heapHandler;
		closeSocket(ctr->controlSocket);
		closeSocket(ctr->dataSocket);
		CloseHandle(ctr->controlThreadHandler);		
		CloseHandle(ctr->dataThreadHandler);		
		freeMemory( hh , ctr);			
	}


	HeapHandler getDisposerHandler(ClientThreadResource * ln){
		return ln->heapHandler;
	}

	