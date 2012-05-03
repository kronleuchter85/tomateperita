/*
 *
 *  file-config.h
 *
 *
 */
/*
 char * getClientIp(void);
 int getClientPort(void);
 int getLoggingLevel(void);
 */
#include "linux-commons.h"
#include "linux-commons-list.h"

#ifndef _FILE_INFORMATION_
#define _FILE_INFORMATION_

typedef struct {

		long size;
		List sectors;
		char * name;
		char * vda;

	} FileInformation;

	typedef struct __attribute__ ((packed)) {
		int sector;
		char data[512];
		int dataLength;
	} FileSector;

	typedef struct __attribute__ ((packed)) {
		FileSector sector1;
		FileSector sector2;
		int fileHandleid;
	} FileBlock;

#endif

	FileInformation * buildFileInformation(char * vdaName , char * fileName);
	char * getKey(char * line);
	char * getValue(char * line);
	void loadConfiguration(File * file , void (*processKeysAndValues)(char * , char * , void *));

	void parseKeyAndValueFile(File * file ,
				Bool (*anIgnoreLineCriteria)(char *),
				void (*processKeysAndValuesCriteria)(char * , char *, void* ),
				void * storageObject);

	void parseFileAndModifyValueForKey(File * file , char * aKey , char * newValue ,
				Bool (*anIgnoreLineCriteria)(char *),
				void (*modifyKeysAndValuesCriteria)(char * , char * , void * , File * ),
				void * storageObject);

	void updateEntry(char * key , char * value ,
				void (*modifyKeysAndValuesCriteria)(char * , char * , void * , File * ) ,
				File * file);

	Bool existsFile(char * name);
	Bool createDirectory(char * name);

	File * openFile(const char * );
	File * openOrCreateFile(const char * filename);
	File * createOrTruncateFile(const char * filepath);

	Bool removeDirectory(char * dirname);
	void removeFile(const char * filename);
	void closeFile(File * file);

	void insertEntry(char * key , char * value , File * file);
	void insertLine( const char * line , File * file);

	List getDirectoryFiles(char * path ,
			Bool (*anIncludeCriteria)(char *), Bool (*sortingCriteria)(char * , char *));

	char * getStartNameMatchingFile(char * path , char * pattern);



	void cleanBlock(FileBlock * block);
	FileBlock * buildEmptyFileBlock();
	void setFileBlockSectors(FileBlock * block, int s1 , int s2);
	void setFileBlockContent(FileBlock * block , char * buffer);
	void setFileBlockContents(FileBlock * block , char * content1 , char * content2 , int length1 , int length2);

	Bool hasTwoSectors(FileBlock * fileBlock);

