#include "mrnet/Packet.h"
#include "MRNet_type_wrapper.h"

using namespace MRN;

extern "C" int packet_unpack(MRNet_Packet* packet, char *ifmt, ... )
{
//	fprintf(*err_stream_ptr, "[%d] entering packet_unpack()!\n", getpid());

	PacketPtr pack = *(PacketPtr*)packet;
 	va_list arg_list;
	va_start(arg_list, ifmt);
	// Needed to add the dummy argument.
	int res = pack->unpack(arg_list, ifmt, 0);
	va_end(arg_list);

//	fprintf(*err_stream_ptr, "[%d] leaving packet_unpack()!\n", getpid());
	return res;
}

extern "C" void packet_delete(MRNet_Packet* packet)
{
	PacketPtr * pack = reinterpret_cast< PacketPtr * >(packet);
	
	if(pack != NULL)
		delete pack;
}
