/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         uda_server_cmd_yyparse
#define yylex           uda_server_cmd_yylex
#define yyerror         uda_server_cmd_yyerror
#define yylval          uda_server_cmd_yylval
#define yychar          uda_server_cmd_yychar
#define yydebug         uda_server_cmd_yydebug
#define yynerrs         uda_server_cmd_yynerrs

/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
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


/* Line 371 of yacc.c  */
#line 111 "uda-rmt-server-parse.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int uda_server_cmd_yydebug;
#endif

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


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 73 "uda-rmt-server-parse.y"

  uda_tword_t val;
  uda_tword_t val2[2];
  uda_tint_t signed_val;
  uda_taddr_t addr_val;
  uda_debugger_pts_t pts;
  char *str;
  uda_binary_data_t data;


/* Line 387 of yacc.c  */
#line 182 "uda-rmt-server-parse.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE uda_server_cmd_yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int uda_server_cmd_yyparse (void *YYPARSE_PARAM);
#else
int uda_server_cmd_yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int uda_server_cmd_yyparse (void);
#else
int uda_server_cmd_yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 210 "uda-rmt-server-parse.c"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

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

#if YYDEBUG || YYERROR_VERBOSE || 0
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
  "pointer_size", "short_size", "signed_hex_num", "unpacked_pts", YY_NULL
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

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
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
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
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

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-112)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
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
  FILE *yyo = yyoutput;
  YYUSE (yyo);
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
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
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
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
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




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

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
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
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

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
/* Line 1792 of yacc.c  */
#line 158 "uda-rmt-server-parse.y"
    {
	      int num_threads = (int)(yyvsp[(6) - (6)].val);
	      int status;
	      status = uda_server_set_num_threads_cmd (num_threads);
	      uda_rmt_send_status (status);
	    }
    break;

  case 15:
/* Line 1792 of yacc.c  */
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
	    }
    break;

  case 16:
/* Line 1792 of yacc.c  */
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
	    }
    break;

  case 17:
/* Line 1792 of yacc.c  */
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
	    }
    break;

  case 18:
/* Line 1792 of yacc.c  */
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
	    }
    break;

  case 19:
/* Line 1792 of yacc.c  */
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
	    }
    break;

  case 20:
/* Line 1792 of yacc.c  */
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
	    }
    break;

  case 21:
/* Line 1792 of yacc.c  */
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
	    }
    break;

  case 22:
/* Line 1792 of yacc.c  */
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
	    }
    break;

  case 23:
/* Line 1792 of yacc.c  */
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
	    }
    break;

  case 24:
/* Line 1792 of yacc.c  */
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
	    }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 402 "uda-rmt-server-parse.y"
    { uda_server_cmd_yysstate = scan_args; }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 406 "uda-rmt-server-parse.y"
    { uda_server_cmd_yysstate = scan_data; }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 410 "uda-rmt-server-parse.y"
    { uda_server_cmd_yysstate = scan_id; }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 430 "uda-rmt-server-parse.y"
    { (yyval.addr_val) = (uda_taddr_t)(yyvsp[(1) - (1)].val); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 434 "uda-rmt-server-parse.y"
    { uda_server_cmd_yysstate = scan_pts; }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 471 "uda-rmt-server-parse.y"
    { (yyval.addr_val) = 0; }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 473 "uda-rmt-server-parse.y"
    { (yyval.addr_val) = (yyvsp[(1) - (1)].addr_val); }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 478 "uda-rmt-server-parse.y"
    { (yyval.val) = 1; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 480 "uda-rmt-server-parse.y"
    { (yyval.val) = (yyvsp[(1) - (1)].val); }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 485 "uda-rmt-server-parse.y"
    { (yyval.val) = 1; }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 487 "uda-rmt-server-parse.y"
    { (yyval.val) = (yyvsp[(2) - (2)].val); }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 492 "uda-rmt-server-parse.y"
    { (yyval.val) = 1; }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 494 "uda-rmt-server-parse.y"
    { (yyval.val) = (yyvsp[(2) - (2)].val); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 499 "uda-rmt-server-parse.y"
    { (yyval.val2)[0] = 0; (yyval.val2)[1] = 1; }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 501 "uda-rmt-server-parse.y"
    { (yyval.val2)[0] = 0; (yyval.val2)[1] = (yyvsp[(2) - (2)].val); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 503 "uda-rmt-server-parse.y"
    { (yyval.val2)[0] = (yyvsp[(2) - (4)].val); (yyval.val2)[1] = (yyvsp[(4) - (4)].val); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 508 "uda-rmt-server-parse.y"
    { (yyval.val) = 1; }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 510 "uda-rmt-server-parse.y"
    { (yyval.val) = (yyvsp[(1) - (1)].val); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 515 "uda-rmt-server-parse.y"
    { (yyval.val2)[0] = 0; (yyval.val2)[1] = 1; }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 517 "uda-rmt-server-parse.y"
    { (yyval.val2)[0] = 0; (yyval.val2)[1] = (yyvsp[(2) - (2)].val); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 519 "uda-rmt-server-parse.y"
    { (yyval.val2)[0] = (yyvsp[(2) - (4)].val); (yyval.val2)[1] = (yyvsp[(4) - (4)].val); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 523 "uda-rmt-server-parse.y"
    { (yyval.signed_val) = 1; }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 524 "uda-rmt-server-parse.y"
    { (yyval.signed_val) = 1; }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 525 "uda-rmt-server-parse.y"
    { (yyval.signed_val) = -1; }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 546 "uda-rmt-server-parse.y"
    { (yyval.signed_val) = (yyvsp[(1) - (2)].signed_val) * (uda_tint_t)(yyvsp[(2) - (2)].val); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 551 "uda-rmt-server-parse.y"
    {
	      (yyval.pts).addrfield = (yyvsp[(1) - (5)].addr_val);
	      (yyval.pts).thread = (yyvsp[(3) - (5)].val);
	      (yyval.pts).phase = (yyvsp[(5) - (5)].val);
	      (yyval.pts).opaque = 0;
	    }
    break;


/* Line 1792 of yacc.c  */
#line 1931 "uda-rmt-server-parse.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
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
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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

  /* Else will try to reuse lookahead token after shifting the error
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
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
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


/* Line 2055 of yacc.c  */
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
