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

class slice_t {
    int m_rank;
    int m_size;
    int* m_low;
    int* m_high;
    char* m_threads;  // for thread focused proc-sets         

public:
    slice_t();
    ~slice_t();

    slice_t& operator=(const slice_t&) = delete;

    slice_t* clone() const;
    
    int getRank() const { return m_rank; }

    int getLow(int n) const { return m_low[n]; }
    int getHigh(int n) const { return m_high[n]; }

    std::string getExpr() const;

    void addSlice(int minIdx, int maxIdx);
    void reverse();

    void setThreads(const char* threads);
    const char* getThreads() const { return m_threads; }

private:
    slice_t(const slice_t&);
    void resize();
};

inline slice_t* CopySlice(slice_t *s) { return s ? s->clone() : nullptr; }


#endif /* !_SLICE_H */
