/*
 * test-vda-1.c
 *
 *  Created on: 16/07/2011
 *      Author: gonzalo
 */

#include "linux-commons-list.h"

#include "kss-vda-service.h"
#include "kss-vda-endpoint.h"


	char * contenido1 = "Un texto es una composición de signos codificado en un sistema de escritura (como un alfabeto) que forma una unidad de sentido. Su tamaño puede ser variable.\nTambién es texto una composición de caracteres imprimibles (con grafema) generados por un algoritmo de cifrado que, aunque no tienen sentido para cualquier persona, si puede ser descifrado por su destinatario original.";
	char * contenido2 = "En otras palabras un texto es un entramado de signos con una intención comunicativa que adquiere sentido en determinado contexto.\nDe los textos se pueden extraer ideas esenciales, a las que llamaremos \"macroproposición\".\nTambién es un conjunto de oraciones agrupadas en párrafos que habla de un tema determinado.\n¿Cómo extraer de un texto una idea esencial? Hay varios pasos.";
	int sect1 = 0;
	int sect2 = 1;

	void testCommunication(){
		Iterator * iteVdas = buildIterator(getConnectedVdas());
		VdaComponent * comp = hasMoreElements(iteVdas)?next(iteVdas) : NULL;
		RuntimeErrorValidator * valid = buildErrorSuccessValidator();

		if(strlen(contenido1)>= 512)
			contenido1 = strndup(contenido1 , 512);

		if(strlen(contenido2)>= 512)
			contenido2 = strndup(contenido2 , 512);

		callVdaOperationPutSectores(comp , sect1 , contenido1 , sect2 , contenido2 , valid);

		List sectors = callVdaOperationGetSectores(comp , sect1, sect2, valid);

		iteVdas = buildIterator(sectors);

		char * c1 = hasMoreElements(iteVdas)?next(iteVdas) : NULL;
		char * c2 = hasMoreElements(iteVdas)?next(iteVdas) : NULL;

		info(concatAll(2 , "Contenido del sector1: " , c1));
		info(concatAll(2, "Contenido del sector1: " , c2));

	}
