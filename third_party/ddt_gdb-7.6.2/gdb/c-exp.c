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
     NSSTRING = 262,
     SELECTOR = 263,
     CHAR = 264,
     NAME = 265,
     UNKNOWN_CPP_NAME = 266,
     COMPLETE = 267,
     TYPENAME = 268,
     CLASSNAME = 269,
     OBJC_LBRAC = 270,
     NAME_OR_INT = 271,
     OPERATOR = 272,
     STRUCT = 273,
     CLASS = 274,
     UNION = 275,
     ENUM = 276,
     SIZEOF = 277,
     UNSIGNED = 278,
     COLONCOLON = 279,
     TEMPLATE = 280,
     ERROR = 281,
     NEW = 282,
     DELETE = 283,
     REINTERPRET_CAST = 284,
     DYNAMIC_CAST = 285,
     STATIC_CAST = 286,
     CONST_CAST = 287,
     ENTRY = 288,
     TYPEOF = 289,
     DECLTYPE = 290,
     SIGNED_KEYWORD = 291,
     LONG = 292,
     SHORT = 293,
     INT_KEYWORD = 294,
     CONST_KEYWORD = 295,
     VOLATILE_KEYWORD = 296,
     DOUBLE_KEYWORD = 297,
     VOID_KEYWORD = 298,
     SHARED_KEYWORD = 299,
     RELAXED_KEYWORD = 300,
     STRICT_KEYWORD = 301,
     VARIABLE = 302,
     ASSIGN_MODIFY = 303,
     UNOP_INTRINSIC = 304,
     BINOP_INTRINSIC = 305,
     TRUEKEYWORD = 306,
     FALSEKEYWORD = 307,
     ABOVE_COMMA = 308,
     OROR = 309,
     ANDAND = 310,
     NOTEQUAL = 311,
     EQUAL = 312,
     GEQ = 313,
     LEQ = 314,
     RSH = 315,
     LSH = 316,
     DECREMENT = 317,
     INCREMENT = 318,
     UNARY = 319,
     DOT_STAR = 320,
     ARROW_STAR = 321,
     ARROW = 322,
     BLOCKNAME = 323,
     FILENAME = 324,
     DOTDOTDOT = 325
   };
#endif
/* Tokens.  */
#define INT 258
#define FLOAT 259
#define DECFLOAT 260
#define STRING 261
#define NSSTRING 262
#define SELECTOR 263
#define CHAR 264
#define NAME 265
#define UNKNOWN_CPP_NAME 266
#define COMPLETE 267
#define TYPENAME 268
#define CLASSNAME 269
#define OBJC_LBRAC 270
#define NAME_OR_INT 271
#define OPERATOR 272
#define STRUCT 273
#define CLASS 274
#define UNION 275
#define ENUM 276
#define SIZEOF 277
#define UNSIGNED 278
#define COLONCOLON 279
#define TEMPLATE 280
#define ERROR 281
#define NEW 282
#define DELETE 283
#define REINTERPRET_CAST 284
#define DYNAMIC_CAST 285
#define STATIC_CAST 286
#define CONST_CAST 287
#define ENTRY 288
#define TYPEOF 289
#define DECLTYPE 290
#define SIGNED_KEYWORD 291
#define LONG 292
#define SHORT 293
#define INT_KEYWORD 294
#define CONST_KEYWORD 295
#define VOLATILE_KEYWORD 296
#define DOUBLE_KEYWORD 297
#define VOID_KEYWORD 298
#define SHARED_KEYWORD 299
#define RELAXED_KEYWORD 300
#define STRICT_KEYWORD 301
#define VARIABLE 302
#define ASSIGN_MODIFY 303
#define UNOP_INTRINSIC 304
#define BINOP_INTRINSIC 305
#define TRUEKEYWORD 306
#define FALSEKEYWORD 307
#define ABOVE_COMMA 308
#define OROR 309
#define ANDAND 310
#define NOTEQUAL 311
#define EQUAL 312
#define GEQ 313
#define LEQ 314
#define RSH 315
#define LSH 316
#define DECREMENT 317
#define INCREMENT 318
#define UNARY 319
#define DOT_STAR 320
#define ARROW_STAR 321
#define ARROW 322
#define BLOCKNAME 323
#define FILENAME 324
#define DOTDOTDOT 325




/* Copy the first part of user declarations.  */
#line 39 "c-exp.y"


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
#include "objc-lang.h"
#include "typeprint.h"
#include "cp-abi.h"

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
#define yyss	c_yyss
#define yysslim	c_yysslim
#define yyssp	c_yyssp
#define yystacksize c_yystacksize
#define yyvs	c_yyvs
#define yyvsp	c_yyvsp

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
#line 137 "c-exp.y"
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
    VEC (type_ptr) *tvec;
    int *ivec;

    struct type_stack *type_stack;

    struct objc_class_str class;
  }
/* Line 187 of yacc.c.  */
#line 363 "c-exp.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
#line 171 "c-exp.y"

/* YYSTYPE gets defined by %union */
static int parse_number (char *, int, int, YYSTYPE *);
static struct stoken operator_stoken (const char *);
static void check_parameter_typelist (VEC (type_ptr) *);


