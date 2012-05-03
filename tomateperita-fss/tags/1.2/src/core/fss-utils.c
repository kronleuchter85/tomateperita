/*
 * fss-utils.c
 *
 *  Created on: 22/06/2011
 *      Author: gonzalo
 */


#include "linux-commons-logging.h"

	void info(char * m){
		logInfo("FSS" , m);
	}

	void debug(char * m){
		logDebug("FSS" , m);
	}

	void error(char * m){
		logError("FSS" , m);
	}

