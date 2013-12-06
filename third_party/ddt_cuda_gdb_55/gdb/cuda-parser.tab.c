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
#define yyparse cuda_parser_parse
#define yylex   cuda_parser_lex
#define yyerror cuda_parser_error
#define yylval  cuda_parser_lval
#define yychar  cuda_parser_char
#define yydebug cuda_parser_debug
#define yynerrs cuda_parser_nerrs


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




/* Copy the first part of user declarations.  */
#line 18 "cuda-parser.exp.y"


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "cuda-parser.h"

/*Input string */
struct {
  char     *buffer;  // copy of the string to parse
  uint32_t  size;    // size of the buffer in bytes
  char     *ptr;     // ptr to the next chunk to parse within the string
} cuda_parser_input;

/*Intermediate Buffers */
static request_value_t value_buffer;

/*Detect when the same coordinate type is specified more than one */
filter_type_t handled_filters;

/*Default value when a coordinate is omitted */
cuda_coords_special_value_t default_value;

/*Command type (to decide which parser to use) */
command_t start_token; /* read once, then set to CMD_NONE. See Bison manual 11.5. */

/*Results */
static cuda_parser_result_t  result;

static void handle_command (command_t cmd);
static void handle_query (filter_type_t type);
static void handle_condition (filter_type_t type, compare_t cmp);
static void handle_switch (filter_type_t type);
static void handle_filter (filter_type_t type);
static void handle_scalar (uint32_t index, uint32_t value);

int  cuda_parser_lex (void);
int  cuda_parser_get_next_input (char *buf, int max_size);
void cuda_parser_error (char *msg);
void cuda_parser_reset_lexer (void);



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
#line 64 "cuda-parser.exp.y"
{
    uint32_t  value;
    compare_t cmp;
  }
