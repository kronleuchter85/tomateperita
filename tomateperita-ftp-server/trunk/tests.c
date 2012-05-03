#include "windows-commons.h"
#include "windows-commons-headers.h"
#include "windows-commons-socket.h"

#include "windows-commons-memory.h"
#include "windows-commons-logging.h"
#include "windows-commons-errors.h"


#include "ftp-client-thread-resources.h"
#include "ftp-response-builder.h"
#include "ftp-configuration.h"
#include "ftp-state.h"
#include "ftp-commons.h"

	void testReplacing(){
	
		char * request1 = "RETRV el valor de un archivo.rar";
		char * request2 = "STOR el valor de un archivo.rar";

		char * path = replaceAll(request1 , "RETRV" , "");
		path = replaceAll(path , "STOR" , "");
		path = replaceAll(path , "PUTO" , "");

		printf("%s\n" , path);

	}

	void testStrings(){

		char * s1 = "una palabra";
		char * s2 = "otra palabra";

		char * s3 = s1;

		s1 = s2;

		printf("%s\n" , s3);

	
	}