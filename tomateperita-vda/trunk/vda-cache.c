#include "windows-commons-logging.h"


#include "vda-cache.h"
#include "vda-state.h"
#include "vda-commons.h"


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
   p = ch->myCache;
   while (p <= ch->endPointer) {
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
	char logBuf[BUFFSIZE];

	p = cache_findSector (ch, ls->sectorId);
	if (p == NULL) {
		if(cache_notFull(ch)) {
			if(isDebugEnabled()) debug(concat(getGlobalHeap(), 2, "[CACHE] Alta sector ",itoa_buf(ls->sectorId)));
			cache_addNew(ch, ls);
		}
		else {
			p = cache_getOlder(ch);
			if(isDebugEnabled()) {
				sprintf_s(logBuf, BUFFSIZE, "[CACHE] Reemplazo sector %d por %d", p->sector.sectorId, ls->sectorId);
				debug(logBuf);
				//debug(concat(getGlobalHeap(), 2, "[CACHE] Baja sector ",itoa_buf(p->sector.sectorId)));
			 	//debug(concat(getGlobalHeap(), 2, "[CACHE] Alta sector (reemplazo) ",itoa_buf(ls->sectorId)));
				}
			cache_replaceObject(cache_getOlder(ch), ls);
			}
		}
	else {  
		if(isDebugEnabled()) debug(concat(getGlobalHeap(), 2, "[CACHE] Uptdate data sector ",itoa_buf(ls->sectorId)));
		cache_replaceObject(p, ls);
		}

}

/* Dado un numero de sector, devuelve 0 si no esta en la cache. No modifica el parametro de intercambio ls */
/* Devuelve 1 si esta en la cache, completa ls con la info del sector y actualiza su timestamp en cache */
int cache_Search (cacheMem * ch, LogicalSector * ls, tSectorId sectorId) {
	cacheObject * p;

	p = cache_findSector (ch, sectorId);
	if (p == NULL) {
		if (ls != NULL) {
			if(isDebugEnabled()) debug(concat(getGlobalHeap(), 2, "[CACHE] Miss sector ",itoa_buf(ls->sectorId)));
			cache_miss(ch);
		}
		return 0;
	}
	if (ls != NULL) {
		if(isDebugEnabled()) debug(concat(getGlobalHeap(), 2, "[CACHE] Hit - Update time sector ",itoa_buf(ls->sectorId)));
		sector_Copy(ls, &(p->sector));
		cache_updateObjectTime(p);
		cache_hit(ch);
	}
	return 1;
}

int cache_getHitAverage(cacheMem * ch) {
	return ((ch->hit + ch->miss)==0) ? 0:(ch->hit * 100) / (ch->hit + ch->miss);
}

int cache_getMissAverage(cacheMem * ch) {
	return ((ch->hit + ch->miss)==0) ? 0:(ch->miss * 100) / (ch->hit + ch->miss);
}

float cache_getAccessTime (void) {
	return (float) CACHE_ACCESS_TIME;
}

char *cache_getSectorIds(cacheMem * ch) {
	char *str;
	cacheObject *p;
	char aux[SECTOR_STRBUF];

	if (cache_isEmpty(ch)) return "Vacio";
	str = myAlloc(SECTOR_STRBUF * CACHE_SIZE);
	p = ch->myCache;
	while (p <= ch->endPointer) {
		_itoa_s(p->sector.sectorId, aux, SECTOR_STRBUF, 10);
		strcat_s(str, SECTOR_STRBUF * CACHE_SIZE, aux );
		strcat_s(str, SECTOR_STRBUF * CACHE_SIZE, ", ");
		p++;
	}
	if (strlen(str)>=2)
		*(str + (strlen(str)-2)) = '\0';
	return str;
}

int cache_getOlderSectorId(cacheMem * ch){
	cacheObject *p;
	if (cache_isEmpty(ch)) return -1;
	p = cache_getOlder (ch);
	return p->sector.sectorId;
}