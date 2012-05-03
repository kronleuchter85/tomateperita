#include "vda-configuration.h"
#include "vda-commons.h"
#include "file-commons.h"
#include "logging.h"
#include "string.h"

/* Los valores iniciarles de las variables solo son utiles en caso de no encontrar el archivo de configuracion */

	char * kssAddress;
	char * kssPort;
	char * vdaName = "VDA";
	char * rpm;
	BOOL availableCache=TRUE;
	BOOL offlineMode=TRUE;
	uint sectorCount=22;
	uint trackCount=11;
	uint iRpm=5400;
	float trackDelay=3;

	char logDir[COMMON_KEY_LEN] = "./";
	char logFullPath[COMMON_KEY_LEN] = "\0";

	char dbDir[COMMON_KEY_LEN] = "./";
	char dbFullPath[COMMON_KEY_LEN] = "\0";
	

	char * getKssAddress(){
		return kssAddress;
	}
	char * getKssPort(){
		return kssPort;
	}
	char * getVdaName(){
		return vdaName;
	}
	char * getRpm(){
		return rpm;
	}
	uint getiRpm(){
		return iRpm;
	}
	float getTrackDelay(){
		return trackDelay;
	}
	BOOL isVdaAvailableCache(){
		return availableCache;
	}
	uint getVdaSectorCount(){
		return sectorCount;
	}
	uint getVdaTrackCount(){
		return trackCount;
	}
	BOOL isVdaOfflineMode(){
		return offlineMode;
	}

	void setKssAddress(char * s){
		kssAddress = s;
	}
	void setKssPort(char * s){
		kssPort = s;
	}
	void setVdaName(char * v){
		vdaName = v;
	}
	void setRpm(char * r){
		rpm = r;
	}
	void setiRpm(uint r){
		iRpm = r;
	}
	void setTrackDelay(float d){
	trackDelay = d;
	}
	void setAvailableCache(BOOL b){
		availableCache = b;
	}
	void setVdaSectorsCount(uint s){
		sectorCount = s;
	}
	void setVdaTracksCount(uint t){
		trackCount = t;
	}
	void setVdaOfflineMode(BOOL b){
		offlineMode = b;
	}

	BOOL getBooleanType(char * strBool){
		return (equalsStrings(VDA_CONFIGURATION_VALUE_TRUE , strBool)) ? TRUE:FALSE;
	}

	void setLoggingDirectory(char * d){
		strcpy_s(logDir, COMMON_KEY_LEN, d);
	}

	void setDBLocation(char * d){
		strcpy_s(dbDir, COMMON_KEY_LEN, d);
	}

	char * getLogFullPath(){
		strcpy_s(logFullPath, COMMON_KEY_LEN, logDir);
		strcat_s(logFullPath, COMMON_KEY_LEN, "/");
		strcat_s(logFullPath, COMMON_KEY_LEN, getVdaName());
		return logFullPath;
	}

	char * getDBFullPath(){
		strcpy_s(dbFullPath, COMMON_KEY_LEN, dbDir);
		strcat_s(dbFullPath, COMMON_KEY_LEN, "/");
		strcat_s(dbFullPath, COMMON_KEY_LEN, getVdaName());
		return dbFullPath;
	}

	void configureLoggingLevel(char * level){
		if( !strcmp("debug" , trim(level , getGlobalHeap())))
			setLoggingLevelEnabled(LOGGING_LEVEL_DEBUG);
		else if( !strcmp("error" , trim(level , getGlobalHeap())))
			setLoggingLevelEnabled(LOGGING_LEVEL_ERROR);
		else //if( !strcmp("info" , trim(level , getGlobalHeap())))
			setLoggingLevelEnabled(LOGGING_LEVEL_INFO);
	}

	void processConfiguration(char * k , char * v){

		if( isDebugEnabled()) debug(concat(getGlobalHeap() , 4, " Seteando " , k , "=" , v ));

		if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_AVAILABLE_CACHE))
			setAvailableCache(getBooleanType(v));
		else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_KSS_ADDRESS))
			setKssAddress(v);
		else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_KSS_PORT))
			setKssPort(v);
		else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_NAME))
			setVdaName(v);
		else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_RPM)){
			setRpm(v);
			setiRpm(atoi(v));
		}else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_SECTORS_COUNT))
			setVdaSectorsCount(atoi(v));
		else if(equalsStrings(k ,VDA_CONFIGURATION_KEY_VDA_TRACKS_COUNT))
			setVdaTracksCount(atoi(v));
		else if(equalsStrings(k ,VDA_CONFIGURATION_KEY_VDA_TRACKS_DELAY))
			setTrackDelay((float) atof(v));
		else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_OFFLINE_MODE))
			setVdaOfflineMode(getBooleanType(v));
		//else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_LOGDIR))
			//setLoggingDirectory(v);
		else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_LOGLEVEL))
			configureLoggingLevel(v);
		else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_DBDIR))
			setDBLocation(v);
	}

	void setupConfiguration(){

		if(isInfoEnabled()){
			info( "Inicializando la configuracion");
			info(concat(getGlobalHeap() , 2 , 
				"Levantando la configurarcion del archivo " , 
				VDA_CONFIGURATION_FILE_NAME));
		}

		/**
		 * TODO: modificar la funcion para pasar el archivo
		 * directamente, de esta forma se puede validar si existe o no.
		 */		
		loadConfigurationW( VDA_CONFIGURATION_FILE_NAME , getGlobalHeap() , &processConfiguration);
	}

	
	
