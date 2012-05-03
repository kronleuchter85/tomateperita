
#include "vda-cache.h"
#include "vda-commons.h"
#include <windows.h>

void cache_Create (cacheMem * ch) {
   ch->endPointer = NULL;
   ch->hit = 0;
   ch->miss = 0;
}

int cache_notFull (cacheMem * ch) {
	return (ch->endPointer < (CACHE_SIZE -1 + ch->myCache));
}

int cache_isEmpty (cacheMem * ch) {
	return (ch->endPointer == NULL);
}

void cache_updateObjectTime (cacheObject * co) {
	GetSystemTimeAsFileTime(&(co->timestamp));
}

void cache_hit (cacheMem * ch) {
	ch->hit = ch->hit + 1;
}

void cache_miss (cacheMem * ch) {
	ch->miss = ch->miss + 1;
}

/* Retorna el elemento de cache mas antiguo, candidato a ser reemplazado */
cacheObject * cache_getOlder (cacheMem * ch) {
   FILETIME aux;
   cacheObject *p, *older;

   if (cache_isEmpty(ch)) return NULL;
   p = ch->myCache;
   aux.dwLowDateTime = (p->timestamp).dwLowDateTime;
   older = p;
   while (p < ch->endPointer) {
	   *p++;
	   if (aux.dwLowDateTime > (p->timestamp).dwLowDateTime) {
		   aux.dwLowDateTime = (p->timestamp).dwLowDateTime; 
		   older = p;
	   }
	}
   return older;
}

cacheObject * cache_findSector (cacheMem * ch, tSectorId sectorId) {
   cacheObject *p;

   if (cache_isEmpty(ch)) return NULL;
   p = ch->myCache;;
   while (p < ch->endPointer) {
	   if (sectorId == (p->sector).sectorId)
		   return p;
	   *p++;
   }
   return NULL;
}

/* Escribe un sector en una posicion de la Cache dada por un puntero */
void cache_replaceObject (cacheObject * co, LogicalSector * ls) {
	cache_updateObjectTime(co);
	sector_Copy(&(co->sector), ls);
}

/* Agrega un sector a la Cache en su ultima posicion. No valida si existe */
void cache_addNew (cacheMem * ch, LogicalSector * ls) {
	if (cache_isEmpty(ch)) {
		ch->endPointer = ch->myCache;
		cache_replaceObject (ch->endPointer, ls);
	}
	else if (cache_notFull(ch)) {
			(ch->endPointer)++;
			cache_replaceObject (ch->endPointer, ls);
	}
}

/* Agrega o actualiza un sector en la cache */
void cache_Write (cacheMem * ch, LogicalSector * ls){
	cacheObject *p;

	p = cache_findSector (ch, ls->sectorId);
	if (p == NULL) {
		if(cache_notFull(ch)) 
			cache_addNew(ch, ls);
		else 
			cache_replaceObject(cache_getOlder(ch), ls);
		}
	else 
		cache_replaceObject(p, ls);
		//cache_updateObjectTime(p);
}

/* Dado un numero de sector, devuelve 0 si no esta en la cache. No modifica el parametro de intercambio ls */
/* Devuelve 1 si esta en la cache, completa ls con la info del sector y actualiza su timestamp en cache */
int cache_Search (cacheMem * ch, LogicalSector * ls, tSectorId sectorId) {
	cacheObject * p;

	p = cache_findSector (ch, sectorId);
	if (p == NULL) {
		cache_miss(ch);
		return 0;
	}
	sector_Copy(ls, &(p->sector));
	cache_updateObjectTime(p);
	cache_hit(ch);
	return 1;
}

int cache_getHitAverage(cacheMem * ch) {
	return (ch->hit * 100) / (ch->hit + ch->miss);
}

int cache_getMissAverage(cacheMem * ch) {
	return (ch->miss * 100) / (ch->hit + ch->miss);
}

float cache_getAccessTime (void) {
	return CACHE_ACCESS_TIME;
}