#include "windows-commons.h"
#include <time.h>


	int equalsStrings(char * s1 , char * s2){
		return !strcmp( s1 , s2);
	}

	char * newString(HeapHandler h , char * content , int size){
		char * s = (char*)allocateMemory(h , (size + 1));
		strncpy(s,content , size);
		return s;
	}

	char * concatString(char * a , char * b , HeapHandler hh){
		char * c = NULL;
		int size = 0;

		if(a != NULL)
			size += (int)strlen(a);
		if(b != NULL)
			size += (int)strlen(b);

		c = allocateMemory(hh , size);
		sprintf(c , "%s%s" , a , b);
		return c;
	}


	/**
	 * Toma el numero de argumentos y luego una lista 
	 * variable de strings y los concatena para generar 
	 * un solo mensaje.
	 */
	char * concat(HeapHandler hh , int n , ...){

		char buffer[MAX_CONCATENATION_LENTH];
		va_list list;
		int i;
		va_start(list , n);
		ZeroMemory(buffer , MAX_CONCATENATION_LENTH);

		for(i =0 ; i<n ; i++){
			strcat(buffer , va_arg(list,char*));
		}
		va_end(list);	
		return trimBlankSpaces(buffer , hh);
	}

	char * intToString(int n , HeapHandler hh){
		char * s = allocateMemory(hh , numdigits(n)+1);
		itoa(n , s , 10);
		return s;
	}

	int numdigits(int n){
		int count = 1;
		while( (n = (n / 10)) > 0)
			count++;
		return count;
	}

	/**
	 * Se fija si la primera cadena empieza con la segunda
	 * cadena.
	 * Itera para todos los eleementos de la segunda cadena
	 * si son los mismos que los n primeros de la primera cadena
	 * sin prestar atencion al CASE.
	 * Devuelve TRUE en caso afirmativo, FALSE en caso contrario.
	 */
	BOOL startsWith(char * s1 , char * s2){
		int i;
		if ((s1 == NULL) || (s2 == NULL)) return FALSE;
		for(i=0; i<(int)strlen(s2) && i<(int)strlen(s1); i++){
			if(toupper(s1[i]) != toupper(s2[i]))
				return FALSE;
		}
		return TRUE;
	}

	char * replaceString(char * string , char replaced , char replacing , HeapHandler h){
		int i;
		int size = (int)strlen(string);
		char * dest = newString(h , "" , size);
		char buffer[1024];
		strcpy(buffer , string);
		for( i=0; i<size ; i++){
			if(buffer[i] == replaced){
				buffer[i] = replacing;
			}
		}
		strncpy(dest , buffer , size);
		return dest;
	}

	char * subString(char * string , int startIndex , int endIndex , HeapHandler h){
		char * theNewString = newString(h , "" , endIndex - startIndex + 1);
		strncpy(theNewString , string + startIndex , endIndex - startIndex + 1);
		return theNewString;
	}

	int indexOfString(char * s , char * b){
		return (int)strcspn(s , b); 
	}

	void blockMutex(HANDLE mutex){
		WaitForSingleObject(mutex , INFINITE);
	}

	void unblockMutex(HANDLE mutex){
		ReleaseMutex(mutex);
	}

	
	int indexOfChar(char * string , char c){
		if(strchr(string , c) != NULL){
			int lenT = (int)strlen(string);
			int len2 = (int)strlen(strchr(string , c));
			return lenT - len2;
		}else {
			return -1;
		}
	}

	char * trim(char * string , HeapHandler h){
		int start = 0;
		int end = (int)strlen(string) -1;

		if (end <= start) return string;

		while(string[start] == ' ')
			start++;

		while(string[end] == ' ' || string[end] == '\n' || string[end] == '\0')
			end--;

		return subString(string , start , end , h);
	}
	
	char * trimBlankSpaces(char * string , HeapHandler h){
		int start = 0;
		int end = (int)strlen(string) -1;

		if (end <= start) return string;

		while(string[start] == ' ')
			start++;

		while(string[end] == ' ')
			end--;

		return subString(string , start , end , h);
	}

	char * trimNewLine(char * string , HeapHandler h){
		int start = 0;
		int end = (int)strlen(string) -1;

		if (end <= start) return string;

		while(string[start] == ' ')
			start++;

		while(string[end] == '\n')
			end--;

		return subString(string , start , end , h);
	}

	char * trimAll(char * string , HeapHandler h){
		int start = 0;
		int end = (int)strlen(string) -1;

		if (end <= start) return string;

		while(string[start] == ' ')
			start++;

		while(string[end] == ' ' || string[end] == '\n' || 
			string[end] == '\0' || string[end] == '\r')
			end--;

		return subString(string , start , end , h);
	}


	char * serializeInt(int v , int spacesToFix , HeapHandler hh){
		char * result = intToString(v , hh);

		while((int)strlen(result) <spacesToFix)
			result = concatString("0" , result , hh);

		return result;
	}

	char * randomBytes(int size , HeapHandler hh) {
		int n;
		char * p;
		char * buf;
		if (size<=0) return NULL;
		buf = newString(hh ,"", size);

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
	}

	char * generateRandomKey(int size , HeapHandler hh){		
		return randomBytes(size , hh);
	}





	/*
	 * s1 - string contenedor
	 * s2 - lo que hay q reemplazar
	 * s3 - con que hay q reemplazarlo
	 */
	char * replaceAll(char * s1 , char * s2 , char * s3){

		char * found = s1;
		int pos = 0;
		char * last;
		int size;
		char * buffer;
		int count = 0;
		char * founds = s1;

		while((founds = strstr(founds , s2)) != NULL){
			founds +=1;
			count++;
		}

		if(count == 0)
			return s1;

		size = (int)strlen(s1) + count*((int)strlen(s3) - (int)strlen(s2));
		buffer = malloc(size+1);
		ZeroMemory(buffer , size+1);

		while( (found = strstr(found , s2)) != NULL ){
			int cal = (int)strlen(s1) - (int)strlen(found) - pos;
			strncat(buffer , s1 + pos , cal);
			strcat(buffer , s3);
			pos += cal + (int)strlen(s2);
			found = found +(int)strlen(s2);
			last = found;
		}

		if(strcmp(last , ""))
			strcat(buffer , last);

		return buffer;
	}

