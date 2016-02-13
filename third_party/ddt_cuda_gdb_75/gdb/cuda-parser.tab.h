/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     START_QUERY_OR_SWITCH = 258,
     START_CONDITIONS = 259,
     START_FILTER = 260,
     START_FILTER_KERNEL = 261,
     DEVICE = 262,
     SM = 263,
     WARP = 264,
     LANE = 265,
     KERNEL = 266,
     GRID = 267,
     BLOCK = 268,
     THREAD = 269,
     BLOCKIDX_X = 270,
     BLOCKIDX_Y = 271,
     BLOCKIDX_Z = 272,
     THREADIDX_X = 273,
     THREADIDX_Y = 274,
     THREADIDX_Z = 275,
     BREAKPOINT = 276,
     CURRENT = 277,
     WILDCARD = 278,
     ALL = 279,
     VALUE = 280,
     CMP = 281,
     COMMA = 282,
     OPENPAR = 283,
     CLOSEPAR = 284,
     LOGICAL_AND = 285,
     LOGICAL_OR = 286,
     PARSING_ERROR = 287
   };
#endif
/* Tokens.  */
#define START_QUERY_OR_SWITCH 258
#define START_CONDITIONS 259
#define START_FILTER 260
#define START_FILTER_KERNEL 261
#define DEVICE 262
#define SM 263
#define WARP 264
#define LANE 265
#define KERNEL 266
#define GRID 267
#define BLOCK 268
#define THREAD 269
#define BLOCKIDX_X 270
#define BLOCKIDX_Y 271
#define BLOCKIDX_Z 272
#define THREADIDX_X 273
#define THREADIDX_Y 274
#define THREADIDX_Z 275
#define BREAKPOINT 276
#define CURRENT 277
#define WILDCARD 278
#define ALL 279
#define VALUE 280
#define CMP 281
#define COMMA 282
#define OPENPAR 283
#define CLOSEPAR 284
#define LOGICAL_AND 285
#define LOGICAL_OR 286
#define PARSING_ERROR 287




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 64 "cuda-parser.exp.y"
{
    uint32_t  value;
    compare_t cmp;
  }
/* Line 1489 of yacc.c.  */
#line 118 "cuda-parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE cuda_parser_lval;