/* Line 187 of yacc.c.  */
#line 218 "cuda-parser.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 231 "cuda-parser.tab.c"

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
#define YYFINAL  48
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   126

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  26
/* YYNRULES -- Number of rules.  */
#define YYNRULES  78
/* YYNRULES -- Number of states.  */
#define YYNSTATES  128

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   287

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     9,    12,    15,    18,    21,    23,
      26,    28,    30,    32,    34,    36,    38,    40,    42,    44,
      47,    50,    53,    56,    59,    62,    65,    68,    71,    74,
      76,    79,    81,    83,    85,    87,    89,    91,    93,    95,
      97,   101,   105,   108,   109,   113,   117,   121,   125,   129,
     133,   135,   138,   141,   144,   147,   150,   153,   156,   159,
     162,   166,   168,   172,   174,   180,   184,   186,   188,   190,
     192,   194,   196,   198,   200,   202,   204,   206,   208
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      34,     0,    -1,     3,    46,    -1,     4,    48,    -1,     4,
      49,    -1,     3,    51,    -1,     5,    35,    -1,     6,    41,
      -1,    36,    -1,    35,    36,    -1,    37,    -1,    38,    -1,
      39,    -1,    40,    -1,    41,    -1,    42,    -1,    43,    -1,
      44,    -1,    45,    -1,     7,    54,    -1,     8,    54,    -1,
       9,    54,    -1,    10,    54,    -1,    11,    54,    -1,    12,
      54,    -1,    13,    53,    -1,    14,    53,    -1,    21,    24,
      -1,    21,    54,    -1,    47,    -1,    46,    47,    -1,     7,
      -1,     8,    -1,     9,    -1,    10,    -1,    11,    -1,    12,
      -1,    13,    -1,    14,    -1,    50,    -1,    48,    30,    50,
      -1,    50,    31,    50,    -1,    49,    31,    -1,    -1,    15,
      26,    54,    -1,    16,    26,    54,    -1,    17,    26,    54,
      -1,    18,    26,    54,    -1,    19,    26,    54,    -1,    20,
      26,    54,    -1,    52,    -1,    51,    52,    -1,     7,    54,
      -1,     8,    54,    -1,     9,    54,    -1,    10,    54,    -1,
      11,    54,    -1,    12,    54,    -1,    13,    53,    -1,    14,
      53,    -1,    28,    55,    29,    -1,    55,    -1,    28,    56,
      29,    -1,    56,    -1,    56,    27,    57,    27,    58,    -1,
      56,    27,    57,    -1,    56,    -1,    25,    -1,    22,    -1,
      23,    -1,    24,    -1,    25,    -1,    22,    -1,    23,    -1,
      24,    -1,    25,    -1,    22,    -1,    23,    -1,    24,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    85,    85,    86,    87,    88,    89,    90,    93,    94,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   108,
     110,   112,   114,   116,   118,   120,   122,   124,   126,   129,
     130,   133,   134,   135,   136,   137,   138,   139,   140,   143,
     144,   147,   148,   148,   151,   152,   153,   154,   155,   156,
     159,   160,   163,   164,   165,   166,   167,   168,   169,   170,
     173,   174,   177,   178,   181,   182,   183,   186,   187,   188,
     189,   192,   193,   194,   195,   198,   199,   200,   201
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "START_QUERY_OR_SWITCH",
  "START_CONDITIONS", "START_FILTER", "START_FILTER_KERNEL", "DEVICE",
  "SM", "WARP", "LANE", "KERNEL", "GRID", "BLOCK", "THREAD", "BLOCKIDX_X",
  "BLOCKIDX_Y", "BLOCKIDX_Z", "THREADIDX_X", "THREADIDX_Y", "THREADIDX_Z",
  "BREAKPOINT", "CURRENT", "WILDCARD", "ALL", "VALUE", "CMP", "COMMA",
  "OPENPAR", "CLOSEPAR", "LOGICAL_AND", "LOGICAL_OR", "PARSING_ERROR",
  "$accept", "command", "filters", "filter", "device", "sm", "warp",
  "lane", "kernel", "grid", "block", "thread", "breakpoint", "queries",
  "query", "and_conditions", "or_conditions", "condition", "switches",
  "switch", "cudim3", "scalar", "triplet", "x", "y", "z", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    33,    34,    34,    34,    34,    34,    34,    35,    35,
      36,    36,    36,    36,    36,    36,    36,    36,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    45,    46,
      46,    47,    47,    47,    47,    47,    47,    47,    47,    48,
      48,    49,    49,    50,    50,    50,    50,    50,    50,    50,
      51,    51,    52,    52,    52,    52,    52,    52,    52,    52,
      53,    53,    54,    54,    55,    55,    55,    56,    56,    56,
      56,    57,    57,    57,    57,    58,    58,    58,    58
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     2,     0,     3,     3,     3,     3,     3,     3,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       3,     1,     3,     1,     5,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    43,     0,     0,     0,    31,    32,    33,    34,
      35,    36,    37,    38,     2,    29,     5,    50,     0,     0,
       0,     0,     0,     0,     3,     4,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     6,     8,    10,    11,
      12,    13,    14,    15,    16,    17,    18,     7,     1,    68,
      69,    70,    67,     0,    52,    63,    53,    54,    55,    56,
      57,     0,    58,    61,    66,    59,    31,    32,    33,    34,
      35,    36,    37,    38,    30,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
      43,    42,    43,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,     9,     0,     0,     0,    44,    45,    46,
      47,    48,    49,    40,    41,    62,    60,    72,    73,    74,
      71,    65,     0,    76,    77,    78,    75,    64
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     5,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    14,    15,    24,    25,    26,    16,    17,
      62,    54,    63,    55,   121,   127
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -75
static const yytype_int8 yypact[] =
{
      61,    32,    14,    -2,    15,    13,    34,    34,    34,    34,
      34,    34,    73,    73,    40,   -75,    80,   -75,     1,    35,
      42,    48,    49,    50,    30,    54,    68,    34,    34,    34,
      34,    34,    34,    73,    73,    81,    -2,   -75,   -75,   -75,
     -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,   -75,   -75,    88,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,    88,   -75,   -75,    75,   -75,   -75,   -75,   -75,   -75,
     -75,   -75,   -75,   -75,   -75,    34,    34,    34,    34,    34,
      34,    73,    73,   -75,    34,    34,    34,    34,    34,    34,
      14,   -75,    14,   -75,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,   -75,   -75,   -75,    71,    78,    92,   -75,   -75,   -75,
     -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,    95,    96,   -75,   -75,   -75,   -75,   -75
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -75,   -75,   -75,    72,   -75,   -75,   -75,   -75,   119,   -75,
     -75,   -75,   -75,   -75,   110,   -75,   -75,   -74,   -75,   109,
       4,    -7,    65,     2,   -75,   -75
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      56,    57,    58,    59,    60,    27,    28,    29,    30,    31,
      32,    33,    34,    48,    64,    64,   113,    65,   114,    35,
      93,    94,    95,    96,    97,    98,    31,    84,   102,    18,
      19,    20,    21,    22,    23,    64,    64,    99,   100,     6,
       7,     8,     9,    10,    11,    12,    13,    66,    67,    68,
      69,    70,    71,    72,    73,   104,    49,    50,    51,    52,
      90,    85,    53,    64,     1,     2,     3,     4,    86,    56,
      57,    58,    59,    60,    87,    88,    89,   107,   108,   109,
     110,   111,   112,    64,    64,    91,    65,    75,    76,    77,
      78,    79,    80,    81,    82,    49,    50,    51,    52,    92,
     115,    61,   106,    49,    50,   101,    52,   116,   103,    53,
      49,    50,    51,    52,   117,   118,   119,   120,   123,   124,
     125,   126,   122,    47,    74,    83,   105
};

