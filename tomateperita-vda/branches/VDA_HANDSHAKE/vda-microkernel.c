

#include <ctype.h>
#include <string.h>

#include "windows-commons-logging.h"
#include "windows-commons-mps.h"
#include "windows-commons-remote-operations.h"
#include "windows-commons-memory.h"

#include "vda-thread-kss-worker.h"
#include "vda-commons.h"
#include "vda-state.h"
#include "vda-configuration.h"
#include "vda-shell.h"
#include "vda-api.h"




	void finalizeResources();
	void launchShellServiceThread();
	void launchKssServiceThread();
	void initializeCache();
	void initializeDisk();
	void processShell();
	void initializeKssConnection();

	void configure(){
		setLoggingLevelEnabled(LOGGING_LEVEL_DEBUG);
		setVdaRunningStatus(TRUE);
		setGlobalHeap( createHeap(1024) );
		setupConfiguration();
		if (!isVdaOfflineMode()) {
			initializeWinsock();
			initializeKssConnection();
		}
		if(isVdaAvailableCache())
			initializeCache();
		initializeDisk();
	}

	void initializeDisk() {
		/* Inicializo la estructura contenedora de parametros del disco usada en calculos y simulacion. */
		/* Uso getters de vda-configuration.h, info que viene directamente del archivo de configuracion. */
		if( isInfoEnabled()) info( "Iniciando parametros del disco");
		diskParmInit (getDiskParms(), getVdaTrackCount(), getVdaSectorCount(), getiRpm(), getTrackDelay());
	}

	void initializeKssConnection(){
		
		ListenSocket kssConnectionSocket;
		
		if( isInfoEnabled()) info( "Iniciando la conexion con el KSS");
		
		kssConnectionSocket = openConnectionSocket( getKssAddress() , getKssPort());
		
		setKssConnection( kssConnectionSocket );
	
		if( isInfoEnabled())
			if( isKssConnectionOpen() ){

				RuntimeErrorValidator * validator;
				List arguments ;
				MpsMessage * request ;
				MpsMessage * response ;

				info( concat(getGlobalHeap() , 4, 
					"Proceso VDA conectado con proceso remoto Kss en host " , 
					getKssAddress() , ":" , getKssPort()));

				validator = buildErrorSuccessValidator(getGlobalHeap());
				arguments = buildStringsListWithValues(getGlobalHeap() , 1, getVdaName());
			
				request = buildMpsMessage(generateRandomKey(16 , getGlobalHeap()) , MPS_RESPONSE_STATUS_CODE_SUCCESS ,
					VDA_IDENTIFY_NAME , arguments , getGlobalHeap());
				
				response = sendAndReceiveMpsMessages(kssConnectionSocket , request , validator, getGlobalHeap());
			
				if( response->statusCode == MPS_RESPONSE_STATUS_CODE_SUCCESS ){
					info(concat(getGlobalHeap() , 2 , "Proceso VDA identificado con el Kss satisfactoriamente bajo el nombre: " , getVdaName()));
				}else{
					error("El proceso no se pudo identificar con el Kss por lo que el Kss no lo reconoce como dispositivo");
				}				

			}else {
				info( "Proceso VDA trabajando en modo offline. Fallo la conexion con servicio KSS");
			}			
	}

	void finalizeResources(){
		if( isInfoEnabled()) info( "Liberando recursos" );
		destroyHeap(getGlobalHeap());
		finalizeWinsock();
	}

	void launchKssServiceThread(){
		setContinueKssThread(TRUE);
		setKssServiceThread( (ThreadHandler) _beginthreadex( NULL, 0, &doKssService, NULL, 0, NULL ) );
	}

	void initializeCache(){
		if(isInfoEnabled())	info( "Iniciando la cache" );
		setVdaCache();
	}

	void processShell(){

		int i=0;
		ShellCommand * command = NULL;
		char commandLine[MAX_BUFFER_LINE];

		printf("\nvda-command> ");
		fgets(commandLine, sizeof(commandLine) - 1, stdin);

		command = createShellCommand(commandLine);

		if(!strcmp(SHELL_COMMAND_QUIT , trim(command->commandName , getGlobalHeap()))){
			setVdaRunningStatus(FALSE);
			printf("\n");
			info(" Finalizando Shell");
			return;
		}

		else if(!strcmp("posicionCabezal" , trim(command->commandName , getGlobalHeap())))
			posicionCabezal();
		else if(!strcmp("obtenerSectores" , trim(command->commandName , getGlobalHeap())) && (command->argsCount >0) && (command->argsCount <=5))
			obtenerSectores(commandTosectorList (command), command->argsCount);
		else if(!strcmp("writeRandom" , trim(command->commandName , getGlobalHeap())) && (command->argsCount == 1))
			writeRandom(atoi(command->args[0]));
		else if(!strcmp("showInfo" , trim(command->commandName , getGlobalHeap())))
			showInfo();
		else if(!strcmp("cacheStatus" , trim(command->commandName , getGlobalHeap())))
			cacheStatus();
		else if(!strcmp("demoGetSectores" , trim(command->commandName , getGlobalHeap())))
			demoGetSectores();
		else if(!strcmp("demoPutSectores" , trim(command->commandName , getGlobalHeap())))
			demoPutSectores();
		else if(!strcmp("demoGetCHS" , trim(command->commandName , getGlobalHeap())))
			demoGetCHS();
		else if(!strcmp("help" , trim(command->commandName , getGlobalHeap())))
			help();
	}


	int main(void){
	
		configure();
		showInfo();

		if (!isVdaOfflineMode()){
			if( isInfoEnabled()) 
				info( "Lanzando proceso servicio de comunicacion con KSS" );
			if(isKssConnectionOpen()) 
				launchKssServiceThread();
		}
		
		info( "Lanzando proceso Shell" );
		help();
		
		Sleep(1000);
		do{
			processShell();

		}while(isVdaRunningStatus());
		
		finalizeResources();
		Sleep(1000);


		//testRandomKeys();

		return EXIT_SUCCESS;	
	}
