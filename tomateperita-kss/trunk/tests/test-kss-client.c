/*
 * test-kss-client.c
 *
 *  Created on: Jul 27, 2011
 *      Author: Fernando
 */

#include <stdlib.h>

#include "linux-commons.h"
#include "linux-commons-mps.h"
#include "linux-commons-list.h"
#include "linux-commons-file.h"
#include "linux-commons-errors.h"
#include "linux-commons-logging.h"

#include "kss-tdd.h"
/*#include "
#include "
#include "
#include "*/


	void populateAndAddTddRecords(){

		TddRecord * tddRec1 = buildTddRecord( "archivo1" , "vda_a" , "1" );
		tddRec1->fileSectors = NULL;
		tddRec1->fileSize = 1024;

		TddRecord * tddRec2 = buildTddRecord( "archivo2" , "vda_b" , "0" );
		tddRec2->fileSectors = NULL;
		tddRec2->fileSize = 2048;

		TddRecord * tddRec3 = buildTddRecord( "archivo3" , "vda_c" , "2" );
		tddRec3->fileSectors = NULL;
		tddRec3->fileSize = 3072;

		List sectores = buildStringsListWithValues( 3 , "8" , "13" , "21");
		TddRecord * tddRec4 = buildTddRecordWithSectors( "archivo4" , "vda_d" , "1",sectores);
		tddRec4->fileSize = 4096;

		addTddRecord( tddRec1 );
		addTddRecord( tddRec2 );
		addTddRecord( tddRec3 );
		addTddRecord( tddRec4 );
	}