/* Line 216 of yacc.c.  */
#line 382 "c-exp.c"

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
#define YYFINAL  181
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1774

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  95
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  268
/* YYNRULES -- Number of states.  */
#define YYNSTATES  425

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   325

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    89,     2,     2,     2,    75,    61,     2,
      81,    88,    73,    71,    53,    72,    79,    74,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    92,     2,
      64,    55,    65,    56,    70,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    80,     2,    91,    60,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    93,    59,    94,    90,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    54,    57,
      58,    62,    63,    66,    67,    68,    69,    76,    77,    78,
      82,    83,    84,    85,    86,    87
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    14,    19,    24,    26,
      30,    33,    36,    39,    42,    45,    48,    51,    54,    57,
      60,    63,    67,    72,    76,    80,    84,    88,    93,    97,
     101,   105,   110,   115,   116,   122,   123,   129,   130,   136,
     138,   140,   142,   145,   149,   152,   155,   156,   162,   163,
     169,   174,   181,   183,   184,   186,   190,   196,   198,   202,
     207,   212,   216,   220,   224,   228,   232,   236,   240,   244,
     248,   252,   256,   260,   264,   268,   272,   276,   280,   284,
     288,   292,   298,   302,   306,   308,   310,   312,   314,   316,
     318,   320,   325,   330,   338,   346,   354,   362,   364,   367,
     369,   371,   373,   375,   377,   379,   383,   386,   390,   394,
     399,   405,   407,   410,   412,   415,   417,   418,   422,   424,
     426,   428,   429,   430,   435,   436,   440,   442,   445,   447,
     450,   452,   454,   458,   461,   463,   466,   468,   471,   474,
     478,   482,   485,   489,   491,   493,   495,   497,   499,   502,
     506,   509,   513,   517,   521,   524,   527,   531,   536,   540,
     544,   549,   553,   558,   562,   567,   570,   574,   577,   581,
     584,   588,   590,   593,   595,   598,   601,   605,   608,   611,
     615,   618,   621,   625,   628,   631,   635,   638,   640,   643,
     645,   651,   654,   657,   659,   661,   663,   665,   667,   669,
     673,   675,   679,   682,   685,   688,   691,   693,   695,   697,
     699,   701,   704,   707,   709,   712,   715,   716,   719,   722,
     725,   727,   729,   732,   735,   740,   745,   750,   755,   758,
     761,   764,   767,   770,   773,   776,   779,   782,   785,   788,
     791,   794,   797,   800,   803,   806,   809,   812,   815,   818,
     821,   824,   827,   830,   833,   836,   840,   844,   848,   851,
     853,   855,   857,   859,   861,   863,   865,   867,   869
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      96,     0,    -1,    98,    -1,    97,    -1,   128,    -1,    34,
      81,    99,    88,    -1,    34,    81,   128,    88,    -1,    35,
      81,    99,    88,    -1,    99,    -1,    98,    53,    99,    -1,
      73,    99,    -1,    61,    99,    -1,    72,    99,    -1,    71,
      99,    -1,    89,    99,    -1,    90,    99,    -1,    77,    99,
      -1,    76,    99,    -1,    99,    77,    -1,    99,    76,    -1,
      22,    99,    -1,    99,    84,   142,    -1,    99,    84,   142,
      12,    -1,    99,    84,    12,    -1,    99,    84,   114,    -1,
      99,    83,    99,    -1,    99,    79,   142,    -1,    99,    79,
     142,    12,    -1,    99,    79,    12,    -1,    99,    79,   114,
      -1,    99,    82,    99,    -1,    99,    80,    98,    91,    -1,
      99,    15,    98,    91,    -1,    -1,    15,    13,   100,   103,
      91,    -1,    -1,    15,    14,   101,   103,    91,    -1,    -1,
      15,    99,   102,   103,    91,    -1,   142,    -1,   104,    -1,
     105,    -1,   104,   105,    -1,   142,    92,    99,    -1,    92,
      99,    -1,    53,    99,    -1,    -1,    99,    81,   106,   109,
      88,    -1,    -1,    11,    81,   107,   109,    88,    -1,    49,
      81,    99,    88,    -1,    50,    81,    99,    53,    99,    88,
      -1,    93,    -1,    -1,    99,    -1,   109,    53,    99,    -1,
      99,    81,   131,    88,   116,    -1,    94,    -1,   108,   109,
     110,    -1,   108,    97,   110,    99,    -1,    81,    97,    88,
      99,    -1,    81,    98,    88,    -1,    99,    70,    99,    -1,
      99,    73,    99,    -1,    99,    74,    99,    -1,    99,    75,
      99,    -1,    99,    71,    99,    -1,    99,    72,    99,    -1,
      99,    69,    99,    -1,    99,    68,    99,    -1,    99,    63,
      99,    -1,    99,    62,    99,    -1,    99,    67,    99,    -1,
      99,    66,    99,    -1,    99,    64,    99,    -1,    99,    65,
      99,    -1,    99,    61,    99,    -1,    99,    60,    99,    -1,
      99,    59,    99,    -1,    99,    58,    99,    -1,    99,    57,
      99,    -1,    99,    56,    99,    92,    99,    -1,    99,    55,
      99,    -1,    99,    48,    99,    -1,     3,    -1,     9,    -1,
      16,    -1,     4,    -1,     5,    -1,   113,    -1,    47,    -1,
       8,    81,   142,    88,    -1,    22,    81,   128,    88,    -1,
      29,    64,    97,    65,    81,    99,    88,    -1,    31,    64,
      97,    65,    81,    99,    88,    -1,    30,    64,    97,    65,
      81,    99,    88,    -1,    32,    64,    97,    65,    81,    99,
      88,    -1,     6,    -1,   111,     6,    -1,   111,    -1,     7,
      -1,    51,    -1,    52,    -1,    85,    -1,    86,    -1,   112,
      24,   142,    -1,   143,    33,    -1,   112,    24,   142,    -1,
      13,    24,   142,    -1,    13,    24,    90,   142,    -1,    13,
      24,   142,    24,   142,    -1,   114,    -1,    24,   143,    -1,
     143,    -1,    70,    10,    -1,   136,    -1,    -1,   116,   115,
     116,    -1,   117,    -1,   136,    -1,   118,    -1,    -1,    -1,
     120,    73,   121,   119,    -1,    -1,    73,   122,   119,    -1,
      61,    -1,    61,   120,    -1,   120,    -1,   123,   125,    -1,
     123,    -1,   125,    -1,    81,   124,    88,    -1,   125,   126,
      -1,   126,    -1,   125,   127,    -1,   127,    -1,    80,    91,
      -1,    15,    91,    -1,    80,     3,    91,    -1,    15,     3,
      91,    -1,    81,    88,    -1,    81,   131,    88,    -1,   137,
      -1,    13,    -1,    39,    -1,    37,    -1,    38,    -1,    37,
      39,    -1,    37,    36,    39,    -1,    37,    36,    -1,    36,
      37,    39,    -1,    23,    37,    39,    -1,    37,    23,    39,
      -1,    37,    23,    -1,    37,    37,    -1,    37,    37,    39,
      -1,    37,    37,    36,    39,    -1,    37,    37,    36,    -1,
      36,    37,    37,    -1,    36,    37,    37,    39,    -1,    23,
      37,    37,    -1,    23,    37,    37,    39,    -1,    37,    37,
      23,    -1,    37,    37,    23,    39,    -1,    38,    39,    -1,
      38,    36,    39,    -1,    38,    36,    -1,    23,    38,    39,
      -1,    38,    23,    -1,    38,    23,    39,    -1,    42,    -1,
      37,    42,    -1,    43,    -1,    18,   142,    -1,    18,    12,
      -1,    18,   142,    12,    -1,    19,   142,    -1,    19,    12,
      -1,    19,   142,    12,    -1,    20,   142,    -1,    20,    12,
      -1,    20,   142,    12,    -1,    21,   142,    -1,    21,    12,
      -1,    21,   142,    12,    -1,    23,   130,    -1,    23,    -1,
      36,   130,    -1,    36,    -1,    25,   142,    64,   128,    65,
      -1,   118,   129,    -1,   129,   118,    -1,    13,    -1,    39,
      -1,    37,    -1,    38,    -1,    43,    -1,   132,    -1,   132,
      53,    87,    -1,   128,    -1,   132,    53,   128,    -1,    44,
      45,    -1,    45,    44,    -1,    44,    46,    -1,    46,    44,
      -1,    44,    -1,   134,    -1,   133,    -1,   140,    -1,   135,
      -1,   140,   135,    -1,   135,   140,    -1,   129,    -1,   137,
     124,    -1,   129,   139,    -1,    -1,   120,   139,    -1,    40,
      41,    -1,    41,    40,    -1,    40,    -1,    41,    -1,    17,
      27,    -1,    17,    28,    -1,    17,    27,    80,    91,    -1,
      17,    28,    80,    91,    -1,    17,    27,    15,    91,    -1,
      17,    28,    15,    91,    -1,    17,    71,    -1,    17,    72,
      -1,    17,    73,    -1,    17,    74,    -1,    17,    75,    -1,
      17,    60,    -1,    17,    61,    -1,    17,    59,    -1,    17,
      90,    -1,    17,    89,    -1,    17,    55,    -1,    17,    64,
      -1,    17,    65,    -1,    17,    48,    -1,    17,    69,    -1,
      17,    68,    -1,    17,    63,    -1,    17,    62,    -1,    17,
      67,    -1,    17,    66,    -1,    17,    58,    -1,    17,    57,
      -1,    17,    77,    -1,    17,    76,    -1,    17,    53,    -1,
      17,    83,    -1,    17,    84,    -1,    17,    81,    88,    -1,
      17,    80,    91,    -1,    17,    15,    91,    -1,    17,   138,
      -1,    10,    -1,    85,    -1,    13,    -1,    16,    -1,    11,
      -1,   141,    -1,    10,    -1,    85,    -1,   141,    -1,    11,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   283,   283,   284,   287,   291,   295,   301,   308,   309,
     314,   318,   322,   326,   330,   334,   338,   342,   346,   350,
     354,   358,   364,   371,   381,   389,   393,   399,   406,   416,
     424,   428,   432,   442,   441,   464,   463,   479,   478,   487,
     489,   492,   493,   496,   498,   500,   507,   504,   515,   514,
     537,   541,   545,   549,   552,   556,   560,   578,   581,   588,
     592,   596,   602,   606,   610,   614,   618,   622,   626,   630,
     634,   638,   642,   646,   650,   654,   658,   662,   666,   670,
     674,   678,   682,   686,   692,   699,   708,   719,   726,   733,
     736,   742,   749,   759,   763,   767,   771,   778,   795,   813,
     846,   855,   862,   871,   879,   885,   895,   910,   932,   947,
     971,   980,   981,  1009,  1073,  1077,  1078,  1081,  1084,  1085,
    1089,  1090,  1095,  1094,  1098,  1097,  1100,  1102,  1106,  1115,
    1117,  1118,  1121,  1123,  1130,  1136,  1142,  1149,  1151,  1153,
    1155,  1159,  1161,  1173,  1177,  1179,  1183,  1187,  1191,  1195,
    1199,  1203,  1207,  1211,  1215,  1219,  1223,  1227,  1231,  1235,
    1239,  1243,  1247,  1251,  1255,  1259,  1263,  1267,  1271,  1275,
    1279,  1283,  1287,  1291,  1293,  1296,  1301,  1307,  1310,  1315,
    1321,  1324,  1329,  1335,  1338,  1343,  1349,  1353,  1357,  1361,
    1368,  1372,  1374,  1378,  1379,  1387,  1395,  1403,  1412,  1414,
    1423,  1429,  1435,  1439,  1444,  1448,  1454,  1456,  1457,  1460,
    1461,  1462,  1463,  1466,  1467,  1474,  1478,  1479,  1482,  1486,
    1490,  1492,  1496,  1498,  1500,  1502,  1504,  1506,  1508,  1510,
    1512,  1514,  1516,  1518,  1520,  1522,  1524,  1526,  1528,  1530,
    1532,  1534,  1574,  1576,  1578,  1580,  1582,  1584,  1586,  1588,
    1590,  1592,  1594,  1596,  1598,  1600,  1602,  1604,  1606,  1622,
    1623,  1624,  1625,  1626,  1627,  1630,  1631,  1639,  1651
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "FLOAT", "DECFLOAT", "STRING",
  "NSSTRING", "SELECTOR", "CHAR", "NAME", "UNKNOWN_CPP_NAME", "COMPLETE",
  "TYPENAME", "CLASSNAME", "OBJC_LBRAC", "NAME_OR_INT", "OPERATOR",
  "STRUCT", "CLASS", "UNION", "ENUM", "SIZEOF", "UNSIGNED", "COLONCOLON",
  "TEMPLATE", "ERROR", "NEW", "DELETE", "REINTERPRET_CAST", "DYNAMIC_CAST",
  "STATIC_CAST", "CONST_CAST", "ENTRY", "TYPEOF", "DECLTYPE",
  "SIGNED_KEYWORD", "LONG", "SHORT", "INT_KEYWORD", "CONST_KEYWORD",
  "VOLATILE_KEYWORD", "DOUBLE_KEYWORD", "VOID_KEYWORD", "SHARED_KEYWORD",
  "RELAXED_KEYWORD", "STRICT_KEYWORD", "VARIABLE", "ASSIGN_MODIFY",
  "UNOP_INTRINSIC", "BINOP_INTRINSIC", "TRUEKEYWORD", "FALSEKEYWORD",
  "','", "ABOVE_COMMA", "'='", "'?'", "OROR", "ANDAND", "'|'", "'^'",
  "'&'", "NOTEQUAL", "EQUAL", "'<'", "'>'", "GEQ", "LEQ", "RSH", "LSH",
  "'@'", "'+'", "'-'", "'*'", "'/'", "'%'", "DECREMENT", "INCREMENT",
  "UNARY", "'.'", "'['", "'('", "DOT_STAR", "ARROW_STAR", "ARROW",
  "BLOCKNAME", "FILENAME", "DOTDOTDOT", "')'", "'!'", "'~'", "']'", "':'",
  "'{'", "'}'", "$accept", "start", "type_exp", "exp1", "exp", "@1", "@2",
  "@3", "msglist", "msgarglist", "msgarg", "@4", "@5", "lcurly", "arglist",
  "rcurly", "string_exp", "block", "variable", "qualified_name",
  "space_identifier", "type_qualifier", "cv_with_space_id",
  "type_qualifier_or_space_identifier_noopt",
  "type_qualifier_or_space_identifier", "ptr_operator", "@6", "@7",
  "ptr_operator_ts", "abs_decl", "direct_abs_decl", "array_mod",
  "func_mod", "type", "typebase", "typename", "parameter_typelist",
  "nonempty_typelist", "shared_and_relaxed", "shared_and_strict",
  "shared_and_strict_or_relaxed", "type_qualifier_noopt", "ptype",
  "conversion_type_id", "conversion_declarator", "const_and_or_volatile",
  "operator", "name", "name_not_typename", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,    44,   308,    61,    63,   309,   310,   124,
      94,    38,   311,   312,    60,    62,   313,   314,   315,   316,
      64,    43,    45,    42,    47,    37,   317,   318,   319,    46,
      91,    40,   320,   321,   322,   323,   324,   325,    41,    33,
     126,    93,    58,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    95,    96,    96,    97,    97,    97,    97,    98,    98,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,   100,    99,   101,    99,   102,    99,   103,
     103,   104,   104,   105,   105,   105,   106,    99,   107,    99,
      99,    99,   108,   109,   109,   109,    99,   110,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,   111,   111,    99,
      99,    99,    99,   112,   112,   112,   113,   113,   114,   114,
     114,   113,   113,   113,   115,   116,   116,   117,   118,   118,
     119,   119,   121,   120,   122,   120,   120,   120,   123,   124,
     124,   124,   125,   125,   125,   125,   125,   126,   126,   126,
     126,   127,   127,   128,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   130,   130,   130,   130,   130,   131,   131,
     132,   132,   133,   133,   134,   134,   135,   135,   135,   136,
     136,   136,   136,   137,   137,   138,   139,   139,   140,   140,
     140,   140,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   142,
     142,   142,   142,   142,   142,   143,   143,   143,   143
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     4,     4,     4,     1,     3,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     4,     3,     3,     3,     3,     4,     3,     3,
       3,     4,     4,     0,     5,     0,     5,     0,     5,     1,
       1,     1,     2,     3,     2,     2,     0,     5,     0,     5,
       4,     6,     1,     0,     1,     3,     5,     1,     3,     4,
       4,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     5,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     7,     7,     7,     7,     1,     2,     1,
       1,     1,     1,     1,     1,     3,     2,     3,     3,     4,
       5,     1,     2,     1,     2,     1,     0,     3,     1,     1,
       1,     0,     0,     4,     0,     3,     1,     2,     1,     2,
       1,     1,     3,     2,     1,     2,     1,     2,     2,     3,
       3,     2,     3,     1,     1,     1,     1,     1,     2,     3,
       2,     3,     3,     3,     2,     2,     3,     4,     3,     3,
       4,     3,     4,     3,     4,     2,     3,     2,     3,     2,
       3,     1,     2,     1,     2,     2,     3,     2,     2,     3,
       2,     2,     3,     2,     2,     3,     2,     1,     2,     1,
       5,     2,     2,     1,     1,     1,     1,     1,     1,     3,
       1,     3,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     2,     2,     1,     2,     2,     0,     2,     2,     2,
       1,     1,     2,     2,     4,     4,     4,     4,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     116,    84,    87,    88,    97,   100,     0,    85,   265,   268,
     144,     0,    86,   116,     0,     0,     0,     0,     0,   187,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   146,
     147,   145,   220,   221,   171,   173,   206,     0,     0,    90,
       0,     0,   101,   102,     0,     0,     0,     0,     0,     0,
     116,   266,   104,     0,     0,    52,     0,     3,     2,     8,
      53,    99,     0,    89,   111,     0,   118,   116,     4,   213,
     208,   207,   210,   119,   143,   209,   267,   113,     0,    48,
       0,    33,    35,    37,   144,     0,   222,   223,   241,   252,
     238,   249,   248,   235,   233,   234,   245,   244,   239,   240,
     247,   246,   243,   242,   228,   229,   230,   231,   232,   251,
     250,     0,     0,   253,   254,   237,   236,   216,   258,   259,
     263,   175,   261,   262,   260,   264,   174,   178,   177,   181,
     180,   184,   183,     0,   116,    20,   193,   195,   196,   194,
     197,   186,   268,   266,   112,     0,   116,   116,   116,   116,
     116,     0,   195,   196,   188,   154,   150,   155,   148,   172,
     169,   167,   165,   218,   219,   202,   204,   203,   205,     0,
       0,    11,    13,    12,    10,    17,    16,     0,     0,    14,
      15,     1,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    19,    18,     0,     0,
      46,     0,     0,     0,     0,    54,     0,    98,     0,     0,
     116,   191,   192,   212,     0,   126,   124,     0,   116,   128,
     130,   214,   131,   134,   136,   211,   106,     0,    53,     0,
     108,     0,     0,     0,   257,     0,     0,     0,     0,   256,
     255,   216,   215,   176,   179,   182,   185,     0,   161,   152,
     168,   116,     0,     0,     0,     0,     0,     0,     0,   159,
     151,   153,   149,   163,   158,   156,   170,   166,     0,     0,
       0,    61,     9,     0,    83,    82,     0,    80,    79,    78,
      77,    76,    71,    70,    74,    75,    73,    72,    69,    68,
      62,    66,    67,    63,    64,    65,    28,   261,    29,    26,
       0,    53,   200,     0,   198,    30,    25,    23,    24,    21,
      57,     0,     0,    58,   107,   114,   117,   115,     0,   138,
     127,   121,     0,   137,   141,     0,     0,   122,   129,   116,
     133,   135,    91,     0,   109,     0,     0,     0,     0,    40,
      41,    39,     0,     0,   226,   224,   227,   225,   122,   217,
      92,   162,     0,     0,     0,     0,     0,     5,     6,     7,
     160,   164,   157,    50,     0,    60,    32,     0,    27,    31,
       0,   116,   116,    22,    59,    55,   140,   120,   125,   139,
     132,   142,   121,    49,   110,    45,    44,    34,    42,     0,
       0,    36,    38,   190,     0,     0,     0,     0,     0,    81,
      47,    56,   199,   201,   123,    43,     0,     0,     0,     0,
      51,    93,    95,    94,    96
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    56,   177,   178,    59,   241,   242,   243,   348,   349,
     350,   311,   238,    60,   216,   321,    61,    62,    63,    64,
     220,    65,    66,    67,   388,   229,   392,   331,   230,   231,
     232,   233,   234,    68,    69,   141,   336,   314,    70,    71,
      72,    73,    74,   118,   252,    75,    76,   351,    77
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -228
static const yytype_int16 yypact[] =
{
     485,  -228,  -228,  -228,  -228,  -228,   -18,  -228,  -228,   -14,
      82,   241,  -228,   837,    43,    75,    85,   119,   667,   596,
      30,   264,    63,    73,    91,    93,    33,    65,   617,   332,
     133,  -228,   120,   123,  -228,  -228,    72,   121,   122,  -228,
      86,   115,  -228,  -228,   758,   758,   758,   758,   758,   758,
     485,   144,  -228,   758,   758,  -228,   197,  -228,   146,  1379,
     394,   199,   179,  -228,  -228,   136,  -228,  1728,  -228,   498,
    -228,  -228,    29,   138,   228,    32,  -228,   176,   264,  -228,
      35,    82,  -228,  1379,  -228,   125,     9,    14,  -228,  -228,
    -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,   135,   142,  -228,  -228,  -228,  -228,   300,  -228,  -228,
    -228,  -228,  -228,  -228,  -228,  -228,   201,  -228,   219,  -228,
     220,  -228,   222,    82,   485,   471,  -228,    44,   200,  -228,
    -228,  -228,  -228,  -228,  -228,   174,  1694,  1694,  1694,  1694,
     576,   758,    87,  -228,  -228,   214,   221,   175,  -228,  -228,
     227,   229,  -228,  -228,  -228,  -228,  -228,  -228,  -228,   758,
     758,   471,   471,   471,   471,   471,   471,   152,    -9,   471,
     471,  -228,   758,   758,   758,   758,   758,   758,   758,   758,
     758,   758,   758,   758,   758,   758,   758,   758,   758,   758,
     758,   758,   758,   758,   758,   758,  -228,  -228,   132,   758,
    1657,   758,   758,   212,   148,  1379,     5,  -228,   264,   249,
     275,   498,  -228,  -228,    18,   188,  -228,    19,   949,   194,
      24,  -228,    42,  -228,  -228,  -228,  -228,   190,   758,   264,
     252,    15,    15,    15,  -228,   204,   205,   207,   208,  -228,
    -228,     0,  -228,  -228,  -228,  -228,  -228,   191,   255,  -228,
    -228,  1728,   238,   239,   240,   242,  1046,   218,  1083,   284,
    -228,  -228,  -228,   285,   289,  -228,  -228,  -228,  1120,  1342,
     758,  -228,  1379,   -26,  1379,  1379,   874,  1413,  1442,  1476,
    1505,  1539,  1568,  1568,  1585,  1585,  1585,  1585,   377,   377,
     629,   718,   718,   471,   471,   471,  -228,    82,  -228,   317,
      22,   758,  -228,   244,   280,   203,   203,  -228,  -228,   325,
    -228,   758,   758,  -228,   314,  -228,  -228,  -228,   251,  -228,
     194,   498,   257,  -228,  -228,   262,   265,  -228,    42,   339,
    -228,  -228,  -228,    31,  -228,   264,   758,   758,   263,    15,
    -228,   273,   276,   281,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,   291,   305,   306,   307,   309,  -228,  -228,  -228,
    -228,  -228,  -228,  -228,   758,   471,  -228,   758,  -228,  -228,
      50,   275,  1013,  -228,   471,  1379,  -228,  -228,  -228,  -228,
    -228,  -228,   498,  -228,  -228,  1379,  1379,  -228,  -228,   273,
     758,  -228,  -228,  -228,   758,   758,   758,   758,  1157,  1006,
    -228,  -228,  -228,  -228,  -228,  1379,  1194,  1231,  1268,  1305,
    -228,  -228,  -228,  -228,  -228
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -228,  -228,     4,    12,   -11,  -228,  -228,  -228,  -109,  -228,
      57,  -228,  -228,  -228,  -218,   192,  -228,  -228,  -228,  -142,
    -228,  -219,  -228,   -67,    -1,  -104,  -228,  -228,  -228,   193,
     232,  -227,  -226,  -120,    49,   392,   253,  -228,  -228,  -228,
     347,  -217,  -228,  -228,   217,   370,    -6,     2,   449
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -117
static const yytype_int16 yytable[] =
{
      83,   326,   222,   327,    57,   340,   341,   135,   125,   125,
     125,   125,    58,   251,   257,   125,   126,   128,   130,   132,
     343,   328,   332,   145,   245,   119,   120,   182,   122,   247,
     267,   123,    13,   171,   172,   173,   174,   175,   176,   224,
       8,   142,   179,   180,   182,   119,   120,    13,   122,   215,
     222,   123,    13,   119,   120,   121,   122,   224,   322,   123,
      13,   225,   117,    78,   214,   376,   308,    79,   346,    32,
      33,   318,   125,   358,   125,   182,    36,    37,    38,   281,
     237,   258,   240,   259,   322,   119,   120,   127,   122,   246,
     312,   123,    13,   380,   248,   119,   120,   129,   122,   320,
     124,   123,    13,   322,   227,   228,    80,   347,   312,   329,
     333,   340,   341,   379,   150,   143,   221,   165,   166,   393,
     124,   330,   227,   339,   269,   239,   270,   146,   124,   119,
     120,   131,   122,   352,   353,   123,    13,   147,   410,   266,
     268,   362,   119,   120,   306,   307,   151,   251,   123,    13,
     262,   263,   264,   265,   222,   148,   160,   149,   278,   279,
     124,   163,   411,   164,   327,   167,   168,   169,  -103,   161,
     124,   282,   162,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   283,   170,   181,   273,   182,
     315,   316,   125,   218,   124,   217,   219,   125,  -115,   236,
     309,   274,   125,   253,   275,   319,   244,   124,   183,   312,
     324,   310,   119,   120,   317,   307,   249,   215,   123,    13,
     250,   254,   255,   125,   256,   125,   125,   125,   261,   260,
     280,   344,   320,   224,     1,     2,     3,     4,     5,     6,
       7,     8,     9,   271,    81,    82,    11,    12,    13,   325,
     272,   226,   413,    18,   387,    20,   276,   337,   277,   375,
      22,    23,    24,    25,   119,   120,   345,   122,   342,   360,
     123,    13,   208,   209,   210,   211,   212,   213,    39,   225,
      40,    41,    42,    43,   361,   354,   355,   124,   356,   357,
     215,   226,    44,   363,   364,   365,   368,   366,   227,   228,
     384,   385,    45,    46,    47,    32,    33,    48,    49,    36,
      37,    38,    50,   370,   371,   387,    51,    52,   372,   378,
      53,    54,   381,   382,    55,   395,   396,   383,  -105,   125,
      32,    33,   386,   125,    36,    37,    38,   394,   389,   124,
     390,   399,    84,   391,   397,   155,   403,    14,    15,    16,
      17,   225,    19,   408,    21,   400,   409,   401,   156,   157,
    -116,   158,   402,   226,   159,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,   404,   405,   406,   415,
     407,   414,   183,   416,   417,   418,   419,     1,     2,     3,
       4,     5,     6,     7,     8,     9,   398,    10,   323,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     154,   335,   235,    22,    23,    24,    25,   334,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   223,    40,    41,    42,    43,   200,   201,   202,
     203,   204,   205,   206,   207,    44,   208,   209,   210,   211,
     212,   213,   338,   313,  -116,    45,    46,    47,   359,   144,
      48,    49,     0,     0,     0,    50,     0,     0,     0,    51,
      52,     0,     0,    53,    54,     0,   183,    55,     1,     2,
       3,     4,     5,     6,     7,     8,     9,     0,    10,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,     0,     0,     0,    22,    23,    24,    25,     0,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,     0,    40,    41,    42,    43,    32,    33,
       0,     0,    36,    37,    38,     0,    44,   206,   207,     0,
     208,   209,   210,   211,   212,   213,    45,    46,    47,     0,
       0,    48,    49,     0,     0,     0,    50,     0,  -116,     0,
      51,    52,     0,     0,    53,    54,     0,     0,    55,     1,
       2,     3,     4,     5,     6,     7,     8,     9,     0,    10,
       0,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     0,     0,     0,    22,    23,    24,    25,   136,
       0,     0,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,     0,    40,    41,    42,    43,     0,
     136,     0,     0,   137,   138,   139,     0,    44,     0,   140,
       0,     0,     0,     0,   183,     0,     0,    45,    46,    47,
       0,     0,    48,    49,   152,   153,   139,    50,     0,     0,
     140,    51,    52,     0,     0,    53,    54,     0,     0,    55,
       1,     2,     3,     4,     5,     6,     7,     8,     9,     0,
     133,     0,    11,    12,    13,     0,     0,     0,     0,    18,
       0,    20,     0,     0,     0,     0,    22,    23,    24,    25,
     201,   202,   203,   204,   205,   206,   207,     0,   208,   209,
     210,   211,   212,   213,    39,     0,    40,    41,    42,    43,
       0,     0,     0,     0,     0,     0,     0,     0,    44,     0,
       0,     0,     0,   183,     0,     0,     0,     0,    45,    46,
      47,     0,     0,    48,    49,     0,     0,     0,   134,     0,
       0,     0,    51,    52,     0,     0,    53,    54,     0,     0,
      55,     1,     2,     3,     4,     5,     6,     7,     8,     9,
       0,   133,     0,    11,    12,    13,     0,     0,     0,     0,
      18,     0,    20,     0,     0,     0,     0,    22,    23,    24,
      25,   203,   204,   205,   206,   207,     0,   208,   209,   210,
     211,   212,   213,     0,     0,    39,     0,    40,    41,    42,
      43,     0,     0,     0,     0,     0,     0,     0,     0,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    45,
      46,    47,     0,     0,    48,    49,     0,     0,     0,    50,
       0,     0,     0,    51,    52,     0,     0,    53,    54,     0,
      84,    55,    85,     0,     0,    14,    15,    16,    17,     0,
      19,     0,    21,     0,    86,    87,     0,     0,     0,     0,
       0,     0,     0,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,     0,    88,     0,     0,     0,   183,
      89,     0,    90,     0,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,     0,   104,   105,
     106,   107,   108,   109,   110,     0,     0,   111,   112,     0,
     113,   114,   184,     0,     0,     0,   115,   116,     0,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,     0,   208,   209,   210,   211,   212,   213,     0,
       0,     0,    84,     0,   224,     0,   377,    14,    15,    16,
      17,     0,    19,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   183,   226,     0,     0,     0,    84,     0,     0,   227,
     228,    14,    15,    16,    17,     0,    19,   334,    21,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
       0,   183,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,     0,   208,   209,   210,   211,   212,
     213,     0,     0,     0,   184,     0,     0,     0,   183,     0,
     412,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,     0,   208,   209,   210,   211,   212,
     213,   184,     0,     0,   367,   183,     0,     0,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,     0,   208,   209,   210,   211,   212,   213,   184,     0,
       0,   369,   183,     0,     0,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,     0,   208,
     209,   210,   211,   212,   213,   184,     0,     0,   373,   183,
       0,     0,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,     0,   208,   209,   210,   211,
     212,   213,   184,     0,     0,   420,   183,     0,     0,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,     0,   208,   209,   210,   211,   212,   213,   184,
       0,     0,   421,   183,     0,     0,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,     0,
     208,   209,   210,   211,   212,   213,   184,     0,     0,   422,
     183,     0,     0,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,     0,   208,   209,   210,
     211,   212,   213,   184,     0,     0,   423,   183,     0,     0,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,     0,   208,   209,   210,   211,   212,   213,
     184,     0,     0,   424,   183,   374,     0,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
       0,   208,   209,   210,   211,   212,   213,   184,   183,     0,
       0,     0,     0,     0,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   183,   208,   209,
     210,   211,   212,   213,     0,     0,     0,     0,     0,     0,
       0,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   183,   208,   209,   210,   211,   212,   213,     0,     0,
       0,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     183,   208,   209,   210,   211,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   183,   208,   209,   210,   211,   212,
     213,     0,     0,     0,     0,     0,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   183,   208,   209,   210,   211,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     183,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,     0,   208,   209,
     210,   211,   212,   213,     0,     0,     0,     0,     0,     0,
       0,     0,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,     0,   208,   209,   210,
     211,   212,   213,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,     0,   208,   209,   210,   211,   212,   213,
      84,     0,     0,     0,     0,    14,    15,    16,    17,     0,
      19,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,     0,     0,     0,    84,     0,     0,
       0,     0,    14,    15,    16,    17,     0,    19,     0,    21,
       0,     0,     0,     0,     0,     0,     0,  -116,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    84,     0,     0,     0,     0,    14,    15,    16,    17,
       0,    19,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38
};

static const yytype_int16 yycheck[] =
{
      11,   220,    69,   220,     0,   232,   232,    18,    14,    15,
      16,    17,     0,   117,   134,    21,    14,    15,    16,    17,
     238,     3,     3,    21,    15,    10,    11,    53,    13,    15,
     150,    16,    17,    44,    45,    46,    47,    48,    49,    15,
      10,    11,    53,    54,    53,    10,    11,    17,    13,    60,
     117,    16,    17,    10,    11,    12,    13,    15,    53,    16,
      17,    61,    13,    81,    60,    91,   208,    81,    53,    40,
      41,   213,    78,    73,    80,    53,    44,    45,    46,    88,
      78,    37,    80,    39,    53,    10,    11,    12,    13,    80,
     210,    16,    17,   311,    80,    10,    11,    12,    13,    94,
      85,    16,    17,    53,    80,    81,    24,    92,   228,    91,
      91,   338,   338,    91,    81,    85,    67,    45,    46,    88,
      85,   225,    80,    81,    37,    90,    39,    64,    85,    10,
      11,    12,    13,   242,   243,    16,    17,    64,    88,   150,
     151,   261,    10,    11,    12,    13,    81,   251,    16,    17,
     146,   147,   148,   149,   221,    64,    23,    64,   169,   170,
      85,    41,   381,    40,   381,    44,    44,    81,    24,    36,
      85,   182,    39,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   183,    81,     0,    23,    53,
     211,   212,   208,    24,    85,     6,    70,   213,    70,    33,
     208,    36,   218,    12,    39,   213,    91,    85,    15,   339,
     218,   209,    10,    11,    12,    13,    91,   238,    16,    17,
      88,    12,    12,   239,    12,   241,   242,   243,    64,    39,
      88,   239,    94,    15,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    39,    13,    14,    15,    16,    17,    10,
      39,    73,   382,    22,   331,    24,    39,    73,    39,   280,
      29,    30,    31,    32,    10,    11,    24,    13,    88,    88,
      16,    17,    79,    80,    81,    82,    83,    84,    47,    61,
      49,    50,    51,    52,    39,    91,    91,    85,    91,    91,
     311,    73,    61,    65,    65,    65,    88,    65,    80,    81,
     321,   322,    71,    72,    73,    40,    41,    76,    77,    44,
      45,    46,    81,    39,    39,   392,    85,    86,    39,    12,
      89,    90,    88,    53,    93,   346,   347,    12,    24,   345,
      40,    41,    91,   349,    44,    45,    46,   345,    91,    85,
      88,   349,    13,    88,    91,    23,    65,    18,    19,    20,
      21,    61,    23,   374,    25,    92,   377,    91,    36,    37,
      70,    39,    91,    73,    42,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    81,    81,    81,   400,
      81,   392,    15,   404,   405,   406,   407,     3,     4,     5,
       6,     7,     8,     9,    10,    11,   349,    13,   216,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      28,   228,    75,    29,    30,    31,    32,    88,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    72,    49,    50,    51,    52,    70,    71,    72,
      73,    74,    75,    76,    77,    61,    79,    80,    81,    82,
      83,    84,   230,   210,    70,    71,    72,    73,   251,    20,
      76,    77,    -1,    -1,    -1,    81,    -1,    -1,    -1,    85,
      86,    -1,    -1,    89,    90,    -1,    15,    93,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    13,    -1,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    29,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    52,    40,    41,
      -1,    -1,    44,    45,    46,    -1,    61,    76,    77,    -1,
      79,    80,    81,    82,    83,    84,    71,    72,    73,    -1,
      -1,    76,    77,    -1,    -1,    -1,    81,    -1,    70,    -1,
      85,    86,    -1,    -1,    89,    90,    -1,    -1,    93,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    -1,    29,    30,    31,    32,    13,
      -1,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    52,    -1,
      13,    -1,    -1,    37,    38,    39,    -1,    61,    -1,    43,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    71,    72,    73,
      -1,    -1,    76,    77,    37,    38,    39,    81,    -1,    -1,
      43,    85,    86,    -1,    -1,    89,    90,    -1,    -1,    93,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      13,    -1,    15,    16,    17,    -1,    -1,    -1,    -1,    22,
      -1,    24,    -1,    -1,    -1,    -1,    29,    30,    31,    32,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    82,    83,    84,    47,    -1,    49,    50,    51,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    71,    72,
      73,    -1,    -1,    76,    77,    -1,    -1,    -1,    81,    -1,
      -1,    -1,    85,    86,    -1,    -1,    89,    90,    -1,    -1,
      93,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    13,    -1,    15,    16,    17,    -1,    -1,    -1,    -1,
      22,    -1,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
      32,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    47,    -1,    49,    50,    51,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    -1,    -1,    76,    77,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    85,    86,    -1,    -1,    89,    90,    -1,
      13,    93,    15,    -1,    -1,    18,    19,    20,    21,    -1,
      23,    -1,    25,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    15,
      53,    -1,    55,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    -1,    -1,    80,    81,    -1,
      83,    84,    48,    -1,    -1,    -1,    89,    90,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    13,    -1,    15,    -1,    92,    18,    19,    20,
      21,    -1,    23,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    73,    -1,    -1,    -1,    13,    -1,    -1,    80,
      81,    18,    19,    20,    21,    -1,    23,    88,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    15,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    48,    -1,    -1,    -1,    15,    -1,
      87,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    79,    80,    81,    82,    83,
      84,    48,    -1,    -1,    88,    15,    -1,    -1,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    80,    81,    82,    83,    84,    48,    -1,
      -1,    88,    15,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    82,    83,    84,    48,    -1,    -1,    88,    15,
      -1,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    80,    81,    82,
      83,    84,    48,    -1,    -1,    88,    15,    -1,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    82,    83,    84,    48,
      -1,    -1,    88,    15,    -1,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    80,    81,    82,    83,    84,    48,    -1,    -1,    88,
      15,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    48,    -1,    -1,    88,    15,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    80,    81,    82,    83,    84,
      48,    -1,    -1,    88,    15,    53,    -1,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    82,    83,    84,    48,    15,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    15,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    15,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      15,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    15,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    15,    79,    80,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    80,    81,    82,    83,    84,
      13,    -1,    -1,    -1,    -1,    18,    19,    20,    21,    -1,
      23,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    -1,    18,    19,    20,    21,    -1,    23,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    13,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      -1,    23,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      13,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    29,    30,    31,    32,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      49,    50,    51,    52,    61,    71,    72,    73,    76,    77,
      81,    85,    86,    89,    90,    93,    96,    97,    98,    99,
     108,   111,   112,   113,   114,   116,   117,   118,   128,   129,
     133,   134,   135,   136,   137,   140,   141,   143,    81,    81,
      24,    13,    14,    99,    13,    15,    27,    28,    48,    53,
      55,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    71,    72,    73,    74,    75,    76,
      77,    80,    81,    83,    84,    89,    90,   129,   138,    10,
      11,    12,    13,    16,    85,   141,   142,    12,   142,    12,
     142,    12,   142,    13,    81,    99,    13,    37,    38,    39,
      43,   130,    11,    85,   143,   142,    64,    64,    64,    64,
      81,    81,    37,    38,   130,    23,    36,    37,    39,    42,
      23,    36,    39,    41,    40,    45,    46,    44,    44,    81,
      81,    99,    99,    99,    99,    99,    99,    97,    98,    99,
      99,     0,    53,    15,    48,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    79,    80,
      81,    82,    83,    84,    97,    99,   109,     6,    24,    70,
     115,   129,   118,   140,    15,    61,    73,    80,    81,   120,
     123,   124,   125,   126,   127,   135,    33,   142,   107,    90,
     142,   100,   101,   102,    91,    15,    80,    15,    80,    91,
      88,   120,   139,    12,    12,    12,    12,   128,    37,    39,
      39,    64,    97,    97,    97,    97,    99,   128,    99,    37,
      39,    39,    39,    23,    36,    39,    39,    39,    99,    99,
      88,    88,    99,    98,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    12,    13,   114,   142,
      98,   106,   128,   131,   132,    99,    99,    12,   114,   142,
      94,   110,    53,   110,   142,    10,   116,   136,     3,    91,
     120,   122,     3,    91,    88,   124,   131,    73,   125,    81,
     126,   127,    88,   109,   142,    24,    53,    92,   103,   104,
     105,   142,   103,   103,    91,    91,    91,    91,    73,   139,
      88,    39,   128,    65,    65,    65,    65,    88,    88,    88,
      39,    39,    39,    88,    53,    99,    91,    92,    12,    91,
     109,    88,    53,    12,    99,    99,    91,   118,   119,    91,
      88,    88,   121,    88,   142,    99,    99,    91,   105,   142,
      92,    91,    91,    65,    81,    81,    81,    81,    99,    99,
      88,   116,    87,   128,   119,    99,    99,    99,    99,    99,
      88,    88,    88,    88,    88
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
#line 288 "c-exp.y"
    { write_exp_elt_opcode(OP_TYPE);
			  write_exp_elt_type((yyvsp[(1) - (1)].tval));
			  write_exp_elt_opcode(OP_TYPE);}
    break;

  case 5:
#line 292 "c-exp.y"
    {
			  write_exp_elt_opcode (OP_TYPEOF);
			}
    break;

  case 6:
#line 296 "c-exp.y"
    {
			  write_exp_elt_opcode (OP_TYPE);
			  write_exp_elt_type ((yyvsp[(3) - (4)].tval));
			  write_exp_elt_opcode (OP_TYPE);
			}
    break;

  case 7:
#line 302 "c-exp.y"
    {
			  write_exp_elt_opcode (OP_DECLTYPE);
			}
    break;

  case 9:
#line 310 "c-exp.y"
    { write_exp_elt_opcode (BINOP_COMMA); }
    break;

  case 10:
#line 315 "c-exp.y"
    { write_exp_elt_opcode (UNOP_IND); }
    break;

  case 11:
#line 319 "c-exp.y"
    { write_exp_elt_opcode (UNOP_ADDR); }
    break;

  case 12:
#line 323 "c-exp.y"
    { write_exp_elt_opcode (UNOP_NEG); }
    break;

  case 13:
#line 327 "c-exp.y"
    { write_exp_elt_opcode (UNOP_PLUS); }
    break;

  case 14:
#line 331 "c-exp.y"
    { write_exp_elt_opcode (UNOP_LOGICAL_NOT); }
    break;

  case 15:
#line 335 "c-exp.y"
    { write_exp_elt_opcode (UNOP_COMPLEMENT); }
    break;

  case 16:
#line 339 "c-exp.y"
    { write_exp_elt_opcode (UNOP_PREINCREMENT); }
    break;

  case 17:
#line 343 "c-exp.y"
    { write_exp_elt_opcode (UNOP_PREDECREMENT); }
    break;

  case 18:
#line 347 "c-exp.y"
    { write_exp_elt_opcode (UNOP_POSTINCREMENT); }
    break;

  case 19:
#line 351 "c-exp.y"
    { write_exp_elt_opcode (UNOP_POSTDECREMENT); }
    break;

  case 20:
#line 355 "c-exp.y"
    { write_exp_elt_opcode (UNOP_SIZEOF); }
    break;

  case 21:
#line 359 "c-exp.y"
    { write_exp_elt_opcode (STRUCTOP_PTR);
			  write_exp_string ((yyvsp[(3) - (3)].sval));
			  write_exp_elt_opcode (STRUCTOP_PTR); }
    break;

  case 22:
#line 365 "c-exp.y"
    { mark_struct_expression ();
			  write_exp_elt_opcode (STRUCTOP_PTR);
			  write_exp_string ((yyvsp[(3) - (4)].sval));
			  write_exp_elt_opcode (STRUCTOP_PTR); }
    break;

  case 23:
#line 372 "c-exp.y"
    { struct stoken s;
			  mark_struct_expression ();
			  write_exp_elt_opcode (STRUCTOP_PTR);
			  s.ptr = "";
			  s.length = 0;
			  write_exp_string (s);
			  write_exp_elt_opcode (STRUCTOP_PTR); }
    break;

  case 24:
#line 382 "c-exp.y"
    { /* exp->type::name becomes exp->*(&type::name) */
			  /* Note: this doesn't work if name is a
			     static member!  FIXME */
			  write_exp_elt_opcode (UNOP_ADDR);
			  write_exp_elt_opcode (STRUCTOP_MPTR); }
    break;

  case 25:
#line 390 "c-exp.y"
    { write_exp_elt_opcode (STRUCTOP_MPTR); }
    break;

  case 26:
#line 394 "c-exp.y"
    { write_exp_elt_opcode (STRUCTOP_STRUCT);
			  write_exp_string ((yyvsp[(3) - (3)].sval));
			  write_exp_elt_opcode (STRUCTOP_STRUCT); }
    break;

  case 27:
#line 400 "c-exp.y"
    { mark_struct_expression ();
			  write_exp_elt_opcode (STRUCTOP_STRUCT);
			  write_exp_string ((yyvsp[(3) - (4)].sval));
			  write_exp_elt_opcode (STRUCTOP_STRUCT); }
    break;

  case 28:
#line 407 "c-exp.y"
    { struct stoken s;
			  mark_struct_expression ();
			  write_exp_elt_opcode (STRUCTOP_STRUCT);
			  s.ptr = "";
			  s.length = 0;
			  write_exp_string (s);
			  write_exp_elt_opcode (STRUCTOP_STRUCT); }
    break;

  case 29:
#line 417 "c-exp.y"
    { /* exp.type::name becomes exp.*(&type::name) */
			  /* Note: this doesn't work if name is a
			     static member!  FIXME */
			  write_exp_elt_opcode (UNOP_ADDR);
			  write_exp_elt_opcode (STRUCTOP_MEMBER); }
    break;

  case 30:
#line 425 "c-exp.y"
    { write_exp_elt_opcode (STRUCTOP_MEMBER); }
    break;

  case 31:
#line 429 "c-exp.y"
    { write_exp_elt_opcode (BINOP_SUBSCRIPT); }
    break;

  case 32:
#line 433 "c-exp.y"
    { write_exp_elt_opcode (BINOP_SUBSCRIPT); }
    break;

  case 33:
#line 442 "c-exp.y"
    {
			  CORE_ADDR class;

			  class = lookup_objc_class (parse_gdbarch,
						     copy_name ((yyvsp[(2) - (2)].tsym).stoken));
			  if (class == 0)
			    error (_("%s is not an ObjC Class"),
				   copy_name ((yyvsp[(2) - (2)].tsym).stoken));
			  write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (parse_type->builtin_int);
			  write_exp_elt_longcst ((LONGEST) class);
			  write_exp_elt_opcode (OP_LONG);
			  start_msglist();
			}
    break;

  case 34:
#line 457 "c-exp.y"
    { write_exp_elt_opcode (OP_OBJC_MSGCALL);
			  end_msglist();
			  write_exp_elt_opcode (OP_OBJC_MSGCALL);
			}
    break;

  case 35:
#line 464 "c-exp.y"
    {
			  write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (parse_type->builtin_int);
			  write_exp_elt_longcst ((LONGEST) (yyvsp[(2) - (2)].class).class);
			  write_exp_elt_opcode (OP_LONG);
			  start_msglist();
			}
    break;

  case 36:
#line 472 "c-exp.y"
    { write_exp_elt_opcode (OP_OBJC_MSGCALL);
			  end_msglist();
			  write_exp_elt_opcode (OP_OBJC_MSGCALL);
			}
    break;

  case 37:
#line 479 "c-exp.y"
    { start_msglist(); }
    break;

  case 38:
#line 481 "c-exp.y"
    { write_exp_elt_opcode (OP_OBJC_MSGCALL);
			  end_msglist();
			  write_exp_elt_opcode (OP_OBJC_MSGCALL);
			}
    break;

  case 39:
#line 488 "c-exp.y"
    { add_msglist(&(yyvsp[(1) - (1)].sval), 0); }
    break;

  case 43:
#line 497 "c-exp.y"
    { add_msglist(&(yyvsp[(1) - (3)].sval), 1); }
    break;

  case 44:
#line 499 "c-exp.y"
    { add_msglist(0, 1);   }
    break;

  case 45:
#line 501 "c-exp.y"
    { add_msglist(0, 0);   }
    break;

  case 46:
#line 507 "c-exp.y"
    { start_arglist (); }
    break;

  case 47:
#line 509 "c-exp.y"
    { write_exp_elt_opcode (OP_FUNCALL);
			  write_exp_elt_longcst ((LONGEST) end_arglist ());
			  write_exp_elt_opcode (OP_FUNCALL); }
    break;

  case 48:
#line 515 "c-exp.y"
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

  case 49:
#line 530 "c-exp.y"
    {
			  write_exp_elt_opcode (OP_FUNCALL);
			  write_exp_elt_longcst ((LONGEST) end_arglist ());
			  write_exp_elt_opcode (OP_FUNCALL);
			}
    break;

  case 50:
#line 538 "c-exp.y"
    { write_exp_elt_opcode ((yyvsp[(1) - (4)].opcode)); }
    break;

  case 51:
#line 542 "c-exp.y"
    { write_exp_elt_opcode ((yyvsp[(1) - (6)].opcode)); }
    break;

  case 52:
#line 546 "c-exp.y"
    { start_arglist (); }
    break;

  case 54:
#line 553 "c-exp.y"
    { arglist_len = 1; }
    break;

  case 55:
#line 557 "c-exp.y"
    { arglist_len++; }
    break;

  case 56:
#line 561 "c-exp.y"
    { int i;
			  VEC (type_ptr) *type_list = (yyvsp[(3) - (5)].tvec);
			  struct type *type_elt;
			  LONGEST len = VEC_length (type_ptr, type_list);

			  write_exp_elt_opcode (TYPE_INSTANCE);
			  write_exp_elt_longcst (len);
			  for (i = 0;
			       VEC_iterate (type_ptr, type_list, i, type_elt);
			       ++i)
			    write_exp_elt_type (type_elt);
			  write_exp_elt_longcst(len);
			  write_exp_elt_opcode (TYPE_INSTANCE);
			  VEC_free (type_ptr, type_list);
			}
    break;

  case 57:
#line 579 "c-exp.y"
    { (yyval.lval) = end_arglist () - 1; }
    break;

  case 58:
#line 582 "c-exp.y"
    { write_exp_elt_opcode (OP_ARRAY);
			  write_exp_elt_longcst ((LONGEST) 0);
			  write_exp_elt_longcst ((LONGEST) (yyvsp[(3) - (3)].lval));
			  write_exp_elt_opcode (OP_ARRAY); }
    break;

  case 59:
#line 589 "c-exp.y"
    { write_exp_elt_opcode (UNOP_MEMVAL_TYPE); }
    break;

  case 60:
#line 593 "c-exp.y"
    { write_exp_elt_opcode (UNOP_CAST_TYPE); }
    break;

  case 61:
#line 597 "c-exp.y"
    { }
    break;

  case 62:
#line 603 "c-exp.y"
    { write_exp_elt_opcode (BINOP_REPEAT); }
    break;

  case 63:
#line 607 "c-exp.y"
    { write_exp_elt_opcode (BINOP_MUL); }
    break;

  case 64:
#line 611 "c-exp.y"
    { write_exp_elt_opcode (BINOP_DIV); }
    break;

  case 65:
#line 615 "c-exp.y"
    { write_exp_elt_opcode (BINOP_REM); }
    break;

  case 66:
#line 619 "c-exp.y"
    { write_exp_elt_opcode (BINOP_ADD); }
    break;

  case 67:
#line 623 "c-exp.y"
    { write_exp_elt_opcode (BINOP_SUB); }
    break;

  case 68:
#line 627 "c-exp.y"
    { write_exp_elt_opcode (BINOP_LSH); }
    break;

  case 69:
#line 631 "c-exp.y"
    { write_exp_elt_opcode (BINOP_RSH); }
    break;

  case 70:
#line 635 "c-exp.y"
    { write_exp_elt_opcode (BINOP_EQUAL); }
    break;

  case 71:
#line 639 "c-exp.y"
    { write_exp_elt_opcode (BINOP_NOTEQUAL); }
    break;

  case 72:
#line 643 "c-exp.y"
    { write_exp_elt_opcode (BINOP_LEQ); }
    break;

  case 73:
#line 647 "c-exp.y"
    { write_exp_elt_opcode (BINOP_GEQ); }
    break;

  case 74:
#line 651 "c-exp.y"
    { write_exp_elt_opcode (BINOP_LESS); }
    break;

  case 75:
#line 655 "c-exp.y"
    { write_exp_elt_opcode (BINOP_GTR); }
    break;

  case 76:
#line 659 "c-exp.y"
    { write_exp_elt_opcode (BINOP_BITWISE_AND); }
    break;

  case 77:
#line 663 "c-exp.y"
    { write_exp_elt_opcode (BINOP_BITWISE_XOR); }
    break;

  case 78:
#line 667 "c-exp.y"
    { write_exp_elt_opcode (BINOP_BITWISE_IOR); }
    break;

  case 79:
#line 671 "c-exp.y"
    { write_exp_elt_opcode (BINOP_LOGICAL_AND); }
    break;

  case 80:
#line 675 "c-exp.y"
    { write_exp_elt_opcode (BINOP_LOGICAL_OR); }
    break;

  case 81:
#line 679 "c-exp.y"
    { write_exp_elt_opcode (TERNOP_COND); }
    break;

  case 82:
#line 683 "c-exp.y"
    { write_exp_elt_opcode (BINOP_ASSIGN); }
    break;

  case 83:
#line 687 "c-exp.y"
    { write_exp_elt_opcode (BINOP_ASSIGN_MODIFY);
			  write_exp_elt_opcode ((yyvsp[(2) - (3)].opcode));
			  write_exp_elt_opcode (BINOP_ASSIGN_MODIFY); }
    break;

  case 84:
#line 693 "c-exp.y"
    { write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type ((yyvsp[(1) - (1)].typed_val_int).type);
			  write_exp_elt_longcst ((LONGEST)((yyvsp[(1) - (1)].typed_val_int).val));
			  write_exp_elt_opcode (OP_LONG); }
    break;

  case 85:
#line 700 "c-exp.y"
    {
			  struct stoken_vector vec;
			  vec.len = 1;
			  vec.tokens = &(yyvsp[(1) - (1)].tsval);
			  write_exp_string_vector ((yyvsp[(1) - (1)].tsval).type, &vec);
			}
    break;

  case 86:
#line 709 "c-exp.y"
    { YYSTYPE val;
			  parse_number ((yyvsp[(1) - (1)].ssym).stoken.ptr, (yyvsp[(1) - (1)].ssym).stoken.length, 0, &val);
			  write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (val.typed_val_int.type);
			  write_exp_elt_longcst ((LONGEST)val.typed_val_int.val);
			  write_exp_elt_opcode (OP_LONG);
			}
    break;

  case 87:
#line 720 "c-exp.y"
    { write_exp_elt_opcode (OP_DOUBLE);
			  write_exp_elt_type ((yyvsp[(1) - (1)].typed_val_float).type);
			  write_exp_elt_dblcst ((yyvsp[(1) - (1)].typed_val_float).dval);
			  write_exp_elt_opcode (OP_DOUBLE); }
    break;

  case 88:
#line 727 "c-exp.y"
    { write_exp_elt_opcode (OP_DECFLOAT);
			  write_exp_elt_type ((yyvsp[(1) - (1)].typed_val_decfloat).type);
			  write_exp_elt_decfloatcst ((yyvsp[(1) - (1)].typed_val_decfloat).val);
			  write_exp_elt_opcode (OP_DECFLOAT); }
    break;

  case 90:
#line 737 "c-exp.y"
    {
			  write_dollar_variable ((yyvsp[(1) - (1)].sval));
			}
    break;

  case 91:
#line 743 "c-exp.y"
    {
			  write_exp_elt_opcode (OP_OBJC_SELECTOR);
			  write_exp_string ((yyvsp[(3) - (4)].sval));
			  write_exp_elt_opcode (OP_OBJC_SELECTOR); }
    break;

  case 92:
#line 750 "c-exp.y"
    { write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (lookup_signed_typename
					      (parse_language, parse_gdbarch,
					       "int"));
			  CHECK_TYPEDEF ((yyvsp[(3) - (4)].tval));
			  write_exp_elt_longcst ((LONGEST) TYPE_LENGTH ((yyvsp[(3) - (4)].tval)));
			  write_exp_elt_opcode (OP_LONG); }
    break;

  case 93:
#line 760 "c-exp.y"
    { write_exp_elt_opcode (UNOP_REINTERPRET_CAST); }
    break;

  case 94:
#line 764 "c-exp.y"
    { write_exp_elt_opcode (UNOP_CAST_TYPE); }
    break;

  case 95:
#line 768 "c-exp.y"
    { write_exp_elt_opcode (UNOP_DYNAMIC_CAST); }
    break;

  case 96:
#line 772 "c-exp.y"
    { /* We could do more error checking here, but
			     it doesn't seem worthwhile.  */
			  write_exp_elt_opcode (UNOP_CAST_TYPE); }
    break;

  case 97:
#line 779 "c-exp.y"
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

  case 98:
#line 796 "c-exp.y"
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

  case 99:
#line 814 "c-exp.y"
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
				    error (_("Undefined string concatenation."));
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

  case 100:
#line 849 "c-exp.y"
    { write_exp_elt_opcode (OP_OBJC_NSSTRING);
			  write_exp_string ((yyvsp[(1) - (1)].sval));
			  write_exp_elt_opcode (OP_OBJC_NSSTRING); }
    break;

  case 101:
#line 856 "c-exp.y"
    { write_exp_elt_opcode (OP_LONG);
                          write_exp_elt_type (parse_type->builtin_bool);
                          write_exp_elt_longcst ((LONGEST) 1);
                          write_exp_elt_opcode (OP_LONG); }
    break;

  case 102:
#line 863 "c-exp.y"
    { write_exp_elt_opcode (OP_LONG);
                          write_exp_elt_type (parse_type->builtin_bool);
                          write_exp_elt_longcst ((LONGEST) 0);
                          write_exp_elt_opcode (OP_LONG); }
    break;

  case 103:
#line 872 "c-exp.y"
    {
			  if ((yyvsp[(1) - (1)].ssym).sym)
			    (yyval.bval) = SYMBOL_BLOCK_VALUE ((yyvsp[(1) - (1)].ssym).sym);
			  else
			    error (_("No file or function \"%s\"."),
				   copy_name ((yyvsp[(1) - (1)].ssym).stoken));
			}
    break;

  case 104:
#line 880 "c-exp.y"
    {
			  (yyval.bval) = (yyvsp[(1) - (1)].bval);
			}
    break;

  case 105:
#line 886 "c-exp.y"
    { struct symbol *tem
			    = lookup_symbol (copy_name ((yyvsp[(3) - (3)].sval)), (yyvsp[(1) - (3)].bval),
					     VAR_DOMAIN, NULL);
			  if (!tem || SYMBOL_CLASS (tem) != LOC_BLOCK)
			    error (_("No function \"%s\" in specified context."),
				   copy_name ((yyvsp[(3) - (3)].sval)));
			  (yyval.bval) = SYMBOL_BLOCK_VALUE (tem); }
    break;

  case 106:
#line 896 "c-exp.y"
    { struct symbol *sym = (yyvsp[(1) - (2)].ssym).sym;

			  if (sym == NULL || !SYMBOL_IS_ARGUMENT (sym)
			      || !symbol_read_needs_frame (sym))
			    error (_("@entry can be used only for function "
				     "parameters, not for \"%s\""),
				   copy_name ((yyvsp[(1) - (2)].ssym).stoken));

			  write_exp_elt_opcode (OP_VAR_ENTRY_VALUE);
			  write_exp_elt_sym (sym);
			  write_exp_elt_opcode (OP_VAR_ENTRY_VALUE);
			}
    break;

  case 107:
#line 911 "c-exp.y"
    { struct symbol *sym;
			  sym = lookup_symbol (copy_name ((yyvsp[(3) - (3)].sval)), (yyvsp[(1) - (3)].bval),
					       VAR_DOMAIN, NULL);
			  if (sym == 0)
			    error (_("No symbol \"%s\" in specified context."),
				   copy_name ((yyvsp[(3) - (3)].sval)));
			  if (symbol_read_needs_frame (sym))
			    {
			      if (innermost_block == 0
				  || contained_in (block_found,
						   innermost_block))
				innermost_block = block_found;
			    }

			  write_exp_elt_opcode (OP_VAR_VALUE);
			  /* block_found is set by lookup_symbol.  */
			  write_exp_elt_block (block_found);
			  write_exp_elt_sym (sym);
			  write_exp_elt_opcode (OP_VAR_VALUE); }
    break;

  case 108:
#line 933 "c-exp.y"
    {
			  struct type *type = (yyvsp[(1) - (3)].tsym).type;
			  CHECK_TYPEDEF (type);
			  if (TYPE_CODE (type) != TYPE_CODE_STRUCT
			      && TYPE_CODE (type) != TYPE_CODE_UNION
			      && TYPE_CODE (type) != TYPE_CODE_NAMESPACE)
			    error (_("`%s' is not defined as an aggregate type."),
				   TYPE_SAFE_NAME (type));

			  write_exp_elt_opcode (OP_SCOPE);
			  write_exp_elt_type (type);
			  write_exp_string ((yyvsp[(3) - (3)].sval));
			  write_exp_elt_opcode (OP_SCOPE);
			}
    break;

  case 109:
#line 948 "c-exp.y"
    {
			  struct type *type = (yyvsp[(1) - (4)].tsym).type;
			  struct stoken tmp_token;
			  CHECK_TYPEDEF (type);
			  if (TYPE_CODE (type) != TYPE_CODE_STRUCT
			      && TYPE_CODE (type) != TYPE_CODE_UNION
			      && TYPE_CODE (type) != TYPE_CODE_NAMESPACE)
			    error (_("`%s' is not defined as an aggregate type."),
				   TYPE_SAFE_NAME (type));

			  tmp_token.ptr = (char*) alloca ((yyvsp[(4) - (4)].sval).length + 2);
			  tmp_token.length = (yyvsp[(4) - (4)].sval).length + 1;
			  tmp_token.ptr[0] = '~';
			  memcpy (tmp_token.ptr+1, (yyvsp[(4) - (4)].sval).ptr, (yyvsp[(4) - (4)].sval).length);
			  tmp_token.ptr[tmp_token.length] = 0;

			  /* Check for valid destructor name.  */
			  destructor_name_p (tmp_token.ptr, (yyvsp[(1) - (4)].tsym).type);
			  write_exp_elt_opcode (OP_SCOPE);
			  write_exp_elt_type (type);
			  write_exp_string (tmp_token);
			  write_exp_elt_opcode (OP_SCOPE);
			}
    break;

  case 110:
#line 972 "c-exp.y"
    {
			  char *copy = copy_name ((yyvsp[(3) - (5)].sval));
			  error (_("No type \"%s\" within class "
				   "or namespace \"%s\"."),
				 copy, TYPE_SAFE_NAME ((yyvsp[(1) - (5)].tsym).type));
			}
    break;

  case 112:
#line 982 "c-exp.y"
    {
			  char *name = copy_name ((yyvsp[(2) - (2)].ssym).stoken);
			  struct symbol *sym;
			  struct minimal_symbol *msymbol;

			  sym =
			    lookup_symbol (name, (const struct block *) NULL,
					   VAR_DOMAIN, NULL);
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
			    error (_("No symbol table is loaded.  Use the \"file\" command."));
			  else
			    error (_("No symbol \"%s\" in current context."), name);
			}
    break;

  case 113:
#line 1010 "c-exp.y"
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
				error (_("No symbol table is loaded.  Use the \"file\" command."));
			      else
				error (_("No symbol \"%s\" in current context."),
				       copy_name ((yyvsp[(1) - (1)].ssym).stoken));
			    }
			}
    break;

  case 114:
