#ifndef _GUARD_MRNET_PACKET_H
#define _GUARD_MRNET_PACKET_H

#include "MRNet_C.h"

int packet_unpack(MRNet_Packet* packet, char *ifmt, ... );
void packet_delete(MRNet_Packet* packet);

#endif /* _GUARD_MRNET_PACKET_H */
