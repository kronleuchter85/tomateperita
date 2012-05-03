/**
 *  Soporte del VDA - Operaciones sobre la Berkeley DB
 */

#include <db.h>
#include <string.h>
#include "vda-commons.h"

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

int vdadisk_write (const char *vda_name, LogicalSector *sector) {
	DB *base;
	DBT key, data;
	int ret;

	if ((ret = db_create(&base, NULL, 0)) != 0)
		return ret;
	/* Associate DB with a file (create a btree)*/
	if ((ret = base->open(base, NULL, vda_name, NULL, DB_BTREE, DB_CREATE, 0)) != 0) {
		//base->err(base, ret, "DB open failed\n");
		base->close(base,0);
		return ret;
	}
	loadDBT (&key, &data, sector);
	ret = base->put(base, NULL, &key, &data, DB_NOOVERWRITE);
	if (ret == DB_KEYEXIST) {
		base->err(base, ret, "La clave %d ya existe!\n");
		if ((ret = base->del(base, NULL, &key, 0)) == 0 ) /* Si existe lo borro y lo vuelvo a escribir */
			ret = base->put(base, NULL, &key, &data, DB_NOOVERWRITE);
	}
	base->close(base,0);
	return ret;
}

int vdadisk_read (const char *vda_name, LogicalSector *sector, tSectorId idToSearch) {
	DB *base;
	DBT key, data;
	int ret;

	if ((ret = db_create(&base, NULL, 0)) != 0)
		return ret;
	/* Associate DB with a file (create a btree)*/
	if ((ret = base->open(base, NULL, vda_name, NULL, DB_BTREE, DB_CREATE, 0)) != 0) {
		base->err(base, ret, "DB open failed\n");
		base->close(base,0);
		return ret;
	}
	sector_Build(sector, idToSearch, NULL);
	loadDBT (&key, &data, sector);
	if((ret = base->get(base, NULL, &key, &data, 0)) != 0)
			base->err(base, ret, "Not found.");
	base->close(base,0);
	return ret;
}