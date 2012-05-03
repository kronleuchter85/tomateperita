/*
 * fss-configuration.c
 *
 *  Created on: 22/06/2011
 *      Author: gonzalo
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "linux-commons-file.h"
#include "linux-commons-logging.h"
#include "linux-commons.h"
#include "linux-commons-errors.h"

#include "fss-utils.h"
#include "fss-configuration.h"

	char * kssHost;
	char * kssPort;
	char * dataDirectory;

	void setKssHost(char * p){
		kssHost = p;
	}
	void setKssPort(char * p){
		kssPort = p;
	}
	void setDataDirectory(char * d){
		dataDirectory = d;
	}
	char * getKssPort(){
		return kssPort;
	}
	char * getKssHost(){
		return kssHost;
	}
	char * getDataDirectory(){
		return dataDirectory;
	}


	void setFssLoggingLevel(char * v){
		if(equalsStrings(v , LOGGING_LEVEL_STRING_DEBUG)){
			setLoggingLevelEnabled(LOGGING_LEVEL_DEBUG);
		}else if(equalsStrings(v , LOGGING_LEVEL_STRING_INFO)){
			setLoggingLevelEnabled(LOGGING_LEVEL_INFO);
		}else if (equalsStrings(v , LOGGING_LEVEL_STRING_ERROR)){
			setLoggingLevelEnabled(LOGGING_LEVEL_ERROR);
		}
	}

	void processKeysAndValues(char * key , char * value , void * storage){

		if(isInfoEnabled()) info(concatAll(4, "Seteando ", key , "=" , value));

		if(equalsStrings(key , FSS_CONFIGURATION_LOGGING_LEVEL)){
			setFssLoggingLevel(value);
		}
		if(equalsStrings(key , FSS_CONFIGURATION_KSS_HOST)){
			setKssHost(value);
		}
		if(equalsStrings(key , FSS_CONFIGURATION_KSS_PORT)){
			setKssPort(value);
		}
		if(equalsStrings(key , FSS_CONFIGURATION_DATA_DIRECTORY)){
			setDataDirectory(value);
		}
	}

	void setupConfiguration(RuntimeErrorValidator * v){

		File * file = openFile(FSS_CONFIGURATION_FILE_NAME);

		if(file == NULL){
			setError(v, "No existe el archivo de configuracion");
			return;
		}

		loadConfiguration(file, processKeysAndValues);
	}
