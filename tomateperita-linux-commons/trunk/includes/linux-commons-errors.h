/*
 * linux-commons-errors.h
 *
 *  Created on: 30/06/2011
 *      Author: gonzalo
 */
#include "linux-commons.h"

#ifndef LINUX_ERRORS_H_
#define LINUX_ERRORS_H_

	enum RuntimeStatus {
		RUNTIME_STATUS_SUCCESS , RUNTIME_STATUS_FAULT
	};

	typedef struct {
		int runtimeStatus;
		char * errorDescription;
	} RuntimeErrorValidator;


	Bool hasError(RuntimeErrorValidator * );
	RuntimeErrorValidator * buildErrorSuccessValidator();
	RuntimeErrorValidator * buildErrorFaultValidator(char *);
	void setError(RuntimeErrorValidator * , char * );

#endif /* LINUX_ERRORS_H_ */
