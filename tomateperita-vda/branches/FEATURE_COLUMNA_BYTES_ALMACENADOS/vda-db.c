/**
 *  Soporte del VDA - Operaciones sobre la Berkeley DB
 */

#include <db.h>
#include <string.h>

#include "windows-commons-logging.h"

#include "vda-configuration.h"
#include "vda-commons.h"
#include "vda-state.h"

void loadDBT (DBT * key, DBT * data, LogicalSector *sector) {
	memset(key, 0, sizeof(DBT)); 
	memset(data, 0, sizeof(DBT)); 
	key->data = &(sector->sectorId);
	key->size = sizeof (tSectorId);
	data->data = sector->sectorContent;
	/* Solo necesario para escribir: */
	data->size = SECTOR_SIZE;
	/* Solo necesarios para leer: */
	data->ulen = SECTOR_SIZE;
	data->flags = DB_DBT_USERMEM;
}

int vdadisk_write (const char *db_name, LogicalSector *sector) {
	DB *base;
	DBT key, data;
	int ret;

	if ((ret = db_create(&base, NULL, 0)) != 0)
		return ret;
	/* Associate DB with a file (create a btree)*/
	//if ((ret = base->open(base, NULL,concat( getGlobalHeap(), 2, db_name, ".db"), NULL, DB_BTREE, DB_CREATE, 0)) != 0) {
	if ((ret = base->open(base, NULL, db_name, NULL, DB_BTREE, DB_CREATE, 0)) != 0) {
		if(isDebugEnabled()) error( "[BDB] Fallo al abrir la base" );
		//base->err(base, ret, "DB open failed\n");
		base->close(base,0);
		return ret;
	}
	loadDBT (&key, &data, sector);
	ret = base->put(base, NULL, &key, &data, DB_NOOVERWRITE);
	if (ret == DB_KEYEXIST) {
		//base->err(base, ret, "La clave %d ya existe!\n");*
		//_itoa_s(sector->sectorId, aux, 10, 10);
		if(isDebugEnabled()) debug(concat( getGlobalHeap() , 2 , "[BDB] Ya existe el sector ", itoa_buf(sector->sectorId)));
		if ((ret = base->del(base, NULL, &key, 0)) == 0 ){  /* Si existe lo borro y lo vuelvo a escribir */
			if(isDebugEnabled()) debug(concat( getGlobalHeap() , 2 , "[BDB] Reescribiendo el sector ", itoa_buf(sector->sectorId)));
			ret = base->put(base, NULL, &key, &data, DB_NOOVERWRITE);
		}
	}
	base->close(base,0);
	return ret;
}

int vdadisk_read (const char *db_name, LogicalSector *sector, tSectorId idToSearch) {
	DB *base;
	DBT key, data;
	int ret;

	if ((ret = db_create(&base, NULL, 0)) != 0)
		return ret;
	/* Associate DB with a file (create a btree)*/
	//if ((ret = base->open(base, NULL, concat( getGlobalHeap(), 2, db_name, ".db"), NULL, DB_BTREE, DB_CREATE, 0)) != 0) {
	if ((ret = base->open(base, NULL, db_name, NULL, DB_BTREE, DB_CREATE, 0)) != 0) {
		if(isDebugEnabled()) error( "[BDB] Fallo al abrir la base" );
		//base->err(base, ret, "DB open failed\n");
		base->close(base,0);
		return ret;
	}
	loadDBT (&key, &data, sector);
	ret = base->get(base, NULL, &key, &data, 0);
	if((ret = base->get(base, NULL, &key, &data, 0)) != 0) {
		if(isDebugEnabled()) 
			debug(concat( getGlobalHeap() , 2 , "[BDB] No se encuentra el sector " ,itoa_buf(sector->sectorId)));
		//base->err(base, ret, "Not found.");
	}
	base->close(base,0);
	return ret;
}