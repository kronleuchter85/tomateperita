/**
 * ftp-microkernel.h
 *
 * Constantes y funciones que controlan señales de control
 * del microkernel del servidor.
 *
 */
#include "windows-commons-errors.h"

	void startService(RuntimeErrorValidator * validator);
	void doDataService(ClientThreadResource * );

	unsigned __stdcall serviceDataThread( void* pArguments );
	unsigned __stdcall serviceControlThread( void* pArguments );

	void execute(char * aRequest , ClientThreadResource * aResource );
	void executeCommandPwd(ClientThreadResource *);
	void executeCommandUser(ClientThreadResource *);
	void executeCommandSyst(ClientThreadResource *);	
	void executeCommandType(ClientThreadResource *, char *  );
	void executeCommandPasv(ClientThreadResource *);	
	void executeCommandList(ClientThreadResource *);
	void executeCommandNotAllowed(ClientThreadResource * );
	void executeCommandStore(ClientThreadResource *, char * );
	void executeCommandRetrieve(ClientThreadResource *, char * );
	void executeCommandCwd(ClientThreadResource * , char * );
	void executeCommandNoop(ClientThreadResource * );



	/*
	 * Offline operations
	 */
	
	void doOfflineListFiles(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator);
	void doOfflineRetrieveFile(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator);
	void doOfflineDeleteFile(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator);
	void doOfflineStoreFile(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator);


	/*
	 * Online operations
	 */

	
	void doOnlineDeleteFile(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator);
	void doOnlineStoreFile(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator);
	void doOnlineRetrieveFile(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator);
	void doOnlineListFiles(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator);




