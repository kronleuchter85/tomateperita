/*
 * kss-client-utils.c
 *
 *  Created on: 22/06/2011
 *      Author: gonzalo
 */

#include <string.h>

#include "linux-commons-logging.h"
#include "linux-commons.h"
#include "linux-commons-list.h"

#include "kss-client-commands.h"

	void info(char * message){
		logInfo("Kss-Client" , message);
	}

	void debug(char * message){
		logDebug("Kss-Client" , message);
	}

	void error(char * message){
		logError("Kss-Client" , message);
	}


	List getFirstParameter(char * message){
		char param1[20];

		printf("%s: " , message);
		scanf("%s" , param1);
		printf("\n--------------------------------------------------------------------------------------\n");

		List params = buildSringsList();

		addNode(params , trim(strdup(param1)));

		return params;
	}
