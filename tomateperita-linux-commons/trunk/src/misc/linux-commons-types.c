/*
 * linux-commons.c
 *
 *  Created on: 05/06/2011
 *      Author: gonzalo
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <strings.h>
#include "linux-commons.h"


	/*
	 * Determina si la primera cadena empieza con la segunda
	 */
	Bool startsWith(char * s1 , char * s2){
		int i=0; for( ; i<strlen(s2) ; i++){
			if(toupper(s1[i]) != toupper(s2[i]))
				return FALSE;
		}
		return TRUE;
	}

	/*
	 * Determina si la primera cadena termina con la segunda
	 */
	Bool endsWith(char * s1 ,char * s2){

		if(s1 == NULL || s2 == NULL)
			return FALSE;

		if(!strcmp(s1 , s2))
			return TRUE;

		if(strlen(s1) == 0 || strlen(s2) == 0)
			return FALSE;

		int i;
		for(i = 0 ; i<strlen(s2) ; i++){
			int index1 = strlen(s1)-(1+i);
			int index2 = strlen(s2)-(1+i);
			if(s1[ index1] != s2 [index2])
				return FALSE;
		}
		return TRUE;
	}

	char * itoa(int i){
		char * content = malloc(SYSTEM_INT_CONVERSION_MAX_DIGITS_COUNT);
		bzero(content, SYSTEM_INT_CONVERSION_MAX_DIGITS_COUNT);
		sprintf(content , "%i" , i);
		content = strndup(content , strlen(content));
		return content;
	}


	char * ltoa(long int l){
		char * s = (char*)malloc(sizeof(long int));
		sprintf( s, "%lu" , l);
		return s;
	}

	int getIndexOf(char * string , char c){
		if(strchr(string , c) != NULL){
			int lenT = strlen(string);
			int len2 = strlen(strchr(string , c));
			return lenT - len2;
		}else {
			return -1;
		}
	}

	char * getSubString(char * string , int startIndex , int endIndex){
		if(startIndex == 0)
			return strndup(string + startIndex , endIndex +1);
		else
			return strndup(string + startIndex , endIndex +1 -startIndex);
	}

	char * trim(char * string){

		if(equalsStrings(string , ""))
			return "";

		int start = 0;
		int end = strlen(string) -1;

		while(string[start] == ' ')
			start++;

		while(string[end] == ' ' || string[end] == '\n' )
			end--;

		return getSubString(string , start , end);
	}

	char * concat(char * s1 , char * s2){
		if(s1 == NULL || s2 == NULL)
			return NULL;

		char * buffer = (char *)malloc(strlen(s1) + strlen(s2) +1);
		sprintf(buffer, "%s%s" , s1 , s2);
		return trim(buffer);
	}


	char * concatAll(int n , ...){

		char buffer[SYSTEM_STRING_CONCATENATION_SIZE];
		cleanMemory(buffer , SYSTEM_STRING_CONCATENATION_SIZE);

		va_list ap;
		va_start(ap, n);
		int i; for(i=0 ; i<n ; i++){
			strcat(buffer, va_arg(ap, char*));
		}
		va_end(ap);

		return trim(buffer);
	}


	Bool equalsStrings(char * s1 , char * s2){
		return !strcmp(s1 , s2);
	}


	void cleanMemory(void * m , int size){
		memset(m, '\0' , size);
	}


	Integer newInteger(int i){
		Integer integer = (Integer)malloc(sizeof(int));
		*integer = i;
		return integer;
	}

	char * cloneString(char * string){
		if(string == NULL)
			return NULL;
		return strdup(string);
	}

	char * serializeInt(int v , int spacesToFix){
		char * result = itoa(v);

		while(strlen(result) <spacesToFix)
			result = concat("0" , result);

		return result;
	}


	char * generateRandomKey(int size){

		int n;
		char * p;
		char * buf;
		if (size<=0) return NULL;
		buf = malloc(size * sizeof (char));
		p=buf;

		while (p < (buf + size -1)) {
			n = rand() % 127;
			while (!(((n>=65) && (n<=90)) || ((n>=97) && (n<=122)) || ((n>=48) && (n<=57))))
				n = rand() % 127;
			*p = (char) n;
			p++;
		}
		while (!(((n>=65) && (n<=90)) || ((n>=97) && (n<=122)) || ((n>=48) && (n<=57))))
			n = rand() % 127;
		*p = (char) n;

		return buf;

		//return "kck-tq2-xp3-tdcq";
	}


	void lockThreadMutex(pthread_mutex_t * m){
		pthread_mutex_lock(m);
	}

	void unlockThreadMutex(pthread_mutex_t * m){
		pthread_mutex_unlock(m);
	}


	/*
	 * s1 - string contenedor
	 * s2 - lo que hay q reemplazar
	 * s3 - con que hay q reemplazarlo
	 */
	char * replaceAll(char * s1 , char * s2 , char * s3){

		int count = 0;
		char * founds = s1;
		while((founds = strstr(founds , s2)) != NULL){
			founds +=1;
			count++;
		}

		if(count == 0)
			return s1;

		int size = strlen(s1) + count*(strlen(s3) - strlen(s2));
		char * buffer = malloc(size+1);

		bzero(buffer , size+1);

		char * found = s1;
		int pos = 0;
		char * last;
		while( (found = strstr(found , s2)) != NULL ){
			int cal = strlen(s1) - strlen(found) - pos;
			strncat(buffer , s1 + pos , cal);
			strcat(buffer , s3);
			pos += cal + strlen(s2);
			found = found +strlen(s2);
			last = found;
		}

		if(strcmp(last , ""))
			strcat(buffer , last);

		return buffer;
	}

