
#include "headers-commons.h"

#define CONFIGURATION_START_MODE_TEST "TEST"
#define CONFIGURATION_START_MODE_REAL "REAL"

#define CONFIGURATION_LOGGING_LEVEL_DEBUG "DEBUG"
#define CONFIGURATION_LOGGING_LEVEL_INFO "INFO"


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

