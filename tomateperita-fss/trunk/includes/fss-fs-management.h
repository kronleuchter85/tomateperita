/*
 * fss-management.h
 *
 *  Created on: 23/06/2011
 *      Author: gonzalo
 */

#include "linux-commons-list.h"
#include "linux-commons-errors.h"
#include "linux-commons-file.h"

#ifndef FSS_MANAGEMENT_H_
#define FSS_MANAGEMENT_H_

/**
 * Constantes que definen los keys de las entradas
 * del archivo de indices
 */

#define FSS_FILE_NAME_SIZE						"size"

#define SECTOR_SIZE								512
#define FILE_NAME_SECTOR						"-sector-"
#define	FILE_NAME_FREE_SECTORS					"_free-sectors"
#define FILE_NAME_SIZE							"size-"

	Bool isExistingFile(char * vdaName , char * fileName );
	void deleteFile(char * vdaName , char* fileName , RuntimeErrorValidator * validator);
	void createFile(char * vdaName , char * fileName, RuntimeErrorValidator * validator);
	void updateFileSize(char * vdaName , char * fileName , long fileSize, RuntimeErrorValidator * validator);
	FileInformation * getFileInformation(char * vdaName, char * fileName , RuntimeErrorValidator * validator);
	void createFreeBitVector(char * vdaName , int sectorsCount, RuntimeErrorValidator * validator);
	int getFreeSector(char * vdaName , RuntimeErrorValidator * validator);
	void addSectorsToFile(char * vdaName , char * fileName , int sector1 , int sector2, RuntimeErrorValidator * validator);
	void freeSectors(char * vdaName, List sectors, RuntimeErrorValidator * validator);
	List getAllFiles(char * vdaName, RuntimeErrorValidator * validator);
	long getCurrentFileSize(char * vdaName , char * fileName , RuntimeErrorValidator * v);

	Bool hasFormatedStorage(char * vda);
	void format(char * vdaName , int sectorsCount, RuntimeErrorValidator * validator);
	char * getAbsolutePathToFile(char * vdaName , char * fileName);
#endif /* FSS_MANAGEMENT_H_ */
