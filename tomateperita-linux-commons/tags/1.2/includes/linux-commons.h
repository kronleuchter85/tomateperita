/*
* commons.h
*
*  Created on: 29/05/2011
*      Author: gonzalo
*/
#include <pthread.h>
#include <stdio.h>
#include <dirent.h>

#ifndef COMMONS_DEFINITIONS
#define COMMONS_DEFINITIONS

#define FALSE 0
#define TRUE 1
#define SOCKET_ERROR -1

#define SYSTEM_FILE_LINE_LENGTH 					100
#define SYSTEM_INT_CONVERSION_MAX_DIGITS_COUNT 		12
#define SYSTEM_STRING_CONCATENATION_SIZE			2048

	/**
	* Alias para darle nombres mas bonitos ^^
	*/

	typedef pthread_t Thread;
	typedef int Bool;
	typedef pthread_mutex_t ThreadMutex;
	typedef FILE File;
	typedef struct dirent FilesIterator;
	typedef DIR Directory;

	typedef int * Integer;


	int getIndexOf(char * string , char c);

	char * ltoa(long int l);
	char * itoa(int i);
	char * getSubString(char * string , int startIndex , int endIndex);
	char * trim(char * string);
	char * concat(char * s1 , char * s2);
	char * concatAll(int n , ...);

	void cleanMemory(void * m , int size);

	Bool equalsStrings(char * s1 , char * s2);
	Bool startsWith(char * s1 , char * s2);
	Bool endsWith(char * s1 ,char * s2);
	char * cloneString(char * string);

	char * serializeInt(int v , int spacesToFix);

	Integer newInteger(int);

	char * generateRandomKey(int size);


	void lockThreadMutex(pthread_mutex_t * m);
	void unlockThreadMutex(pthread_mutex_t * m);

	char * replaceAll(char * s1 , char * s2 , char * s3);


#endif




