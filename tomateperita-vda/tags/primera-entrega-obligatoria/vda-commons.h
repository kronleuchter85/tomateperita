/**
 *  VDA common stuff
 */

#ifndef _W_VDA_COMMONS_H_
#define _W_VDA_COMMONS_H_

#include <stdio.h>

#define SECTOR_SIZE 512
#define SECTORLIST_SIZE 5

typedef unsigned int uint;
typedef unsigned int tSectorId;
typedef unsigned int tCounter;
typedef int tSectorList[SECTORLIST_SIZE];

typedef struct {
  tSectorId sectorId;
  char sectorContent[SECTOR_SIZE];
} LogicalSector;

typedef struct {
  LogicalSector sector1;
  LogicalSector sector2;
} PuttingTransferSector;

typedef struct {
  char sectorContent1[SECTOR_SIZE];
  char sectorContent2[SECTOR_SIZE];
} GettingTransferSector;

	void raiseError (char * text);
	void sector_Copy (LogicalSector * a, LogicalSector * b);    /* a <- b */
	void sector_Build (LogicalSector * ls, tSectorId sid, char * content);

	LogicalSector * sector_BuildLogical (tSectorId sid, char * content); /* Reserva memoria */
	GettingTransferSector * sector_BuildGetting(LogicalSector * ls1, LogicalSector * ls2); /* Reserva memoria */
	void sector_Putting2Logical (PuttingTransferSector *pts, LogicalSector * ls1, LogicalSector * ls2);

	char * sector_getFirstLastBytes(LogicalSector * ls); /* Reserva memoria */

	/* Funciones para la lista que usa obtenerSectores() */
	void sectorList_init (tSectorList *sl);
	void sectorList_addItem (tSectorList *sl, tSectorId id);
	tSectorId sectorList_getItem (tSectorList *sl);
	/* Fin funciones para la lista que usa obtenerSectores() */
	
	void info(char * m);
	void debug(char * m);
	void error(char * m);

#endif