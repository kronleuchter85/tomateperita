
#define COMMAND_CHANGEDIR "CWD"

/**
 * Comandos FTP obligatorios pedidos por la catedra
 */
#define COMMAND_PORT "PORT" //---> Se saco despues de especificarse el TP
#define COMMAND_PASSIVE "PASV"
#define COMMAND_LIST "LIST"
#define COMMAND_DELETE "DELE"
#define COMMAND_HELP "HELP" //---> Falta implementarse
#define COMMAND_NOOP "NOOP"
#define COMMAND_RETRIEVE "RETR"
#define COMMAND_STORE "STOR"
#define COMMAND_PWD "PWD"
#define COMMAND_CWD "CWD"


/**
 * Comandos opcionales
 */
#define COMMAND_QUIT "QUIT"
#define COMMAND_USER "USER"
#define COMMAND_SYST "SYST"
#define COMMAND_TYPE "TYPE"
#define COMMAND_OPTS "OPTS"
#define COMMAND_SIZE "SIZE"

/**
 * Caracteres especiales en las cadenas de comandos FTP
 */
#define CHARACTER_CRLF "\r\n"
#define CHARACTER_LF "\n"
#define CHARACTER_SP " "

/**
 * Codigos de respuesta de los encabezados FTP
 */
#define RESPONSE_CODE_220 "220"
#define RESPONSE_CODE_257 "257"
#define RESPONSE_CODE_230 "230"

/**
 * Cantidad de bytes a recibir o escribir por archivo
 */
#define FILE_CHUNK 1024
#define STOR_TRANSFER 0
#define RETR_TRANSFER 1
#define LIST_TRANSFER 2