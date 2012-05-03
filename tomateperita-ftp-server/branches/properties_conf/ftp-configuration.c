/**
 * Configuracion general que mantiene el estado
 * de lo que se cargo desde el archivo.
 *
 */

#include "windows-commons-headers.h"
#include "windows-commons-file.h"
#include "windows-commons-logging.h"
#include "windows-commons.h"

#include "ftp-state.h"
#include "ftp-commons.h"
#include "ftp-configuration.h"



	BOOL anonymousAccess = TRUE;
	BOOL offlineStartMode = TRUE;

	char * dataDirectory = NULL;
	char * serverDataPort = NULL;
	char * serverControlPort = NULL;
	char * serverAddress = NULL;
	char * loggingDirectory = NULL;
	
	char * loggingLevel = NULL;
	char * kssAddress = NULL;
	char * kssPort = NULL;
	
	int maxRetriesFindingDataPort = 0;

	void configureLoggingLevel(char * level){
		if( !strcmp("debug" , trim(level , getGlobalHeap()))){
			setLoggingLevelEnabled(LOGGING_LEVEL_DEBUG);
		}else if( !strcmp("error" , trim(level , getGlobalHeap()))){
			setLoggingLevelEnabled(LOGGING_LEVEL_ERROR);
		}else if( !strcmp("info" , trim(level , getGlobalHeap()))){
			setLoggingLevelEnabled(LOGGING_LEVEL_INFO);
		}
	}


	void setAnonymousAccess(BOOL s){
		anonymousAccess = s;
	}
	BOOL isAnonymousAccess(){
		return anonymousAccess;
	}


	void setDataDirectory(char * d){
		dataDirectory = d;
	}
	char * getDataDirectory(){
		return dataDirectory;
	}


	void setServerDataPort(char * p){
		serverDataPort = p;
	}
	char * getServerDataPort(){
		return serverDataPort;
	}


	void setServerControlPort(char * p){
		serverControlPort = p;
	}
	char * getServerControlPort(){
		return serverControlPort;
	}


	void setServerAddress(char * a){
		serverAddress = a;
	}
	char * getServerAddress(){
		return serverAddress;
	}


	void setLoggingDirectory(char * d){
		loggingDirectory = d;
	}
	char * getLoggingDirectory(){
		return loggingDirectory;
	}



	void setOfflineStartMode(BOOL b){
		offlineStartMode = b;
	}
	BOOL isOfflineStartMode(){
		return offlineStartMode;
	}


	void setLoggingLevel(char * l){
		loggingLevel = l;
		configureLoggingLevel(l);
	}
	char * getLoggingLevel(){
		return loggingLevel;
	}

	void setMaxRetriesFindingDataPort(int retries){
		maxRetriesFindingDataPort = retries;
	}

	int getMaxRetriesFindingDataPort(){
		return maxRetriesFindingDataPort;
	}

	void setKssAddress(char * a){
		kssAddress = a;
	}

	char * getKssAddress(){
		return kssAddress;
	}

	void setKssPort(char * p){
		kssPort = p;
	}

	char * getKssPort(){
		return kssPort;
	}

	void processConfiguration(char * key , char * value){

		if( equalsStrings(key , FTPS_CONFIGURATION_KEY_FTPS_LOGLEVEL) ){
			setLoggingLevel(value);
		}
		if( equalsStrings(key , FTPS_CONFIGURATION_KEY_FTPS_DATADIR) ){
			setDataDirectory(value);
		}
		if( equalsStrings(key , FTPS_CONFIGURATION_KEY_FTPS_CONTROLPORT) ){
			setServerControlPort(value);
		}
		if( equalsStrings(key , FTPS_CONFIGURATION_KEY_FTPS_DATAPORT) ){
			setServerDataPort(value);
		}
		if( equalsStrings(key , FTPS_CONFIGURATION_KEY_FTPS_ADDRESS) ){
			setServerAddress(value);
		}
		if( equalsStrings(key , FTPS_CONFIGURATION_KEY_FTPS_LOGDIR) ){
			setLoggingDirectory(value);
		}
		if( equalsStrings(key , FTPS_CONFIGURATION_KEY_MAX_RETRIES) ){
			setMaxRetriesFindingDataPort( atoi(value) );
		}
		if( equalsStrings(key , FTPS_CONFIGURATION_KEY_FTPS_STARTMODE) ){
			if(!strcmp("true" , value)){
				setOfflineStartMode(TRUE);
			}else{
				setOfflineStartMode(FALSE);
			}
		}
		if( equalsStrings(key , FTPS_CONFIGURATION_KEY_KSS_ADDRESS) ){
			setKssAddress(value);
		}
		if( equalsStrings(key , FTPS_CONFIGURATION_KEY_KSS_PORT) ){
			setKssPort(value);
		}

		if( isDebugEnabled()) 
			debug(concat(getGlobalHeap() , 4, " Seteando " , key , "=" , value ));
	}

	void setupConfiguration(){

		if(isInfoEnabled()){
			info( "Inicializando la configuracion.");
			info(concat(getGlobalHeap() , 2 , 
				"Levantando la configurarcion del archivo " , 
				FTPS_CONFIGURATION_FILE));
		}

		loadConfigurationW( FTPS_CONFIGURATION_FILE , getGlobalHeap() , &processConfiguration);
	}