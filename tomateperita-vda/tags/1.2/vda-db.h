/**
 *  Soporte del VDA - Operaciones sobre la Berkeley DB
 */

#ifndef _W_VDA_DB_H_
#define _W_VDA_DB_H_

#include <db.h>

int vdadisk_write (const char *db_name, LogicalSector *sector);
int vdadisk_read (const char *db_name, LogicalSector *sector, tSectorId idToSearch);

#endif