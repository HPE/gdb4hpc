/*********************************************************************************\
 * Program location.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020,2022 Hewlett Packard Enterprise Development LP.
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

#ifndef _DBGLOC_H
#define _DBGLOC_H

#include <stdlib.h>

struct dbgloc {
    dbgloc() : loc_file(nullptr), loc_func(nullptr), loc_addr(nullptr), loc_line(0) {}
    // takes ownership of the passed pointers
    dbgloc(char* file, char* func, char* addr, int line)
        : loc_file{file}, loc_func{func}, loc_addr{addr}, loc_line{line} {}

    dbgloc(const dbgloc &other);

    dbgloc& operator=(const dbgloc &other);

    dbgloc(dbgloc &&other)
        : loc_file{other.loc_file}, loc_func{other.loc_func}
        , loc_addr{other.loc_addr}, loc_line{other.loc_line}
    {
        other.loc_file = nullptr;
        other.loc_func = nullptr;
        other.loc_addr = nullptr;
    }

    ~dbgloc() {
        free(loc_file);
        free(loc_addr);
        free(loc_func);
    }

    // returns true if all fields match
    bool operator==(const dbgloc& other) const;

    char* loc_file;
    char* loc_func;
    char* loc_addr;
    int loc_line;
};

using dbgloc_t = dbgloc;

// kept as independent functions (rather than e.g. a clone method and destructor)
// since the old c-style event interface still calls these in DupEvent and FreeEvent

inline dbgloc_t *DupLocation(const dbgloc_t *l) {
    if (l == nullptr) return nullptr;;
    return new dbgloc{*l};
}

inline void FreeLocation(dbgloc_t *l) {
    if (l == nullptr) return;
    delete l;
}

#endif /* !_DBGLOC_H */
