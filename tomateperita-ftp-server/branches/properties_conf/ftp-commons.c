
#include <stdlib.h>

#include "windows-commons.h"
#include "windows-commons-logging.h"

#include "ftp-state.h"
#include "ftp-commons.h"



	char * encodeHost(char * address , HeapHandler h){
		return replaceString(address , '.' , ',' , h);
	}


	char * encodePort(char * port , HeapHandler h){		
		int q = atoi(port)/ 256;
		int r = atoi(port) % 256;
		return concat(h , 3 , intToString(q , h) , "," , intToString(r,h) , h);
	}
	

	char * calculatePort(char * port , HeapHandler h){
		 return intToString(atoi(port) + 5 , h);
	}

	
	char * decodeHost(char * m , HeapHandler h){				
		int i, found = 0;
		for(i=0 ; m[i] != '\0' ; i++){
			if(m[i] == ','){
				found++;
				if(found == 4)
					break;
			}
		}		
		return subString(m , 0 , i-1 , h);
	}


	char * decodePort(char * m , HeapHandler h){
		char * encodedPort , * q , * r;
		int i, found = 0;		
		for(i=0; m[i] != '\0' ; i++){
			if(m[i] == ','){
				found++;
				if(found == 4){
					i++;
					break;
				}
			}
		}
		encodedPort = subString(m , i , strlen(m) , h);

		q = subString(encodedPort , 0 , indexOfString(encodedPort , ",")-1 , h);
		r = subString(encodedPort , indexOfString(encodedPort , ",")+1 , strlen(encodedPort) , h);
		
		return intToString(atoi(q)*256 + atoi(r) , h);		
	}

	
	char * parseHostAndPort(char * request , HeapHandler h){
		return subString(request , 5 , strlen(request) , h);
	}


	char * encodeHostAndPort(char * host , char * port , HeapHandler h){
		return concat(h , 3 ,  encodeHost(host , h) , "," , encodePort(port , h));	
	}
	

	void info(char * m){
		logInfo( FTPS , m , getGlobalHeap());
	}
	void error(char * e){
		logError( FTPS , e , getGlobalHeap());
	}
	void debug(char * d){
		logDebug( FTPS , d , getGlobalHeap());
	}


	

	char * getPhysicalFilePath( ClientThreadResource * res , char * fileName ){
		fileName = concat(res->heapHandler , 3 , getDataDirectory() ,  res->relativeWorkingDir , fileName) ;
		fileName = replaceAll(fileName , "\"" , "");
		return fileName;
	}
