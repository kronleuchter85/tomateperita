/*
 * test4.c
 *
 *  Created on: 27/07/2011
 *      Author: gonzalo
 */


#include "linux-commons-logging.h"


	void testReplaceAll(){

		char * texto1 = "Las fichas son tarjetas de forma rectangular de diversos tamaños que se utilizan para registrar los datos extraídos de las fuentes bibliográficas, como libros, revistas y periódicos; y no bibliográficas que son objeto de estudio por la persona que las elabora. Contiene datos de identificación de las obras, conceptos, ideas, resúmenes, síntesis, utilizados como medios para realizar un trabajo de investigación. Son aquellas fichas que utilizamos El fichero: Un fichero o archivo es un sistema real o virtual de organización de la información mediante una clasificación determinada. La ficha bibliográfica es una ficha pequeña, destinada a anotar meramente los datos de un libro o articulo. Estas fichas se hacen para todos los libros o artículos que eventualmente pueden ser útiles a nuestra investigación, no solo para los que se han encontrado físicamente o leído. En ellas se registran las fuentes encontradas, por ejemplo, en el catalogo de una biblioteca, en una bibliografía, en índices de publicaciones, etc.";

		printf("%s\n" , texto1);

		texto1 = replaceAll(texto1 , " " , "{%S%}");
		texto1 = replaceAll(texto1 , "á" , "{%AC%}");

		printf("%s\n" , texto1);

		texto1 = replaceAll(texto1 , "{%S%}" , " ");
		texto1 = replaceAll(texto1 , "{%AC%}" , "á" );

		printf("%s\n" , texto1);

	}



	void testTddRecords(){

		setLoggingLevelEnabled(LOGGING_LEVEL_DEBUG);
		initializeTdd();


		addTddRecord(buildTddRecord("archivo" , "vda" , "a"));
		addTddRecord(buildTddRecord("archivo" , "vda" , "a"));
		addTddRecord(buildTddRecord("archivo" , "vda" , "a"));
		addTddRecord(buildTddRecord("archivo" , "vda" , "a"));

		listNodes(getTdd());


	}
