/*
 * tests2.c
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

	void testGetCurrentSize(){
		RuntimeErrorValidator * v = buildErrorSuccessValidator();

		long s1 = getCurrentFileSize("VDA1" , "archivo1.txt" , v);
		info(concatAll(2 , "FileSize: " , ltoa(s1)));

		long s2 = getCurrentFileSize("VDA1" , "archivo2.txt" , v);
		info(concatAll(2 , "FileSize: " , ltoa(s2)));

		long s3 = getCurrentFileSize("VDA1" , "archivo3.txt" , v);
		info(concatAll(2 , "FileSize: " , ltoa(s3)));

		long s4 = getCurrentFileSize("VDA1" , "archivo4.txt" , v);
		info(concatAll(2 , "FileSize: " , ltoa(s4)));

	}



	void testListImplementation(){


		Bool equling(char * f1 , char * f2){
			return !strcmp(f1 , f2);
		}

		Bool sorting(char * f1 , char * f2){
			return ( strcmp(f1 , f2) < 0 );
		}

		void listing(char * e){
			if(e != NULL) logInfo( "linux-commons" , e);
		}

		List list = createList(listing , equling , sorting);

		addNode(list , "hola1");
		addNode(list , "hola2");
		addNode(list , "hola3");
		addNode(list , "hola4");
		addNode(list , "hola5");
		addNode(list , "hola6");

		logInfo("linux-commons" , concatAll(2 , "list size: " , itoa(list->size)));

		//una forma chota de listar los elementos de la lista
		listNodes(list);

		//otra forma de listar elementos mas prolija y con mas
		//control sobre el proceso de listado

		Iterator * iterator = buildIterator(list);

		while(hasMoreElements(iterator)){
			char * element = next(iterator);
			logInfo("linux-commons" , concatAll(2 , "element: " , element));
		}
	}


	void testListImplementation2(){


		Bool equling(int * f1 , int * f2){
			return *f1 == *f2;
		}

		Bool sorting(int * f1 , int * f2){
			return *f1<*f2;
		}

		void listing(int * e){
			if(e != NULL) logInfo( "linux-commons" , itoa(*e));
		}

		List list = createList(listing , equling , sorting);

		addNode(list , newInteger(1));
		addNode(list , newInteger(2));
		addNode(list , newInteger(3));
		addNode(list , newInteger(4));
		addNode(list , newInteger(5));
		addNode(list , newInteger(6));

		logInfo("linux-commons" , concatAll(2 , "list size: " , itoa(list->size)));

		listNodes(list);

		//otra forma de listar elementos

		Iterator * iterator = buildIterator(list);

		while(hasMoreElements(iterator)){
			int * element = next(iterator);
			logInfo("linux-commons" , concatAll(2 , "element: " , itoa(*element)));
		}
	}



	void testListingFiles(){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		List l = getAllFiles("VDA1", validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}

		Iterator * ite = buildIterator(l);

		char * element = NULL;
		while( hasMoreElements(ite)){
			element = next(ite);
			info( concatAll(4, "Archivo: " , element , " con tamaño: " , ltoa(getCurrentFileSize("VDA1" , element , validator))));
		}

	}

	void testFreeingSectors(){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		Bool aSortingCriteria(int * i , int * x){
			return (*i < *x);
		}

		Bool equalityCriteria(int * a , int * b){
			return (*a == *b);
		}

		List lista = createList(NULL ,
				(Bool (*)(void* , void*))equalityCriteria,
				(Bool (*)(void* , void*))aSortingCriteria);

		addNode(lista , newInteger(18));
		addNode(lista , newInteger(20));
		addNode(lista , newInteger(2));
		addNode(lista , newInteger(11));
		addNode(lista , newInteger(13));
		addNode(lista , newInteger(1));
		addNode(lista , newInteger(17));

		freeSectors("VDA1" , lista , validator);

		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
	}

	void testDeleting(){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		deleteFile("VDA1" , "archivo1.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
		deleteFile("VDA1" , "archivo2.txt" , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}


	}


	void testFormat(){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		format("VDA1" , 40 , validator);
		if(hasError(validator)){
			error(validator->errorDescription);
			return ;
		}
	}
