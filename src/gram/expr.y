/*
** Expression parser.
**
** Copyright 2012-2024 Hewlett Packard Enterprise Development LP.
**
*/

%define api.pure

%{
#include "config.h"

#include <ctype.h>
#include <cstdlib>
#include <memory>
#include <stdexcept>

#include "gram.h"
#define YYERROR_VERBOSE
#define YYDEBUG 1

static const char* ExprStr = nullptr;
static char* ParseErr = nullptr;
static gram::Expression* ParseResult = nullptr;
%}

%union
{
    int v_int;
    double v_float;
    std::string* v_str;
    std::string* v_sym;
    std::string* v_dsym;
    gram::Expression* v_expr;
}

%{
int	expr_error(const char *);
#ifdef notdef
int	expr_verror(const char *, ...);
#endif /* notdef */
int	expr_lex(YYSTYPE *);
#ifdef DEBUG
/*
** You can uncomment the following variable to turn on debug output from the
** parser. However this is *VERY* verbose.
*/
//int     yydebug=1;
#endif
%}

%token <v_int>		INT
%token <v_float>    FLOAT
%token <v_str>      STRING
%token <v_sym>      SYM
%token <v_dsym>     DSYM

%token	COND		"?"
%token	COLON		":"
%token	COLONCOLON	"::"
%token	DOTDOT		".."
%token	LT			"<"
%token	GT			">"
%token	LEQ			"<="
%token	GEQ			">="
%token	EQUAL		"="
%token	DBLEQ		"=="
%token	NOTEQUAL	"!="
%token  NOT			"!"
%token	ANDAND		"&&"
%token	OROR		"||"
%token	AND      	"&"
%token	OR			"|"
%token	PLUS		"+"
%token	MINUS		"-"
%token	STAR      	"*"
%token	DIV			"/"
// TODO: %token	REM						%CT_MOD%
%token	PERC		"%"
%token	LPAREN		"("
%token	RPAREN		")"
%token	LBRACE		"{"
%token	RBRACE		"}"
%token	LBRCK		"["
%token	RBRCK		"]"
%token	COMMA		","
%token	AT			"@"
%token  ATLOCATION
%token	DOT			"."
%token	ARROW		"->"
%token	PLUSPLUS	"++"
%token	MINUSMINUS	"--"

%type <v_expr>	start expr 

 // todo --, ++, : for thread scope
%left		COMMA
%right		EQUAL
%right		COND
%left		OROR
%left		ANDAND
%left		OR
%left		AND
%left		DBLEQ NOTEQUAL
%left		LT GT LEQ GEQ
%left		PLUS MINUS 
%left		STAR DIV
%nonassoc   ATLOCATION
%nonassoc	DOTDOT
%left       CAST
%right		UNARY
%nonassoc	COLONCOLON
%nonassoc   AT
%right		ARROW DOT PERC LPAREN LBRCK 
%right		LBRACE

%%

start : expr { $$ = ParseResult = $1; }

expr : INT { $$ = new gram::Int{$1}; }

expr : FLOAT { $$ = new gram::Float{$1}; }

expr : STRING { $$ = new gram::String{*$1}; delete $1; }

expr : SYM { $$ = new gram::Sym{*$1}; delete $1; }

expr : DSYM { $$ = new gram::DSym(*$1); delete $1; }

expr : expr "::" expr { $$ = new gram::ScopedVal($1, $3); }

expr : expr "+" expr { $$ = new gram::Binop($1, "+", $3); }

expr : expr "<" expr { $$ = new gram::Binop($1, "<", $3); }

expr : expr ">" expr { $$ = new gram::Binop($1, ">", $3); }

expr : expr "<=" expr { $$ = new gram::Binop($1, "<=", $3); }

expr : expr ">=" expr { $$ = new gram::Binop($1, ">=", $3); }

expr : expr "=" expr { $$ = new gram::Binop($1, "=", $3); }

expr : expr "==" expr { $$ = new gram::Binop($1, "==", $3); }

expr : expr "!=" expr { $$ = new gram::Binop($1, "!=", $3); }

expr : expr "&&" expr { $$ = new gram::Binop($1, "&&", $3); }

expr : expr "||" expr { $$ = new gram::Binop($1, "||", $3); }

expr : expr "&" expr { $$ = new gram::Binop($1, "&", $3); }

expr : expr "|" expr { $$ = new gram::Binop($1, "|", $3); }

expr : expr "-" expr %prec PLUS { $$ = new gram::Binop($1, "-", $3); }

expr : expr "*" expr { $$ = new gram::Binop($1, "*", $3); }

expr : expr "/" expr { $$ = new gram::Binop($1, "/", $3); }

expr : expr "%" expr { $$ = new gram::Binop($1, "%", $3); }

expr : expr ".." expr { $$ = new gram::Range($1, $3); }

expr : ".." { $$ = new gram::Range(nullptr, nullptr); }

expr : expr ".." { $$ = new gram::Range($1, nullptr); }

expr : ".." expr { $$ = new gram::Range(nullptr, $2); }

expr : expr "." expr { $$ = new gram::Binop($1, ".", $3); }

expr : expr "->" SYM { $$ = new gram::Binop($1, "->", new gram::Sym{*$3}); delete $3; }

expr : expr "?" expr ":" expr %prec COND { $$ = new gram::Conditional($1, $3, $5); }

expr : expr "@" expr { $$ = new gram::Binop($1, "@", $3); }

expr : "!" expr %prec UNARY { $$ = new gram::UnaryOp("!", $2); } 

