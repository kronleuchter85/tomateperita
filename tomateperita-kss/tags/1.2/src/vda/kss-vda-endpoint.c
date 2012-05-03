/*
 * kss-vda-endpoint.c
 *
 *  Created on: 16/07/2011
 *      Author: gonzalo
 */

#include "linux-commons-mps.h"
#include "linux-commons-remote-services.h"

#include "kss-utils.h"
#include "kss-vda-service.h"
#include "kss-vda-state.h"


	void callVdaOperationPutSectores(VdaComponent * vdaComp ,
			int sector1 ,  char * content1 , int sector2 , char * content2 ,
			RuntimeErrorValidator * validator){

		if(sector1 < 0 || content1 == NULL ){
			setError(validator , "El primer sector a ingresar debe ser valido");
			return;
		}

		List arguments = buildStringsListWithValues(4 , itoa(sector1)  , content1 , itoa(sector2) , content2 );
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



	List callVdaOperationGetSectores(VdaComponent * vdaComp , int sector1 , int sector2 , RuntimeErrorValidator * validator){

		if(sector1 < 0 || sector2 < 0){
			setError(validator , "Alguno de los sectores es un numero negativo");
			return NULL;
		}

		List arguments = buildStringsListWithValues(2 , itoa(sector1) , itoa(sector2));
		MpsRequest * req = buildMpsRequest(generateRandomKey(16) , VDA_SECTORS_GETTING , arguments);
		MpsResponse * response = sendAndReceiveMpsMessages(vdaComp->listenSocket , req , validator);

		if(!equalsStrings(response->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			Iterator * ite = buildIterator(response->returnValues);
			char * errorMessage = hasMoreElements(ite)?next(ite):NULL;
			setError(validator , errorMessage);
			return NULL;
		}else{
			return response->returnValues;
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


