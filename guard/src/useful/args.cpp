/*********************************************************************************\
 * Argument routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013, 2020-2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#include "args.h"

char *argsToBuf(const char * const * argv, int argc) {
    return ArgvStream(argv).createBuffer(0, argc);
}

std::string argsToString(const char * const * argv, int argc) {
    return ArgvStream(argv).asString(0, argc);
}
