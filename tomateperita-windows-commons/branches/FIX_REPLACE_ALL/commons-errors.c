/*
 * linux-errors.c
 *
 *  Created on: 30/06/2011
 *      Author: gonzalo
 */
#include <stdlib.h>

#include "commons-errors.h"

	BOOL hasError(RuntimeErrorValidator * r){
		return (r->runtimeStatus == RUNTIME_STATUS_FAULT);
	}
	RuntimeErrorValidator * buildErrorSuccessValidator(HeapHandler hh){
		RuntimeErrorValidator * r = (RuntimeErrorValidator *) allocateMemory(hh , sizeof(RuntimeErrorValidator));
		r->runtimeStatus = RUNTIME_STATUS_SUCCESS;
		r->errorDescription = NULL;
		return r;
	}
	RuntimeErrorValidator * buildErrorFaultValidator(char * d, HeapHandler hh){
		RuntimeErrorValidator * r = (RuntimeErrorValidator *)  allocateMemory(hh ,sizeof(RuntimeErrorValidator));
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

