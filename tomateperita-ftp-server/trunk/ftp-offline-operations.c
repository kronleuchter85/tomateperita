#include "windows-commons-socket.h"
#include "windows-commons-file.h"
#include "windows-commons.h"
#include "windows-commons-errors.h"

#include "ftp-client-thread-resources.h"
#include "ftp-response-builder.h"
#include "ftp-configuration.h"
#include "ftp-commons.h"
#include "ftp.h"

	void doOfflineStoreFile(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator){
		int bytesWritten;
		int lastError = 0;

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
		if (lastError != ERROR_NO_MORE_FILES) 
			res->resultMessage = buildResponseMessageType226(res->heapHandler);
		else 
			res->resultMessage = buildResponseMessageType426(res->heapHandler);

	}



	void doOfflineRetrieveFile(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator){
		//HANDLE existingFile = getReadFileHandle(res->argument , res->heapHandler);
		HANDLE existingFile = getReadFileHandle(res->argument);
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

		if (!hasError(validator))
			res->resultMessage = buildResponseMessageType226(res->heapHandler);
		else
			res->resultMessage = buildResponseMessageType426(res->heapHandler);
	}


	void doOfflineListFiles(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator){
		WIN32_FIND_DATA ffd;
		LARGE_INTEGER filesize;
		//char szDir[MAX_PATH];
		char * arrayFileSize;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		DWORD lastError = 0;
		char * msg;

		char * thePath = concat(res->heapHandler , 3 , getDataDirectory() , res->relativeWorkingDir , "*");

		hFind = FindFirstFileA(thePath, &ffd);

		while( hFind != INVALID_HANDLE_VALUE && FindNextFileA(hFind, &ffd) != 0){

			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){				
				msg = concat(res->heapHandler, 3,
					"drwxrwxrwx 1 ftp ftp 4096 Jul 11 2011 ", ffd.cFileName , CHARACTER_CRLF);
				sendBytes(clientSocket , msg , (int) strlen(msg));
			} else {
				filesize.LowPart = ffd.nFileSizeLow;
				filesize.HighPart = ffd.nFileSizeHigh;
				arrayFileSize = intToString( ffd.nFileSizeLow , res->heapHandler );
				msg = concat(res->heapHandler, 5 ,
						"-rwxrwxrwx 1 ftp ftp " , arrayFileSize , "Jul 11 2011 " , ffd.cFileName , CHARACTER_CRLF);
				sendBytes(clientSocket , msg , (int) strlen(msg));
			}
			freeMemory(res->heapHandler, msg);
		} 

		if ( INVALID_HANDLE_VALUE == hFind ) {
			error("Error al leer lista de archivos (FindFirstFile)");
		}

		lastError = GetLastError();
		FindClose(hFind);

		if (lastError == 0 || lastError == ERROR_NO_MORE_FILES )
			res->resultMessage = buildResponseMessageType226(res->heapHandler);
		else
			res->resultMessage = buildResponseMessageType426(res->heapHandler);
	}

	void doOfflineHelp(ClientThreadResource * res , ListenSocket clientSocket){
			char * msg = "USER\r\nTYPE\r\nPWD\r\nCWD\r\nSYST\r\nPASV\r\nNOOP\r\nLIST\r\nSTOR [archivo]\r\nRETR [archivo]\r\nDELE [archivo]\r\nHELP";
			sendBytes(clientSocket , msg , (int) strlen(msg));
			res->resultMessage = buildResponseMessageType226(res->heapHandler);
	}