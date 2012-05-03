/*
 * linux-commons-list.c
 *
 *	Esta lista tiene accesos sincronizados para
 *	agregar y eliminar elementos.
 *
 *  Created on: 02/06/2011
 *      Author: gonzalo
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <strings.h>

#include "linux-commons-list.h"


	List createList(
			void (*aListingCriteria)(void *) ,
			Bool (*aEqualityCriteria)(void * , void *) ,
			Bool (*aSortingCriteria)(void * , void *)){

		ThreadMutex m = PTHREAD_MUTEX_INITIALIZER;

		List myList = malloc(sizeof(L));
		myList->size = 0;
		myList->elements = NULL;
		myList->mutex = m;
		myList->listingCriteria = aListingCriteria;
		myList->equalityCriteria = aEqualityCriteria;
		myList->sortingCriteria = aSortingCriteria;
		return myList;
	}

	Node * createNode(void * data){
		Node * node = malloc(sizeof(Node));
		node->data = data;
		node->next = NULL;
		return node;
	}

	void addNode(List l ,void * data){

		pthread_mutex_lock(&(l->mutex));

		Node * n = createNode(data);

		if(l->elements == NULL){
			l->elements = n;
		}else{
			Node * aux = l->elements;
			while(aux->next != NULL
					&& l->sortingCriteria(aux->data , n->data)){
				aux = aux->next;
			}

			if( aux->next != NULL){
				n->next = aux->next;
			}

			aux->next = n;
		}

		l->size++;

		pthread_mutex_unlock(&(l->mutex));
	}

	void listNodes(List aList){
		Node * aux = aList->elements;
		while(aux != NULL){
			aList->listingCriteria(aux->data);
			aux = aux->next;
		}
	}

	void removeNode(List aList,	void * data){

		pthread_mutex_lock(&(aList->mutex));

		Node * prev = NULL;
		Node * act = aList->elements;

		while(act != NULL && !aList->equalityCriteria(act->data , data)){
			prev = act;
			act = act->next;
		}

		if(act != NULL){
			if(prev != NULL){
				prev->next = act->next;
			}else{
				aList->elements = act->next;
			}

			free(act);
			aList->size--;
		}

		pthread_mutex_unlock(&(aList->mutex));
	}

	Bool containsNode(List list, void * element ){
		Iterator * ite = buildIterator(list);
		while(hasMoreElements(ite)){
			void * elem = next(ite);
			if(list->equalityCriteria(elem , element)){
				return TRUE;
			}
		}
		return FALSE;
	}

	void * getNode(List list , Bool (*selectionCriteria)(void *)){
		Iterator * ite = buildIterator(list);
		while(hasMoreElements(ite)){
			void * elem = next(ite);
			if(selectionCriteria(elem)){
				return elem;
			}
		}
		return NULL;
	}


	Iterator * buildIterator(List list){
		if(list == NULL)
			return NULL;

		Iterator * iterator = (Iterator *)malloc(sizeof(Iterator));
		iterator->elements = list->elements;
		return iterator;
	}
	Bool hasMoreElements(Iterator * i){
		if(i == NULL)
			return FALSE;
		return (i->elements != NULL);
	}
	void * next(Iterator * i ){

		if( i == NULL || i->elements == NULL)
			return NULL;

		void * data = i->elements->data;
		i->elements = i->elements->next;
		return data;
	}


	List buildSringsList(){

		void listing(char * c){}
		Bool equal(char * c , char * d){
			return equalsStrings(c , d);
		}
		Bool sorting(char * c , char * d){
			return TRUE;
		}

		return createList(
				(void (*)(void *)) listing ,
				(Bool (*)(void *, void *)) equal ,
				(Bool (*)(void *, void *)) sorting);
	}


	List buildStringsListWithValues(int count , ...){

		List list = buildSringsList();

		va_list ap;
		va_start(ap, count);
		int i; for(i=0 ; i<count ; i++){
			char * currentElement = va_arg(ap, char*);
			if(currentElement != NULL)
				addNode(list , currentElement );
		}
		va_end(ap);

		return list;
	}

	char * formatListToPreetyString(List l){
		char * formatedList = serializeList(l , ", ");
		if(formatedList == NULL)
			return "(no hay elementos)";
		else
			return formatedList;
	}

	char * serializeList(List list , char * separator){
		Iterator * ite = buildIterator(list);
		char * serialization = NULL;
		while(hasMoreElements(ite)){
			char * element = next(ite);
			if(serialization == NULL){
				serialization = strdup(element);
			}else{
				serialization = concatAll(3 , serialization , separator , strdup(element));
			}
		}
		return serialization;
	}

	List tokenize(char * tokens , char separator){

		if(tokens == NULL)
			return NULL;

		List list = buildSringsList();

		int mark1=0;
		int i;
		char * token;
		for(i=0 ; i<strlen(tokens) ; i++){

			if(tokens[i] == separator){
				token = strndup(tokens + mark1 , i-mark1);
				mark1 = i+1;

				if(!equalsStrings(trim(token) , "" ))
					addNode(list , token);
			}

		}
		if(i==strlen(tokens)){
			token = strndup(tokens + mark1 , strlen(tokens) - mark1);

			if(!equalsStrings(trim(token) , "" ))
				addNode(list , token);
		}

		return (list->size > 0) ? list : NULL;
	}
	List deserializeList(char * listInString ){
		return tokenize(listInString , ',');
	}


	int getListSize(List l){
		if(l == NULL)
			return -1;
		else return l->size;
	}



