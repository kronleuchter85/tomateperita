/*
 * configuration.c
 *
 *  Created on: 29/05/2011
 *      Author: gonzalo
 */
#include <stdio.h>
#include <string.h>

#include "linux-commons-logging.h"
#include "linux-commons-file.h"

#include "kss-utils.h"
#include "kss-configuration.h"

	char * shellListenPort;
	char * ftpsListenPort;
	char * vdaListenPort;
	char * fssListenPort;

	char * getShellPort(){
		return shellListenPort;
	}

	void setShellPort(char * sp){
		shellListenPort = sp;
	}

	char * getFtpsPort(){
		return ftpsListenPort;
	}

	void setFtpsPort(char * fp){
		ftpsListenPort = fp;
	}

	void setVdaListenPort(char * v){
		vdaListenPort = v;
	}

	char * getVdaListenPort(){
		return vdaListenPort;
	}

	char * getFssListenPort(){
		return fssListenPort;
	}
	void setFssListenPort(char * l){
		fssListenPort = l;
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

		if( !strcmp(key , KSS_CONFIGURATION_KEY_FTP_PORT)){
			setFtpsPort(value);
		}else if(!strcmp(key , KSS_CONFIGURATION_KEY_FSS_PORT)){
			setFssListenPort(value);
		}else if(!strcmp(key , KSS_CONFIGURATION_KEY_SHELL_PORT)){
			setShellPort(value);
		}else if(!strcmp(key , KSS_CONFIGURATION_KEY_VDA_PORT)){
			setVdaListenPort(value);
		}else if(equalsStrings(key , KSS_CONFIGURATION_LOGGING_LEVEL)){
			setFssLoggingLevel(value);
		}

	}

	void setupConfiguration(RuntimeErrorValidator * validator){

		if(!existsFile(KSS_CONFIGURATION_FILE_NAME)){
			setError(validator , concatAll(2 ,"No existe el archivo de configuracion no: " , KSS_CONFIGURATION_FILE_NAME));
			return;
		}

		File * file = openFile(KSS_CONFIGURATION_FILE_NAME);
		loadConfiguration(file , &processKeysAndValues);
		info("Levantando la configuracion del archivo");
	}
