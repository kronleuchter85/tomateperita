/*
* commons.h
*
*  Created on: 29/05/2011
*      Author: gonzalo
*/
#include <pthread.h>

#ifndef COMMONS_DEFINITIONS
#define COMMONS_DEFINITIONS

#define FALSE 0
#define TRUE 1
#define SOCKET_ERROR -1

	/**
	* Alias para darle nombres mas bonitos ^^
	*/

	typedef pthread_t Thread;
	typedef int BOOL;
	typedef pthread_mutex_t ThreadMutex;

	BOOL startsWith(char * s1 , char * s2);
	char * itoa(int i);
	int getIndexOf(char * string , char c);
	char * getSubString(char * string , int startIndex , int endIndex);
	char * trim(char * string);


#endif



