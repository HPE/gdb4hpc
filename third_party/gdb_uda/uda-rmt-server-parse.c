/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse uda_server_cmd_yyparse
#define yylex   uda_server_cmd_yylex
#define yyerror uda_server_cmd_yyerror
#define yylval  uda_server_cmd_yylval
#define yychar  uda_server_cmd_yychar
#define yydebug uda_server_cmd_yydebug
#define yynerrs uda_server_cmd_yynerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     KW_ADDRESS = 258,
     KW_LEN = 259,
     KW_READ = 260,
     KW_TYPE = 261,
     KW_UPC = 262,
     KW_WRITE = 263,
     KW_DIFF = 264,
     KW_INDEX = 265,
     KW_PACK = 266,
     KW_PTS = 267,
     KW_SHARED = 268,
     KW_SIZES = 269,
     KW_SYM = 270,
     KW_THREADS = 271,
     KW_UNPACK = 272,
     ID_STRING = 273,
     BINARY_DATA = 274,
     HEX_NUM = 275
   };
#endif
/* Tokens.  */
#define KW_ADDRESS 258
#define KW_LEN 259
#define KW_READ 260
#define KW_TYPE 261
#define KW_UPC 262
#define KW_WRITE 263
#define KW_DIFF 264
#define KW_INDEX 265
#define KW_PACK 266
#define KW_PTS 267
#define KW_SHARED 268
#define KW_SIZES 269
#define KW_SYM 270
#define KW_THREADS 271
#define KW_UNPACK 272
#define ID_STRING 273
#define BINARY_DATA 274
#define HEX_NUM 275




/* Copy the first part of user declarations.  */
#line 37 "uda-rmt-server-parse.y"


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "uda-types.h"
#include "uda-defs.h"
#include "uda-rmt-utils.h"
#include "uda-server.h"
#define uda_server_cmd_yyline uda_server_cmd_uda_server_cmd_yyline
#define uda_server_cmd_yytext uda_server_cmd_uda_server_cmd_yytext
#define uda_server_cmd_yyleng uda_server_cmd_uda_server_cmd_yyleng
#define uda_server_cmd_yysstate uda_server_cmd_uda_server_cmd_yysstate
#define uda_server_cmd_yycp uda_server_cmd_uda_server_cmd_yycp
#define uda_server_cmd_yykw uda_server_cmd_uda_server_cmd_yykw
#define uda_server_cmd_exec uda_server_cmd_exec


#define YYERRORVERBOSE 1
#define YYINITDEPTH 20
#define YYMAXDEPTH  20

extern int uda_server_cmd_yylex(void);
extern void uda_server_cmd_yyerror(const char *msg);

/* Lexer state communication variables  */

typedef enum {scan_cmd, scan_args, scan_data, scan_id, scan_pts} uda_server_cmd_yysstate_t;

