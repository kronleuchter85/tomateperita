/**
 *  API para comunicacion con KSS y Shell
 */

#ifndef _W_VDA_API_H_
#define _W_VDA_API_H_

#include "vda-disk-functions.h"

/* Funciones para intercambio con KSS */

	/* Devuelve puntero a estructura de 512+512. Reserva memoria. 
	   Si le paso un numero de sector invalido (ej -1 o un numero mas alto de lo que el disco soporta,
	   los 512 bytes de ese sector seran caracteres \0 */
	GettingTransferSector * getSectores(int s1, int s2);

	/* Devuelve cantidad de bytes escritos (0, 512 o 512+512) */
	int putSectores (int sector1, char * contenido1, int sector2, char * contenido2);

	/* Devuelve puntero a vector de 3 int: (C,H,S). Liberar memoria luego de usarla */
	infoCHS * getCHS(void);


/* Funciones para el shell */
	void posicionCabezal(void);
	void obtenerSectores(tSectorId * slist, int argsCount);
	void help(void);

/* Ampliacion del shell */
	void cacheStatus(void);
	void writeRandom(tSectorId sid);
	void showInfo(void); /* Usada tambien al iniciar el microkernel */

/* Ejemplos de llamado a la API de intercambmio con el KSS  - invocables desde Shell */
	void demoGetSectores(void);
	void demoPutSectores(void);
	void demoGetCHS(void);

#endif