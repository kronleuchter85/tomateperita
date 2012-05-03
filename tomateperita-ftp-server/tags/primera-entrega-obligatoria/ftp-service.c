/**
 * ftp-service.c
 *
 * Modulo ftp del servidor.
 *
 */

#include "commons.h"
#include "headers-commons.h"
#include "ftp-constants.h"
#include "ftp-service-validation.h"
#include "ftp-thread-worker.h"
#include "socket-commons.h"
#include "socket-validation.h"
#include "memory-commons.h"
#include "ftp-client-thread-resources.h"
#include "logging.h"
#include "ftp-protocol-constants.h"
#include "ftp-protocol-service.h"
#include "ftp-state.h"
#include "ftp-configuration.h"



	ClientThreadResource * doControlService(ListenSocket socket , HeapHandler h);

	/**
	 * Socket del server determinado por el host y el puerto
	 * de escucha al que los clientes se conectan.
	 */
	ListenSocket serverSocket;
	
	/**
	 * Lista con los recursos de los threads clientes
	 */
	SortedList clientResources;


	/**
	 * Punto de comienzo del servidor.
	 * Comienza a atender peticiones
	 */
	int startService(){
				
		ListenSocket clientSocket , kssConnection;

		int iResult = initializeWinsock();

		if(isInfoEnabled()){
			logInfo("ftp-service" , "Inicializando Winsock v2.2" , getGlobalHeap());
		}		

		if(validateStartup(iResult) == RESULT_WITH_ERRORS){
			return CLOSED_BY_FAULT;
		}	

		if(isInfoEnabled()){
			logInfo("ftp-service" , "Abriendo sockets para recibir conecciones" , getGlobalHeap());
		}
		
		serverSocket = openServerSocket(getServerAddress() , getServerControlPort());
		kssConnection = openConnectionSocket(getKssAddress() , getKssPort());
		
		if(isInfoEnabled()){
			logInfo("ftp-service" , concat(getGlobalHeap() , 4 , 
				"Socket abierto, conecciones entrantes en " , getServerAddress() ,":", 
				getServerControlPort()) , getGlobalHeap());		
		}
		
		if( validateSocketCreation(serverSocket) == RESULT_WITH_ERRORS){
			finalizeWinsock();
			return CLOSED_BY_FAULT;
		}
		
		if(isInfoEnabled()){
			logInfo("ftp-service" , "Server en funcionamiento" , getGlobalHeap());
		}	

		clientResources = createClientThreadResourcesList();

		while( isKernelStatusContinue() ){

			if(isDebugEnabled()){
				logDebug("ftp-service" , "Aguardando por conecciones remotas" , getGlobalHeap());
			}

			clientSocket = acceptRequest(serverSocket);		

			/**
			 * Estudiamos en caso de error al aceptar la coneccion del socket
			 * remoto analizando el codigo de error
			 *
			 * WSAECONNRESET: No se puede aceptar la coneccion por que el problema 
			 * lo tiene el host remoto. No es un problema grave, por lo tanto volvemos a 
			 * esperar una nueva coneccion sin modificar el funcionamiento del servidor.
			 * 
			 * WSAEINPROGRESS: Existe una funcion bloqueante de Windows Sockets 1.1 en 
			 * progreso o el service provider esta todavia procesando la funcion callback;
			 * por lo tanto volvemos a evaluar la aceptacion sin modificar el funcionamiento
			 * del servidor puesto que este error no es critico.
			 *
			 * En cualquier otro caso se finaliza el servidor
			 * (http://msdn.microsoft.com/en-us/library/ms737526(v=vs.85).aspx)
			 */
			if(clientSocket == INVALID_SOCKET){
				
				if( WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAEINPROGRESS){
					continue;
				}else {
					return CLOSED_BY_FAULT;
				}
			}

			if(isDebugEnabled()){
				logDebug("ftp-service" , concat(getGlobalHeap() , 2 , "Se recibe una peticion nueva: Socket Id=" , 
					intToString(clientSocket , getGlobalHeap())) , getGlobalHeap());
			}

			/**
			 * Se crea cada nuevo recurso para el nuevo cliente
			 * y se agrega a una lista global luego de levantarse el thread
			 * que atiende a dicho cliente.
			 * Sera responsable el thread de eliminar su recurso asignado de la lista.
			 *
			 */
			{
				HeapHandler heapHandler = createHeap(HEAP_SIZE_PER_THREAD);
				ClientThreadResource * ctr = doControlService(clientSocket , heapHandler);

				//TODO: crear accesos sincronizados a la lista !!!
				addElement( &clientResources , createListNode(heapHandler , clientSocket , ctr));
			}
		}
		return CLOSED_SUCCESSFULLY;
	}
	
	ClientThreadResource * doControlService(ListenSocket socket , HeapHandler h){
		ClientThreadResource * ctr = buildClientThreadResource(socket , h);
		ThreadHandler th = _beginthreadex( NULL, 0, &serviceControlThread, ctr, 0, NULL );
		ctr->controlThreadHandler = th;
		return ctr;
	}

	void finalizeService(){

		if(isInfoEnabled()){
			logInfo("ftp-service" , concat(getGlobalHeap() , 4 , "Cerrando socket local " , 
				intToString(serverSocket , getGlobalHeap()) , " en " , 
				concat(getGlobalHeap() , 3 , getServerAddress() ,":" , getServerControlPort())) , 
				getGlobalHeap());
		}
			
		if ( closeSocket(serverSocket) != 0 ){
			logError("ftp-service" , concat(getGlobalHeap() , 2 , 
				"Se produjo un error cerrando el socket. Codigo de error: " , 
				WSAGetLastError()) , getGlobalHeap());
		}

		if(isInfoEnabled()){
			logInfo("ftp-service" , "Finalizando Winsock v2.2 " , getGlobalHeap());
		}

		if( finalizeWinsock() != 0){
			logError("ftp-service" , concat(getGlobalHeap() , 2 , "Se produjo un error finalizando biblioteca Winsock. Codigo de error: " , WSAGetLastError()) , getGlobalHeap());
		}
	}

	void releaseClientServiceResources(){
		disposeClientThreadResourcesList(&clientResources);
	}