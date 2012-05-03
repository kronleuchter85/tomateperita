/*
 * linux-errors.h
 *
 *  Created on: 30/06/2011
 *      Author: gonzalo
 */

#include "memory-commons.h"
#include "headers-commons.h"

#ifndef LINUX_ERRORS_H_
#define LINUX_ERRORS_H_

	enum RuntimeStatus {
		RUNTIME_STATUS_SUCCESS , RUNTIME_STATUS_FAULT
	};

	typedef struct {
		int runtimeStatus;
		char * errorDescription;
	} RuntimeErrorValidator;


	BOOL hasError(RuntimeErrorValidator * );
	RuntimeErrorValidator * buildErrorSuccessValidator( HeapHandler);
	RuntimeErrorValidator * buildErrorFaultValidator(char *, HeapHandler);
	void setError(RuntimeErrorValidator * , char * );

#endif /* LINUX_ERRORS_H_ */
