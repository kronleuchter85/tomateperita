


#include "windows-commons-mps.h"
#include "windows-commons-errors.h"
#include "windows-commons-socket.h"

	void executeOperationGetSectors(MpsMessage * req , ListenSocket ls , RuntimeErrorValidator * v);
	void executeOperationPutSectors(MpsMessage * req , ListenSocket ls , RuntimeErrorValidator * v);
	void executeOperationGetChs(MpsMessage * req , ListenSocket ls , RuntimeErrorValidator * v);