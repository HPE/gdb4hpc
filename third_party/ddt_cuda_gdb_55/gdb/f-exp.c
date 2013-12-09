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
     STRING_LITERAL = 260,
     BOOLEAN_LITERAL = 261,
     NAME = 262,
     TYPENAME = 263,
     NAME_OR_INT = 264,
     POINTER = 265,
     SIZEOF = 266,
     COLONCOLON = 267,
     KIND = 268,
     ERROR = 269,
     INT_KEYWORD = 270,
     INT_S2_KEYWORD = 271,
     INT_S8_KEYWORD = 272,
     LOGICAL_S1_KEYWORD = 273,
     LOGICAL_S2_KEYWORD = 274,
     LOGICAL_S8_KEYWORD = 275,
     LOGICAL_KEYWORD = 276,
     REAL_KEYWORD = 277,
     REAL_S8_KEYWORD = 278,
     REAL_S16_KEYWORD = 279,
     COMPLEX_S8_KEYWORD = 280,
     COMPLEX_S16_KEYWORD = 281,
     COMPLEX_S32_KEYWORD = 282,
     BOOL_AND = 283,
     BOOL_OR = 284,
     BOOL_NOT = 285,
     BIN_MOD = 286,
     CHARACTER = 287,
     VARIABLE = 288,
     ASSIGN_MODIFY = 289,
     UNOP_INTRINSIC = 290,
     BINOP_INTRINSIC = 291,
     ABOVE_COMMA = 292,
     NOTEQUAL = 293,
     EQUAL = 294,
     GEQ = 295,
     LEQ = 296,
     GREATERTHAN = 297,
     LESSTHAN = 298,
     RSH = 299,
     LSH = 300,
     STARSTAR = 301,
     UNARY = 302,
     ARROW = 303,
     BELOW_SIZE = 304,
     SIZE = 305
   };
#endif
/* Tokens.  */
#define INT 258
#define FLOAT 259
#define STRING_LITERAL 260
#define BOOLEAN_LITERAL 261
#define NAME 262
#define TYPENAME 263
#define NAME_OR_INT 264
#define POINTER 265
#define SIZEOF 266
#define COLONCOLON 267
#define KIND 268
#define ERROR 269
#define INT_KEYWORD 270
#define INT_S2_KEYWORD 271
#define INT_S8_KEYWORD 272
#define LOGICAL_S1_KEYWORD 273
#define LOGICAL_S2_KEYWORD 274
#define LOGICAL_S8_KEYWORD 275
#define LOGICAL_KEYWORD 276
#define REAL_KEYWORD 277
#define REAL_S8_KEYWORD 278
#define REAL_S16_KEYWORD 279
#define COMPLEX_S8_KEYWORD 280
#define COMPLEX_S16_KEYWORD 281
#define COMPLEX_S32_KEYWORD 282
#define BOOL_AND 283
#define BOOL_OR 284
#define BOOL_NOT 285
#define BIN_MOD 286
#define CHARACTER 287
#define VARIABLE 288
#define ASSIGN_MODIFY 289
#define UNOP_INTRINSIC 290
#define BINOP_INTRINSIC 291
#define ABOVE_COMMA 292
#define NOTEQUAL 293
#define EQUAL 294
#define GEQ 295
#define LEQ 296
#define GREATERTHAN 297
#define LESSTHAN 298
#define RSH 299
#define LSH 300
#define STARSTAR 301
#define UNARY 302
#define ARROW 303
#define BELOW_SIZE 304
#define SIZE 305




/* Copy the first part of user declarations.  */
#line 44 "f-exp.y"


#include "defs.h"
#include "gdb_string.h"
#include "expression.h"
#include "value.h"
#include "parser-defs.h"
#include "language.h"
#include "f-lang.h"
#include "f-module.h"
#include "bfd.h" /* Required by objfiles.h.  */
#include "symfile.h" /* Required by objfiles.h.  */
#include "objfiles.h" /* For have_full_symbols and have_partial_symbols */
#include "block.h"
#include <ctype.h>

#define parse_type builtin_type (parse_gdbarch)
#define parse_f_type builtin_f_type (parse_gdbarch)

/* Remap normal yacc parser interface names (yyparse, yylex, yyerror, etc),
   as well as gratuitiously global symbol names, so we can have multiple
   yacc generated parsers in gdb.  Note that these are only the variables
   produced by yacc.  If other parser generators (bison, byacc, etc) produce
   additional global names that conflict at link time, then those parser
   generators need to be fixed instead of adding those names to this list. */

#define	yymaxdepth f_maxdepth
#define	yyparse	f_parse
#define	yylex	f_lex
#define	yyerror	f_error
#define	yylval	f_lval
#define	yychar	f_char
#define	yydebug	f_debug
#define	yypact	f_pact	
#define	yyr1	f_r1			
#define	yyr2	f_r2			
#define	yydef	f_def		
#define	yychk	f_chk		
#define	yypgo	f_pgo		
#define	yyact	f_act		
#define	yyexca	f_exca
#define yyerrflag f_errflag
#define yynerrs	f_nerrs
#define	yyps	f_ps
#define	yypv	f_pv
#define	yys	f_s
#define	yy_yys	f_yys
#define	yystate	f_state
#define	yytmp	f_tmp
#define	yyv	f_v
#define	yy_yyv	f_yyv
#define	yyval	f_val
#define	yylloc	f_lloc
#define yyreds	f_reds		/* With YYDEBUG defined */
#define yytoks	f_toks		/* With YYDEBUG defined */
#define yyname	f_name		/* With YYDEBUG defined */
#define yyrule	f_rule		/* With YYDEBUG defined */
#define yylhs	f_yylhs
#define yylen	f_yylen
#define yydefred f_yydefred
#define yydgoto	f_yydgoto
#define yysindex f_yysindex
#define yyrindex f_yyrindex
#define yygindex f_yygindex
#define yytable	 f_yytable
#define yycheck	 f_yycheck

#ifndef YYDEBUG
#define	YYDEBUG	1		/* Default to yydebug support */
#endif

#define YYFPRINTF parser_fprintf

int yyparse (void);

static int yylex (void);

void yyerror (char *);

static void growbuf_by_size (int);

static int match_string_literal (void);

static struct type *follow_f_types (struct type *follow_type);



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
#line 136 "f-exp.y"
{
    LONGEST lval;
    struct {
      LONGEST val;
      struct type *type;
    } typed_val;
    DOUBLEST dval;
    struct symbol *sym;
    struct type *tval;
    struct stoken sval;
    struct ttype tsym;
    struct symtoken ssym;
    int voidval;
    struct block *bval;
    enum exp_opcode opcode;
    struct internalvar *ivar;

    struct type **tvec;
    int *ivec;
  }
/* Line 187 of yacc.c.  */
#line 304 "f-exp.c.tmp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
#line 157 "f-exp.y"

/* YYSTYPE gets defined by %union */
static int parse_number (char *, int, int, YYSTYPE *);


