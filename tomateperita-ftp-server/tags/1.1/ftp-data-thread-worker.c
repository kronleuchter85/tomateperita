#include "socket-commons.h"
#include "ftp-client-thread-resources.h"
#include "ftp-constants.h"
#include "ftp-protocol-constants.h"
#include "file-commons.h"
#include "validation.h"
#include "commons.h"
#include "ftp-commons.h"

	void executeActiveCommand(ClientThreadResource * res );
	char * executeListFor(char * a);
	void storeFile(ClientThreadResource * , ListenSocket , int *);
	void retrieveFile(ClientThreadResource * , ListenSocket , int *);
	void listFiles(ClientThreadResource * , ListenSocket , int *);


	unsigned __stdcall serviceDataThread( void* pArguments ){
		ListenSocket clientDataSocket;
		int iResult = 0;
		ClientThreadResource * res = (ClientThreadResource*) pArguments;
		clientDataSocket = acceptRequest(res->dataSocket);

		if(res->mode == FTP_DATA_MODE_PASIVE){
			
			if(isDebugEnabled())
				debug(concat(res->heapHandler, 2 ,"[FtpData] Transfer task: ", intToString(res->transferTask, res->heapHandler)));

			if ( res->transferTask == STOR_TRANSFER ){
				storeFile( res , clientDataSocket , &iResult);
			} else if ( res->transferTask == RETR_TRANSFER ){
				retrieveFile( res , clientDataSocket , &iResult );
			} else if ( res->transferTask == LIST_TRANSFER ){
				listFiles( res , clientDataSocket , &iResult);
			}
			
		}/*else if(res->mode == FTP_DATA_MODE_ACTIVE){
			
			while(res->mode == FTP_DATA_MODE_ACTIVE){		
				
				blockMutex(res->dataMutex );
				executeActiveCommand(res);
				//unblockMutex(res->controlMutex);
				unblockMutex(res->controlMutex);
			}			
		}*/

		res->mode = FTP_DATA_MODE_INACTIVE;
		iResult = closeSocket(clientDataSocket);
		_endthread();
	}



	/*void executeActiveCommand(ClientThreadResource * res ){
		char * command = res->commandToExecute;
		int result;

		res->dataSocket = openConnectionSocket(res->remoteHost, res->remotePort);

		if(res->dataSocket == INVALID_SOCKET){
			res->resultMessage = "425 accion no realizada, no se pudo abrir la conexion";		
		}else{

			char * ordenList = executeListFor("");

			if(result == 1){
				res->resultMessage = "450 accion no realizada, fichero no encontrado";
			}else{

				sendMessage(res->dataSocket , concat(res->heapHandler , 2 ,
					ordenList , CHARACTER_CRLF) , &result);
				
				if(result == 0){
					res->resultMessage = "226 transferencia realizada con exito";
				}else{
					res->resultMessage = "426 accion no realizada, ocurrio un error";
				}
			}
		}

		closeSocket(res->dataSocket);
	}


	char * executeListFor(char * a){
		return "archivo1.txt archivo2.txt archivo3.txt ";
	}*/

	void storeFile(ClientThreadResource * res , ListenSocket clientSocket , int * result){
		int bytesWritten;
		int lastError = 0;
		char * receiveBuffer = (char *)allocateMemory( res->heapHandler , FILE_CHUNK );
		HANDLE newFile = getWriteFileHandle(res->argument);
		BOOL writeFlag;

		ZeroMemory(receiveBuffer , 1024);

		receiveBuffer = receiveFileChunk(clientSocket , res->heapHandler , &result);
		
		while ( receiveBuffer ){
			writeFlag = WriteFile(newFile , receiveBuffer , FILE_CHUNK , &bytesWritten , NULL);
			if ( !writeFlag ){
				lastError = GetLastError();
				error("Error al escribir archivo");
				break;
			}
			ZeroMemory(receiveBuffer , 1024);
			receiveBuffer = receiveFileChunk(clientSocket , res->heapHandler , &result);
		}
		
		CloseHandle(newFile);

		if (lastError != ERROR_NO_MORE_FILES) {
			res->resultMessage = concat(res->heapHandler , 2 , 
				"226 transferencia realizada con exito" , CHARACTER_CRLF);
		} else {
			res->resultMessage = concat(res->heapHandler , 2 , 
				"426 accion no realizada, ocurrio un error" , CHARACTER_CRLF);
		}
	}

	void retrieveFile(ClientThreadResource * res , ListenSocket clientSocket , int * result){
		HANDLE existingFile = getReadFileHandle(res->argument);
		char readBuffer[FILE_CHUNK];
		int bytesRead;
		BOOL readFlag;
		
		ZeroMemory(readBuffer, 1024);

		readFlag = ReadFile( existingFile , readBuffer , FILE_CHUNK , &bytesRead , NULL );
				
		while( readFlag && bytesRead > 0){
			sendFileChunk(clientSocket , readBuffer , &result);
			if (result == SOCKET_ERROR)
				break;
			ZeroMemory(readBuffer, 1024);
			readFlag = ReadFile( existingFile , readBuffer , FILE_CHUNK , &bytesRead , NULL );
		}

		CloseHandle(existingFile);

		if ( result ) {
			res->resultMessage = concat(res->heapHandler , 2 , 
				"226 transferencia realizada con exito" , CHARACTER_CRLF);
		} else {
			res->resultMessage = concat(res->heapHandler , 2 , 
				"226 accion no realizada, ocurrio un error" , CHARACTER_CRLF);
		}
	}

	void listFiles(ClientThreadResource * res , ListenSocket clientSocket , int * result){
		WIN32_FIND_DATA ffd;
		LARGE_INTEGER filesize;
		char szDir[MAX_PATH];
		char * arrayFileSize;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		DWORD lastError = 0;

		strcpy(szDir, trim(res->argument , res->heapHandler) );
		strcat(szDir, "/*");

		hFind = FindFirstFileA(szDir, &ffd);
		if ( INVALID_HANDLE_VALUE == hFind ) {
			printf("Error de FindFirstFile.\n");
		}

		do {
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){				
				sendMessage( clientSocket , concat(res->heapHandler, 3,
					"drwxrwxrwx 1 ftp ftp 4096 Jul 11 2011 ", ffd.cFileName , CHARACTER_CRLF) ,
					&result);
			} else {
				filesize.LowPart = ffd.nFileSizeLow;
				filesize.HighPart = ffd.nFileSizeHigh;
				arrayFileSize = intToString( ffd.nFileSizeLow , res->heapHandler );
				
				sendMessage( clientSocket , concat(res->heapHandler, 5 ,
					"-rwxrwxrwx 1 ftp ftp " , arrayFileSize , "Jul 11 2011 " , ffd.cFileName , CHARACTER_CRLF) ,
					&result);
			}
		} while (FindNextFileA(hFind, &ffd) != 0);

		lastError = GetLastError();
		FindClose(hFind);

		if (lastError != ERROR_NO_MORE_FILES) {
			res->resultMessage = concat(res->heapHandler , 2 , 
				"226 transferencia realizada con exito" , CHARACTER_CRLF);
		} else {
			res->resultMessage = concat(res->heapHandler , 2 , 
				"226 accion no realizada, ocurrio un error" , CHARACTER_CRLF);
		}
	}
