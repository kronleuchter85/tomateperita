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

#define CONFIGURATION_FILE "ftps-conf.ini"
#define FTPS_SECTION "ftps-section"
#define KSS_SECTION "kss-section"

#define FTPS_CONFIGURATION_KEY_FTPS_DATADIR			 "ftps.configuration.key.ftps.datadir"
#define FTPS_CONFIGURATION_KEY_FTPS_CONTROLPORT		 "ftps.configuration.key.ftps.controlport"
#define FTPS_CONFIGURATION_KEY_FTPS_DATAPORT		 "ftps.configuration.key.ftps.dataport"
#define FTPS_CONFIGURATION_KEY_FTPS_ADDRESS			 "ftps.configuration.key.ftps.address"
#define FTPS_CONFIGURATION_KEY_FTPS_LOGDIR			 "ftps.configuration.key.ftps.logdir"
#define FTPS_CONFIGURATION_KEY_FTPS_STARTMODE		 "ftps.configuration.key.ftps.startmode"
#define FTPS_CONFIGURATION_KEY_FTPS_LOGLEVEL		 "ftps.configuration.key.ftps.loglevel"

#define FTPS_CONFIGURATION_KEY_KSS_PORT				 "ftps.configuration.key.kss.port"
#define FTPS_CONFIGURATION_KEY_KSS_ADDRESS			 "ftps.configuration.key.kss.address"

	BOOL anonymousAccess = TRUE;

	char * dataDirectory = NULL;
	char * serverDataPort = NULL;
	char * serverControlPort = NULL;
	char * serverAddress = NULL;
	char * loggingDirectory = NULL;
	char * startMode = NULL;
	char * loggingLevel = NULL;
	char * kssAddress = NULL;
	char * kssPort = NULL;
	
	int maxRetriesFindingDataPort = 0;

	void configureLoggingLevel(char * level){
		//if(level == CONFIGURATION_LOGGING_LEVEL_DEBUG)
			setLoggingLevelEnabled(LOGGING_LEVEL_DEBUG);
		//if(level == CONFIGURATION_LOGGING_LEVEL_INFO)
			//setLoggingLevelEnabled(LOGGING_LEVEL_INFO);
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


	void setStartMode(char * m){
		startMode = m;
	}
	char * getStartMode(){
		return startMode;
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
		
		setDataDirectory(dataDir);
		setServerControlPort(ctrlPort);
		setServerDataPort(ctrlPort);
		setServerAddress(serverAddr);
		setLoggingDirectory(logDir);
		setLoggingLevel(logLevel);
		setStartMode(startMode);
		setKssAddress(kssAddress);
		setKssPort(kssPort);
	}