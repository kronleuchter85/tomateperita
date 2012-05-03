/*
 * linux-commons-list.h
 *
 *  Created on: 02/06/2011
 *      Author: gonzalo
 */
#include "linux-commons.h"


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
		pthread_mutex_t mutex;
		Node * elements;
		int size;

		/*
		 * Funciones
		 */
		BOOL (*sortingCriteria)(void * , void*);
		BOOL (*equalityCriteria)(void * , void*);
		void (*listingCriteria)(void *);

	}L;

	typedef L * List;


	List createList(void (*aListingCriteria)(void *) ,
			BOOL (*aEqualityCriteria)(void * , void *) ,
			BOOL (*aSortingCriteria)(void * , void *));
	Node * createNode(void * data);
	void addNode(List l ,void * n);
	void listNodes(List aList);
	void removeNode(List  aList,void * aNode);


#endif /* LINUX_COMMONS_LIST_H_ */
