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
		Bool (*sortingCriteria)(void * , void*);
		Bool (*equalityCriteria)(void * , void*);
		void (*listingCriteria)(void *);

	}L;

	typedef struct {
		Node * elements;
	} Iterator;

	typedef L * List;


	List createList(void (*aListingCriteria)(void *) ,
			Bool (*aEqualityCriteria)(void * , void *) ,
			Bool (*aSortingCriteria)(void * , void *));
	Node * createNode(void * data);
	void addNode(List l ,void * n);
	void listNodes(List aList);
	void removeNode(List  aList,void * aNode);

	Iterator * buildIterator(List);
	Bool hasMoreElements(Iterator *);
	void * next(Iterator* );

	List buildSringsList();
	List buildStringsListWithValues(int n , ...);

	char * listToString(List l);

	char * formatListToPreetyString(List l);
	char * serializeList(List list , char * separator);

	List deserializeList(char * listInString );
	List tokenize(char * listInString , char separator);

	int getListSize(List l);

	Bool containsNode(List list, void * element );
	void * getNodeByCriteria(List list , Bool (*selectionCriteria)(void *));


	Bool ALWAYS_FIRST_ORDER(void * , void *);
	Bool ALWAYS_LAST_ORDER(void * , void *);

#endif /* LINUX_COMMONS_LIST_H_ */
