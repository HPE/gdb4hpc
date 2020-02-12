/*********************************************************************************\
 * <filter.cpp>
 *
 * Copyright 2012-2013,2020 Cray Inc.
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

#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <map>
#include <vector>

#include "bitset.h"
#include "dbg_sci.h"
#include "message.hpp"
#include "sci.h"

using namespace std;

// typedefs
typedef pair<SingleMsg*, bitset*> MsgBitsetPair;
typedef map<SingleMsg*, bitset*, MsgCompare> MsgBitsetMap;

extern "C" {

int filter_initialize(void** user_param);
int filter_terminate(void* user_param);
int filter_input(void* user_param, sci_group_t group, void* buf, int size);

} /* extern "C" */

bool isReady(int numOfBEs);
void freeMemory(MsgBitsetMap* pMsg);
int outputData(sci_group_t group, ConcatMsg& msg);

FILE* sci_filter_log = NULL;

int filter_initialize(void** user_param) {
    MsgBitsetMap* pMsgBitsetMap = new MsgBitsetMap();
    *user_param = pMsgBitsetMap;

    int pid = getpid();
    char log[1024];
    sprintf(log, "/lus/scratch/n13942/sci_log/sci_filter.log.%d", pid);
    //	sci_filter_log = fopen(log, "a+");

    return SCI_SUCCESS;
}

int filter_terminate(void* user_param) {
    bitset* bs;
    SingleMsg* msg;
    MsgBitsetMap* pMsgBitsetMap = (MsgBitsetMap*)user_param;

    MsgBitsetMap::iterator it;
    it = pMsgBitsetMap->begin();

    while (it != pMsgBitsetMap->end()) {
        msg = (SingleMsg*)(it->first);
        bs = (bitset*)(it->second);

        /*		if(bs != NULL)
                                bitset_free(bs);

                        if(msg != NULL)
                                delete msg; */
    }

    pMsgBitsetMap->clear();
    delete pMsgBitsetMap;

    if (sci_filter_log != NULL) fclose(sci_filter_log);

    return SCI_SUCCESS;
}

int filter_input(void* user_param, sci_group_t group, void* buf, int size) {
    char* tmp = (char*)buf;
    MsgBitsetMap* pMsgBitsetMap = (MsgBitsetMap*)user_param;
    ConcatMsg concatMsg, outputMsg;
    SingleMsg* singleMsg;
    bitset* bs;
    bitset* agg;

    MsgBitsetMap::iterator it;
    int index, numOfMsgs, numOfBEs;
    int rc = SCI_SUCCESS;

    // unpack messages
    if ((*tmp) == (char)DBG_SCI_MSG_CONCAT) {
        //		fprintf(sci_filter_log, "filter_input: CONCAT msg is
        //received, size[%d], group[%d]\n", size, group);
        //		fflush(sci_filter_log);

        concatMsg.unpack(tmp + 1, size - 1);
    } else if (*tmp == DBG_SCI_MSG_SINGLE) {
        //		fprintf(sci_filter_log, "filter_input: SINGLE msg is
        //received, size[%d], group[%d]\n", size, group);

        singleMsg = new SingleMsg();
        singleMsg->unpack(tmp + 1, size - 1);

        //		fprintf(sci_filter_log, "filter_input: singleMsg[%p],
        //content[%p], content[%s]\n", singleMsg, singleMsg->getContent(),
        //singleMsg->getContent()); 		fflush(sci_filter_log);

        concatMsg.addSingleMsg(singleMsg);
    } else {
        return SCI_ERR_MSG;
    }

    numOfMsgs = concatMsg.getNumOfMsgs();
    //	fprintf(sci_filter_log, "filter_input: numOfMsgs[%d]\n", numOfMsgs);
    //	fflush(sci_filter_log);

    // aggregate redundant messages
    for (index = 0; index < numOfMsgs; ++index) {
        singleMsg = concatMsg.getMsg(index);
        if ((it = pMsgBitsetMap->find(singleMsg)) ==
            pMsgBitsetMap->end())  // new message
        {
            bs = singleMsg->fetchBS();
            pMsgBitsetMap->insert(MsgBitsetPair(singleMsg, bs));
        } else {  // aggregate with old message
            agg = (bitset*)it->second;
            bs = singleMsg->fetchBS();
            bitset_oreq(agg, bs);
            //			bitset_free(bs);
            delete singleMsg;
        }
    }
    concatMsg.freeMemory(false);

    numOfBEs = 0;
    for (it = pMsgBitsetMap->begin(); it != pMsgBitsetMap->end(); it++) {
        agg = (bitset*)it->second;
        numOfBEs += bitset_count(agg);
    }

    //	fprintf(sci_filter_log, "filter_input: numOfBEs[%d]\n", numOfBEs);
    //	fflush(sci_filter_log);

    if (isReady(numOfBEs)) {
        for (it = pMsgBitsetMap->begin(); it != pMsgBitsetMap->end(); it++) {
            singleMsg = (SingleMsg*)it->first;
            agg = (bitset*)it->second;

            singleMsg->setBS(agg);
            outputMsg.addSingleMsg(singleMsg);
        }

        rc = outputData(group, outputMsg);

        freeMemory(pMsgBitsetMap);

        if (sci_filter_log != NULL) {
            fprintf(sci_filter_log, "filter_input: ready numOfBEs[%d]\n",
                    numOfBEs);
            fflush(sci_filter_log);
        }
    }
    outputMsg.freeMemory(false);

    return rc;
}

// functions
bool isReady(int numOfBEs) {
    int rc, ret_val;
    rc = SCI_Group_query(SCI_GROUP_ALL, GROUP_MEMBER_NUM, &ret_val);
    if (rc != SCI_SUCCESS) {
        assert(!"SCI_Query_info_ext did not work!");
    }

    if (ret_val <= numOfBEs) return true;

    return false;
}

void freeMemory(MsgBitsetMap* pMsg) {
    MsgBitsetMap::iterator it;
    SingleMsg* singleMsg;
    //	bitset*		agg;

    for (it = pMsg->begin(); it != pMsg->end(); it++) {
        singleMsg = (SingleMsg*)it->first;
        //		agg = (bitset*)it->second;

        //		if(agg != NULL)
        //			bitset_free(agg);

        if (singleMsg != NULL) {
            delete singleMsg;
        }
    }

    pMsg->clear();
}

int outputData(sci_group_t group, ConcatMsg& msg) {
    char* tmp;
    void* bufs[1];
    int rc, sizes[1], msgSize;

    msg.prePack();
    msgSize = msg.getMsgSize();

    bufs[0] = new char[msgSize + 1];

    tmp = (char*)bufs[0];
    msg.pack(tmp + 1, msgSize);

    tmp = (char*)(bufs[0]);
    *tmp = DBG_SCI_MSG_CONCAT;

    sizes[0] = msgSize + 1;

    rc = SCI_Filter_upload(SCI_FILTER_NULL, group, 1, bufs, sizes);

    return rc;
}
