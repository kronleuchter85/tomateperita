/*
 * linux-errors.c
 *
 *  Created on: 30/06/2011
 *      Author: gonzalo
 */
#include <stdlib.h>

#include "linux-commons.h"
#include "linux-errors.h"

	Bool hasError(RuntimeErrorValidator * r){
		return (r->runtimeStatus == RUNTIME_STATUS_FAULT);
	}
	RuntimeErrorValidator * buildErrorSuccessValidator(){
		RuntimeErrorValidator * r = (RuntimeErrorValidator *) malloc(sizeof(RuntimeErrorValidator));
		r->runtimeStatus = RUNTIME_STATUS_SUCCESS;
		r->errorDescription = NULL;
		return r;
	}
	RuntimeErrorValidator * buildErrorFaultValidator(char * d){
		RuntimeErrorValidator * r = (RuntimeErrorValidator *) malloc(sizeof(RuntimeErrorValidator));
		r->runtimeStatus = RUNTIME_STATUS_FAULT;
		r->errorDescription = d;
		return r;
	}

	void setError(RuntimeErrorValidator * v , char * m){
		if(v != NULL){
			v->runtimeStatus = RUNTIME_STATUS_FAULT;
			v->errorDescription = m;
		}
	}

