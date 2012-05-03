
#include "windows-commons-headers.h"


#ifndef _FTP_CONFIGURATION_
#define _FTP_CONFIGURATION_



#define FTPS_CONFIGURATION_FILE						"../conf/tomateperita-ftps.properties"
#define FTPS_SECTION								"ftps-section"
#define KSS_SECTION									"kss-section"

#define FTPS_CONFIGURATION_KEY_FTPS_DATADIR			"ftps.configuration.key.ftps.datadir"
#define FTPS_CONFIGURATION_KEY_FTPS_CONTROLPORT		"ftps.configuration.key.ftps.controlport"
#define FTPS_CONFIGURATION_KEY_FTPS_DATAPORT		"ftps.configuration.key.ftps.dataport"
#define FTPS_CONFIGURATION_KEY_FTPS_ADDRESS			"ftps.configuration.key.ftps.address"
#define FTPS_CONFIGURATION_KEY_FTPS_LOGDIR			"ftps.configuration.key.ftps.logdir"
#define FTPS_CONFIGURATION_KEY_FTPS_STARTMODE		"ftps.configuration.key.ftps.startmode.offline"
#define FTPS_CONFIGURATION_KEY_FTPS_LOGLEVEL		"ftps.configuration.key.ftps.loglevel"
#define FTPS_CONFIGURATION_KEY_MAX_RETRIES			"ftps.configuration.key.ftps.maxretries"

#define FTPS_CONFIGURATION_KEY_KSS_PORT				"ftps.configuration.key.kss.port"
#define FTPS_CONFIGURATION_KEY_KSS_ADDRESS			"ftps.configuration.key.kss.address"

#endif

	void setAnonymousAccess(BOOL s);
	BOOL isAnonymousAccess();

	void setDataDirectory(char * d);
	char * getDataDirectory();

	void setServerDataPort(char * p);
	char * getServerDataPort();

	void setServerControlPort(char * p);
	char * getServerControlPort();

	void setServerAddress(char * a);
	char * getServerAddress();

	void setLoggingDirectory(char * d);
	char * getLoggingDirectory();

	void setStartMode(char * m);
	char * getStartMode();

	void setLoggingLevel(char * l);
	char * getLoggingLevel();

	int getMaxRetriesFindingDataPort();
	void setMaxRetriesFindingDataPort(int retries);

	void setKssAddress(char * a);
	char * getKssAddress();

	void setKssPort(char * p);
	char * getKssPort();

	void setUpConfiguration();
	void processConfiguration(char * , char *);

	BOOL isOfflineStartMode();