/**
 *  API para comunicacion con KSS y Shell
 */

#ifndef _W_VDA_API_H_
#define _W_VDA_API_H_

#include "vda-disk-functions.h"

//extern tDiskParm diskParameters;
//extern cacheMem cache;

GettingTransferSector * getSectores(tSectorId s1, tSectorId s2);
void posicionCabezal(tDiskParm *d);
void obtenerSectores(tSectorList * slist);

#endif