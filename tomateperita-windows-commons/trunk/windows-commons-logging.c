/**
 * logging.h
 *
 * Funciones para obtener y escribir 
 * el string del log.
 */
 
#include "windows-commons-logging.h"

	void setLoggingLevelEnabled(int level){
		levelEnabled = level;
	}

	int getLoggingLevelEnabled(void){
		return levelEnabled;
	}

	BOOL isDebugEnabled(void){
		return (levelEnabled >= LOGGING_LEVEL_DEBUG) ? TRUE:FALSE;
	}

	BOOL isInfoEnabled(void){
		return (levelEnabled >= LOGGING_LEVEL_INFO) ? TRUE:FALSE;
	}

	void setProcessName(char * p){
		processName = p;
	}

	char * getProcessName(void){
		return processName;
	}


    /* Devuelve el string formateado
       segun las normas del TP.
    */
 
    char * getLogString(char * procName , char * type, char * desc , HeapHandler hh) {
        char *logString = allocateMemory(hh , BUFFSIZE);
        SYSTEMTIME time;
        char inst[20];
        DWORD pid = GetCurrentProcessId();
        DWORD tid = GetCurrentThreadId();

        GetLocalTime(&time);
        sprintf_s(inst, 20, "%02d:%02d:%02d.%03d", time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
		sprintf_s(logString, BUFFSIZE, "[%s][Proc: %s][ProcessID: %d][ThreadID: %d]:[Level: %s] %s\r\n",inst,procName,pid,tid,type,desc);
        
        return logString;
    }

    /* Escribe en el archivo dado por procName.log
       el string que se obtuvo con getLogString.
    */
    
    void writeLogString(char * procName , char * logString) {
        HANDLE logfile;
        char logname[30];
        DWORD position, bytesWritten;
        
		ZeroMemory(logname , 30);
		sprintf_s(logname , sizeof(logname) , "../logs/%s.log" , procName);
                    
        logfile = CreateFileA(logname,
            FILE_APPEND_DATA,
            FILE_SHARE_READ,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (logfile == INVALID_HANDLE_VALUE) {
            printf("[IO] No se pudo crear or abrir el log.\n");
        } else {
            position = SetFilePointer(logfile, 0 , NULL , FILE_END);
            LockFile(logfile, position , 0 , (DWORD) strlen(logString) , 0);
            WriteFile(logfile, logString , (DWORD) strlen(logString) , &bytesWritten , NULL);
            UnlockFile(logfile , position , 0 , (DWORD) strlen(logString) , 0);
        }
        
        CloseHandle(logfile);
    }

	void println(char * proc , char * type , char * description , HeapHandler hh){
		printf("%s" , getLogString(proc , type , description , hh));
	}

	void logInfo(char * proc , char * description , HeapHandler hh){
		println(proc , "INFO" , description, hh);		
		writeLogString(proc, getLogString(proc, "INFO", description, hh ));
	}
	void logError(char * proc , char * description , HeapHandler hh){
		println(proc , "ERROR" , description, hh);
		writeLogString(proc, getLogString(proc, "ERROR", description, hh ));
	}	
	void logDebug(char * proc , char * description , HeapHandler hh){
		println(proc , "DEBUG" , description, hh);
		writeLogString(proc, getLogString(proc, "DEBUG", description, hh ));
	}

	/*void logInfoScreen(char * proc , char * description , HeapHandler hh){
		println(proc , "INFO" , description, hh);		
	}
	void logErrorScreen(char * proc , char * description , HeapHandler hh){
		println(proc , "ERROR" , description, hh);
	}	
	void logDebugScreen(char * proc , char * description , HeapHandler hh){
		println(proc , "DEBUG" , description, hh);
	}*/