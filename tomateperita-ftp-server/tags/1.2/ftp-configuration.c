/**
 * Configuracion general que mantiene el estado
 * de lo que se cargo desde el archivo.
 *
 */
#include "ftp-configuration.h"
#include "headers-commons.h"
#include "file-commons.h"
#include "logging.h"
#include "ftp-state.h"
#include "ftp-commons.h"
#include "commons.h"


#define CONFIGURATION_FILE							"tomateperita-ftps.ini"
#define FTPS_SECTION								"ftps-section"
#define KSS_SECTION									"kss-section"

#define FTPS_CONFIGURATION_KEY_FTPS_DATADIR			 "ftps.configuration.key.ftps.datadir"
#define FTPS_CONFIGURATION_KEY_FTPS_CONTROLPORT		 "ftps.configuration.key.ftps.controlport"
#define FTPS_CONFIGURATION_KEY_FTPS_DATAPORT		 "ftps.configuration.key.ftps.dataport"
#define FTPS_CONFIGURATION_KEY_FTPS_ADDRESS			 "ftps.configuration.key.ftps.address"
#define FTPS_CONFIGURATION_KEY_FTPS_LOGDIR			 "ftps.configuration.key.ftps.logdir"
#define FTPS_CONFIGURATION_KEY_FTPS_STARTMODE		 "ftps.configuration.key.ftps.startmode.offline"
#define FTPS_CONFIGURATION_KEY_FTPS_LOGLEVEL		 "ftps.configuration.key.ftps.loglevel"
#define FTPS_CONFIGURATION_KEY_MAX_RETRIES			 "ftps.configuration.key.ftps.maxretries"

#define FTPS_CONFIGURATION_KEY_KSS_PORT				 "ftps.configuration.key.kss.port"
#define FTPS_CONFIGURATION_KEY_KSS_ADDRESS			 "ftps.configuration.key.kss.address"

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

	void setUpConfiguration(){

		char * dataDir = getConfigurationStrValue( getGlobalHeap() , 
			CONFIGURATION_FILE , FTPS_SECTION , FTPS_CONFIGURATION_KEY_FTPS_DATADIR);
				
		char * ctrlPort = getConfigurationStrValue( getGlobalHeap() ,
			CONFIGURATION_FILE , FTPS_SECTION , FTPS_CONFIGURATION_KEY_FTPS_CONTROLPORT);

		char * dataPort = getConfigurationStrValue( getGlobalHeap() ,
			CONFIGURATION_FILE , FTPS_SECTION , FTPS_CONFIGURATION_KEY_FTPS_DATAPORT);
	
		char * serverAddr = getConfigurationStrValue( getGlobalHeap() ,
			CONFIGURATION_FILE , FTPS_SECTION , FTPS_CONFIGURATION_KEY_FTPS_ADDRESS);
		
		char * logDir = getConfigurationStrValue( getGlobalHeap() ,
			CONFIGURATION_FILE , FTPS_SECTION , FTPS_CONFIGURATION_KEY_FTPS_LOGDIR);
		
		char * logLevel = getConfigurationStrValue( getGlobalHeap() ,
			CONFIGURATION_FILE , FTPS_SECTION , FTPS_CONFIGURATION_KEY_FTPS_LOGLEVEL);

		char * startMode = getConfigurationStrValue( getGlobalHeap() ,
			CONFIGURATION_FILE , FTPS_SECTION , FTPS_CONFIGURATION_KEY_FTPS_STARTMODE);

		char * kssAddress = getConfigurationStrValue( getGlobalHeap() , 
			CONFIGURATION_FILE , KSS_SECTION , FTPS_CONFIGURATION_KEY_KSS_ADDRESS);

		char * kssPort = getConfigurationStrValue( getGlobalHeap() , 
			CONFIGURATION_FILE , KSS_SECTION , FTPS_CONFIGURATION_KEY_KSS_PORT);

		char * maxRetries = getConfigurationStrValue( getGlobalHeap() ,
			CONFIGURATION_FILE , FTPS_SECTION , FTPS_CONFIGURATION_KEY_MAX_RETRIES);

		info("Cargando la configuracion");
		
		setDataDirectory(dataDir);
		setServerControlPort(ctrlPort);
		setServerDataPort(dataPort);
		setServerAddress(serverAddr);
		setLoggingDirectory(logDir);
		setLoggingLevel(logLevel);
		setMaxRetriesFindingDataPort(atoi(maxRetries));
		setKssAddress(kssAddress);
		setKssPort(kssPort);

		if(!strcmp("true" , startMode)){
			setOfflineStartMode(TRUE);
		}else{
			setOfflineStartMode(FALSE);
		}

		if(isDebugEnabled()) 
			debug(concat(getGlobalHeap() , 4 , "Seteando " , 
				FTPS_CONFIGURATION_KEY_FTPS_DATADIR , "=" , dataDir));

		if(isDebugEnabled()) 
			debug(concat(getGlobalHeap() , 4 , "Seteando " , 
				FTPS_CONFIGURATION_KEY_FTPS_CONTROLPORT , "=" , ctrlPort));

		if(isDebugEnabled()) 
			debug(concat(getGlobalHeap() , 4 , "Seteando " , 
				FTPS_CONFIGURATION_KEY_FTPS_DATAPORT , "=" , dataPort));

		if(isDebugEnabled()) 
			debug(concat(getGlobalHeap() , 4 , "Seteando " , 
				FTPS_CONFIGURATION_KEY_FTPS_ADDRESS , "=" , serverAddr));

		if(isDebugEnabled()) 
			debug(concat(getGlobalHeap() , 4 , "Seteando " , 
				FTPS_CONFIGURATION_KEY_FTPS_LOGDIR , "=" , logDir));

		if(isDebugEnabled()) 
			debug(concat(getGlobalHeap() , 4 , "Seteando " , 
				FTPS_CONFIGURATION_KEY_FTPS_LOGLEVEL , "=" , logLevel));

		if(isDebugEnabled()) 
			debug(concat(getGlobalHeap() , 4 , "Seteando " , 
				FTPS_CONFIGURATION_KEY_FTPS_STARTMODE , "=" , startMode));

		if(isDebugEnabled()) 
			debug(concat(getGlobalHeap() , 4 , "Seteando " , 
				FTPS_CONFIGURATION_KEY_MAX_RETRIES , "=" , maxRetries));

		if(isDebugEnabled()) 
			debug(concat(getGlobalHeap() , 4 , "Seteando " , 
				FTPS_CONFIGURATION_KEY_KSS_ADDRESS , "=" , kssAddress));

		if(isDebugEnabled()) 
			debug(concat(getGlobalHeap() , 4 , "Seteando " , 
				FTPS_CONFIGURATION_KEY_KSS_ADDRESS , "=" , kssPort));			
		
		
	}