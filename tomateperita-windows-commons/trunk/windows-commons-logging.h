/**
 * logging.h
 *
 * Declaracion de las funciones de logging.
 * 
 *
 */

#include "windows-commons-memory.h"

#define BUFFSIZE 2048

#define LOGGING_LEVEL_DEBUG 3 
#define LOGGING_LEVEL_INFO	2
#define LOGGING_LEVEL_ERROR	1

	int levelEnabled;
	char * processName;

	/**
	 * Niveles de logging...
	 */
	void setLoggingLevelEnabled(int level);
	int getLoggingLevelEnabled(void);
	BOOL isDebugEnabled(void);
	BOOL isInfoEnabled(void);
	void setProcessName(char *);

 
    char * getLogString(char * , char * , char * , HeapHandler);
    void writeLogString(char * , char *);
	void println(char * proc , char * type , char * description , HeapHandler hh);
	void logInfo(char * proc , char * description , HeapHandler hh);
	void logError(char * proc , char * description , HeapHandler hh);
	void logDebug(char * proc , char * description , HeapHandler hh);
	/*
	void logInfoScreen(char * proc , char * description , HeapHandler hh);
	void logErrorScreen(char * proc , char * description , HeapHandler hh);
	void logDebugScreen(char * proc , char * description , HeapHandler hh);
	*/