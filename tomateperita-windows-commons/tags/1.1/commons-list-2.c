/*
 * linux-commons-list.c
 *
 *	Esta lista tiene accesos sincronizados para
 *	agregar y eliminar elementos.
 *
 *  Created on: 02/06/2011
 *      Author: gonzalo
 */


#include "commons-list-2.h"


	List createList(
			void (*aListingCriteria)(void *) ,
			BOOL (*aEqualityCriteria)(void * , void *) ,
			BOOL (*aSortingCriteria)(void * , void *) , HeapHandler hh){

		

		List myList = allocateMemory(hh , sizeof(L));
		myList->size = 0;
		myList->elements = NULL;

		myList->listingCriteria = aListingCriteria;
		myList->equalityCriteria = aEqualityCriteria;
		myList->sortingCriteria = aSortingCriteria;
		return myList;
	}

	Node * createNode(void * data , HeapHandler hh){
		Node * node = allocateMemory(hh , sizeof(Node));
		node->data = data;
		node->next = NULL;
		return node;
	}

	void addNode(List l ,void * data , HeapHandler hh){

		Node * n = createNode(data ,hh);

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
	}

	void listNodes(List aList){
		Node * aux = aList->elements;
		while(aux != NULL){
			aList->listingCriteria(aux->data);
			aux = aux->next;
		}
	}

	void removeNode(List aList,	void * data , HeapHandler hh){		

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

			freeMemory(hh , act);
			aList->size--;
		}	
	}

	Iterator * buildIterator(List list , HeapHandler hh){
		Iterator * iterator;
		if(list == NULL)
			return NULL;

		iterator = (Iterator *)allocateMemory(hh , sizeof(Iterator));
		iterator->elements = list->elements;
		return iterator;
	}

	BOOL hasMoreElements(Iterator * i){
		if(i == NULL)
			return FALSE;
		return (i->elements != NULL);
	}

	void * next(Iterator * i ){
		void * data;
		if( i == NULL || i->elements == NULL)
			return NULL;

		data = i->elements->data;
		i->elements = i->elements->next;
		return data;
	}

	void listingStrings(char * c){}

	BOOL equalingStringsInList(char * c , char * d){
		return equalsStrings(c , d);
	}
	BOOL sortingStringsInList(char * c , char * d){
		return TRUE;
	}

	List buildSringsList(HeapHandler hh){
		return createList(
				(void (*)(void *)) &listingStrings ,
				(BOOL (*)(void *, void *)) &equalingStringsInList ,
				(BOOL (*)(void *, void *)) &sortingStringsInList , 
				hh);
	}


	List buildStringsListWithValues(HeapHandler hh ,int count , ...){

		List list = buildSringsList(hh);
		va_list ap;
		int i; 
		va_start(ap, count);
		for(i=0 ; i<count ; i++){
			char * currentElement = va_arg(ap, char*);
			if(currentElement != NULL)
				addNode(list , currentElement , hh);
		}
		va_end(ap);

		return list;
	}

	char * formatListToPreetyString(List l , HeapHandler hh){
		char * formatedList = serializeList(l , ", " , hh);
		if(formatedList == NULL)
			return "(no hay elementos)";
		else
			return formatedList;
	}

	char * serializeList(List list , char * separator , HeapHandler hh){
		Iterator * ite = buildIterator(list , hh);
		char * serialization = NULL;
		while(hasMoreElements(ite)){
			char * element = next(ite);
			if(serialization == NULL){
				serialization = strdup(element);
			}else{
				serialization = concat( hh , 3 , serialization , separator , strdup(element));
			}
		}
		return serialization;
	}

	List tokenize(char * tokens , char separator , HeapHandler hh){
		
		List list;
		int mark1=0;
		int i;
		char * token;

		if(tokens == NULL)
			return NULL;

		list = buildSringsList(hh);
		
		for(i=0 ; i<strlen(tokens) ; i++){

			if(tokens[i] == separator){
				token = subString(tokens + mark1 , 0 , i-mark1-1 , hh);
				mark1 = i+1;

				if(!equalsStrings(trim(token , hh) , "" ))
					addNode(list , token , hh);
			}

		}
		if(i==strlen(tokens)){
			token = subString(tokens + mark1 ,0, strlen(tokens) - mark1 -1 , hh);

			if(!equalsStrings(trim(token) , "" ))
				addNode(list , token,  hh);
		}

		return (list->size > 0) ? list : NULL;
	}
	List deserializeList(char * listInString , HeapHandler hh){
		return tokenize(listInString , ',' , hh);
	}

