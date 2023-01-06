/*********************************************************************************\
 * Slice support.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2014,2020-2023 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "defs.h"
#include "useful.h"
#include "sparse_bitset.h"
#include "slice.h"

#define INIT_SLICE 4

void slice_t::cleanup() {
    delete[] m_low;
    delete[] m_high;
    free(m_threads);
    free(m_args);

    m_low = nullptr;
    m_high = nullptr;
    m_threads = nullptr;
    m_args = nullptr;
}

slice_t::slice_t() : m_rank(0), m_size(INIT_SLICE),
                     m_low(new int[INIT_SLICE]), m_high(new int[INIT_SLICE]),
                     m_threads(nullptr), m_args(nullptr) {}

slice_t::slice_t(slice_t &&other)
    : m_rank{other.m_rank}, m_size{other.m_size}
    , m_low{other.m_low}, m_high{other.m_high}
    , m_threads{other.m_threads}, m_args{other.m_args}
{
    other.m_rank = 0;
    other.m_size = 0;
    other.m_low = nullptr;
    other.m_high = nullptr;
    other.m_threads = nullptr;
    other.m_args = nullptr;
}

slice_t& slice_t::operator=(slice_t &&other) {
    if (this == &other) return *this;

    cleanup();

    m_rank = other.m_rank;
    m_size = other.m_size;
    m_low = other.m_low;
    m_high = other.m_high;
    m_threads = other.m_threads;
    m_args = other.m_args;

    other.m_rank = 0;
    other.m_size = 0;
    other.m_low = nullptr;
    other.m_high = nullptr;
    other.m_threads = nullptr;
    other.m_args = nullptr;

    return *this;
}

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
    cleanup();
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

    parseIdcs();
}

std::string slice_t::getExpr(const char* var) const
{
    if (m_args && !m_rank) {
        return std::string{var} + m_args;
    }

    std::ostringstream sliceexpr;
    sliceexpr << var;
    for (int i=0; i<m_rank; ++i) {
        sliceexpr << "[" << m_low[i] << "]";
    }

    if (int d = m_rank-1; d >=0 ) {
        if (int len = m_high[d] - m_low[d] + 1; len > 1) {
            sliceexpr << "@" << len;
        }
    }
    return sliceexpr.str();
}

const char * slice_t::getArgs() const {
    return m_args;
}

void slice_t::parseIdcs() {
    m_rank = 0;
    if (!m_args || !strlen(m_args)) return;

    // the gdb4hpc syntax has some ambiguity as process lists can look like fortran array
    // references.   So here we examine a parsed argument list and create the array
    // of min/max indices if it fits that interpretation
    auto arg = std::string{m_args};
    if (arg[0] == '(' && arg[arg.size()-1] == ')') {
        arg = arg.substr(1,arg.size()-2);
    }
        
    auto nonMatch = arg.find_first_not_of("0123456789,:*-");
    if (nonMatch != std::string::npos) {
        DPRINT("argument is not a slice, it has: %c\n", arg[nonMatch]);
        return;
    }

    auto is = std::istringstream{arg};
    auto a = std::string{};
    while (std::getline(is, a, ',')) {
        if (a == "*") {
            addSlice(0,INT_MAX);
        } else if (auto col = a.find(':'); col != std::string::npos) {
            auto low = std::stoi(a);
            auto high = std::stoi(a.substr(col+1));
            addSlice(low,high);
        } else {
            auto val = std::stoi(a);
            addSlice(val,val);
        }
    }
}

std::unique_ptr<Bitset> slice_t::createBitset() const {
    // find largest index in slice to determine bitset size
    int size = -1;
    for (int i = 0; i < getRank(); i++) {
        size = std::max(size, getHigh(i));
    }
    size++;

    if (size <= 0) {
        return nullptr;
    } else {
        auto bs = std::make_unique<SparseBitset>(size);
        for (int i = 0; i < getRank(); i++) {
            for (int j = getLow(i); j <= getHigh(i); j++) {
                bs->set(j);
            }
        }
        return bs;
    }
}