#line 1074 "c-exp.y"
    { insert_type_address_space (copy_name ((yyvsp[(2) - (2)].ssym).stoken)); }
    break;

  case 122:
#line 1095 "c-exp.y"
    { insert_type (tp_pointer); }
    break;

  case 124:
#line 1098 "c-exp.y"
    { insert_type (tp_pointer); }
    break;

  case 126:
#line 1101 "c-exp.y"
    { insert_type (tp_reference); }
    break;

  case 127:
#line 1103 "c-exp.y"
    { insert_type (tp_reference); }
    break;

  case 128:
#line 1107 "c-exp.y"
    {
			  (yyval.type_stack) = get_type_stack ();
			  /* This cleanup is eventually run by
			     c_parse.  */
			  make_cleanup (type_stack_cleanup, (yyval.type_stack));
			}
    break;

  case 129:
#line 1116 "c-exp.y"
    { (yyval.type_stack) = append_type_stack ((yyvsp[(2) - (2)].type_stack), (yyvsp[(1) - (2)].type_stack)); }
    break;

  case 132:
#line 1122 "c-exp.y"
    { (yyval.type_stack) = (yyvsp[(2) - (3)].type_stack); }
    break;

  case 133:
#line 1124 "c-exp.y"
    {
			  push_type_stack ((yyvsp[(1) - (2)].type_stack));
			  push_type_int ((yyvsp[(2) - (2)].lval));
			  push_type (tp_array);
			  (yyval.type_stack) = get_type_stack ();
			}
    break;

  case 134:
