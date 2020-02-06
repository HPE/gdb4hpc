/*********************************************************************************\
 * Command token routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "defs.h"
#include "error.h"
#include "gram.h"
#include "hash.h"

static htable_t *_token_table;
static htable_t *_type_specifier_table;
static htable_t *_type_identifier_table;
static htable_t *_symtab = (htable_t *)NULL;

// Prototypes
static ct_type convnum(ct_val *, const char **);

static void skipwhite(const char **s) {
    while (**s != '\0' && **s != '\n' && isspace((int)**s)) (*s)++;
}

/*
** Returns true if c is a valid identifier
** character.
*/
int isid(char c) { return (int)(c == '_' || isalnum((int)c)); }

static ct_type convnum(ct_val *cv, const char **str) {
    ct_type res;
    const char *s = *str;
    int is_float = 0;

    /*
    ** If number contains '.' or ends in 'e' or 'E', then
    ** it is likely to be floating point. Otherwise, it is
    ** probably an int. Unless of course, it is part of a
    ** range (i.e. nn..mm). We'd better check that.
    */

    while (*s != '\0' && isdigit((int)*s)) s++;

    /*
    ** Skip over a decimal point followed by additional digits if presnt.
    ** If this is true it is a float
    */
    if (*s == '.' && *(s + 1) != '.') {
        is_float = 1;
        s++;
        while (*s != '\0' && isdigit((int)*s)) s++;
    }

    /*
    ** Skip over the 'e' or 'E' and optional '+' or '-' if present.
    ** Skip over additional digits
    */
    if (*s == 'e' || *s == 'E') {
        is_float = 1;
        ++s;
        if (*s == '+' || *s == '-') ++s;
        while (*s != '\0' && isdigit((int)*s)) s++;
    }

    if (is_float) {
        cv->cv_float = strtold(*str, const_cast<char **>(&s));
        DPRINT("convnum: res is CT_FLOAT\n");
        res = CT_FLOAT;
    } else {
        cv->cv_int = strtol(*str, const_cast<char **>(&s), 10);
        DPRINT("convnum: res is CT_INT\n");
        res = CT_INT;
    }

    if (*str == s) {
        DPRINT("convnum: returning CT_ERR!\n");
        return CT_ERR;
    }

    *str = s;

    return res;
}

static int AddCmdToken(const char *str, ct_type pt) {
    hentry_t *h;
    int *ptr;

    // Allocate your memory for this int
    ptr = (int *)Alloc(sizeof(ct_type));

    // Dereference the ptr to indicate you are storing a value.
    *ptr = pt;

    if ((h = HashSearch(_token_table, str)) != (hentry_t *)NULL) {
        h->h_data = (void *)ptr;
        return 0;
    }

    if (HashInsert(_token_table, str, (void *)ptr) == (hentry_t *)NULL) {
        // Free the ptr since the call failed.
        Free((void *)ptr);
        SetError("Could not add token \"%s\".", str);
        return -1;
    }

    return 0;
}

static int AddTypeSpecifierToken(const char *str, ct_type pt) {
    hentry_t *h;
    int *ptr;

    // Allocate your memory for this int
    ptr = (int *)Alloc(sizeof(ct_type));

    // Dereference the ptr to indicate you are storing a value.
    *ptr = pt;

    if ((h = HashSearch(_type_specifier_table, str)) != (hentry_t *)NULL) {
        h->h_data = (void *)ptr;
        return 0;
    }

    if (HashInsert(_type_specifier_table, str, (void *)ptr) ==
        (hentry_t *)NULL) {
        // Free the ptr since the call failed.
        Free((void *)ptr);
        SetError("Could not add type \"%s\".", str);
        return -1;
    }

    return 0;
}

static int AddTypeIdentifierToken(const char *str, ct_type pt) {
    hentry_t *h;
    int *ptr;

    // Allocate your memory for this int
    ptr = (int *)Alloc(sizeof(ct_type));

    // Dereference the ptr to indicate you are storing a value.
    *ptr = pt;

    if ((h = HashSearch(_type_identifier_table, str)) != (hentry_t *)NULL) {
        h->h_data = (void *)ptr;
        return 0;
    }

    if (HashInsert(_type_identifier_table, str, (void *)ptr) ==
        (hentry_t *)NULL) {
        // Free the ptr since the call failed.
        Free((void *)ptr);
        SetError("Could not add type \"%s\".", str);
        return -1;
    }

    return 0;
}

