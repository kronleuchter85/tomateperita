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
		char *buf = myAlloc(30 * sizeof (char));
		*buf='\"';
		memcpy(buf+1, ls->sectorContent, 10);
		strcpy_s(buf+11, 6, "\"...\"");
		memcpy(buf+16, ls->sectorContent + SECTOR_SIZE - 11, 10);
		*(buf+26)='\"';
		*(buf+27)='\0';
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

	char * randomBytes(int size) {
		int n;
		char * p;
		char * buf = myAlloc(size * sizeof (char));
		p=buf;
		srand((unsigned)time( NULL ));
		while (p < (buf + size -4)) {
			n = rand() % 127;
			if ((n>=33) && (n<=126))  
				(*p) = (char) n;
			else
				(*p) = '-';
			p++;
		}
		*p='x';p++;*p='y';p++;*p='z';p++;*p='\0';
		return buf;
		//return "Esta es una frase para persistir, hoy es un lindo dia.";
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

	void errorInvalidSector(tSectorId sid) {
		error(concat( getGlobalHeap() , 2 , "[VAL] Numero de sector invalido: ", itoa_buf(sid)));
	}