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
#include "linux-commons.h"


#define INT_CONVERSION_MAX_DIGITS_COUNT 12

	BOOL startsWith(char * s1 , char * s2){
		int i=0; for( ; i<strlen(s2) ; i++){
			if(toupper(s1[i]) != toupper(s2[i]))
				return FALSE;
		}
		return TRUE;
	}


	char * itoa(int i){
		char * content = malloc(INT_CONVERSION_MAX_DIGITS_COUNT);
		bzero(content, INT_CONVERSION_MAX_DIGITS_COUNT);
		sprintf(content , "%i" , i);
		content = strndup(content , strlen(content));
		return content;
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
		int start = 0;
		int end = strlen(string) -1;

		while(string[start] == ' ')
			start++;

		while(string[end] == ' ' || string[end] == '\n' || string[end] == '\0')
			end--;

		return getSubString(string , start , end);
	}

