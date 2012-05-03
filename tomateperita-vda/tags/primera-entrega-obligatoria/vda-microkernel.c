
#include "memory-commons.h"
#include "vda-state.h"
#include "vda-configuration.h"
#include "vda-shell.h"
#include <ctype.h>
#include <string.h>
#include "vda-commons.h"
#include "logging.h"
#include "vda-thread-kss-worker.h"


	void finalizeResources();
	void launchShellServiceThread();
	void launchKssServiceThread();
	void initializeCache();
	void processShell();
	void initializeKssConnection();


	void configure(){
		setLoggingLevelEnabled(LOGGING_LEVEL_DEBUG);
		setVdaRunningStatus(TRUE);
		setGlobalHeap( createHeap(1024) );
		setupConfiguration();
		initializeWinsock();
		initializeKssConnection();
		initializeCache();
	}


	void initializeKssConnection(){
		if( isInfoEnabled()) info( "Iniciando la coneccion con el KSS");
		setKssConnection( openConnectionSocket( getKssAddress() , getKssPort()));
	
		if( isInfoEnabled())
			if( isKssConnectionOpen() ){
				info( concat(getGlobalHeap() , 4, 
					"Proceso VDA conectado con proceso remoto Kss en host " , 
					getKssAddress() , ":" , getKssPort()));
			}else {
				info( "Proceso VDA trabajando en modo offline. Fallo la coneccion con servicio KSS");
			}			
	}

	void finalizeResources(){
		if( isInfoEnabled()) info( "Liberando recursos" );
		destroyHeap(getGlobalHeap());
		finalizeWinsock();
	}

	void launchKssServiceThread(){
		setKssServiceThread( _beginthreadex( NULL, 0, &doKssService, NULL, 0, NULL ) );
	}

	void initializeCache(){
		if(isVdaAvailableCache()){
			if( isInfoEnabled()) info( "Iniciando la cache" );

			/**
			 * TODO: Iniciar la cache...
			 */
		}
	}

	void processShell(){

		int i;
		ShellCommand * command = NULL;
		char commandLine[MAX_BUFFER_LINE];

		printf("\n");
		printf("vda-command> ");
		fgets(commandLine, sizeof(commandLine) - 1, stdin);

		command = createShellCommand(commandLine);

		if(!strcmp(SHELL_COMMAND_QUIT , trim(command->commandName , getGlobalHeap()))){
			setVdaRunningStatus(FALSE);
			printf("\n");
			info(" Finalizando Shell");
			return;
		}

		info( "La implementacion de los comandos de calculos esta en fase de Test");
		info( "Disculpe las molestias ocacionadas Sr. Ayudante");
		info( concat( getGlobalHeap() , 2 , "Comando ingresado: " , command->commandName));
		info( concat( getGlobalHeap() , 2 , "Cantidad de argumentos ingresados " , 
			intToString(command->argsCount , getGlobalHeap())));
		
		for(i=0; i<command->argsCount; i++)
			info( concat(getGlobalHeap() , 4, "argumento[", 
				intToString(i, getGlobalHeap()), "]=", command->args[i]));

		printf("\n");
	}


	
	int main(void){
		
		configure();

		if( isInfoEnabled()) info( "Lanzando proceso servicio de comunicacion con KSS" );
		
		if(isKssConnectionOpen()) 
			launchKssServiceThread();

		info( "Lanzando proceso Shell" );
		info( "Ingrese comandos para ser atendidos o bien 'quit' para finalizar el programa" );
		
		Sleep(2000);
		do{
			processShell();

		}while(isVdaRunningStatus());
		
		finalizeResources();
		
		Sleep(1000);

		return EXIT_SUCCESS;	
	}
