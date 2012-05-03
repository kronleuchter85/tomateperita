/**
 *  Cosas varias para el VDA
 */

#include "vda-commons.h"
#include "vda-state.h"
#include "memory-commons.h"
#include <stdio.h>
#include <string.h>
#include "logging.h"

	void raiseError (char * text) {
			printf ("[!] ERROR: %s\n", text);
			return;
			}

	void sector_Copy (LogicalSector * a, LogicalSector * b) {
		a->sectorId = b->sectorId;
		memcpy(a->sectorContent, b->sectorContent, SECTOR_SIZE);
		//for (x=0; x<SECTOR_SIZE; x++)
		//	a->sectorContent[x] = b->sectorContent[x];
	}

	/* funcion a reemplazar por sector_BuildLogical() */
	void sector_Build (LogicalSector * ls, tSectorId sid, char * content) {
		ls->sectorId = sid;
		if (content != NULL)
			memcpy(ls->sectorContent, content, SECTOR_SIZE);
	}

	LogicalSector * sector_BuildLogical (tSectorId sid, char * content) {
		LogicalSector * ls = allocateMemory(getGlobalHeap(), sizeof (LogicalSector));
		ls->sectorId = sid;
		if (content != NULL)
			memcpy(ls->sectorContent, content, SECTOR_SIZE);
		else
			memset(ls->sectorContent, '\0', SECTOR_SIZE);
		return ls;
	}

	void sector_Putting2Logical (PuttingTransferSector *pts, LogicalSector * ls1, LogicalSector * ls2) {
		ls1 = &(pts->sector1);
		ls2 = &(pts->sector2);
	}

	GettingTransferSector * sector_BuildGetting(LogicalSector * ls1, LogicalSector * ls2) {
		GettingTransferSector * gts = allocateMemory(getGlobalHeap(), sizeof (GettingTransferSector));
		memcpy(gts->sectorContent1, ls1->sectorContent, SECTOR_SIZE);
		memcpy(gts->sectorContent2, ls2->sectorContent, SECTOR_SIZE);
		return gts;
	}

	char * sector_getFirstLastBytes(LogicalSector * ls) {
		char *buf = allocateMemory(getGlobalHeap(), 30 * sizeof (char));
		//memset(buf,'x',30);
		*buf='"';
		memcpy(buf+1, ls->sectorContent, 10);
		memcpy(buf+15, ls->sectorContent + SECTOR_SIZE - 11, 10);
		strcpy_s(buf+11, 6, "\"...\"");
		*(buf+26)='"';
		*(buf+27)='\0';
		return buf;
	}

	/* Funciones para la lista que usa obtenerSectores() */
	void sectorList_init (tSectorList *sl) {
		int i;
		for (i=0; i < SECTORLIST_SIZE; i++)
			(*sl)[i] = -1;
	}

	void sectorList_addItem (tSectorList *sl, tSectorId id) {
		int i;
		for (i=0; ((*sl)[i] != -1) && (i < SECTORLIST_SIZE); i++);
		(*sl)[i] = id;
	}

	tSectorId sectorList_getItem (tSectorList *sl) {
		int x;
		int i=0;
		while (i < SECTORLIST_SIZE) {
			if ((*sl)[i] != -1) {
				x = (*sl)[i];
				(*sl)[i] = -1;
				return x;
			}
			i++;
		}
		return -1;
	}
		
	/* Fin funciones para la lista que usa obtenerSectores() */

	void info(char * m){
		logInfo("VDA" , m , getGlobalHeap());
	}

	void debug(char * m){
		logDebug("VDA" , m , getGlobalHeap());
	}

	void error(char * m){
		logError("VDA" , m , getGlobalHeap());
	}
