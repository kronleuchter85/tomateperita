/**
 *  API para comunicacion con KSS y Shell
 */
#include <stdio.h>

#include "windows-commons-logging.h"
#include "windows-commons.h"

#include "vda-api.h"
#include "vda-commons.h"
#include "vda-state.h"
#include "vda-db.h"
#include "vda-configuration.h"
#include "vda-disk-functions.h"
#include "vda-cache.h"

extern tDiskParm diskParameters;
extern cacheMem cache;

int putSectorOnDisk (LogicalSector * ls) {
	char logBuf[BUFFSIZE];
	tSearchParm search;
	uint headPos;
	int ret = 0;

	vdadisk_write (getDBFullPath(), ls);
	headPos = disk_getHeadPos(getDiskParms());
	/* Actualizo la cache si el sector esta en ella, sino hay bolonki despues al leer */
	if (cache_Search (getVdaCache(), NULL, ls->sectorId) == 1)
		cache_Write (getVdaCache(), ls);
	/* Realizo la simulacion */
	scanSector(getDiskParms(), &search, ls->sectorId);
	if(isInfoEnabled()) {
		sprintf_s(logBuf, BUFFSIZE,"[PUT] Posicion cabezal: %d, Tiempo consumido: %.2f, Sectores leidos: %s", headPos, search_getTime(&search), search_getHistory(&search));
		info(logBuf); 
		}
	searchParmDestroy (&search);
	return SECTOR_SIZE;
}

LogicalSector * getSectorFromCacheOrDisk1 (tSectorId sid) {
	char logBuf[BUFFSIZE];
	LogicalSector *ls;
	tSearchParm search;
	uint headPos;
	int ret = 0;

	ls = sector_BuildLogical(sid, NULL); /* Reserva memoria - usaba SECTOR_BLANK */
	if (isVdaAvailableCache())
			if ((ret = cache_Search (getVdaCache(), ls, sid)) == 1) 
					if(isInfoEnabled()) {
						sprintf_s(logBuf, BUFFSIZE,"[GET] Tiempo consumido: %.2f, Sectores leidos: %d (en cache), Cache Hit/Miss %d/%d", cache_getAccessTime(), sid, cache_getHitAverage(getVdaCache()), cache_getMissAverage(getVdaCache()));
						info(logBuf); 
					}
	if (ret != 1) {
				vdadisk_read (getDBFullPath(), ls, sid);
				/* Lo agrego a la cache para que este en la proxima lectura */
				if (isVdaAvailableCache())
					cache_Write (getVdaCache(), ls);
				headPos = disk_getHeadPos(getDiskParms());
				/* Realizo la simulacion */
				scanSector(getDiskParms(), &search, sid);
				if(isInfoEnabled()) {
					if (isVdaAvailableCache())
						sprintf_s(logBuf, BUFFSIZE,"[GET] Posicion cabezal: %d, Tiempo consumido: %.2f, Sectores leidos: %s, Cache Hit/Miss %d/%d", headPos, search_getTime(&search), search_getHistory(&search), cache_getHitAverage(getVdaCache()), cache_getMissAverage(getVdaCache()));
					else
						sprintf_s(logBuf, BUFFSIZE,"[GET] Posicion cabezal: %d, Tiempo consumido: %.2f, Sectores leidos: %s", headPos, search_getTime(&search), search_getHistory(&search));
						info(logBuf); 
					}
	}
	searchParmDestroy (&search);
	return ls;
}

