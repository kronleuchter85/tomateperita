/**
 *  Cache stuff - Algoritmo LRU
 */

#ifndef _W_VDA_CACHE_H_
#define _W_VDA_CACHE_H_

#include "windows-commons.h"

#include "vda-commons.h"


#define CACHE_SIZE 10
#define CACHE_ACCESS_TIME 0.1

	typedef struct {
	  LogicalSector sector;
	  FILETIME timestamp;
	} cacheObject;

	typedef struct {
	  cacheObject myCache[CACHE_SIZE];
	  cacheObject * endPointer;
	  tCounter hit;
	  tCounter miss;
	} cacheMem;

	void cache_Create (cacheMem * cache);

	/* Agrega o actualiza un sector en la cache */
	void cache_Write (cacheMem * ch, LogicalSector * ls);

	/* Dado un numero de sector, devuelve 0 si no esta en la cache. No modifica el parametro de intercambio ls */
	/* Devuelve 1 si esta en la cache, completa ls con la info del sector y actualiza su timestamp en cache */
	int  cache_Search (cacheMem * ch, LogicalSector * ls, tSectorId sectorId);

	/* Getters para calculos relacionados con la cache */
	int cache_getHitAverage(cacheMem * ch);
	int cache_getMissAverage(cacheMem * ch); 
	float cache_getAccessTime (void);
	char *cache_getSectorIds(cacheMem * ch); /* devuelve string con la lista de sectores en cache */

	int cache_getOlderSectorId(cacheMem * ch); /* devuelve -1 si cache esta vacia */
	int cache_notFull (cacheMem * ch);

#endif
