/*
 * kss-client-configuration.c
 *
 *  Created on: 05/06/2011
 *      Author: gonzalo
 */

#include <string.h>


#include "linux-commons-file.h"

#include "kss-client-utils.h"
#include "kss-client-configuration.h"

	char * kssServerHost;
	char * kssServerPort;

	void setKssServerHost(char * s){
		kssServerHost = s;
	}
	void setKssServerPort(char * s){
		kssServerPort = s;
	}

	char * getKssServerHost(){
		return kssServerHost;
	}
	char * getKssServerPort(){
		return kssServerPort;
	}

	void processKeysAndValues(char * key , char * value){

		info(key);
		info(value);

		if( !strcmp(key , KSS_CLIENT_CONFIGURATION_KEY_KSS_PORT)){
			setKssServerPort(value);
		}else if(!strcmp(key , KSS_CLIENT_CONFIGURATION_KEY_KSS_HOST)){
			setKssServerHost(value);
		}
	}

	void setupConfiguration(){
		File * file = openFile(KSS_CLIENT_CONFIGURATION_FILE_NAME );
		loadConfiguration(file , ( void (*)(char * , char * , void *)) processKeysAndValues);
	}

