#include "windows-commons.h"

#include "vda-api.h"
#include "vda-entrypoint.h"
#include "vda-commons.h"
#include "vda-state.h"


	
	void executeOperationGetSectors(MpsMessage * req , ListenSocket ls , RuntimeErrorValidator * v){
		Iterator * iteSectors = buildIterator(req->commands , getGlobalHeap());
		char * sector1 = hasMoreElements(iteSectors)?next(iteSectors) : NULL;
		char * sector2 = hasMoreElements(iteSectors)?next(iteSectors) : NULL;

		if(sector1 == NULL || sector2 == NULL || atoi(sector1)< 0 || atoi(sector2) < 0 ){
			setError(v , "Los numeros de sectores indicados no son validos");
			replyValidationError(ls , req , v , getGlobalHeap());
		}else{

			GettingTransferSector * tfs = getSectores(atoi(sector1) , atoi(sector2));

			List returnValues = buildStringsListWithValues(getGlobalHeap() , 2 , 
				_strdup(tfs->sectorContent1) , _strdup(tfs->sectorContent2));

			MpsMessage * resp = buildMpsMessage(req->descriptorId , 
				MPS_RESPONSE_STATUS_CODE_SUCCESS , 
				req->operationName ,
				returnValues , getGlobalHeap());

			sendMpsMessage(resp,  ls , v , getGlobalHeap());
		}
	}

	void executeOperationPutSectors(MpsMessage * req , ListenSocket ls , RuntimeErrorValidator * v){
		Iterator * iteSectors = buildIterator(req->commands , getGlobalHeap());
		char * sector1 = hasMoreElements(iteSectors)?next(iteSectors) : NULL;
		char * content1 = hasMoreElements(iteSectors)?next(iteSectors) : NULL;
		char * sector2 = hasMoreElements(iteSectors)?next(iteSectors) : NULL;
		char * content2 = hasMoreElements(iteSectors)?next(iteSectors) : NULL;
		
		if(sector1 == NULL || atoi(sector1)<0 ){
			
			setError(v, "El primer sector debe ser valido");
			replyValidationError(ls , req , v , getGlobalHeap());

		}else{
			int bytesWriten = putSectores(atoi(sector1) , content1 , atoi(sector2) , content2);

			if(bytesWriten <= 0){
				setError(v , "No se han escrito bytes por algun motivo");
				replyValidationError(ls , req , v , getGlobalHeap());
			}else{

				MpsMessage * resp = buildMpsMessage(req->descriptorId , 
					MPS_RESPONSE_STATUS_CODE_SUCCESS , 
					req->operationName ,
					NULL , getGlobalHeap());

				sendMpsMessage(resp,  ls , v , getGlobalHeap());
			}
		}			
	}

	void executeOperationGetChs(MpsMessage * req , ListenSocket ls , RuntimeErrorValidator * v){

		infoCHS * infoChs = getCHS();
		char * cilinders = intToString(infoChs->cilinders , getGlobalHeap());
		char * heads = intToString(infoChs->heads, getGlobalHeap());
		char * sectors = intToString(infoChs->sectors, getGlobalHeap());

		List chs = buildStringsListWithValues(getGlobalHeap() , 3 , cilinders , heads , sectors);
		
		MpsMessage * resp = buildMpsMessage(req->descriptorId , 
				MPS_RESPONSE_STATUS_CODE_SUCCESS , 
				req->operationName ,
				chs , getGlobalHeap());

		info("Ejecutando Get CHS");

		sendMpsMessage(resp,  ls , v , getGlobalHeap());
	}