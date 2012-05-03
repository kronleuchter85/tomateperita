


/**
 * Comandos FTP obligatorios pedidos por la catedra
 */
#define COMMAND_PASSIVE						"PASV"
#define COMMAND_LIST						"LIST"
#define COMMAND_DELETE						"DELE"
#define COMMAND_HELP						"HELP" //---> Falta implementarse
#define COMMAND_NOOP						"NOOP"
#define COMMAND_RETRIEVE					"RETR"
#define COMMAND_STORE						"STOR"
#define COMMAND_PWD							"PWD"
#define COMMAND_CWD							"CWD"


/**
 * Comandos opcionales
 */
#define COMMAND_QUIT						"QUIT"
#define COMMAND_USER						"USER"
#define COMMAND_SYST						"SYST"
#define COMMAND_TYPE						"TYPE"


/**
 * Caracteres especiales en las cadenas de comandos FTP
 */
#define CHARACTER_CRLF						"\r\n"
#define CHARACTER_LF						"\n"
#define CHARACTER_SP						" "


/**
 * Cantidad de bytes a recibir o escribir por archivo
 */
#define FILE_CHUNK 1024
#define STOR_TRANSFER 0
#define RETR_TRANSFER 1
#define LIST_TRANSFER 2



	char * buildResponseMessageType220(HeapHandler hh);
	char * buildResponseMessageType150(HeapHandler hh);
	char * buildResponseMessageType426(HeapHandler hh);
	char * buildResponseMessageType226(HeapHandler hh);
	char * buildResponseMessageType215(HeapHandler hh );	
	char * buildResponseMessageType230(HeapHandler hh );
	char * buildResponseMessageType500(HeapHandler hh );
	char * buildResponseMessageType250(HeapHandler hh );
	char * buildResponseMessageType257(HeapHandler hh , char * text);
	char * buildResponseMessageType227(HeapHandler hh , char * port);

	char * buildResponseMessageType250(HeapHandler hh );