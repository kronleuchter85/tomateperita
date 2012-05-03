/**
 * ftp-service.c
 *
 * Modulo ftp del servidor.
 *
 */

#include "windows-commons.h"
#include "windows-commons-headers.h"
#include "windows-commons-socket.h"
#include "windows-commons-mps.h"
#include "windows-commons-remote-operations.h"

#include "windows-commons-memory.h"
#include "windows-commons-logging.h"

#include "ftp-client-thread-resources.h"
#include "ftp-response-builder.h"
#include "ftp-state.h"
#include "ftp-configuration.h"
#include "ftp-commons.h"
#include "ftp.h"



	ClientThreadResource * doControlService(ListenSocket socket , HeapHandler h);

	/**
	 * Socket del server determinado por el host y el puerto
	 * de escucha al que los clientes se conectan.
	 */
	ListenSocket serverSocket;
	
	
	void startService(RuntimeErrorValidator * validator , HeapHandler h){
		
		if(isInfoEnabled())
			info( "Inicializando Winsock v2.2" );

		if( initializeWinsock() ){
			setError(validator , "No se pudo inicializar Winsock");
			return;
		}
			
		if(isInfoEnabled())		
			info("Abriendo sockets para recibir conexiones" );
			
		serverSocket = openServerSocket(getServerAddress() , getServerControlPort());
		if( serverSocket == INVALID_SOCKET){
			finalizeWinsock();
			setError(validator, "No se pudo abrir el socket");
			return ;
		}		

		if(isInfoEnabled()){
			info( concat(getGlobalHeap() , 4 , 
				"Socket abierto, conexiones entrantes en " , getServerAddress() ,":", 
				getServerControlPort()));		
		}
		
		if(!isOfflineStartMode()){
			MpsHandshake * handshake;
			ListenSocket  kssConnection = openConnectionSocket(getKssAddress() , getKssPort());
		
			if(kssConnection != INVALID_SOCKET){
				handshake = buildMpsHandhake( generateRandomKey(16,h) , FTP_HANDSHAKE , h );
				handshake = sendAndReceiveMpsHandshake( handshake , kssConnection , validator , h );

				if( handshake != NULL && handshake->statusCode == MPS_RESPONSE_STATUS_CODE_SUCCESS ){
					setKssConnection(kssConnection);
					if(isInfoEnabled()){
					info( concat(getGlobalHeap() , 4, 
						"Proceso FTPS conectado con proceso remoto Kss en host " , 
						getKssAddress() , ":" , getKssPort()));
					}
				} else {
					setError( validator , "Hubo un problema y no se realizo el handshake.");
				}
			} else {
				if(isInfoEnabled()){
					info( "Proceso FTPS trabajando en modo offline. Fallo la conexion con servicio KSS");
				}
			}
		}

		if(isInfoEnabled()){
			info("Inicializacion completada" );
			info("Server en funcionamiento" );
		}	

		if(isDebugEnabled())
			debug( "Aguardando conexiones de clientes FTP" );

		while( isKernelStatusContinue() ){
	
			ListenSocket clientSocket = acceptRequest(serverSocket);		

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
				} else {
					setError(validator , "Se ha producido un error en la aceptacion de conecciones");
					return ;
				}
			}

			if(isDebugEnabled()){
				debug( concat(getGlobalHeap() , 2 , "Se recibe una peticion nueva: Socket Id=" , 
					intToString(clientSocket , getGlobalHeap())) );
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
			}
		}	
	}
	
	ClientThreadResource * doControlService(ListenSocket socket , HeapHandler h){
		ClientThreadResource * ctr = buildClientThreadResource(socket , h);
		ThreadHandler th = _beginthreadex( NULL, 0, &serviceControlThread, ctr, 0, NULL );
		ctr->controlThreadHandler = th;
		return ctr;
	}

