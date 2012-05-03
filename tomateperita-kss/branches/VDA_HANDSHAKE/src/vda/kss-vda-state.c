/*
 * kss-state-vda.c
 *
 *  Created on: 19/07/2011
 *      Author: gonzalo
 */
#include <stdlib.h>

#include "linux-commons-list.h"
#include "linux-commons-socket.h"
#include "kss-vda-state.h"
#include "kss-utils.h"

	/*
	 * Vdas...
	 */
	List connectedVdas;
	List mountedVdas;


	void createConnectedVdas(){
		connectedVdas = createVdasList();
	}

	List getConnectedVdas(){
		return connectedVdas;
	}

	void addConnectedVda(VdaComponent * c){
		if(c != NULL)
			addNode(connectedVdas , c);
	}
	void removeConnectedVda(VdaComponent * c){
		if(c != NULL)
			removeNode(connectedVdas , c);
	}

	Bool hasAvailableVdas(){
		return getListSize(getConnectedVdas()) > 0;
	}



	void createMountedVdas(){
		mountedVdas = createVdasList();
	}

	List getMountedVdas(){
		return mountedVdas;
	}

	void addMountedVda(VdaComponent * c){
		if(c != NULL)
			addNode(mountedVdas , c);
	}

	void removeMountedVda(VdaComponent * c){
		if(c != NULL)
			removeNode(mountedVdas , c);
	}


	VdaComponent * buildVdaComponent(char * vdaName , ListenSocket listenSocket){

		if(vdaName == NULL)
			return NULL;

		VdaComponent * vda = malloc(sizeof(VdaComponent));
		vda->vdaName = vdaName;
		vda->listenSocket = listenSocket;
		return vda;
	}


	VdaComponent * getVdaFromList(List list , char * vdaName){
		Bool selectionCriteria(VdaComponent * c){
			return (equalsStrings(c->vdaName , vdaName));
		}
		return getNodeByCriteria(list , (Bool (*) (void*))selectionCriteria);
	}

	VdaComponent * getMountedVda(char * vdaName){
		return getVdaFromList(mountedVdas , vdaName);
	}

	VdaComponent * getConnectedVda(char * vdaName){
		return getVdaFromList(connectedVdas , vdaName);
	}


	List getVdasMountedNames(){
		List vdaNames = buildSringsList();
		Iterator * ite = buildIterator(mountedVdas);
		while(hasMoreElements(ite)){
			VdaComponent * comp = next(ite);
			addNode(vdaNames , comp->vdaName);
		}
		return (vdaNames->size > 0) ? vdaNames : NULL;
	}


	VdaInformation * buildVdaInformation(char * vdaName , List values){
		Iterator * ite = buildIterator(values);
		char * tracks = hasMoreElements(ite) ? next(ite) : NULL;
		char * heads = hasMoreElements(ite) ? next(ite) : NULL;
		char * sectors = hasMoreElements(ite) ? next(ite) : NULL;

		if(tracks == NULL || heads == NULL || sectors == NULL)
			return NULL;

		VdaInformation * vdaInfo = malloc(sizeof(VdaInformation));
		vdaInfo->headCounts = atoi(heads);
		vdaInfo->sectorsCount = atoi(sectors);
		vdaInfo->tracksCount = atoi(tracks);
		vdaInfo->vdaName = vdaName;

		return vdaInfo;
	}

	Bool isReconnectedVda(VdaComponent * c){
		if ( containsNode( getConnectedVdas() , c) ){
			return TRUE;
		}
		return FALSE;
	}

	void overwriteConnectedVda(VdaComponent * c){
		removeNode( getConnectedVdas() , getConnectedVda(c->vdaName) );
		addNode( getConnectedVdas() , c )	;
	}
