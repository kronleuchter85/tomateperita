/*
 * kss-vda-state.h
 *
 *  Created on: 19/07/2011
 *      Author: gonzalo
 */

#ifndef KSS_VDA_STATE_H_
#define KSS_VDA_STATE_H_

	typedef struct {
		char * vdaName;
		int sectorsCount;
		int tracksCount;
		int headCounts;
	} VdaInformation;


	typedef struct {
		char * vdaName;
		ListenSocket listenSocket;
	}VdaComponent;

	VdaComponent * buildVdaComponent(char * vdaName , ListenSocket listenSocket);

	/**
	 * available Vdas
	 */
	Bool hasAvailableVdas();
	void createConnectedVdas();
	void addConnectedVda(VdaComponent * );
	List getConnectedVdas();
	void removeConnectedVda(VdaComponent * c);
	VdaComponent * getConnectedVda(char * vdaName);

	void createMountedVdas();
	List getMountedVdas();
	void addMountedVda(VdaComponent * c);
	void removeMountedVda(VdaComponent * c);
	VdaComponent * getMountedVda(char * vdaName);

	VdaComponent * getVdaFromList(List list , char * vdaName);
	List getVdasMountedNames();

	VdaInformation * buildVdaInformation(char * vdaName , List values);


#endif /* KSS_VDA_STATE_H_ */