/* Line 216 of yacc.c.  */
#line 321 "f-exp.c.tmp"

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
#define YYFINAL  62
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   892

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  70
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  116
/* YYNRULES -- Number of states.  */
#define YYNSTATES  198

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   305

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    58,    43,     2,
      60,    64,    55,    53,    37,    54,    69,    56,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    66,     2,
      68,    39,    67,    40,    52,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    42,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    41,     2,    65,     2,     2,     2,
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
      35,    36,    38,    44,    45,    46,    47,    48,    49,    50,
      51,    57,    59,    61,    62,    63
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    13,    16,    19,    22,
      25,    28,    31,    32,    38,    43,    50,    51,    53,    55,
      59,    63,    67,    70,    73,    75,    79,    83,    88,    92,
      96,   100,   104,   108,   112,   116,   120,   124,   128,   132,
     136,   140,   144,   148,   152,   156,   160,   164,   168,   172,
     176,   180,   184,   188,   190,   192,   194,   196,   198,   203,
     205,   207,   211,   215,   217,   219,   221,   225,   228,   233,
     237,   241,   243,   247,   250,   253,   255,   257,   259,   262,
     264,   267,   269,   271,   274,   277,   281,   283,   286,   288,
     290,   294,   300,   302,   304,   306,   310,   316,   318,   322,
     328,   330,   332,   334,   336,   340,   346,   348,   350,   354,
     360,   362,   364,   366,   368,   370,   372
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      71,     0,    -1,    73,    -1,    72,    -1,    79,    -1,    60,
      73,    64,    -1,    55,    73,    -1,    43,    73,    -1,    54,
      73,    -1,    30,    73,    -1,    65,    73,    -1,    11,    73,
      -1,    -1,    73,    60,    74,    75,    64,    -1,    35,    60,
      73,    64,    -1,    36,    60,    73,    37,    73,    64,    -1,
      -1,    73,    -1,    76,    -1,    75,    37,    73,    -1,    75,
      37,    76,    -1,    73,    66,    73,    -1,    73,    85,    -1,
      84,    73,    -1,    86,    -1,    73,    37,    73,    -1,    60,
      77,    64,    -1,    60,    79,    64,    73,    -1,    73,    58,
      89,    -1,    73,    52,    73,    -1,    73,    57,    73,    -1,
      73,    55,    73,    -1,    73,    56,    73,    -1,    73,    53,
      73,    -1,    73,    54,    73,    -1,    73,    51,    73,    -1,
      73,    50,    73,    -1,    73,    45,    73,    -1,    73,    44,
      73,    -1,    73,    47,    73,    -1,    73,    46,    73,    -1,
      73,    49,    73,    -1,    73,    48,    73,    -1,    73,    67,
      73,    -1,    73,    68,    73,    -1,    73,    43,    73,    -1,
      73,    42,    73,    -1,    73,    41,    73,    -1,    73,    28,
      73,    -1,    73,    29,    73,    -1,    73,    31,    73,    -1,
      73,    39,    73,    -1,    73,    34,    73,    -1,     3,    -1,
       9,    -1,     4,    -1,    78,    -1,    33,    -1,    11,    60,
      79,    64,    -1,     6,    -1,     5,    -1,    73,    61,    89,
      -1,    73,    69,    89,    -1,    90,    -1,    80,    -1,    88,
      -1,    88,    37,    10,    -1,    88,    81,    -1,    88,    37,
      10,    81,    -1,    60,    82,    64,    -1,    83,    37,    83,
      -1,    83,    -1,    87,    66,    87,    -1,    87,    85,    -1,
      84,    87,    -1,    86,    -1,    87,    -1,    66,    -1,    55,
      66,    -1,    66,    -1,    66,    55,    -1,    66,    -1,    55,
      -1,    66,    55,    -1,    55,    41,    -1,    55,    66,    55,
      -1,     3,    -1,    54,     3,    -1,     8,    -1,    15,    -1,
      15,    55,     3,    -1,    15,    13,    39,     3,    64,    -1,
      16,    -1,    17,    -1,    32,    -1,    32,    55,     3,    -1,
      32,    13,    39,     3,    64,    -1,    21,    -1,    21,    55,
       3,    -1,    21,    13,    39,     3,    64,    -1,    20,    -1,
      19,    -1,    18,    -1,    22,    -1,    22,    55,     3,    -1,
      22,    13,    39,     3,    64,    -1,    23,    -1,    24,    -1,
      25,    55,     3,    -1,    25,    13,    39,     3,    64,    -1,
      25,    -1,    26,    -1,    27,    -1,     7,    -1,     8,    -1,
       9,    -1,     7,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   242,   242,   243,   246,   252,   257,   261,   265,   269,
     273,   277,   287,   286,   294,   298,   301,   304,   308,   312,
     316,   322,   328,   334,   340,   346,   350,   356,   362,   370,
     374,   378,   382,   386,   390,   394,   398,   402,   406,   410,
     414,   418,   422,   426,   430,   434,   438,   442,   446,   450,
     454,   458,   462,   468,   475,   484,   491,   494,   497,   505,
     512,   520,   526,   532,   571,   574,   575,   577,   579,   583,
     586,   587,   590,   597,   604,   610,   616,   622,   623,   626,
     627,   630,   631,   632,   633,   634,   637,   639,   644,   646,
     648,   655,   662,   664,   666,   668,   670,   672,   674,   683,
     692,   694,   696,   698,   700,   707,   714,   716,   718,   725,
     732,   734,   736,   740,   742,   744,   748
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "FLOAT", "STRING_LITERAL",
  "BOOLEAN_LITERAL", "NAME", "TYPENAME", "NAME_OR_INT", "POINTER",
  "SIZEOF", "COLONCOLON", "KIND", "ERROR", "INT_KEYWORD", "INT_S2_KEYWORD",
  "INT_S8_KEYWORD", "LOGICAL_S1_KEYWORD", "LOGICAL_S2_KEYWORD",
  "LOGICAL_S8_KEYWORD", "LOGICAL_KEYWORD", "REAL_KEYWORD",
  "REAL_S8_KEYWORD", "REAL_S16_KEYWORD", "COMPLEX_S8_KEYWORD",
  "COMPLEX_S16_KEYWORD", "COMPLEX_S32_KEYWORD", "BOOL_AND", "BOOL_OR",
  "BOOL_NOT", "BIN_MOD", "CHARACTER", "VARIABLE", "ASSIGN_MODIFY",
  "UNOP_INTRINSIC", "BINOP_INTRINSIC", "','", "ABOVE_COMMA", "'='", "'?'",
  "'|'", "'^'", "'&'", "NOTEQUAL", "EQUAL", "GEQ", "LEQ", "GREATERTHAN",
  "LESSTHAN", "RSH", "LSH", "'@'", "'+'", "'-'", "'*'", "'/'", "STARSTAR",
  "'%'", "UNARY", "'('", "ARROW", "BELOW_SIZE", "SIZE", "')'", "'~'",
  "':'", "'>'", "'<'", "'.'", "$accept", "start", "type_exp", "exp", "@1",
  "arglist", "subrange", "complexnum", "variable", "type", "ptype",
  "array_mod", "range", "subrange2", "star_colon", "colon_star",
  "star_colon_star", "signed_int", "typebase", "name", "name_not_typename", 0
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
     285,   286,   287,   288,   289,   290,   291,    44,   292,    61,
      63,   124,    94,    38,   293,   294,   295,   296,   297,   298,
     299,   300,    64,    43,    45,    42,    47,   301,    37,   302,
      40,   303,   304,   305,    41,   126,    58,    62,    60,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    70,    71,    71,    72,    73,    73,    73,    73,    73,
      73,    73,    74,    73,    73,    73,    75,    75,    75,    75,
      75,    76,    76,    76,    76,    77,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    78,    79,    80,    80,    80,    80,    81,
      82,    82,    83,    83,    83,    83,    83,    84,    84,    85,
      85,    86,    86,    86,    86,    86,    87,    87,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    89,    89,    89,    90
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     3,     2,     2,     2,     2,
       2,     2,     0,     5,     4,     6,     0,     1,     1,     3,
       3,     3,     2,     2,     1,     3,     3,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     4,     1,
       1,     3,     3,     1,     1,     1,     3,     2,     4,     3,
       3,     1,     3,     2,     2,     1,     1,     1,     2,     1,
       2,     1,     1,     2,     2,     3,     1,     2,     1,     1,
       3,     5,     1,     1,     1,     3,     5,     1,     3,     5,
       1,     1,     1,     1,     3,     5,     1,     1,     3,     5,
       1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    53,    55,    60,    59,   116,    88,    54,     0,    89,
      92,    93,   102,   101,   100,    97,   103,   106,   107,   110,
     111,   112,     0,    94,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     3,     2,    56,     4,    64,    65,    63,
       0,    11,     0,     0,     0,     0,     0,     0,     0,     0,
       9,     0,     0,     0,     0,     7,     8,     6,     0,     0,
       0,    10,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    12,     0,     0,     0,
       0,     0,     0,    67,     0,     0,    90,     0,    98,     0,
     104,     0,   108,     0,    95,     0,     0,     0,     5,    26,
       0,    48,    49,    50,    52,    51,    47,    46,    45,    38,
      37,    40,    39,    42,    41,    36,    35,    29,    33,    34,
      31,    32,    30,   113,   114,   115,    28,    16,    61,    43,
      44,    62,    66,    86,     0,    82,    77,     0,    71,     0,
      75,    76,    58,     0,     0,     0,     0,     0,    14,     0,
      25,    27,    82,    17,     0,    18,     0,    24,    68,    87,
      84,    78,    83,    69,     0,    74,    79,    73,    91,    99,
     105,   109,    96,     0,    79,    22,     0,    13,    23,    85,
      70,    80,    72,    15,    80,    21,    19,    20
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    32,    33,    57,   137,   164,   165,    59,    35,    36,
      37,    93,   147,   148,   149,   185,   150,   151,    38,   136,
      39
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -138
static const yytype_int16 yypact[] =
{
     222,  -138,  -138,  -138,  -138,  -138,  -138,  -138,   263,   -10,
    -138,  -138,  -138,  -138,  -138,    -8,    -7,  -138,  -138,    -6,
    -138,  -138,   304,     2,  -138,   -42,   -41,   304,   304,   304,
     222,   304,    21,  -138,   621,  -138,  -138,  -138,   -24,  -138,
     222,   264,   -14,    20,   -13,    39,     4,    41,     7,    47,
     264,    22,    56,   304,   304,   264,   264,   264,   411,    -4,
      36,   264,  -138,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,    25,  -138,    25,   304,   304,
      25,    52,    32,  -138,    37,    82,  -138,    93,  -138,    99,
    -138,   100,  -138,   101,  -138,   453,   495,   304,  -138,  -138,
     304,   683,   652,   621,   621,   621,   711,   739,   767,   795,
     795,   823,   823,   823,   823,    59,    59,   142,    78,    78,
     147,   147,   147,  -138,  -138,  -138,  -138,   158,  -138,   621,
     621,  -138,    45,  -138,   103,   -25,   -26,    44,    85,    -2,
    -138,    64,   386,    61,    68,    76,    77,    80,  -138,   304,
     621,   264,    88,   537,   -27,  -138,   304,  -138,  -138,  -138,
    -138,    94,  -138,  -138,    32,  -138,     1,  -138,  -138,  -138,
    -138,  -138,  -138,   579,   345,  -138,   158,  -138,   621,  -138,
    -138,  -138,  -138,  -138,   304,   621,   537,  -138
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -138,  -138,  -138,     0,  -138,  -138,   -36,  -138,  -138,   -16,
    -138,     9,  -138,   -19,  -135,     5,  -128,  -137,  -138,   -70,
    -138
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -82
static const yytype_int16 yytable[] =
{
      34,   143,   166,    42,   143,    44,    46,    48,    41,   167,
     186,   -81,   175,    91,    60,    51,   170,   138,    53,    54,
     141,    62,    50,    96,    94,    95,    97,    55,    56,   172,
      58,    61,   133,   134,   135,   143,    92,   187,   -81,   192,
      58,   171,    98,    99,   100,    43,   101,    45,    47,    49,
     102,   166,   144,   105,   106,   144,   191,    52,   167,   104,
     109,   103,   142,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   153,   144,   145,   139,   140,
      65,     1,     2,     3,     4,     5,   154,     7,   146,     8,
     110,   152,   155,   156,   157,    92,   169,   160,   173,    65,
     161,    79,    80,    81,    82,    83,    84,    85,    22,    86,
      87,    24,   174,    25,    26,   178,    88,    89,    90,   170,
     176,    27,   179,    82,    83,    84,    85,   163,    86,    87,
     180,   181,    28,    29,   182,    88,    89,    90,    30,   189,
     197,   168,   161,    31,   171,   190,   177,     0,     0,   183,
       0,     1,     2,     3,     4,     5,   188,     7,     0,     8,
       0,     0,     0,    65,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,   195,     0,   196,     0,    22,     0,
       0,    24,     0,    25,    26,    80,    81,    82,    83,    84,
      85,    27,    86,    87,    84,    85,     0,    86,    87,    88,
      89,    90,    28,   162,    88,    89,    90,     0,    30,     0,
       0,     0,     0,    31,   146,     1,     2,     3,     4,     5,
       6,     7,     0,     8,     0,     0,     0,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
       0,     0,    22,     0,    23,    24,     0,    25,    26,     0,
       0,     0,     0,     0,     0,    27,     1,     2,     3,     4,
       5,     0,     7,     0,     8,     0,    28,    29,     0,     0,
       0,     0,    30,     0,     0,     0,     0,    31,     0,     0,
       0,     0,     0,    22,     0,    65,    24,     0,    25,    26,
       0,     0,     0,     0,     0,     0,    27,     1,     2,     3,
       4,     5,     0,     7,     0,     8,     0,    28,    29,     0,
       0,     0,     0,    40,    86,    87,     0,     0,    31,     0,
       0,    88,    89,    90,    22,     0,     0,    24,     0,    25,
      26,     0,     0,     0,     0,     0,     0,    27,     1,     2,
       3,     4,     5,     0,     7,     0,     8,     0,    28,    29,
       0,     0,     0,     0,    30,     0,     0,     0,     0,    31,
       0,     0,     0,     0,     0,    22,     0,     0,    24,     0,
      25,    26,     0,     0,     0,     0,     0,     0,    27,     1,
       2,     3,     4,     5,     0,     7,     0,     8,     0,    28,
     194,     0,     0,     0,     0,    30,     0,     0,     0,     0,
      31,     0,     0,     0,     0,     0,    22,     0,     0,    24,
       0,    25,    26,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,     0,    65,     0,     0,    66,    30,     0,   107,     0,
      67,    31,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
       0,    86,    87,     0,     0,   108,     0,     0,    88,    89,
      90,    63,    64,     0,    65,     0,     0,    66,     0,     0,
       0,     0,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,     0,    86,    87,     0,     0,   158,     0,     0,
      88,    89,    90,    63,    64,     0,    65,     0,     0,    66,
       0,     0,   159,     0,    67,     0,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,     0,    86,    87,     0,     0,     0,
       0,     0,    88,    89,    90,    63,    64,     0,    65,     0,
       0,    66,     0,     0,     0,     0,    67,     0,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,     0,    86,    87,     0,
       0,     0,     0,   184,    88,    89,    90,    63,    64,     0,
      65,     0,     0,    66,     0,     0,     0,     0,    67,     0,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,     0,    86,
      87,     0,     0,   193,     0,     0,    88,    89,    90,    63,
      64,     0,    65,     0,     0,    66,     0,     0,     0,     0,
      67,     0,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      63,    86,    87,    65,     0,     0,     0,     0,    88,    89,
      90,     0,     0,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,     0,    86,    87,    65,     0,     0,     0,     0,    88,
      89,    90,     0,     0,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    65,    86,    87,     0,     0,     0,     0,     0,
      88,    89,    90,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      65,    86,    87,     0,     0,     0,     0,     0,    88,    89,
      90,     0,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    65,    86,
      87,     0,     0,     0,     0,     0,    88,    89,    90,     0,
       0,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    65,    86,    87,     0,
       0,     0,     0,     0,    88,    89,    90,     0,     0,     0,
       0,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    65,    86,    87,     0,     0,     0,
       0,     0,    88,    89,    90,     0,     0,     0,     0,     0,
       0,     0,     0,    77,    78,    79,    80,    81,    82,    83,
      84,    85,     0,    86,    87,     0,     0,     0,     0,     0,
      88,    89,    90
};

static const yytype_int16 yycheck[] =
{
       0,     3,   137,    13,     3,    13,    13,    13,     8,   137,
      37,    37,   149,    37,    30,    13,    41,    87,    60,    60,
      90,     0,    22,     3,    40,    39,    39,    27,    28,    55,
      30,    31,     7,     8,     9,     3,    60,    64,    64,   176,
      40,    66,     3,    39,     3,    55,    39,    55,    55,    55,
       3,   186,    54,    53,    54,    54,    55,    55,   186,     3,
      64,    39,    10,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,     3,    54,    55,    88,    89,
      31,     3,     4,     5,     6,     7,     3,     9,    66,    11,
      64,    64,     3,     3,     3,    60,     3,   107,    64,    31,
     110,    52,    53,    54,    55,    56,    57,    58,    30,    60,
      61,    33,    37,    35,    36,    64,    67,    68,    69,    41,
      66,    43,    64,    55,    56,    57,    58,   137,    60,    61,
      64,    64,    54,    55,    64,    67,    68,    69,    60,    55,
     186,   142,   152,    65,    66,   174,   151,    -1,    -1,   159,
      -1,     3,     4,     5,     6,     7,   166,     9,    -1,    11,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,   184,    -1,   186,    -1,    30,    -1,
      -1,    33,    -1,    35,    36,    53,    54,    55,    56,    57,
      58,    43,    60,    61,    57,    58,    -1,    60,    61,    67,
      68,    69,    54,    55,    67,    68,    69,    -1,    60,    -1,
      -1,    -1,    -1,    65,    66,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    -1,    30,    -1,    32,    33,    -1,    35,    36,    -1,
      -1,    -1,    -1,    -1,    -1,    43,     3,     4,     5,     6,
       7,    -1,     9,    -1,    11,    -1,    54,    55,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    31,    33,    -1,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    43,     3,     4,     5,
       6,     7,    -1,     9,    -1,    11,    -1,    54,    55,    -1,
      -1,    -1,    -1,    60,    60,    61,    -1,    -1,    65,    -1,
      -1,    67,    68,    69,    30,    -1,    -1,    33,    -1,    35,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    43,     3,     4,
       5,     6,     7,    -1,     9,    -1,    11,    -1,    54,    55,
      -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    -1,
      35,    36,    -1,    -1,    -1,    -1,    -1,    -1,    43,     3,
       4,     5,     6,     7,    -1,     9,    -1,    11,    -1,    54,
      55,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,
      -1,    35,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      29,    -1,    31,    -1,    -1,    34,    60,    -1,    37,    -1,
      39,    65,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      -1,    60,    61,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    28,    29,    -1,    31,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    -1,    60,    61,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    28,    29,    -1,    31,    -1,    -1,    34,
      -1,    -1,    37,    -1,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    28,    29,    -1,    31,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    -1,    60,    61,    -1,
      -1,    -1,    -1,    66,    67,    68,    69,    28,    29,    -1,
      31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    28,
      29,    -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      28,    60,    61,    31,    -1,    -1,    -1,    -1,    67,    68,
      69,    -1,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    -1,    60,    61,    31,    -1,    -1,    -1,    -1,    67,
      68,    69,    -1,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    31,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      31,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    -1,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    31,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    -1,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    31,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    -1,    -1,    -1,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    31,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    11,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    30,    32,    33,    35,    36,    43,    54,    55,
      60,    65,    71,    72,    73,    78,    79,    80,    88,    90,
      60,    73,    13,    55,    13,    55,    13,    55,    13,    55,
      73,    13,    55,    60,    60,    73,    73,    73,    73,    77,
      79,    73,     0,    28,    29,    31,    34,    39,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    60,    61,    67,    68,
      69,    37,    60,    81,    79,    39,     3,    39,     3,    39,
       3,    39,     3,    39,     3,    73,    73,    37,    64,    64,
      64,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,     7,     8,     9,    89,    74,    89,    73,
      73,    89,    10,     3,    54,    55,    66,    82,    83,    84,
      86,    87,    64,     3,     3,     3,     3,     3,    64,    37,
      73,    73,    55,    73,    75,    76,    84,    86,    81,     3,
      41,    66,    55,    64,    37,    87,    66,    85,    64,    64,
      64,    64,    64,    73,    66,    85,    37,    64,    73,    55,
      83,    55,    87,    64,    55,    73,    73,    76
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
#line 247 "f-exp.y"
    { write_exp_elt_opcode(OP_TYPE);
			  write_exp_elt_type((yyvsp[(1) - (1)].tval));
			  write_exp_elt_opcode(OP_TYPE); }
    break;

  case 5:
#line 253 "f-exp.y"
    { }
    break;

  case 6:
#line 258 "f-exp.y"
    { write_exp_elt_opcode (UNOP_IND); }
    break;

  case 7:
#line 262 "f-exp.y"
    { write_exp_elt_opcode (UNOP_ADDR); }
    break;

  case 8:
#line 266 "f-exp.y"
    { write_exp_elt_opcode (UNOP_NEG); }
    break;

  case 9:
#line 270 "f-exp.y"
    { write_exp_elt_opcode (UNOP_LOGICAL_NOT); }
    break;

  case 10:
#line 274 "f-exp.y"
    { write_exp_elt_opcode (UNOP_COMPLEMENT); }
    break;

  case 11:
#line 278 "f-exp.y"
    { write_exp_elt_opcode (UNOP_SIZEOF); }
    break;

  case 12:
#line 287 "f-exp.y"
    { start_arglist (); }
    break;

  case 13:
#line 289 "f-exp.y"
    { write_exp_elt_opcode (OP_F77_UNDETERMINED_ARGLIST);
			  write_exp_elt_longcst ((LONGEST) end_arglist ());
			  write_exp_elt_opcode (OP_F77_UNDETERMINED_ARGLIST); }
    break;

  case 14:
#line 295 "f-exp.y"
    { write_exp_elt_opcode ((yyvsp[(1) - (4)].opcode)); }
    break;

  case 15:
#line 299 "f-exp.y"
    { write_exp_elt_opcode ((yyvsp[(1) - (6)].opcode)); }
    break;

  case 17:
#line 305 "f-exp.y"
    { arglist_len = 1; }
    break;

  case 18:
#line 309 "f-exp.y"
    { arglist_len = 1; }
    break;

  case 19:
#line 313 "f-exp.y"
    { arglist_len++; }
    break;

  case 20:
#line 317 "f-exp.y"
    { arglist_len++; }
    break;

  case 21:
#line 323 "f-exp.y"
    { write_exp_elt_opcode (OP_F90_RANGE); 
			  write_exp_elt_longcst (NONE_BOUND_DEFAULT);
			  write_exp_elt_opcode (OP_F90_RANGE); }
    break;

  case 22:
#line 329 "f-exp.y"
    { write_exp_elt_opcode (OP_F90_RANGE);
			  write_exp_elt_longcst (HIGH_BOUND_DEFAULT);
			  write_exp_elt_opcode (OP_F90_RANGE); }
    break;

  case 23:
#line 335 "f-exp.y"
    { write_exp_elt_opcode (OP_F90_RANGE);
			  write_exp_elt_longcst (LOW_BOUND_DEFAULT);
			  write_exp_elt_opcode (OP_F90_RANGE); }
    break;

  case 24:
#line 341 "f-exp.y"
    { write_exp_elt_opcode (OP_F90_RANGE);
			  write_exp_elt_longcst (BOTH_BOUND_DEFAULT);
			  write_exp_elt_opcode (OP_F90_RANGE); }
    break;

  case 25:
#line 347 "f-exp.y"
    { }
    break;

  case 26:
#line 351 "f-exp.y"
    { write_exp_elt_opcode(OP_COMPLEX);
			  write_exp_elt_type (parse_f_type->builtin_complex_s16);
                	  write_exp_elt_opcode(OP_COMPLEX); }
    break;

  case 27:
#line 357 "f-exp.y"
    { write_exp_elt_opcode (UNOP_CAST);
			  write_exp_elt_type ((yyvsp[(2) - (4)].tval));
			  write_exp_elt_opcode (UNOP_CAST); }
    break;

  case 28:
#line 363 "f-exp.y"
    { write_exp_elt_opcode (STRUCTOP_STRUCT);
                          write_exp_string ((yyvsp[(3) - (3)].sval));
                          write_exp_elt_opcode (STRUCTOP_STRUCT); }
    break;

  case 29:
#line 371 "f-exp.y"
    { write_exp_elt_opcode (BINOP_REPEAT); }
    break;

  case 30:
#line 375 "f-exp.y"
    { write_exp_elt_opcode (BINOP_EXP); }
    break;

  case 31:
#line 379 "f-exp.y"
    { write_exp_elt_opcode (BINOP_MUL); }
    break;

  case 32:
#line 383 "f-exp.y"
    { write_exp_elt_opcode (BINOP_DIV); }
    break;

  case 33:
#line 387 "f-exp.y"
    { write_exp_elt_opcode (BINOP_ADD); }
    break;

  case 34:
#line 391 "f-exp.y"
    { write_exp_elt_opcode (BINOP_SUB); }
    break;

  case 35:
#line 395 "f-exp.y"
    { write_exp_elt_opcode (BINOP_LSH); }
    break;

  case 36:
#line 399 "f-exp.y"
    { write_exp_elt_opcode (BINOP_RSH); }
    break;

  case 37:
#line 403 "f-exp.y"
    { write_exp_elt_opcode (BINOP_EQUAL); }
    break;

  case 38:
#line 407 "f-exp.y"
    { write_exp_elt_opcode (BINOP_NOTEQUAL); }
    break;

  case 39:
#line 411 "f-exp.y"
    { write_exp_elt_opcode (BINOP_LEQ); }
    break;

  case 40:
#line 415 "f-exp.y"
    { write_exp_elt_opcode (BINOP_GEQ); }
    break;

  case 41:
#line 419 "f-exp.y"
    { write_exp_elt_opcode (BINOP_LESS); }
    break;

  case 42:
#line 423 "f-exp.y"
    { write_exp_elt_opcode (BINOP_GTR); }
    break;

  case 43:
#line 427 "f-exp.y"
    { write_exp_elt_opcode (BINOP_GTR); }
    break;

  case 44:
#line 431 "f-exp.y"
    { write_exp_elt_opcode (BINOP_LESS); }
    break;

  case 45:
#line 435 "f-exp.y"
    { write_exp_elt_opcode (BINOP_BITWISE_AND); }
    break;

  case 46:
#line 439 "f-exp.y"
    { write_exp_elt_opcode (BINOP_BITWISE_XOR); }
    break;

  case 47:
#line 443 "f-exp.y"
    { write_exp_elt_opcode (BINOP_BITWISE_IOR); }
    break;

  case 48:
#line 447 "f-exp.y"
    { write_exp_elt_opcode (BINOP_LOGICAL_AND); }
    break;

  case 49:
#line 451 "f-exp.y"
    { write_exp_elt_opcode (BINOP_LOGICAL_OR); }
    break;

  case 50:
#line 455 "f-exp.y"
    { write_exp_elt_opcode (BINOP_REM); }
    break;

  case 51:
#line 459 "f-exp.y"
    { write_exp_elt_opcode (BINOP_ASSIGN); }
    break;

  case 52:
#line 463 "f-exp.y"
    { write_exp_elt_opcode (BINOP_ASSIGN_MODIFY);
			  write_exp_elt_opcode ((yyvsp[(2) - (3)].opcode));
			  write_exp_elt_opcode (BINOP_ASSIGN_MODIFY); }
    break;

  case 53:
#line 469 "f-exp.y"
    { write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type ((yyvsp[(1) - (1)].typed_val).type);
			  write_exp_elt_longcst ((LONGEST)((yyvsp[(1) - (1)].typed_val).val));
			  write_exp_elt_opcode (OP_LONG); }
    break;

  case 54:
#line 476 "f-exp.y"
    { YYSTYPE val;
			  parse_number ((yyvsp[(1) - (1)].ssym).stoken.ptr, (yyvsp[(1) - (1)].ssym).stoken.length, 0, &val);
			  write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (val.typed_val.type);
			  write_exp_elt_longcst ((LONGEST)val.typed_val.val);
			  write_exp_elt_opcode (OP_LONG); }
    break;

  case 55:
#line 485 "f-exp.y"
    { write_exp_elt_opcode (OP_DOUBLE);
			  write_exp_elt_type (parse_f_type->builtin_real_s8);
			  write_exp_elt_dblcst ((yyvsp[(1) - (1)].dval));
			  write_exp_elt_opcode (OP_DOUBLE); }
    break;

  case 58:
#line 498 "f-exp.y"
    { write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (parse_f_type->builtin_integer);
			  CHECK_TYPEDEF ((yyvsp[(3) - (4)].tval));
			  write_exp_elt_longcst ((LONGEST) TYPE_LENGTH ((yyvsp[(3) - (4)].tval)));
			  write_exp_elt_opcode (OP_LONG); }
    break;

  case 59:
#line 506 "f-exp.y"
    { write_exp_elt_opcode (OP_BOOL);
			  write_exp_elt_longcst ((LONGEST) (yyvsp[(1) - (1)].lval));
			  write_exp_elt_opcode (OP_BOOL);
			}
    break;

  case 60:
#line 513 "f-exp.y"
    {
			  write_exp_elt_opcode (OP_STRING);
			  write_exp_string ((yyvsp[(1) - (1)].sval));
			  write_exp_elt_opcode (OP_STRING);
			}
    break;

  case 61:
#line 521 "f-exp.y"
    { write_exp_elt_opcode (STRUCTOP_PTR);
			  write_exp_string ((yyvsp[(3) - (3)].sval));
			  write_exp_elt_opcode (STRUCTOP_PTR); }
    break;

  case 62:
#line 527 "f-exp.y"
    { write_exp_elt_opcode (STRUCTOP_STRUCT);
			  write_exp_string ((yyvsp[(3) - (3)].sval));
			  write_exp_elt_opcode (STRUCTOP_STRUCT); }
    break;

  case 63:
#line 533 "f-exp.y"
    { struct symbol *sym = (yyvsp[(1) - (1)].ssym).sym;

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
			      break;
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

  case 66:
#line 576 "f-exp.y"
    { (yyval.tval) = lookup_pointer_type ((yyvsp[(1) - (3)].tval)); }
    break;

  case 67:
#line 578 "f-exp.y"
    { (yyval.tval) = follow_f_types ((yyvsp[(1) - (2)].tval)); }
    break;

  case 68:
#line 580 "f-exp.y"
    { (yyval.tval) = lookup_pointer_type (follow_f_types ((yyvsp[(1) - (4)].tval))); }
    break;

  case 72:
#line 591 "f-exp.y"
    {
			  push_type_int ((yyvsp[(1) - (3)].lval));
			  push_type_int ((yyvsp[(3) - (3)].lval));
			  push_type (tp_array); }
    break;

  case 73:
#line 598 "f-exp.y"
    {
			  push_type_int ((yyvsp[(1) - (2)].lval));
			  push_type_int ((yyvsp[(1) - (2)].lval) - 1);
			  push_type (tp_array); }
    break;

  case 74:
#line 605 "f-exp.y"
    { push_type_int (1);
			  push_type_int ((yyvsp[(2) - (2)].lval));
			  push_type (tp_array); }
    break;

  case 75:
#line 611 "f-exp.y"
    { push_type_int (1);
			  push_type_int (0);
			  push_type (tp_array); }
    break;

  case 76:
#line 617 "f-exp.y"
    { push_type_int (1);
			  push_type_int ((yyvsp[(1) - (1)].lval));
			  push_type (tp_array); }
    break;

  case 86:
#line 638 "f-exp.y"
    { (yyval.lval) = (yyvsp[(1) - (1)].typed_val).val; }
    break;

  case 87:
#line 640 "f-exp.y"
    { (yyval.lval) = -(yyvsp[(2) - (2)].typed_val).val; }
    break;

  case 88:
#line 645 "f-exp.y"
    { (yyval.tval) = (yyvsp[(1) - (1)].tsym).type; }
    break;

  case 89:
#line 647 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_integer; }
    break;

  case 90:
#line 649 "f-exp.y"
    { if ((yyvsp[(3) - (3)].typed_val).val == 2)
			  	(yyval.tval) = parse_f_type->builtin_integer_s2;
			  else if ((yyvsp[(3) - (3)].typed_val).val == 4)
			  	(yyval.tval) = parse_f_type->builtin_integer;
			  else if ((yyvsp[(3) - (3)].typed_val).val == 8)
				(yyval.tval) = parse_f_type->builtin_integer_s8; }
    break;

  case 91:
#line 656 "f-exp.y"
    { if ((yyvsp[(4) - (5)].typed_val).val == 2)
			  	(yyval.tval) = parse_f_type->builtin_integer_s2;
			  else if ((yyvsp[(4) - (5)].typed_val).val == 4)
			  	(yyval.tval) = parse_f_type->builtin_integer;
			  else if ((yyvsp[(4) - (5)].typed_val).val == 8)
				(yyval.tval) = parse_f_type->builtin_integer_s8; }
    break;

  case 92:
#line 663 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_integer_s2; }
    break;

  case 93:
#line 665 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_integer_s8; }
    break;

  case 94:
#line 667 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_character; }
    break;

  case 95:
#line 669 "f-exp.y"
    { if ((yyvsp[(3) - (3)].typed_val).val == 1) (yyval.tval) = parse_f_type->builtin_character; }
    break;

  case 96:
#line 671 "f-exp.y"
    { if ((yyvsp[(4) - (5)].typed_val).val == 1) (yyval.tval) = parse_f_type->builtin_character; }
    break;

  case 97:
#line 673 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_logical;}
    break;

  case 98:
#line 675 "f-exp.y"
    { if ((yyvsp[(3) - (3)].typed_val).val == 1)
				(yyval.tval) = parse_f_type->builtin_logical_s1;
			  else if ((yyvsp[(3) - (3)].typed_val).val == 2)
				(yyval.tval) = parse_f_type->builtin_logical_s2;
			  else if ((yyvsp[(3) - (3)].typed_val).val == 4)
				(yyval.tval) = parse_f_type->builtin_logical;
			  else if ((yyvsp[(3) - (3)].typed_val).val == 8)
				(yyval.tval) = parse_f_type->builtin_logical_s8; }
    break;

  case 99:
#line 684 "f-exp.y"
    { if ((yyvsp[(4) - (5)].typed_val).val == 1)
				(yyval.tval) = parse_f_type->builtin_logical_s1;
			  else if ((yyvsp[(4) - (5)].typed_val).val == 2)
				(yyval.tval) = parse_f_type->builtin_logical_s2;
			  else if ((yyvsp[(4) - (5)].typed_val).val == 4)
				(yyval.tval) = parse_f_type->builtin_logical;
			  else if ((yyvsp[(4) - (5)].typed_val).val == 8)
				(yyval.tval) = parse_f_type->builtin_logical_s8; }
    break;

  case 100:
#line 693 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_logical_s8;}
    break;

  case 101:
#line 695 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_logical_s2;}
    break;

  case 102:
#line 697 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_logical_s1;}
    break;

  case 103:
#line 699 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_real;}
    break;

  case 104:
#line 701 "f-exp.y"
    { if ((yyvsp[(3) - (3)].typed_val).val == 4)
				(yyval.tval) = parse_f_type->builtin_real;
			  else if ((yyvsp[(3) - (3)].typed_val).val == 8)
				(yyval.tval) = parse_f_type->builtin_real_s8;
			  else if ((yyvsp[(3) - (3)].typed_val).val == 16)
				(yyval.tval) = parse_f_type->builtin_real_s16; }
    break;

  case 105:
#line 708 "f-exp.y"
    { if ((yyvsp[(4) - (5)].typed_val).val == 4)
				(yyval.tval) = parse_f_type->builtin_real;
			  else if ((yyvsp[(4) - (5)].typed_val).val == 8)
				(yyval.tval) = parse_f_type->builtin_real_s8;
			  else if ((yyvsp[(4) - (5)].typed_val).val == 16)
				(yyval.tval) = parse_f_type->builtin_real_s16; }
    break;

  case 106:
#line 715 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_real_s8;}
    break;

  case 107:
#line 717 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_real_s16; }
    break;

  case 108:
#line 719 "f-exp.y"
    { if ((yyvsp[(3) - (3)].typed_val).val == 8)
				(yyval.tval) = parse_f_type->builtin_complex_s8;
			  else if ((yyvsp[(3) - (3)].typed_val).val == 16)
				(yyval.tval) = parse_f_type->builtin_complex_s16;
			  else if ((yyvsp[(3) - (3)].typed_val).val == 32)
				(yyval.tval) = parse_f_type->builtin_complex_s32; }
    break;

  case 109:
#line 726 "f-exp.y"
    { if ((yyvsp[(4) - (5)].typed_val).val == 8)
				(yyval.tval) = parse_f_type->builtin_complex_s8;
			  else if ((yyvsp[(4) - (5)].typed_val).val == 16)
				(yyval.tval) = parse_f_type->builtin_complex_s16;
			  else if ((yyvsp[(4) - (5)].typed_val).val == 32)
				(yyval.tval) = parse_f_type->builtin_complex_s32; }
    break;

  case 110:
#line 733 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_complex_s8;}
    break;

  case 111:
#line 735 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_complex_s16;}
    break;

  case 112:
#line 737 "f-exp.y"
    { (yyval.tval) = parse_f_type->builtin_complex_s32; }
    break;

  case 113:
#line 741 "f-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].ssym).stoken; }
    break;

  case 114:
#line 743 "f-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].tsym).stoken; }
    break;

  case 115:
#line 745 "f-exp.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].ssym).stoken; }
    break;


/* Line 1267 of yacc.c.  */
#line 2457 "f-exp.c.tmp"
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


#line 758 "f-exp.y"


/* Take care of parsing a number (anything that starts with a digit).
   Set yylval and return the token type; update lexptr.
   LEN is the number of characters in it.  */

/*** Needs some error checking for the float case ***/

static int
parse_number (p, len, parsed_float, putithere)
     char *p;
     int len;
     int parsed_float;
     YYSTYPE *putithere;
{
  LONGEST n = 0;
  LONGEST prevn = 0;
  int c;
  int base = input_radix;
  int unsigned_p = 0;
  int long_p = 0;
  ULONGEST high_bit;
  struct type *signed_type;
  struct type *unsigned_type;

  if (parsed_float)
    {
      /* It's a float since it contains a point or an exponent.  */
      /* [dD] is not understood as an exponent by atof, change it to 'e'.  */
      char *tmp, *tmp2;

      tmp = xstrdup (p);
      for (tmp2 = tmp; *tmp2; ++tmp2)
	if (*tmp2 == 'd' || *tmp2 == 'D')
	  *tmp2 = 'e';
      putithere->dval = atof (tmp);
      xfree (tmp);
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
      if (isupper (c))
	c = tolower (c);
      if (len == 0 && c == 'l')
	long_p = 1;
      else if (len == 0 && c == 'u')
	unsigned_p = 1;
      else
	{
	  int i;
	  if (c >= '0' && c <= '9')
	    i = c - '0';
	  else if (c >= 'a' && c <= 'f')
	    i = c - 'a' + 10;
	  else
	    return ERROR;	/* Char not a digit */
	  if (i >= base)
	    return ERROR;		/* Invalid digit in this base */
	  n *= base;
	  n += i;
	}
      /* Portably test for overflow (only works for nonzero values, so make
	 a second check for zero).  */
      if ((prevn >= n) && n != 0)
	unsigned_p=1;		/* Try something unsigned */
      /* If range checking enabled, portably test for unsigned overflow.  */
      if (RANGE_CHECK && n != 0)
	{
	  if ((unsigned_p && (unsigned)prevn >= (unsigned)n))
	    range_error("Overflow on numeric constant.");	 
	}
      prevn = n;
    }
  
  /* If the number is too big to be an int, or it's got an l suffix
     then it's a long.  Work out if this has to be a long by
     shifting right and and seeing if anything remains, and the
     target int size is different to the target long size.
     
     In the expression below, we could have tested
     (n >> gdbarch_int_bit (parse_gdbarch))
     to see if it was zero,
     but too many compilers warn about that, when ints and longs
     are the same size.  So we shift it twice, with fewer bits
     each time, for the same result.  */
  
  if ((gdbarch_int_bit (parse_gdbarch) != gdbarch_long_bit (parse_gdbarch)
       && ((n >> 2)
	   >> (gdbarch_int_bit (parse_gdbarch)-2))) /* Avoid shift warning */
      || long_p)
    {
      high_bit = ((ULONGEST)1) << (gdbarch_long_bit (parse_gdbarch)-1);
      unsigned_type = parse_type->builtin_unsigned_long;
      signed_type = parse_type->builtin_long;
    }
  else 
    {
      high_bit = ((ULONGEST)1) << (gdbarch_int_bit (parse_gdbarch)-1);
      unsigned_type = parse_type->builtin_unsigned_int;
      signed_type = parse_type->builtin_int;
    }    
  
  putithere->typed_val.val = n;
  
  /* If the high bit of the worked out type is set then this number
     has to be unsigned. */
  
  if (unsigned_p || (n & high_bit)) 
    putithere->typed_val.type = unsigned_type;
  else 
    putithere->typed_val.type = signed_type;
  
  return INT;
}

struct token
{
  char *operator;
  int token;
  enum exp_opcode opcode;
};

static const struct token dot_ops[] =
{
  { ".and.", BOOL_AND, BINOP_END },
  { ".AND.", BOOL_AND, BINOP_END },
  { ".or.", BOOL_OR, BINOP_END },
  { ".OR.", BOOL_OR, BINOP_END },
  { ".mod.", BIN_MOD, BINOP_END },
  { ".MOD.", BIN_MOD, BINOP_END },
  { "==", EQUAL, BINOP_END },
  { ".not.", BOOL_NOT, BINOP_END },
  { ".NOT.", BOOL_NOT, BINOP_END },
  { ".eq.", EQUAL, BINOP_END },
  { ".EQ.", EQUAL, BINOP_END },
  { ".eqv.", EQUAL, BINOP_END },
  { ".NEQV.", NOTEQUAL, BINOP_END },
  { ".neqv.", NOTEQUAL, BINOP_END },
  { ".EQV.", EQUAL, BINOP_END },
  { "!=", NOTEQUAL, BINOP_END },
  { ".ne.", NOTEQUAL, BINOP_END },
  { ".NE.", NOTEQUAL, BINOP_END },
  { ".le.", LEQ, BINOP_END },
  { ".LE.", LEQ, BINOP_END },
  { "<=", LEQ, BINOP_END },
  { ".ge.", GEQ, BINOP_END },
  { ".GE.", GEQ, BINOP_END },
  { ">=", GEQ, BINOP_END },
  { ".gt.", GREATERTHAN, BINOP_END },
  { ".GT.", GREATERTHAN, BINOP_END },
  { ".lt.", LESSTHAN, BINOP_END },
  { ".LT.", LESSTHAN, BINOP_END },
  { NULL, 0, 0 }
};

struct f77_boolean_val 
{
  char *name;
  int value;
}; 

static const struct f77_boolean_val boolean_values[]  = 
{
  { ".true.", 1 },
  { ".TRUE.", 1 },
  { ".false.", 0 },
  { ".FALSE.", 0 },
  { NULL, 0 }
};

static const struct token f77_keywords[] = 
{
  { "complex_16", COMPLEX_S16_KEYWORD, BINOP_END },
  { "complex_32", COMPLEX_S32_KEYWORD, BINOP_END },
  { "character", CHARACTER, BINOP_END },
  { "integer_2", INT_S2_KEYWORD, BINOP_END },
  { "logical_1", LOGICAL_S1_KEYWORD, BINOP_END },
  { "logical_2", LOGICAL_S2_KEYWORD, BINOP_END },
  { "logical_8", LOGICAL_S8_KEYWORD, BINOP_END },
  { "complex_8", COMPLEX_S8_KEYWORD, BINOP_END },
  { "integer", INT_KEYWORD, BINOP_END },
  { "integer_8", INT_S8_KEYWORD, BINOP_END },
  { "logical", LOGICAL_KEYWORD, BINOP_END },
  { "real_16", REAL_S16_KEYWORD, BINOP_END },
  { "complex", COMPLEX_S8_KEYWORD, BINOP_END },
  { "sizeof", SIZEOF, BINOP_END },
  { "real_8", REAL_S8_KEYWORD, BINOP_END },
  { "real", REAL_KEYWORD, BINOP_END },
  { "pointer", POINTER, BINOP_END },
  { NULL, 0, 0 }
}; 

static const struct token intrinsics[] =
  {
    {"ABS", UNOP_INTRINSIC, UNOP_FABS},
    {"AIMAG", UNOP_INTRINSIC, UNOP_CIMAG},
    {"CMPLX", BINOP_INTRINSIC, BINOP_CMPLX},
    {"REALPART", UNOP_INTRINSIC, UNOP_CREAL},
    {"ISINF", UNOP_INTRINSIC, UNOP_IEEE_IS_INF},
    {"IEEE_IS_INF", UNOP_INTRINSIC, UNOP_IEEE_IS_INF},
    {"ISFINITE", UNOP_INTRINSIC, UNOP_IEEE_IS_FINITE},
    {"IEEE_IS_FINITE", UNOP_INTRINSIC, UNOP_IEEE_IS_FINITE},
    {"ISNAN", UNOP_INTRINSIC, UNOP_IEEE_IS_NAN},
    {"IEEE_IS_NAN", UNOP_INTRINSIC, UNOP_IEEE_IS_NAN},
    {"ISNORMAL", UNOP_INTRINSIC, UNOP_IEEE_IS_NORMAL},
    {"IEEE_IS_NORMAL", UNOP_INTRINSIC, UNOP_IEEE_IS_NORMAL},
    {"CEILING", UNOP_INTRINSIC, UNOP_CEIL},
    {"FLOOR", UNOP_INTRINSIC, UNOP_FLOOR},
    {"MOD", BINOP_INTRINSIC, BINOP_FMOD},
    {"MODULO", BINOP_INTRINSIC, BINOP_MODULO},
  };

/* Implementation of a dynamically expandable buffer for processing input
   characters acquired through lexptr and building a value to return in
   yylval. Ripped off from ch-exp.y */ 

static char *tempbuf;		/* Current buffer contents */
static int tempbufsize;		/* Size of allocated buffer */
static int tempbufindex;	/* Current index into buffer */

#define GROWBY_MIN_SIZE 64	/* Minimum amount to grow buffer by */

#define CHECKBUF(size) \
  do { \
    if (tempbufindex + (size) >= tempbufsize) \
      { \
	growbuf_by_size (size); \
      } \
  } while (0);


/* Grow the static temp buffer if necessary, including allocating the first one
   on demand. */

static void
growbuf_by_size (count)
     int count;
{
  int growby;

  growby = max (count, GROWBY_MIN_SIZE);
  tempbufsize += growby;
  if (tempbuf == NULL)
    tempbuf = (char *) xmalloc (tempbufsize);
  else
    tempbuf = (char *) xrealloc (tempbuf, tempbufsize);
}

/* Blatantly ripped off from ch-exp.y. This routine recognizes F77 
   string-literals. 
   
   Recognize a string literal.  A string literal is a nonzero sequence
   of characters enclosed in matching single quotes, except that
   a single character inside single quotes is a character literal, which
   we reject as a string literal.  To embed the terminator character inside
   a string, it is simply doubled (I.E. 'this''is''one''string') */

static int
match_string_literal ()
{
  char *tokptr = lexptr;

  for (tempbufindex = 0, tokptr++; *tokptr != '\0'; tokptr++)
    {
      CHECKBUF (1);
      if (*tokptr == *lexptr)
	{
	  if (*(tokptr + 1) == *lexptr)
	    tokptr++;
	  else
	    break;
	}
      tempbuf[tempbufindex++] = *tokptr;
    }
  if (*tokptr == '\0'					/* no terminator */
      || tempbufindex == 0)				/* no string */
    return 0;
  else
    {
      tempbuf[tempbufindex] = '\0';
      yylval.sval.ptr = tempbuf;
      yylval.sval.length = tempbufindex;
      lexptr = ++tokptr;
      return STRING_LITERAL;
    }
}

static struct type *
follow_f_types (struct type *follow_type)
{
  /* This is where the interesting stuff happens.  */
  int done = 0;
  int lower_bound, upper_bound;
  int *bounds, index;
  enum type_pieces type;
  struct type *range_type;

  while (!done)
    switch (pop_type ())
      {
      case tp_end:
		done = 1;
		break;
      case tp_pointer:
		follow_type = lookup_pointer_type (follow_type);
		break;
      case tp_array:
		bounds = xmalloc (80 * sizeof (int));
		index = 0;
		do {
		  upper_bound = pop_type_int ();
		  lower_bound = pop_type_int ();
		  bounds[index++] = upper_bound;
		  bounds[index++] = lower_bound;
		} while ((type = pop_type()) == tp_array);
		push_type (type);
		while (index > 0)
		  {
			lower_bound = bounds[--index];
			upper_bound = bounds[--index];
			range_type =
				create_range_type ((struct type *) NULL,
				  parse_f_type->builtin_integer, lower_bound,
				  upper_bound);
			follow_type =
				create_array_type ((struct type *) NULL,
				  follow_type, range_type);
		  }
		xfree (bounds);
		break;
	  }
  return follow_type;
}

/* Read one token, getting characters through lexptr.  */

static int
yylex ()
{
  int c;
  int namelen;
  unsigned int i,token;
  char *tokstart;
  
 retry:
 
  prev_lexptr = lexptr;
 
  tokstart = lexptr;
  
  /* First of all, let us make sure we are not dealing with the 
     special tokens .true. and .false. which evaluate to 1 and 0.  */
  
  if (*lexptr == '.')
    { 
      for (i = 0; boolean_values[i].name != NULL; i++)
	{
	  if (strncmp (tokstart, boolean_values[i].name,
		       strlen (boolean_values[i].name)) == 0)
	    {
	      lexptr += strlen (boolean_values[i].name); 
	      yylval.lval = boolean_values[i].value; 
	      return BOOLEAN_LITERAL;
	    }
	}
    }
  
  /* See if it is a special .foo. operator.  */
  
  for (i = 0; dot_ops[i].operator != NULL; i++)
    if (strncmp (tokstart, dot_ops[i].operator, strlen (dot_ops[i].operator)) == 0)
      {
	lexptr += strlen (dot_ops[i].operator);
	yylval.opcode = dot_ops[i].opcode;
	return dot_ops[i].token;
      }
  
  /* See if it is an exponentiation operator.  */

  if (strncmp (tokstart, "**", 2) == 0)
    {
      lexptr += 2;
      yylval.opcode = BINOP_EXP;
      return STARSTAR;
    }

  switch (c = *tokstart)
    {
    case 0:
      return 0;
      
    case ' ':
    case '\t':
    case '\n':
      lexptr++;
      goto retry;
      
    case '\'':
      token = match_string_literal ();
      if (token != 0)
	return (token);
      break;
      
    case '(':
      paren_depth++;
      lexptr++;
      if (strncasecmp(lexptr, "KIND", 4) == 0)
	{
	  /* yacc only supports one lookahead symbols but two are needed to
	   * decide whether a left paranthesis is starting a size/kind or
	   * array dimensions. */
	  lexptr += 4;
	  yylval.opcode = BINOP_END;
	  return KIND;
	}
      return c;
      
    case ')':
      if (paren_depth == 0)
	return 0;
      paren_depth--;
      lexptr++;
      return c;
      
    case ',':
      if (comma_terminates && paren_depth == 0)
	return 0;
      lexptr++;
      return c;
      
    case '.':
      /* Might be a floating point number.  */
      if (lexptr[1] < '0' || lexptr[1] > '9')
	goto symbol;		/* Nope, must be a symbol. */
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
	int got_dot = 0, got_e = 0, got_d = 0, toktype;
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
	    if (!hex && !got_e && (*p == 'e' || *p == 'E'))
	      got_dot = got_e = 1;
	    else if (!hex && !got_d && (*p == 'd' || *p == 'D'))
	      got_dot = got_d = 1;
	    else if (!hex && !got_dot && *p == '.')
	      got_dot = 1;
	    else if (((got_e && (p[-1] == 'e' || p[-1] == 'E'))
		     || (got_d && (p[-1] == 'd' || p[-1] == 'D')))
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
	toktype = parse_number (tokstart, p - tokstart, got_dot|got_e|got_d,
				&yylval);
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
      
    case '-':
      if (tokstart[1] == '>'){
	lexptr+=2;
	return ARROW;
      }
    case '+':
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
    case '[':
    case ']':
    case '?':
      lexptr++;
      return c;
    case ':':
      if (tokstart[1] == ':'){
	lexptr+=2;
	return COLONCOLON;
      }
    case '=':
    case '{':
    case '}':
    symbol:
      lexptr++;
      return c;
    }
  
  if (!(c == '_' || c == '$' || c ==':'
	|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
    /* We must have come across a bad character (e.g. ';').  */
    error ("Invalid character '%c' in expression.", c);
  
  namelen = 0;
  for (c = tokstart[namelen];
       (c == '_' || c == '$' || c == ':' || (c >= '0' && c <= '9')
	|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); 
       c = tokstart[++namelen]);
  
  /* The token "if" terminates the expression and is NOT 
     removed from the input stream.  */
  
  if (namelen == 2 && tokstart[0] == 'i' && tokstart[1] == 'f')
    return 0;

  /* For the same reason (breakpoint conditions), "thread N"
     terminates the expression.  "thread" could be an identifier, but
     an identifier is never followed by a number without intervening
     punctuation.  "task" is similar.  Handle abbreviations of these,
     similarly to breakpoint.c:find_condition_and_thread.  */
  if (namelen >= 1
      && (strncmp (tokstart, "thread", namelen) == 0
	  || strncmp (tokstart, "task", namelen) == 0
	  || strncmp (tokstart, "inf", namelen) == 0)
      && (tokstart[namelen] == ' ' || tokstart[namelen] == '\t'))
    {
      char *p = tokstart + namelen + 1;
      while (*p == ' ' || *p == '\t')
	p++;
      if (*p >= '0' && *p <= '9')
	return 0;
    }
  
  lexptr += namelen;
  
  /* Catch specific keywords.  */
  
  for (i = 0; f77_keywords[i].operator != NULL; i++)
    if (strncasecmp (tokstart, f77_keywords[i].operator,
		 strlen(f77_keywords[i].operator)) == 0
	&& strlen(f77_keywords[i].operator) == namelen)
      {
	/* 	lexptr += strlen(f77_keywords[i].operator); */ 
	yylval.opcode = f77_keywords[i].opcode;
	return f77_keywords[i].token;
      }

  for (i = 0; i < sizeof intrinsics / sizeof intrinsics[0]; i++)
    if (strncasecmp (tokstart, intrinsics[i].operator, strlen(intrinsics[i].operator)) == 0
 	&& strlen(intrinsics[i].operator) == namelen)
      {
        yylval.opcode = intrinsics[i].opcode;
        return intrinsics[i].token;
      }

  yylval.sval.ptr = tokstart;
  yylval.sval.length = namelen;
  
  if (*tokstart == '$')
    {
      write_dollar_variable (yylval.sval);
      return VARIABLE;
    }

  /* Use token-type TYPENAME for symbols that happen to be defined
     currently as names of types; NAME for other symbols.
     The caller is not constrained to care about the distinction.  */
  {
    char *tmp = copy_name (yylval.sval);
    struct symbol *sym;
    int is_a_field_of_this = 0;
    int hextype;
    
    sym = lookup_symbol (tmp, expression_context_block,
			 VAR_DOMAIN,
			 parse_language->la_language == language_cplus
			 ? &is_a_field_of_this : NULL);
    if (sym && SYMBOL_CLASS (sym) == LOC_TYPEDEF)
      {
	yylval.tsym.type = SYMBOL_TYPE (sym);
	return TYPENAME;
      }
    yylval.tsym.type
      = language_lookup_primitive_type_by_name (parse_language,
						parse_gdbarch, tmp);
    if (yylval.tsym.type != NULL)
      return TYPENAME;
    
    /* Input names that aren't symbols but ARE valid hex numbers,
       when the input radix permits them, can be names or numbers
       depending on the parse.  Note we support radixes > 16 here.  */
    if (!sym
	&& ((tokstart[0] >= 'a' && tokstart[0] < 'a' + input_radix - 10)
	    || (tokstart[0] >= 'A' && tokstart[0] < 'A' + input_radix - 10)))
      {
 	YYSTYPE newlval;	/* Its value is ignored.  */
	hextype = parse_number (tokstart, namelen, 0, &newlval);
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
    return NAME;
  }
}

void
yyerror (msg)
     char *msg;
{
  if (prev_lexptr)
    lexptr = prev_lexptr;

  error ("A %s in expression, near `%s'.", (msg ? msg : "error"), lexptr);
}

