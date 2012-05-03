/*
 *  logging.c
 *  
 *
 *  Created by Fernando Nino on 4/11/11.
 *
 *  Pendiente:
 *  - Implementar milisegundos en la timestamp del log.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "linux-commons-logging.h"

int levelEnabled;

	void setLoggingLevelEnabled(int level){
		levelEnabled = level;
	}

	int getLoggingLevelEnabled(){
		return levelEnabled;
	}

	int isDebugEnabled(){
		if(levelEnabled >= LOGGING_LEVEL_DEBUG){
			return 1;
		}else{
			return 0;
		}
	}

	int isInfoEnabled(){
		if(levelEnabled >= LOGGING_LEVEL_INFO){
			return 1;
		}else{
			return 0;
		}
	}
	
	char * getLogString(char * procName , char * type, char * desc) {
		char * logString = (char *)malloc(BUFFSIZE);
		char inst[10]; //Expandir array con implementacion de milisegundos.
		int pid = getpid();
		pthread_t tid = pthread_self();

		time_t curtime = time(NULL);
		struct tm * loctime = localtime(&curtime);
		sprintf( inst , "%d:%d:%d" , loctime->tm_hour , loctime->tm_min , loctime->tm_sec );

		sprintf( logString , "[%s] [Proc:%s] [PID:%d] [TID:%d] [Type:%s] %s\r\n" , inst,procName , pid , (int)tid , type , desc );

		return logString;
	}
	
	void writeLogString(char * procName , char * logString ){
		FILE *logfile;
		char logname[20];

		bzero( logname , sizeof(logname) );
		sprintf( logname , "../logs/%s.log" , procName );

		//logfile = fopen("../logs/tomateperita-kss.log" , "a+");
		logfile = fopen( logname , "a+");

		if ( logfile == NULL ) {
			printf("No se pudo abrir el logfile.\n");
		} else {
			fwrite( logString, strlen(logString) , 1 , logfile );
			fclose(logfile);
		}
	}
	
	void println(char * proc , char * type , char * description ){
		printf("%s" , getLogString(proc , type , description) );
	}
	
	void logInfo(char * proc , char * description ){
		if(isInfoEnabled())
			println(proc , "INFO" , description );
		writeLogString(proc , getLogString(proc , "INFO" , description));
	}

	void logError(char * proc , char * description ){
			println(proc , "ERROR" , description );
		writeLogString(proc , getLogString(proc , "ERROR" , description));
	}

	void logDebug(char * proc , char * description ){
		if(isDebugEnabled())
			println(proc , "DEBUG" , description );
		writeLogString(proc , getLogString(proc , "DEBUG" , description));
	}
