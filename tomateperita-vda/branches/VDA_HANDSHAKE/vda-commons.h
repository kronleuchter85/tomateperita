/**
 *  VDA common stuff
 */

#ifndef _W_VDA_COMMONS_H_
#define _W_VDA_COMMONS_H_

#include <stdio.h>

#include "vda-shell.h"


#define SECTOR_SIZE 512
#define SECTORLIST_SIZE 5
#define SECTOR_BLANK "z"
#define SECTOR_STRBUF 10

typedef unsigned int uint;
typedef unsigned int tCounter;
typedef int tSectorId;
typedef int tSectorList[SECTORLIST_SIZE];

typedef struct {
  tSectorId sectorId;
  char sectorContent[SECTOR_SIZE];
} LogicalSector;

typedef struct {
  char sectorContent1[SECTOR_SIZE];
  char sectorContent2[SECTOR_SIZE];
} GettingTransferSector;

typedef struct {
  int cilinders;
  int heads;
  int sectors;
} infoCHS;

/*
typedef struct {
  LogicalSector *sector1;
  LogicalSector *sector2;
} PuttingTransferSector;
*/

	void sector_Copy (LogicalSector * a, LogicalSector * b);    /* a <- b */
	void sector_BuildLogical_nr (LogicalSector * ls, tSectorId sid, char * content);
	LogicalSector * sector_BuildLogical (tSectorId sid, char * content); /* Reserva memoria */
	GettingTransferSector * sector_BuildGetting(LogicalSector * ls1, LogicalSector * ls2); /* Reserva memoria */

	char * sector_getFirstLastBytes(LogicalSector * ls); /* Reserva memoria */
	tSectorId * commandTosectorList (ShellCommand * c); /* Reserva memoria */
	int sectorInvalido(int sid);

	char * itoa_buf(int n);

	/* Memoria */
	void * myAlloc(int size);
	void myFree(void * ptr);

	/* Logging */
	void info(char * m);
	void debug(char * m);
	void error(char * m);
	
	void errorInvalidSector(tSectorId sid);

#endif