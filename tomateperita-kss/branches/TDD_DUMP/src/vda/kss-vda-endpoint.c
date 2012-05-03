/*
 * kss-vda-endpoint.c
 *
 *  Created on: 16/07/2011
 *      Author: gonzalo
 */

#include <stdlib.h>

#include "linux-commons-mps.h"
#include "linux-commons-remote-services.h"
#include "linux-commons-file.h"

#include "kss-utils.h"
#include "kss-tdd.h"
#include "kss-vda-service.h"
#include "kss-vda-state.h"
#include "kss-core-services.h"


	void callVdaOperationPutSectores(VdaComponent * vdaComp ,
			FileBlock * fileBlock ,
			RuntimeErrorValidator * validator){

		if(fileBlock->sector1.sector < 0 || fileBlock->sector1.data == NULL ){
			setError(validator , "El primer sector a ingresar debe ser valido");
			return;
		}

		List arguments = NULL;

		if(hasTwoSectors(fileBlock)){

			arguments = buildStringsListWithValues(4 ,
				itoa(fileBlock->sector1.sector ), fileBlock->sector1.data ,
				itoa(fileBlock->sector2.sector ), fileBlock->sector2.data );
		}else{
			arguments = buildStringsListWithValues(2 , itoa(fileBlock->sector1.sector) , fileBlock->sector1.data );
		}

		MpsRequest * req = buildMpsRequest(generateRandomKey(16) , VDA_SECTORS_PUTTING , arguments);

		MpsResponse * response = sendAndReceiveMpsMessages(vdaComp->listenSocket , req , validator);

		if(!equalsStrings(response->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			Iterator * ite = buildIterator(response->returnValues);
			char * errorMessage = hasMoreElements(ite)? next(ite) : NULL;
			setError(validator , errorMessage);
		}else{
			debug("La operacion PutSectores se ejecuto correctamente");
		}
	}



	FileBlock * callVdaOperationGetSectores(VdaComponent * vdaComp , int sector1 , int sector2 , RuntimeErrorValidator * validator){

		if(sector1 < 0 || sector2 < 0){
			setError(validator , "Alguno de los sectores es un numero negativo");
			return NULL;
		}

		List arguments = buildStringsListWithValues(2 , itoa(sector1) , itoa(sector2));
		MpsRequest * req = buildMpsRequest(generateRandomKey(16) , VDA_SECTORS_GETTING , arguments);
		MpsResponse * response = sendAndReceiveMpsMessages(vdaComp->listenSocket , req , validator);
		Iterator * ite = buildIterator(response->returnValues);

		if(!equalsStrings(response->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			char * errorMessage = hasMoreElements(ite)?next(ite):NULL;
			setError(validator , errorMessage);
			return NULL;
		}else{
			char * content1 = hasMoreElements(ite) ? next(ite) : NULL;
			char * content2 = hasMoreElements(ite) ? next(ite) : NULL;

			FileBlock * block = buildEmptyFileBlock();
			setFileBlockSectors(block ,sector1 , sector2);
			setFileBlockContents(block , content1 , content2);
			return block;
		}
	}

	VdaInformation * callVdaOperationGetChs(VdaComponent * vdaComp , RuntimeErrorValidator * validator){
		MpsRequest * req = buildMpsRequest(generateRandomKey(16) , VDA_CHS_GETTING , NULL);
		MpsResponse * response = sendAndReceiveMpsMessages(vdaComp->listenSocket , req , validator);
		if(!equalsStrings(response->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			Iterator * ite = buildIterator(response->returnValues);
			char * errorMessage = hasMoreElements(ite)?next(ite):NULL;
			setError(validator , errorMessage);
			return NULL;
		}else{
			return buildVdaInformation(vdaComp->vdaName , response->returnValues);
		}
	}