void getSectorFromCacheOrDisk2 (tSectorId sid) {
	LogicalSector *ls;
	tSearchParm search;
	int ret = 0;
	
	ls = sector_BuildLogical(sid, NULL); /* Reserva memoria - usaba SECTOR_BLANK */
	if (isVdaAvailableCache()) {
			if ((ret = cache_Search (getVdaCache(), ls, sid)) == 1) {
				printf("Sector Leido: %d (en cache)\n", sid);
				printf("Tiempo consumido: %.2f ms\n", cache_getAccessTime());
				printf("Sector %d: %s\n\n", sid, sector_getFirstLastBytes(ls));
			}
	}
	if (ret != 1) {
				vdadisk_read (getDBFullPath(), ls, sid);
				printf("Posicion actual: %d\n", disk_getHeadPos(getDiskParms()));
				/* Realizo la simulacion */
				scanSector(getDiskParms(), &search, sid);
				printf("Sectores Leidos: %s\n", search_getHistory(&search));
				printf("Tiempo consumido: %.2f ms\n", search_getTime(&search));
				printf("Sector %d: %s\n\n", sid, sector_getFirstLastBytes(ls));
				/*Lo agrego a la cache para que este en la proxima lectura*/
				if (isVdaAvailableCache())
					cache_Write (getVdaCache(), ls);
	}
	myFree(ls);
	searchParmDestroy (&search);
}

LogicalSector * getOneSector (int sid) {
	LogicalSector *ls;
	if (sectorInvalido(sid)) {
		if (isDebugEnabled()) errorInvalidSector(sid);
		ls = sector_BuildLogical(sid, NULL); /* lo lleno de \0 */
	}
	else ls = getSectorFromCacheOrDisk1(sid);
	return ls;
}

int putOneSector(int sid, char * content) {
	int ret;
	LogicalSector *ls;
	if (content == NULL) /* sector que no quiero escribir y le puse NULL a proposito */
		return 0;
	if (sectorInvalido(sid)) {
		if (isDebugEnabled()) errorInvalidSector(sid);
		return 0;
		}
	ls = sector_BuildLogical (sid, content);
	ret = putSectorOnDisk(ls);
	myFree(ls);
	return ret;
}

/* 
Funciones para intercambio con KSS 
*/

GettingTransferSector * getSectores(int s1, int s2) {
	GettingTransferSector *gts;
	LogicalSector *ls1, *ls2;

	ls1 = getOneSector(s1);
	ls2 = getOneSector(s2);
	gts = sector_BuildGetting(ls1, ls2);
	myFree(ls1);
	myFree(ls2);
	return gts;
}

int putSectores (int sector1, char * contenido1, int sector2, char * contenido2) {
	return putOneSector(sector1, contenido1) + putOneSector(sector2, contenido2);
}

infoCHS * getCHS(void) {
	return disk_getCHS(getDiskParms());
}

/* 
Funciones para el shell
*/

void obtenerSectores(tSectorId * slist, int argsCount) {
	int i;
	for	(i=0; i < argsCount; i++)
		if (sectorInvalido(slist[i])) {
			if (isDebugEnabled()) errorInvalidSector(slist[i]);
		}
		else getSectorFromCacheOrDisk2(slist[i]);
	myFree(slist);
}

void posicionCabezal(void) {
	printf ("Posicion actual del cabezal: %d\n", disk_getHeadPos(getDiskParms()));
}

void writeRandom(tSectorId sid) {
	LogicalSector *ls;
	char *content;
	if (sectorInvalido(sid)) {
			if (isDebugEnabled()) errorInvalidSector(sid);
			return;
		}
	content = randomBytes(SECTOR_SIZE , getGlobalHeap());
	ls = sector_BuildLogical(sid, content);
	putSectorOnDisk(ls);
	content[SECTOR_SIZE-1] = '\0';
	printf ("Sector escrito: %d\n", sid);
	printf ("Contenido: %s\n", content);
	myFree(ls);
}

void cacheStatus(void){
	if (isVdaAvailableCache()) {
		printf("Sectores presentes en cache: %s\n", cache_getSectorIds(getVdaCache()));
		if (!cache_notFull(getVdaCache()))
			printf("Candidato a ser reemplazado: %d\n", cache_getOlderSectorId(getVdaCache()));
	}
	else printf("Cache Desactivada");
}

