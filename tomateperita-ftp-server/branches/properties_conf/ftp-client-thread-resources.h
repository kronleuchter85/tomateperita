/**
 * ftp-client-thread-resources.h
 *
 * utilidades para el manejo de los recursos de 
 * cada thread.
 */

typedef HANDLE ThreadHandler ;
typedef HANDLE ThreadMutex ;

#ifndef STRUCT_CLIENT_THREAD_RESOURCES
#define STRUCT_CLIENT_THREAD_RESOURCES

	/**
	 * Estructura pasada a cada thread para que 
	 * este disponga del socket por el que se 
	 * atiende al cliente y el handler para
	 * gestionar el heap que tiene asignado.
	 */
	typedef struct {

		ListenSocket controlSocket;
		ListenSocket dataSocket;
		ThreadHandler controlThreadHandler;
		ThreadHandler dataThreadHandler;
	
		HeapHandler heapHandler;

		int transferTask;

		char * argument;
		char * resultMessage;
		char * dtpPort;


		char * relativeWorkingDir;

	} ClientThreadResource;


#endif
	

	/**
	 * funciones....
	 */

	ClientThreadResource * buildClientThreadResource(ListenSocket ls , HeapHandler h);
	void disposeClientThreadResource(ClientThreadResource * );

	