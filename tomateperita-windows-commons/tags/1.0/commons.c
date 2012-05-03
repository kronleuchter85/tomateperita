#include "commons.h"



	int equalsStrings(char * s1 , char * s2){
		return !strcmp( s1 , s2);
	}

	char * newString(HeapHandler h , char * content , int size){
		char * s = (char*)allocateMemory(h , (size + 1));
		strncpy(s,content , size);
		return s;
	}


	/**
	 * Devuelve el mensaje de error en un string
	 * correspondiente al error producido en el sistema
	 */
	LPVOID getErrorMessage(void){

		//TODO: !!!!!!
		//TODO: probar bien esta funcion !!!!
		//TODO: !!!!!!

		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				  FORMAT_MESSAGE_FROM_SYSTEM | 
				  FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL, 
				  GetLastError(), 
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				  (LPTSTR) &lpMsgBuf, 
				  0, 
				  NULL);
		return lpMsgBuf;
	}
	
	/* LOGGER:
        Pendiente:
		- Revisar el último argumento: se escribe en el archivo
        cosas diferentes a lo que se imprime por pantalla. Primero 
        sería necesiario probar extensivamente la función
        getErrorMessage por sí misma, y luego en conjunción con logger.
	*/

    int logger(char *procName,DWORD pid,DWORD tid,char *type,LPVOID *desc) {
        HANDLE logfile;
        char logname[20];
        char logbuffer[BUFFSIZE];
        DWORD position, bytesWritten;
        SYSTEMTIME time;
        char inst[20];

        sprintf_s(logname,sizeof(logname),"%s.log",procName);
        GetLocalTime(&time);
        sprintf(inst,"%02d:%02d:%02d.%03d", time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
        sprintf(logbuffer,"[%s Proc:%s PID:%d TID:%d Type:%s] %s\n",inst,procName,pid,tid,type,desc);
        
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
            printf("Escribiendo log...\n");
            position = SetFilePointer(logfile, 0, NULL, FILE_END);
            LockFile(logfile, position, 0, strlen(logbuffer), 0);
            WriteFile(logfile, logbuffer, strlen(logbuffer), &bytesWritten, NULL);
            UnlockFile(logfile, position, 0, strlen(logbuffer), 0);
        }
        
        CloseHandle(logfile);
        return EXIT_SUCCESS;
    }