#line 1131 "c-exp.y"
    {
			  push_type_int ((yyvsp[(1) - (1)].lval));
			  push_type (tp_array);
			  (yyval.type_stack) = get_type_stack ();
			}
    break;

  case 135:
#line 1137 "c-exp.y"
    {
			  push_type_stack ((yyvsp[(1) - (2)].type_stack));
			  push_typelist ((yyvsp[(2) - (2)].tvec));
			  (yyval.type_stack) = get_type_stack ();
			}
    break;

  case 136:
#line 1143 "c-exp.y"
    {
			  push_typelist ((yyvsp[(1) - (1)].tvec));
			  (yyval.type_stack) = get_type_stack ();
			}
    break;

  case 137:
#line 1150 "c-exp.y"
    { (yyval.lval) = -1; }
    break;

  case 138:
#line 1152 "c-exp.y"
    { (yyval.lval) = -1; }
    break;

  case 139:
#line 1154 "c-exp.y"
    { (yyval.lval) = (yyvsp[(2) - (3)].typed_val_int).val; }
    break;

  case 140:
#line 1156 "c-exp.y"
    { (yyval.lval) = (yyvsp[(2) - (3)].typed_val_int).val; }
    break;

  case 141:
#line 1160 "c-exp.y"
    { (yyval.tvec) = NULL; }
    break;

  case 142:
