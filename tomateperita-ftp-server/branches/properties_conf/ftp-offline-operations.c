#include "windows-commons-socket.h"
#include "windows-commons-file.h"
#include "windows-commons.h"
#include "windows-commons-errors.h"

#include "ftp-client-thread-resources.h"
#include "ftp-response-builder.h"
#include "ftp-commons.h"
#include "ftp.h"

	void doOfflineDeleteFile(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator){
	}


	void doOfflineStoreFile(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator){
		int bytesWritten;
		int lastError = 0;
		char * receiveBuffer;

		HANDLE newFile = getWriteFileHandle(trim(res->argument , res->heapHandler));
		BOOL writeFlag;
		int received;
		char buffer[65536];
		ZeroMemory(buffer, 65536);
		received = receiveBytes(clientSocket , buffer , 65536);
		
		while ( received > 0) {
			writeFlag = WriteFile(newFile , buffer , received , &bytesWritten , NULL);
			if ( !writeFlag ){
				lastError = GetLastError();
				error("Error al escribir archivo");
				break;
			}		
			ZeroMemory(buffer, 65536);
			received = receiveBytes(clientSocket , buffer , 65536);
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



	void doOfflineRetrieveFile(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator){
		HANDLE existingFile = getReadFileHandle(res->argument , res->heapHandler);
		char readBuffer[65536];
		int bytesRead;
		BOOL readFlag;
		
		ZeroMemory(readBuffer, 65536);
		readFlag = ReadFile( existingFile , readBuffer , 65536 , &bytesRead , NULL );
				
		while( readFlag && bytesRead > 0){
			if( sendBytes(clientSocket , readBuffer , bytesRead) < bytesRead ){
				setError(validator , "Se han enviado menos bytes de los que se han leido");
				break;
			}
			ZeroMemory(readBuffer, 65536);
			readFlag = ReadFile( existingFile , readBuffer , 65536 , &bytesRead , NULL );
		}

		CloseHandle(existingFile);

		closeSocket(res->dataSocket);

		if (!hasError(validator)) {
			res->resultMessage = buildResponseMessageType226(res->heapHandler);
		} else {
			res->resultMessage = buildResponseMessageType426(res->heapHandler);
		}
	}


	void doOfflineListFiles(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator){
		WIN32_FIND_DATA ffd;
		LARGE_INTEGER filesize;
		char szDir[MAX_PATH];
		char * arrayFileSize;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		DWORD lastError = 0;

		char * thePath = concat(res->heapHandler , 3 , getDataDirectory() , res->relativeWorkingDir , "*");

		hFind = FindFirstFileA(thePath, &ffd);

		while( hFind != INVALID_HANDLE_VALUE && FindNextFileA(hFind, &ffd) != 0){
			int result;
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){				
				
				char * msg = concat(res->heapHandler, 3,
					"drwxrwxrwx 1 ftp ftp 4096 Jul 11 2011 ", ffd.cFileName , CHARACTER_CRLF);

				sendBytes(clientSocket , msg , strlen(msg));
				/*sendMessage( clientSocket , concat(res->heapHandler, 3,
					"drwxrwxrwx 1 ftp ftp 4096 Jul 11 2011 ", ffd.cFileName , CHARACTER_CRLF) ,
					&result);*/


			} else {
				filesize.LowPart = ffd.nFileSizeLow;
				filesize.HighPart = ffd.nFileSizeHigh;
				arrayFileSize = intToString( ffd.nFileSizeLow , res->heapHandler );
				

				{
					char * msg = concat(res->heapHandler, 5 ,
						"-rwxrwxrwx 1 ftp ftp " , arrayFileSize , "Jul 11 2011 " , ffd.cFileName , CHARACTER_CRLF);
					sendBytes(clientSocket , msg , strlen(msg));
				}
				
				/*sendMessage( clientSocket , concat(res->heapHandler, 5 ,
					"-rwxrwxrwx 1 ftp ftp " , arrayFileSize , "Jul 11 2011 " , ffd.cFileName , CHARACTER_CRLF) ,
					&result);*/
			}
		} 

		if ( INVALID_HANDLE_VALUE == hFind ) {
			error("Error al leer lista de archivos (FindFirstFile)");
		}

		lastError = GetLastError();
		FindClose(hFind);

		if (lastError == 0 || lastError == ERROR_NO_MORE_FILES ) {
			res->resultMessage = buildResponseMessageType226(res->heapHandler);
		} else {
			res->resultMessage = concat(res->heapHandler , 2 , 
				"426 accion no realizada, ocurrio un error" , CHARACTER_CRLF);
		}
	}