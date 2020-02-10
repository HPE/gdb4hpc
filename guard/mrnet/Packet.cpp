/*********************************************************************************\
 * <Packet.cpp>
 *
 * © 2011-2013,2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#include "mrnet/Packet.h"

#include "MRNet_type_wrapper.h"

using namespace MRN;

extern "C" int packet_unpack(MRNet_Packet* packet, const char* ifmt, ...) {
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

extern "C" void packet_delete(MRNet_Packet* packet) {
    PacketPtr* pack = reinterpret_cast<PacketPtr*>(packet);

    if (pack != NULL) delete pack;
}
