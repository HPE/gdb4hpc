/*********************************************************************************\
 * <Aggregation.cpp>
 *
 * © 2012-2013,2020 Cray Inc.
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

#include <stdlib.h>
#include <string.h>

#include <vector>

#include "dmr.h"
#include "mrnet/NetworkTopology.h"
#include "mrnet/Packet.h"
#include "mrnet/Types.h"
#include "smart_bitset.h"

using namespace DMR; /*Debugging Message Reduction */
using namespace MRN; /* MRnet */

extern "C" {

const char* DbgMessageAggregation_format_string = "%as";
void DbgMessageAggregation(std::vector<PacketPtr>& packets_in,
                           std::vector<PacketPtr>& packets_out,
                           std::vector<PacketPtr>& /* packets_out_reverse */,
                           void** /* client data */, PacketPtr& /* params */,
                           const TopologyLocalInfo& localInfo) {
    PriorityQueue<MessageVector, MessageComparison> merger;
    MessageVector reductionMVector(false);
    MessageVector** inputMVector;

    char** inputArr;
    int inputLen;
    unsigned int i;
    unsigned int in_size = packets_in.size();

    if (in_size == 0) return;

    inputMVector = new MessageVector*[in_size];
    for (i = 0; i < in_size; ++i) {
        PacketPtr cur_packet = packets_in[i];
        cur_packet->unpack("%as", &inputArr, &inputLen);

        inputMVector[i] = new MessageVector(true);
        inputMVector[i]->Initialization(inputArr, inputLen);

        free(inputArr);
    }

    merger.Initialization(in_size);
    for (i = 0; i < in_size; ++i) merger.Insert(inputMVector[i]);

    MessageVector* pTop = NULL;
    MessageVector::iterator it;
    while (merger.Size() > 0) {
        pTop = merger.Top();
        it = pTop->Current();
        reductionMVector.Append(*it);

        if (!pTop->MoveNext())
            merger.Pop();
        else
            merger.Adjust();
    }

    char** reductionArr = NULL;
    Message* pMessage = NULL;
    i = 0;
    reductionArr = (char**)malloc(sizeof(char*) * reductionMVector.size() * 2);
    for (it = reductionMVector.begin(); it < reductionMVector.end(); ++it) {
        pMessage = *it;
        pMessage->SetReleaseMemory(false);
        reductionArr[i] = pMessage->GetContent();
        reductionArr[i + 1] = pMessage->GetBitsetStr();

        i += 2;
    }

    int reduction_size = reductionMVector.size() * 2;

    PacketPtr new_packet(new Packet(packets_in[0]->get_StreamId(),
                                    packets_in[0]->get_Tag(), "%as",
                                    reductionArr, reduction_size));

    new_packet->set_DestroyData(true);
    packets_out.push_back(new_packet);

    for (i = 0; i < in_size; ++i) delete inputMVector[i];

    delete[] inputMVector;
}
} /* extern "C" */
