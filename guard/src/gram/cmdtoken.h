/*********************************************************************************\
 * Command token definitions
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Cray Inc.
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

#ifndef _CMDTOKEN_H
#define _CMDTOKEN_H

#include "hash.h"

#define SYM_SIZE 7   /* 2^7 = 128 = symbol table size */
#define TOKEN_SIZE 8 /* 2^8 = 256 = token table size */
#define TYPE_SIZE 7  /* 2^7 = 128 = lang type table size */

/*
** Command tokens start here
*/
#define CT_ERR -1
// CT_EOF must be 0 for yyparse().
#define CT_EOF 0

/*
** Operator tokens - may or may not be a debugger operator
*/
// arithmetic
#define CT_PLUS 128   // +
#define CT_MINUS 129  // -
#define CT_STAR 130   // *
#define CT_DIV 131    // /
#define CT_MOD 132    // %
// assignment
#define CT_EQUAL 133  // =
// augmented assignment
#define CT_ADDEQ 134    // +=
#define CT_SUBEQ 135    // -=
#define CT_MULEQ 136    // *=
#define CT_DIVEQ 137    // /=
#define CT_MODEQ 138    // %=
#define CT_BTANDEQ 139  // &=
#define CT_BTOREQ 140   // |=
#define CT_BTXOREQ 141  // ^=
#define CT_BTLSEQ 142   // <<=
#define CT_BTRSEQ 143   // >>=
// bitwise logic
#define CT_BTNOT 144  // ~
#define CT_BTAND 145  // &
#define CT_BTOR 146   // |
#define CT_BTXOR 147  // ^
// bitwise shifts
#define CT_BTLS 148  // <<
#define CT_BTRS 149  // >>
// boolean logic
#define CT_NOT 150  // !
#define CT_AND 151  // &&
#define CT_OR 152   // ||
// conditional evaluation
#define CT_TERNARY 153  // ?
#define CT_COLON 154    // :
// equality testing
#define CT_DBLEQ 155  // ==
#define CT_NOTEQ 156  // !=
// functions or ftn array reference
#define CT_LPAREN 157  // (
#define CT_RPAREN 158  // )
// increment/decrement
#define CT_DBLPLUS 159   // ++
#define CT_DBLMINUS 160  // --
// member selection
#define CT_ARROW 161  // ->
#define CT_DOT 162    // .
// order relations
#define CT_LT 163   // <
#define CT_LEQ 164  // <=
#define CT_GT 165   // >
#define CT_GEQ 166  // >=
// reference/dereference
// star/and already above
#define CT_LBRCK 167  // [
#define CT_RBRCK 168  // ]
// sequencing
#define CT_COMMA 169  // ,

/*
** Debugger operators
*/
#define CT_AT 170      // @
#define CT_DBLCOL 171  // ::
#define CT_DBLDOT 172  // ..
#define CT_LBRACE 173  // {
#define CT_RBRACE 174  // }

/*
** Keywords - c only for now...
*/
#define CT_SIZEOF 175      // sizeof - TODO
#define CT_TYPE_SPEC 176   // unsigned, signed, etc
#define CT_TYPE_IDENT 177  // float, int, double, etc

/*
** Format tokens
*/
#define CT_HASH 178  // #
#define CT_NL 179    // \n

/*
** Tokens with values
*/
#define CT_DSYM 180   // $dbgVar
#define CT_FLOAT 181  // 1.035e-13
#define CT_INT 182    // 1234
#define CT_STR 183    // "some string"
#define CT_SYM 184
#define CT_SYM_STR 185

/*
** Other stuff
** FIXME: Remove all of these...
*/
#define CT_BLOCK 186
#define CT_CYCLIC 187
#define CT_TEXT 188
#define CT_AIF 189
#define CT_HDF 190
#define CT_HDF5 191
#define CT_SUM 192
#define CT_PRODUCT 193
#define CT_AVERAGE 194
#define CT_MIN 195
#define CT_MAX 196
#define CT_STDEV 197
#define CT_COUNTELEM 198
#define CT_HISTOGRAM 199
#define CT_BOBHASH 200
#define CT_NONE 201
#define CT_DEFAULT 202
#define CT_SYNC 203
#define CT_ASYNC 204
#define CT_BLOCK_CYCLIC 205

/*
** Tokens for GC and digraph files.
*/
#define CT_ABSOLUTE 206
#define CT_ASSERT 207
#define CT_DFMODE 208
#define CT_DISPLAY 209
#define CT_END 210
#define CT_ERROR 211
#define CT_FALSE 212
#define CT_FORCE 213
#define CT_HALT 214
#define CT_INVOKE 215
#define CT_LOGFILE 216
#define CT_LOGGING 217
#define CT_OUTPUT 218
#define CT_RELATIVE 219
#define CT_SET 220
#define CT_TRUE 221
#define CT_USING 222
#define CT_AUTO 223

typedef int ct_type;

/*
** FIXME: andrewg: The ct_val looks a lot like an expr_val...
** The two probably should be merged.
*/

struct ct_val {
    union {
        int cv_u_int;           /* value of last CT_INT */
        long double cv_u_float; /* value of last CT_FLOAT */
        hentry_t* cv_u_sym;     /* value of last CT_DSYM */
        char* cv_u_sym_str;     /* value of last CT_SYM */
        char* cv_u_str;         /* value of last CT_STR */
    } cv_u;
#define cv_int cv_u.cv_u_int
#define cv_float cv_u.cv_u_float
#define cv_sym cv_u.cv_u_sym
#define cv_sym_str cv_u.cv_u_sym_str
#define cv_str cv_u.cv_u_str
};
typedef struct ct_val ct_val;

#endif /* !_CMDTOKEN_H */
