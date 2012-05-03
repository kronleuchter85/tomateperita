#include "vda-configuration.h"



	char * kssAddress;
	char * kssPort;
	char * vdaName;
	char * rpm;
	BOOL availableCache;
	BOOL offlineMode;
	int sectorCount;
	int trackCount;
	

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
	BOOL isVdaAvailableCache(){
		return availableCache;
	}
	int getVdaSectorsCount(){
		return sectorCount;
	}
	int getVdaTracksCount(){
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
	void setAvailableCache(BOOL b){
		availableCache = b;
	}
	void setVdaSectorsCount(int s){
		sectorCount = s;
	}
	void setVdaTracksCount(int t){
		trackCount = t;
	}
	void setVdaOfflineMode(BOOL b){
		offlineMode = b;
	}

	BOOL getBooleanType(char * strBool){
		if( equalsStrings(VDA_CONFIGURATION_VALUE_FALSE , strBool)){
			return FALSE;
		}else if(equalsStrings(VDA_CONFIGURATION_VALUE_TRUE , strBool)){
			return TRUE;	
		}
	}

	void processConfiguration(char * k , char * v){

		if( isDebugEnabled()) debug( concat(getGlobalHeap() , 4, " Seteando " , k , "=" , v ));

		if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_AVAILABLE_CACHE)){
			setAvailableCache(getBooleanType(v));
		}else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_KSS_ADDRESS)){
			setKssAddress(v);
		}else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_KSS_PORT)){
			setKssPort(v);
		}else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_NAME)){
			setVdaName(v);
		}else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_RPM)){
			setRpm(v);
		}else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_SECTORS_COUNT)){
			setVdaSectorsCount(atoi(v));
		} else if(equalsStrings(k ,VDA_CONFIGURATION_KEY_VDA_TRACKS_COUNT)){
			setVdaTracksCount(atoi(v));
		} else if(equalsStrings(k , VDA_CONFIGURATION_KEY_VDA_OFFLINE_MODE)){
			setVdaOfflineMode(getBooleanType(v));	
		}
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
		loadConfiguration( VDA_CONFIGURATION_FILE_NAME, getGlobalHeap() , &processConfiguration);
		//loadConfiguration( "C:\\tomateperita-vda.properties" , getGlobalHeap() , &processConfiguration);
	}

	
	
