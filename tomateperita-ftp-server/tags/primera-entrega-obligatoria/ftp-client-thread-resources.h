/**
 * ftp-client-thread-resources.h
 *
 * utilidades para el manejo de los recursos de 
 * cada thread.
 */
#include "list-commons.h"

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
		
		/**
		 * Control...
		 */
		ListenSocket controlSocket;
		ThreadHandler controlThreadHandler;
		ThreadMutex controlMutex;
		
		/**
		 * Datos ...
		 */
		ListenSocket dataSocket;
		ThreadHandler dataThreadHandler;
		ThreadMutex dataMutex;

		HeapHandler heapHandler;

		/**
		 * Modo de funcionamiento, ACTIVO, PASIVO o INACTIVO
		 */
		int mode;
		int transferTask;

		/**
		 * Comando listo para ser ejecutado...
		 */
		char * commandToExecute;
		char * argument;

		/**
		 * Resultado de la ejecucion...
		 */
		char * resultMessage;

		char * remoteHost;
		char * remotePort;

	} ClientThreadResource;


#endif
	

	/**
	 * funciones....
	 */

	ClientThreadResource * buildClientThreadResource(ListenSocket ls , HeapHandler h);
	void disposeClientThreadResource(ClientThreadResource * );

	SortedList createClientThreadResourcesList();
	void disposeClientThreadResourcesList(SortedList *);

	