/*
** Free command token. Not sure why the hashkey is being passed
** since HashDestroy free's it.
*/
static void FreeCmdToken(char *ct_key, void *ct_token) {
    if (ct_token != (void *)NULL) Free(ct_token);
}

ct_type FindCmdToken(char *s) {
    hentry_t *h;

    if ((h = HashSearch(_token_table, s)) != (hentry_t *)NULL)
        return (*((ct_type *)h->h_data));

    return CT_ERR;
}

static void FreeTypeToken(char *ct_key, void *ct_token) {
    if (ct_token != (void *)NULL) Free(ct_token);
}

/*
** This function is used to return the symbol associated with a command token.
** This way we can ensure sterility. If a matching hash is found, we no longer
*operate on
** that matched string, we operate on the internal string associated with that
*hash.
*/
ct_type FindCmdTokenStr(char *s, ct_val *cv) {
    hentry_t *h;

    if ((h = HashSearch(_token_table, s)) != (hentry_t *)NULL) {
        if (cv != NULL) {
            cv->cv_sym = h;
        }
        return (*((ct_type *)h->h_data));
    }

    return CT_ERR;
}

/*
** Similar to FindCmdTokenStr except here we are looking for a type specifier
*/
static ct_type FindTypeSpecifierTokenStr(char *s, ct_val *cv) {
    hentry_t *h;

    if ((h = HashSearch(_type_specifier_table, s)) != (hentry_t *)NULL) {
        if (cv != NULL) {
            cv->cv_sym = h;
        }
        return (*((ct_type *)h->h_data));
    }

    return CT_ERR;
}

/*
** Similar to FindCmdTokenStr except here we are looking for a type identifier
*/
static ct_type FindTypeIdentifierTokenStr(char *s, ct_val *cv) {
    hentry_t *h;

    if ((h = HashSearch(_type_identifier_table, s)) != (hentry_t *)NULL) {
        if (cv != NULL) {
            cv->cv_sym = h;
        }
        return (*((ct_type *)h->h_data));
    }

    return CT_ERR;
}

int InitCmdToken(void) {
    /*
    ** andrewg: This function now also inits the lang type table as well.
    */
    _token_table = HashCreate(TOKEN_SIZE);
    _type_specifier_table = HashCreate(TYPE_SIZE);
    _type_identifier_table = HashCreate(TYPE_SIZE);

    if (AddCmdToken("absolute", CT_ABSOLUTE) < 0) return -1;

    if (AddCmdToken("assert", CT_ASSERT) < 0) return -1;

    if (AddCmdToken("dfmode", CT_DFMODE) < 0) return -1;

    if (AddCmdToken("display", CT_DISPLAY) < 0) return -1;

    if (AddCmdToken("error", CT_ERROR) < 0) return -1;

    if (AddCmdToken("false", CT_FALSE) < 0) return -1;

    if (AddCmdToken("off", CT_FALSE) < 0) return -1;

    if (AddCmdToken("force", CT_FORCE) < 0) return -1;

    if (AddCmdToken("halt", CT_HALT) < 0) return -1;

    if (AddCmdToken("invoke", CT_INVOKE) < 0) return -1;

    if (AddCmdToken("logfile", CT_LOGFILE) < 0) return -1;

    if (AddCmdToken("logging", CT_LOGGING) < 0) return -1;

    if (AddCmdToken("output", CT_OUTPUT) < 0) return -1;

    if (AddCmdToken("relative", CT_RELATIVE) < 0) return -1;

    if (AddCmdToken("set", CT_SET) < 0) return -1;

    if (AddCmdToken("true", CT_TRUE) < 0) return -1;

    if (AddCmdToken("on", CT_TRUE) < 0) return -1;

    if (AddCmdToken("using", CT_USING) < 0) return -1;

    if (AddCmdToken("end", CT_END) < 0) return -1;

    // Blockmap commands
    if (AddCmdToken("block", CT_BLOCK) < 0) return -1;

    if (AddCmdToken("cyclic", CT_CYCLIC) < 0) return -1;

    // Graph commands
    if (AddCmdToken("text", CT_TEXT) < 0) return -1;
    if (AddCmdToken("aif", CT_AIF) < 0) return -1;
    if (AddCmdToken("hdf", CT_HDF) < 0) return -1;
    if (AddCmdToken("hdf5", CT_HDF5) < 0) return -1;
    if (AddCmdToken("sum", CT_SUM) < 0) return -1;
    if (AddCmdToken("product", CT_PRODUCT) < 0) return -1;
    if (AddCmdToken("average", CT_AVERAGE) < 0) return -1;
    if (AddCmdToken("min", CT_MIN) < 0) return -1;
    if (AddCmdToken("max", CT_MAX) < 0) return -1;
    if (AddCmdToken("stdev", CT_STDEV) < 0) return -1;
    if (AddCmdToken("histogram", CT_HISTOGRAM) < 0) return -1;
    if (AddCmdToken("countelem", CT_COUNTELEM) < 0) return -1;
    // For now, "hash" translates to bobhash
    if (AddCmdToken("hash", CT_BOBHASH) < 0) return -1;
    if (AddCmdToken("none", CT_NONE) < 0) return -1;
    if (AddCmdToken("default", CT_DEFAULT) < 0) return -1;
    if (AddCmdToken("sync", CT_SYNC) < 0) return -1;
    if (AddCmdToken("async", CT_ASYNC) < 0) return -1;

    // C type specifiers
    if (AddTypeSpecifierToken("signed", CT_TYPE_SPEC) < 0) return -1;
    if (AddTypeSpecifierToken("unsigned", CT_TYPE_SPEC) < 0) return -1;
    if (AddTypeSpecifierToken("short", CT_TYPE_SPEC) < 0) return -1;
    if (AddTypeSpecifierToken("long", CT_TYPE_SPEC) < 0) return -1;

    // C type identifiers
    if (AddTypeIdentifierToken("char", CT_TYPE_IDENT) < 0) return -1;
    if (AddTypeIdentifierToken("int", CT_TYPE_IDENT) < 0) return -1;
    if (AddTypeIdentifierToken("float", CT_TYPE_IDENT) < 0) return -1;
    if (AddTypeIdentifierToken("double", CT_TYPE_IDENT) < 0) return -1;
    if (AddTypeIdentifierToken("struct", CT_TYPE_IDENT) < 0) return -1;

    return 0;
}

