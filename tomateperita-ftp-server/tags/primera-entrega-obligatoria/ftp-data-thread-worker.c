#include "socket-commons.h"
#include "ftp-client-thread-resources.h"
#include "ftp-constants.h"
#include "ftp-protocol-constants.h"
#include "file-commons.h"
#include "validation.h"
#include "commons.h"


	
	void executeActiveCommand(ClientThreadResource * res );
	char * executeListFor(char * a);
	void storeFile(ClientThreadResource * , ListenSocket * );
	void retrieveFile(ClientThreadResource * , ListenSocket * );
	void listFiles(ClientThreadResource * , ListenSocket * );

	unsigned __stdcall serviceDataThread( void* pArguments ){
		ListenSocket * clientDataSocket;
		ClientThreadResource * res = (ClientThreadResource*) pArguments;
		clientDataSocket = acceptRequest(res->dataSocket);
		
		if(res->mode == FTP_DATA_MODE_PASIVE){
			
			if ( res->transferTask == STOR_TRANSFER ){
				storeFile( res , clientDataSocket );
			} else if ( res->transferTask == RETR_TRANSFER ){
				retrieveFile( res , clientDataSocket );
			} else if ( res->transferTask == LIST_TRANSFER ){
				listFiles( res , clientDataSocket );
			}
			
		}else if(res->mode == FTP_DATA_MODE_ACTIVE){
			
			while(res->mode == FTP_DATA_MODE_ACTIVE){		
				
				blockMutex(res->dataMutex );
				executeActiveCommand(res);
				//unblockMutex(res->controlMutex);
			}			
		}

		res->mode = FTP_DATA_MODE_INACTIVE;
		_endthread();
	}



	void executeActiveCommand(ClientThreadResource * res ){
		char * command = res->commandToExecute;
		int result;

		res->dataSocket = openConnectionSocket(res->remoteHost, res->remotePort);

		if(res->dataSocket == INVALID_SOCKET){
			res->resultMessage = "425 accion no realizada, no se pudo abrir la coneccion";		
		}else{

			char * ordenList = executeListFor("" , &result);

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
	}

	void storeFile(ClientThreadResource * res , ListenSocket * clientSocket){
		int result = 0;
		DWORD bytesRead , position;
		char * readBuffer = (char *)allocateMemory( res->heapHandler , FILE_CHUNK );
		HANDLE newFile = getFileHandle(res->argument);

		_tprintf("====> Llego al STOR...\n");

		position = SetFilePointer(newFile, 0 , NULL , FILE_END);
		LockFile(newFile, position , 0 , strlen(readBuffer) , 0);
			
		do {
			readBuffer = receiveMessage(clientSocket , res->heapHandler , &result);
		    position = SetFilePointer(newFile, 0 , NULL , FILE_END);
			WriteFile(newFile, readBuffer , strlen(readBuffer) , &bytesRead , NULL);
		} while( readBuffer > 0 );
			
		UnlockFile(newFile , position , 0 , strlen(readBuffer) , 0);
		CloseHandle(newFile);
	}

	void retrieveFile(ClientThreadResource * res , ListenSocket * clientSocket){
		int result = 0;
		DWORD bytesRead , position;
		char * readBuffer = (char *)allocateMemory( res->heapHandler , FILE_CHUNK );
		HANDLE existingFile = getFileHandle(res->argument);

		_tprintf("====> Llego al RETR...\n");

		position = SetFilePointer(existingFile, 0 , NULL , FILE_END);
		LockFile(existingFile, position , 0 , strlen(readBuffer) , 0);
			
		do {
			ReadFile( existingFile , readBuffer , FILE_CHUNK , &bytesRead , NULL );

			sendMessage( clientSocket , readBuffer , &result);
		} while( readBuffer > 0 );
			
		UnlockFile(existingFile , position , 0 , strlen(readBuffer) , 0);
		CloseHandle(existingFile);
	}

	void listFiles(ClientThreadResource * res , ListenSocket * clientSocket){
		WIN32_FIND_DATA ffd;
		LARGE_INTEGER filesize;
		TCHAR szDir[MAX_PATH];
		size_t length_of_arg;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		DWORD dwError=0;

		StringCchCopy(szDir, MAX_PATH, res->argument );
		StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

		hFind = FindFirstFile(szDir, &ffd);
		if ( INVALID_HANDLE_VALUE == hFind ) {
			printf("Error de FindFirstFile...\n");
		}

		do {
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName); //reemplazar por el sendMessage
			} else {
				filesize.LowPart = ffd.nFileSizeLow;
				filesize.HighPart = ffd.nFileSizeHigh;
				_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart); //reemplazar por el sendMessage
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		dwError = GetLastError();
		FindClose(hFind);

		if (dwError != ERROR_NO_MORE_FILES) {
			res->resultMessage = newString( res->heapHandler , (char *)RESULT_SUCCESSFULLY , 1);
		} else {
			res->resultMessage = newString( res->heapHandler , (char *)RESULT_WITH_ERRORS , 1);
		}
	}