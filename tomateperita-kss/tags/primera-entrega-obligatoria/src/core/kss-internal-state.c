/*
 * kss-internal-state.c
 *
 *  Created on: 31/05/2011
 *      Author: gonzalo
 */
#include <unistd.h>
#include <stdlib.h>

#include "linux-commons-socket.h"
#include "linux-commons-list.h"
#include "kss-vda-component.h"
#include "kss-state.h"

#define SERVER_CONNECTION_INDEX_VDA		0
#define SERVER_CONNECTION_INDEX_FTPS	1
#define SERVER_CONNECTION_INDEX_SHELL	2


	ServerSocket *serverConnections[3];

	ListenSocket fssComponentConnection;

	List vdas;


	void setFssComponentConnection(ListenSocket c){
		if(c > 0){
			fssComponentConnection = c;
			setAvailableFss(TRUE);
		}else {
			setAvailableFss(FALSE);
		}
	}

	void initializeVdas(){
		vdas = createList(
				(void (*)(void *))&vdaListingCriteria ,
				(BOOL (*)(void *, void *))&vdaEqualityCriteria ,
				(BOOL (*)(void *, void *)) &vdaSortingCriteria);
	}

	void addVdaComponent(VdaComponent * c){
		if(c != NULL){
			addNode(vdas , c);
			incrementAvaialbleVdasCount();
		}
	}

	void setServerConnection(ServerSocket * s , int index){
		serverConnections[index] = s;
	}

	void setVdaServerConnection(ServerSocket * s){
		setServerConnection(s , SERVER_CONNECTION_INDEX_VDA);
	}
	void setFtpsServerConnection(ServerSocket * s){
		setServerConnection(s , SERVER_CONNECTION_INDEX_FTPS);
	}
	void setShellServerConnection(ServerSocket * s){
		setServerConnection(s , SERVER_CONNECTION_INDEX_SHELL);
	}

	void closeAndFreeServerConnection(ServerSocket * s){
		close(s->listenSocket);
		free(s);
	}

	void closeVdaServerConnection(){
		closeAndFreeServerConnection(serverConnections[SERVER_CONNECTION_INDEX_VDA]);
	}
	void closeFtpsServerConnection(){
		closeAndFreeServerConnection(serverConnections[SERVER_CONNECTION_INDEX_FTPS]);
	}
	void closeShellServerConnection(){
		closeAndFreeServerConnection(serverConnections[SERVER_CONNECTION_INDEX_SHELL]);
	}

	void closeServerConnections(){
		closeVdaServerConnection();
		closeFtpsServerConnection();
		closeShellServerConnection();
	}
