/**
 * ftp-client-thread-resources.h
 *
 * utilidades para el manejo de los recursos de 
 * cada thread.
 */

#include "memory-commons.h"
#include "socket-commons.h"
#include "list-commons.h"

typedef HANDLE ThreadHandler ;

#ifndef STRUCT_CLIENT_THREAD_RESOURCES
#define STRUCT_CLIENT_THREAD_RESOURCES

	/**
	 * Estructura pasada a cada thread para que 
	 * este disponga del socket por el que se 
	 * atiende al cliente y el handler para
	 * gestionar el heap que tiene asignado.
	 */
	typedef struct {
		ListenSocket socket;
		HeapHandler heapHandler;
		ThreadHandler threadHandler;
	} ClientThreadResource;


#endif
	

	/**
	 * funciones....
	 */

	ClientThreadResource * buildClientThreadResource(ListenSocket ls , HeapHandler h);
	void disposeClientThreadResource(ClientThreadResource * );

	SortedList createClientThreadResourcesList();
	void disposeClientThreadResourcesList(SortedList *);

	