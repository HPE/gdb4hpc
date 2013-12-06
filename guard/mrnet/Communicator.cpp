#include "mrnet/Communicator.h"
#include "MRNet_type_wrapper.h"

using namespace MRN;

extern "C" int communicator_get_endpoints_size(MRNet_Communicator* communicator)
{
	Communicator *pCommunicator = (Communicator*) communicator;
	return pCommunicator->get_EndPoints().size();
}
