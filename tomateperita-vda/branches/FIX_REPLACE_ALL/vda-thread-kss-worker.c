#include "vda-state.h"
#include "socket-commons.h"
#include "commons.h"
#include "logging.h"
#include "commons-mps.h"
#include "vda-entrypoint.h"
#include "windows-commons-remote-operations.h"



	void executeOperation(MpsRequest * req , ListenSocket ls , RuntimeErrorValidator * v);


	unsigned __stdcall doKssService( void* pArguments ){

		MpsRequest * request;
		RuntimeErrorValidator * validator ;
		
		if(isDebugEnabled()) debug( "Atendiendo peticiones Kss");

		do{
			validator = buildErrorSuccessValidator(getGlobalHeap());
			request = receiveMpsRequest(getKssConnection() , validator , getGlobalHeap());
			executeOperation(request , getKssConnection() , validator);

			/*
			 * recibir mensaje MPS y procesarlo
			 */
		}while( isContinueKssThread() );

		/*
		 * liberar recursos ocupados antes de finalizar
		 * el thread
		 */
		return EXIT_SUCCESS;
	}
	

		
	void executeOperation(MpsRequest * req , ListenSocket ls , RuntimeErrorValidator * v){
		if(equalsStrings(req->operationName , VDA_SECTORS_GETTING)){
			executeOperationGetSectors(req , ls , v);
		}else if(equalsStrings(req->operationName , VDA_SECTORS_PUTTING)){
			executeOperationPutSectors(req , ls , v);
		}else if(equalsStrings(req->operationName , VDA_CHS_GETTING)){
			executeOperationGetChs(req , ls , v);
		}
	}