/*
 *  file-config.c
 *  
 *
 *  Created by Fernando Nino on 4/11/11.
 *
 *  
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linux-commons-file.h"
#include "linux-commons.h"

#define FILE_LINE_LENGTH 100


	void loadConfiguration(char * configFile , void (*processKeysAndValues)(char * , char *)){
		FILE * file = fopen(configFile , "r");

		if(file == NULL){
			//TODO: sacar este printline
			printf("no existe el archivo de configuracion\n %s" , configFile);
			return;
		}

		char lineBuffer[FILE_LINE_LENGTH];
		while(fgets(lineBuffer , FILE_LINE_LENGTH , file)){
			char * key = getKey(lineBuffer);
			char * value = getValue(lineBuffer);
			if(key != NULL && value != NULL)
				processKeysAndValues(key , value);
		}
	}

	char * getKey(char * line ){
		char * formated = trim(line);
		int indexEqual = getIndexOf(formated , '=');
		if(indexEqual > 0){
			return getSubString(formated , 0 , indexEqual -1);
		}else{
			return NULL;
		}
	}

	char * getValue(char * line){
		char * formated = trim(line);
		int indexEqual = getIndexOf(formated , '=');
		if(indexEqual > 0){
			return getSubString(formated , indexEqual +1 , strlen(formated)-1);
		}else{
			return NULL;
		}
	}



	/*

	#define BUFFSIZE 150

	char * getClientIp(){
		FILE * arch;
		char linea[100+2], buffer[BUFFSIZE];
		char * dato;
		char * ip = (char *)malloc(20);

		if ((arch=fopen("config.ini", "r"))==NULL){
			printf("Error al arbrir el archivo de configuracion.\n");
			exit(EXIT_FAILURE);
		}

		while (fgets(linea, 100, arch)){
			dato = strstr(linea, ":");
			dato++;
			dato++;

			if (strstr (linea,"IP")!=NULL){
				strcpy(ip,dato);
				return ip;
			}
		}
		return NULL;
	}

	int getClientPort(){
		FILE * arch;
		char linea[100+2], buffer[BUFFSIZE];
		char * dato;
		int port;

		if ((arch=fopen("config.ini", "r"))==NULL){
			printf("Error al arbrir el archivo de configuracion.\n");
			exit(EXIT_FAILURE);
		}

		while (fgets(linea, 100, arch)){
			dato = strstr(linea, ":");
			dato++;
			dato++;

			if (strstr (linea,"Port")!=NULL){
				port = atoi(dato);
				return port;
			}
		}
		return 0;
	}

	int getLoggingLevel(){
		FILE * arch;
		char linea[100+2], buffer[BUFFSIZE];
		char * dato;
		int loggingLevel;

		if ((arch=fopen("config.ini", "r"))==NULL){
			printf("Error al arbrir el archivo de configuracion.\n");
			exit(EXIT_FAILURE);
		}

		while (fgets(linea, 100, arch)){
			dato = strstr(linea, ":");
			dato++;
			dato++;

			if (strstr (linea,"LoggingLevel")!=NULL){
				loggingLevel = atoi(dato);
				return loggingLevel;
			}
		}
		return 0;
	}
	*/

