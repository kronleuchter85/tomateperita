/*
 * tests.c
 *
 *  Created on: 07/07/2011
 *      Author: gonzalo
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "linux-commons.h"
#include "linux-commons-logging.h"
#include "linux-commons-file.h"
#include "linux-commons-errors.h"
#include "linux-commons-list.h"

#include "fss-configuration.h"
#include "fss-state.h"
#include "fss-utils.h"
#include "fss-fs-management.h"


	void testCreate(){

		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		createFile("VDA1" , "archivo1.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}

		createFile("VDA1" , "archivo2.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		createFile("VDA1" , "archivo3.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		createFile("VDA1" , "archivo4.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		createFile("VDA1" , "archivo5.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		createFile("VDA1" , "archivo6.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		createFile("VDA1" , "archivo7.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		createFile("VDA1" , ".archivo7.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		createFile("VDA1" , ".archivo8.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		createFile("VDA1" , ".archivo9.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		createFile("VDA1" , ".archivo10.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		createFile("VDA1" , "archivo con espacios.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}

		createFile("VDA1" , "archivo con espacios2.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
	}

	void testUpdateSize(){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		updateFileSize("VDA1" , "archivo2.txt" , 512 , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		updateFileSize("VDA1" , "archivo1.txt" , 1024 , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		updateFileSize("VDA1" , "archivo3.txt" , 512 , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		updateFileSize("VDA1" , "archivo4.txt" , 1024 , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		updateFileSize("VDA1" , "archivo5.txt" , 512 , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		updateFileSize("VDA1" , "archivo6.txt" , 1024 , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		updateFileSize("VDA1" , "archivo7.txt" , 512 , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		updateFileSize("VDA1" , ".archivo8.txt" , 1024 , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		updateFileSize("VDA1" , ".archivo7.txt" , 512 , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		updateFileSize("VDA1" , ".archivo8.txt" , 512 , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
	}

	void logInformation(FileInformation * f){
		info(concatAll(2 , "FileName: " , f->name));
		info(concatAll(2 , "FileSize: " , ltoa(f->size)));
	}

	void testGettingFileInformation(){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		FileInformation * f = getFileInformation("VDA1" , "archivo2.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		logInformation(f);
		f = getFileInformation("VDA1" , "archivo3.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		logInformation(f);
		f = getFileInformation("VDA1" , "archivo4.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		logInformation(f);
		f = getFileInformation("VDA1" , "archivo5.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		logInformation(f);
		f = getFileInformation("VDA1" , ".archivo10.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		logInformation(f);
		f = getFileInformation("VDA1" , ".archivo7.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		logInformation(f);
		f = getFileInformation("VDA1" , ".archivo8.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		logInformation(f);
		f = getFileInformation("VDA1" , ".archivo9.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		logInformation(f);
	}

	void testCreatingBitVector(){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		createFreeBitVector("VDA1" , 40 , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
	}

	void testAddingSectors(){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		addSectorsToFile("VDA1" , "archivo1.txt" ,getFreeSector("VDA1" , validator) ,getFreeSector("VDA1" , validator) , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		addSectorsToFile("VDA1" , "archivo2.txt" ,getFreeSector("VDA1" , validator) ,getFreeSector("VDA1" , validator) , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		addSectorsToFile("VDA1" , "archivo3.txt" ,getFreeSector("VDA1" , validator) ,getFreeSector("VDA1" , validator) , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		addSectorsToFile("VDA1" , "archivo4.txt" ,getFreeSector("VDA1" , validator) ,getFreeSector("VDA1" , validator) , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		addSectorsToFile("VDA1" , "archivo5.txt" ,getFreeSector("VDA1" , validator) ,getFreeSector("VDA1" , validator) , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		addSectorsToFile("VDA1" , "archivo6.txt" ,getFreeSector("VDA1" , validator) ,getFreeSector("VDA1" , validator) , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		addSectorsToFile("VDA1" , "archivo7.txt" ,getFreeSector("VDA1" , validator) ,getFreeSector("VDA1" , validator) , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		addSectorsToFile("VDA1" , ".archivo8.txt" ,getFreeSector("VDA1" , validator) ,getFreeSector("VDA1" , validator) , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		addSectorsToFile("VDA1" , ".archivo9.txt" ,getFreeSector("VDA1" , validator) ,getFreeSector("VDA1" , validator) , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		addSectorsToFile("VDA1" , ".archivo10.txt" ,getFreeSector("VDA1" , validator) ,getFreeSector("VDA1" , validator) , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		addSectorsToFile("VDA1" , "archivo1.txt" ,getFreeSector("VDA1" , validator) ,getFreeSector("VDA1" , validator) , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		addSectorsToFile("VDA1" , ".archivo7.txt" , -1,getFreeSector("VDA1" , validator) , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
	}
