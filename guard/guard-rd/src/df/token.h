/*********************************************************************************\
 * Token definitions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Hewlett Packard Enterprise Development LP.
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
 **********************************************************************************/

#ifndef _TOKEN_H
#define _TOKEN_H

#include "aif.h"
#include "dense_bitset.h"
#include "procset.h"

#define T_CHUNK 128
#define T_NCHUNKS 20

#define TK_BAD 0x0000
#define TK_ANY 0xffff
#define TK_VALID 0x0fff

#define TK_DATA 0x0001
#define TK_BOOL 0x0002
//#define	 TK_INV			0x0004
#define TK_INT 0x0008
#define TK_ADDR 0x0010
#define TK_STR 0x0020
//#define	 TK_PROC		0x0040
#define TK_FLOAT 0x0080

#define TK_PROC_SET 0x0100
#define TK_INT_SET 0x0200
#define TK_DATA_SET 0x0400
#define TK_BIT_SET 0x0800

typedef unsigned int token_type;

struct exec_t {
    void* e_exec;
    int e_lang;
};

struct token_t {
    int t_id;
    int t_ref;
    struct node_t* t_node;

    token_type t_type;
    struct token_t* t_prev;
    struct token_t* t_next;

    char* t_name;

    // Set-related data member
    char* t_set_name;
    int t_set_length;
    std::unique_ptr<DenseBitset> t_set_bits;

    union t_d {
        AIF* t_d_data;
        int t_d_int;
        double t_d_float;
        char* t_d_str;
        void* t_d_addr;
        dbgprocset_t* t_d_procset;
        int* t_d_int_set;
        AIF** t_d_data_set;
        std::unique_ptr<DenseBitset> t_d_bit_set;
        int t_d_bool;

        t_d() : t_d_data{nullptr} {};
        ~t_d() {}; // properly destroyed by DestroyToken
    } t_d;
#define t_data t_d.t_d_data
#define t_int t_d.t_d_int
#define t_float t_d.t_d_float
#define t_str t_d.t_d_str
#define t_addr t_d.t_d_addr
#define t_proc_set t_d.t_d_procset
#define t_int_set t_d.t_d_int_set
#define t_data_set t_d.t_d_data_set
#define t_bit_set t_d.t_d_bit_set
#define t_bool t_d.t_d_bool

    /* keep unique memory addersses in t_d_data_set for release */
    // FIXME: This is a temporary hack until graph is modified to understand
    // either reply_set_t or val_t.
    rep_set_t* t_d_shadow_reply;
};
typedef struct token_t token_t;

#endif /* !_TOKEN_H */
