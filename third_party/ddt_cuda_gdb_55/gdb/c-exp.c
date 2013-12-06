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



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     FLOAT = 259,
     DECFLOAT = 260,
     STRING = 261,
     CHAR = 262,
     NAME = 263,
     UNKNOWN_CPP_NAME = 264,
     COMPLETE = 265,
     TYPENAME = 266,
     NAME_OR_INT = 267,
     OPERATOR = 268,
     STRUCT = 269,
     CLASS = 270,
     UNION = 271,
     ENUM = 272,
     SIZEOF = 273,
     UNSIGNED = 274,
     COLONCOLON = 275,
     TEMPLATE = 276,
     ERROR = 277,
     NEW = 278,
     DELETE = 279,
     REINTERPRET_CAST = 280,
     DYNAMIC_CAST = 281,
     STATIC_CAST = 282,
     CONST_CAST = 283,
     SIGNED_KEYWORD = 284,
     LONG = 285,
     SHORT = 286,
     INT_KEYWORD = 287,
     CONST_KEYWORD = 288,
     VOLATILE_KEYWORD = 289,
     DOUBLE_KEYWORD = 290,
     VOID_KEYWORD = 291,
     SHARED_KEYWORD = 292,
     RELAXED_KEYWORD = 293,
     STRICT_KEYWORD = 294,
     VARIABLE = 295,
     ASSIGN_MODIFY = 296,
     UNOP_INTRINSIC = 297,
     BINOP_INTRINSIC = 298,
     TRUEKEYWORD = 299,
     FALSEKEYWORD = 300,
     ABOVE_COMMA = 301,
     OROR = 302,
     ANDAND = 303,
     NOTEQUAL = 304,
     EQUAL = 305,
     GEQ = 306,
     LEQ = 307,
     RSH = 308,
     LSH = 309,
     DECREMENT = 310,
     INCREMENT = 311,
     UNARY = 312,
     DOT_STAR = 313,
     ARROW_STAR = 314,
     ARROW = 315,
     BLOCKNAME = 316,
     FILENAME = 317
   };
#endif
/* Tokens.  */
#define INT 258
#define FLOAT 259
#define DECFLOAT 260
#define STRING 261
#define CHAR 262
#define NAME 263
#define UNKNOWN_CPP_NAME 264
#define COMPLETE 265
#define TYPENAME 266
#define NAME_OR_INT 267
#define OPERATOR 268
#define STRUCT 269
#define CLASS 270
#define UNION 271
#define ENUM 272
#define SIZEOF 273
#define UNSIGNED 274
#define COLONCOLON 275
#define TEMPLATE 276
#define ERROR 277
#define NEW 278
#define DELETE 279
#define REINTERPRET_CAST 280
#define DYNAMIC_CAST 281
#define STATIC_CAST 282
#define CONST_CAST 283
#define SIGNED_KEYWORD 284
#define LONG 285
#define SHORT 286
#define INT_KEYWORD 287
#define CONST_KEYWORD 288
#define VOLATILE_KEYWORD 289
#define DOUBLE_KEYWORD 290
#define VOID_KEYWORD 291
#define SHARED_KEYWORD 292
#define RELAXED_KEYWORD 293
#define STRICT_KEYWORD 294
#define VARIABLE 295
#define ASSIGN_MODIFY 296
#define UNOP_INTRINSIC 297
#define BINOP_INTRINSIC 298
#define TRUEKEYWORD 299
#define FALSEKEYWORD 300
#define ABOVE_COMMA 301
#define OROR 302
#define ANDAND 303
#define NOTEQUAL 304
#define EQUAL 305
#define GEQ 306
#define LEQ 307
#define RSH 308
#define LSH 309
#define DECREMENT 310
#define INCREMENT 311
#define UNARY 312
#define DOT_STAR 313
#define ARROW_STAR 314
#define ARROW 315
#define BLOCKNAME 316
#define FILENAME 317




/* Copy the first part of user declarations.  */
#line 56 "c-exp.y"


#include "defs.h"
#include "gdb_string.h"
#include <ctype.h>
#include "expression.h"
#include "value.h"
#include "parser-defs.h"
#include "language.h"
#include "c-lang.h"
#include "bfd.h" /* Required by objfiles.h.  */
#include "symfile.h" /* Required by objfiles.h.  */
#include "objfiles.h" /* For have_full_symbols and have_partial_symbols */
#include "charset.h"
#include "block.h"
#include "cp-support.h"
#include "dfp.h"
#include "gdb_assert.h"
#include "macroscope.h"

#define parse_type builtin_type (parse_gdbarch)

/* Remap normal yacc parser interface names (yyparse, yylex, yyerror, etc),
   as well as gratuitiously global symbol names, so we can have multiple
   yacc generated parsers in gdb.  Note that these are only the variables
   produced by yacc.  If other parser generators (bison, byacc, etc) produce
   additional global names that conflict at link time, then those parser
   generators need to be fixed instead of adding those names to this list. */

#define	yymaxdepth c_maxdepth
#define	yyparse	c_parse_internal
#define	yylex	c_lex
#define	yyerror	c_error
#define	yylval	c_lval
#define	yychar	c_char
#define	yydebug	c_debug
#define	yypact	c_pact	
#define	yyr1	c_r1			
#define	yyr2	c_r2			
#define	yydef	c_def		
#define	yychk	c_chk		
#define	yypgo	c_pgo		
#define	yyact	c_act		
#define	yyexca	c_exca
#define yyerrflag c_errflag
#define yynerrs	c_nerrs
#define	yyps	c_ps
#define	yypv	c_pv
#define	yys	c_s
#define	yy_yys	c_yys
#define	yystate	c_state
#define	yytmp	c_tmp
#define	yyv	c_v
#define	yy_yyv	c_yyv
#define	yyval	c_val
#define	yylloc	c_lloc
#define yyreds	c_reds		/* With YYDEBUG defined */
#define yytoks	c_toks		/* With YYDEBUG defined */
#define yyname	c_name		/* With YYDEBUG defined */
#define yyrule	c_rule		/* With YYDEBUG defined */
#define yylhs	c_yylhs
#define yylen	c_yylen
#define yydefred c_yydefred
#define yydgoto	c_yydgoto
#define yysindex c_yysindex
#define yyrindex c_yyrindex
#define yygindex c_yygindex
#define yytable	 c_yytable
#define yycheck	 c_yycheck

#ifndef YYDEBUG
#define	YYDEBUG 1		/* Default to yydebug support */
#endif

#define YYFPRINTF parser_fprintf

int yyparse (void);

static int yylex (void);

void yyerror (char *);



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
#line 145 "c-exp.y"
{
    LONGEST lval;
    struct {
      LONGEST val;
      struct type *type;
    } typed_val_int;
    struct {
      DOUBLEST dval;
      struct type *type;
    } typed_val_float;
    struct {
      gdb_byte val[16];
      struct type *type;
    } typed_val_decfloat;
    struct symbol *sym;
    struct type *tval;
    struct stoken sval;
    struct typed_stoken tsval;
    struct ttype tsym;
    struct symtoken ssym;
    int voidval;
    struct block *bval;
    enum exp_opcode opcode;
    struct internalvar *ivar;

    struct stoken_vector svec;
    struct type **tvec;
    int *ivec;
  }
/* Line 187 of yacc.c.  */
#line 334 "c-exp.c.tmp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
#line 175 "c-exp.y"

/* YYSTYPE gets defined by %union */
static int parse_number (char *, int, int, YYSTYPE *);
static struct stoken operator_stoken (const char *);


/* Line 216 of yacc.c.  */
#line 352 "c-exp.c.tmp"

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

