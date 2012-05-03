/*
 * kss-ftps-entrypoint-core.h
 *
 *  Created on: 24/07/2011
 *      Author: gonzalo
 */

#ifndef KSS_FTPS_ENTRYPOINT_CORE_H_
#define KSS_FTPS_ENTRYPOINT_CORE_H_


#define KSS_FTP_ENTRYPOINT_READ_FILE_NO_PREVIOUS_SEARCH_INDEX				"-1"



	char * coreOperationOpenFile(char * path ,
			char * openingMode, RuntimeErrorValidator * validator);

	void coreOperationCloseFile(char * fhId , RuntimeErrorValidator * validator);

	char * coreOperationListFiles(char * pattern , RuntimeErrorValidator * validator);

	void coreOperationWriteFile(char * fileId , char * contentBlock , RuntimeErrorValidator * validator);

	FileBlock * coreOperationReadFile(char * fileId , char * lastSectorSended , RuntimeErrorValidator * validator);

#endif /* KSS_FTPS_ENTRYPOINT_CORE_H_ */
