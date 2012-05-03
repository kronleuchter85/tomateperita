#include "vda-state.h"

#define VDA_CONFIGURATION_FILE_NAME					"../conf/tomateperita-vda.properties"
#define VDA_CONFIGURATION_FILE_SECTION				"tomateperita-vda"

#define VDA_CONFIGURATION_KEY_VDA_NAME				"vda.configuration.key.name"
#define VDA_CONFIGURATION_KEY_VDA_KSS_PORT			"vda.configuration.key.kss.port"
#define VDA_CONFIGURATION_KEY_VDA_KSS_ADDRESS		"vda.configuration.key.kss.address"
#define VDA_CONFIGURATION_KEY_VDA_RPM				"vda.configuration.key.rpm"
#define VDA_CONFIGURATION_KEY_VDA_AVAILABLE_CACHE	"vda.configuration.key.availabe.cache"
#define VDA_CONFIGURATION_KEY_VDA_SECTORS_COUNT		"vda.configuration.key.sector.count"
#define VDA_CONFIGURATION_KEY_VDA_TRACKS_COUNT		"vda.configuration.key.track.count"
#define VDA_CONFIGURATION_KEY_VDA_OFFLINE_MODE		"vda.configuration.key.offline.mode"

#define VDA_CONFIGURATION_VALUE_FALSE				"false"
#define VDA_CONFIGURATION_VALUE_TRUE				"true"


	char * getKssAddress();
	char * getKssPort();
	char * getVdaName();
	char * getRpm();
	BOOL isVdaAvailableCache();
	int getSectorCount();
	int getTrackCount();
	BOOL isVdaOfflineMode();

	void setKssAddress(char *);
	void setKssPort(char *);
	void setVdaName(char *);
	void setRpm(char *);
	void setAvailableCache(BOOL b);
	void setVdaSectorsCount(int);
	void setVdaTracksCount(int);
	void setVdaOfflineMode(BOOL b);
	void setupConfiguration();

	void processConfiguration(char * k , char * v);