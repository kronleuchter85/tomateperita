/*
 * test-ftp-1.c
 *
 *  Created on: 24/07/2011
 *      Author: gonzalo
 */


#include <stdlib.h>

#include "linux-commons.h"
#include "linux-commons-mps.h"
#include "linux-commons-list.h"
#include "linux-commons-file.h"
#include "linux-commons-errors.h"
#include "linux-commons-logging.h"

#include "kss-core-services.h"
#include "kss-fss-endpoint.h"
#include "kss-tdd.h"
#include "kss-utils.h"
#include "kss-vda-endpoint.h"
#include "kss-tdd.h"
#include "kss-ftps-entrypoint-core.h"
#include "kss-vda-state.h"




	char * content1 = "Un texto es una composición de signos codificado en un sistema de escritura (como un alfabeto) que forma una unidad de sentido. Su tamaño puede ser variable.\nTambién es texto una composición de caracteres imprimibles (con grafema) generados por un algoritmo de cifrado que, aunque no tienen sentido para cualquier persona, si puede ser descifrado por su destinatario original.";
	char * content2 = "En otras palabras un texto es un entramado de signos con una intención comunicativa que adquiere sentido en determinado contexto.\nDe los textos se pueden extraer ideas esenciales, a las que llamaremos \"macroproposición\".\nTambién es un conjunto de oraciones agrupadas en párrafos que habla de un tema determinado.\n¿Cómo extraer de un texto una idea esencial? Hay varios pasos.";
	char * filePath = "/VDA1/archivo1.txt";


	void testOpenFileForWrite(){



		RuntimeErrorValidator * v = buildErrorSuccessValidator();


		/*
		 * Se abre solo para crear el archivo y ver si funciona
		 */
		char * fhId = coreOperationOpenFile(filePath , "1" , v);

		listNodes(getTdd());

		coreOperationCloseFile(fhId , v);

		listNodes(getTdd());

		/*
		 * Se vuelve a abrir, pero esta vez para agregar contenido
		 */
		fhId = coreOperationOpenFile(filePath , "1" , v);

		coreOperationWriteFile(fhId , content1 , v);

		coreOperationCloseFile(fhId , v);

		listNodes(getTdd());

	}


	void testOpenFileForRead(){
		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		char * fhId = coreOperationOpenFile(filePath , "0" , v);

		listNodes(getTdd());

		FileBlock * block = coreOperationReadFile(fhId , "-1" , v );

		printf("leyendo: %s%s\n" , block->sector1.data , block->sector2.data);

		coreOperationCloseFile(fhId , v);
	}



