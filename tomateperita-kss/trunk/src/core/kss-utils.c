/*
 * utils.c
 *
 *  Created on: 14/06/2011
 *      Author: gonzalo
 */
#include "linux-commons-logging.h"
#include "linux-commons-remote-services.h"

#include "kss-vda-service.h"
#include "kss-vda-state.h"

	void info(char * m){
		logInfo("KSS" , m);
	}

	void debug(char * m){
		logDebug("KSS" , m);
	}

	void error(char * m){
		logError("KSS" , m);
	}



	List createVdasList(){
		Bool equaly(VdaComponent * v , VdaComponent * z){
			return (equalsStrings(v->vdaName , z->vdaName));
		}
		Bool sorting(VdaComponent * v , VdaComponent * z){
			return TRUE;
		}

		return createList( NULL,
				(Bool (*)(void *, void *))equaly ,
				(Bool (*)(void *, void *)) sorting);
	}



	void doHandshake(ListenSocket clientSocket , char code ,  RuntimeErrorValidator * validator){

		MpsHandshake * handshake = receiveMpsHandshake(clientSocket , validator);

		if(handshake != NULL && handshake->statusCode == code){

			handshake->statusCode = MPS_RESPONSE_STATUS_CODE_SUCCESS;
			sendMpsHandshake(handshake , clientSocket , validator);

			if(hasError(validator)){
				setError(validator , "No se ha logrado realizar el handshake.");
				return;
			}

			info(concatAll(2 , "Handshake realizado satisfactoriamente. DescriptorId: " , handshake->descriptorId));
		}else{
			setError(validator , "No se ha logrado realizar el handshake.");
		}
	}
