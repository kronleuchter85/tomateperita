/**
 * logging.h
 *
 * Declaracion de las funciones de logging (Linux).
 * 
 *
 */

#define BUFFSIZE 2048

#define LOGGING_LEVEL_DEBUG 			3
#define LOGGING_LEVEL_INFO				2
#define LOGGING_LEVEL_ERROR				1

#define LOGGING_LEVEL_STRING_DEBUG		"debug"
#define LOGGING_LEVEL_STRING_INFO		"info"
#define LOGGING_LEVEL_STRING_ERROR		"error"

	/**
	 * Niveles de logging.
	 */
	void setLoggingLevelEnabled(int level);
	int getLoggingLevelEnabled();
	int isDebugEnabled();
	int isInfoEnabled();

	/**
	 * Strings de logging.
	 */
	char * getLogString(char * , char * , char * );
	void writeLogString(char * , char * );

	void println(char * proc , char * type , char * description );
	void logInfo(char * proc , char * description );
	void logError(char * proc , char * description );
	void logDebug(char * proc , char * description );
