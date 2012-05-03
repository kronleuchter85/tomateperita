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

#include "linux-commons-list.h"


	List createList(
			void (*aListingCriteria)(void *) ,
			BOOL (*aEqualityCriteria)(void * , void *) ,
			BOOL (*aSortingCriteria)(void * , void *)){

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
			l->size++;
		}

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