#line 1162 "c-exp.y"
    { (yyval.tvec) = (yyvsp[(2) - (3)].tvec); }
    break;

  case 144:
#line 1178 "c-exp.y"
    { (yyval.tval) = (yyvsp[(1) - (1)].tsym).type; }
    break;

  case 145:
#line 1180 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "int"); }
    break;

  case 146:
#line 1184 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "long"); }
    break;

  case 147:
#line 1188 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "short"); }
    break;

  case 148:
#line 1192 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "long"); }
    break;

  case 149:
#line 1196 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "long"); }
    break;

  case 150:
#line 1200 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "long"); }
    break;

  case 151:
#line 1204 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "long"); }
    break;

  case 152:
#line 1208 "c-exp.y"
    { (yyval.tval) = lookup_unsigned_typename (parse_language,
							 parse_gdbarch,
							 "long"); }
    break;

  case 153:
#line 1212 "c-exp.y"
    { (yyval.tval) = lookup_unsigned_typename (parse_language,
							 parse_gdbarch,
							 "long"); }
    break;

  case 154:
#line 1216 "c-exp.y"
    { (yyval.tval) = lookup_unsigned_typename (parse_language,
							 parse_gdbarch,
							 "long"); }
    break;

  case 155:
#line 1220 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "long long"); }
    break;

  case 156:
