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
     DEVICE = 261,
     SM = 262,
     WARP = 263,
     LANE = 264,
     KERNEL = 265,
     GRID = 266,
     BLOCK = 267,
     THREAD = 268,
     BLOCKIDX_X = 269,
     BLOCKIDX_Y = 270,
     BLOCKIDX_Z = 271,
     THREADIDX_X = 272,
     THREADIDX_Y = 273,
     THREADIDX_Z = 274,
     CURRENT = 275,
     WILDCARD = 276,
     ALL = 277,
     VALUE = 278,
     CMP = 279,
     COMMA = 280,
     OPENPAR = 281,
     CLOSEPAR = 282,
     LOGICAL_AND = 283,
     LOGICAL_OR = 284,
     PARSING_ERROR = 285
   };
#endif
/* Tokens.  */
#define START_QUERY_OR_SWITCH 258
#define START_CONDITIONS 259
#define START_FILTER 260
#define DEVICE 261
#define SM 262
#define WARP 263
#define LANE 264
#define KERNEL 265
#define GRID 266
#define BLOCK 267
#define THREAD 268
#define BLOCKIDX_X 269
#define BLOCKIDX_Y 270
#define BLOCKIDX_Z 271
#define THREADIDX_X 272
#define THREADIDX_Y 273
#define THREADIDX_Z 274
#define CURRENT 275
#define WILDCARD 276
#define ALL 277
#define VALUE 278
#define CMP 279
#define COMMA 280
#define OPENPAR 281
#define CLOSEPAR 282
#define LOGICAL_AND 283
#define LOGICAL_OR 284
#define PARSING_ERROR 285




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 67 "cuda-parser.exp.y"
{
    uint32_t  value;
    compare_t cmp;
  }
/* Line 1489 of yacc.c.  */
#line 114 "cuda-parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE cuda_parser_lval;

