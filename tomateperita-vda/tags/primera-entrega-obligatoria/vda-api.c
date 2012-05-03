/**
 *  API para comunicacion con KSS y Shell
 */

#include "vda-api.h"
#include "vda-commons.h"
#include "vda-state.h"
#include "vda-db.h"
#include "vda-configuration.h"
#include "vda-disk-functions.h"
#include "vda-cache.h"
#include <stdio.h>

extern tDiskParm diskParameters;
extern cacheMem cache;


LogicalSector * getSectorFromCacheOrDisk (tSectorId s) {
	LogicalSector *ls;
	
	ls = sector_BuildLogical(s, NULL);
	if (cache_Search (&cache, ls, s) == 1)
		return ls;
	vdadisk_read (getVdaName(), ls, s);
	return ls;
}

GettingTransferSector * getSectores(tSectorId s1, tSectorId s2) {
	GettingTransferSector *gts;
	LogicalSector *ls1, *ls2;

	ls1 = getSectorFromCacheOrDisk(s1);
	ls2 = getSectorFromCacheOrDisk(s1);

	/*
		aca falta la parte de simulacion y logging
	*/

	gts = sector_BuildGetting(ls1, ls2);
	freeMemory(getGlobalHeap(), ls1);
	freeMemory(getGlobalHeap(), ls2);
	return gts;
}

void obtenerSectores(tSectorList * slist) {
	LogicalSector *ls;
	tSearchParm search;
	tSectorId sid;

	sid = 0;
	while (sid != -1) {
		sid = sectorList_getItem(slist);
		ls = sector_BuildLogical(sid, NULL); /* Reserva memoria */
		if (cache_Search (&cache, ls, sid) == 1) {
			printf("Sectores Leidos: %d - En cache\n", sid);
			printf("Tiempo consumido: %.2f ms\n", cache_getAccessTime());
			printf("Sector %d: %s\n\n", sid, sector_getFirstLastBytes(ls));
		}
		else {
			vdadisk_read (getVdaName(), ls, sid);
			scanSector(&diskParameters, &search, sid);
			printf("Posicion actual: %d\n", disk_getHeadPos(&diskParameters));
			printf("Sectores Leidos: %s\n", search_getHistory(&search));
			printf("Tiempo consumido: %.2f ms\n\n", search_getTime(&search));
			printf("Sector %d: %s\n\n", sid, sector_getFirstLastBytes(ls));
		}
	}
	freeMemory(getGlobalHeap(), ls);
}

void posicionCabezal(tDiskParm *d) {
	printf ("Posicion actual del cabezal: sector %d \n", disk_getHeadPos(d));
}

