#include "vda-state.h"

#define VDA_CONFIGURATION_FILE_NAME					"../conf/tomateperita-vda.properties"//"C:/tomateperita-vda.properties"//
#define VDA_CONFIGURATION_FILE_SECTION				"tomateperita-vda"

#define VDA_CONFIGURATION_KEY_VDA_NAME				"vda.configuration.key.name"
#define VDA_CONFIGURATION_KEY_VDA_KSS_PORT			"vda.configuration.key.kss.port"
#define VDA_CONFIGURATION_KEY_VDA_KSS_ADDRESS		"vda.configuration.key.kss.address"
#define VDA_CONFIGURATION_KEY_VDA_RPM				"vda.configuration.key.rpm"
#define VDA_CONFIGURATION_KEY_VDA_AVAILABLE_CACHE	"vda.configuration.key.availabe.cache"
#define VDA_CONFIGURATION_KEY_VDA_SECTORS_COUNT		"vda.configuration.key.sector.count"
#define VDA_CONFIGURATION_KEY_VDA_TRACKS_COUNT		"vda.configuration.key.track.count"
#define VDA_CONFIGURATION_KEY_VDA_TRACKS_DELAY		"vda.configuration.key.track.delay"
#define VDA_CONFIGURATION_KEY_VDA_OFFLINE_MODE		"vda.configuration.key.offline.mode"
#define VDA_CONFIGURATION_KEY_VDA_LOGDIR			"vda.configuration.key.logdir"
#define VDA_CONFIGURATION_KEY_VDA_LOGLEVEL			"vda.configuration.key.loglevel"

#define VDA_CONFIGURATION_VALUE_FALSE				"false"
#define VDA_CONFIGURATION_VALUE_TRUE				"true"


	char * getKssAddress();
	char * getKssPort();
	char * getVdaName();
	char * getRpm();		
	BOOL isVdaAvailableCache();
	uint getVdaSectorCount();		/*usado en calculos*/
	uint getVdaTrackCount();		/*usado en calculos*/
	uint getiRpm();					/*usado en calculos*/
	float getTrackDelay();			/*usado en calculos*/
	BOOL isVdaOfflineMode();

	void setKssAddress(char *);
	void setKssPort(char *);
	void setVdaName(char *);
	void setRpm(char *);			/*usado en calculos*/
	void setTrackDelay(float);		/*usado en calculos*/
	void setAvailableCache(BOOL b);
	void setVdaSectorsCount(uint);	/*usado en calculos*/
	void setVdaTracksCount(uint);	/*usado en calculos*/
	void setVdaOfflineMode(BOOL b);
	void setupConfiguration();

	void processConfiguration(char * k , char * v);