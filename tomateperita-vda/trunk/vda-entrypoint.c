#include "windows-commons.h"
#include "windows-commons-mps.h"

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

			//List returnValues = buildStringsListWithValues(getGlobalHeap() , 2 , 
			//	_strdup(tfs->sectorContent1) , _strdup(tfs->sectorContent2));

			MpsMessage * resp = buildMpsMessage(req->descriptorId , 
				MPS_RESPONSE_STATUS_CODE_SUCCESS , 
				req->operationName ,
				NULL , getGlobalHeap());
			
			resp->block = buildEmptyFileBlock(getGlobalHeap());
			setFileBlockSectors(resp->block , atoi(sector1) , atoi(sector2));
			setFileBlockContents(resp->block , tfs->sectorContent1 , tfs->sectorContent2 , 
				SECTOR_SIZE , SECTOR_SIZE);

			sendMpsBlockMessage(ls,  resp , v , getGlobalHeap());
		}
	}

	void executeOperationPutSectors(MpsMessage * req , ListenSocket ls , RuntimeErrorValidator * v){

		int sector1 = req->block->sector1.sector;
		char * content1 = req->block->sector1.data;
		int sector2 = req->block->sector2.sector;
		char * content2 = req->block->sector2.data;

		if( req->block == NULL || sector1<0 ){
			
			setError(v, "El primer sector debe ser valido");
			replyValidationError(ls , req , v , getGlobalHeap());

		}else{
			int bytesWriten = putSectores(sector1 , content1 , sector2 , content2);

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