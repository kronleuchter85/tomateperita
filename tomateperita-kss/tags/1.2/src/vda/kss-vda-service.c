/*
 * kss-vda-service.c
 *
 *  Created on: 29/05/2011
 *      Author: gonzalo
 */
#include <stdio.h>
#include <stdlib.h>

#include "linux-commons-logging.h"
#include "linux-commons-mps.h"
#include "linux-commons-list.h"
#include "linux-commons-socket.h"

#include "kss-vda-service.h"
#include "kss-state.h"
#include "kss-utils.h"
#include "kss-configuration.h"
#include "kss-vda-state.h"



	void * runVdaServiceThread(void * arg){

		if(isInfoEnabled()){
			info( "Quedando a la espera de conecciones VDA");
		}

		ServerSocket * serverSocket = openServerConnection(getVdaListenPort());
		ListenSocket listenSocket;

		//no se para q se esta haciendo esto!
		//setVdaServerConnection(serverSocket);

		createConnectedVdas();
		createMountedVdas();

		MpsRequest * mpsRequest = NULL;
		MpsResponse * response = NULL;
		VdaComponent * vdaComponent = NULL;
		RuntimeErrorValidator * validator = NULL;
		Iterator * paramsIterator = NULL;

		do{

			listenSocket = acceptConnection(serverSocket);

			//TODO: aca estaria faltando el handshake

			/*
			 * Este mpsRequest contiene la informacion del VDA
			 * que el cliente envia en la primera coneccion del mismo.
			 */
			mpsRequest = receiveMpsRequest(listenSocket , validator);
			paramsIterator = buildIterator(mpsRequest->arguments);
			char * vdaName = hasMoreElements(paramsIterator)? next(paramsIterator) : NULL;

			vdaComponent = buildVdaComponent(vdaName , listenSocket);

			if(vdaComponent == NULL){
				setError(validator , "El nombre del Vda es nulo");
				replyValidationError(listenSocket , mpsRequest , validator);
			}else{
				response = buildMpsResponse(
					mpsRequest->descriptorId ,
					MPS_RESPONSE_STATUS_CODE_SUCCESS ,
					mpsRequest->operationName , NULL);

				sendMpsResponse(response , listenSocket , validator);
				addConnectedVda(vdaComponent);
			}

		}while(isKssRunningStatus());

		//TODO: limpiar lista de conecciones vda...
		//closeVdaServerConnection();

		return EXIT_SUCCESS;
	}
