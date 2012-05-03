
#include "list-commons.h"

	
	/**
	 * Crea una lista vacia, o sea en null...
	 */
	SortedList createSortedList(){
		return NULL;
	}

	/**
	 * elimina una lista elemento por elemento.
	 * si la lista esta vacia no hace nada.
	 * recibe una funcion como argumento que es invocada por orden
	 * superior para darle polimorfismo a la eliminacion del contenido 
	 * del ListNode.
	 */
	void deleteSortedList(SortedList * l ,  HeapHandler (*gethandlerfunction)(void*)
		,  void (*deletingfunction)(void*) ){
		
		ListNode * prev = NULL;
		while( *l != NULL){
			prev = *l;
			*l = (*l)->next;

			if(deletingfunction != NULL){
				deletingfunction(prev->content);
			}		
			if( gethandlerfunction(prev->content) != NULL )
				freeMemory( gethandlerfunction(prev->content) , prev);
		}
	}

	/**
	 * Agrega un elemento a la lista.
	 * Si existe un elemento en la lista con el 
	 * mismo indice igual se agrega otro con el mismo
	 * indice pero con el nuevo contenido.
	 */
	void addElement(SortedList* l , ListNode * n){	
		ListNode * aux = *l;
		if(aux == NULL){
			*l = n;
		}
		else{
			while(aux->next != NULL && aux->index < n->index){
				aux = aux->next;
			}
			n->next = aux->next;
			aux->next = n;
		}
	}

	/**
	 * Elimina un elemento de la lista dado el indice por 
	 * el cual se encuentra ordenado. En caso de que no existe
	 * un elemento en la lista con el indice en cuestion no se 
	 * efectua ninguna tarea, la lista queda inalterada.
	 * La funcion pasada como argumento es invocada por orden superior para 
	 * darle polimorfismo a la la eliminacion del contenido del nodo de ListNode.
	 */
	void removeElement(SortedList * list , unsigned int index 
		, HeapHandler hh , void (*deletingfunction)(void*)){

		ListNode * aux = *list;
		ListNode * prev = NULL;
		while(aux != NULL && aux->index < index){
			prev = aux;
			aux = aux->next;
		}
		if(aux != NULL && aux->index == index){

			if(prev != NULL){
				prev->next = aux->next;
			}else{
				*list = aux->next;
			}

			if(deletingfunction != NULL){
				deletingfunction(aux->content);
			}

			if(hh != NULL){
				freeMemory(hh , aux);
				destroyHeap(hh);
			}
		}
	}

	/**
	 * Crea un nuevo ListNode para la lista con los indice y valor pasados
	 * como parametros.
	 * Devuelve un puntero a dicho nodo.
	 */
	ListNode* createListNode(HeapHandler hh , unsigned int index , void * value){	
		ListNode * n = allocateMemory(hh , sizeof(ListNode));
		n->index = index;
		n->content = value;
		n->next = NULL;
		return n;
	}

	/**
	 * Imprime el contenido de cada uno de los elementos de la lista
	 * utilizando una invocacion por orden superior a la funcion
	 * pasada como argumento con cada nodo.
	 */
	void printSortedList(SortedList list , void (*printfunction)(void*)){
		ListNode * aux = list;
		while(aux!= NULL){
			printfunction(aux->content);
			aux = aux->next;
		}
	}