void FinishCmdToken() {
    HashDestroy(_token_table, FreeCmdToken);
    HashDestroy(_type_specifier_table, FreeTypeToken);
    HashDestroy(_type_identifier_table, FreeTypeToken);
    HashDestroy(_symtab, (void (*)(char *, void *))NULL);
}

char *SymLookup(hentry_t *s) { return s->h_key; }

void *SymLookupData(hentry_t *s) { return s->h_data; }

void SetSymData(hentry_t *s, void *d) { s->h_data = d; }

hentry_t *FindSym(char *s) {
    if (_symtab == (htable_t *)NULL) return (hentry_t *)NULL;

    return HashSearch(_symtab, s);
}

hentry_t *AddSym(char *s) {
    hentry_t *sym;

    if (_symtab == (htable_t *)NULL &&
        (_symtab = HashCreate(SYM_SIZE)) == (htable_t *)NULL) {
        SetError("Could not create symbol table.");
        return (hentry_t *)NULL;
    }

    if ((sym = HashSearch(_symtab, s)) != (hentry_t *)NULL) return sym;

    DPRINT("AddSym: Adding \"%s\" to symbol table.\n", s);

    if ((sym = HashInsert(_symtab, s, (void *)NULL)) == (hentry_t *)NULL) {
        SetError("Symbol table is full.");
        return (hentry_t *)NULL;
    }

    return sym;
}

void RemoveSym(hentry_t *sym) {
    DPRINT("RemoveSym: Removing \"%s\" from symbol table.\n", SymLookup(sym));

    HashRemove(_symtab, sym);
}

