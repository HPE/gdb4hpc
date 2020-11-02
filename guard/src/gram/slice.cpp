/*********************************************************************************\
 * Slice support.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "defs.h"
#include "gram.h"
#include "useful.h"

#define INIT_SLICE 4

slice_t::slice_t() : m_rank(0), m_size(INIT_SLICE),
                     m_low(new int[INIT_SLICE]), m_high(new int[INIT_SLICE]),
                     m_threads(nullptr), m_args(nullptr) {}

void slice_t::resize() {
    m_size *= 2;

    auto low = new int[m_size];
    auto high = new int[m_size];

    std::copy(m_low, m_low+m_rank, low);
    std::copy(m_high, m_high+m_rank, high);

    std::swap(m_low, low);
    std::swap(m_high, high);

    delete[] low;
    delete[] high;
}

void slice_t::addSlice(int f, int t) {
    int rank = m_rank;

    if (rank >= m_size) resize();

    m_low[rank] = f;
    m_high[rank] = t;
    m_rank++;
}

slice_t::slice_t(const slice_t& rhs) :
    m_rank(rhs.m_rank),
    m_size(rhs.m_size),
    m_low(new int[m_size]),
    m_high(new int[m_size]),
    m_threads(rhs.m_threads ? strdup(rhs.m_threads) : nullptr),
    m_args(rhs.m_args ? strdup(rhs.m_args) : nullptr)
{

    std::copy(rhs.m_low, rhs.m_low+m_rank, m_low);
    std::copy(rhs.m_high, rhs.m_high+m_rank, m_high);
}

slice_t* slice_t::clone() const {
    return new slice_t(*this);
}

/*
** Reverse order of slices.
*/
void slice_t::reverse() {
    std::reverse(m_low, m_low+m_rank);
    std::reverse(m_high, m_high+m_rank);
}

slice_t::~slice_t() {
    delete[] m_low;
    delete[] m_high;
    free(m_threads);
    free(m_args);
}

void slice_t::setThreads(const char* threads)
{
    auto t = (threads ? strdup(threads) : nullptr);
    std::swap(m_threads, t);
    free(t);
}

void slice_t::setArgs(const char* args)
{
    auto a = (args ? strdup(args) : nullptr);
    std::swap(m_args, a);
    free(a);
}

std::string slice_t::getExpr() const
{
    if (m_args) {
        return m_args;
    }

    std::ostringstream sliceexpr;
    for (int n = 0; n < m_rank; n++) {
        sliceexpr << (n ? "," : "(" ) << m_low[n] << "," << m_high[n];
    }
    sliceexpr << ")";
    return sliceexpr.str();
}
