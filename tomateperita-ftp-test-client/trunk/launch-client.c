
#include "headers-commons.h"
#include "socket-commons.h"
#include "socket-validation.h"


#define SIGNAL_KILL		"KILL_SERVER"
#define THREAD_COUNT	100
#define HOST			"localhost"
#define PORT			"27015"

	unsigned __stdcall theThread( void* pArguments ){
		int iResult;
		char * messageToSend ;
		int * index = (int*) pArguments;

		ListenSocket commSocket = openConnectionSocket(HOST , PORT );
		
		//messageToSend = SIGNAL_KILL;
		messageToSend = "HOLA COMO ANDAS! " ;
		//messageToSend = "13198239182u398u898)((/&(&//&%/&%&%$%&#%#""$#!$\"!#%$#%&/&/(/)(=)=)\\\\\\\\\\\"";

		if(commSocket == INVALID_SOCKET){
			finalizeWinsock();		
			return 1;
		}
		
		iResult = send( commSocket, messageToSend, (int)strlen(messageToSend), 0 );
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(commSocket);
			finalizeWinsock();
			return 1;
		}

		printf("[LOG: Mensaje Numero %i enviado a %s:%s en SocketId=%i ]\n" , *index , HOST, PORT , commSocket);
		
		iResult = shutdown(commSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(commSocket);
			finalizeWinsock();
			return 1;
		}
		
		closeSocket(commSocket);
		
		printf("[LOG: Poniendo a dormir el hilo por 5 segundos ]\n" );

		Sleep(5000);

		//printf("[LOG: El hilo desperto pero el thread aun no termino ]\n" );

		_endthreadex(0);
		return 0;
	}


	int main(void){
				
		int index;
		int * threadNumber ;
		HANDLE threadHandlers[THREAD_COUNT];

		initializeWinsock();

		for( index = 0 ; index < THREAD_COUNT ; index++){		
			threadNumber = malloc(sizeof(int));
			*threadNumber = index;
			threadHandlers[index] = _beginthreadex( NULL, 0, &theThread, threadNumber, 0, NULL );
			
		}

		//printf("[LOG: Esperando que terminen de ejecutarse los %i threads ]\n" , THREAD_COUNT);
		//WaitForMultipleObjects(THREAD_COUNT , threadHandlers , FALSE , INFINITE );

		//WaitForSingleObject(threadHandlers[0] , INFINITE );
		//printf("[LOG: Poniendo a esperar a que despierte el hilo ]\n" );
		WaitForMultipleObjectsEx(THREAD_COUNT , threadHandlers , TRUE , INFINITE , TRUE);
		printf("[LOG: ****************************************** Se terminaron de ejecutar los %i threads , se procede a eliminar los recursos ********************************************************** ]\n" , THREAD_COUNT);
		Sleep(5000);
		//printf("[LOG: El hilo desperto y el thread termino ]\n" );

		//Sleep(20000);
		/*for( index = 0 ; index < THREAD_COUNT ; index++){
			printf("[LOG: Cerrando threadId %i ]\n" , threadHandlers[index]);
			CloseHandle(threadHandlers[index]);
		}*/

		finalizeWinsock();

		return 0;
	}