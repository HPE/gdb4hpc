/*********************************************************************************\
 * <Communicator.cpp>
 *
 * Copyright 2011-2013,2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#include "mrnet/Communicator.h"

#include "MRNet_type_wrapper.h"

using namespace MRN;

extern "C" int communicator_get_endpoints_size(
    MRNet_Communicator* communicator) {
    Communicator* pCommunicator = (Communicator*)communicator;
    return pCommunicator->get_EndPoints().size();
}
