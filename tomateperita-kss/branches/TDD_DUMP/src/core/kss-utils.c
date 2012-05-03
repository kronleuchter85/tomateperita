/*
 * utils.c
 *
 *  Created on: 14/06/2011
 *      Author: gonzalo
 */
#include "linux-commons-logging.h"

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
