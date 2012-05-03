
#include "windows-commons.h"
#include "ftp-response-builder.h"



	char * buildResponseMessageType220(HeapHandler hh){
		return concat(hh, 2 , " 220 iniciando conexion " , CHARACTER_CRLF);	
	}

	char * buildResponseMessageType150(HeapHandler hh){
		return concat(hh , 2 , " 150 Iniciando transferencia de datos " , CHARACTER_CRLF);
	}

	char * buildResponseMessageType426(HeapHandler hh){
		return concat(hh , 2 , "426 Fracaso " , CHARACTER_CRLF);
	}

	char * buildResponseMessageType226(HeapHandler hh){
		return concat(hh , 2 , "226 Exito " , CHARACTER_CRLF);
	}

	char * buildResponseMessageType227(HeapHandler hh , char * port){
		return concat(hh , 4 , 
			"227 (" , encodeHostAndPort(getServerAddress() , port , hh), ")" , CHARACTER_CRLF);
	}

	char * buildResponseMessageType215(HeapHandler hh ){
		return concat(hh , 2 , "215 Windows XP " , CHARACTER_CRLF);
	}

	char * buildResponseMessageType230(HeapHandler hh ){
		return concat(hh , 2 , "230 Ok " , CHARACTER_CRLF);
	}

	char * buildResponseMessageType257(HeapHandler hh , char * text){
		return concat(hh, 3 , "257 " , text , CHARACTER_CRLF);
	}

	char * buildResponseMessageType500(HeapHandler hh ){
		return concat(hh , 2 ,"500 Comando no permitido" , CHARACTER_CRLF);
	}
	
	char * buildResponseMessageType200(HeapHandler hh){
		return concat(hh , 2 , "200 Ok " , CHARACTER_CRLF);
	}

	char * buildResponseMessageType250(HeapHandler hh){
		return concat(hh , 2 , "250 Ok " , CHARACTER_CRLF);
	}
