/**
 *  Planificacion de disco - Algoritmo SCAN
 */

#include "vda-disk-functions.h"
#include "vda-commons.h"
#include "vda-state.h"
#include "memory-commons.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

void diskParmInit (tDiskParm * d) {
        d->nTracks = DISK_TRACKS;
        d->nSectors = TRACK_SECTORS;
        d->trackDelay = TRACK_DELAY;
        d->sectorDelay = (float) 60000 / (d->nSectors * RPM);
        d->sectorInc = (int) ceil (d->trackDelay / d->sectorDelay);
        d->sectorPointer = 0;
        d->trackPointer = 0;
        d->up = 1;
        }

/*void diskParmInit (tDiskParm * d, uint numTracks, uint numSectors, uint rpm, uint delay) {
        d->nTracks = numTracks;
        d->nSectors = numSectors;
        d->trackDelay = delay;
        d->sectorDelay = (float) 60000 / (d->nSectors * rpm);
        d->sectorInc = (int) ceil (d->trackDelay / d->sectorDelay);
        d->sectorPointer = 0;
        d->trackPointer = 0;
        d->up = 1;
        }
*/
int searchParmInit (tSearchParm * s, tDiskParm d, tSectorId givenSector) {
        int i;
        s->sTrack = givenSector / d.nSectors;
        s->sSector = givenSector % d.nSectors;
        s->historySize = d.nSectors + (2 * d.nTracks);
        s->time = 0;
        //if (s->history == NULL)
                s->history = (int *) malloc (sizeof (int) * s->historySize);
        if (s->history == NULL) {
                //raiseError ("No se pudo reservar memoria para History");
                return 1;
                }
        for (i=0; i< s->historySize; i++)
                s->history[i]=-1;
        return 0;
        }

int CHS2Int(int tracks, int C, int S) {
        return (C * tracks + S);
        }

void saveHistory (tSearchParm * s, int sectorFound) {
        int i;
        for (i=0; ((s->history[i])!=-1) && (i<s->historySize); i++);
        s->history[i] = sectorFound;
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

int scanSector(tDiskParm * d, tSearchParm * s, tSectorId givenSector) {
        int res;

        res = searchParmInit(s, *d, givenSector);
        if (res != 0) return res;

        if (givenSector == CHS2Int(d->nSectors, d->trackPointer, d->sectorPointer))
            return 0;

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
            saveHistory (s, CHS2Int(d->nSectors, d->trackPointer, d->sectorPointer));
            }
        if (d->sectorPointer != s->sSector) {
            gotoSector(d, s);
            saveHistory (s, CHS2Int(d->nSectors, d->trackPointer, d->sectorPointer));
            }
        return res;
        };


uint disk_getHeadPos(tDiskParm * d) {
	return CHS2Int(d->nSectors, d->trackPointer, d->sectorPointer);
}

uint * disk_getCHS(tDiskParm * d) {
	uint *vec = allocateMemory(getGlobalHeap(), 3 * sizeof (uint));
	*vec = d->nTracks;
	*(vec+1) = DISK_HEADS;
	*(vec+2) = d->nSectors;
	return vec;
}

uint disk_sectorCount(tDiskParm * d) {
	return (d->nTracks * d->nSectors);
}

uint disk_size(tDiskParm * d) {
	return (disk_sectorCount(d) * SECTOR_SIZE);
}

float disk_worstTime(tDiskParm * d) {
	return (d->trackDelay * d->nTracks) + (d->sectorDelay * (d->nSectors - 1));
}

float disk_averageTime(tDiskParm * d) {
	return (disk_worstTime(d) / 2);
}

uint disk_transferRate(tDiskParm * d) {
	return (1000 * SECTOR_SIZE / (uint) ceil (disk_averageTime(d)));
}

float search_getTime(tSearchParm * s) {
	return s->time;
}

char * search_getHistory(tSearchParm * s) {
	char aux[6];
	int x;
	char *p, *q;
	char *buf = allocateMemory(getGlobalHeap(), 6 * s->historySize);
	//memset(buf, '1', 6 * s->historySize);
	p=buf;
	for (x=0; (x < s->historySize) && (s->history[x]!=-1); x++) {
		_itoa_s(s->history[x],aux,6,10);
		memcpy(p, aux, strlen(aux));
		p = p + strlen(aux);
		*p=',';
		q=p;
		p++;
	}
	*q='\0';
	return buf;
}

