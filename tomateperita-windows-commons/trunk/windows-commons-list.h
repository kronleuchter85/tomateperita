/*
 * linux-commons-list.h
 *
 *  Created on: 02/06/2011
 *      Author: gonzalo
 */

#include "windows-commons-memory.h"
#include "windows-commons-headers.h"

#ifndef LINUX_COMMONS_LIST_H_
#define LINUX_COMMONS_LIST_H_

	struct N{
		void * data;
		void * next;
	};

	typedef struct N Node;

	typedef struct {

		/*
		 * Campos...
		 */	
		Node * elements;
		int size;

		/*
		 * Funciones
		 */
		BOOL (*sortingCriteria)(void * , void*);
		BOOL (*equalityCriteria)(void * , void*);
		void (*listingCriteria)(void *);

	}L;

	typedef struct {
		Node * elements;
	} Iterator;

	typedef L * List;


	List createList(void (*aListingCriteria)(void *) ,
			BOOL (*aEqualityCriteria)(void * , void *) ,
			BOOL (*aSortingCriteria)(void * , void *), HeapHandler);
	
	Node * createNode(void * data, HeapHandler);
	void addNode(List l ,void * n, HeapHandler);
	void listNodes(List aList);
	void removeNode(List  aList,void * aNode, HeapHandler);

	Iterator * buildIterator(List, HeapHandler);
	BOOL hasMoreElements(Iterator *);
	void * next(Iterator* );

	List buildSringsList( HeapHandler);
	List buildStringsListWithValues( HeapHandler , int n , ...);

	char * listToString(List l);

	char * formatListToPreetyString(List l, HeapHandler);
	char * serializeList(List list , char * separator, HeapHandler);

	List deserializeList(char * listInString , HeapHandler);
	List tokenize(char * listInString , char separator, HeapHandler);

#endif /* LINUX_COMMONS_LIST_H_ */
