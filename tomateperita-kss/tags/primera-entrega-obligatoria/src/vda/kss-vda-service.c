/*
 * kss-vda-service.c
 *
 *  Created on: 29/05/2011
 *      Author: gonzalo
 */
#include <stdio.h>
#include <stdlib.h>

#include "kss-vda-service.h"
#include "linux-commons-socket.h"
#include "kss-state.h"
#include "kss-internal-state.h"
#include "kss-vda-component.h"
#include "linux-commons-logging.h"

	void * runVdaServiceThread(void * arg){

		if(isInfoEnabled()){
			info( "Quedando a la espera de conecciones VDA");
		}

		ServerSocket * serverSocket = openServerConnection(getVdaListenPort());
		ListenSocket listenSocket;

		setVdaServerConnection(serverSocket);

		initializeVdas();

		do{

			listenSocket = acceptConnection(serverSocket);

			//TODO: verificar alguna tramoya MPS
			//TODO: para determinar el nombre del vda
			//TODO: ¿como se determina el nombre del vda?

			char * vdaName = "VDA1";
			int vdaId = 1;

			VdaComponent * vda = buildVdaComponent(vdaId , vdaName , listenSocket);

			addVdaComponent(vda);

		}while(isKssRunningStatus());

		//TODO: limpiar lista de conecciones vda...

		closeVdaServerConnection();

		return EXIT_SUCCESS;
	}
