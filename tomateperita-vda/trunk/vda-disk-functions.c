/**
 *  Planificacion de disco - Algoritmo SCAN
 */
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "windows-commons-memory.h"

#include "vda-disk-functions.h"
#include "vda-commons.h"

void diskParmInit (tDiskParm * d, uint numTracks, uint numSectors, uint rpm, float delay) {
	  /*d->nTracks = DISK_TRACKS;
        d->nSectors = TRACK_SECTORS;
        d->trackDelay = TRACK_DELAY;*/
        d->nTracks = numTracks;
        d->nSectors = numSectors;
        d->trackDelay = delay;
        d->sectorDelay = (float) 60000 / (d->nSectors * rpm);
        d->sectorInc = (int) ceil (d->trackDelay / d->sectorDelay);
        d->sectorPointer = 0;
        d->trackPointer = 0;
        d->up = 1;
        }

tHistory * historyCreate (int nSectors, int nTracks) {
		tHistory * h;
		h = myAlloc(sizeof (tHistory));
		h->size = nSectors + (2 * nTracks);
		h->buf = myAlloc(h->size * sizeof (int));
		h->ini = h->buf;
		h->end = h->buf;
		return h;
}

void historyAdd (tHistory * h, tSectorId sectorFound){
	*(h->end) = sectorFound;
	h->end++;
}

int historyExtract (tHistory * h){
	int aux = -1;
	if (h->ini < h->end) {
		aux = *(h->ini);
		h->ini++;
	}
	return aux;
}

void searchParmCreate (tSearchParm * s, tDiskParm * d, tSectorId givenSector) {
        s->sTrack = givenSector / d->nSectors;
        s->sSector = givenSector % d->nSectors;
        s->history = historyCreate (d->nSectors, d->nTracks);
        s->time = 0;
        }

void searchParmDestroy (tSearchParm * s) {
	myFree(s->history);
}

int CHS2Int(int tracks, int C, int S) {
        return (C * tracks + S);
        }

void nextTrack (tDiskParm * d, tSearchParm * s) {
        s->time += d->trackDelay;
        d->sectorPointer += d->sectorInc;
        d->sectorPointer = d->sectorPointer % d->nSectors;
        d->trackPointer++;
        }

void lastTrack (tDiskParm * d, tSearchParm * s) {
        s->time += d->trackDelay;
        d->sectorPointer += d->sectorInc;
        d->sectorPointer = d->sectorPointer % d->nSectors;
        d->trackPointer--;
        }

void gotoSector (tDiskParm * d, tSearchParm * s) {
        s->time += (s->sSector >= d->sectorPointer) ? (s->sSector - d->sectorPointer) * d->sectorDelay : (float)(s->sSector + d->nSectors - d->sectorPointer) * d->sectorDelay;
        d->sectorPointer = s->sSector;
        }

void scanSector(tDiskParm * d, tSearchParm * s, tSectorId givenSector) {
        searchParmCreate(s, d, givenSector);
		if (givenSector == CHS2Int(d->nSectors, d->trackPointer, d->sectorPointer)){
			/* Si busco el sector donde estoy parado no hace falta buscar nada
			pero igual lo guardo en el historial para despues imprimirlo*/
			historyAdd(s->history, CHS2Int(d->nSectors, d->trackPointer, d->sectorPointer));
            return; 
		}
        while (d->trackPointer != s->sTrack) {
            if (d->up == 1) {
                if (d->trackPointer == (d->nTracks)-1) {
                   d->up = 0;
                   lastTrack(d, s);
                   }
                else
                   nextTrack(d, s);
                }
            else {
                if (d->trackPointer == 0) {
                   d->up = 1;
                   nextTrack(d, s);
                   }
                else
                   lastTrack(d, s);
                }
			historyAdd(s->history, CHS2Int(d->nSectors, d->trackPointer, d->sectorPointer));
            }
        if (d->sectorPointer != s->sSector) {
            gotoSector(d, s);
			historyAdd(s->history, CHS2Int(d->nSectors, d->trackPointer, d->sectorPointer));
            }
        return;
        };

/* 
Funciones para shell y API:
*/

tSectorId disk_lastSectorId(tDiskParm * d){
	return (d->nTracks * d->nSectors -1);
}

uint disk_getHeadPos(tDiskParm * d) {
	return CHS2Int(d->trackPointer, d->nSectors, d->sectorPointer);
}

char * disk_getHeadPos_s(tDiskParm * d) { 
	char *buf = myAlloc(6 * sizeof (char));
	sprintf_s(buf, 6, "%d", disk_getHeadPos(d));
	return buf;
	}

infoCHS * disk_getCHS(tDiskParm * d) {
	infoCHS * chs = myAlloc(sizeof(infoCHS));
	chs->cilinders = d->nTracks;
	chs->heads = DISK_HEADS;
	chs->sectors = d->nSectors;
	return chs;
}

uint disk_size(tDiskParm * d) {
	return (disk_lastSectorId(d)+1 * SECTOR_SIZE);
}

float disk_worstTime(tDiskParm * d) {
	return (d->trackDelay * d->nTracks) + (d->sectorDelay * (d->nSectors - 1));
}

float disk_averageTime(tDiskParm * d) {
	return (disk_worstTime(d) / 2);
}

uint disk_transferRate(tDiskParm * d) {
	float dt = disk_averageTime(d);
	return (dt==0) ? 0:(1000 * SECTOR_SIZE / (uint) ceil (dt));
}

float search_getTime(tSearchParm * s) {
	return s->time;
}

char * search_getHistory(tSearchParm * s) {
	int sid;
	char aux[SECTOR_STRBUF];
	char *str = myAlloc(SECTOR_STRBUF * s->history->size);
	*str = '\0';
	sid = historyExtract(s->history);
	while (sid != -1) {
		_itoa_s(sid, aux, SECTOR_STRBUF, 10);
		strcat_s(str, SECTOR_STRBUF * s->history->size, aux );
		strcat_s(str, SECTOR_STRBUF * s->history->size, ", ");
		sid = historyExtract(s->history);
	}
	if (strlen(str)>=2)
		*(str + (strlen(str)-2)) = '\0';
	return str;
}