#line 1224 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "long long"); }
    break;

  case 157:
#line 1228 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "long long"); }
    break;

  case 158:
#line 1232 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "long long"); }
    break;

  case 159:
#line 1236 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "long long"); }
    break;

  case 160:
#line 1240 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "long long"); }
    break;

  case 161:
#line 1244 "c-exp.y"
    { (yyval.tval) = lookup_unsigned_typename (parse_language,
							 parse_gdbarch,
							 "long long"); }
    break;

  case 162:
#line 1248 "c-exp.y"
    { (yyval.tval) = lookup_unsigned_typename (parse_language,
							 parse_gdbarch,
							 "long long"); }
    break;

  case 163:
#line 1252 "c-exp.y"
    { (yyval.tval) = lookup_unsigned_typename (parse_language,
							 parse_gdbarch,
							 "long long"); }
    break;

  case 164:
#line 1256 "c-exp.y"
    { (yyval.tval) = lookup_unsigned_typename (parse_language,
							 parse_gdbarch,
							 "long long"); }
    break;

  case 165:
#line 1260 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "short"); }
    break;

  case 166:
#line 1264 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "short"); }
    break;

  case 167:
#line 1268 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "short"); }
    break;

  case 168:
#line 1272 "c-exp.y"
    { (yyval.tval) = lookup_unsigned_typename (parse_language,
							 parse_gdbarch,
							 "short"); }
    break;

  case 169:
#line 1276 "c-exp.y"
    { (yyval.tval) = lookup_unsigned_typename (parse_language,
							 parse_gdbarch,
							 "short"); }
    break;

  case 170:
#line 1280 "c-exp.y"
    { (yyval.tval) = lookup_unsigned_typename (parse_language,
							 parse_gdbarch,
							 "short"); }
    break;

  case 171:
#line 1284 "c-exp.y"
    { (yyval.tval) = lookup_typename (parse_language, parse_gdbarch,
						"double", (struct block *) NULL,
						0); }
    break;

  case 172:
#line 1288 "c-exp.y"
    { (yyval.tval) = lookup_typename (parse_language, parse_gdbarch,
						"long double",
						(struct block *) NULL, 0); }
    break;

  case 173:
#line 1292 "c-exp.y"
    { (yyval.tval) = parse_type->builtin_void; }
    break;

  case 174:
#line 1294 "c-exp.y"
    { (yyval.tval) = lookup_struct (copy_name ((yyvsp[(2) - (2)].sval)),
					      expression_context_block); }
    break;

  case 175:
#line 1297 "c-exp.y"
    {
			  mark_completion_tag (TYPE_CODE_STRUCT, "", 0);
			  (yyval.tval) = NULL;
			}
    break;

  case 176:
