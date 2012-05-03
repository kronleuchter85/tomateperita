/*
 * tests3.c
 *
 *  Created on: 22/07/2011
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


	void testTokenizingPathFiles(){
		char * pathFIle = "/VDA1/archivo de texto 1.txt/";
		List tokes = tokenize(pathFIle , '/');
		Iterator * ite = buildIterator(tokes);
		char * vda = hasMoreElements(ite) ? next(ite) : NULL;
		char * file = hasMoreElements(ite) ? next(ite) : NULL;

		printf("vda name: %s\n" , vda);
		printf("file name: %s\n" , file);

		pathFIle = "/VDA1/undir/archivo de texto 1.txt";
		tokes = tokenize(pathFIle , '/');
		ite = buildIterator(tokes);
		vda = hasMoreElements(ite) ? next(ite) : NULL;
		char * dir = hasMoreElements(ite) ? next(ite) : NULL;
		file = hasMoreElements(ite) ? next(ite) : NULL;

		printf("vda name: %s\n" , vda);
		printf("vda name: %s\n" , dir);
		printf("file name: %s\n" , file);

	}



	void testRandomKey(){

		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
		printf("key: %s\n" , generateRandomKey(16));
	}



	void testLists(){

		printf("******************************************************************************\n");
		void listing(int * a){
			printf("%i\n" , *a);
		}

		Bool order(int * a , int * b){
			return *a < *b;
		}

		Bool eq(int * a , int * b){
			return *a == *b;
		}

		List list = createList(listing , eq , order);

		listNodes(list);

		addNode(list , newInteger(12));
		addNode(list , newInteger(1));
		addNode(list , newInteger(15));
		addNode(list , newInteger(11));
		addNode(list , newInteger(19));
		addNode(list , newInteger(14));
		addNode(list , newInteger(34));
		addNode(list , newInteger(24));
		addNode(list , newInteger(0));

		listNodes(list);

	}


	void testSearchingFiles(){

		printf("******************************************************************************\n");
		List files = getFileSectors("/home/gonzalo/tomateperita-data/VDA1/archivo1.txt");
		Iterator * filesIte = buildIterator(files);
		while(hasMoreElements(filesIte)){
			printf("%s\n" , next(filesIte));
		}

		printf("******************************************************************************\n");
		files = getDirectoryFiles("/home/gonzalo/tomateperita-data/VDA1/archivo1.txt" , NULL , ALWAYS_LAST_ORDER);
		filesIte = buildIterator(files);
		while(hasMoreElements(filesIte)){
			printf("%s\n" , next(filesIte));
		}

		printf("******************************************************************************\n");
		files = getDirectoryFiles("/home/gonzalo/tomateperita-data/VDA1/archivo1.txt" , NULL , ALWAYS_FIRST_ORDER);
		filesIte = buildIterator(files);
		while(hasMoreElements(filesIte)){
			printf("%s\n" , next(filesIte));
		}


	}
