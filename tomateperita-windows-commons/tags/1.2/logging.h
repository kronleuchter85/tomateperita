/**
 * logging.h
 *
 * Declaracion de las funciones de logging.
 * 
 *
 */

//#include "headers-commons.h"
#include "memory-commons.h"

#define BUFFSIZE 2048

#define LOGGING_LEVEL_DEBUG 3 
#define LOGGING_LEVEL_INFO	2
#define LOGGING_LEVEL_ERROR	1


	/**
	 * Niveles de logging...
	 */
	void setLoggingLevelEnabled(int level);
	int getLoggingLevelEnabled(void);
	BOOL isDebugEnabled(void);
	BOOL isInfoEnabled(void);

 
    char * getLogString(char * , char * , char * , HeapHandler);
    void writeLogString(char * , char *);
	void println(char * proc , char * type , char * description , HeapHandler hh);
	void logInfo(char * proc , char * description , HeapHandler hh);
	void logError(char * proc , char * description , HeapHandler hh);
	void logDebug(char * proc , char * description , HeapHandler hh);