void showInfo(void) {
	char logBuf[BUFFSIZE];
	if (isInfoEnabled()) {
		sprintf_s(logBuf, BUFFSIZE, "[SHOW] Tiempo medio de busqueda: %.2f ", disk_averageTime(getDiskParms()));
		info(logBuf); 
		sprintf_s(logBuf, BUFFSIZE, "[SHOW] Tiempo peor de busqueda: %.2f ", disk_worstTime(getDiskParms()));
		info(logBuf);
		info(concat(getGlobalHeap(), 2,"[SHOW] Tasa de transferencia: ", itoa_buf(disk_transferRate(getDiskParms()))));
		if (isVdaAvailableCache()) {
			sprintf_s(logBuf, BUFFSIZE, "[SHOW] Tiempo medio de acceso a la cache: %.2f", cache_getAccessTime());
			info(logBuf);
		}
		else info("[SHOW] Cache desactivada");
		sprintf_s(logBuf, BUFFSIZE, "[SHOW] Rango valido de sectores [0..%d]", disk_lastSectorId(getDiskParms()));
		info(logBuf);
	}
}

void help(void) {
	printf("\nComandos:");
	printf("\n\tobtenerSectores([Lista de no mas de 5 sectores separados por coma])");
	printf("\n\tposicionCabezal");
	printf("\n\n\tshowInfo");
	printf("\n\tcacheStatus");
	printf("\n\twriteRandom([Nro de sector])");
	printf("\n\n\tdemoGetSectores");
	printf("\n\tdemoPutSectores");
	printf("\n\tdemoGetCHS");
	printf("\n\n\tquit\n");
}


/* Ejemplos de llamado a la API de intercambmio con el KSS - invocables desde Shell */

void demoGetSectores(void){
	GettingTransferSector * gts;
	int numSectorSolicitado1;
	int numSectorSolicitado2;

	/* Si probamos con numero de sector -1 o fuera de rango valido, devuelve el contenido del invalido lleno de \0 */
	numSectorSolicitado1=0;
	numSectorSolicitado2=1;
	gts = getSectores(numSectorSolicitado1, numSectorSolicitado2);
	gts->sectorContent1[511]='\0'; /* Los termino como string para poder imprimirlos aca, pero no deberia tocarlos */
	gts->sectorContent2[511]='\0';
	printf ("\n\tContenido sector %d: %s",   numSectorSolicitado1, gts->sectorContent1);
	printf ("\n\tContenido sector %d: %s\n", numSectorSolicitado2, gts->sectorContent2);
	myFree(gts);
}

void demoPutSectores(void){
	char * contenido1 = "Un texto es una composición de signos codificado en un sistema de escritura (como un alfabeto) que forma una unidad de sentido. Su tamaño puede ser variable.\nTambién es texto una composición de caracteres imprimibles (con grafema) generados por un algoritmo de cifrado que, aunque no tienen sentido para cualquier persona, si puede ser descifrado por su destinatario original.";
	char * contenido2 = "En otras palabras un texto es un entramado de signos con una intención comunicativa que adquiere sentido en determinado contexto.\nDe los textos se pueden extraer ideas esenciales, a las que llamaremos \"macroproposición\".\nTambién es un conjunto de oraciones agrupadas en párrafos que habla de un tema determinado.\n¿Cómo extraer de un texto una idea esencial? Hay varios pasos.";
	int c1 = strlen(contenido1);
	int c2 = strlen(contenido2);
	/* Si probamos con numero de sector -1 o fuera de rango valido, solo escribe el valido */
	//printf("\n\t%d Bytes escritos.", putSectores(11, "Reconcholis!!??\0", 15, "Probando persistencia y eliminacion de basuritas yyy\0"));
	putSectores(0 , contenido1 , 1 , contenido2);
	printf("\n\t%d Bytes escritos.", putSectores(0, contenido1, 1, contenido2));
}

void demoGetCHS(void){
	infoCHS * aux = getCHS();
	printf ("\n\tCilinders: %d", aux->cilinders);
	printf ("\n\tHeads:     %d", aux->heads);
	printf ("\n\tSectors:   %d\n", aux->sectors);
	myFree(aux);
}