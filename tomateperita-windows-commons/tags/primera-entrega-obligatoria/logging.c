/**
 * logging.h
 *
 * Funciones para obtener y escribir 
 * el string del log.
 */
 
#include "logging.h"
 

	int levelEnabled;

	void setLoggingLevelEnabled(int level){
		levelEnabled = level;
	}

	int getLoggingLevelEnabled(){
		return levelEnabled;
	}

	BOOL isDebugEnabled(){
		if(levelEnabled >= LOGGING_LEVEL_DEBUG){
			return TRUE;
		}else{
			return FALSE;
		}
	}

	BOOL isInfoEnabled(){	
		if(levelEnabled >= LOGGING_LEVEL_INFO){
			return TRUE;
		}else{
			return FALSE;
		}
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
        sprintf(inst,"%02d:%02d:%02d.%03d", time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
		sprintf(logString,"[%s][Proc: %s][ProcessID: %d][ThreadID: %d]:[Level: %s] %s",inst,procName,pid,tid,type,desc);
        
        return logString;
    }

    /* Escribe en un archivo(nombre_proceso).log
       el string que se obtuvo con getLogString.
    */
    
    void writeLogString(char * procName , char * logString) {
        HANDLE logfile;
        char logname[20];
        DWORD position, bytesWritten;
        
        sprintf_s(logname , sizeof(logname) , "%s.log" , procName);
                    
        logfile = CreateFileA(logname,
            FILE_APPEND_DATA,
            FILE_SHARE_READ,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (logfile == INVALID_HANDLE_VALUE) {
            printf("No se pudo crear or abrir el log.\n");
        } else {
            position = SetFilePointer(logfile, 0 , NULL , FILE_END);
            LockFile(logfile, position , 0 , strlen(logString) , 0);
            WriteFile(logfile, logString , strlen(logString) , &bytesWritten , NULL);
            UnlockFile(logfile , position , 0 , strlen(logString) , 0);
        }
        
        CloseHandle(logfile);
        return EXIT_SUCCESS;
    }

	void println(char * proc , char * type , char * description , HeapHandler hh){
		printf("%s\n" , getLogString(proc , type , description , hh));
	}

	void logInfo(char * proc , char * description , HeapHandler hh){
		println(proc , "INFO" , description, hh);
	}
	void logError(char * proc , char * description , HeapHandler hh){
		println(proc , "ERROR" , description, hh);
	}	
	void logDebug(char * proc , char * description , HeapHandler hh){
		println(proc , "DEBUG" , description, hh);
	}