#line 1302 "c-exp.y"
    {
			  mark_completion_tag (TYPE_CODE_STRUCT, (yyvsp[(2) - (3)].sval).ptr,
					       (yyvsp[(2) - (3)].sval).length);
			  (yyval.tval) = NULL;
			}
    break;

  case 177:
#line 1308 "c-exp.y"
    { (yyval.tval) = lookup_struct (copy_name ((yyvsp[(2) - (2)].sval)),
					      expression_context_block); }
    break;

  case 178:
#line 1311 "c-exp.y"
    {
			  mark_completion_tag (TYPE_CODE_CLASS, "", 0);
			  (yyval.tval) = NULL;
			}
    break;

  case 179:
#line 1316 "c-exp.y"
    {
			  mark_completion_tag (TYPE_CODE_CLASS, (yyvsp[(2) - (3)].sval).ptr,
					       (yyvsp[(2) - (3)].sval).length);
			  (yyval.tval) = NULL;
			}
    break;

  case 180:
#line 1322 "c-exp.y"
    { (yyval.tval) = lookup_union (copy_name ((yyvsp[(2) - (2)].sval)),
					     expression_context_block); }
    break;

  case 181:
#line 1325 "c-exp.y"
    {
			  mark_completion_tag (TYPE_CODE_UNION, "", 0);
			  (yyval.tval) = NULL;
			}
    break;

  case 182:
#line 1330 "c-exp.y"
    {
			  mark_completion_tag (TYPE_CODE_UNION, (yyvsp[(2) - (3)].sval).ptr,
					       (yyvsp[(2) - (3)].sval).length);
			  (yyval.tval) = NULL;
			}
    break;

  case 183:
#line 1336 "c-exp.y"
    { (yyval.tval) = lookup_enum (copy_name ((yyvsp[(2) - (2)].sval)),
					    expression_context_block); }
    break;

  case 184:
#line 1339 "c-exp.y"
    {
			  mark_completion_tag (TYPE_CODE_ENUM, "", 0);
			  (yyval.tval) = NULL;
			}
    break;

  case 185:
#line 1344 "c-exp.y"
    {
			  mark_completion_tag (TYPE_CODE_ENUM, (yyvsp[(2) - (3)].sval).ptr,
					       (yyvsp[(2) - (3)].sval).length);
			  (yyval.tval) = NULL;
			}
    break;

  case 186:
#line 1350 "c-exp.y"
    { (yyval.tval) = lookup_unsigned_typename (parse_language,
							 parse_gdbarch,
							 TYPE_NAME((yyvsp[(2) - (2)].tsym).type)); }
    break;

  case 187:
#line 1354 "c-exp.y"
    { (yyval.tval) = lookup_unsigned_typename (parse_language,
							 parse_gdbarch,
							 "int"); }
    break;

  case 188:
#line 1358 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       TYPE_NAME((yyvsp[(2) - (2)].tsym).type)); }
    break;

  case 189:
#line 1362 "c-exp.y"
    { (yyval.tval) = lookup_signed_typename (parse_language,
						       parse_gdbarch,
						       "int"); }
    break;

  case 190:
#line 1369 "c-exp.y"
    { (yyval.tval) = lookup_template_type(copy_name((yyvsp[(2) - (5)].sval)), (yyvsp[(4) - (5)].tval),
						    expression_context_block);
			}
    break;

  case 191:
#line 1373 "c-exp.y"
    { (yyval.tval) = follow_types ((yyvsp[(2) - (2)].tval)); }
    break;

  case 192:
#line 1375 "c-exp.y"
    { (yyval.tval) = follow_types ((yyvsp[(1) - (2)].tval)); }
    break;

  case 194:
#line 1380 "c-exp.y"
    {
		  (yyval.tsym).stoken.ptr = "int";
		  (yyval.tsym).stoken.length = 3;
		  (yyval.tsym).type = lookup_signed_typename (parse_language,
						    parse_gdbarch,
						    "int");
		}
    break;

  case 195:
#line 1388 "c-exp.y"
    {
		  (yyval.tsym).stoken.ptr = "long";
		  (yyval.tsym).stoken.length = 4;
		  (yyval.tsym).type = lookup_signed_typename (parse_language,
						    parse_gdbarch,
						    "long");
		}
    break;

  case 196:
#line 1396 "c-exp.y"
    {
		  (yyval.tsym).stoken.ptr = "short";
		  (yyval.tsym).stoken.length = 5;
		  (yyval.tsym).type = lookup_signed_typename (parse_language,
						    parse_gdbarch,
						    "short");
		}
    break;

  case 197:
#line 1404 "c-exp.y"
    {
		  (yyval.tsym).stoken.ptr = "void";
		  (yyval.tsym).stoken.length = 4;
		  (yyval.tsym).type = parse_type->builtin_void;
		}
    break;

  case 198:
#line 1413 "c-exp.y"
    { check_parameter_typelist ((yyvsp[(1) - (1)].tvec)); }
    break;

  case 199:
#line 1415 "c-exp.y"
    {
			  VEC_safe_push (type_ptr, (yyvsp[(1) - (3)].tvec), NULL);
			  check_parameter_typelist ((yyvsp[(1) - (3)].tvec));
			  (yyval.tvec) = (yyvsp[(1) - (3)].tvec);
			}
    break;

  case 200:
#line 1424 "c-exp.y"
    {
		  VEC (type_ptr) *typelist = NULL;
		  VEC_safe_push (type_ptr, typelist, (yyvsp[(1) - (1)].tval));
		  (yyval.tvec) = typelist;
		}
    break;

  case 201:
#line 1430 "c-exp.y"
    {
		  VEC_safe_push (type_ptr, (yyvsp[(1) - (3)].tvec), (yyvsp[(3) - (3)].tval));
		  (yyval.tvec) = (yyvsp[(1) - (3)].tvec);
		}
    break;

  case 202:
#line 1436 "c-exp.y"
    { push_type (tp_shared);
			  push_type (tp_relaxed); 
			}
    break;

  case 203:
#line 1440 "c-exp.y"
    { push_type (tp_shared);
			  push_type (tp_relaxed); 
			}
    break;

  case 204:
#line 1445 "c-exp.y"
    { push_type (tp_shared);
			  push_type (tp_strict); 
			}
    break;

  case 205:
#line 1449 "c-exp.y"
    { push_type (tp_shared);
			  push_type (tp_strict); 
			}
    break;

  case 206:
#line 1455 "c-exp.y"
    { push_type (tp_shared); }
    break;

  case 214:
#line 1468 "c-exp.y"
    {
		  push_type_stack ((yyvsp[(2) - (2)].type_stack));
		  (yyval.tval) = follow_types ((yyvsp[(1) - (2)].tval));
		}
    break;

  case 215:
#line 1475 "c-exp.y"
    { (yyval.tval) = follow_types ((yyvsp[(1) - (2)].tval)); }
    break;

  case 218:
#line 1483 "c-exp.y"
    { insert_type (tp_const);
			  insert_type (tp_volatile); 
			}
    break;

  case 219:
#line 1487 "c-exp.y"
    { insert_type (tp_const);
			  insert_type (tp_volatile); 
			}
    break;

  case 220:
#line 1491 "c-exp.y"
    { insert_type (tp_const); }
    break;

  case 221:
#line 1493 "c-exp.y"
    { insert_type (tp_volatile); }
    break;

  case 222:
#line 1497 "c-exp.y"
    { (yyval.sval) = operator_stoken (" new"); }
    break;

  case 223:
#line 1499 "c-exp.y"
    { (yyval.sval) = operator_stoken (" delete"); }
    break;

  case 224:
#line 1501 "c-exp.y"
    { (yyval.sval) = operator_stoken (" new[]"); }
    break;

  case 225:
#line 1503 "c-exp.y"
    { (yyval.sval) = operator_stoken (" delete[]"); }
    break;

  case 226:
#line 1505 "c-exp.y"
    { (yyval.sval) = operator_stoken (" new[]"); }
    break;

  case 227:
#line 1507 "c-exp.y"
    { (yyval.sval) = operator_stoken (" delete[]"); }
    break;

  case 228:
#line 1509 "c-exp.y"
    { (yyval.sval) = operator_stoken ("+"); }
    break;

  case 229:
#line 1511 "c-exp.y"
    { (yyval.sval) = operator_stoken ("-"); }
    break;

  case 230:
#line 1513 "c-exp.y"
    { (yyval.sval) = operator_stoken ("*"); }
    break;

  case 231:
#line 1515 "c-exp.y"
    { (yyval.sval) = operator_stoken ("/"); }
    break;

  case 232:
#line 1517 "c-exp.y"
    { (yyval.sval) = operator_stoken ("%"); }
    break;

  case 233:
#line 1519 "c-exp.y"
    { (yyval.sval) = operator_stoken ("^"); }
    break;

  case 234:
#line 1521 "c-exp.y"
    { (yyval.sval) = operator_stoken ("&"); }
    break;

  case 235:
#line 1523 "c-exp.y"
    { (yyval.sval) = operator_stoken ("|"); }
    break;

  case 236:
#line 1525 "c-exp.y"
    { (yyval.sval) = operator_stoken ("~"); }
    break;

  case 237:
#line 1527 "c-exp.y"
    { (yyval.sval) = operator_stoken ("!"); }
    break;

  case 238:
#line 1529 "c-exp.y"
    { (yyval.sval) = operator_stoken ("="); }
    break;

  case 239:
#line 1531 "c-exp.y"
    { (yyval.sval) = operator_stoken ("<"); }
    break;

  case 240:
#line 1533 "c-exp.y"
    { (yyval.sval) = operator_stoken (">"); }
    break;

  case 241:
#line 1535 "c-exp.y"
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

  case 242:
#line 1575 "c-exp.y"
    { (yyval.sval) = operator_stoken ("<<"); }
    break;

  case 243:
#line 1577 "c-exp.y"
    { (yyval.sval) = operator_stoken (">>"); }
    break;

  case 244:
#line 1579 "c-exp.y"
    { (yyval.sval) = operator_stoken ("=="); }
    break;

  case 245:
#line 1581 "c-exp.y"
    { (yyval.sval) = operator_stoken ("!="); }
    break;

  case 246:
#line 1583 "c-exp.y"
    { (yyval.sval) = operator_stoken ("<="); }
    break;

  case 247:
#line 1585 "c-exp.y"
    { (yyval.sval) = operator_stoken (">="); }
    break;

  case 248:
#line 1587 "c-exp.y"
    { (yyval.sval) = operator_stoken ("&&"); }
    break;

  case 249:
#line 1589 "c-exp.y"
    { (yyval.sval) = operator_stoken ("||"); }
    break;

  case 250:
#line 1591 "c-exp.y"
    { (yyval.sval) = operator_stoken ("++"); }
    break;

  case 251:
#line 1593 "c-exp.y"
    { (yyval.sval) = operator_stoken ("--"); }
    break;

  case 252:
#line 1595 "c-exp.y"
    { (yyval.sval) = operator_stoken (","); }
    break;

  case 253:
#line 1597 "c-exp.y"
    { (yyval.sval) = operator_stoken ("->*"); }
    break;

  case 254:
#line 1599 "c-exp.y"
    { (yyval.sval) = operator_stoken ("->"); }
    break;

  case 255:
#line 1601 "c-exp.y"
    { (yyval.sval) = operator_stoken ("()"); }
    break;

  case 256:
#line 1603 "c-exp.y"
    { (yyval.sval) = operator_stoken ("[]"); }
    break;

  case 257:
#line 1605 "c-exp.y"
    { (yyval.sval) = operator_stoken ("[]"); }
    break;

  case 258:
#line 1607 "c-exp.y"
    { char *name;
			  long length;
			  struct ui_file *buf = mem_fileopen ();

			  c_print_type ((yyvsp[(2) - (2)].tval), NULL, buf, -1, 0,
					&type_print_raw_options);
			  name = ui_file_xstrdup (buf, &length);
			  ui_file_delete (buf);
			  (yyval.sval) = operator_stoken (name);
			  xfree (name);
			}
    break;

  case 259:
#line 1622 "c-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].ssym).stoken; }
    break;

  case 260:
#line 1623 "c-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].ssym).stoken; }
    break;

  case 261:
#line 1624 "c-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].tsym).stoken; }
    break;

  case 262:
#line 1625 "c-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].ssym).stoken; }
    break;

  case 263:
#line 1626 "c-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].ssym).stoken; }
    break;

  case 264:
#line 1627 "c-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].sval); }
    break;

  case 267:
#line 1640 "c-exp.y"
    {
			  struct field_of_this_result is_a_field_of_this;

			  (yyval.ssym).stoken = (yyvsp[(1) - (1)].sval);
			  (yyval.ssym).sym = lookup_symbol ((yyvsp[(1) - (1)].sval).ptr,
						  expression_context_block,
						  VAR_DOMAIN,
						  &is_a_field_of_this);
			  (yyval.ssym).is_a_field_of_this
			    = is_a_field_of_this.type != NULL;
			}
    break;


/* Line 1267 of yacc.c.  */
#line 4042 "c-exp.c"
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