static uda_server_cmd_yysstate_t uda_server_cmd_yysstate;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 73 "uda-rmt-server-parse.y"
{
  uda_tword_t val;
  uda_tword_t val2[2];
  uda_tint_t signed_val;
  uda_taddr_t addr_val;
  uda_debugger_pts_t pts;
  char *str;
  uda_binary_data_t data;
}
/* Line 187 of yacc.c.  */
#line 190 "uda-rmt-server-parse.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 203 "uda-rmt-server-parse.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   115

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  28
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNRULES -- Number of states.  */
#define YYNSTATES  151

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   275

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    26,    23,    27,    22,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    25,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    21,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    24,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    27,    34,    53,    66,    74,    85,
      97,   109,   123,   137,   150,   163,   165,   167,   169,   171,
     173,   175,   177,   179,   180,   182,   184,   186,   188,   190,
     192,   194,   196,   197,   199,   200,   202,   203,   206,   207,
     210,   211,   214,   219,   220,   222,   223,   226,   231,   232,
     234,   236,   238,   240,   242,   244,   247
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      29,     0,    -1,    30,    -1,    39,    -1,    38,    -1,    34,
      -1,    36,    -1,    40,    -1,    31,    -1,    32,    -1,    33,
      -1,    35,    -1,    37,    -1,    41,    -1,    21,     7,    22,
      16,    42,    58,    -1,    21,     7,    22,     6,    22,    14,
      42,    70,    23,    55,    23,    57,    23,    56,    23,    69,
      23,    52,    -1,    24,     7,    22,    15,    42,    64,    23,
      60,    23,    59,    44,    47,    -1,    24,     7,    22,    12,
      22,     4,    62,    -1,    24,     7,    22,    12,    22,    17,
      42,    50,    67,    61,    -1,    24,     7,    22,    12,    22,
      11,    42,    49,    23,    48,    65,    -1,    24,     7,    22,
      12,    22,     3,    42,    49,    23,    48,    63,    -1,    24,
       7,    22,    12,    22,    10,    42,    72,    23,    54,    23,
      53,    61,    -1,    24,     7,    22,    12,    22,     9,    42,
      72,    23,    72,    23,    53,    61,    -1,    24,     7,    22,
       5,    22,    13,    42,    49,    23,    48,    23,    46,    -1,
      21,     7,    22,     8,    22,    13,    42,    49,    23,    48,
      43,    45,    -1,    25,    -1,    23,    -1,    23,    -1,    19,
      -1,    20,    -1,    18,    -1,    20,    -1,    20,    -1,    -1,
      20,    -1,    20,    -1,    20,    -1,    71,    -1,    20,    -1,
      20,    -1,    20,    -1,    20,    -1,    -1,    49,    -1,    -1,
      51,    -1,    -1,    23,    60,    -1,    -1,    42,    51,    -1,
      -1,    23,    64,    -1,    23,    51,    23,    64,    -1,    -1,
      53,    -1,    -1,    23,    60,    -1,    23,    68,    23,    60,
      -1,    -1,    26,    -1,    27,    -1,    19,    -1,    20,    -1,
      20,    -1,    20,    -1,    66,    20,    -1,    49,    23,    48,
      23,    68,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   136,   136,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   157,   167,   185,   207,   221,   261,
     285,   309,   339,   360,   381,   402,   406,   410,   413,   417,
     421,   425,   429,   434,   438,   442,   446,   450,   454,   458,
     462,   466,   471,   472,   478,   479,   485,   486,   492,   493,
     499,   500,   502,   508,   509,   515,   516,   518,   523,   524,
     525,   529,   533,   537,   541,   545,   550
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "KW_ADDRESS", "KW_LEN", "KW_READ",
  "KW_TYPE", "KW_UPC", "KW_WRITE", "KW_DIFF", "KW_INDEX", "KW_PACK",
  "KW_PTS", "KW_SHARED", "KW_SIZES", "KW_SYM", "KW_THREADS", "KW_UNPACK",
  "ID_STRING", "BINARY_DATA", "HEX_NUM", "'Q'", "'.'", "','", "'q'", "':'",
  "'+'", "'-'", "$accept", "command", "server_cmd", "set_num_threads_cmd",
  "set_type_sizes_and_byte_order_cmd", "shared_sym_to_pts_cmd",
  "length_of_upc_pts_cmd", "unpack_upc_pts_cmd", "pack_upc_pts_cmd",
  "upc_pts_to_addr_cmd", "calc_upc_pts_index_add_cmd",
  "calc_upc_pts_diff_cmd", "read_upc_shared_mem_cmd",
  "write_upc_shared_mem_cmd", "begin_args", "begin_binary_data_arg",
  "begin_id_arg", "data", "length", "symbol", "thread_num", "addrfield",
  "begin_packed_pts", "block_size", "byte_order", "elem_size", "index",
  "int_size", "long_long_size", "long_size", "num_threads",
  "opt_addrfield", "opt_block_size", "opt_block_size_arg",
  "opt_block_size_arg1", "opt_block_and_elem_size", "opt_elem_size",
  "opt_phase_and_block_size_arg", "opt_sign", "packed_pts", "phase",
  "pointer_size", "short_size", "signed_hex_num", "unpacked_pts", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,    81,    46,    44,   113,    58,    43,    45
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    28,    29,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    59,    60,    60,    61,    61,    62,    62,
      63,    63,    63,    64,    64,    65,    65,    65,    66,    66,
      66,    67,    68,    69,    70,    71,    72
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     6,    18,    12,     7,    10,    11,
      11,    13,    13,    12,    12,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     0,     1,     0,     2,     0,     2,
       0,     2,     4,     0,     1,     0,     2,     4,     0,     1,
       1,     1,     1,     1,     1,     2,     5
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     2,     8,     9,    10,     5,    11,
       6,    12,     4,     3,     7,    13,     0,     0,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,    53,     0,     0,    41,    14,     0,     0,
      48,     0,     0,     0,     0,    36,    54,     0,     0,     0,
       0,     0,     0,    17,     0,     0,     0,    33,    44,    64,
       0,    32,     0,     0,     0,    34,    49,     0,     0,     0,
       0,     0,    45,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    61,    46,    42,    38,     0,    31,     0,     0,
      50,     0,     0,    59,    60,     0,     0,    37,    55,    44,
      18,    43,     0,     0,    26,     0,     0,    53,    20,     0,
       0,     0,    65,    44,    19,    47,    27,     0,    40,     0,
      28,    24,    29,    23,    34,     0,    51,    62,    66,    46,
      46,    34,    56,     0,    30,    16,     0,    53,    22,    21,
      44,    39,     0,    52,    57,     0,    63,     0,     0,    35,
      15
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    30,   105,   117,   121,   123,   135,
      88,    67,    71,    72,   150,    46,    95,    86,   142,   119,
      37,   102,    73,   100,    53,   108,    47,   114,    96,    83,
     128,   147,    60,    97,    68
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -112
static const yytype_int8 yypact[] =
{
     -14,    20,    21,    31,  -112,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,  -112,  -112,  -112,  -112,     7,    23,  -112,    28,
      36,    25,    27,    18,    34,    37,    18,    44,    47,  -112,
      30,    48,    29,    42,    18,    18,  -112,  -112,    18,    18,
      18,    18,    18,    18,    18,  -112,  -112,    40,    45,    46,
      46,    46,    49,  -112,    46,    46,    46,  -112,    49,  -112,
      41,  -112,    50,    51,    52,  -112,  -112,    53,    54,    55,
      56,    61,  -112,    58,    62,    63,    63,    63,    63,    46,
     -15,    63,  -112,    64,    46,  -112,    65,  -112,    66,    67,
      68,    69,    70,  -112,  -112,    71,    75,  -112,    73,    49,
    -112,  -112,    74,    78,  -112,    80,    81,    82,  -112,    83,
      42,    42,  -112,    84,  -112,  -112,  -112,    87,  -112,    77,
    -112,  -112,  -112,  -112,    72,    85,  -112,  -112,  -112,    64,
      64,    86,  -112,    88,  -112,  -112,    90,    42,  -112,  -112,
      49,  -112,    89,  -112,  -112,    93,  -112,    91,    95,  -112,
    -112
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,  -112,  -112,   -18,  -112,  -112,  -112,  -112,  -112,
     -24,   -47,  -112,   -52,  -112,   -97,  -112,  -112,  -112,  -112,
    -112,  -112,   -98,  -111,  -112,  -112,  -102,  -112,  -112,  -112,
     -46,  -112,  -112,  -112,   -49
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -63
static const yytype_int16 yytable[] =
{
      66,   115,    62,    63,    64,   126,    69,     1,    33,    70,
       2,    93,    94,   129,   130,   132,    48,    49,   138,   139,
      50,    51,    52,    54,    55,    56,    57,    16,    17,    19,
      92,    18,    39,    40,    21,   143,    22,   101,    41,    42,
      43,    24,   144,    29,    23,    20,    44,    27,    25,    28,
      36,    26,    89,    90,    91,   125,    31,    98,    34,    32,
      35,    38,    45,    58,    74,    59,    61,   133,     0,    65,
       0,     0,   -36,    75,    76,    77,    78,    79,    80,    81,
      82,    84,    85,    87,     0,     0,     0,    99,   103,   104,
     106,   107,   109,   110,   111,   112,   113,   116,   118,   120,
     136,   122,   124,   127,   131,   134,     0,     0,   137,   -62,
     141,   140,   145,   146,   148,   149
};

static const yytype_int16 yycheck[] =
{
      52,    99,    49,    50,    51,   107,    55,    21,    26,    56,
      24,    26,    27,   110,   111,   113,    34,    35,   129,   130,
      38,    39,    40,    41,    42,    43,    44,     7,     7,    22,
      79,     0,     3,     4,     6,   137,     8,    84,     9,    10,
      11,     5,   140,    25,    16,    22,    17,    22,    12,    22,
      20,    15,    76,    77,    78,   107,    22,    81,    14,    22,
      13,    13,    20,    23,    23,    20,    20,   113,    -1,    20,
      -1,    -1,     0,    23,    23,    23,    23,    23,    23,    23,
      19,    23,    20,    20,    -1,    -1,    -1,    23,    23,    23,
      23,    23,    23,    23,    23,    20,    23,    23,    20,    19,
      23,    20,    20,    20,    20,    18,    -1,    -1,    23,    23,
      20,    23,    23,    20,    23,    20
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    21,    24,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,     7,     7,     0,    22,
      22,     6,     8,    16,     5,    12,    15,    22,    22,    25,
      42,    22,    22,    42,    14,    13,    20,    58,    13,     3,
       4,     9,    10,    11,    17,    20,    53,    64,    42,    42,
      42,    42,    42,    62,    42,    42,    42,    42,    23,    20,
      70,    20,    49,    49,    49,    20,    51,    49,    72,    72,
      49,    50,    51,    60,    23,    23,    23,    23,    23,    23,
      23,    23,    19,    67,    23,    20,    55,    20,    48,    48,
      48,    48,    72,    26,    27,    54,    66,    71,    48,    23,
      61,    49,    59,    23,    23,    43,    23,    23,    63,    23,
      23,    23,    20,    23,    65,    60,    23,    44,    20,    57,
      19,    45,    20,    46,    20,    51,    64,    20,    68,    53,
      53,    20,    60,    68,    18,    47,    23,    23,    61,    61,
      23,    20,    56,    64,    60,    23,    20,    69,    23,    20,
      52
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 14:
#line 158 "uda-rmt-server-parse.y"
    {
	      int num_threads = (int)(yyvsp[(6) - (6)].val);
	      int status;
	      status = uda_server_set_num_threads_cmd (num_threads);
	      uda_rmt_send_status (status);
	    ;}
    break;

  case 15:
#line 170 "uda-rmt-server-parse.y"
    {
	      int byte_order = (int)(yyvsp[(18) - (18)].val);
	      int status;
	      uda_target_type_sizes_t sizes;
	      sizes.short_size = (int)(yyvsp[(8) - (18)].val);
	      sizes.int_size = (int)(yyvsp[(10) - (18)].val);
	      sizes.long_size = (int)(yyvsp[(12) - (18)].val);
	      sizes.long_long_size = (int)(yyvsp[(14) - (18)].val);
	      sizes.pointer_size = (int)(yyvsp[(16) - (18)].val);
	      status = uda_server_set_target_info_cmd (sizes, byte_order);
	      uda_rmt_send_status (status);
	    ;}
    break;

  case 16:
#line 188 "uda-rmt-server-parse.y"
    {
	      const uda_tword_t elem_size  = (yyvsp[(6) - (12)].val);
	      const uda_tword_t block_size = (yyvsp[(8) - (12)].val);
	      const uda_taddr_t sym_addr = (yyvsp[(10) - (12)].addr_val);
	      const char *symbol = (yyvsp[(12) - (12)].str);
	      uda_debugger_pts_t pts;
	      int status;
	      status = uda_server_symbol_to_pts_cmd (symbol,
				 sym_addr, block_size, elem_size, &pts);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%lux,%lux",
		                    pts.addrfield, pts.thread);
	      uda_free ((void *) symbol);
	    ;}
    break;

  case 17:
#line 208 "uda-rmt-server-parse.y"
    {
	      const uda_tword_t block_size = (yyvsp[(7) - (7)].val);
	      uda_tword_t pts_len;
	      int status;
	      status = uda_server_length_of_pts_cmd (block_size, &pts_len);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%lux", pts_len);
	    ;}
    break;

  case 18:
#line 223 "uda-rmt-server-parse.y"
    {
	      const uda_target_pts_t *packed_pts =
	                 (uda_target_pts_t *)(yyvsp[(9) - (10)].data).bytes;
	      const uda_tword_t block_size = (yyvsp[(10) - (10)].val);
	      uda_debugger_pts_t pts;
	      int status;
              status = uda_server_unpack_pts_cmd (packed_pts, block_size, &pts);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        {
	          const int has_opaque = uda_job->image->target_pts_has_opaque
	                                 && (pts.opaque != 0);
		  if (has_opaque)
		    {
		      if (pts.phase != 0)
	                uda_rmt_send_reply ("%lux,%lux,%lux,%lux",
		             pts.addrfield, pts.thread,
			     pts.phase, pts.opaque);
		      else
	                uda_rmt_send_reply ("%lux,%lux,,%lux",
		             pts.addrfield, pts.thread, pts.opaque);
		    }
		  else
		    {
		      if (pts.phase != 0)
	                uda_rmt_send_reply ("%lux,%lux,%lux",
		             pts.addrfield, pts.thread, pts.phase);
		      else
	                uda_rmt_send_reply ("%lux,%lux",
		             pts.addrfield, pts.thread);
		    }
	        }
	       uda_free ((void *) packed_pts);
	    ;}
    break;

  case 19:
#line 263 "uda-rmt-server-parse.y"
    {
	      uda_debugger_pts_t pts;
	      uda_tword_t block_size;
	      uda_target_pts_t packed_pts;
	      size_t packed_pts_len;
	      int status;
	      pts.addrfield = (yyvsp[(8) - (11)].addr_val);
	      pts.thread = (yyvsp[(10) - (11)].val);
	      pts.phase = (yyvsp[(11) - (11)].val2)[0];
	      pts.opaque = 0;
	      block_size = (yyvsp[(11) - (11)].val2)[1];
              status = uda_server_pack_pts_cmd (&pts, block_size,
						&packed_pts_len,
	                                        &packed_pts);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%*X", packed_pts_len, &packed_pts);
	    ;}
    break;

  case 20:
#line 287 "uda-rmt-server-parse.y"
    {
	      uda_debugger_pts_t pts;
	      uda_tword_t block_size;
	      uda_tword_t elem_size;
	      uda_taddr_t local_addr;
	      int status;
	      pts.addrfield = (yyvsp[(8) - (11)].addr_val);
	      pts.thread = (yyvsp[(10) - (11)].val);
	      pts.phase = 0;
	      pts.opaque = 0;
	      block_size = (yyvsp[(11) - (11)].val2)[0];
	      elem_size = (yyvsp[(11) - (11)].val2)[1];
              status = uda_server_pts_to_addr_cmd (&pts, block_size,
	                                           elem_size, &local_addr);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%lx", local_addr);
	    ;}
    break;

  case 21:
#line 312 "uda-rmt-server-parse.y"
    {
	      const uda_debugger_pts_t pts_operand = (yyvsp[(8) - (13)].pts);
	      const uda_tint_t index = (yyvsp[(10) - (13)].signed_val);
	      const uda_tword_t elem_size = (yyvsp[(12) - (13)].val);
	      const uda_tword_t block_size = (yyvsp[(13) - (13)].val);
	      uda_debugger_pts_t pts;
	      int status;
              status = uda_server_pts_index_add_cmd (&pts_operand,
	                                             index,
	                                             elem_size, block_size,
						     &pts);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        {
		  if (pts.phase != 0)
	            uda_rmt_send_reply ("%lux,%lux,%lux",
		                        pts.addrfield, pts.thread,
					pts.phase);
                  else
	            uda_rmt_send_reply ("%lux,%lux",
		                        pts.addrfield, pts.thread);
		}
	    ;}
    break;

  case 22:
#line 342 "uda-rmt-server-parse.y"
    {
	      const uda_debugger_pts_t pts_oprnd_1 = (yyvsp[(8) - (13)].pts);
	      const uda_debugger_pts_t pts_oprnd_2 = (yyvsp[(10) - (13)].pts);
	      const uda_tword_t elem_size = (yyvsp[(12) - (13)].val);
	      const uda_tword_t block_size = (yyvsp[(13) - (13)].val);
	      uda_tint_t diff;
	      int status;
              status = uda_server_pts_diff_cmd (&pts_oprnd_1, &pts_oprnd_2,
	                                        elem_size, block_size,
				                &diff);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%lx", diff);
	    ;}
    break;

  case 23:
#line 362 "uda-rmt-server-parse.y"
    {
	      const uda_taddr_t addr = (yyvsp[(8) - (12)].addr_val);
	      const uda_tword_t thread_num = (yyvsp[(10) - (12)].val);
	      const uda_tword_t length = (yyvsp[(12) - (12)].val);
	      uda_binary_data_t data;
	      int status;
	      data.len = length;
              data.bytes = (uda_byte_t *) uda_malloc ((size_t) length);
              status = uda_server_read_upc_shared_mem_cmd (
				  addr, thread_num, length, &data);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%*b", data.len, data.bytes);
	      uda_free (data.bytes);
	    ;}
    break;

  case 24:
#line 383 "uda-rmt-server-parse.y"
    {
	      const uda_taddr_t addrfield = (yyvsp[(8) - (12)].addr_val);
	      const uda_tword_t thread_num = (yyvsp[(10) - (12)].val);
	      const uda_binary_data_t data = (yyvsp[(12) - (12)].data);
	      uda_tword_t bytes_written;
	      int status;
              status = uda_server_write_upc_shared_mem_cmd (
				  addrfield, thread_num,
				  &bytes_written, &data);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%lux", bytes_written);
	      uda_free ((void *) data.bytes);
	    ;}
    break;

  case 25:
#line 402 "uda-rmt-server-parse.y"
    { uda_server_cmd_yysstate = scan_args; ;}
    break;

  case 26:
#line 406 "uda-rmt-server-parse.y"
    { uda_server_cmd_yysstate = scan_data; ;}
    break;

  case 27:
#line 410 "uda-rmt-server-parse.y"
    { uda_server_cmd_yysstate = scan_id; ;}
    break;

  case 32:
#line 430 "uda-rmt-server-parse.y"
    { (yyval.addr_val) = (uda_taddr_t)(yyvsp[(1) - (1)].val); ;}
    break;

  case 33:
#line 434 "uda-rmt-server-parse.y"
    { uda_server_cmd_yysstate = scan_pts; ;}
    break;

  case 42:
#line 471 "uda-rmt-server-parse.y"
    { (yyval.addr_val) = 0; ;}
    break;

  case 43:
#line 473 "uda-rmt-server-parse.y"
    { (yyval.addr_val) = (yyvsp[(1) - (1)].addr_val); ;}
    break;

  case 44:
#line 478 "uda-rmt-server-parse.y"
    { (yyval.val) = 1; ;}
    break;

  case 45:
#line 480 "uda-rmt-server-parse.y"
    { (yyval.val) = (yyvsp[(1) - (1)].val); ;}
    break;

  case 46:
#line 485 "uda-rmt-server-parse.y"
    { (yyval.val) = 1; ;}
    break;

  case 47:
#line 487 "uda-rmt-server-parse.y"
    { (yyval.val) = (yyvsp[(2) - (2)].val); ;}
    break;

  case 48:
#line 492 "uda-rmt-server-parse.y"
    { (yyval.val) = 1; ;}
    break;

  case 49:
#line 494 "uda-rmt-server-parse.y"
    { (yyval.val) = (yyvsp[(2) - (2)].val); ;}
    break;

  case 50:
#line 499 "uda-rmt-server-parse.y"
    { (yyval.val2)[0] = 0; (yyval.val2)[1] = 1; ;}
    break;

  case 51:
#line 501 "uda-rmt-server-parse.y"
    { (yyval.val2)[0] = 0; (yyval.val2)[1] = (yyvsp[(2) - (2)].val); ;}
    break;

  case 52:
#line 503 "uda-rmt-server-parse.y"
    { (yyval.val2)[0] = (yyvsp[(2) - (4)].val); (yyval.val2)[1] = (yyvsp[(4) - (4)].val); ;}
    break;

  case 53:
#line 508 "uda-rmt-server-parse.y"
    { (yyval.val) = 1; ;}
    break;

  case 54:
#line 510 "uda-rmt-server-parse.y"
    { (yyval.val) = (yyvsp[(1) - (1)].val); ;}
    break;

  case 55:
#line 515 "uda-rmt-server-parse.y"
    { (yyval.val2)[0] = 0; (yyval.val2)[1] = 1; ;}
    break;

  case 56:
#line 517 "uda-rmt-server-parse.y"
    { (yyval.val2)[0] = 0; (yyval.val2)[1] = (yyvsp[(2) - (2)].val); ;}
    break;

  case 57:
#line 519 "uda-rmt-server-parse.y"
    { (yyval.val2)[0] = (yyvsp[(2) - (4)].val); (yyval.val2)[1] = (yyvsp[(4) - (4)].val); ;}
    break;

  case 58:
#line 523 "uda-rmt-server-parse.y"
    { (yyval.signed_val) = 1; ;}
    break;

  case 59:
#line 524 "uda-rmt-server-parse.y"
    { (yyval.signed_val) = 1; ;}
    break;

  case 60:
#line 525 "uda-rmt-server-parse.y"
    { (yyval.signed_val) = -1; ;}
    break;

  case 65:
#line 546 "uda-rmt-server-parse.y"
    { (yyval.signed_val) = (yyvsp[(1) - (2)].signed_val) * (uda_tint_t)(yyvsp[(2) - (2)].val); ;}
    break;

  case 66:
#line 551 "uda-rmt-server-parse.y"
    {
	      (yyval.pts).addrfield = (yyvsp[(1) - (5)].addr_val);
	      (yyval.pts).thread = (yyvsp[(3) - (5)].val);
	      (yyval.pts).phase = (yyvsp[(5) - (5)].val);
	      (yyval.pts).opaque = 0;
	    ;}
    break;


/* Line 1267 of yacc.c.  */
#line 1891 "uda-rmt-server-parse.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 560 "uda-rmt-server-parse.y"


static const char *uda_server_cmd_yyline;
static const char *uda_server_cmd_yycp;
static const char *uda_server_cmd_yytext;
static size_t uda_server_cmd_yyleng;

typedef struct kw_entry_struct
  {
    const char *kw_id;
    const int kw_token;
  } kw_entry_t;

const kw_entry_t uda_server_cmd_yykw[] =
{
  {"address", KW_ADDRESS},
  {"len", KW_LEN},
  {"read", KW_READ},
  {"type", KW_TYPE},
  {"upc", KW_UPC},
  {"write", KW_WRITE},
  {"diff", KW_DIFF},
  {"index", KW_INDEX},
  {"pack", KW_PACK},
  {"pts", KW_PTS},
  {"shared", KW_SHARED},
  {"sizes", KW_SIZES},
  {"sym", KW_SYM},
  {"threads", KW_THREADS},
  {"unpack", KW_UNPACK},
};

#define NUM_KW (sizeof (uda_server_cmd_yykw) / sizeof (kw_entry_t))

#define init_scan(BUF) { uda_server_cmd_yyline = (BUF); uda_server_cmd_yycp = uda_server_cmd_yyline; uda_server_cmd_yysstate = scan_cmd; }
#define nextch() { if (*uda_server_cmd_yycp) { ++uda_server_cmd_yycp; ++uda_server_cmd_yyleng; } }
#define unch() { if (uda_server_cmd_yycp != uda_server_cmd_yyline) { --uda_server_cmd_yycp; --uda_server_cmd_yyleng; } }
#define reset_scan() { uda_server_cmd_yycp = uda_server_cmd_yytext; uda_server_cmd_yyleng = 0; }

int
uda_server_cmd_yylex()
{
  int ch_token;
  uda_server_cmd_yytext = uda_server_cmd_yycp;
  reset_scan ();
  switch (uda_server_cmd_yysstate)
  {
  case scan_cmd:
    {
      /* The first character is a special case, because it
         is used in the protocol to qualify the type of command. */
      if (uda_server_cmd_yycp != uda_server_cmd_yyline && isalpha (*uda_server_cmd_yycp))
        {
	  size_t i;
	  while (isalpha (*uda_server_cmd_yycp)) { nextch(); }
	  for (i = 0; i < NUM_KW; ++i)
	    {
	      if ((strlen (uda_server_cmd_yykw[i].kw_id) == uda_server_cmd_yyleng)
	          && (strncmp (uda_server_cmd_yytext, uda_server_cmd_yykw[i].kw_id, uda_server_cmd_yyleng) == 0))
		{
#ifdef DEBUG_LEX
		  printf ("scan: keyword: %s\n", uda_server_cmd_yykw[i].kw_id);
#endif
		  return uda_server_cmd_yykw[i].kw_token;
	        }
	    }
	  reset_scan ();
        }
      break;
    }
  case scan_args:
    {
      if (isxdigit (*uda_server_cmd_yycp))
        {
	  while (isxdigit (*uda_server_cmd_yycp)) { nextch (); }
          yylval.val = uda_decode_hex_word (uda_server_cmd_yytext, uda_server_cmd_yyleng);
#ifdef DEBUG_LEX
	  printf ("scan: hex: %lx\n", yylval.val);
#endif
	  return HEX_NUM;
	}
      break;
    }
  case scan_pts:
    {
      if (isxdigit (*uda_server_cmd_yycp))
        {
          const size_t n_bytes = uda_scan_hex_bytes (&uda_server_cmd_yycp);
	  uda_server_cmd_yyleng = uda_server_cmd_yycp - uda_server_cmd_yytext;
          yylval.data.bytes = (unsigned char *)uda_malloc(n_bytes);
          yylval.data.len = uda_decode_hex_bytes ((char *) yylval.data.bytes,
	                                           uda_server_cmd_yytext, uda_server_cmd_yyleng);
          uda_server_cmd_yysstate = scan_args;
          return BINARY_DATA;
	}
      break;
    }
  case scan_data:
    {
      const size_t n_bytes = uda_scan_binary_data (&uda_server_cmd_yycp);
      uda_server_cmd_yyleng = uda_server_cmd_yycp - uda_server_cmd_yytext;
      yylval.data.bytes = (unsigned char *)uda_malloc(n_bytes);
      yylval.data.len = uda_decode_binary_data ((char *) yylval.data.bytes,
                                                 uda_server_cmd_yytext, uda_server_cmd_yyleng);
      uda_server_cmd_yysstate = scan_args;
#ifdef DEBUG_LEX
      printf ("scan: biinary len: %ld\n", yylval.data.len);
#endif
      return BINARY_DATA;
    }
  case scan_id:
    {
      if (isalpha (*uda_server_cmd_yycp) || *uda_server_cmd_yycp == '$' || *uda_server_cmd_yycp == '_')
        {
	  nextch();
          while (isalnum (*uda_server_cmd_yycp) || *uda_server_cmd_yycp == '$' || *uda_server_cmd_yycp == '_'
	         || *uda_server_cmd_yycp == '-' || *uda_server_cmd_yycp == '.' || *uda_server_cmd_yycp == '@')
	    { nextch(); }
          yylval.str = (char *)uda_malloc(uda_server_cmd_yyleng + 1);
	  memcpy (yylval.str, uda_server_cmd_yytext, uda_server_cmd_yyleng);
	  yylval.str[uda_server_cmd_yyleng] = '\0';
          uda_server_cmd_yysstate = scan_args;
#ifdef DEBUG_LEX
	  printf ("scan: id: %s\n", yylval.str);
#endif
	  return ID_STRING;
	}
      break;
    }
  default:
    abort ();
  }
  ch_token = (*uda_server_cmd_yycp & 0xff);
  nextch ();
#ifdef DEBUG_LEX
  printf ("scan: char: '%c'\n", ch_token);
#endif
  return ch_token;
}

void
uda_server_cmd_yyerror (const char *msg)
{
  fprintf (stderr, "%s at '%s'\n", msg, uda_server_cmd_yycp);
}

int
uda_server_cmd_exec (const char *cmd)
{
  init_scan (cmd);
  return yyparse();
}

