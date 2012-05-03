/**
 *  Cosas varias para el VDA
 */

#include "vda-commons.h"
#include "vda-state.h"
#include "memory-commons.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "logging.h"
#include "vda-configuration.h"
#include "commons.h"


	void * myAlloc(int size) {
		return allocateMemory(getGlobalHeap(), size);
	}

	void myFree(void * ptr) {
		if (ptr!=NULL) 
			freeMemory(getGlobalHeap(), ptr);
	}

	int sectorInvalido(int sid){
		return (sid > disk_lastSectorId(getDiskParms()) || (sid < 0));
	}

	void sector_Copy (LogicalSector * a, LogicalSector * b) {
		a->sectorId = b->sectorId;
		memcpy(a->sectorContent, b->sectorContent, SECTOR_SIZE);
	}

	char * fillLastBytesWithSectorBlank(char * contenido){
		/* Cuando encuentra \0, completa hasta el final con ese mismo caracter.
		   Sino deja todo igual */
		char * p = contenido;
		if (p != NULL)
			while ((p < contenido + SECTOR_SIZE - 1) && (p != '\0'))
				p++;
			while (p < contenido + SECTOR_SIZE - 1) {
				p++;
				*p='\0';
			}
		return contenido;
	}

	void sector_BuildLogical_nr (LogicalSector * ls, tSectorId sid, char * content) { /*No reserva memoria*/
		ls->sectorId = sid;
		if (content == NULL)
			memset(ls->sectorContent, '\0', SECTOR_SIZE);
		//else if (strlen(content) == 1)
		//	memset(ls->sectorContent, *content, SECTOR_SIZE);
		else {
			memcpy(ls->sectorContent, content, SECTOR_SIZE);
			fillLastBytesWithSectorBlank(ls->sectorContent);
		}
	}

	LogicalSector * sector_BuildLogical (tSectorId sid, char * content) { /* Reserva memoria */
		LogicalSector * ls;
		//if (sectorInvalido(sid))
		//	return NULL;
		ls = myAlloc(sizeof (LogicalSector));
		sector_BuildLogical_nr(ls, sid, content);
		return ls;
	}

	GettingTransferSector * sector_BuildGetting(LogicalSector * ls1, LogicalSector * ls2) {
		GettingTransferSector * gts;
		if ((ls1 == NULL) || (ls2 == NULL)) return NULL;
		gts = myAlloc(sizeof (GettingTransferSector));
		memcpy(gts->sectorContent1, ls1->sectorContent, SECTOR_SIZE);
		memcpy(gts->sectorContent2, ls2->sectorContent, SECTOR_SIZE);
		return gts;
	}

	char * sector_getFirstLastBytes(LogicalSector * ls) {
		int i;
		char * p;
		char *buf = myAlloc(28 * sizeof (char));
		//char buf[28];
		buf[0]='\"';
		p = ls->sectorContent;
		i = 1;
		while ((*p != '\0') && (i<=10)){
			buf[i] = *p;
			i++;
			p++;
		}
		while (i<=10) {
			buf[i] = ' ';
			i++;
		}
		strcpy_s(buf+11, 6, "\"...\"");
		p = ls->sectorContent + 10;
		while (p < ls->sectorContent + SECTOR_SIZE - 11) {
			if (*p == '\0') {
				strcpy_s(buf+16, 12, "          \"");
				return buf;
			}
			p++;
		}
		i=16;
		while ((*p != '\0') && (i<=25)){
			buf[i] = *p;
			i++;
			p++;
		}
		while (i<=25) {
			buf[i] = ' ';
			i++;
		}
		buf[26]='\"';
		buf[27]='\0';
		return buf;
	}

	/* Funcion para la lista que usa obtenerSectores() */
	tSectorId * commandTosectorList (ShellCommand * c) {
		int i;
		tSectorId *buf = myAlloc(SECTORLIST_SIZE * sizeof (tSectorId));
		for(i=0; (i<c->argsCount) && (i<=SECTORLIST_SIZE); i++)
			buf[i] = atoi(c->args[i]);
		return buf;
	}

	char * itoa_buf(int n) {
		char * buf = myAlloc(SECTOR_STRBUF * sizeof (char));
		_itoa_s(n, buf, SECTOR_STRBUF, 10);
		return buf;
	}



	void info(char * m){
		logInfo("VDA" , m , getGlobalHeap());
	}

	void debug(char * m){
		logDebug("VDA" , m , getGlobalHeap());
	}

	void error(char * m){
		logError("VDA" , m , getGlobalHeap());
	}

	/*
	void infoScreen(char * m){
		logInfoScreen("VDA" , m , getGlobalHeap());
	}

	void debugScreen(char * m){
		logDebugScreen("VDA" , m , getGlobalHeap());
	}

	void errorScreen(char * m){
		logErrorScreen("../logs/VDA" , m , getGlobalHeap());
	}
    */
	void errorInvalidSector(tSectorId sid) {
		error(concat( getGlobalHeap() , 2 , "[VAL] Numero de sector invalido: ", itoa_buf(sid)));
	}