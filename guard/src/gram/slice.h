/*********************************************************************************\
 * Slice defines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2014,2020 Hewlett Packard Enterprise Development LP.
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

#ifndef _SLICE_H
#define _SLICE_H

#include <string>
#include <vector>

// This is a candidate for a rename.   Originally, it was used to track the info for
// extracting a portion of an array - and it still is.   But it has morphed into a more 
// general modifier for a symbol encompassing the thread set for a reference, and
// the argument list for a function call.   Note: actually due to ambiguities in the
// expression syntax - it can be called to represent a modifier that could be *either*
// an array reference/slice or a function call.   
class slice_t {
    int m_rank;       // number of dimensions for an array reference/slice
    int m_size;       // allocated size of m_low and m_high
    int* m_low;       // minimum of each rank range
    int* m_high;      // maximum of each rank range
    char* m_threads;  // for thread focused proc-sets
    char* m_args;     // argument list for a function call

public:
    slice_t();
    ~slice_t();

    slice_t& operator=(const slice_t&) = delete;

    slice_t* clone() const;
    
    int getRank() const { return m_rank; }

    int getLow(int n) const { return m_low[n]; }
    int getHigh(int n) const { return m_high[n]; }

    // get the xlice expression for variable var
    std::string getExpr(const char* var) const;

    // add another dimension value
    void addSlice(int minIdx, int maxIdx);

    // reverse the order of the ranks (addSlice is typically call back to front)
    void reverse();

    void setThreads(const char* threads);
    const char* getThreads() const { return m_threads; }

    void setArgs(const char* args);

private:
    slice_t(const slice_t&);
    void resize();
    void parseIdcs();
};

inline slice_t* CopySlice(slice_t *s) { return s ? s->clone() : nullptr; }


#endif /* !_SLICE_H */