ct_type ScanString(ct_val *cv, const char **str) {
    // FIXME: I hacked this so that a CT_SYM is returning a string instead of
    // an actual symbol.

    char c;
    char *s;
    ct_type t;
    static char laststr[BUFSIZ];

    skipwhite(str);

    DPRINT("<ScanString buf>%s</ScanString buf>\n", *str);

    switch (*(*str)++) {
        case '\0':
            DPRINT("ScanString: found CT_EOF\n");
            return CT_EOF;

        case '+':
            if (*(*str) == '=') {
                (*str)++;
                DPRINT("ScanString: found CT_ADDEQ\n");
                return CT_ADDEQ;
            }
            if (*(*str) == '+') {
                (*str)++;
                DPRINT("ScanString: found CT_DBLPLUS\n");
                return CT_DBLPLUS;
            }
            DPRINT("ScanString: found CT_PLUS\n");
            return CT_PLUS;

        case '-':
            if (*(*str) == '=') {
                (*str)++;
                DPRINT("ScanString: found CT_SUBEQ\n");
                return CT_SUBEQ;
            }
            if (*(*str) == '-') {
                (*str)++;
                DPRINT("ScanString: found CT_DBLMINUS\n");
                return CT_DBLMINUS;
            }
            if (*(*str) == '>') {
                (*str)++;
                DPRINT("ScanString: found CT_ARROW\n");
                return CT_ARROW;
            }
            DPRINT("ScanString: found CT_MINUS\n");
            return CT_MINUS;

        case '*':
            if (*(*str) == '=') {
                (*str)++;
                DPRINT("ScanString: found CT_MULEQ\n");
                return CT_MULEQ;
            }
            DPRINT("ScanString: found CT_STAR\n");
            return CT_STAR;

        case '/':
            if (*(*str) == '=') {
                (*str)++;
                DPRINT("ScanString: found CT_DIVEQ\n");
                return CT_DIVEQ;
            }
            DPRINT("ScanString: found CT_DIV\n");
            return CT_DIV;

        case '%':
            if (*(*str) == '=') {
                (*str)++;
                DPRINT("ScanString: found CT_MODEQ\n");
                return CT_MODEQ;
            }
            DPRINT("ScanString: found CT_MOD\n");
            return CT_MOD;

        case '=':
            if (*(*str) == '=') {
                (*str)++;
                DPRINT("ScanString: found CT_DBLEQ\n");
                return CT_DBLEQ;
            }
            DPRINT("ScanString: found CT_EQUAL\n");
            return CT_EQUAL;

        case '~':
            DPRINT("ScanString: found CT_BTNOT\n");
            return CT_BTNOT;

        case '&':
            if (*(*str) == '&') {
                (*str)++;
                DPRINT("ScanString: found CT_AND\n");
                return CT_AND;
            }
            if (*(*str) == '=') {
                (*str)++;
                DPRINT("ScanString: found CT_BTANDEQ\n");
                return CT_BTANDEQ;
            }
            DPRINT("ScanString: found CT_BTAND\n");
            return CT_BTAND;

        case '|':
            if (*(*str) == '|') {
                (*str)++;
                DPRINT("ScanString: found CT_OR\n");
                return CT_OR;
            }
            if (*(*str) == '=') {
                (*str)++;
                DPRINT("ScanString: found CT_BTOREQ\n");
                return CT_BTOREQ;
            }
            DPRINT("ScanString: found CT_BTOR\n");
            return CT_BTOR;

        case '^':
            if (*(*str) == '=') {
                (*str)++;
                DPRINT("ScanString: found CT_BTXOREQ\n");
                return CT_BTXOREQ;
            }
            DPRINT("ScanString: found CT_BTXOR\n");
            return CT_BTXOR;

        case '!':
            if (*(*str) == '=') {
                (*str)++;
                DPRINT("ScanString: found CT_NOTEQ\n");
                return CT_NOTEQ;
            }
            DPRINT("ScanString: found CT_NOT\n");
            return CT_NOT;

        case '?':
            DPRINT("ScanString: found CT_TERNARY\n");
            return CT_TERNARY;

        case ':':
            if (*(*str) == ':') {
                (*str)++;
                DPRINT("ScanString: found CT_DBLCOL\n");
                return CT_DBLCOL;
            }
            DPRINT("ScanString: found CT_COLON\n");
            return CT_COLON;

        case '(':
            DPRINT("ScanString: found CT_LPAREN\n");
            return CT_LPAREN;

        case ')':
            DPRINT("ScanString: found CT_RPAREN\n");
            return CT_RPAREN;

        case '.':
            if (*(*str) == '.') {
                (*str)++;
                DPRINT("ScanString: found CT_DBLDOT\n");
                return CT_DBLDOT;
            }
            DPRINT("ScanString: found CT_DOT\n");
            return CT_DOT;

        case '<':
            if (*(*str) == '<') {
                (*str)++;
                DPRINT("ScanString: found CT_BTLS\n");
                return CT_BTLS;
            }
            if (*(*str) == '=') {
                (*str)++;
                DPRINT("ScanString: found CT_LEQ\n");
                return CT_LEQ;
            }
            DPRINT("ScanString: found CT_LT\n");
            return CT_LT;

        case '>':
            if (*(*str) == '>') {
                (*str)++;
                DPRINT("ScanString: found CT_BTRS\n");
                return CT_BTRS;
            }
            if (*(*str) == '=') {
                (*str)++;
                DPRINT("ScanString: found CT_GEQ\n");
                return CT_GEQ;
            }
            DPRINT("ScanString: found CT_GT\n");
            return CT_GT;

        case '[':
            DPRINT("ScanString: found CT_LBRCK\n");
            return CT_LBRCK;

        case ']':
            DPRINT("ScanString: found CT_RBRCK\n");
            return CT_RBRCK;

        case ',':
            DPRINT("ScanString: found CT_COMMA\n");
            return CT_COMMA;

        case '@':
            DPRINT("ScanString: found CT_AT\n");
            return CT_AT;

        case '{':
            DPRINT("ScanString: found CT_LBRACE\n");
            return CT_LBRACE;

        case '}':
            DPRINT("ScanString: found CT_RBRACE\n");
            return CT_RBRACE;

        case '#':
            DPRINT("ScanString: found CT_HASH\n");
            return CT_HASH;

        case '\n':
            DPRINT("ScanString: found CT_NL\n");
            return CT_NL;

        // Tokens with values start here

        // CT_STR
        case '"':
            s = laststr;

            DPRINT("ScanString: found CT_STR\n");

            while (*(*str) != '\0' && *(*str) != '"') {
                if (*(*str) == '\\') {
                    (*str)++;

                    switch (c = *(*str)) {
                        case '\\':
                        case '"':
                            *s++ = c;
                            break;

                        case 'n':
                            *s++ = '\n';
                            break;

                        case 'r':
                            *s++ = '\r';
                            break;

                        case 't':
                            *s++ = '\t';
                            break;

                        case '\0':
                            DPRINT("ScanString: returning CT_ERR!\n");
                            return CT_ERR;

                        default:
                            *s++ = *(*str);
                    }
                } else
                    *s++ = *(*str);

                (*str)++;
            }

            if (*(*str) == '\0') {
                DPRINT("ScanString: returning CT_ERR!\n");
                return CT_ERR;
            }
            (*str)++;
            *s = '\0';

            cv->cv_str = laststr;

            return CT_STR;

        // CT_DSYM
        case '$':
            // FIXME: This should recognize all known debugger symbols like
            // procset, blockmap, graph, etc... There are so many hacks patched
            // into the code base to work around this one simple change here
            // that it makes my head spin...
            s = laststr;

            DPRINT("ScanString: found CT_DSYM\n");

            while (*(*str) != '\0' && isid(*(*str))) *s++ = *(*str)++;

            // Make sure to add in null terminator
            *s = '\0';

            DPRINT("ScanString: Adding DSYM into cv_sym => %s\n", laststr);
            cv->cv_sym = (hentry_t *)AddSym(laststr);

            return CT_DSYM;

        default:
            // restore str since we encountered an unknown char
            (*str)--;

            // Check for CT_INT or CT_FLOAT
            if (isdigit((int)*(*str))) {
                DPRINT("ScanString: found CT_INT or CT_FLOAT\n");

                /*
                ** Get the number...
                */
                if ((t = convnum(cv, str)) != CT_ERR) return t;

                /*
                ** Not a number, so return error.
                */
                DPRINT("ScanString: returning CT_ERR!\n");
                return CT_ERR;
            }

            /*
            ** Not a number, so must be a keyword or new symbol.
            */
            s = laststr;

            DPRINT("ScanString: found CT_SYM\n");

            while (*(*str) != '\0' && isid(*(*str))) *s++ = *(*str)++;

            // Make sure to add in null terminator
            *s = '\0';

            if (*laststr == '\0') {
                DPRINT("ScanString: returning CT_ERR!\n");
                return CT_ERR;
            }

            // Set the string in the return type
            cv->cv_sym_str = laststr;

            /*
            ** Check for keyword.
            ** FIXME: These shouldn't even be here I think.
            ** To get around the symtable hack, I needed to treat these like a
            *sym_str
            ** instead of a sym.
            */
            if ((t = FindCmdTokenStr(laststr, NULL)) != CT_ERR) return t;
            if ((t = FindTypeSpecifierTokenStr(laststr, NULL)) != CT_ERR)
                return t;
            if ((t = FindTypeIdentifierTokenStr(laststr, NULL)) != CT_ERR)
                return t;

            return CT_SYM;
    }

    /* NOT REACHED */
}
