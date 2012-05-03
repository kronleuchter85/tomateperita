


#include "commons-mps.h"
#include "commons-errors.h"
#include "socket-commons.h"

	void executeOperationGetSectors(MpsRequest * req , ListenSocket ls , RuntimeErrorValidator * v);
	void executeOperationPutSectors(MpsRequest * req , ListenSocket ls , RuntimeErrorValidator * v);
	void executeOperationGetChs(MpsRequest * req , ListenSocket ls , RuntimeErrorValidator * v);