/* The parser invokes alloca or xmalloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
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
       && ! ((defined YYMALLOC || defined xmalloc) \
	     && (defined YYFREE || defined xfree)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC xmalloc
#   if ! defined xmalloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *xmalloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE xfree
#   if ! defined xfree && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void xfree (void *); /* INFRINGES ON USER NAME SPACE */
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
#define YYFINAL  164
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1192

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  87
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  224
/* YYNRULES -- Number of states.  */
#define YYNSTATES  349

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   317

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    80,     2,     2,     2,    68,    54,     2,
      74,    83,    66,    64,    46,    65,    72,    67,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    86,     2,
      57,    48,    58,    49,    63,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    73,     2,    82,    53,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    84,    52,    85,    81,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    47,    50,    51,    55,    56,    59,    60,    61,    62,
      69,    70,    71,    75,    76,    77,    78,    79
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    15,    18,    21,
      24,    27,    30,    33,    36,    39,    42,    45,    48,    52,
      57,    61,    65,    69,    73,    78,    82,    86,    90,    95,
      96,   102,   103,   109,   114,   121,   123,   124,   126,   130,
     136,   138,   142,   147,   152,   156,   160,   164,   168,   172,
     176,   180,   184,   188,   192,   196,   200,   204,   208,   212,
     216,   220,   224,   228,   232,   238,   242,   246,   248,   250,
     252,   254,   256,   258,   260,   265,   273,   281,   289,   297,
     299,   302,   304,   306,   308,   310,   312,   316,   320,   324,
     329,   335,   337,   340,   342,   344,   345,   347,   348,   350,
     354,   357,   359,   362,   364,   367,   369,   373,   376,   378,
     381,   383,   386,   390,   393,   397,   399,   401,   403,   405,
     407,   410,   414,   417,   421,   425,   429,   432,   435,   439,
     444,   448,   452,   457,   461,   466,   470,   475,   478,   482,
     485,   489,   492,   496,   498,   501,   503,   506,   509,   512,
     515,   518,   520,   523,   525,   531,   534,   537,   539,   541,
     543,   545,   547,   549,   553,   555,   560,   563,   566,   568,
     570,   573,   576,   579,   582,   584,   586,   588,   590,   592,
     595,   598,   601,   604,   609,   614,   617,   620,   623,   626,
     629,   632,   635,   638,   641,   644,   647,   650,   653,   656,
     659,   662,   665,   668,   671,   674,   677,   680,   683,   686,
     689,   692,   695,   699,   703,   706,   708,   710,   712,   714,
     716,   718,   720,   722,   724
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      88,     0,    -1,    90,    -1,    89,    -1,   108,    -1,    91,
      -1,    90,    46,    91,    -1,    66,    91,    -1,    54,    91,
      -1,    65,    91,    -1,    64,    91,    -1,    80,    91,    -1,
      81,    91,    -1,    70,    91,    -1,    69,    91,    -1,    91,
      70,    -1,    91,    69,    -1,    18,    91,    -1,    91,    77,
     119,    -1,    91,    77,   119,    10,    -1,    91,    77,    10,
      -1,    91,    77,   100,    -1,    91,    76,    91,    -1,    91,
      72,   119,    -1,    91,    72,   119,    10,    -1,    91,    72,
      10,    -1,    91,    72,   100,    -1,    91,    75,    91,    -1,
      91,    73,    90,    82,    -1,    -1,    91,    74,    92,    95,
      83,    -1,    -1,     9,    74,    93,    95,    83,    -1,    42,
      74,    91,    83,    -1,    43,    74,    91,    46,    91,    83,
      -1,    84,    -1,    -1,    91,    -1,    95,    46,    91,    -1,
      91,    74,   111,    83,   101,    -1,    85,    -1,    94,    95,
      96,    -1,    94,   108,    96,    91,    -1,    74,   108,    83,
      91,    -1,    74,    90,    83,    -1,    91,    63,    91,    -1,
      91,    66,    91,    -1,    91,    67,    91,    -1,    91,    68,
      91,    -1,    91,    64,    91,    -1,    91,    65,    91,    -1,
      91,    62,    91,    -1,    91,    61,    91,    -1,    91,    56,
      91,    -1,    91,    55,    91,    -1,    91,    60,    91,    -1,
      91,    59,    91,    -1,    91,    57,    91,    -1,    91,    58,
      91,    -1,    91,    54,    91,    -1,    91,    53,    91,    -1,
      91,    52,    91,    -1,    91,    51,    91,    -1,    91,    50,
      91,    -1,    91,    49,    91,    86,    91,    -1,    91,    48,
      91,    -1,    91,    41,    91,    -1,     3,    -1,     7,    -1,
      12,    -1,     4,    -1,     5,    -1,    99,    -1,    40,    -1,
      18,    74,   108,    83,    -1,    25,    57,   108,    58,    74,
      91,    83,    -1,    27,    57,   108,    58,    74,    91,    83,
      -1,    26,    57,   108,    58,    74,    91,    83,    -1,    28,
      57,   108,    58,    74,    91,    83,    -1,     6,    -1,    97,
       6,    -1,    97,    -1,    44,    -1,    45,    -1,    78,    -1,
      79,    -1,    98,    20,   119,    -1,    98,    20,   119,    -1,
      11,    20,   119,    -1,    11,    20,    81,   119,    -1,    11,
      20,   119,    20,   119,    -1,   100,    -1,    20,   120,    -1,
     120,    -1,   117,    -1,    -1,   103,    -1,    -1,   117,    -1,
      63,   119,   103,    -1,    63,   119,    -1,    66,    -1,    66,
     104,    -1,    54,    -1,    54,   104,    -1,   105,    -1,    74,
     104,    83,    -1,   105,   106,    -1,   106,    -1,   105,   107,
      -1,   107,    -1,    73,    82,    -1,    73,     3,    82,    -1,
      74,    83,    -1,    74,   111,    83,    -1,   112,    -1,    11,
      -1,    32,    -1,    30,    -1,    31,    -1,    30,    32,    -1,
      30,    29,    32,    -1,    30,    29,    -1,    29,    30,    32,
      -1,    19,    30,    32,    -1,    30,    19,    32,    -1,    30,
      19,    -1,    30,    30,    -1,    30,    30,    32,    -1,    30,
      30,    29,    32,    -1,    30,    30,    29,    -1,    29,    30,
      30,    -1,    29,    30,    30,    32,    -1,    19,    30,    30,
      -1,    19,    30,    30,    32,    -1,    30,    30,    19,    -1,
      30,    30,    19,    32,    -1,    31,    32,    -1,    31,    29,
      32,    -1,    31,    29,    -1,    19,    31,    32,    -1,    31,
      19,    -1,    31,    19,    32,    -1,    35,    -1,    30,    35,
      -1,    36,    -1,    14,   119,    -1,    15,   119,    -1,    16,
     119,    -1,    17,   119,    -1,    19,   110,    -1,    19,    -1,
      29,   110,    -1,    29,    -1,    21,   119,    57,   108,    58,
      -1,   103,   109,    -1,   109,   103,    -1,    11,    -1,    32,
      -1,    30,    -1,    31,    -1,    36,    -1,   108,    -1,   111,
      46,   108,    -1,   109,    -1,   112,   102,   104,   102,    -1,
      33,    34,    -1,    34,    33,    -1,    33,    -1,    34,    -1,
      37,    38,    -1,    38,    37,    -1,    37,    39,    -1,    39,
      37,    -1,    37,    -1,   115,    -1,   114,    -1,   113,    -1,
     116,    -1,   113,   116,    -1,   116,   113,    -1,    13,    23,
      -1,    13,    24,    -1,    13,    23,    73,    82,    -1,    13,
      24,    73,    82,    -1,    13,    64,    -1,    13,    65,    -1,
      13,    66,    -1,    13,    67,    -1,    13,    68,    -1,    13,
      53,    -1,    13,    54,    -1,    13,    52,    -1,    13,    81,
      -1,    13,    80,    -1,    13,    48,    -1,    13,    57,    -1,
      13,    58,    -1,    13,    41,    -1,    13,    62,    -1,    13,
      61,    -1,    13,    56,    -1,    13,    55,    -1,    13,    60,
      -1,    13,    59,    -1,    13,    51,    -1,    13,    50,    -1,
      13,    70,    -1,    13,    69,    -1,    13,    46,    -1,    13,
      76,    -1,    13,    77,    -1,    13,    74,    83,    -1,    13,
      73,    82,    -1,    13,   112,    -1,     8,    -1,    78,    -1,
      11,    -1,    12,    -1,     9,    -1,   118,    -1,     8,    -1,
      78,    -1,   118,    -1,     9,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   271,   271,   272,   275,   282,   283,   288,   292,   296,
     300,   304,   308,   312,   316,   320,   324,   328,   332,   338,
     345,   355,   363,   367,   373,   380,   390,   398,   402,   409,
     406,   417,   416,   439,   443,   447,   451,   454,   458,   462,
     474,   477,   484,   490,   496,   502,   506,   510,   514,   518,
     522,   526,   530,   534,   538,   542,   546,   550,   554,   558,
     562,   566,   570,   574,   578,   582,   586,   592,   599,   608,
     619,   626,   633,   636,   642,   650,   656,   662,   668,   677,
     694,   712,   746,   753,   762,   770,   776,   786,   801,   816,
     840,   849,   850,   878,   960,   961,   964,   965,   968,   969,
     973,   980,   982,   984,   986,   988,   991,   993,   998,  1004,
    1006,  1010,  1012,  1016,  1018,  1030,  1034,  1036,  1038,  1040,
    1042,  1044,  1046,  1048,  1050,  1052,  1054,  1056,  1058,  1060,
    1062,  1064,  1066,  1068,  1070,  1072,  1074,  1076,  1078,  1080,
    1082,  1084,  1086,  1088,  1090,  1092,  1094,  1097,  1100,  1103,
    1106,  1110,  1112,  1116,  1121,  1125,  1127,  1131,  1132,  1138,
    1144,  1150,  1159,  1164,  1171,  1172,  1176,  1180,  1184,  1186,
    1189,  1193,  1198,  1202,  1208,  1210,  1211,  1214,  1215,  1216,
    1217,  1220,  1222,  1224,  1226,  1228,  1230,  1232,  1234,  1236,
    1238,  1240,  1242,  1244,  1246,  1248,  1250,  1252,  1254,  1294,
    1296,  1298,  1300,  1302,  1304,  1306,  1308,  1310,  1312,  1314,
    1316,  1318,  1320,  1322,  1324,  1339,  1340,  1341,  1342,  1343,
    1344,  1347,  1348,  1356,  1364
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "FLOAT", "DECFLOAT", "STRING",
  "CHAR", "NAME", "UNKNOWN_CPP_NAME", "COMPLETE", "TYPENAME",
  "NAME_OR_INT", "OPERATOR", "STRUCT", "CLASS", "UNION", "ENUM", "SIZEOF",
  "UNSIGNED", "COLONCOLON", "TEMPLATE", "ERROR", "NEW", "DELETE",
  "REINTERPRET_CAST", "DYNAMIC_CAST", "STATIC_CAST", "CONST_CAST",
  "SIGNED_KEYWORD", "LONG", "SHORT", "INT_KEYWORD", "CONST_KEYWORD",
  "VOLATILE_KEYWORD", "DOUBLE_KEYWORD", "VOID_KEYWORD", "SHARED_KEYWORD",
  "RELAXED_KEYWORD", "STRICT_KEYWORD", "VARIABLE", "ASSIGN_MODIFY",
  "UNOP_INTRINSIC", "BINOP_INTRINSIC", "TRUEKEYWORD", "FALSEKEYWORD",
  "','", "ABOVE_COMMA", "'='", "'?'", "OROR", "ANDAND", "'|'", "'^'",
  "'&'", "NOTEQUAL", "EQUAL", "'<'", "'>'", "GEQ", "LEQ", "RSH", "LSH",
  "'@'", "'+'", "'-'", "'*'", "'/'", "'%'", "DECREMENT", "INCREMENT",
  "UNARY", "'.'", "'['", "'('", "DOT_STAR", "ARROW_STAR", "ARROW",
  "BLOCKNAME", "FILENAME", "'!'", "'~'", "']'", "')'", "'{'", "'}'", "':'",
  "$accept", "start", "type_exp", "exp1", "exp", "@1", "@2", "lcurly",
  "arglist", "rcurly", "string_exp", "block", "variable", "qualified_name",
  "type_qualifier", "type_qualifier_or_space_identifier_or_nothing",
  "type_qualifier_or_space_identifier", "abs_decl", "direct_abs_decl",
  "array_mod", "func_mod", "type", "typebase", "typename",
  "nonempty_typelist", "ptype", "const_and_or_volatile",
  "shared_and_relaxed", "shared_and_strict",
  "shared_and_strict_or_relaxed", "type_qualifier_noopt", "operator",
  "name", "name_not_typename", 0
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,    44,   301,    61,    63,
     302,   303,   124,    94,    38,   304,   305,    60,    62,   306,
     307,   308,   309,    64,    43,    45,    42,    47,    37,   310,
     311,   312,    46,    91,    40,   313,   314,   315,   316,   317,
      33,   126,    93,    41,   123,   125,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    87,    88,    88,    89,    90,    90,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    92,
      91,    93,    91,    91,    91,    94,    95,    95,    95,    91,
      96,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    97,
      97,    91,    91,    91,    98,    98,    98,    99,   100,   100,
     100,    99,    99,    99,   101,   101,   102,   102,   103,   103,
     103,   104,   104,   104,   104,   104,   105,   105,   105,   105,
     105,   106,   106,   107,   107,   108,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   110,   110,   110,
     110,   110,   111,   111,   112,   112,   113,   113,   113,   113,
     114,   114,   115,   115,   116,   116,   116,   117,   117,   117,
     117,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   119,   119,   119,   119,   119,
     119,   120,   120,   120,   120
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     3,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     4,
       3,     3,     3,     3,     4,     3,     3,     3,     4,     0,
       5,     0,     5,     4,     6,     1,     0,     1,     3,     5,
       1,     3,     4,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     5,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     4,     7,     7,     7,     7,     1,
       2,     1,     1,     1,     1,     1,     3,     3,     3,     4,
       5,     1,     2,     1,     1,     0,     1,     0,     1,     3,
       2,     1,     2,     1,     2,     1,     3,     2,     1,     2,
       1,     2,     3,     2,     3,     1,     1,     1,     1,     1,
       2,     3,     2,     3,     3,     3,     2,     2,     3,     4,
       3,     3,     4,     3,     4,     3,     4,     2,     3,     2,
       3,     2,     3,     1,     2,     1,     2,     2,     2,     2,
       2,     1,     2,     1,     5,     2,     2,     1,     1,     1,
       1,     1,     1,     3,     1,     4,     2,     2,     1,     1,
       2,     2,     2,     2,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     4,     4,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    67,    70,    71,    79,    68,   221,   224,   116,    69,
       0,     0,     0,     0,     0,     0,   151,     0,     0,     0,
       0,     0,     0,   153,   118,   119,   117,   168,   169,   143,
     145,   174,     0,     0,    73,     0,     0,    82,    83,     0,
       0,     0,     0,     0,     0,     0,     0,   222,    85,     0,
       0,    35,     0,     3,     2,     5,    36,    81,     0,    72,
      91,     0,     4,   164,   115,   177,   176,   175,   178,    98,
     223,    93,    31,     0,   116,   181,   182,   198,   209,   195,
     206,   205,   192,   190,   191,   202,   201,   196,   197,   204,
     203,   200,   199,   185,   186,   187,   188,   189,   208,   207,
       0,     0,   210,   211,   194,   193,   214,   215,   219,   217,
     218,   216,   220,   146,   147,   148,   149,     0,     0,    17,
     157,   159,   160,   158,   161,   150,   224,   222,    92,     0,
       0,     0,     0,     0,   159,   160,   152,   126,   122,   127,
     120,   144,   141,   139,   137,   166,   167,   170,   172,   171,
     173,     0,     0,     8,   100,    10,     9,     7,    14,    13,
       0,     0,    11,    12,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,    15,
       0,     0,    29,     0,     0,     0,    37,     0,     0,    80,
       0,   155,   156,     0,    96,   179,   180,    36,     0,    88,
       0,     0,   213,   212,     0,   133,   124,   140,     0,     0,
       0,     0,     0,   131,   123,   125,   121,   135,   130,   128,
     142,   138,     0,     0,    99,    44,     0,     6,    66,    65,
       0,    63,    62,    61,    60,    59,    54,    53,    57,    58,
      56,    55,    52,    51,    45,    49,    50,    46,    47,    48,
      25,   217,    26,    23,     0,    36,   162,     0,    27,    22,
      20,    21,    18,     0,    40,    41,     0,    87,   103,   101,
       0,     0,    97,   105,   108,   110,     0,    89,     0,   183,
     184,    74,   134,     0,     0,     0,     0,     0,   132,   136,
     129,    33,     0,    43,     0,    24,    28,     0,     0,    95,
      19,    38,    42,   104,   102,     0,   111,   113,     0,     0,
     165,     0,   107,   109,    32,    90,   154,     0,     0,     0,
       0,     0,    64,    30,   163,    39,    94,   112,   106,   114,
       0,     0,     0,     0,    34,    75,    77,    76,    78
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    52,    53,   160,    55,   265,   207,    56,   197,   275,
      57,    58,    59,    60,   335,   203,    61,   282,   283,   284,
     285,   266,    63,   125,   319,    64,    65,    66,    67,    68,
      69,    70,   113,    71
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -212
static const yytype_int16 yypact[] =
{
     313,  -212,  -212,  -212,  -212,  -212,  -212,   -38,    -3,  -212,
     552,    31,    31,    31,    31,   226,   164,    24,    31,   -11,
      -6,    12,    38,   171,   230,   116,  -212,    65,    79,  -212,
    -212,    90,    77,    89,  -212,    75,    99,  -212,  -212,   395,
      31,   395,   395,   395,   395,   395,   313,   160,  -212,   395,
     395,  -212,   183,  -212,   139,   928,   313,   191,   169,  -212,
    -212,   620,  -212,    67,   351,    55,  -212,  -212,   113,  -212,
    -212,  -212,  -212,    10,  -212,   132,   138,  -212,  -212,  -212,
    -212,  -212,  -212,  -212,  -212,  -212,  -212,  -212,  -212,  -212,
    -212,  -212,  -212,  -212,  -212,  -212,  -212,  -212,  -212,  -212,
     136,   141,  -212,  -212,  -212,  -212,   351,  -212,  -212,  -212,
    -212,  -212,  -212,  -212,  -212,  -212,  -212,    -3,   313,   140,
    -212,    54,   194,  -212,  -212,  -212,  -212,  -212,  -212,   179,
     620,   620,   620,   620,    81,  -212,  -212,   208,   209,   155,
    -212,  -212,   210,   211,  -212,  -212,  -212,  -212,  -212,  -212,
    -212,   395,   395,   140,   244,   140,   140,   140,   140,   140,
     -21,   162,   140,   140,  -212,   395,   395,   395,   395,   395,
     395,   395,   395,   395,   395,   395,   395,   395,   395,   395,
     395,   395,   395,   395,   395,   395,   395,   395,  -212,  -212,
     112,   395,   620,   395,   395,   130,   928,   -31,   163,  -212,
      31,    67,  -212,    42,  -212,  -212,  -212,   395,    31,   227,
     173,   174,  -212,  -212,   180,   225,  -212,  -212,   620,   206,
     214,   215,   216,   235,  -212,  -212,  -212,   243,   252,  -212,
    -212,  -212,   675,   898,  -212,  -212,   395,   928,   928,   928,
     636,   984,  1010,  1035,  1059,  1082,  1103,  1103,   599,   599,
     599,   599,   296,   296,   378,  1115,  1115,   140,   140,   140,
    -212,    -3,  -212,   275,   -26,   395,  -212,     2,   354,   354,
    -212,  -212,   278,   395,  -212,  -212,   395,   273,   154,   103,
       5,    44,    67,   149,  -212,  -212,     6,  -212,    31,  -212,
    -212,   477,  -212,   236,   223,   224,   228,   229,  -212,  -212,
    -212,  -212,   395,   140,   395,  -212,  -212,     7,   620,   244,
    -212,   928,   140,  -212,  -212,   217,  -212,  -212,   218,    20,
    -212,   497,  -212,  -212,  -212,  -212,  -212,   395,   395,   395,
     395,   712,   957,  -212,  -212,  -212,  -212,  -212,  -212,  -212,
     749,   786,   823,   860,  -212,  -212,  -212,  -212,  -212
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -212,  -212,  -212,    13,   -15,  -212,  -212,  -212,  -193,   110,
    -212,  -212,  -212,  -145,  -212,    29,   -57,  -211,  -212,    40,
      52,     1,   276,   331,   144,   364,   307,  -212,  -212,   315,
      72,    -9,    -2,   369
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -98
static const yytype_int16 yytable[] =
{
     119,    62,   112,   112,   112,   112,   202,   204,   315,   112,
     114,   115,   116,    54,   286,   273,   129,    73,   107,   108,
     165,   109,   110,    10,   153,   165,   155,   156,   157,   158,
     159,   112,     6,   126,   162,   163,    72,    10,   154,   107,
     108,   196,   109,   110,    10,   262,   130,   161,   308,   204,
     271,   131,   273,   273,   274,    74,   306,   198,    11,    12,
      13,    14,   235,    16,   112,    18,   308,   313,   314,   132,
     318,   209,   307,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,   215,   309,   216,   316,   111,   324,
     333,   208,    31,    32,    33,   133,   278,   234,   278,   145,
      27,    28,   127,   339,    31,    32,    33,    40,   279,   111,
     279,   223,   146,   224,   149,   280,   281,   280,   281,   214,
     107,   108,   260,   261,   110,    10,   150,   317,   147,   148,
      40,   219,   220,   221,   222,   142,   232,   233,   107,   108,
     270,   261,   110,    10,   202,   143,    27,    28,   144,   151,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   152,   227,   120,   280,   281,   268,   269,
     -84,   112,   120,   164,   228,   165,   112,   229,   263,   200,
     111,   112,   196,   272,   121,   122,   123,   199,   277,   112,
     124,   134,   135,   123,   264,   210,   287,   124,   111,   188,
     189,   211,   190,   191,   192,   193,   194,   195,   212,   293,
     279,   303,   280,   321,   213,   204,   217,   280,   281,     1,
       2,     3,     4,     5,     6,     7,   218,   117,     9,    10,
     225,   226,   230,   231,    15,   236,    17,   288,   274,   137,
     196,    19,    20,    21,    22,   289,   290,   292,   311,   138,
     139,   312,   140,   291,   294,   141,    34,   298,    35,    36,
      37,    38,   295,   296,   297,   299,   303,    27,    28,   112,
      39,    31,    32,    33,   300,   305,   325,   331,   310,   332,
      41,    42,    43,   -86,   326,    44,    45,   327,   328,   337,
     118,   338,   329,   330,    47,    48,    49,    50,   276,   334,
      51,   320,   340,   341,   342,   343,     1,     2,     3,     4,
       5,     6,     7,   322,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,   323,   267,   201,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,   136,    35,    36,    37,    38,   182,
     183,   184,   185,   186,   187,   188,   189,    39,   190,   191,
     192,   193,   194,   195,   106,   206,    40,    41,    42,    43,
     205,   336,    44,    45,    27,    28,   128,    46,    31,    32,
      33,    47,    48,    49,    50,     0,     0,    51,     1,     2,
       3,     4,     5,     6,     7,   -97,   117,     9,    10,     0,
       0,     0,     0,    15,    40,    17,     0,   -97,     0,     0,
      19,    20,    21,    22,   -97,   -97,   190,   191,   192,   193,
     194,   195,     0,     0,     0,    34,     0,    35,    36,    37,
      38,     0,   183,   184,   185,   186,   187,   188,   189,    39,
     190,   191,   192,   193,   194,   195,     0,     0,     0,    41,
      42,    43,     0,     0,    44,    45,     0,     0,     0,    46,
       0,     0,     0,    47,    48,    49,    50,     0,     0,    51,
       1,     2,     3,     4,     5,     6,     7,     0,   117,     9,
      10,     0,     0,     0,     0,    15,     0,    17,     0,     0,
       0,     0,    19,    20,    21,    22,     0,     0,    74,     0,
       0,    11,    12,    13,    14,     0,    16,    34,    18,    35,
      36,    37,    38,     0,     0,     0,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    44,    45,     0,     0,
       0,    46,     0,     0,     0,    47,    48,    49,    50,     0,
      40,    51,     0,    74,     0,     0,    11,    12,    13,    14,
       0,    16,     0,    18,     0,    75,    76,     0,     0,     0,
     317,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,     0,    77,     0,     0,     0,     0,    78,     0,
      79,     0,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    40,    93,    94,    95,    96,
      97,    98,    99,     0,     0,   100,   101,     0,   102,   103,
       0,    74,   104,   105,    11,    12,    13,    14,     0,    16,
       0,    18,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,   190,   191,   192,   193,   194,   195,   166,     0,     0,
       0,     0,     0,    40,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,     0,   190,   191,
     192,   193,   194,   195,     0,     0,   166,     0,     0,     0,
       0,     0,   304,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,     0,   190,   191,   192,
     193,   194,   195,   166,     0,     0,     0,     0,   301,     0,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,     0,   190,   191,   192,   193,   194,   195,
     166,     0,     0,     0,     0,   344,     0,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,   190,   191,   192,   193,   194,   195,   166,     0,     0,
       0,     0,   345,     0,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,     0,   190,   191,
     192,   193,   194,   195,   166,     0,     0,     0,     0,   346,
       0,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,     0,   190,   191,   192,   193,   194,
     195,   166,     0,     0,     0,     0,   347,     0,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,     0,   190,   191,   192,   193,   194,   195,     0,   166,
       0,     0,     0,   348,   302,     0,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   166,
     190,   191,   192,   193,   194,   195,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,     0,
     190,   191,   192,   193,   194,   195,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,     0,   190,
     191,   192,   193,   194,   195,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,   190,   191,   192,   193,
     194,   195,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,     0,   190,   191,   192,   193,   194,   195,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,     0,   190,   191,   192,
     193,   194,   195,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,   190,   191,   192,   193,   194,   195,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,     0,   190,   191,   192,   193,   194,   195,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,     0,   190,   191,   192,   193,   194,
     195,   185,   186,   187,   188,   189,     0,   190,   191,   192,
     193,   194,   195
};

static const yytype_int16 yycheck[] =
{
      15,     0,    11,    12,    13,    14,    63,    64,     3,    18,
      12,    13,    14,     0,   207,    46,    18,    20,     8,     9,
      46,    11,    12,    13,    39,    46,    41,    42,    43,    44,
      45,    40,     8,     9,    49,    50,    74,    13,    40,     8,
       9,    56,    11,    12,    13,   190,    57,    46,    46,   106,
     195,    57,    46,    46,    85,    11,    82,    56,    14,    15,
      16,    17,    83,    19,    73,    21,    46,   278,   279,    57,
     281,    73,   265,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    30,    83,    32,    82,    78,    83,
      83,    81,    37,    38,    39,    57,    54,   154,    54,    34,
      33,    34,    78,    83,    37,    38,    39,    63,    66,    78,
      66,    30,    33,    32,    37,    73,    74,    73,    74,   118,
       8,     9,    10,    11,    12,    13,    37,    83,    38,    39,
      63,   130,   131,   132,   133,    19,   151,   152,     8,     9,
      10,    11,    12,    13,   201,    29,    33,    34,    32,    74,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,    74,    19,    11,    73,    74,   193,   194,
      20,   190,    11,     0,    29,    46,   195,    32,   190,    20,
      78,   200,   207,   195,    30,    31,    32,     6,   200,   208,
      36,    30,    31,    32,   191,    73,   208,    36,    78,    69,
      70,    73,    72,    73,    74,    75,    76,    77,    82,   218,
      66,   236,    73,    74,    83,   282,    32,    73,    74,     3,
       4,     5,     6,     7,     8,     9,    57,    11,    12,    13,
      32,    32,    32,    32,    18,    83,    20,    20,    85,    19,
     265,    25,    26,    27,    28,    82,    82,    32,   273,    29,
      30,   276,    32,    83,    58,    35,    40,    32,    42,    43,
      44,    45,    58,    58,    58,    32,   291,    33,    34,   288,
      54,    37,    38,    39,    32,    10,   288,   302,    10,   304,
      64,    65,    66,    20,    58,    69,    70,    74,    74,    82,
      74,    83,    74,    74,    78,    79,    80,    81,   198,   308,
      84,   282,   327,   328,   329,   330,     3,     4,     5,     6,
       7,     8,     9,   283,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   283,   192,    61,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    23,    42,    43,    44,    45,    63,
      64,    65,    66,    67,    68,    69,    70,    54,    72,    73,
      74,    75,    76,    77,    10,    68,    63,    64,    65,    66,
      65,   309,    69,    70,    33,    34,    17,    74,    37,    38,
      39,    78,    79,    80,    81,    -1,    -1,    84,     3,     4,
       5,     6,     7,     8,     9,    54,    11,    12,    13,    -1,
      -1,    -1,    -1,    18,    63,    20,    -1,    66,    -1,    -1,
      25,    26,    27,    28,    73,    74,    72,    73,    74,    75,
      76,    77,    -1,    -1,    -1,    40,    -1,    42,    43,    44,
      45,    -1,    64,    65,    66,    67,    68,    69,    70,    54,
      72,    73,    74,    75,    76,    77,    -1,    -1,    -1,    64,
      65,    66,    -1,    -1,    69,    70,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    78,    79,    80,    81,    -1,    -1,    84,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    12,
      13,    -1,    -1,    -1,    -1,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    -1,    -1,    11,    -1,
      -1,    14,    15,    16,    17,    -1,    19,    40,    21,    42,
      43,    44,    45,    -1,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    78,    79,    80,    81,    -1,
      63,    84,    -1,    11,    -1,    -1,    14,    15,    16,    17,
      -1,    19,    -1,    21,    -1,    23,    24,    -1,    -1,    -1,
      83,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    -1,    41,    -1,    -1,    -1,    -1,    46,    -1,
      48,    -1,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    -1,    73,    74,    -1,    76,    77,
      -1,    11,    80,    81,    14,    15,    16,    17,    -1,    19,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    41,    -1,    -1,
      -1,    -1,    -1,    63,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    86,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,    74,
      75,    76,    77,    41,    -1,    -1,    -1,    -1,    83,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      41,    -1,    -1,    -1,    -1,    83,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    41,    -1,    -1,
      -1,    -1,    83,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    41,    -1,    -1,    -1,    -1,    83,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    41,    -1,    -1,    -1,    -1,    83,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    -1,    41,
      -1,    -1,    -1,    83,    46,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    41,
      72,    73,    74,    75,    76,    77,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    72,
      73,    74,    75,    76,    77,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,    74,
      75,    76,    77,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    72,    73,    74,    75,    76,
      77,    66,    67,    68,    69,    70,    -1,    72,    73,    74,
      75,    76,    77
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    42,    43,    44,    45,    54,
      63,    64,    65,    66,    69,    70,    74,    78,    79,    80,
      81,    84,    88,    89,    90,    91,    94,    97,    98,    99,
     100,   103,   108,   109,   112,   113,   114,   115,   116,   117,
     118,   120,    74,    20,    11,    23,    24,    41,    46,    48,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    64,    65,    66,    67,    68,    69,    70,
      73,    74,    76,    77,    80,    81,   112,     8,     9,    11,
      12,    78,   118,   119,   119,   119,   119,    11,    74,    91,
      11,    30,    31,    32,    36,   110,     9,    78,   120,   119,
      57,    57,    57,    57,    30,    31,   110,    19,    29,    30,
      32,    35,    19,    29,    32,    34,    33,    38,    39,    37,
      37,    74,    74,    91,   119,    91,    91,    91,    91,    91,
      90,   108,    91,    91,     0,    46,    41,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      72,    73,    74,    75,    76,    77,    91,    95,   108,     6,
      20,   109,   103,   102,   103,   116,   113,    93,    81,   119,
      73,    73,    82,    83,   108,    30,    32,    32,    57,   108,
     108,   108,   108,    30,    32,    32,    32,    19,    29,    32,
      32,    32,    91,    91,   103,    83,    83,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      10,    11,   100,   119,    90,    92,   108,   111,    91,    91,
      10,   100,   119,    46,    85,    96,    96,   119,    54,    66,
      73,    74,   104,   105,   106,   107,    95,   119,    20,    82,
      82,    83,    32,   108,    58,    58,    58,    58,    32,    32,
      32,    83,    46,    91,    86,    10,    82,    95,    46,    83,
      10,    91,    91,   104,   104,     3,    82,    83,   104,   111,
     102,    74,   106,   107,    83,   119,    58,    74,    74,    74,
      74,    91,    91,    83,   108,   101,   117,    82,    83,    83,
      91,    91,    91,    91,    83,    83,    83,    83,    83
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
     to xreallocate them elsewhere.  */

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
	/* Give user a chance to xreallocate the stack.  Use copies of
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
        case 4:
#line 276 "c-exp.y"
    { write_exp_elt_opcode(OP_TYPE);
			  write_exp_elt_type((yyvsp[(1) - (1)].tval));
			  write_exp_elt_opcode(OP_TYPE);}
    break;

  case 6:
#line 284 "c-exp.y"
    { write_exp_elt_opcode (BINOP_COMMA); }
    break;

  case 7:
#line 289 "c-exp.y"
    { write_exp_elt_opcode (UNOP_IND); }
    break;

  case 8:
#line 293 "c-exp.y"
    { write_exp_elt_opcode (UNOP_ADDR); }
    break;

  case 9:
#line 297 "c-exp.y"
    { write_exp_elt_opcode (UNOP_NEG); }
    break;

  case 10:
#line 301 "c-exp.y"
    { write_exp_elt_opcode (UNOP_PLUS); }
    break;

  case 11:
#line 305 "c-exp.y"
    { write_exp_elt_opcode (UNOP_LOGICAL_NOT); }
    break;

  case 12:
#line 309 "c-exp.y"
    { write_exp_elt_opcode (UNOP_COMPLEMENT); }
    break;

  case 13:
#line 313 "c-exp.y"
    { write_exp_elt_opcode (UNOP_PREINCREMENT); }
    break;

  case 14:
#line 317 "c-exp.y"
    { write_exp_elt_opcode (UNOP_PREDECREMENT); }
    break;

  case 15:
#line 321 "c-exp.y"
    { write_exp_elt_opcode (UNOP_POSTINCREMENT); }
    break;

  case 16:
#line 325 "c-exp.y"
    { write_exp_elt_opcode (UNOP_POSTDECREMENT); }
    break;

  case 17:
#line 329 "c-exp.y"
    { write_exp_elt_opcode (UNOP_SIZEOF); }
    break;

  case 18:
#line 333 "c-exp.y"
    { write_exp_elt_opcode (STRUCTOP_PTR);
			  write_exp_string ((yyvsp[(3) - (3)].sval));
			  write_exp_elt_opcode (STRUCTOP_PTR); }
    break;

  case 19:
#line 339 "c-exp.y"
    { mark_struct_expression ();
			  write_exp_elt_opcode (STRUCTOP_PTR);
			  write_exp_string ((yyvsp[(3) - (4)].sval));
			  write_exp_elt_opcode (STRUCTOP_PTR); }
    break;

  case 20:
#line 346 "c-exp.y"
    { struct stoken s;
			  mark_struct_expression ();
			  write_exp_elt_opcode (STRUCTOP_PTR);
			  s.ptr = "";
			  s.length = 0;
			  write_exp_string (s);
			  write_exp_elt_opcode (STRUCTOP_PTR); }
    break;

  case 21:
#line 356 "c-exp.y"
    { /* exp->type::name becomes exp->*(&type::name) */
			  /* Note: this doesn't work if name is a
			     static member!  FIXME */
			  write_exp_elt_opcode (UNOP_ADDR);
			  write_exp_elt_opcode (STRUCTOP_MPTR); }
    break;

  case 22:
#line 364 "c-exp.y"
    { write_exp_elt_opcode (STRUCTOP_MPTR); }
    break;

  case 23:
#line 368 "c-exp.y"
    { write_exp_elt_opcode (STRUCTOP_STRUCT);
			  write_exp_string ((yyvsp[(3) - (3)].sval));
			  write_exp_elt_opcode (STRUCTOP_STRUCT); }
    break;

  case 24:
#line 374 "c-exp.y"
    { mark_struct_expression ();
			  write_exp_elt_opcode (STRUCTOP_STRUCT);
			  write_exp_string ((yyvsp[(3) - (4)].sval));
			  write_exp_elt_opcode (STRUCTOP_STRUCT); }
    break;

  case 25:
#line 381 "c-exp.y"
    { struct stoken s;
			  mark_struct_expression ();
			  write_exp_elt_opcode (STRUCTOP_STRUCT);
			  s.ptr = "";
			  s.length = 0;
			  write_exp_string (s);
			  write_exp_elt_opcode (STRUCTOP_STRUCT); }
    break;

  case 26:
#line 391 "c-exp.y"
    { /* exp.type::name becomes exp.*(&type::name) */
			  /* Note: this doesn't work if name is a
			     static member!  FIXME */
			  write_exp_elt_opcode (UNOP_ADDR);
			  write_exp_elt_opcode (STRUCTOP_MEMBER); }
    break;

  case 27:
#line 399 "c-exp.y"
    { write_exp_elt_opcode (STRUCTOP_MEMBER); }
    break;

  case 28:
#line 403 "c-exp.y"
    { write_exp_elt_opcode (BINOP_SUBSCRIPT); }
    break;

  case 29:
#line 409 "c-exp.y"
    { start_arglist (); }
    break;

  case 30:
#line 411 "c-exp.y"
    { write_exp_elt_opcode (OP_FUNCALL);
			  write_exp_elt_longcst ((LONGEST) end_arglist ());
			  write_exp_elt_opcode (OP_FUNCALL); }
    break;

  case 31:
#line 417 "c-exp.y"
    {
			  /* This could potentially be a an argument defined
			     lookup function (Koenig).  */
			  write_exp_elt_opcode (OP_ADL_FUNC);
			  write_exp_elt_block (expression_context_block);
			  write_exp_elt_sym (NULL); /* Placeholder.  */
			  write_exp_string ((yyvsp[(1) - (2)].ssym).stoken);
			  write_exp_elt_opcode (OP_ADL_FUNC);

			/* This is to save the value of arglist_len
			   being accumulated by an outer function call.  */

			  start_arglist ();
			}
    break;

  case 32:
#line 432 "c-exp.y"
    {
			  write_exp_elt_opcode (OP_FUNCALL);
			  write_exp_elt_longcst ((LONGEST) end_arglist ());
			  write_exp_elt_opcode (OP_FUNCALL);
			}
    break;

  case 33:
#line 440 "c-exp.y"
    { write_exp_elt_opcode ((yyvsp[(1) - (4)].opcode)); }
    break;

  case 34:
#line 444 "c-exp.y"
    { write_exp_elt_opcode ((yyvsp[(1) - (6)].opcode)); }
    break;

  case 35:
#line 448 "c-exp.y"
    { start_arglist (); }
    break;

  case 37:
#line 455 "c-exp.y"
    { arglist_len = 1; }
    break;

  case 38:
#line 459 "c-exp.y"
    { arglist_len++; }
    break;

  case 39:
#line 463 "c-exp.y"
    { int i;
			  write_exp_elt_opcode (TYPE_INSTANCE);
			  write_exp_elt_longcst ((LONGEST) (yyvsp[(3) - (5)].ivec)[0]);
			  for (i = 0; i < (yyvsp[(3) - (5)].ivec)[0]; ++i)
			    write_exp_elt_type ((yyvsp[(3) - (5)].tvec)[i + 1]);
			  write_exp_elt_longcst((LONGEST) (yyvsp[(3) - (5)].ivec)[0]);
			  write_exp_elt_opcode (TYPE_INSTANCE);
			  xfree ((yyvsp[(3) - (5)].tvec));
			}
    break;

  case 40:
#line 475 "c-exp.y"
    { (yyval.lval) = end_arglist () - 1; }
    break;

  case 41:
#line 478 "c-exp.y"
    { write_exp_elt_opcode (OP_ARRAY);
			  write_exp_elt_longcst ((LONGEST) 0);
			  write_exp_elt_longcst ((LONGEST) (yyvsp[(3) - (3)].lval));
			  write_exp_elt_opcode (OP_ARRAY); }
    break;

  case 42:
#line 485 "c-exp.y"
    { write_exp_elt_opcode (UNOP_MEMVAL);
			  write_exp_elt_type ((yyvsp[(2) - (4)].tval));
			  write_exp_elt_opcode (UNOP_MEMVAL); }
    break;

  case 43:
#line 491 "c-exp.y"
    { write_exp_elt_opcode (UNOP_CAST);
			  write_exp_elt_type ((yyvsp[(2) - (4)].tval));
			  write_exp_elt_opcode (UNOP_CAST); }
    break;

  case 44:
#line 497 "c-exp.y"
    { }
    break;

  case 45:
#line 503 "c-exp.y"
    { write_exp_elt_opcode (BINOP_REPEAT); }
    break;

  case 46:
#line 507 "c-exp.y"
    { write_exp_elt_opcode (BINOP_MUL); }
    break;

  case 47:
#line 511 "c-exp.y"
    { write_exp_elt_opcode (BINOP_DIV); }
    break;

  case 48:
#line 515 "c-exp.y"
    { write_exp_elt_opcode (BINOP_REM); }
    break;

  case 49:
#line 519 "c-exp.y"
    { write_exp_elt_opcode (BINOP_ADD); }
    break;

  case 50:
#line 523 "c-exp.y"
    { write_exp_elt_opcode (BINOP_SUB); }
    break;

  case 51:
#line 527 "c-exp.y"
    { write_exp_elt_opcode (BINOP_LSH); }
    break;

  case 52:
#line 531 "c-exp.y"
    { write_exp_elt_opcode (BINOP_RSH); }
    break;

  case 53:
#line 535 "c-exp.y"
    { write_exp_elt_opcode (BINOP_EQUAL); }
    break;

  case 54:
#line 539 "c-exp.y"
    { write_exp_elt_opcode (BINOP_NOTEQUAL); }
    break;

  case 55:
#line 543 "c-exp.y"
    { write_exp_elt_opcode (BINOP_LEQ); }
    break;

  case 56:
#line 547 "c-exp.y"
    { write_exp_elt_opcode (BINOP_GEQ); }
    break;

  case 57:
#line 551 "c-exp.y"
    { write_exp_elt_opcode (BINOP_LESS); }
    break;

  case 58:
#line 555 "c-exp.y"
    { write_exp_elt_opcode (BINOP_GTR); }
    break;

  case 59:
#line 559 "c-exp.y"
    { write_exp_elt_opcode (BINOP_BITWISE_AND); }
    break;

  case 60:
#line 563 "c-exp.y"
    { write_exp_elt_opcode (BINOP_BITWISE_XOR); }
    break;

  case 61:
#line 567 "c-exp.y"
    { write_exp_elt_opcode (BINOP_BITWISE_IOR); }
    break;

  case 62:
#line 571 "c-exp.y"
    { write_exp_elt_opcode (BINOP_LOGICAL_AND); }
    break;

  case 63:
#line 575 "c-exp.y"
    { write_exp_elt_opcode (BINOP_LOGICAL_OR); }
    break;

  case 64:
#line 579 "c-exp.y"
    { write_exp_elt_opcode (TERNOP_COND); }
    break;

  case 65:
#line 583 "c-exp.y"
    { write_exp_elt_opcode (BINOP_ASSIGN); }
    break;

  case 66:
#line 587 "c-exp.y"
    { write_exp_elt_opcode (BINOP_ASSIGN_MODIFY);
			  write_exp_elt_opcode ((yyvsp[(2) - (3)].opcode));
			  write_exp_elt_opcode (BINOP_ASSIGN_MODIFY); }
    break;

  case 67:
#line 593 "c-exp.y"
    { write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type ((yyvsp[(1) - (1)].typed_val_int).type);
			  write_exp_elt_longcst ((LONGEST)((yyvsp[(1) - (1)].typed_val_int).val));
			  write_exp_elt_opcode (OP_LONG); }
    break;

  case 68:
#line 600 "c-exp.y"
    {
			  struct stoken_vector vec;
			  vec.len = 1;
			  vec.tokens = &(yyvsp[(1) - (1)].tsval);
			  write_exp_string_vector ((yyvsp[(1) - (1)].tsval).type, &vec);
			}
    break;

  case 69:
#line 609 "c-exp.y"
    { YYSTYPE val;
			  parse_number ((yyvsp[(1) - (1)].ssym).stoken.ptr, (yyvsp[(1) - (1)].ssym).stoken.length, 0, &val);
			  write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (val.typed_val_int.type);
			  write_exp_elt_longcst ((LONGEST)val.typed_val_int.val);
			  write_exp_elt_opcode (OP_LONG);
			}
    break;

  case 70:
#line 620 "c-exp.y"
    { write_exp_elt_opcode (OP_DOUBLE);
			  write_exp_elt_type ((yyvsp[(1) - (1)].typed_val_float).type);
			  write_exp_elt_dblcst ((yyvsp[(1) - (1)].typed_val_float).dval);
			  write_exp_elt_opcode (OP_DOUBLE); }
    break;

  case 71:
#line 627 "c-exp.y"
    { write_exp_elt_opcode (OP_DECFLOAT);
			  write_exp_elt_type ((yyvsp[(1) - (1)].typed_val_decfloat).type);
			  write_exp_elt_decfloatcst ((yyvsp[(1) - (1)].typed_val_decfloat).val);
			  write_exp_elt_opcode (OP_DECFLOAT); }
    break;

  case 73:
#line 637 "c-exp.y"
    {
			  write_dollar_variable ((yyvsp[(1) - (1)].sval));
			}
    break;

  case 74:
#line 643 "c-exp.y"
    { write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (parse_type->builtin_int);
			  CHECK_TYPEDEF ((yyvsp[(3) - (4)].tval));
			  write_exp_elt_longcst ((LONGEST) TYPE_LENGTH ((yyvsp[(3) - (4)].tval)));
			  write_exp_elt_opcode (OP_LONG); }
    break;

  case 75:
#line 651 "c-exp.y"
    { write_exp_elt_opcode (UNOP_REINTERPRET_CAST);
			  write_exp_elt_type ((yyvsp[(3) - (7)].tval));
			  write_exp_elt_opcode (UNOP_REINTERPRET_CAST); }
    break;

  case 76:
#line 657 "c-exp.y"
    { write_exp_elt_opcode (UNOP_CAST);
			  write_exp_elt_type ((yyvsp[(3) - (7)].tval));
			  write_exp_elt_opcode (UNOP_CAST); }
    break;

  case 77:
#line 663 "c-exp.y"
    { write_exp_elt_opcode (UNOP_DYNAMIC_CAST);
			  write_exp_elt_type ((yyvsp[(3) - (7)].tval));
			  write_exp_elt_opcode (UNOP_DYNAMIC_CAST); }
    break;

  case 78:
#line 669 "c-exp.y"
    { /* We could do more error checking here, but
			     it doesn't seem worthwhile.  */
			  write_exp_elt_opcode (UNOP_CAST);
			  write_exp_elt_type ((yyvsp[(3) - (7)].tval));
			  write_exp_elt_opcode (UNOP_CAST); }
    break;

  case 79:
#line 678 "c-exp.y"
    {
			  /* We copy the string here, and not in the
			     lexer, to guarantee that we do not leak a
			     string.  Note that we follow the
			     NUL-termination convention of the
			     lexer.  */
			  struct typed_stoken *vec = XNEW (struct typed_stoken);
			  (yyval.svec).len = 1;
			  (yyval.svec).tokens = vec;

			  vec->type = (yyvsp[(1) - (1)].tsval).type;
			  vec->length = (yyvsp[(1) - (1)].tsval).length;
			  vec->ptr = xmalloc ((yyvsp[(1) - (1)].tsval).length + 1);
			  memcpy (vec->ptr, (yyvsp[(1) - (1)].tsval).ptr, (yyvsp[(1) - (1)].tsval).length + 1);
			}
    break;

  case 80:
#line 695 "c-exp.y"
    {
			  /* Note that we NUL-terminate here, but just
			     for convenience.  */
			  char *p;
			  ++(yyval.svec).len;
			  (yyval.svec).tokens = xrealloc ((yyval.svec).tokens,
					       (yyval.svec).len * sizeof (struct typed_stoken));

			  p = xmalloc ((yyvsp[(2) - (2)].tsval).length + 1);
			  memcpy (p, (yyvsp[(2) - (2)].tsval).ptr, (yyvsp[(2) - (2)].tsval).length + 1);

			  (yyval.svec).tokens[(yyval.svec).len - 1].type = (yyvsp[(2) - (2)].tsval).type;
			  (yyval.svec).tokens[(yyval.svec).len - 1].length = (yyvsp[(2) - (2)].tsval).length;
			  (yyval.svec).tokens[(yyval.svec).len - 1].ptr = p;
			}
    break;

  case 81:
#line 713 "c-exp.y"
    {
			  int i;
			  enum c_string_type type = C_STRING;

			  for (i = 0; i < (yyvsp[(1) - (1)].svec).len; ++i)
			    {
			      switch ((yyvsp[(1) - (1)].svec).tokens[i].type)
				{
				case C_STRING:
				  break;
				case C_WIDE_STRING:
				case C_STRING_16:
				case C_STRING_32:
				  if (type != C_STRING
				      && type != (yyvsp[(1) - (1)].svec).tokens[i].type)
				    error ("Undefined string concatenation.");
				  type = (yyvsp[(1) - (1)].svec).tokens[i].type;
				  break;
				default:
				  /* internal error */
				  internal_error (__FILE__, __LINE__,
						  "unrecognized type in string concatenation");
				}
			    }

			  write_exp_string_vector (type, &(yyvsp[(1) - (1)].svec));
			  for (i = 0; i < (yyvsp[(1) - (1)].svec).len; ++i)
			    xfree ((yyvsp[(1) - (1)].svec).tokens[i].ptr);
			  xfree ((yyvsp[(1) - (1)].svec).tokens);
			}
    break;

  case 82:
#line 747 "c-exp.y"
    { write_exp_elt_opcode (OP_LONG);
                          write_exp_elt_type (parse_type->builtin_bool);
                          write_exp_elt_longcst ((LONGEST) 1);
                          write_exp_elt_opcode (OP_LONG); }
    break;

  case 83:
#line 754 "c-exp.y"
    { write_exp_elt_opcode (OP_LONG);
                          write_exp_elt_type (parse_type->builtin_bool);
                          write_exp_elt_longcst ((LONGEST) 0);
                          write_exp_elt_opcode (OP_LONG); }
    break;

  case 84:
#line 763 "c-exp.y"
    {
			  if ((yyvsp[(1) - (1)].ssym).sym)
			    (yyval.bval) = SYMBOL_BLOCK_VALUE ((yyvsp[(1) - (1)].ssym).sym);
			  else
			    error ("No file or function \"%s\".",
				   copy_name ((yyvsp[(1) - (1)].ssym).stoken));
			}
    break;

  case 85:
#line 771 "c-exp.y"
    {
			  (yyval.bval) = (yyvsp[(1) - (1)].bval);
			}
    break;

  case 86:
#line 777 "c-exp.y"
    { struct symbol *tem
			    = lookup_symbol (copy_name ((yyvsp[(3) - (3)].sval)), (yyvsp[(1) - (3)].bval),
					     VAR_DOMAIN, (int *) NULL);
			  if (!tem || SYMBOL_CLASS (tem) != LOC_BLOCK)
			    error ("No function \"%s\" in specified context.",
				   copy_name ((yyvsp[(3) - (3)].sval)));
			  (yyval.bval) = SYMBOL_BLOCK_VALUE (tem); }
    break;

  case 87:
#line 787 "c-exp.y"
    { struct symbol *sym;
			  sym = lookup_symbol (copy_name ((yyvsp[(3) - (3)].sval)), (yyvsp[(1) - (3)].bval),
					       VAR_DOMAIN, (int *) NULL);
			  if (sym == 0)
			    error ("No symbol \"%s\" in specified context.",
				   copy_name ((yyvsp[(3) - (3)].sval)));

			  write_exp_elt_opcode (OP_VAR_VALUE);
			  /* block_found is set by lookup_symbol.  */
			  write_exp_elt_block (block_found);
			  write_exp_elt_sym (sym);
			  write_exp_elt_opcode (OP_VAR_VALUE); }
    break;

  case 88:
#line 802 "c-exp.y"
    {
			  struct type *type = (yyvsp[(1) - (3)].tsym).type;
			  CHECK_TYPEDEF (type);
			  if (TYPE_CODE (type) != TYPE_CODE_STRUCT
			      && TYPE_CODE (type) != TYPE_CODE_UNION
			      && TYPE_CODE (type) != TYPE_CODE_NAMESPACE)
			    error ("`%s' is not defined as an aggregate type.",
				   TYPE_NAME (type));

			  write_exp_elt_opcode (OP_SCOPE);
			  write_exp_elt_type (type);
			  write_exp_string ((yyvsp[(3) - (3)].sval));
			  write_exp_elt_opcode (OP_SCOPE);
			}
    break;

  case 89:
#line 817 "c-exp.y"
    {
			  struct type *type = (yyvsp[(1) - (4)].tsym).type;
			  struct stoken tmp_token;
			  CHECK_TYPEDEF (type);
			  if (TYPE_CODE (type) != TYPE_CODE_STRUCT
			      && TYPE_CODE (type) != TYPE_CODE_UNION
			      && TYPE_CODE (type) != TYPE_CODE_NAMESPACE)
			    error ("`%s' is not defined as an aggregate type.",
				   TYPE_NAME (type));

			  tmp_token.ptr = (char*) alloca ((yyvsp[(4) - (4)].sval).length + 2);
			  tmp_token.length = (yyvsp[(4) - (4)].sval).length + 1;
			  tmp_token.ptr[0] = '~';
			  memcpy (tmp_token.ptr+1, (yyvsp[(4) - (4)].sval).ptr, (yyvsp[(4) - (4)].sval).length);
			  tmp_token.ptr[tmp_token.length] = 0;

			  /* Check for valid destructor name.  */
			  destructor_name_p (tmp_token.ptr, type);
			  write_exp_elt_opcode (OP_SCOPE);
			  write_exp_elt_type (type);
			  write_exp_string (tmp_token);
			  write_exp_elt_opcode (OP_SCOPE);
			}
    break;

  case 90:
#line 841 "c-exp.y"
    {
			  char *copy = copy_name ((yyvsp[(3) - (5)].sval));
			  error (_("No type \"%s\" within class "
				   "or namespace \"%s\"."),
				 copy, TYPE_NAME ((yyvsp[(1) - (5)].tsym).type));
			}
    break;

  case 92:
#line 851 "c-exp.y"
    {
			  char *name = copy_name ((yyvsp[(2) - (2)].ssym).stoken);
			  struct symbol *sym;
			  struct minimal_symbol *msymbol;

			  sym =
			    lookup_symbol (name, (const struct block *) NULL,
					   VAR_DOMAIN, (int *) NULL);
			  if (sym)
			    {
			      write_exp_elt_opcode (OP_VAR_VALUE);
			      write_exp_elt_block (NULL);
			      write_exp_elt_sym (sym);
			      write_exp_elt_opcode (OP_VAR_VALUE);
			      break;
			    }

			  msymbol = lookup_minimal_symbol (name, NULL, NULL);
			  if (msymbol != NULL)
			    write_exp_msymbol (msymbol);
			  else if (!have_full_symbols () && !have_partial_symbols ())
			    error ("No symbol table is loaded.  Use the \"file\" command.");
			  else
			    error ("No symbol \"%s\" in current context.", name);
			}
    break;

  case 93:
#line 879 "c-exp.y"
    { struct symbol *sym = (yyvsp[(1) - (1)].ssym).sym;
			  struct internalvar *isym = NULL;

			  if (sym)
			    {
			      if (symbol_read_needs_frame (sym))
				{
				  if (innermost_block == 0
				      || contained_in (block_found, 
						       innermost_block))
				    innermost_block = block_found;
				}

			      write_exp_elt_opcode (OP_VAR_VALUE);
			      /* We want to use the selected frame, not
				 another more inner frame which happens to
				 be in the same block.  */
			      write_exp_elt_block (NULL);
			      write_exp_elt_sym (sym);
			      write_exp_elt_opcode (OP_VAR_VALUE);
			    }
			  else if ((yyvsp[(1) - (1)].ssym).is_a_field_of_this)
			    {
			      /* C++: it hangs off of `this'.  Must
			         not inadvertently convert from a method call
				 to data ref.  */
			      if (innermost_block == 0
				  || contained_in (block_found,
						   innermost_block))
				innermost_block = block_found;
			      write_exp_elt_opcode (OP_THIS);
			      write_exp_elt_opcode (OP_THIS);
			      write_exp_elt_opcode (STRUCTOP_PTR);
			      write_exp_string ((yyvsp[(1) - (1)].ssym).stoken);
			      write_exp_elt_opcode (STRUCTOP_PTR);
			    }
			  else if (parse_language->la_language == language_upc
			           && (!strcmp ((yyvsp[(1) - (1)].ssym).stoken.ptr, "THREADS")
				       || !strcmp ((yyvsp[(1) - (1)].ssym).stoken.ptr, "MYTHREAD"))
				   && (isym = lookup_only_internalvar (copy_name ((yyvsp[(1) - (1)].ssym).stoken))))
			    {
				write_exp_elt_opcode (OP_INTERNALVAR);
				write_exp_elt_intern (isym);
				write_exp_elt_opcode (OP_INTERNALVAR);
			    }
			  else
			    {
			      struct minimal_symbol *msymbol;
			      char *arg = copy_name ((yyvsp[(1) - (1)].ssym).stoken);

			      msymbol =
				lookup_minimal_symbol (arg, NULL, NULL);
			      if (msymbol != NULL)
				write_exp_msymbol (msymbol);
			      else if (!have_full_symbols () && !have_partial_symbols ())
				error ("No symbol table is loaded.  Use the \"file\" command.");
			      else
				error ("No symbol \"%s\" in current context.",
				       copy_name ((yyvsp[(1) - (1)].ssym).stoken));
			    }
			}
    break;

  case 99:
#line 970 "c-exp.y"
    { push_type_address_space (copy_name ((yyvsp[(2) - (3)].sval)));
		  push_type (tp_space_identifier);
		}
    break;

  case 100:
#line 974 "c-exp.y"
    { push_type_address_space (copy_name ((yyvsp[(2) - (2)].sval)));
		  push_type (tp_space_identifier);
		}
    break;

  case 101:
#line 981 "c-exp.y"
    { push_type (tp_pointer); (yyval.voidval) = 0; }
    break;

  case 102:
#line 983 "c-exp.y"
    { push_type (tp_pointer); (yyval.voidval) = (yyvsp[(2) - (2)].voidval); }
    break;

  case 103:
#line 985 "c-exp.y"
    { push_type (tp_reference); (yyval.voidval) = 0; }
    break;

  case 104:
#line 987 "c-exp.y"
    { push_type (tp_reference); (yyval.voidval) = (yyvsp[(2) - (2)].voidval); }
    break;

  case 106:
#line 992 "c-exp.y"
    { (yyval.voidval) = (yyvsp[(2) - (3)].voidval); }
    break;

  case 107:
#line 994 "c-exp.y"
    {
			  push_type_int ((yyvsp[(2) - (2)].lval));
			  push_type (tp_array);
			}
    break;

  case 108:
#line 999 "c-exp.y"
    {
			  push_type_int ((yyvsp[(1) - (1)].lval));
			  push_type (tp_array);
			  (yyval.voidval) = 0;
			}
    break;

  case 109:
#line 1005 "c-exp.y"
    { push_type (tp_function); }
    break;

  case 110:
#line 1007 "c-exp.y"
    { push_type (tp_function); }
    break;

  case 111:
#line 1011 "c-exp.y"
    { (yyval.lval) = -1; }
    break;

  case 112:
#line 1013 "c-exp.y"
    { (yyval.lval) = (yyvsp[(2) - (3)].typed_val_int).val; }
    break;

  case 113:
#line 1017 "c-exp.y"
    { (yyval.voidval) = 0; }
    break;

  case 114:
#line 1019 "c-exp.y"
    { xfree ((yyvsp[(2) - (3)].tvec)); (yyval.voidval) = 0; }
    break;

  case 116:
#line 1035 "c-exp.y"
    { (yyval.tval) = (yyvsp[(1) - (1)].tsym).type; }
    break;

  case 117:
#line 1037 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_int; }
    break;

  case 118:
#line 1039 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_long; }
    break;

  case 119:
#line 1041 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_short; }
    break;

  case 120:
#line 1043 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_long; }
    break;

  case 121:
#line 1045 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_long; }
    break;

  case 122:
#line 1047 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_long; }
    break;

  case 123:
#line 1049 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_long; }
    break;

  case 124:
#line 1051 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_unsigned_long; }
    break;

  case 125:
#line 1053 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_unsigned_long; }
    break;

  case 126:
#line 1055 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_unsigned_long; }
    break;

  case 127:
#line 1057 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_long_long; }
    break;

  case 128:
#line 1059 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_long_long; }
    break;

  case 129:
#line 1061 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_long_long; }
    break;

  case 130:
#line 1063 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_long_long; }
    break;

  case 131:
#line 1065 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_long_long; }
    break;

  case 132:
#line 1067 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_long_long; }
    break;

  case 133:
#line 1069 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_unsigned_long_long; }
    break;

  case 134:
#line 1071 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_unsigned_long_long; }
    break;

  case 135:
#line 1073 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_unsigned_long_long; }
    break;

  case 136:
#line 1075 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_unsigned_long_long; }
    break;

  case 137:
#line 1077 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_short; }
    break;

  case 138:
#line 1079 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_short; }
    break;

  case 139:
#line 1081 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_short; }
    break;

  case 140:
#line 1083 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_unsigned_short; }
    break;

  case 141:
#line 1085 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_unsigned_short; }
    break;

  case 142:
#line 1087 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_unsigned_short; }
    break;

  case 143:
#line 1089 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_double; }
    break;

  case 144:
#line 1091 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_long_double; }
    break;

  case 145:
#line 1093 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_void; }
    break;

  case 146:
#line 1095 "c-exp.y"
    { (yyval.tval) = lookup_struct (copy_name ((yyvsp[(2) - (2)].sval)),
					      expression_context_block); }
    break;

  case 147:
#line 1098 "c-exp.y"
    { (yyval.tval) = lookup_struct (copy_name ((yyvsp[(2) - (2)].sval)),
					      expression_context_block); }
    break;

  case 148:
#line 1101 "c-exp.y"
    { (yyval.tval) = lookup_union (copy_name ((yyvsp[(2) - (2)].sval)),
					     expression_context_block); }
    break;

  case 149:
#line 1104 "c-exp.y"
    { (yyval.tval) = lookup_enum (copy_name ((yyvsp[(2) - (2)].sval)),
					    expression_context_block); }
    break;

  case 150:
#line 1107 "c-exp.y"
    { (yyval.tval) = lookup_unsigned_typename (parse_language,
							 parse_gdbarch,
							 TYPE_NAME((yyvsp[(2) - (2)].tsym).type)); }
    break;

  case 151:
#line 1111 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_unsigned_int; }
    break;

  case 152:
#line 1113 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       TYPE_NAME((yyvsp[(2) - (2)].tsym).type)); }
    break;

  case 153:
#line 1117 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_int; }
    break;

  case 154:
#line 1122 "c-exp.y"
    { (yyval.tval) = lookup_template_type(copy_name((yyvsp[(2) - (5)].sval)), (yyvsp[(4) - (5)].tval),
						    expression_context_block);
			}
    break;

  case 155:
#line 1126 "c-exp.y"
    { (yyval.tval) = follow_types ((yyvsp[(2) - (2)].tval)); }
    break;

  case 156:
#line 1128 "c-exp.y"
    { (yyval.tval) = follow_types ((yyvsp[(1) - (2)].tval)); }
    break;

  case 158:
#line 1133 "c-exp.y"
    {
		  (yyval.tsym).stoken.ptr = "int";
		  (yyval.tsym).stoken.length = 3;
		  (yyval.tsym).type = parse_type->builtin_int;
		}
    break;

  case 159:
#line 1139 "c-exp.y"
    {
		  (yyval.tsym).stoken.ptr = "long";
		  (yyval.tsym).stoken.length = 4;
		  (yyval.tsym).type = parse_type->builtin_long;
		}
    break;

  case 160:
#line 1145 "c-exp.y"
    {
		  (yyval.tsym).stoken.ptr = "short";
		  (yyval.tsym).stoken.length = 5;
		  (yyval.tsym).type = parse_type->builtin_short;
		}
    break;

  case 161:
#line 1151 "c-exp.y"
    {
		  (yyval.tsym).stoken.ptr = "void";
		  (yyval.tsym).stoken.length = 4;
		  (yyval.tsym).type = parse_type->builtin_void;
		}
    break;

  case 162:
#line 1160 "c-exp.y"
    { (yyval.tvec) = (struct type **) xmalloc (sizeof (struct type *) * 2);
		  (yyval.ivec)[0] = 1;	/* Number of types in vector */
		  (yyval.tvec)[1] = (yyvsp[(1) - (1)].tval);
		}
    break;

  case 163:
#line 1165 "c-exp.y"
    { int len = sizeof (struct type *) * (++((yyvsp[(1) - (3)].ivec)[0]) + 1);
		  (yyval.tvec) = (struct type **) xrealloc ((char *) (yyvsp[(1) - (3)].tvec), len);
		  (yyval.tvec)[(yyval.ivec)[0]] = (yyvsp[(3) - (3)].tval);
		}
    break;

  case 165:
#line 1173 "c-exp.y"
    { (yyval.tval) = follow_types ((yyvsp[(1) - (4)].tval)); }
    break;

  case 166:
#line 1177 "c-exp.y"
    { push_type (tp_const);
			  push_type (tp_volatile); 
			}
    break;

  case 167:
#line 1181 "c-exp.y"
    { push_type (tp_const);
			  push_type (tp_volatile); 
			}
    break;

  case 168:
#line 1185 "c-exp.y"
    { push_type (tp_const); }
    break;

  case 169:
#line 1187 "c-exp.y"
    { push_type (tp_volatile); }
    break;

  case 170:
#line 1190 "c-exp.y"
    { push_type (tp_shared);
			  push_type (tp_relaxed); 
			}
    break;

  case 171:
#line 1194 "c-exp.y"
    { push_type (tp_shared);
			  push_type (tp_relaxed); 
			}
    break;

  case 172:
#line 1199 "c-exp.y"
    { push_type (tp_shared);
			  push_type (tp_strict); 
			}
    break;

  case 173:
#line 1203 "c-exp.y"
    { push_type (tp_shared);
			  push_type (tp_strict); 
			}
    break;

  case 174:
#line 1209 "c-exp.y"
    { push_type (tp_shared); }
    break;

  case 181:
#line 1221 "c-exp.y"
    { (yyval.sval) = operator_stoken (" new"); }
    break;

  case 182:
#line 1223 "c-exp.y"
    { (yyval.sval) = operator_stoken (" delete"); }
    break;

  case 183:
#line 1225 "c-exp.y"
    { (yyval.sval) = operator_stoken (" new[]"); }
    break;

  case 184:
#line 1227 "c-exp.y"
    { (yyval.sval) = operator_stoken (" delete[]"); }
    break;

  case 185:
#line 1229 "c-exp.y"
    { (yyval.sval) = operator_stoken ("+"); }
    break;

  case 186:
#line 1231 "c-exp.y"
    { (yyval.sval) = operator_stoken ("-"); }
    break;

  case 187:
#line 1233 "c-exp.y"
    { (yyval.sval) = operator_stoken ("*"); }
    break;

  case 188:
#line 1235 "c-exp.y"
    { (yyval.sval) = operator_stoken ("/"); }
    break;

  case 189:
#line 1237 "c-exp.y"
    { (yyval.sval) = operator_stoken ("%"); }
    break;

  case 190:
#line 1239 "c-exp.y"
    { (yyval.sval) = operator_stoken ("^"); }
    break;

  case 191:
#line 1241 "c-exp.y"
    { (yyval.sval) = operator_stoken ("&"); }
    break;

  case 192:
#line 1243 "c-exp.y"
    { (yyval.sval) = operator_stoken ("|"); }
    break;

  case 193:
#line 1245 "c-exp.y"
    { (yyval.sval) = operator_stoken ("~"); }
    break;

  case 194:
#line 1247 "c-exp.y"
    { (yyval.sval) = operator_stoken ("!"); }
    break;

  case 195:
#line 1249 "c-exp.y"
    { (yyval.sval) = operator_stoken ("="); }
    break;

  case 196:
#line 1251 "c-exp.y"
    { (yyval.sval) = operator_stoken ("<"); }
    break;

  case 197:
#line 1253 "c-exp.y"
    { (yyval.sval) = operator_stoken (">"); }
    break;

  case 198:
#line 1255 "c-exp.y"
    { const char *op = "unknown";
			  switch ((yyvsp[(2) - (2)].opcode))
			    {
			    case BINOP_RSH:
			      op = ">>=";
			      break;
			    case BINOP_LSH:
			      op = "<<=";
			      break;
			    case BINOP_ADD:
			      op = "+=";
			      break;
			    case BINOP_SUB:
			      op = "-=";
			      break;
			    case BINOP_MUL:
			      op = "*=";
			      break;
			    case BINOP_DIV:
			      op = "/=";
			      break;
			    case BINOP_REM:
			      op = "%=";
			      break;
			    case BINOP_BITWISE_IOR:
			      op = "|=";
			      break;
			    case BINOP_BITWISE_AND:
			      op = "&=";
			      break;
			    case BINOP_BITWISE_XOR:
			      op = "^=";
			      break;
			    default:
			      break;
			    }

			  (yyval.sval) = operator_stoken (op);
			}
    break;

  case 199:
#line 1295 "c-exp.y"
    { (yyval.sval) = operator_stoken ("<<"); }
    break;

  case 200:
#line 1297 "c-exp.y"
    { (yyval.sval) = operator_stoken (">>"); }
    break;

  case 201:
#line 1299 "c-exp.y"
    { (yyval.sval) = operator_stoken ("=="); }
    break;

  case 202:
#line 1301 "c-exp.y"
    { (yyval.sval) = operator_stoken ("!="); }
    break;

  case 203:
#line 1303 "c-exp.y"
    { (yyval.sval) = operator_stoken ("<="); }
    break;

  case 204:
#line 1305 "c-exp.y"
    { (yyval.sval) = operator_stoken (">="); }
    break;

  case 205:
#line 1307 "c-exp.y"
    { (yyval.sval) = operator_stoken ("&&"); }
    break;

  case 206:
#line 1309 "c-exp.y"
    { (yyval.sval) = operator_stoken ("||"); }
    break;

  case 207:
#line 1311 "c-exp.y"
    { (yyval.sval) = operator_stoken ("++"); }
    break;

  case 208:
#line 1313 "c-exp.y"
    { (yyval.sval) = operator_stoken ("--"); }
    break;

  case 209:
#line 1315 "c-exp.y"
    { (yyval.sval) = operator_stoken (","); }
    break;

  case 210:
#line 1317 "c-exp.y"
    { (yyval.sval) = operator_stoken ("->*"); }
    break;

  case 211:
#line 1319 "c-exp.y"
    { (yyval.sval) = operator_stoken ("->"); }
    break;

  case 212:
#line 1321 "c-exp.y"
    { (yyval.sval) = operator_stoken ("()"); }
    break;

  case 213:
#line 1323 "c-exp.y"
    { (yyval.sval) = operator_stoken ("[]"); }
    break;

  case 214:
#line 1325 "c-exp.y"
    { char *name;
			  long length;
			  struct ui_file *buf = mem_fileopen ();

			  c_print_type ((yyvsp[(2) - (2)].tval), NULL, buf, -1, 0);
			  name = ui_file_xstrdup (buf, &length);
			  ui_file_delete (buf);
			  (yyval.sval) = operator_stoken (name);
			  xfree (name);
			}
    break;

  case 215:
#line 1339 "c-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].ssym).stoken; }
    break;

  case 216:
#line 1340 "c-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].ssym).stoken; }
    break;

  case 217:
#line 1341 "c-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].tsym).stoken; }
    break;

  case 218:
#line 1342 "c-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].ssym).stoken; }
    break;

  case 219:
#line 1343 "c-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].ssym).stoken; }
    break;

  case 220:
#line 1344 "c-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].sval); }
    break;

  case 223:
#line 1357 "c-exp.y"
    {
			  (yyval.ssym).stoken = (yyvsp[(1) - (1)].sval);
			  (yyval.ssym).sym = lookup_symbol ((yyvsp[(1) - (1)].sval).ptr,
						  expression_context_block,
						  VAR_DOMAIN,
						  &(yyval.ssym).is_a_field_of_this);
			}
    break;


/* Line 1267 of yacc.c.  */
#line 3491 "c-exp.c.tmp"
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


#line 1367 "c-exp.y"


/* Returns a stoken of the operator name given by OP (which does not
   include the string "operator").  */ 
static struct stoken
operator_stoken (const char *op)
{
  static const char *operator_string = "operator";
  struct stoken st = { NULL, 0 };
  st.length = strlen (operator_string) + strlen (op);
  st.ptr = xmalloc (st.length + 1);
  strcpy (st.ptr, operator_string);
  strcat (st.ptr, op);

  /* The toplevel (c_parse) will free the memory allocated here.  */
  make_cleanup (xfree, st.ptr);
  return st;
};

/* Take care of parsing a number (anything that starts with a digit).
   Set yylval and return the token type; update lexptr.
   LEN is the number of characters in it.  */

/*** Needs some error checking for the float case ***/

static int
parse_number (char *p, int len, int parsed_float, YYSTYPE *putithere)
{
  /* FIXME: Shouldn't these be unsigned?  We don't deal with negative values
     here, and we do kind of silly things like cast to unsigned.  */
  LONGEST n = 0;
  LONGEST prevn = 0;
  ULONGEST un;

  int i = 0;
  int c;
  int base = input_radix;
  int unsigned_p = 0;

  /* Number of "L" suffixes encountered.  */
  int long_p = 0;

  /* We have found a "L" or "U" suffix.  */
  int found_suffix = 0;

  ULONGEST high_bit;
  struct type *signed_type;
  struct type *unsigned_type;

  if (parsed_float)
    {
      /* It's a float since it contains a point or an exponent.  */
      char *s;
      int num;	/* number of tokens scanned by scanf */
      char saved_char;

      /* If it ends at "df", "dd" or "dl", take it as type of decimal floating
         point.  Return DECFLOAT.  */

      if (len >= 2 && p[len - 2] == 'd' && p[len - 1] == 'f')
	{
	  p[len - 2] = '\0';
	  putithere->typed_val_decfloat.type
	    = parse_type->builtin_decfloat;
	  decimal_from_string (putithere->typed_val_decfloat.val, 4,
			       gdbarch_byte_order (parse_gdbarch), p);
	  p[len - 2] = 'd';
	  return DECFLOAT;
	}

      if (len >= 2 && p[len - 2] == 'd' && p[len - 1] == 'd')
	{
	  p[len - 2] = '\0';
	  putithere->typed_val_decfloat.type
	    = parse_type->builtin_decdouble;
	  decimal_from_string (putithere->typed_val_decfloat.val, 8,
			       gdbarch_byte_order (parse_gdbarch), p);
	  p[len - 2] = 'd';
	  return DECFLOAT;
	}

      if (len >= 2 && p[len - 2] == 'd' && p[len - 1] == 'l')
	{
	  p[len - 2] = '\0';
	  putithere->typed_val_decfloat.type
	    = parse_type->builtin_declong;
	  decimal_from_string (putithere->typed_val_decfloat.val, 16,
			       gdbarch_byte_order (parse_gdbarch), p);
	  p[len - 2] = 'd';
	  return DECFLOAT;
	}

      s = xmalloc (len);
      saved_char = p[len];
      p[len] = 0;	/* null-terminate the token */
      num = sscanf (p, "%" DOUBLEST_SCAN_FORMAT "%s",
		    &putithere->typed_val_float.dval, s);
      p[len] = saved_char;	/* restore the input stream */

      if (num == 0)
	{
	  xfree (s);
	  return ERROR;
	}

      if (num == 1)
	putithere->typed_val_float.type = 
	  parse_type->builtin_double;

      if (num == 2 )
	{
	  /* See if it has any float suffix: 'f' for float, 'l' for long 
	     double.  */
	  if (!strcasecmp (s, "f"))
	    putithere->typed_val_float.type = 
	      parse_type->builtin_float;
	  else if (!strcasecmp (s, "l"))
	    putithere->typed_val_float.type = 
	      parse_type->builtin_long_double;
	  else
	    {
	      xfree (s);
	      return ERROR;
	    }
	}

      xfree (s);
      return FLOAT;
    }

  /* Handle base-switching prefixes 0x, 0t, 0d, 0 */
  if (p[0] == '0')
    switch (p[1])
      {
      case 'x':
      case 'X':
	if (len >= 3)
	  {
	    p += 2;
	    base = 16;
	    len -= 2;
	  }
	break;

      case 'b':
      case 'B':
	if (len >= 3)
	  {
	    p += 2;
	    base = 2;
	    len -= 2;
	  }
	break;

      case 't':
      case 'T':
      case 'd':
      case 'D':
	if (len >= 3)
	  {
	    p += 2;
	    base = 10;
	    len -= 2;
	  }
	break;

      default:
	base = 8;
	break;
      }

  while (len-- > 0)
    {
      c = *p++;
      if (c >= 'A' && c <= 'Z')
	c += 'a' - 'A';
      if (c != 'l' && c != 'u')
	n *= base;
      if (c >= '0' && c <= '9')
	{
	  if (found_suffix)
	    return ERROR;
	  n += i = c - '0';
	}
      else
	{
	  if (base > 10 && c >= 'a' && c <= 'f')
	    {
	      if (found_suffix)
		return ERROR;
	      n += i = c - 'a' + 10;
	    }
	  else if (c == 'l')
	    {
	      ++long_p;
	      found_suffix = 1;
	    }
	  else if (c == 'u')
	    {
	      unsigned_p = 1;
	      found_suffix = 1;
	    }
	  else
	    return ERROR;	/* Char not a digit */
	}
      if (i >= base)
	return ERROR;		/* Invalid digit in this base */

      /* Portably test for overflow (only works for nonzero values, so make
	 a second check for zero).  FIXME: Can't we just make n and prevn
	 unsigned and avoid this?  */
      if (c != 'l' && c != 'u' && (prevn >= n) && n != 0)
	unsigned_p = 1;		/* Try something unsigned */

      /* Portably test for unsigned overflow.
	 FIXME: This check is wrong; for example it doesn't find overflow
	 on 0x123456789 when LONGEST is 32 bits.  */
      if (c != 'l' && c != 'u' && n != 0)
	{	
	  if ((unsigned_p && (ULONGEST) prevn >= (ULONGEST) n))
	    error ("Numeric constant too large.");
	}
      prevn = n;
    }

  /* An integer constant is an int, a long, or a long long.  An L
     suffix forces it to be long; an LL suffix forces it to be long
     long.  If not forced to a larger size, it gets the first type of
     the above that it fits in.  To figure out whether it fits, we
     shift it right and see whether anything remains.  Note that we
     can't shift sizeof (LONGEST) * HOST_CHAR_BIT bits or more in one
     operation, because many compilers will warn about such a shift
     (which always produces a zero result).  Sometimes gdbarch_int_bit
     or gdbarch_long_bit will be that big, sometimes not.  To deal with
     the case where it is we just always shift the value more than
     once, with fewer bits each time.  */

  un = (ULONGEST)n >> 2;
  if (long_p == 0
      && (un >> (gdbarch_int_bit (parse_gdbarch) - 2)) == 0)
    {
      high_bit = ((ULONGEST)1) << (gdbarch_int_bit (parse_gdbarch) - 1);

      /* A large decimal (not hex or octal) constant (between INT_MAX
	 and UINT_MAX) is a long or unsigned long, according to ANSI,
	 never an unsigned int, but this code treats it as unsigned
	 int.  This probably should be fixed.  GCC gives a warning on
	 such constants.  */

      unsigned_type = parse_type->builtin_unsigned_int;
      signed_type = parse_type->builtin_int;
    }
  else if (long_p <= 1
	   && (un >> (gdbarch_long_bit (parse_gdbarch) - 2)) == 0)
    {
      high_bit = ((ULONGEST)1) << (gdbarch_long_bit (parse_gdbarch) - 1);
      unsigned_type = parse_type->builtin_unsigned_long;
      signed_type = parse_type->builtin_long;
    }
  else
    {
      int shift;
      if (sizeof (ULONGEST) * HOST_CHAR_BIT 
	  < gdbarch_long_long_bit (parse_gdbarch))
	/* A long long does not fit in a LONGEST.  */
	shift = (sizeof (ULONGEST) * HOST_CHAR_BIT - 1);
      else
	shift = (gdbarch_long_long_bit (parse_gdbarch) - 1);
      high_bit = (ULONGEST) 1 << shift;
      unsigned_type = parse_type->builtin_unsigned_long_long;
      signed_type = parse_type->builtin_long_long;
    }

   putithere->typed_val_int.val = n;

   /* If the high bit of the worked out type is set then this number
      has to be unsigned. */

   if (unsigned_p || (n & high_bit)) 
     {
       putithere->typed_val_int.type = unsigned_type;
     }
   else 
     {
       putithere->typed_val_int.type = signed_type;
     }

   return INT;
}

/* Temporary obstack used for holding strings.  */
static struct obstack tempbuf;
static int tempbuf_init;

/* Parse a C escape sequence.  The initial backslash of the sequence
   is at (*PTR)[-1].  *PTR will be updated to point to just after the
   last character of the sequence.  If OUTPUT is not NULL, the
   translated form of the escape sequence will be written there.  If
   OUTPUT is NULL, no output is written and the call will only affect
   *PTR.  If an escape sequence is expressed in target bytes, then the
   entire sequence will simply be copied to OUTPUT.  Return 1 if any
   character was emitted, 0 otherwise.  */

int
c_parse_escape (char **ptr, struct obstack *output)
{
  char *tokptr = *ptr;
  int result = 1;

  /* Some escape sequences undergo character set conversion.  Those we
     translate here.  */
  switch (*tokptr)
    {
      /* Hex escapes do not undergo character set conversion, so keep
	 the escape sequence for later.  */
    case 'x':
      if (output)
	obstack_grow_str (output, "\\x");
      ++tokptr;
      if (!isxdigit (*tokptr))
	error (_("\\x escape without a following hex digit"));
      while (isxdigit (*tokptr))
	{
	  if (output)
	    obstack_1grow (output, *tokptr);
	  ++tokptr;
	}
      break;

      /* Octal escapes do not undergo character set conversion, so
	 keep the escape sequence for later.  */
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
      {
	int i;
	if (output)
	  obstack_grow_str (output, "\\");
	for (i = 0;
	     i < 3 && isdigit (*tokptr) && *tokptr != '8' && *tokptr != '9';
	     ++i)
	  {
	    if (output)
	      obstack_1grow (output, *tokptr);
	    ++tokptr;
	  }
      }
      break;

      /* We handle UCNs later.  We could handle them here, but that
	 would mean a spurious error in the case where the UCN could
	 be converted to the target charset but not the host
	 charset.  */
    case 'u':
    case 'U':
      {
	char c = *tokptr;
	int i, len = c == 'U' ? 8 : 4;
	if (output)
	  {
	    obstack_1grow (output, '\\');
	    obstack_1grow (output, *tokptr);
	  }
	++tokptr;
	if (!isxdigit (*tokptr))
	  error (_("\\%c escape without a following hex digit"), c);
	for (i = 0; i < len && isxdigit (*tokptr); ++i)
	  {
	    if (output)
	      obstack_1grow (output, *tokptr);
	    ++tokptr;
	  }
      }
      break;

      /* We must pass backslash through so that it does not
	 cause quoting during the second expansion.  */
    case '\\':
      if (output)
	obstack_grow_str (output, "\\\\");
      ++tokptr;
      break;

      /* Escapes which undergo conversion.  */
    case 'a':
      if (output)
	obstack_1grow (output, '\a');
      ++tokptr;
      break;
    case 'b':
      if (output)
	obstack_1grow (output, '\b');
      ++tokptr;
      break;
    case 'f':
      if (output)
	obstack_1grow (output, '\f');
      ++tokptr;
      break;
    case 'n':
      if (output)
	obstack_1grow (output, '\n');
      ++tokptr;
      break;
    case 'r':
      if (output)
	obstack_1grow (output, '\r');
      ++tokptr;
      break;
    case 't':
      if (output)
	obstack_1grow (output, '\t');
      ++tokptr;
      break;
    case 'v':
      if (output)
	obstack_1grow (output, '\v');
      ++tokptr;
      break;

      /* GCC extension.  */
    case 'e':
      if (output)
	obstack_1grow (output, HOST_ESCAPE_CHAR);
      ++tokptr;
      break;

      /* Backslash-newline expands to nothing at all.  */
    case '\n':
      ++tokptr;
      result = 0;
      break;

      /* A few escapes just expand to the character itself.  */
    case '\'':
    case '\"':
    case '?':
      /* GCC extensions.  */
    case '(':
    case '{':
    case '[':
    case '%':
      /* Unrecognized escapes turn into the character itself.  */
    default:
      if (output)
	obstack_1grow (output, *tokptr);
      ++tokptr;
      break;
    }
  *ptr = tokptr;
  return result;
}

/* Parse a string or character literal from TOKPTR.  The string or
   character may be wide or unicode.  *OUTPTR is set to just after the
   end of the literal in the input string.  The resulting token is
   stored in VALUE.  This returns a token value, either STRING or
   CHAR, depending on what was parsed.  *HOST_CHARS is set to the
   number of host characters in the literal.  */
static int
parse_string_or_char (char *tokptr, char **outptr, struct typed_stoken *value,
		      int *host_chars)
{
  int quote;
  enum c_string_type type;

  /* Build the gdb internal form of the input string in tempbuf.  Note
     that the buffer is null byte terminated *only* for the
     convenience of debugging gdb itself and printing the buffer
     contents when the buffer contains no embedded nulls.  Gdb does
     not depend upon the buffer being null byte terminated, it uses
     the length string instead.  This allows gdb to handle C strings
     (as well as strings in other languages) with embedded null
     bytes */

  if (!tempbuf_init)
    tempbuf_init = 1;
  else
    obstack_free (&tempbuf, NULL);
  obstack_init (&tempbuf);

  /* Record the string type.  */
  if (*tokptr == 'L')
    {
      type = C_WIDE_STRING;
      ++tokptr;
    }
  else if (*tokptr == 'u')
    {
      type = C_STRING_16;
      ++tokptr;
    }
  else if (*tokptr == 'U')
    {
      type = C_STRING_32;
      ++tokptr;
    }
  else
    type = C_STRING;

  /* Skip the quote.  */
  quote = *tokptr;
  if (quote == '\'')
    type |= C_CHAR;
  ++tokptr;

  *host_chars = 0;

  while (*tokptr)
    {
      char c = *tokptr;
      if (c == '\\')
	{
	  ++tokptr;
	  *host_chars += c_parse_escape (&tokptr, &tempbuf);
	}
      else if (c == quote)
	break;
      else
	{
	  obstack_1grow (&tempbuf, c);
	  ++tokptr;
	  /* FIXME: this does the wrong thing with multi-byte host
	     characters.  We could use mbrlen here, but that would
	     make "set host-charset" a bit less useful.  */
	  ++*host_chars;
	}
    }

  if (*tokptr != quote)
    {
      if (quote == '"')
	error ("Unterminated string in expression.");
      else
	error ("Unmatched single quote.");
    }
  ++tokptr;

  value->type = type;
  value->ptr = obstack_base (&tempbuf);
  value->length = obstack_object_size (&tempbuf);

  *outptr = tokptr;

  return quote == '"' ? STRING : CHAR;
}

struct token
{
  char *operator;
  int token;
  enum exp_opcode opcode;
  int cxx_only;
};

static const struct token tokentab3[] =
  {
    {">>=", ASSIGN_MODIFY, BINOP_RSH, 0},
    {"<<=", ASSIGN_MODIFY, BINOP_LSH, 0},
    {"->*", ARROW_STAR, BINOP_END, 1}
  };

static const struct token tokentab2[] =
  {
    {"+=", ASSIGN_MODIFY, BINOP_ADD, 0},
    {"-=", ASSIGN_MODIFY, BINOP_SUB, 0},
    {"*=", ASSIGN_MODIFY, BINOP_MUL, 0},
    {"/=", ASSIGN_MODIFY, BINOP_DIV, 0},
    {"%=", ASSIGN_MODIFY, BINOP_REM, 0},
    {"|=", ASSIGN_MODIFY, BINOP_BITWISE_IOR, 0},
    {"&=", ASSIGN_MODIFY, BINOP_BITWISE_AND, 0},
    {"^=", ASSIGN_MODIFY, BINOP_BITWISE_XOR, 0},
    {"++", INCREMENT, BINOP_END, 0},
    {"--", DECREMENT, BINOP_END, 0},
    {"->", ARROW, BINOP_END, 0},
    {"&&", ANDAND, BINOP_END, 0},
    {"||", OROR, BINOP_END, 0},
    /* "::" is *not* only C++: gdb overrides its meaning in several
       different ways, e.g., 'filename'::func, function::variable.  */
    {"::", COLONCOLON, BINOP_END, 0},
    {"<<", LSH, BINOP_END, 0},
    {">>", RSH, BINOP_END, 0},
    {"==", EQUAL, BINOP_END, 0},
    {"!=", NOTEQUAL, BINOP_END, 0},
    {"<=", LEQ, BINOP_END, 0},
    {">=", GEQ, BINOP_END, 0},
    {".*", DOT_STAR, BINOP_END, 1}
  };

/* Identifier-like tokens.  */
static const struct token ident_tokens[] =
  {
    {"unsigned", UNSIGNED, OP_NULL, 0},
    {"template", TEMPLATE, OP_NULL, 1},
    {"volatile", VOLATILE_KEYWORD, OP_NULL, 0},
    {"struct", STRUCT, OP_NULL, 0},
    {"signed", SIGNED_KEYWORD, OP_NULL, 0},
    {"sizeof", SIZEOF, OP_NULL, 0},
    {"double", DOUBLE_KEYWORD, OP_NULL, 0},
    {"false", FALSEKEYWORD, OP_NULL, 1},
    {"class", CLASS, OP_NULL, 1},
    {"union", UNION, OP_NULL, 0},
    {"short", SHORT, OP_NULL, 0},
    {"const", CONST_KEYWORD, OP_NULL, 0},
    {"enum", ENUM, OP_NULL, 0},
    {"long", LONG, OP_NULL, 0},
    {"true", TRUEKEYWORD, OP_NULL, 1},
    {"int", INT_KEYWORD, OP_NULL, 0},
    {"new", NEW, OP_NULL, 1},
    {"delete", DELETE, OP_NULL, 1},
    {"operator", OPERATOR, OP_NULL, 1},
    {"void", VOID_KEYWORD, OP_NULL, 0},

    {"and", ANDAND, BINOP_END, 1},
    {"and_eq", ASSIGN_MODIFY, BINOP_BITWISE_AND, 1},
    {"bitand", '&', OP_NULL, 1},
    {"bitor", '|', OP_NULL, 1},
    {"compl", '~', OP_NULL, 1},
    {"not", '!', OP_NULL, 1},
    {"not_eq", NOTEQUAL, BINOP_END, 1},
    {"or", OROR, BINOP_END, 1},
    {"or_eq", ASSIGN_MODIFY, BINOP_BITWISE_IOR, 1},
    {"xor", '^', OP_NULL, 1},
    {"xor_eq", ASSIGN_MODIFY, BINOP_BITWISE_XOR, 1},

    {"const_cast", CONST_CAST, OP_NULL, 1 },
    {"dynamic_cast", DYNAMIC_CAST, OP_NULL, 1 },
    {"static_cast", STATIC_CAST, OP_NULL, 1 },
    {"reinterpret_cast", REINTERPRET_CAST, OP_NULL, 1 },

    {"shared", SHARED_KEYWORD, OP_NULL, 0 },
    {"relaxed", RELAXED_KEYWORD, OP_NULL, 0 },
    {"strict", STRICT_KEYWORD, OP_NULL, 0 }
  };

/* When we find that lexptr (the global var defined in parse.c) is
   pointing at a macro invocation, we expand the invocation, and call
   scan_macro_expansion to save the old lexptr here and point lexptr
   into the expanded text.  When we reach the end of that, we call
   end_macro_expansion to pop back to the value we saved here.  The
   macro expansion code promises to return only fully-expanded text,
   so we don't need to "push" more than one level.

   This is disgusting, of course.  It would be cleaner to do all macro
   expansion beforehand, and then hand that to lexptr.  But we don't
   really know where the expression ends.  Remember, in a command like

     (gdb) break *ADDRESS if CONDITION

   we evaluate ADDRESS in the scope of the current frame, but we
   evaluate CONDITION in the scope of the breakpoint's location.  So
   it's simply wrong to try to macro-expand the whole thing at once.  */
static char *macro_original_text;

/* We save all intermediate macro expansions on this obstack for the
   duration of a single parse.  The expansion text may sometimes have
   to live past the end of the expansion, due to yacc lookahead.
   Rather than try to be clever about saving the data for a single
   token, we simply keep it all and delete it after parsing has
   completed.  */
static struct obstack expansion_obstack;

static void
scan_macro_expansion (char *expansion)
{
  char *copy;

  /* We'd better not be trying to push the stack twice.  */
  gdb_assert (! macro_original_text);

  /* Copy to the obstack, and then free the intermediate
     expansion.  */
  copy = obstack_copy0 (&expansion_obstack, expansion, strlen (expansion));
  xfree (expansion);

  /* Save the old lexptr value, so we can return to it when we're done
     parsing the expanded text.  */
  macro_original_text = lexptr;
  lexptr = copy;
}


static int
scanning_macro_expansion (void)
{
  return macro_original_text != 0;
}


static void 
finished_macro_expansion (void)
{
  /* There'd better be something to pop back to.  */
  gdb_assert (macro_original_text);

  /* Pop back to the original text.  */
  lexptr = macro_original_text;
  macro_original_text = 0;
}


static void
scan_macro_cleanup (void *dummy)
{
  if (macro_original_text)
    finished_macro_expansion ();

  obstack_free (&expansion_obstack, NULL);
}

/* Return true iff the token represents a C++ cast operator.  */

static int
is_cast_operator (const char *token, int len)
{
  return (! strncmp (token, "dynamic_cast", len)
	  || ! strncmp (token, "static_cast", len)
	  || ! strncmp (token, "reinterpret_cast", len)
	  || ! strncmp (token, "const_cast", len));
}

/* The scope used for macro expansion.  */
static struct macro_scope *expression_macro_scope;

/* This is set if a NAME token appeared at the very end of the input
   string, with no whitespace separating the name from the EOF.  This
   is used only when parsing to do field name completion.  */
static int saw_name_at_eof;

/* This is set if the previously-returned token was a structure
   operator -- either '.' or ARROW.  This is used only when parsing to
   do field name completion.  */
static int last_was_structop;

static const struct token intrinsics[] =
  {
    {"isinf", UNOP_INTRINSIC, UNOP_ISINF},
    {"isnan", UNOP_INTRINSIC, UNOP_ISNAN},
    {"isfinite", UNOP_INTRINSIC, UNOP_ISFINITE},
    {"isnormal", UNOP_INTRINSIC, UNOP_ISNORMAL},
    {"creal", UNOP_INTRINSIC, UNOP_CREAL},
    {"cimag", UNOP_INTRINSIC, UNOP_CIMAG},
    {"fabs", UNOP_INTRINSIC, UNOP_FABS},
    {"fmod", BINOP_INTRINSIC, BINOP_FMOD},
    {"ceil", UNOP_INTRINSIC, UNOP_CEIL},
    {"floor", UNOP_INTRINSIC, UNOP_FLOOR},
  };

/* Read one token, getting characters through lexptr.  */

static int
lex_one_token (void)
{
  int c;
  int namelen;
  unsigned int i;
  char *tokstart;
  int saw_structop = last_was_structop;
  char *copy;

  last_was_structop = 0;

 retry:

  /* Check if this is a macro invocation that we need to expand.  */
  if (! scanning_macro_expansion ())
    {
      char *expanded = macro_expand_next (&lexptr,
                                          standard_macro_lookup,
                                          expression_macro_scope);

      if (expanded)
        scan_macro_expansion (expanded);
    }

  prev_lexptr = lexptr;

  tokstart = lexptr;
  /* See if it is a special token of length 3.  */
  for (i = 0; i < sizeof tokentab3 / sizeof tokentab3[0]; i++)
    if (strncmp (tokstart, tokentab3[i].operator, 3) == 0)
      {
	if (tokentab3[i].cxx_only
	    && parse_language->la_language != language_cplus)
	  break;

	lexptr += 3;
	yylval.opcode = tokentab3[i].opcode;
	return tokentab3[i].token;
      }

  /* See if it is a special token of length 2.  */
  for (i = 0; i < sizeof tokentab2 / sizeof tokentab2[0]; i++)
    if (strncmp (tokstart, tokentab2[i].operator, 2) == 0)
      {
	if (tokentab2[i].cxx_only
	    && parse_language->la_language != language_cplus)
	  break;

	lexptr += 2;
	yylval.opcode = tokentab2[i].opcode;
	if (in_parse_field && tokentab2[i].token == ARROW)
	  last_was_structop = 1;
	return tokentab2[i].token;
      }

  switch (c = *tokstart)
    {
    case 0:
      /* If we were just scanning the result of a macro expansion,
         then we need to resume scanning the original text.
	 If we're parsing for field name completion, and the previous
	 token allows such completion, return a COMPLETE token.
         Otherwise, we were already scanning the original text, and
         we're really done.  */
      if (scanning_macro_expansion ())
        {
          finished_macro_expansion ();
          goto retry;
        }
      else if (saw_name_at_eof)
	{
	  saw_name_at_eof = 0;
	  return COMPLETE;
	}
      else if (saw_structop)
	return COMPLETE;
      else
        return 0;

    case ' ':
    case '\t':
    case '\n':
      lexptr++;
      goto retry;

    case '[':
    case '(':
      paren_depth++;
      lexptr++;
      return c;

    case ']':
    case ')':
      if (paren_depth == 0)
	return 0;
      paren_depth--;
      lexptr++;
      return c;

    case ',':
      if (comma_terminates
          && paren_depth == 0
          && ! scanning_macro_expansion ())
	return 0;
      lexptr++;
      return c;

    case '.':
      /* Might be a floating point number.  */
      if (lexptr[1] < '0' || lexptr[1] > '9')
	{
	  if (in_parse_field)
	    last_was_structop = 1;
	  goto symbol;		/* Nope, must be a symbol. */
	}
      /* FALL THRU into number case.  */

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      {
	/* It's a number.  */
	int got_dot = 0, got_e = 0, toktype;
	char *p = tokstart;
	int hex = input_radix > 10;

	if (c == '0' && (p[1] == 'x' || p[1] == 'X'))
	  {
	    p += 2;
	    hex = 1;
	  }
	else if (c == '0' && (p[1]=='t' || p[1]=='T' || p[1]=='d' || p[1]=='D'))
	  {
	    p += 2;
	    hex = 0;
	  }

	for (;; ++p)
	  {
	    /* This test includes !hex because 'e' is a valid hex digit
	       and thus does not indicate a floating point number when
	       the radix is hex.  */
	    if (!hex && !got_e && (*p == 'e' || *p == 'E'))
	      got_dot = got_e = 1;
	    /* This test does not include !hex, because a '.' always indicates
	       a decimal floating point number regardless of the radix.  */
	    else if (!got_dot && *p == '.')
	      got_dot = 1;
	    else if (got_e && (p[-1] == 'e' || p[-1] == 'E')
		     && (*p == '-' || *p == '+'))
	      /* This is the sign of the exponent, not the end of the
		 number.  */
	      continue;
	    /* We will take any letters or digits.  parse_number will
	       complain if past the radix, or if L or U are not final.  */
	    else if ((*p < '0' || *p > '9')
		     && ((*p < 'a' || *p > 'z')
				  && (*p < 'A' || *p > 'Z')))
	      break;
	  }
	toktype = parse_number (tokstart, p - tokstart, got_dot|got_e, &yylval);
        if (toktype == ERROR)
	  {
	    char *err_copy = (char *) alloca (p - tokstart + 1);

	    memcpy (err_copy, tokstart, p - tokstart);
	    err_copy[p - tokstart] = 0;
	    error ("Invalid number \"%s\".", err_copy);
	  }
	lexptr = p;
	return toktype;
      }

    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '|':
    case '&':
    case '^':
    case '~':
    case '!':
    case '@':
    case '<':
    case '>':
    case '?':
    case ':':
    case '=':
    case '{':
    case '}':
    symbol:
      lexptr++;
      return c;

    case 'L':
    case 'u':
    case 'U':
      if (tokstart[1] != '"' && tokstart[1] != '\'')
	break;
      /* Fall through.  */
    case '\'':
    case '"':
      {
	int host_len;
	int result = parse_string_or_char (tokstart, &lexptr, &yylval.tsval,
					   &host_len);
	if (result == CHAR)
	  {
	    if (host_len == 0)
	      error ("Empty character constant.");
	    else if (host_len > 2 && c == '\'')
	      {
		++tokstart;
		namelen = lexptr - tokstart - 1;
		goto tryname;
	      }
	    else if (host_len > 1)
	      error ("Invalid character constant.");
	  }
	return result;
      }
    }

  if (!(c == '_' || c == '$'
	|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
    /* We must have come across a bad character (e.g. ';').  */
    error ("Invalid character '%c' in expression.", c);

  /* It's a name.  See how long it is.  */
  namelen = 0;
  for (c = tokstart[namelen];
       (c == '_' || c == '$' || (c >= '0' && c <= '9')
	|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '<');)
    {
      /* Template parameter lists are part of the name.
	 FIXME: This mishandles `print $a<4&&$a>3'.  */

      if (c == '<')
	{
	  if (! is_cast_operator (tokstart, namelen))
	    {
	      /* Scan ahead to get rest of the template specification.  Note
		 that we look ahead only when the '<' adjoins non-whitespace
		 characters; for comparison expressions, e.g. "a < b > c",
		 there must be spaces before the '<', etc. */
               
	      char * p = find_template_name_end (tokstart + namelen);
	      if (p)
		namelen = p - tokstart;
	    }
	  break;
	}
      c = tokstart[++namelen];
    }

  /* The token "if" terminates the expression and is NOT removed from
     the input stream.  It doesn't count if it appears in the
     expansion of a macro.  */
  if (namelen == 2
      && tokstart[0] == 'i'
      && tokstart[1] == 'f'
      && ! scanning_macro_expansion ())
    {
      return 0;
    }

  /* For the same reason (breakpoint conditions), "thread N"
     terminates the expression.  "thread" could be an identifier, but
     an identifier is never followed by a number without intervening
     punctuation.  "task" is similar.  Handle abbreviations of these,
     similarly to breakpoint.c:find_condition_and_thread.  */
  if (namelen >= 1
      && (strncmp (tokstart, "thread", namelen) == 0
	  || strncmp (tokstart, "task", namelen) == 0
	  || strncmp (tokstart, "inf", namelen) == 0)
      && (tokstart[namelen] == ' ' || tokstart[namelen] == '\t')
      && ! scanning_macro_expansion ())
    {
      char *p = tokstart + namelen + 1;
      while (*p == ' ' || *p == '\t')
	p++;
      if (*p >= '0' && *p <= '9')
	return 0;
    }

  lexptr += namelen;

  tryname:

  for (i = 0; i < sizeof intrinsics / sizeof intrinsics[0]; i++)
    if (strncmp (tokstart, intrinsics[i].operator, strlen(intrinsics[i].operator)) == 0
	&& strlen(intrinsics[i].operator) == namelen)
      {
        yylval.opcode = intrinsics[i].opcode;
        return intrinsics[i].token;
      }

  yylval.sval.ptr = tokstart;
  yylval.sval.length = namelen;

  /* Catch specific keywords.  */
  copy = copy_name (yylval.sval);
  for (i = 0; i < sizeof ident_tokens / sizeof ident_tokens[0]; i++)
    if (strcmp (copy, ident_tokens[i].operator) == 0)
      {
	if (ident_tokens[i].cxx_only
	    && parse_language->la_language != language_cplus)
	  break;

	/* It is ok to always set this, even though we don't always
	   strictly need to.  */
	yylval.opcode = ident_tokens[i].opcode;
	return ident_tokens[i].token;
      }

  if (*tokstart == '$')
    return VARIABLE;

  if (in_parse_field && *lexptr == '\0')
    saw_name_at_eof = 1;
  return NAME;
}

/* An object of this type is pushed on a FIFO by the "outer" lexer.  */
typedef struct
{
  int token;
  YYSTYPE value;
} token_and_value;

DEF_VEC_O (token_and_value);

/* A FIFO of tokens that have been read but not yet returned to the
   parser.  */
static VEC (token_and_value) *token_fifo;

/* Non-zero if the lexer should return tokens from the FIFO.  */
static int popping;

/* Temporary storage for c_lex; this holds symbol names as they are
   built up.  */
static struct obstack name_obstack;

/* Classify a NAME token.  The contents of the token are in `yylval'.
   Updates yylval and returns the new token type.  BLOCK is the block
   in which lookups start; this can be NULL to mean the global
   scope.  */
static int
classify_name (struct block *block)
{
  struct symbol *sym;
  char *copy;
  int is_a_field_of_this = 0;

  copy = copy_name (yylval.sval);

  sym = lookup_symbol (copy, block, VAR_DOMAIN, 
		       parse_language->la_language == language_cplus
		       ? &is_a_field_of_this : (int *) NULL);

  if (sym && SYMBOL_CLASS (sym) == LOC_BLOCK)
    {
      yylval.ssym.sym = sym;
      yylval.ssym.is_a_field_of_this = is_a_field_of_this;
      return BLOCKNAME;
    }
  else if (!sym)
    {
      /* See if it's a file name. */
      struct symtab *symtab;

      symtab = lookup_symtab (copy);
      if (symtab)
	{
	  yylval.bval = BLOCKVECTOR_BLOCK (BLOCKVECTOR (symtab), STATIC_BLOCK);
	  return FILENAME;
	}
    }

  if (sym && SYMBOL_CLASS (sym) == LOC_TYPEDEF)
    {
      yylval.tsym.type = SYMBOL_TYPE (sym);
      return TYPENAME;
    }

  yylval.tsym.type
    = language_lookup_primitive_type_by_name (parse_language,
					      parse_gdbarch, copy);
  if (yylval.tsym.type != NULL)
    return TYPENAME;

  /* Input names that aren't symbols but ARE valid hex numbers, when
     the input radix permits them, can be names or numbers depending
     on the parse.  Note we support radixes > 16 here.  */
  if (!sym
      && ((copy[0] >= 'a' && copy[0] < 'a' + input_radix - 10)
	  || (copy[0] >= 'A' && copy[0] < 'A' + input_radix - 10)))
    {
      YYSTYPE newlval;	/* Its value is ignored.  */
      int hextype = parse_number (copy, yylval.sval.length, 0, &newlval);
      if (hextype == INT)
	{
	  yylval.ssym.sym = sym;
	  yylval.ssym.is_a_field_of_this = is_a_field_of_this;
	  return NAME_OR_INT;
	}
    }

  /* Any other kind of symbol */
  yylval.ssym.sym = sym;
  yylval.ssym.is_a_field_of_this = is_a_field_of_this;

  if (sym == NULL
      && parse_language->la_language == language_cplus
      && !lookup_minimal_symbol (copy, NULL, NULL))
    return UNKNOWN_CPP_NAME;

  return NAME;
}

/* Like classify_name, but used by the inner loop of the lexer, when a
   name might have already been seen.  FIRST_NAME is true if the token
   in `yylval' is the first component of a name, false otherwise.  If
   this function returns NAME, it might not have updated `yylval'.
   This is ok because the caller only cares about TYPENAME.  */
static int
classify_inner_name (struct block *block, int first_name)
{
  struct type *type, *new_type;
  char *copy;

  if (first_name)
    return classify_name (block);

  type = check_typedef (yylval.tsym.type);
  if (TYPE_CODE (type) != TYPE_CODE_STRUCT
      && TYPE_CODE (type) != TYPE_CODE_UNION
      && TYPE_CODE (type) != TYPE_CODE_NAMESPACE)
    /* We know the caller won't expect us to update yylval.  */
    return NAME;

  copy = copy_name (yylval.tsym.stoken);
  new_type = cp_lookup_nested_type (type, copy, block);

  if (new_type == NULL)
    /* We know the caller won't expect us to update yylval.  */
    return NAME;

  yylval.tsym.type = new_type;
  return TYPENAME;
}

/* The outer level of a two-level lexer.  This calls the inner lexer
   to return tokens.  It then either returns these tokens, or
   aggregates them into a larger token.  This lets us work around a
   problem in our parsing approach, where the parser could not
   distinguish between qualified names and qualified types at the
   right point.
   
   This approach is still not ideal, because it mishandles template
   types.  See the comment in lex_one_token for an example.  However,
   this is still an improvement over the earlier approach, and will
   suffice until we move to better parsing technology.  */
static int
yylex (void)
{
  token_and_value current;
  int first_was_coloncolon, last_was_coloncolon, first_iter;

  if (popping && !VEC_empty (token_and_value, token_fifo))
    {
      token_and_value tv = *VEC_index (token_and_value, token_fifo, 0);
      VEC_ordered_remove (token_and_value, token_fifo, 0);
      yylval = tv.value;
      return tv.token;
    }
  popping = 0;

  current.token = lex_one_token ();
  if (current.token == NAME)
    current.token = classify_name (expression_context_block);
  if (parse_language->la_language != language_cplus
      || (current.token != TYPENAME && current.token != COLONCOLON))
    return current.token;

  first_was_coloncolon = current.token == COLONCOLON;
  last_was_coloncolon = first_was_coloncolon;
  obstack_free (&name_obstack, obstack_base (&name_obstack));
  if (!last_was_coloncolon)
    obstack_grow (&name_obstack, yylval.sval.ptr, yylval.sval.length);
  current.value = yylval;
  first_iter = 1;
  while (1)
    {
      token_and_value next;

      next.token = lex_one_token ();
      next.value = yylval;

      if (next.token == NAME && last_was_coloncolon)
	{
	  int classification;

	  classification = classify_inner_name (first_was_coloncolon
						? NULL
						: expression_context_block,
						first_iter);
	  /* We keep going until we either run out of names, or until
	     we have a qualified name which is not a type.  */
	  if (classification != TYPENAME)
	    {
	      /* Push the final component and leave the loop.  */
	      VEC_safe_push (token_and_value, token_fifo, &next);
	      break;
	    }

	  /* Update the partial name we are constructing.  */
	  if (!first_iter)
	    {
	      /* We don't want to put a leading "::" into the name.  */
	      obstack_grow_str (&name_obstack, "::");
	    }
	  obstack_grow (&name_obstack, next.value.sval.ptr,
			next.value.sval.length);

	  yylval.sval.ptr = obstack_base (&name_obstack);
	  yylval.sval.length = obstack_object_size (&name_obstack);
	  current.value = yylval;
	  current.token = classification;

	  last_was_coloncolon = 0;
	}
      else if (next.token == COLONCOLON && !last_was_coloncolon)
	last_was_coloncolon = 1;
      else
	{
	  /* We've reached the end of the name.  */
	  VEC_safe_push (token_and_value, token_fifo, &next);
	  break;
	}

      first_iter = 0;
    }

  popping = 1;

  /* If we ended with a "::", insert it too.  */
  if (last_was_coloncolon)
    {
      token_and_value cc;
      memset (&cc, 0, sizeof (token_and_value));
      if (first_was_coloncolon && first_iter)
	{
	  yylval = cc.value;
	  return COLONCOLON;
	}
      cc.token = COLONCOLON;
      VEC_safe_insert (token_and_value, token_fifo, 0, &cc);
    }

  yylval = current.value;
  yylval.sval.ptr = obstack_copy0 (&expansion_obstack,
				   yylval.sval.ptr,
				   yylval.sval.length);
  return current.token;
}

int
c_parse (void)
{
  int result;
  struct cleanup *back_to = make_cleanup (free_current_contents,
					  &expression_macro_scope);

  /* Set up the scope for macro expansion.  */
  expression_macro_scope = NULL;

  if (expression_context_block)
    expression_macro_scope
      = sal_macro_scope (find_pc_line (expression_context_pc, 0));
  else
    expression_macro_scope = default_macro_scope ();
  if (! expression_macro_scope)
    expression_macro_scope = user_macro_scope ();

  /* Initialize macro expansion code.  */
  obstack_init (&expansion_obstack);
  gdb_assert (! macro_original_text);
  make_cleanup (scan_macro_cleanup, 0);

  make_cleanup_restore_integer (&yydebug);
  yydebug = parser_debug;

  /* Initialize some state used by the lexer.  */
  last_was_structop = 0;
  saw_name_at_eof = 0;

  VEC_free (token_and_value, token_fifo);
  popping = 0;
  obstack_init (&name_obstack);
  make_cleanup_obstack_free (&name_obstack);

  result = yyparse ();
  do_cleanups (back_to);
  return result;
}


void
yyerror (char *msg)
{
  if (prev_lexptr)
    lexptr = prev_lexptr;

  error ("A %s in expression, near `%s'.", (msg ? msg : "error"), lexptr);
}

