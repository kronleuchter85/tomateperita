/**
 *  Planificacion de disco - Algoritmo SCAN
 */

#ifndef _W_DISK_FUNC_H_
#define _W_DISK_FUNC_H_
 
#include "vda-commons.h"

#define DISK_HEADS 1
#define DISK_TRACKS 10
#define TRACK_SECTORS 11
#define TRACK_DELAY 5
#define RPM 6900

	typedef struct {
			uint nTracks;
			uint nSectors;			/*cantidad de sectores por track*/
			float trackDelay;       /*tiempo (ms) insumido en cambiar de track*/
			float sectorDelay;      /*tiempo (ms) insumido en pasar sobre un sector*/
			uint sectorPointer;
			uint trackPointer;
			uint sectorInc;
			int up;                 /*1:ascendente 0:descendente*/
			} tDiskParm;

	typedef struct {
			int * buf;    /*historial de sectores leidos*/
			int * ini;
			int * end;
			int size;
			} tHistory;

	typedef struct {
			tHistory * history;
			uint sTrack;			/*numero de Track del sector a encontrar*/
			uint sSector;			/*numero de Sector dentro del Track*/
			float time;             /*tiempo total de la busqueda*/
			} tSearchParm;

	void diskParmInit (tDiskParm * d, uint numTracks, uint numSectors, uint rpm, float delay);
	void searchParmDestroy (tSearchParm * s);
	void scanSector(tDiskParm * d, tSearchParm * s, tSectorId givenSector);

	/* Getters para calculos de simulacion de la busqueda */
	float search_getTime(tSearchParm * s);
	char * search_getHistory(tSearchParm * s); /* Reserva memoria */

	/* Getters para calculos con parametros fijos del disco */
	tSectorId disk_lastSectorId(tDiskParm * d);
	uint disk_getHeadPos(tDiskParm * d);
	char * disk_getHeadPos_s(tDiskParm * d); /* Reserva memoria */
	infoCHS * disk_getCHS(tDiskParm * d); /* Reserva memoria, devuelve un struct de 3 enteros (C,H,S) */
	uint disk_size(tDiskParm * d);
	uint disk_transferRate(tDiskParm * d); /* En bytes/segundo */
	float disk_worstTime(tDiskParm * d);
	float disk_averageTime(tDiskParm * d);


#endif