expr : "&" expr %prec UNARY { $$ = new gram::UnaryOp("&", $2); }

expr : "*" expr %prec UNARY { $$ = new gram::UnaryOp("*", $2); }

expr : "-" expr %prec UNARY { $$ = $2->negate(); }

expr: "--" expr %prec UNARY { $$ = new gram::UnaryOp("--", $2); }

expr: "++" expr %prec UNARY { $$ = new gram::UnaryOp("++", $2); }

expr : "(" expr ")" %prec UNARY { $$ = new gram::SubExpr($2); }

expr : expr "(" expr ")" { $$ = new gram::FuncCall($1, $3); }

expr : expr "(" ")" { $$ = new gram::FuncCall($1, nullptr); }

expr : DSYM "{" expr "}" { $$ = new gram::DSymArray(*$1, $3); delete $1; }

expr : DSYM "{" "*" ":" expr "}" { auto pe = new gram::ThreadedProcsetExpr(nullptr, $5);
                                   $$ = new gram::DSymArray(*$1, pe); delete $1; }

expr : DSYM "{" expr ":" expr "}" { auto pe = new gram::ThreadedProcsetExpr($3, $5);
                                    $$ = new gram::DSymArray(*$1, pe); delete $1; }

expr : expr "," expr { $$ = new gram::List($1, $3); }

expr : expr "[" expr "]" { $$ = new gram::ArrayRef($1, $3); }

expr : expr ATLOCATION STRING ":" INT %prec ATLOCATION { $$ = new gram::AssertExpr($1, *$3, $5); delete $3; }

expr : "(" expr ")" expr %prec CAST { $$ = new gram::CastExpr($2->toString(), $4); delete $2; }

expr : "(" expr "*" ")" expr %prec CAST { $$ = new gram::CastExpr($2->toString()+"*", $5); delete $2; }

%%

inline bool isword(char c) { return isalnum(c) || c == '_'; }

static bool isLocation(const char* s) {
    while (*s && isspace(*s)) ++s;
    return *s == '"';
}

int
expr_lex(YYSTYPE *lv)
{
    auto& s = ExprStr;
    while (*s && isspace(*s)) ++s;
    if (!*s) return YYEOF;

    if (isdigit(*s)) {
        char* endi, *endf;
        int ival = strtol(s, &endi, 10);
        double dval = strtod(s, &endf);

        if (endf > s && endf > endi && !isword(*endf) && *endf != '.') {
            // *endf == '.' happens in ranges like: 1..2
            lv->v_float = dval;
            s = endf;
            return FLOAT;
        } else if (endi > s && (!*endi || !isword(*endi))) {
            lv->v_int = ival;
            s = endi;
            return INT;
        }
    }
    if (*s =='"') {
        auto e = s+1;
        for ( ; *e && *e != '"'; ++e) {
            if (*e == '\\') ++e;
        }
        if (*e == '"') {
            lv->v_str = new std::string(s+1, e);
            s = e+1;
            return STRING;
        }
    } else if (*s == '$') {
        auto end = s + 1;
        while (isword(*end)) end++;
        lv->v_dsym = new std::string(s, end);
        s = end;
        return DSYM;
    } else if (isword(*s)) {
        auto e = s+1;
        for ( ; *e && isword(*e); ++e) ;
        lv->v_sym = new std::string(s,e);
        s = e;
        return SYM;
    }

    auto c = *s++;

    switch (c) {
    case '?': return COND;
    case ':': if (*s == ':') { ++s; return COLONCOLON; } else { return COLON; }
    case '.': if (*s == '.') { ++s; return DOTDOT; } else { return DOT; }
    case '<': if (*s == '=') { ++s; return LEQ; } else { return LT; }
    case '>': if (*s == '=') { ++s; return GEQ; } else { return GT; }
    case '=': if (*s == '=') { ++s; return DBLEQ; } else { return EQUAL; }
    case '!': if (*s == '=') { ++s; return NOTEQUAL; } else { return NOT; }
    case '&': if (*s == '&') { ++s; return ANDAND; } else { return AND; }
    case '|': if (*s == '|') { ++s; return OROR; } else { return OR; }
    case '+': if (*s == '+') { ++s; return PLUSPLUS; } else { return PLUS; }
    case '-': if (*s == '>') {
            ++s; return ARROW;
        } if (*s == '-') {
            ++s; return MINUSMINUS;
        } else {
            return MINUS;
        }
    case '*': return STAR;
    case '/': return DIV;
    case '%': return PERC;
    case '(': return LPAREN;
    case ')': return RPAREN;
    case '{': return LBRACE;
    case '}': return RBRACE;
    case '[': return LBRCK;
    case ']': return RBRCK;
    case ',': return COMMA;
    case '@': return (isLocation(s) ? ATLOCATION : AT);
    default: --s; break;
    };

    fprintf(stderr, "unexpected character %c\n", c);
    return YYEOF;
}

int
expr_error(const char *err)
{
    fprintf(stderr, "%s", err);
    // bison might pass a stack-allocated address as err
    if (ParseErr != nullptr) ::free(ParseErr);
    ParseErr = ::strdup(err);
    return 0;
}

std::unique_ptr<gram::Expression>
ParseString(const std::string& str) {
    ExprStr = str.c_str();
    if (expr_parse()) {
        throw ParseError(ParseErr ? ParseErr : "parse failed");
    }

    return std::unique_ptr<gram::Expression>(ParseResult);
}