#line 1654 "c-exp.y"


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

/* Validate a parameter typelist.  */

static void
check_parameter_typelist (VEC (type_ptr) *params)
{
  struct type *type;
  int ix;

  for (ix = 0; VEC_iterate (type_ptr, params, ix, type); ++ix)
    {
      if (type != NULL && TYPE_CODE (check_typedef (type)) == TYPE_CODE_VOID)
	{
	  if (ix == 0)
	    {
	      if (VEC_length (type_ptr, params) == 1)
		{
		  /* Ok.  */
		  break;
		}
	      VEC_free (type_ptr, params);
	      error (_("parameter types following 'void'"));
	    }
	  else
	    {
	      VEC_free (type_ptr, params);
	      error (_("'void' invalid as parameter type"));
	    }
	}
    }
}

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

      if (! parse_c_float (parse_gdbarch, p, len,
			   &putithere->typed_val_float.dval,
			   &putithere->typed_val_float.type))
	return ERROR;
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
	    error (_("Numeric constant too large."));
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
  int is_objc = 0;

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
  else if (*tokptr == '@')
    {
      /* An Objective C string.  */
      is_objc = 1;
      type = C_STRING;
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
	error (_("Unterminated string in expression."));
      else
	error (_("Unmatched single quote."));
    }
  ++tokptr;

  value->type = type;
  value->ptr = obstack_base (&tempbuf);
  value->length = obstack_object_size (&tempbuf);

  *outptr = tokptr;

  return quote == '"' ? (is_objc ? NSSTRING : STRING) : CHAR;
}

/* This is used to associate some attributes with a token.  */

enum token_flags
{
  /* If this bit is set, the token is C++-only.  */

  FLAG_CXX = 1,

  /* If this bit is set, the token is conditional: if there is a
     symbol of the same name, then the token is a symbol; otherwise,
     the token is a keyword.  */

  FLAG_SHADOW = 2
};

struct token
{
  char *operator;
  int token;
  enum exp_opcode opcode;
  enum token_flags flags;
};

static const struct token tokentab3[] =
  {
    {">>=", ASSIGN_MODIFY, BINOP_RSH, 0},
    {"<<=", ASSIGN_MODIFY, BINOP_LSH, 0},
    {"->*", ARROW_STAR, BINOP_END, FLAG_CXX},
    {"...", DOTDOTDOT, BINOP_END, 0}
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
    {".*", DOT_STAR, BINOP_END, FLAG_CXX}
  };

/* Identifier-like tokens.  */
static const struct token ident_tokens[] =
  {
    {"unsigned", UNSIGNED, OP_NULL, 0},
    {"template", TEMPLATE, OP_NULL, FLAG_CXX},
    {"volatile", VOLATILE_KEYWORD, OP_NULL, 0},
    {"struct", STRUCT, OP_NULL, 0},
    {"signed", SIGNED_KEYWORD, OP_NULL, 0},
    {"sizeof", SIZEOF, OP_NULL, 0},
    {"double", DOUBLE_KEYWORD, OP_NULL, 0},
    {"false", FALSEKEYWORD, OP_NULL, FLAG_CXX},
    {"class", CLASS, OP_NULL, FLAG_CXX},
    {"union", UNION, OP_NULL, 0},
    {"short", SHORT, OP_NULL, 0},
    {"const", CONST_KEYWORD, OP_NULL, 0},
    {"enum", ENUM, OP_NULL, 0},
    {"long", LONG, OP_NULL, 0},
    {"true", TRUEKEYWORD, OP_NULL, FLAG_CXX},
    {"int", INT_KEYWORD, OP_NULL, 0},
    {"new", NEW, OP_NULL, FLAG_CXX},
    {"delete", DELETE, OP_NULL, FLAG_CXX},
    {"operator", OPERATOR, OP_NULL, FLAG_CXX},
    {"void", VOID_KEYWORD, OP_NULL, 0},

    {"and", ANDAND, BINOP_END, FLAG_CXX},
    {"and_eq", ASSIGN_MODIFY, BINOP_BITWISE_AND, FLAG_CXX},
    {"bitand", '&', OP_NULL, FLAG_CXX},
    {"bitor", '|', OP_NULL, FLAG_CXX},
    {"compl", '~', OP_NULL, FLAG_CXX},
    {"not", '!', OP_NULL, FLAG_CXX},
    {"not_eq", NOTEQUAL, BINOP_END, FLAG_CXX},
    {"or", OROR, BINOP_END, FLAG_CXX},
    {"or_eq", ASSIGN_MODIFY, BINOP_BITWISE_IOR, FLAG_CXX},
    {"xor", '^', OP_NULL, FLAG_CXX},
    {"xor_eq", ASSIGN_MODIFY, BINOP_BITWISE_XOR, FLAG_CXX},

    {"const_cast", CONST_CAST, OP_NULL, FLAG_CXX },
    {"dynamic_cast", DYNAMIC_CAST, OP_NULL, FLAG_CXX },
    {"static_cast", STATIC_CAST, OP_NULL, FLAG_CXX },
    {"reinterpret_cast", REINTERPRET_CAST, OP_NULL, FLAG_CXX },

    {"__typeof__", TYPEOF, OP_TYPEOF, 0 },
    {"__typeof", TYPEOF, OP_TYPEOF, 0 },
    {"typeof", TYPEOF, OP_TYPEOF, FLAG_SHADOW },
    {"__decltype", DECLTYPE, OP_DECLTYPE, FLAG_CXX },
    {"decltype", DECLTYPE, OP_DECLTYPE, FLAG_CXX | FLAG_SHADOW },
    
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
	if ((tokentab3[i].flags & FLAG_CXX) != 0
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
	if ((tokentab2[i].flags & FLAG_CXX) != 0
	    && parse_language->la_language != language_cplus)
	  break;

	lexptr += 2;
	yylval.opcode = tokentab2[i].opcode;
	if (parse_completion && tokentab2[i].token == ARROW)
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
      if (parse_language->la_language == language_objc && c == '[')
	return OBJC_LBRAC;
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
	  if (parse_completion)
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
	    error (_("Invalid number \"%s\"."), err_copy);
	  }
	lexptr = p;
	return toktype;
      }

    case '@':
      {
	char *p = &tokstart[1];
	size_t len = strlen ("entry");

	if (parse_language->la_language == language_objc)
	  {
	    size_t len = strlen ("selector");

	    if (strncmp (p, "selector", len) == 0
		&& (p[len] == '\0' || isspace (p[len])))
	      {
		lexptr = p + len;
		return SELECTOR;
	      }
	    else if (*p == '"')
	      goto parse_string;
	  }

	while (isspace (*p))
	  p++;
	if (strncmp (p, "entry", len) == 0 && !isalnum (p[len])
	    && p[len] != '_')
	  {
	    lexptr = &p[len];
	    return ENTRY;
	  }
      }
      /* FALLTHRU */
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

    parse_string:
      {
	int host_len;
	int result = parse_string_or_char (tokstart, &lexptr, &yylval.tsval,
					   &host_len);
	if (result == CHAR)
	  {
	    if (host_len == 0)
	      error (_("Empty character constant."));
	    else if (host_len > 2 && c == '\'')
	      {
		++tokstart;
		namelen = lexptr - tokstart - 1;
		goto tryname;
	      }
	    else if (host_len > 1)
	      error (_("Invalid character constant."));
	  }
	return result;
      }
    }

  if (!(c == '_' || c == '$'
	|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
    /* We must have come across a bad character (e.g. ';').  */
    error (_("Invalid character '%c' in expression."), c);

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
	if ((ident_tokens[i].flags & FLAG_CXX) != 0
	    && parse_language->la_language != language_cplus)
	  break;

	if ((ident_tokens[i].flags & FLAG_SHADOW) != 0)
	  {
	    struct field_of_this_result is_a_field_of_this;

	    if (lookup_symbol (copy, expression_context_block,
			       VAR_DOMAIN,
			       (parse_language->la_language == language_cplus
				? &is_a_field_of_this
				: NULL))
		!= NULL)
	      {
		/* The keyword is shadowed.  */
		break;
	      }
	  }

	/* It is ok to always set this, even though we don't always
	   strictly need to.  */
	yylval.opcode = ident_tokens[i].opcode;
	return ident_tokens[i].token;
      }

  if (*tokstart == '$')
    return VARIABLE;

  if (parse_completion && *lexptr == '\0')
    saw_name_at_eof = 1;

  yylval.ssym.stoken = yylval.sval;
  yylval.ssym.sym = NULL;
  yylval.ssym.is_a_field_of_this = 0;
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
classify_name (const struct block *block)
{
  struct symbol *sym;
  char *copy;
  struct field_of_this_result is_a_field_of_this;

  copy = copy_name (yylval.sval);

  /* Initialize this in case we *don't* use it in this call; that way
     we can refer to it unconditionally below.  */
  memset (&is_a_field_of_this, 0, sizeof (is_a_field_of_this));

  sym = lookup_symbol (copy, block, VAR_DOMAIN, 
		       parse_language->la_name_of_this
		       ? &is_a_field_of_this : NULL);

  if (sym && SYMBOL_CLASS (sym) == LOC_BLOCK)
    {
      yylval.ssym.sym = sym;
      yylval.ssym.is_a_field_of_this = is_a_field_of_this.type != NULL;
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

      /* If we found a field of 'this', we might have erroneously
	 found a constructor where we wanted a type name.  Handle this
	 case by noticing that we found a constructor and then look up
	 the type tag instead.  */
      if (is_a_field_of_this.type != NULL
	  && is_a_field_of_this.fn_field != NULL
	  && TYPE_FN_FIELD_CONSTRUCTOR (is_a_field_of_this.fn_field->fn_fields,
					0))
	{
	  struct field_of_this_result inner_is_a_field_of_this;

	  sym = lookup_symbol (copy, block, STRUCT_DOMAIN,
			       &inner_is_a_field_of_this);
	  if (sym != NULL)
	    {
	      yylval.tsym.type = SYMBOL_TYPE (sym);
	      return TYPENAME;
	    }
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

  /* See if it's an ObjC classname.  */
  if (parse_language->la_language == language_objc && !sym)
    {
      CORE_ADDR Class = lookup_objc_class (parse_gdbarch, copy);
      if (Class)
	{
	  yylval.class.class = Class;
	  sym = lookup_struct_typedef (copy, expression_context_block, 1);
	  if (sym)
	    yylval.class.type = SYMBOL_TYPE (sym);
	  return CLASSNAME;
	}
    }

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
	  yylval.ssym.is_a_field_of_this = is_a_field_of_this.type != NULL;
	  return NAME_OR_INT;
	}
    }

  /* Any other kind of symbol */
  yylval.ssym.sym = sym;
  yylval.ssym.is_a_field_of_this = is_a_field_of_this.type != NULL;

  if (sym == NULL
      && parse_language->la_language == language_cplus
      && is_a_field_of_this.type == NULL
      && !lookup_minimal_symbol (copy, NULL, NULL))
    return UNKNOWN_CPP_NAME;

  return NAME;
}

/* Like classify_name, but used by the inner loop of the lexer, when a
   name might have already been seen.  CONTEXT is the context type, or
   NULL if this is the first component of a name.  */

static int
classify_inner_name (const struct block *block, struct type *context)
{
  struct type *type;
  char *copy;

  if (context == NULL)
    return classify_name (block);

  type = check_typedef (context);
  if (TYPE_CODE (type) != TYPE_CODE_STRUCT
      && TYPE_CODE (type) != TYPE_CODE_UNION
      && TYPE_CODE (type) != TYPE_CODE_NAMESPACE)
    return ERROR;

  copy = copy_name (yylval.ssym.stoken);
  yylval.ssym.sym = cp_lookup_nested_symbol (type, copy, block);
  if (yylval.ssym.sym == NULL)
    return ERROR;

  switch (SYMBOL_CLASS (yylval.ssym.sym))
    {
    case LOC_BLOCK:
    case LOC_LABEL:
      return ERROR;

    case LOC_TYPEDEF:
      yylval.tsym.type = SYMBOL_TYPE (yylval.ssym.sym);;
      return TYPENAME;

    default:
      return NAME;
    }
  internal_error (__FILE__, __LINE__, _("not reached"));
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
  struct type *context_type = NULL;

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
    {
      obstack_grow (&name_obstack, yylval.sval.ptr, yylval.sval.length);
      context_type = yylval.tsym.type;
    }
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
						context_type);
	  /* We keep going until we either run out of names, or until
	     we have a qualified name which is not a type.  */
	  if (classification != TYPENAME && classification != NAME)
	    {
	      /* Push the final component and leave the loop.  */
	      VEC_safe_push (token_and_value, token_fifo, &next);
	      break;
	    }

	  /* Update the partial name we are constructing.  */
	  if (context_type != NULL)
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
	  
	  if (classification == NAME)
	    break;

	  context_type = yylval.tsym.type;
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

  error (_("A %s in expression, near `%s'."), (msg ? msg : "error"), lexptr);
}

