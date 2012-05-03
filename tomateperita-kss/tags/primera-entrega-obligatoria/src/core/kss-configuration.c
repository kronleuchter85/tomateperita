/*
 * configuration.c
 *
 *  Created on: 29/05/2011
 *      Author: gonzalo
 */
#include <stdio.h>
#include <string.h>
#include "linux-commons-file.h"
#include "kss-utils.h"

#define KSS_CONFIGURATION_FILE_NAME			"../conf/tomateperita-kss.properties"

#define KSS_CONFIGURATION_KEY_FTP_PORT		"kss.configuration.ftp.listen.port"
#define KSS_CONFIGURATION_KEY_FSS_PORT		"kss.configuration.fss.listen.port"
#define KSS_CONFIGURATION_KEY_VDA_PORT		"kss.configuration.vda.listen.port"
#define KSS_CONFIGURATION_KEY_SHELL_PORT	"kss.configuration.shell.listen.port"

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

	void processKeysAndValues(char * key , char * value){

		if( !strcmp(key , KSS_CONFIGURATION_KEY_FTP_PORT)){
			setFtpsPort(value);
		}else if(!strcmp(key , KSS_CONFIGURATION_KEY_FSS_PORT)){
			setFssListenPort(value);
		}else if(!strcmp(key , KSS_CONFIGURATION_KEY_SHELL_PORT)){
			setShellPort(value);
		}else if(!strcmp(key , KSS_CONFIGURATION_KEY_VDA_PORT)){
			setVdaListenPort(value);
		}
	}

	void setupConfiguration(){
		loadConfiguration(KSS_CONFIGURATION_FILE_NAME , &processKeysAndValues);
		info("Levantando la configuracion del archivo");
	}
