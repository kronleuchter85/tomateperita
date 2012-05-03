/**
 * ftp-service.c
 *
 * Modulo ftp del servidor.
 *
 */

#include "ftp.h"


	ClientThreadResource * service(ListenSocket socket , HeapHandler h);




	/**
	 * Socket del server determinado por el host y el puerto
	 * de escucha al que los clientes se conectan.
	 */
	ListenSocket serverSocket;
	
	/**
	 *
	 */
	SortedList clientResources;

	/**
	 * Punto de comienzo del servidor.
	 * Comienza a atender peticiones
	 */
	int startService(){
				
		ListenSocket clientSocket;

		int iResult = initializeWinsock();

		printf("[LOG: Inicializando Winsock v2.2 ]\n");
		
		if(validateStartup(iResult) == RESULT_WITH_ERRORS){
			return 1;
		}	

		printf("[LOG: Abriendo sockets para recibir conecciones ]\n");
		
		serverSocket = openServerSocket(HOST , PORT);
		
		printf("[LOG: Socket abierto, conecciones entrantes en %s:%s ]\n" , HOST , PORT);
		
		if( validateSocketCreation(serverSocket) == RESULT_WITH_ERRORS){
			finalizeWinsock();
			return 1;
		}

		printf("[LOG: Server en funcionamiento ]\n");

		clientResources = createClientThreadResourcesList();

		while( isKernelStatusContinue() ){

			printf("[LOG: Aguardando por conecciones remotas ]\n" );

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
					return 1;
				}
			}

			printf("[LOG: Se recibe una peticion nueva: Socket Id=%i ]\n" , clientSocket);

			/**
			 * Se crea cada nuevo recurso para el nuevo cliente
			 * y se agrega a una lista global luego de levantarse el thread
			 * que atiende a dicho cliente.
			 * Sera responsable el thread de eliminar su recurso asignado de la lista.
			 *
			 */
			{
				HeapHandler heapHandler = createHeap(HEAP_SIZE_PER_THREAD);
				ClientThreadResource * ctr = service(clientSocket , heapHandler);

				//TODO: crear accesos sincronizados a la lista !!!
				addElement( &clientResources , createListNode(heapHandler , clientSocket , ctr));
			}
		}
		return 0;
	}
	
	ClientThreadResource * service(ListenSocket socket , HeapHandler h){
		ClientThreadResource * ctr = buildClientThreadResource(socket , h);
		ThreadHandler th = _beginthreadex( NULL, 0, &serviceClientThread, ctr, 0, NULL );
		ctr->threadHandler = th;
		return ctr;
	}

	void finalizeService(){

		printf("[LOG: Cerrando socket local %s:%s  SocketId=%i ]\n" , HOST, PORT , serverSocket);		
		
		if ( closeSocket(serverSocket) != 0 ){
			printf("[ERROR: Se produjo un error cerrando el socket %d - %s ]\n" , WSAGetLastError() , getErrorMessage());
		}

		printf("[LOG: Finalizando Winsock v2.2 ]\n");		

		if( finalizeWinsock() != 0){
			printf("[ERROR: Se produjo un error finalizando biblioteca Winsock %d - %s ]\n" , WSAGetLastError() , getErrorMessage());
		}
	}

	void releaseClientServiceResources(){
		disposeClientThreadResourcesList(&clientResources);
	}