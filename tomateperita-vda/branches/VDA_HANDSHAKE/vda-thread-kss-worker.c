
#include "windows-commons-socket.h"
#include "windows-commons.h"
#include "windows-commons-logging.h"
#include "windows-commons-mps.h"
#include "windows-commons-remote-operations.h"

#include "vda-entrypoint.h"
#include "vda-state.h"



	void executeOperation(MpsMessage * req , ListenSocket ls , RuntimeErrorValidator * v);


	unsigned __stdcall doKssService( void* pArguments ){

		MpsMessage * request;
		RuntimeErrorValidator * validator ;
		
		if(isDebugEnabled()) debug( "Atendiendo peticiones Kss");

		do{
			validator = buildErrorSuccessValidator(getGlobalHeap());
			request = receiveMpsMessage(getKssConnection() , validator , getGlobalHeap());
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
	

		
	void executeOperation(MpsMessage * req , ListenSocket ls , RuntimeErrorValidator * v){
		if(equalsStrings(req->operationName , VDA_SECTORS_GETTING)){
			executeOperationGetSectors(req , ls , v);
		}else if(equalsStrings(req->operationName , VDA_SECTORS_PUTTING)){
			executeOperationPutSectors(req , ls , v);
		}else if(equalsStrings(req->operationName , VDA_CHS_GETTING)){
			executeOperationGetChs(req , ls , v);
		}
	}