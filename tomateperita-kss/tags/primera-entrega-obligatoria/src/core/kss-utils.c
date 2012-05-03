/*
 * utils.c
 *
 *  Created on: 14/06/2011
 *      Author: gonzalo
 */
#include "linux-commons-logging.h"

	void info(char * m){
		logInfo("KSS" , m);
	}

	void debug(char * m){
		logDebug("KSS" , m);
	}

	void error(char * m){
		logError("KSS" , m);
	}