static const yytype_uint8 yycheck[] =
{
       7,     8,     9,    10,    11,     7,     8,     9,    10,    11,
      12,    13,    14,     0,    12,    13,    90,    13,    92,    21,
      27,    28,    29,    30,    31,    32,    11,    26,    35,    15,
      16,    17,    18,    19,    20,    33,    34,    33,    34,     7,
       8,     9,    10,    11,    12,    13,    14,     7,     8,     9,
      10,    11,    12,    13,    14,    53,    22,    23,    24,    25,
      30,    26,    28,    61,     3,     4,     5,     6,    26,    76,
      77,    78,    79,    80,    26,    26,    26,    84,    85,    86,
      87,    88,    89,    81,    82,    31,    82,     7,     8,     9,
      10,    11,    12,    13,    14,    22,    23,    24,    25,    31,
      29,    28,    27,    22,    23,    24,    25,    29,    36,    28,
      22,    23,    24,    25,    22,    23,    24,    25,    22,    23,
      24,    25,    27,     4,    14,    16,    61
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,    34,     7,     8,     9,    10,
      11,    12,    13,    14,    46,    47,    51,    52,    15,    16,
      17,    18,    19,    20,    48,    49,    50,     7,     8,     9,
      10,    11,    12,    13,    14,    21,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    41,     0,    22,
      23,    24,    25,    28,    54,    56,    54,    54,    54,    54,
      54,    28,    53,    55,    56,    53,     7,     8,     9,    10,
      11,    12,    13,    14,    47,     7,     8,     9,    10,    11,
      12,    13,    14,    52,    26,    26,    26,    26,    26,    26,
      30,    31,    31,    54,    54,    54,    54,    54,    54,    53,
      53,    24,    54,    36,    56,    55,    27,    54,    54,    54,
      54,    54,    54,    50,    50,    29,    29,    22,    23,    24,
      25,    57,    27,    22,    23,    24,    25,    58
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
        case 2:
#line 85 "cuda-parser.exp.y"
    { handle_command (CMD_QUERY); }
    break;

  case 3:
#line 86 "cuda-parser.exp.y"
    { handle_command (CMD_COND_AND); }
    break;

  case 4:
#line 87 "cuda-parser.exp.y"
    { handle_command (CMD_COND_OR); }
    break;

  case 5:
#line 88 "cuda-parser.exp.y"
    { handle_command (CMD_SWITCH); }
    break;

  case 6:
#line 89 "cuda-parser.exp.y"
    { handle_command (CMD_FILTER); }
    break;

  case 7:
#line 90 "cuda-parser.exp.y"
    { handle_command (CMD_FILTER_KERNEL); }
    break;

  case 19:
#line 108 "cuda-parser.exp.y"
    { handle_filter (FILTER_TYPE_DEVICE); }
    break;

  case 20:
#line 110 "cuda-parser.exp.y"
    { handle_filter (FILTER_TYPE_SM); }
    break;

  case 21:
#line 112 "cuda-parser.exp.y"
    { handle_filter (FILTER_TYPE_WARP); }
    break;

  case 22:
#line 114 "cuda-parser.exp.y"
    { handle_filter (FILTER_TYPE_LANE); }
    break;

  case 23:
#line 116 "cuda-parser.exp.y"
    { handle_filter (FILTER_TYPE_KERNEL); }
    break;

  case 24:
#line 118 "cuda-parser.exp.y"
    { handle_filter (FILTER_TYPE_GRID); }
    break;

  case 25:
#line 120 "cuda-parser.exp.y"
    { handle_filter (FILTER_TYPE_BLOCK); }
    break;

  case 26:
#line 122 "cuda-parser.exp.y"
    { handle_filter (FILTER_TYPE_THREAD); }
    break;

  case 27:
#line 124 "cuda-parser.exp.y"
    { handle_filter (FILTER_TYPE_BREAKPOINT); handle_scalar (0, 0); }
    break;

  case 28:
#line 126 "cuda-parser.exp.y"
    { handle_filter (FILTER_TYPE_BREAKPOINT); }
    break;

  case 31:
#line 133 "cuda-parser.exp.y"
    { handle_query (FILTER_TYPE_DEVICE); }
    break;

  case 32:
#line 134 "cuda-parser.exp.y"
    { handle_query (FILTER_TYPE_SM); }
    break;

  case 33:
#line 135 "cuda-parser.exp.y"
    { handle_query (FILTER_TYPE_WARP); }
    break;

  case 34:
#line 136 "cuda-parser.exp.y"
    { handle_query (FILTER_TYPE_LANE); }
    break;

  case 35:
#line 137 "cuda-parser.exp.y"
    { handle_query (FILTER_TYPE_KERNEL); }
    break;

  case 36:
#line 138 "cuda-parser.exp.y"
    { handle_query (FILTER_TYPE_GRID); }
    break;

  case 37:
#line 139 "cuda-parser.exp.y"
    { handle_query (FILTER_TYPE_BLOCK); }
    break;

  case 38:
#line 140 "cuda-parser.exp.y"
    { handle_query (FILTER_TYPE_THREAD); }
    break;

  case 44:
#line 151 "cuda-parser.exp.y"
    { handle_condition (FILTER_TYPE_BLOCKIDX_X, (yyvsp[(2) - (3)].cmp)); }
    break;

  case 45:
#line 152 "cuda-parser.exp.y"
    { handle_condition (FILTER_TYPE_BLOCKIDX_Y, (yyvsp[(2) - (3)].cmp)); }
    break;

  case 46:
#line 153 "cuda-parser.exp.y"
    { handle_condition (FILTER_TYPE_BLOCKIDX_Z, (yyvsp[(2) - (3)].cmp)); }
    break;

  case 47:
#line 154 "cuda-parser.exp.y"
    { handle_condition (FILTER_TYPE_THREADIDX_X, (yyvsp[(2) - (3)].cmp)); }
    break;

  case 48:
#line 155 "cuda-parser.exp.y"
    { handle_condition (FILTER_TYPE_THREADIDX_Y, (yyvsp[(2) - (3)].cmp)); }
    break;

  case 49:
#line 156 "cuda-parser.exp.y"
    { handle_condition (FILTER_TYPE_THREADIDX_Z, (yyvsp[(2) - (3)].cmp)); }
    break;

  case 52:
#line 163 "cuda-parser.exp.y"
    { handle_switch (FILTER_TYPE_DEVICE); }
    break;

  case 53:
#line 164 "cuda-parser.exp.y"
    { handle_switch (FILTER_TYPE_SM); }
    break;

  case 54:
#line 165 "cuda-parser.exp.y"
    { handle_switch (FILTER_TYPE_WARP); }
    break;

  case 55:
#line 166 "cuda-parser.exp.y"
    { handle_switch (FILTER_TYPE_LANE); }
    break;

  case 56:
#line 167 "cuda-parser.exp.y"
    { handle_switch (FILTER_TYPE_KERNEL); }
    break;

  case 57:
#line 168 "cuda-parser.exp.y"
    { handle_switch (FILTER_TYPE_GRID); }
    break;

  case 58:
#line 169 "cuda-parser.exp.y"
    { handle_switch (FILTER_TYPE_BLOCK); }
    break;

  case 59:
#line 170 "cuda-parser.exp.y"
    { handle_switch (FILTER_TYPE_THREAD); }
    break;

  case 64:
#line 181 "cuda-parser.exp.y"
    { ; }
    break;

  case 65:
#line 182 "cuda-parser.exp.y"
    { handle_scalar (2, default_value); }
    break;

  case 66:
#line 183 "cuda-parser.exp.y"
    { handle_scalar (1, default_value); handle_scalar (2, default_value); }
    break;

  case 67:
#line 186 "cuda-parser.exp.y"
    { handle_scalar (0, (yyvsp[(1) - (1)].value)); }
    break;

  case 68:
#line 187 "cuda-parser.exp.y"
    { handle_scalar (0, CUDA_CURRENT); }
    break;

  case 69:
#line 188 "cuda-parser.exp.y"
    { handle_scalar (0, CUDA_WILDCARD); }
    break;

  case 70:
#line 189 "cuda-parser.exp.y"
    { handle_scalar (0, CUDA_WILDCARD); }
    break;

  case 71:
#line 192 "cuda-parser.exp.y"
    { handle_scalar (1, (yyvsp[(1) - (1)].value)); }
    break;

  case 72:
#line 193 "cuda-parser.exp.y"
    { handle_scalar (1, CUDA_CURRENT); }
    break;

  case 73:
#line 194 "cuda-parser.exp.y"
    { handle_scalar (1, CUDA_WILDCARD); }
    break;

  case 74:
#line 195 "cuda-parser.exp.y"
    { handle_scalar (1, CUDA_WILDCARD); }
    break;

  case 75:
#line 198 "cuda-parser.exp.y"
    { handle_scalar (2, (yyvsp[(1) - (1)].value)); }
    break;

  case 76:
#line 199 "cuda-parser.exp.y"
    { handle_scalar (2, CUDA_CURRENT); }
    break;

  case 77:
#line 200 "cuda-parser.exp.y"
    { handle_scalar (2, CUDA_WILDCARD); }
    break;

  case 78:
#line 201 "cuda-parser.exp.y"
    { handle_scalar (2, CUDA_WILDCARD); }
    break;


/* Line 1267 of yacc.c.  */
#line 1801 "cuda-parser.tab.c"
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


#line 204 "cuda-parser.exp.y"


static void
new_request (filter_type_t type, request_value_t value, compare_t cmp)
{
  request_t *request;

  ++result.num_requests;

  if (result.num_requests > result.max_requests)
    {
      if (result.max_requests == 0)
        result.max_requests = 4;
      else
        result.max_requests *= 2;

      result.requests = xrealloc (result.requests, result.max_requests * sizeof (*request));
    }

  request = &result.requests[result.num_requests-1];
  request->type  = type;
  request->value = value;
  request->cmp   = cmp;
}

static void
reset_value_buffer (void)
{
  memset (&value_buffer, ~0, sizeof (value_buffer));
}

static void
handle_command (command_t cmd)
{
  if (result.command != CMD_ERROR)
    result.command = cmd;
}

static void
handle_query (filter_type_t type)
{
  if (handled_filters & type)
    cuda_parser_error ("Duplicate filter type");
  handled_filters |= type;
  value_buffer.cudim3 = (CuDim3) {CUDA_CURRENT, CUDA_CURRENT, CUDA_CURRENT};
  new_request (type, value_buffer, CMP_NONE);
  reset_value_buffer ();
}

static void
handle_condition (filter_type_t type, compare_t cmp)
{
  new_request (type, value_buffer, cmp);
  reset_value_buffer ();
}

static void
handle_switch (filter_type_t type)
{
  if (handled_filters & type)
    cuda_parser_error ("Duplicate filter type");
  handled_filters |= type;
  new_request (type, value_buffer, CMP_NONE);
  reset_value_buffer ();
}

static void
handle_filter (filter_type_t type)
{
  if (handled_filters & type)
    cuda_parser_error ("Duplicate filter type");
  handled_filters |= type;
  new_request (type, value_buffer, CMP_NONE);
  reset_value_buffer ();
}

static void
handle_scalar (uint32_t index, uint32_t value)
{
  switch (index)
    {
    case 0: value_buffer.cudim3.x = value; break;
    case 1: value_buffer.cudim3.y = value; break;
    case 2: value_buffer.cudim3.z = value; break;
    default: cuda_parser_error ("Unexpected index in handle_scalar.");
    }
}

static void
print_coord  (uint32_t value)
{
  if (value == ~0)
    printf ("x");
  else
    printf ("%u", value);
}

static void
print_request (request_t *request)
{
  if (request->type & FILTER_TYPE_DEVICE) printf ("device");
  if (request->type & FILTER_TYPE_SM)     printf ("sm");
  if (request->type & FILTER_TYPE_WARP)   printf ("warp");
  if (request->type & FILTER_TYPE_LANE)   printf ("lane");
  if (request->type & FILTER_TYPE_KERNEL) printf ("kernel");
  if (request->type & FILTER_TYPE_GRID)   printf ("grid");
  if (request->type & FILTER_TYPE_BLOCK)  printf ("block");
  if (request->type & FILTER_TYPE_THREAD) printf ("thread");
  if (request->type & FILTER_TYPE_BLOCKIDX_X)  printf ("blockIdx.x");
  if (request->type & FILTER_TYPE_BLOCKIDX_Y)  printf ("blockIdx.y");
  if (request->type & FILTER_TYPE_BLOCKIDX_Z)  printf ("blockIdx.z");
  if (request->type & FILTER_TYPE_THREADIDX_X) printf ("threadIdx.z");
  if (request->type & FILTER_TYPE_THREADIDX_Y) printf ("threadIdx.y");
  if (request->type & FILTER_TYPE_THREADIDX_Z) printf ("threadIdx.z");
  if (request->type & FILTER_TYPE_BREAKPOINT) printf ("breakpoint");

  if (request->cmp == CMP_EQ) printf (" == ");
  if (request->cmp == CMP_NE) printf (" != ");
  if (request->cmp == CMP_LT) printf (" < ");
  if (request->cmp == CMP_GT) printf (" > ");
  if (request->cmp == CMP_LE) printf (" <= ");
  if (request->cmp == CMP_GE) printf (" >= ");
  if (request->cmp == CMP_NONE) printf (" ");

  if (request->type & FILTER_TYPE_THREAD ||
      request->type & FILTER_TYPE_BLOCK)
    {
      printf ("(");
      print_coord (request->value.cudim3.x);
      printf (",");
      print_coord (request->value.cudim3.y);
      printf (",");
      print_coord (request->value.cudim3.z);
      printf (")");
    }
  else
    print_coord (request->value.scalar);

  printf ("\n");
}


void
cuda_parser_error (char *msg)
{
  //fprintf(stderr, "CUDA parser error: %s\n", msg);
  result.command = CMD_ERROR;
}

void
cuda_parser_initialize (const char *input, command_t command,
                        cuda_coords_special_value_t dflt_value)
{
  uint32_t size = strlen (input) + 1;

  /* Initialized the result */
  result.command = CMD_NONE;
  result.num_requests = 0;
  result.max_requests = 0;
  result.requests = NULL;

  /* Initialize the input buffer */
  if (cuda_parser_input.buffer)
    xfree (cuda_parser_input.buffer);
  cuda_parser_input.buffer = xmalloc (size);
  cuda_parser_input.size   = size - 1; // do not pass the '\0' character
  cuda_parser_input.ptr    = memcpy (cuda_parser_input.buffer, input, size);

  /* Initialize the temporary buffer */
  reset_value_buffer ();

  /* Initialize the sanity check variables */
  handled_filters = FILTER_TYPE_NONE;

  /* Initialize the requested command */
  if (command & CMD_SWITCH ||
      command & CMD_QUERY ||
      command & CMD_FILTER ||
      command & CMD_FILTER_KERNEL)
{
    start_token = command;
}
  else
    cuda_parser_error ("Incorrect requested command\n");

  /* Initialize the default value */
  default_value = dflt_value;
}

int
cuda_parser_get_next_input (char *buf, int max_size)
{
  int remaining_size = cuda_parser_input.buffer + cuda_parser_input.size - cuda_parser_input.ptr;
  int copy_size = remaining_size > max_size ? max_size : remaining_size;

  if (copy_size == 0)
    return 0;

  memcpy (buf, cuda_parser_input.ptr, copy_size);
  cuda_parser_input.ptr += copy_size;
  return copy_size;
}

void
cuda_parser (const char *input, command_t command, cuda_parser_result_t **res,
             cuda_coords_special_value_t dflt_value)
{
  cuda_parser_reset_lexer ();
  cuda_parser_initialize (input, command, dflt_value);
  cuda_parser_parse ();

  *res = &result;
}

void
cuda_parser_print (cuda_parser_result_t *result)
{
  request_t *request;
  uint32_t i;

  printf ("Command: ");
  switch (result->command)
    {
    case CMD_NONE: printf ("none"); break;
    case CMD_ERROR: printf ("error"); break;
    case CMD_QUERY: printf ("query"); break;
    case CMD_COND_AND: printf ("and conditions"); break;
    case CMD_COND_OR: printf ("or conditions"); break;
    case CMD_SWITCH: printf ("switch"); break;
    default: break;
    }
  printf ("\n");

  if (result->command == CMD_ERROR)
    return;

  for (i = 0; i < result->num_requests; ++i)
    {
      request = &result->requests[i];
      if (i > 0)
        if (result->command == CMD_COND_AND)
          printf (" && ");
        else if (result->command == CMD_COND_OR)
          printf (" || ");
        else
          printf ("    ");
      else
        printf ("    ");
      print_request (request);
    }
}

