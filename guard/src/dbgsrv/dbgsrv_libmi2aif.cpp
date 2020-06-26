/******************************************************************************\
 * libmi to aif conversion routines
 *
 * Copyright 2011-2019 Cray Inc.	All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "dbgevent.h"
#include "dbgfn.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_libmi2aif.h"
#include "dbgsrv_util.h"
#include "defs.h"
#include "useful.h"
#include <vector>
#include <string>
#include <sstream>

/* The following is used to turn on debug printing for the CreateSimpleAIF func.
** It generates a ton of output for large arrays, which is why we want to
** normally leave it off.
*/
//#define CreateSimpleAIF_PRINT

#ifdef CreateSimpleAIF_PRINT
#define CSAPRINT(...) fprintf(stderr, __VA_ARGS__)
#else /* CreateSimpleAIF_PRINT */
#define CSAPRINT(...)
#endif /* CreateSimpleAIF_PRINT */

// Used to fix infinite loop issue for recursive calls to whatis
#define LOOP_PREVENTION 20

/*
** Types
*/
namespace {

// Type information for describing a function with its argument and return types
class func_info_t {
public:
    // note: steals ownership of all arguments
    func_info_t(int addr_len, char* rtn_type, int argc, char** argv)
        : m_addr_len(addr_len), m_rtn_type(rtn_type), m_argc(argc), m_argv(argv) {}
    ~func_info_t();
    func_info_t(const func_info_t&) = delete;
    func_info_t& operator=(const func_info_t&) = delete;

    int getAddrLen() const { return m_addr_len; }
    char* getRtnType() const { return m_rtn_type; }
    int getArgc() const { return m_argc; }
    char** getArgv() const { return m_argv; }

private:
    int m_addr_len;
    char *m_rtn_type;
    int m_argc;
    char **m_argv;
};
func_info_t::~func_info_t() {
    free(m_rtn_type);
    for (int i=0; i<m_argc; ++i)
        free(m_argv[i]);
    free(m_argv);
}

// type information for describing the data members of a struct or class
class struct_info_t {
public:
    struct_info_t(const char* name, int numFields);
    ~struct_info_t();
    struct_info_t(const struct_info_t&) = delete;
    struct_info_t& operator=(const struct_info_t&) = delete;

    // descriptor type for a single field of the struct.
    struct field {
        field() : id(0), type_str(nullptr), type_len(0), type_func(nullptr), inner_struct(nullptr) {}
        int id;
        char* type_str;
        int type_len;
        func_info_t* type_func;
        struct_info_t* inner_struct;
    };

    char* getName() { return m_name; }
    field& getField(int idx) { return m_fields[idx]; }
    const std::vector<field>& getFields() const { return m_fields; }
private:
    char* m_name;
    std::vector<field> m_fields;
}; 

struct_info_t::struct_info_t(const char* name, int numFields)
    : m_name(strdup(name)), m_fields(numFields) {
}
struct_info_t::~struct_info_t() {
    free(m_name);
    for (auto f : m_fields) {
        free(f.type_str);
        delete(f.type_func);
        delete(f.inner_struct);
    }
}
} // namespace
// external prototypes
extern void SendMICommand(MISession *, MICommand *);

/*
** Static prototypes
*/
static std::shared_ptr<MIVar> CreateMIVar(MISession *, const char *);
static void DeleteMIVar(MISession *, const char *);
static int MIVarGetChildren(MISession *, MIVar *);
static char *GetMIWhatisValue(MISession *, char *, MIVar*);
static int GetTypeLength(MISession *, const char *);
static int GetSimpleType(const char *);
static int GetComplexType(const char *);
static int GetType(char *);
static int GetArrayBaseType(MISession *, MIVar *, char **, int *,
                            struct_info_t **, func_info_t **);
static arrayDimension_t *GetArrayDimensionShape(MISession *, MIVar *);
static char *GetTypeName(char *);
static void RemoveWhitespace(char *);
static func_info_t *GetFunctionInfo(MISession *, char *);
static int GetFunctionArguments(MISession *, char *, char **[]);
static char *GetFunctionReturnType(MISession *session, char *);
static AIF *GetFunctionAddress(MISession *, char *);
static char *GetFunctionName(char *);
static AIF *CreateSimpleAIF(int, char *, char *, int, struct_info_t *,
                            func_info_t *);
static char *MIToFDS(MISession *, int, char *);
static char *GetVarValue(MISession *, char *);
static AIFStream *GetAIF(MISession *, std::shared_ptr<MIVar>);
static AIFStream *GetSimpleAIF(MISession *, MIVar *);
static AIFStream *GetComplexAIF(MISession *, std::shared_ptr<MIVar>);
static struct_info_t *GetStructInfo(MISession *, char *);
static AIFStream *GetStructAIF(MISession *, MIVar *);
// static AIF *				GetUnionAIF(MISession *, MIVar *);
static AIFStream *GetArrayAIF(MISession *, std::shared_ptr<MIVar>);
static AIFStream *GetVectorLikeAIF(MISession *, std::shared_ptr<MIVar>);
static AIF *GetArrayAIFHash(MISession *, MIVar*, char *, int, int, int);
static AIFStream *GetPointerAIF(MISession *, MIVar *);
static char *GetArrayDataString(MISession *, MIVar *);
// static arrayDimension_t *   oneDArray(int, int, int);
static void fixTypeString(char *, const char *, char *);

const char *_aif_ignored_type_specifiers[] = {"const", "restrict", NULL};

static char* EvalExpression(MISession* session, const char* expression);

// Internal globals
static dbgevent_t *dbg_error = NULL;

void printArray(unsigned int *arr, int size) {
    fprintf(stderr, "[");
    int i;
    for (i = 0; i < size; i++) {
        if (i != size - 1) fprintf(stderr, "%d, ", arr[i]);
    }
    fprintf(stderr, "]");
}

static void reset_err(void) {
    if (dbg_error != NULL) {
        FreeEvent(dbg_error);
    }
    dbg_error = NULL;
}

static dbgevent_t *get_err(void) {
    dbgevent_t *rtn = NULL;

    if (dbg_error != NULL) {
        rtn = dbg_error;
        dbg_error = NULL;
    }

    return rtn;
}

// This is the "main" function that should be called in this interface...
// It returns NULL on success, otherwise it returns an error event.
dbgevent_t *GetAIFVar(MISession *session, const char *var, AIFStream **val,
                      char **type) {
    // reset the error var
    reset_err();

    DVPRINT("GetAIFVar: Resolving variable %s.\n", var);

    auto mivar = CreateMIVar(session, var);

    if (!mivar) {
        DVPRINT("GetAIFVar: CreateMIVar returned null.\n");
        if (dbg_error == NULL) {
            EVENT_ERROR(dbg_error, DBGERR_INTERNAL, "CreateMIVar failed.");
        }
        return get_err();
    }

    AIFStream* res = GetAIF(session, mivar);
    if (!res) {
        DVPRINT("GetAIFVar: GetAIF returned null.\n");
        if (dbg_error == NULL) {
            EVENT_ERROR(dbg_error, DBGERR_INTERNAL,
                        "Parser failed to interpret gdb variable.");
        }
        return get_err();
    }
    *type = strdup(mivar->type);
    *val = res;

    return NULL;
}

// TODO: Merge this with the above somehow?
dbgevent_t *GetAIFVarHash(MISession *session, char *var, AIF **val,
                          char *hashinfo, int hashfunc, int precision_lower,
                          int precision_upper) {
    // reset the error var
    reset_err();

    auto mivar = CreateMIVar(session, var);

    if (mivar == NULL) {
        DVPRINT("GetAIFVarHash: Unknown variable\n");
        if (dbg_error == NULL) {
            EVENT_ERROR(dbg_error, DBGERR_INTERNAL, "CreateMIVar failed.");
        }
        return get_err();
    }

    if (strcmp(mivar->type, "<text variable, no debug info>") == 0) {
        DVPRINT("GetAIFVarHash: No symbols\n");
        EVENT_ERROR(dbg_error, DBGERR_NOSYMS, NULL);
        return get_err();
    }

    int id;
    int cnt = 0;

    while ((id = GetComplexType(mivar->type)) == T_UNKNOWN) {
        if (char* pt = GetMIWhatisValue(session, nullptr, mivar.get())) {
            free(mivar->type);
            mivar->type = pt;
            if (++cnt >= LOOP_PREVENTION) {
                EVENT_ERROR(dbg_error, DBGERR_INTERNAL,
                            "GetArrayAIFHash failed.");
                return get_err();
            }
        } else
            break;
    }

    AIF *res = nullptr;

    if (mivar->numchild == 0 || !(id == T_ARRAY || id == T_FTNARRAY)) {
        // simple type
        AIFStream* a = GetAIF(session, mivar);
        res = PerformHash(a->joinAIF(), hashinfo, hashfunc, precision_lower,
                          precision_upper);
        delete a;
    } else {
        // of type array
        if ((res = GetArrayAIFHash(session, mivar.get(), hashinfo, hashfunc,
                                   precision_lower, precision_upper)) == NULL) {
            DVPRINT("GetAIFVarHash: Unknown type\n");
            if (dbg_error == NULL) {
                EVENT_ERROR(dbg_error, DBGERR_INTERNAL,
                            "GetArrayAIFHash failed.");
            }
            return get_err();
        }
    }

    *val = res;
    return nullptr;
}

/*
 * Create a MI variable in gdb and get its MIVar type.
 * Returns null on error.
 */
static std::shared_ptr<MIVar> CreateMIVar(MISession *session, const char *expr) {

    std::shared_ptr<MIVar> var(nullptr, [session](MIVar* mivar) {
            if (mivar) { DeleteMIVar(session,mivar->name);  MIVarFree(mivar); }});
    
    MICommand* cmd = MIVarCreate("-", "*", expr);
    SendMICommand(session, cmd);
    // If this fails, it means the symbol doesn't exist.
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(dbg_error, DBGERR_NOSYM, expr);
        MICommandFree(cmd);
        return var;
    }

    MIVar* mivar = MIGetVarCreateInfo(cmd);
    MICommandFree(cmd);

    if (mivar->exp == NULL) {
        mivar->exp = strdup(expr);
    }

    mivar->expr = strdup(expr);

    // remove newline
    if (mivar->expr[strlen(expr) - 1] == '\n') {
        mivar->expr[strlen(expr) - 1] = '\0';
    }

    // Remove whitespace and the final '&' for reference types.
    // ptype breaks with '&'.
    char* p = &(mivar->type)[strlen(mivar->type) - 1];

    while ((p >= mivar->type) && isspace(*p)) {
        *p-- = '\0';
    }

    if (*p == '&') {
        *p = '\0';
    }

    var.reset(mivar);
    return var;
}

static void DeleteMIVar(MISession *session, const char *name) {
    MICommand *cmd;

    cmd = MIVarDelete(name);
    SendMICommand(session, cmd);
    MICommandFree(cmd);
}

static int MIVarGetChildren(MISession *session, MIVar *var) {
    MICommand *cmd;

    if (session == NULL || var == NULL) return 1;

    cmd = MIVarListChildren(var->name);
    SendMICommand(session, cmd);
    if (!MICommandResultOK(cmd)) {
        if (MICommandResultClass(cmd) == MIResultRecordERROR) {
            char *err = MICommandResultErrorMessage(cmd);
            if (err != NULL) {
                DVPRINT("MIVarGetChildren: Debugger error: %s\n", err);
                EVENT_ERROR(dbg_error, DBGERR_DEBUGGER, err);
                free(err);
            } else {
                DVPRINT(
                    "MIVarGetChildren: Error from GDB, no error message.\n");
                EVENT_ERROR(dbg_error, DBGERR_UNKNOWN,
                            "Error without message!");
            }
        } else {
            DVPRINT("MIVarGetChildren: Bad response from GDB.\n");
            EVENT_ERROR(dbg_error, DBGERR_UNKNOWN, "Bad response from GDB.");
        }
        MICommandFree(cmd);
        return 1;
    }
    MIGetVarListChildrenInfo(cmd, var);
    MICommandFree(cmd);

    return 0;
}

/*
 * Find the type using whatis. Return null on error.
 */
static char *GetMIWhatisValue(MISession *session, char *expr, MIVar *var) {
    MICommand *cmd;
    char *type;

    DVPRINT("GetMIWhatisValue: (%s, %s)\n", expr != NULL ? expr : "NULL",
            var != NULL ? var->type : "NULL");

    if (var != NULL) {
        cmd = CLIWhatis(var->type);
        SendMICommand(session, cmd);
        if (!MICommandResultOK(cmd)) {
            if (expr == NULL) {
                DVPRINT("CLIWhatis failed and NULL expr\n");
                MICommandFree(cmd);
                return NULL;
            }

            // As a last resort, try to get the whatis based on the provided
            // expr
            cmd = CLIWhatis(expr);
            SendMICommand(session, cmd);
            if (!MICommandResultOK(cmd)) {
                DVPRINT("CLIWhatis failed for both\n");
                MICommandFree(cmd);
                return NULL;
            }
        }
    } else if (expr != NULL) {
        cmd = CLIWhatis(expr);
        SendMICommand(session, cmd);
        if (!MICommandResultOK(cmd)) {
            DVPRINT("CLIWhatis failed for expr\n");
            MICommandFree(cmd);
            return NULL;
        }
    } else {
        DVPRINT("CLIWhatis both expr and var are NULL!\n");
        return NULL;
    }

    type = CLIGetWhatisInfo(cmd);

    DVPRINT("Type is \"%s\"\n", type);

    MICommandFree(cmd);

    return type;
}

static void fixTypeString(char *p, const char *s, char *orig) {
    int extra_char = 0;  // used for extra characters to account for

    if (p == NULL || s == NULL || orig == NULL) {
        DVPRINT("fixTypeString: bad args\n");
        return;
    }

    DVPRINT("fixTypeString: p is \"%s\"\n", p);
    DVPRINT("fixTypeString: s is \"%s\"\n", s);

    // If this is the last stuff in the string, we simply null terminate.
    if (strlen(p) == strlen(s)) {
        DVPRINT("fixTypeString: null terminate method.\n");
        // Check for a preceeding " "
        if (*(p - 1) == ' ') {
            *--p = '\0';
        } else {
            *p = '\0';
        }
    } else {
        DVPRINT("fixTypeString: memmove method.\n");
        // If we are not at the start of the string, there might be a
        // preceeding space to deal with
        if (p > orig) {
            // check for a preceeding " "
            if (*(p - 1) == ' ') {
                // decrement p to pick up the space
                --p;
                ++extra_char;
            }
        }

        // Check for space after string
        if (*(p + strlen(s)) == ' ') {
            // point past the ' ' character
            memmove(p, p + (strlen(s) + extra_char + 1),
                    (strlen(p) + 1) - (strlen(s) + extra_char + 1));
        } else {
            // no ' ' character
            memmove(p, p + strlen(s) + extra_char,
                    (strlen(p) + 1) - (strlen(s) + extra_char));
        }
    }

    DVPRINT("fixTypeString: New string is \"%s\"\n", p);
}

static char* EvalExpression(MISession* session, const char* expression)
{
    MICommand *cmd = MIDataEvaluateExpression(expression);
    SendMICommand(session, cmd);
    if (!MICommandResultOK(cmd)) {
        DVPRINT("EvalExpression got error from %s\n", expression);
        MICommandFree(cmd);
        return nullptr;
    }

    char* res = MIGetDataEvaluateExpressionInfo(cmd);
    MICommandFree(cmd);
    return res;
}

static char *sanitize_type_string(const char *input) {
    // first we need to sanitize the stupid string if gpu debugging is turned on
    // I don't know why Nvidia decided to be dumb by putting the address space
    // idents in but then making the gdb parser not understand them...
    char *clean_str;
    char *p;
    int found;
    clean_str = strdup(input);

    if (gpu_flag) {
        while ((p = strchr(clean_str, '@')) != NULL) {
            // reset found
            found = 0;
            if (NV_CHECK_ADDR_IDENT(p, CODE_S)) {
                DVPRINT("GetTypeLength: Found %s address space identifier\n",
                        CODE_S);
                ++found;
                fixTypeString(p, CODE_S, clean_str);
            }
            if (NV_CHECK_ADDR_IDENT(p, CONSTANT_S)) {
                DVPRINT("GetTypeLength: Found %s address space identifier\n",
                        CONSTANT_S);
                ++found;
                fixTypeString(p, CONSTANT_S, clean_str);
            }
            if (NV_CHECK_ADDR_IDENT(p, GENERIC_S)) {
                DVPRINT("GetTypeLength: Found %s address space identifier\n",
                        GENERIC_S);
                ++found;
                fixTypeString(p, GENERIC_S, clean_str);
            }
            if (NV_CHECK_ADDR_IDENT(p, GLOBAL_S)) {
                DVPRINT("GetTypeLength: Found %s address space identifier\n",
                        GLOBAL_S);
                ++found;
                fixTypeString(p, GLOBAL_S, clean_str);
            }
            if (NV_CHECK_ADDR_IDENT(p, PARAMETER_S)) {
                DVPRINT("GetTypeLength: Found %s address space identifier\n",
                        PARAMETER_S);
                ++found;
                fixTypeString(p, PARAMETER_S, clean_str);
            }
            if (NV_CHECK_ADDR_IDENT(p, SHARED_S)) {
                DVPRINT("GetTypeLength: Found %s address space identifier\n",
                        SHARED_S);
                ++found;
                fixTypeString(p, SHARED_S, clean_str);
            }
            if (NV_CHECK_ADDR_IDENT(p, TEXTURE_S)) {
                DVPRINT("GetTypeLength: Found %s address space identifier\n",
                        TEXTURE_S);
                ++found;
                fixTypeString(p, TEXTURE_S, clean_str);
            }
            if (NV_CHECK_ADDR_IDENT(p, LOCAL_S)) {
                DVPRINT("GetTypeLength: Found %s address space identifier\n",
                        LOCAL_S);
                ++found;
                fixTypeString(p, LOCAL_S, clean_str);
            }
            if (NV_CHECK_ADDR_IDENT(p, REGISTER_S)) {
                DVPRINT("GetTypeLength: Found %s address space identifier\n",
                        REGISTER_S);
                ++found;
                fixTypeString(p, REGISTER_S, clean_str);
            }

            // ensure found triggered, otherwise just break to avoid infinite
            // loop this will cause an error later on though.
            if (!found) {
                DVPRINT("GetTypeLength: Avoiding infinite loop!\n");
                break;
            }
        }
    }

    const char **current = _aif_ignored_type_specifiers;

    while (*current != NULL) {
        while ((p = strstr(clean_str, *current)) != NULL) {
            fixTypeString(p, *current, clean_str);
        }
        current++;
    }

    return clean_str;
}

static int GetTypeLength(MISession *session, const char *in_base_type) {
    char *res;
    MICommand *cmd;
    char *buf;
    int type_len;  // in bytes

    DVPRINT("GetTypeLength: Resolving length of %s\n", in_base_type);

    // sanity
    if (in_base_type == NULL) return 0;

    char *base_type = sanitize_type_string(in_base_type);

    // create the command string
    Asprintf(&buf, "\"sizeof(%s)\"", base_type);

    DBEPERFLOG(SetTimerStart("GetTypeLength: -data-evaluate-expression"));
    cmd = MIDataEvaluateExpression(buf);
    SendMICommand(session, cmd);
    if (!MICommandResultOK(cmd)) {
        DVPRINT("GetTypeLength: Error\n");
        MICommandFree(cmd);
        free(buf);
        return 0;
    }
    DBEPERFLOG(SetTimerStopAndLog("GetTypeLength: -data-evaluate-expression"));
    res = MIGetDataEvaluateExpressionInfo(cmd);
    MICommandFree(cmd);
    free(buf);

    type_len = (int)strtol(res, NULL, 10);

    free(res);

    // free the base_type if gpu flag is turned on
    if (gpu_flag) {
        free(base_type);
    }

    return type_len;
}

static char *GetVarValue(MISession *session, char *var) {
    char *res;
    MICommand *cmd;

    DVPRINT("GetVarValue: Evaluate %s.\n", var);

    DBEPERFLOG(SetTimerStart("GetVarValue: -data-evaluate-expression"));
    cmd = MIVarEvaluateExpression(var);
    DBEPERFLOG(SetTimerStopAndLog("GetVarValue: -data-evaluate-expression"));

    SendMICommand(session, cmd);
    if (!MICommandResultOK(cmd)) {
        DVPRINT("GetVarValue: Error\n");
        EVENT_ERROR(dbg_error, DBGERR_INTERNAL, "GetAIF failed.");
        MICommandFree(cmd);
        return NULL;
    }
    res = MIGetVarEvaluateExpressionInfo(cmd);
    MICommandFree(cmd);

    // Check to see if the value was optimized out
    if (strcmp(res, "<value optimized out>") == 0) {
        EVENT_ERROR(dbg_error, DBGERR_OPT, NULL);
        free(res);
        return NULL;
    }

    return res;
}

static AIFStream *GetAIF(MISession *session, std::shared_ptr<MIVar> var) {
    DVPRINT("GetAIF: %s\n", var->expr);

    if (strcmp(var->type, "<text variable, no debug info>") == 0) {
        DVPRINT("GetAIF: No symbols\n");
        EVENT_ERROR(dbg_error, DBGERR_NOSYMS, nullptr);
        return nullptr;
    }
    if (var->numchild == 0) {
        // simple type
        DVPRINT("GetAIF: Resolved to simple type.\n");
        return GetSimpleAIF(session, var.get());
    }

    // complex type
    DVPRINT("GetAIF: Resolved to complex type.\n");
    return GetComplexAIF(session, var);
}

static AIFStream *GetSimpleAIF(MISession *session, MIVar *var) {

    AIF *ac;
    AIF *addr;
    char *v;
    int argc;
    char **argv;
    char *rtn;
    char *name;

    DVPRINT(
        "GetSimpleAIF: MIVar: expr %s, name %s, exp %s, numchild %d, type %s, "
        "value %s, thread_id %d\n",
        var->expr, var->name, var->exp, var->numchild, var->type, var->value,
        var->thread_id);

    int cnt = 0;
    int id;
    while ((id = GetSimpleType(var->type)) == T_UNKNOWN) {
        // default to using whatis
        if (char* pt = GetMIWhatisValue(session, nullptr, var)) {
            free(var->type);
            var->type = pt;
            if (++cnt >= LOOP_PREVENTION) {
                break;
            }
        } else
            break;
    }

    AIFStream *a = nullptr;
    switch (id) {
        case T_FUNCTION:
            DVPRINT("GetSimpleAIF: Creating AIF function.\n");
            argc = GetFunctionArguments(session, var->type, &argv);
            if (debug_flag) {
                int i;
                DVPRINT("GetSimpleAIF: Num args: %d\n", argc);
                for (i = 0; i < argc; ++i) {
                    DVPRINT("GetSimpleAIF: argv[%d]=\"%s\"\n", i, argv[i]);
                }
            }
            rtn = GetFunctionReturnType(session, var->type);
            if (rtn == NULL) {
                // see PE-17307:
                // to prevent segfault, check the rtn value for null, return
                // null if rtn== NULL
                // TODO: create test case for fortran real named "TIME"
                DVPRINT(
                    "GetSimpleAIF: GetFunctionReturnType returned: %s, "
                    "returning NULL!\n",
                    rtn);
                return nullptr;
            }
            DVPRINT("GetSimpleAIF: Function return type: %s\n", rtn);
            if ((v = GetVarValue(session, var->name)) == NULL) {
                // error already set
                free(rtn);
                return nullptr;
            }
            name = GetFunctionName(v);
            DVPRINT("GetSimpleAIF: Function name: %s\n", name);
            addr = GetFunctionAddress(session, v);
            if (debug_flag) {
                char *aif_str;
                AIFToStr(&aif_str, 0, addr);
                DVPRINT(
                    "GetSimpleAIF: Function address: %s func_name %s "
                    "func_rtn_fmt %p argc %d\n",
                    aif_str, name, rtn, argc);
                Free(aif_str);
            }
            ac = FunctionToAIF(addr, name, rtn, argc, argv);
            if (debug_flag) {
                char *aif_str;
                AIFToStr(&aif_str, 0, ac);
                DVPRINT("GetSimpleAIF: FDS string is: %s\n", AIF_FORMAT(ac));
                DVPRINT("GetSimpleAIF: Function AIF size is: %d\n",
                        AIF_LEN(ac));
                DVPRINT("GetSimpleAIF: Function AIF type is: %s\n", aif_str);
                Free(aif_str);
            }
            free(v);
            free(rtn);
            free(name);
            AIFFree(addr);
            return new AIFStream(ac);

        case T_ENUM:
            DVPRINT("GetSimpleAIF: Creating AIF enum.\n");
            return new AIFStream(EmptyEnumToAIF(GetTypeName(var->type)));

        case T_VOID_PTR:
            // void ptr's don't have any children and are considered simple
            // types
            DVPRINT("GetComplexAIF: Creating AIF void pointer.\n");
            return GetPointerAIF(session, var);

        case T_UNKNOWN:
            // Unrecognized type.
            DVPRINT("GetSimpleAIF: Unknown type. Returning error.\n");
            EVENT_ERROR(dbg_error, DBGERR_BADTYPE, NULL);
            return NULL;

        default:
            DVPRINT("GetSimpleAIF: Creating simple type.\n");
            if ((v = GetVarValue(session, var->name)) == NULL) {
                // error already set
                return NULL;
            }
            a = new AIFStream(CreateSimpleAIF(id, v, NULL, 0, NULL, NULL));
            free(v);
            return a;
    }
}

static AIFStream *GetComplexAIF(MISession *session, std::shared_ptr<MIVar> var) {
    int id;

    //--- Find the AIF type -----
    // We allow multiple passes to break off extra type decoration.   I believe
    // the LOOP_PREVENTION count was primarily in place because of the non-progress
    // case that is now recognized as T_CLASS, but I'm not 100% sure so we'll leave it
    // in a safety backup.
    for (int cnt=0; cnt < LOOP_PREVENTION &&
             ((id = GetComplexType(var->type)) == T_UNKNOWN); ++cnt) {
        if (char* pt = GetMIWhatisValue(session, NULL, var.get())) {
            if (strcmp(pt, var->type) == 0) {
                // C++ style struct and classes loop into here because the class type
                // name can't be broken down further.  So whatis("MyStruct") == "MyStruct"
                DVPRINT("GetComplexAIF: Creating AIF class.\n");
                free(pt);
                id = T_STRUCT;
                break;
            } 

            // otherwise, we have a hopefully simpler version of type, so give
            // GetComplexType another chance.
            free(var->type);
            var->type = pt;
        } else {
            break;
        }
    }

    //--- Create the AIF. ---- 
    AIFStream *a = nullptr;
    switch (id) {
        case T_ARRAY:
        case T_FTNARRAY:
            DVPRINT("GetComplexAIF: Creating AIF array.\n");
            a = GetArrayAIF(session, var);
            break;

        case T_POINTER:
            DVPRINT("GetComplexAIF: Creating AIF pointer.\n");
            a = GetPointerAIF(session, var.get());
            break;

        case T_UNION:
            // NOTE: Unions in C are untagged, meaning that we have no intrinsic
            // language support to tell us what is currently placed inside the
            // union. We only know that one of the elements is allocated. To
            // deal with this, gdb will show the user the representation of each
            // field, with the assumption that all but the actual field
            // represents junk. In order to deal with this, we treat unions as
            // structs in this layer, that way the user will be able to see all
            // of the representations of what is currently placed in the union.
            DVPRINT(
                "GetComplexAIF: Creating AIF struct to represent union "
                "type.\n");
            a = GetStructAIF(session, var.get());
            break;

        case T_STRUCT:
        case T_CLASS:
        // Pretend a derived type is a struct
        case T_FTNDTYPE:
            DVPRINT("GetComplexAIF: Creating AIF struct.\n");
            a = GetStructAIF(session, var.get());
            break;

        case T_STD_VECTOR:
        case T_STD_DEQUE:
        case T_STD_STACK:
        case T_STD_QUEUE:
        case T_STD_PQUEUE:
            DVPRINT("GetComplexAIF: Creating AIF Vector.\n");
            a = GetVectorLikeAIF(session, var);
            break;

        default:
            /*
             * Maybe it was simple all alongg
             */
            DVPRINT("GetComplexAIF: Defaulting to simple aif.\n");
            a = GetSimpleAIF(session, var.get());
    }

    return a;
}

static int GetSimpleType(const char *type) {
    char *t = sanitize_type_string(type);
    int len = strlen(type);

    DVPRINT("GetSimpleType: %s\n", type);

    // instead of strncmp on everything, lets jump in char by char
    // Note that we need to compare case insensitive since every compiler
    // generates different type information.
start:
    switch (*t++) {
            // This is for Nvidia address space identifiers that may or may not
            // be present case "@"
        case '@':
            switch (*t++) {
                    // case "@c"
                case 'c':
                case 'C':
                    switch (*t++) {
                            // case "@co"
                        case 'o':
                        case 'O':
                            switch (*t++) {
                                    // case "@cod"
                                case 'd':
                                case 'D':
                                    if (strncasecmp(t, "e ", 2) == 0) {
                                        // string "@code "
                                        DVPRINT(
                                            "GetSimpleType: Found @code "
                                            "address space identifier!\n");
                                        t += 2;
                                        // we ignore these address space
                                        // identifiers, so simply return the
                                        // value of the recursive call to this
                                        // function
                                        return GetSimpleType(t);
                                    }
                                    // case "@cod" unknown
                                    goto unknown;

                                    // case "@con"
                                case 'n':
                                case 'N':
                                    if (strncasecmp(t, "stant ", 6) == 0) {
                                        // string "@constant "
                                        DVPRINT(
                                            "GetSimpleType: Found @constant "
                                            "address space identifier!\n");
                                        t += 6;
                                        // we ignore these address space
                                        // identifiers, so simply return the
                                        // value of the recursive call to this
                                        // function
                                        return GetSimpleType(t);
                                    }
                                    // case "@con" unknown
                                    goto unknown;

                                    // case "@co" unknown
                                default:
                                    goto unknown;
                            }

                            // case "@c" unknown
                        default:
                            goto unknown;
                    }

                    // case "@g"
                case 'g':
                case 'G':
                    switch (*t++) {
                            // case "@ge"
                        case 'e':
                        case 'E':
                            if (strncasecmp(t, "neric ", 6) == 0) {
                                // string "@generic "
                                DVPRINT(
                                    "GetSimpleType: Found @generic address "
                                    "space identifier!\n");
                                t += 6;
                                // we ignore these address space identifiers, so
                                // simply return the value of the recursive call
                                // to this function
                                return GetSimpleType(t);
                            }
                            // case "@ge" unknown
                            goto unknown;

                            // case "@gl"
                        case 'l':
                        case 'L':
                            if (strncasecmp(t, "obal ", 5) == 0) {
                                // string "@global "
                                DVPRINT(
                                    "GetSimpleType: Found @global address "
                                    "space identifier!\n");
                                t += 5;
                                // we ignore these address space identifiers, so
                                // simply return the value of the recursive call
                                // to this function
                                return GetSimpleType(t);
                            }
                            // case "@gl" unknown
                            goto unknown;

                            // case "@g" unknown
                        default:
                            goto unknown;
                    }

                    // case "@p"
                case 'p':
                case 'P':
                    if (strncasecmp(t, "arameter ", 9) == 0) {
                        // string "@parameter "
                        DVPRINT(
                            "GetSimpleType: Found @parameter address space "
                            "identifier!\n");
                        t += 9;
                        // we ignore these address space identifiers, so simply
                        // return the value of the recursive call to this
                        // function
                        return GetSimpleType(t);
                    }
                    // case "@p" unknown
                    goto unknown;

                    // case "@s"
                case 's':
                case 'S':
                    if (strncasecmp(t, "hared ", 6) == 0) {
                        // string "@shared "
                        DVPRINT(
                            "GetSimpleType: Found @shared address space "
                            "identifier!\n");
                        t += 6;
                        // we ignore these address space identifiers, so simply
                        // return the value of the recursive call to this
                        // function
                        return GetSimpleType(t);
                    }
                    // case "@s" unknown
                    goto unknown;

                    // case "@t"
                case 't':
                case 'T':
                    if (strncasecmp(t, "exture ", 7) == 0) {
                        // string "@texture "
                        DVPRINT(
                            "GetSimpleType: Found @texture address space "
                            "identifier!\n");
                        t += 7;
                        // we ignore these address space identifiers, so simply
                        // return the value of the recursive call to this
                        // function
                        return GetSimpleType(t);
                    }
                    // case "@t" unknown
                    goto unknown;

                    // case "@l"
                case 'l':
                case 'L':
                    if (strncasecmp(t, "ocal ", 5) == 0) {
                        // string "@local "
                        DVPRINT(
                            "GetSimpleType: Found @local address space "
                            "identifier!\n");
                        t += 5;
                        // we ignore these address space identifiers, so simply
                        // return the value of the recursive call to this
                        // function
                        return GetSimpleType(t);
                    }
                    // case "@l" unknown
                    goto unknown;

                    // case "@r"
                case 'r':
                case 'R':
                    if (strncasecmp(t, "egister ", 8) == 0) {
                        // string "@register "
                        DVPRINT(
                            "GetSimpleType: Found @register address space "
                            "identifier!\n");
                        t += 8;
                        // we ignore these address space identifiers, so simply
                        // return the value of the recursive call to this
                        // function
                        return GetSimpleType(t);
                    }
                    // case "@r" unknown
                    goto unknown;

                    // case "@" unknown
                default:
                    goto unknown;
            }

            // case "b"
        case 'b':
        case 'B':
            switch (*t++) {
                    // case "bo"
                case 'o':
                case 'O':
                    if (strncasecmp(t, "ol", 2) == 0) {
                        // string "bool"
                        DVPRINT("GetSimpleType: Found boolean!\n");
                        return T_BOOLEAN;
                    }
                    // case "bo" unknown
                    goto unknown;

                    // case "by"
                case 'y':
                case 'Y':
                    if (strncasecmp(t, "te", 2) == 0) {
                        // string "byte"
                        DVPRINT("GetSimpleType: Found fortran byte!\n");
                        return T_INTEGER_1;
                    }
                    // case "by" unknown
                    goto unknown;

                default:
                    // case "b" unknown
                    goto unknown;
            }

            // case "c"
        case 'c':
        case 'C':
            switch (*t++) {
                    // case "ch"
                case 'h':
                case 'H': {
                    if (strncasecmp(t, "ar", 2) == 0) {
                        // string "char"
                        t += 2;
                        switch (*t) {
                            case '\0':
                                // case "char"
                                DVPRINT("GetSimpleType: Found char!\n");
                                return T_CHAR;

                            case 'a':
                            case 'A':
                                // case "chara"
                                // NOTE: fortran only defines one type of
                                // character (kind=1)
                                ++t;
                                if (strncasecmp(t, "cter", 4) == 0) {
                                    // string "character"
                                    DVPRINT(
                                        "GetSimpleType: Found fortran "
                                        "character!\n");
                                    return T_CHARACTER;
                                }
                                // "chara" unknown
                                goto unknown;

                            default:
                                // "char" unknown
                                goto unknown;
                        }
                    }
                    // "ch" unknown
                    goto unknown;
                }

                    // case "co"
                case 'o':
                case 'O': {
                    switch (*t++) {
                            // case "con"
                        case 'n':
                        case 'N':
                            if (strncasecmp(t, "st ", 3) == 0) {
                                // string "const "
                                // This gets rid of the const modifier
                                DVPRINT(
                                    "GetSimpleType: Found const modifier.\n");
                                t += 3;  // one past the white space
                                goto start;
                            }
                            // case "con" unknown
                            goto unknown;

                            // case "com"
                        case 'm':
                        case 'M':
                            if (strncasecmp(t, "plex", 4) == 0) {
                                // string "complex"
                                DVPRINT(
                                    "GetSimpleType: Found fortran complex!\n");
                                t += 4;
                                switch (*t) {
                                    case '\0':
                                        // pgi defaults to kind = 4
                                        DVPRINT(
                                            "GetSimpleType: Fortran kind=4\n");
                                        return T_COMPLEX_4;

                                    case '(':
                                        // note that type[len-1] is the closing
                                        // bracket or ')' intel compiler gives
                                        // us COMPLEX(n) where n is the kind gnu
                                        // compiler gives us complex(kind=n)
                                        switch (type[len - 2]) {
                                            case '4':
                                                DVPRINT(
                                                    "GetSimpleType: Fortran "
                                                    "kind=4\n");
                                                return T_COMPLEX_4;

                                            case '8':
                                                DVPRINT(
                                                    "GetSimpleType: Fortran "
                                                    "kind=8\n");
                                                return T_COMPLEX_8;

                                            case '6':
                                                DVPRINT(
                                                    "GetSimpleType: Fortran "
                                                    "kind=16\n");
                                                return T_COMPLEX_16;

                                            default:
                                                DVPRINT(
                                                    "GetSimpleType: "
                                                    "Unsupported kind!\n");
                                                goto unknown;
                                        }

                                    case '*':
                                        // cray compiler uses this. COMPLEX*n
                                        // where n is the kind.
                                        switch (*++t) {
                                            case '4':
                                                DVPRINT(
                                                    "GetSimpleType: Fortran "
                                                    "kind=4\n");
                                                return T_COMPLEX_4;

                                            case '8':
                                                DVPRINT(
                                                    "GetSimpleType: Fortran "
                                                    "kind=8\n");
                                                return T_COMPLEX_8;

                                            case '1':
                                                if (*++t == '6') {
                                                    DVPRINT(
                                                        "GetSimpleType: "
                                                        "Fortran kind=16\n");
                                                    return T_COMPLEX_16;
                                                }

                                            default:
                                                DVPRINT(
                                                    "GetSimpleType: "
                                                    "Unsupported kind!\n");
                                                goto unknown;
                                        }

                                    default:
                                        DVPRINT(
                                            "GetSimpleType: Unsupported "
                                            "kind!\n");
                                        goto unknown;
                                }
                            }
                            // "com" unknown
                            goto unknown;

                        default:
                            // case "co" unknown
                            goto unknown;
                    }
                }

                default:
                    // case "c" unknown
                    goto unknown;
            }

            // case "d"
        case 'd':
        case 'D':
            if (strncasecmp(t, "ouble", 5) == 0) {
                // string "double"
                t += 5;
                switch (*t) {
                    case '\0':
                        // case "double"
                        DVPRINT("GetSimpleType: Found double!\n");
                        return T_DOUBLE;

                    case ' ':
                        // case "double "
                        switch (*++t) {
                            case 'c':
                            case 'C':
                                ++t;
                                if (strncasecmp(t, "omplex", 6) == 0) {
                                    // string "double complex"
                                    // pgi uses double complex
                                    DVPRINT(
                                        "GetSimpleType: Found fortran double "
                                        "complex!\n");
                                    return T_COMPLEX_8;
                                }
                                // case "double c" unknown
                                // this is not a valid function!
                                goto unknown_invalid;

                            case 'p':
                            case 'P':
                                ++t;
                                if (strncasecmp(t, "recision", 8) == 0) {
                                    // string "double precision"
                                    DVPRINT(
                                        "GetSimpleType: Found fortran double "
                                        "precision!\n");
                                    return T_REAL_8;
                                }
                                // case "double p" unknown
                                goto unknown_invalid;

                            default:
                                // case "double " unknown
                                goto unknown_invalid;
                        }

                    default:
                        // case "double" unknown
                        goto unknown;
                }
            }
            // case "d" unknown
            goto unknown;

            // case "e"
        case 'e':
        case 'E':
            if (strncasecmp(t, "num", 3) == 0) {
                // string "enum"
                DVPRINT("GetSimpleType: Found enum!\n");
                return T_ENUM;
            }
            // case "e" unknown
            goto unknown;

            // case "f"
        case 'f':
        case 'F':
            if (strncasecmp(t, "loat", 4) == 0) {
                // string "float"
                DVPRINT("GetSimpleType: Found float!\n");
                return T_FLOAT;
            }
            // case "f" unknown
            goto unknown;

            // case "i"
        case 'i':
        case 'I':
            if (strncasecmp(t, "nt", 2) == 0) {
                // string "int"
                t += 2;
                switch (*t) {
                    case '\0':
                        DVPRINT("GetSimpleType: Found int!\n");
                        return T_INT;

                    case 'e':
                    case 'E':
                        ++t;
                        if (strncasecmp(t, "ger", 3) == 0) {
                            // string "integer"
                            DVPRINT("GetSimpleType: Found fortran integer!\n");
                            t += 3;
                            switch (*t) {
                                case '\0':
                                    // pgi defaults to kind = 4
                                    DVPRINT("GetSimpleType: Fortran kind=4\n");
                                    return T_INTEGER_4;

                                case '(':
                                    // note that type[len-1] is the closing
                                    // bracket or ')' intel compiler gives us
                                    // INTEGER(n) where n is the kind gnu
                                    // compiler gives us integer(kind=n)
                                    switch (type[len - 2]) {
                                        case '1':
                                            DVPRINT(
                                                "GetSimpleType: Fortran "
                                                "kind=1\n");
                                            return T_INTEGER_1;
                                        case '2':
                                            DVPRINT(
                                                "GetSimpleType: Fortran "
                                                "kind=2\n");
                                            return T_INTEGER_2;
                                        case '4':
                                            DVPRINT(
                                                "GetSimpleType: Fortran "
                                                "kind=4\n");
                                            return T_INTEGER_4;
                                        case '8':
                                            DVPRINT(
                                                "GetSimpleType: Fortran "
                                                "kind=8\n");
                                            return T_INTEGER_8;
                                        default:
                                            DVPRINT(
                                                "GetSimpleType: Unsupported "
                                                "kind!\n");
                                            goto unknown;
                                    }

                                case '*':
                                    // cray compiler uses this INTEGER*n where n
                                    // is the kind
                                    switch (*++t) {
                                        case '1':
                                            DVPRINT(
                                                "GetSimpleType: Fortran "
                                                "kind=1\n");
                                            return T_INTEGER_1;
                                        case '2':
                                            DVPRINT(
                                                "GetSimpleType: Fortran "
                                                "kind=2\n");
                                            return T_INTEGER_2;
                                        case '4':
                                            DVPRINT(
                                                "GetSimpleType: Fortran "
                                                "kind=4\n");
                                            return T_INTEGER_4;
                                        case '8':
                                            DVPRINT(
                                                "GetSimpleType: Fortran "
                                                "kind=8\n");
                                            return T_INTEGER_8;
                                        default:
                                            DVPRINT(
                                                "GetSimpleType: Unsupported "
                                                "kind!\n");
                                            goto unknown;
                                    }

                                default:
                                    // case "integer" unknown
                                    goto unknown;
                            }
                        }
                        // case "inte" unknown
                        goto unknown;

                    case '1':
                        DVPRINT("GetSimpleType: Fortran kind=1\n");
                        return T_INTEGER_1;

                    case '2':
                        DVPRINT("GetSimpleType: Fortran kind=2\n");
                        return T_INTEGER_2;

                    case '4':
                        DVPRINT("GetSimpleType: Fortran kind=4\n");
                        return T_INTEGER_4;

                    case '8':
                        DVPRINT("GetSimpleType: Fortran kind=8\n");
                        return T_INTEGER_8;

                    default:
                        // check for stdint int*_t types including null
                        // terminator
                        if (strncasecmp(t, "8_t", 4) == 0) {
                            return T_CHAR;
                        } else if (strncasecmp(t, "16_t", 5) == 0) {
                            return T_SHORT;
                        } else if (strncasecmp(t, "32_t", 5) == 0) {
                            return T_INT;
                        } else if (strncasecmp(t, "64_t", 5) == 0) {
                            return T_LONGLONG;
                        }

                        // case "int" unknown
                        goto unknown;
                }
            }
            // case "i" unknown
            goto unknown;

            // case "l"
        case 'l':
        case 'L':
            switch (*t++) {
                    // case "lo"
                case 'o':
                case 'O':
                    switch (*t++) {
                            // case "lon"
                        case 'n':
                        case 'N':
                            switch (*t++) {
                                    // case "long"
                                case 'g':
                                case 'G':
                                    switch (*t) {
                                        case '\0':
                                            // case "long"
                                            DVPRINT(
                                                "GetSimpleType: Found long!\n");
                                            return T_LONG;

                                        case ' ':
                                            // here we recursively call this
                                            // function to avoid having to
                                            // duplicate cases. We return a
                                            // value based on the return value
                                            // of the recursive call. Make sure
                                            // to preinc t to point past the '
                                            // '.
                                            DVPRINT(
                                                "GetSimpleType: Going "
                                                "recursive\n");
                                            switch (GetSimpleType(++t)) {
                                                case T_INT:
                                                    DVPRINT(
                                                        "GetSimpleType: Found "
                                                        "long!\n");
                                                    return T_LONG;
                                                case T_UINT:
                                                    DVPRINT(
                                                        "GetSimpleType: Found "
                                                        "unsigned long!\n");
                                                    return T_ULONG;
#ifdef CC_HAS_LONG_LONG
                                                case T_LONG:
                                                    DVPRINT(
                                                        "GetSimpleType: Found "
                                                        "long long!\n");
                                                    return T_LONGLONG;
                                                case T_ULONG:
                                                    DVPRINT(
                                                        "GetSimpleType: Found "
                                                        "unsigned long "
                                                        "long!\n");
                                                    return T_ULONGLONG;
#endif /* CC_HAS_LONG_LONG */
#ifdef HAVE_LONG_DOUBLE
                                                case T_DOUBLE:
                                                    DVPRINT(
                                                        "GetSimpleType: Found "
                                                        "long double!\n");
                                                    return T_LONGDOUBLE;
#endif
                                                default:
                                                    // case "long " unknown,
                                                    // this is not a valid func
                                                    // name
                                                    goto unknown_invalid;
                                            }
                                        default:
                                            // case "long" unknown
                                            goto unknown;
                                    }
                                default:
                                    // case "lon" unknown
                                    goto unknown;
                            }
                            // case "log"
                        case 'g':
                        case 'G':
                            if (strncasecmp(t, "ical", 4) == 0) {
                                // string "logical"
                                DVPRINT(
                                    "GetSimpleType: Found fortran logical!\n");
                                t += 4;

                                switch (*t) {
                                    case '\0':
                                        // pgi defaults to kind = 4
                                        DVPRINT(
                                            "GetSimpleType: Fortran kind=4\n");
                                        return T_LOGICAL_4;

                                    case '(':
                                        // note that type[len-1] is the closing
                                        // bracket or ')' intel compiler gives
                                        // us LOGICAL(n) where n is the kind gnu
                                        // compiler gives us logical(kind=n)
                                        switch (type[len - 2]) {
                                            case '1':
                                                DVPRINT(
                                                    "GetSimpleType: Fortran "
                                                    "kind=1\n");
                                                return T_LOGICAL_1;
                                            case '2':
                                                DVPRINT(
                                                    "GetSimpleType: Fortran "
                                                    "kind=2\n");
                                                return T_LOGICAL_2;
                                            case '4':
                                                DVPRINT(
                                                    "GetSimpleType: Fortran "
                                                    "kind=4\n");
                                                return T_LOGICAL_4;
                                            case '8':
                                                DVPRINT(
                                                    "GetSimpleType: Fortran "
                                                    "kind=8\n");
                                                return T_LOGICAL_8;
                                            default:
                                                DVPRINT(
                                                    "GetSimpleType: "
                                                    "Unsupported kind!\n");
                                                goto unknown;
                                        }

                                    case '*':
                                        // cray compiler uses this LOGICAL*n
                                        // where n is the kind
                                        switch (*++t) {
                                            case '1':
                                                DVPRINT(
                                                    "GetSimpleType: Fortran "
                                                    "kind=1\n");
                                                return T_LOGICAL_1;
                                            case '2':
                                                DVPRINT(
                                                    "GetSimpleType: Fortran "
                                                    "kind=2\n");
                                                return T_LOGICAL_2;
                                            case '4':
                                                DVPRINT(
                                                    "GetSimpleType: Fortran "
                                                    "kind=4\n");
                                                return T_LOGICAL_4;
                                            case '8':
                                                DVPRINT(
                                                    "GetSimpleType: Fortran "
                                                    "kind=8\n");
                                                return T_LOGICAL_8;
                                            default:
                                                DVPRINT(
                                                    "GetSimpleType: "
                                                    "Unsupported kind!\n");
                                                goto unknown;
                                        }

                                    default:
                                        DVPRINT(
                                            "GetSimpleType: Unsupported "
                                            "kind!\n");
                                        goto unknown;
                                }
                            }
                            // case "log" unknown
                            goto unknown;

                        default:
                            // case "lo" unknown
                            goto unknown;
                    }

                default:
                    // case "l" unknown
                    goto unknown;
            }

            // case "r"
        case 'r':
        case 'R':
            if (strncasecmp(t, "eal", 3) == 0) {
                // string "real"
                DVPRINT("GetSimpleType: Found fortran real!\n");
                t += 3;
                switch (*t) {
                    case '\0':
                        // pgi defaults to kind = 4
                        DVPRINT("GetSimpleType: Fortran kind=4\n");
                        return T_REAL_4;

                    case '(':
                        // note that type[len-1] is the closing bracket or ')'
                        // intel compiler gives us REAL(n) where n is the kind
                        // gnu compiler gives us real(kind=n)
                        switch (type[len - 2]) {
                            case '4':
                                DVPRINT("GetSimpleType: Fortran kind=4\n");
                                return T_REAL_4;
                            case '8':
                                DVPRINT("GetSimpleType: Fortran kind=8\n");
                                return T_REAL_8;
                            case '6':
                                DVPRINT("GetSimpleType: Fortran kind=16\n");
                                return T_REAL_16;
                            default:
                                DVPRINT("GetSimpleType: Unsupported kind!\n");
                                return T_UNKNOWN;
                        }

                    case '*':
                        // cray compiler uses this REAL*n where n is the kind
                        switch (*++t) {
                            case '4':
                                DVPRINT("GetSimpleType: Fortran kind=4\n");
                                return T_REAL_4;
                            case '8':
                                DVPRINT("GetSimpleType: Fortran kind=8\n");
                                return T_REAL_8;
                            case '1':
                                if (*++t == '6') {
                                    DVPRINT("GetSimpleType: Fortran kind=16\n");
                                    return T_REAL_16;
                                }
                            default:
                                DVPRINT("GetSimpleType: Unsupported kind!\n");
                                return T_UNKNOWN;
                        }

                    default:
                        DVPRINT("GetSimpleType: Unsupported kind!\n");
                        return T_UNKNOWN;
                }
            }
            // case "r" unknown
            goto unknown;

            // case "s"
        case 's':
        case 'S':
            switch (*t++) {
                    // case "sh"
                case 'h':
                case 'H':
                    if (strncasecmp(t, "ort", 3) == 0) {
                        t += 3;
                        switch (*t) {
                            case ' ':
                                // string "short "
                                DVPRINT(
                                    "GetSimpleType: Found short specifier!\n");
                                t += 1;
                                // here we recursively call this function to
                                // avoid having to duplicate cases. We return a
                                // value based on the return value of the
                                // recursive call.
                                DVPRINT("GetSimpleType: Going recursive\n");
                                switch (GetSimpleType(t)) {
                                    case T_INT:
                                        DVPRINT(
                                            "GetSimpleType: Found short!\n");
                                        return T_SHORT;
                                    case T_UINT:
                                        DVPRINT(
                                            "GetSimpleType: Found unsigned "
                                            "short!\n");
                                        return T_USHORT;
                                    default:
                                        DVPRINT(
                                            "GetSimpleType: Unknown short!\n");
                                        goto unknown_invalid;
                                }

                            case '\0':
                                // type short
                                DVPRINT("GetSimpleType: short!\n");
                                return T_SHORT;
                        }
                    }
                    if (strncasecmp(t, "ared ", 3) == 0) {
                        // string "shared "
                        // This gets rid of the shared modifier
                        DVPRINT("GetSimpleType: Found shared modifier.\n");
                        t += 5;  // one past the white space
                        goto start;
                    }
                    // case "sh" unknown
                    goto unknown;

                    // case "si"
                case 'i':
                case 'I':
                    if (strncasecmp(t, "gned ", 5) == 0) {
                        // string "signed "
                        DVPRINT("GetSimpleType: Found signed specifier!\n");
                        t += 5;
                        // since we regard signed the same as a normal type,
                        // simply return the value of the recursive call to this
                        // function
                        return GetSimpleType(t);
                    }
                    // case "si" unknown
                    goto unknown;

                    // case "st"
                case 't':
                case 'T':
                    if (strncasecmp(t, "ring", 4) == 0) {
                        // string "string"
                        DVPRINT("GetSimpleType: Found string!\n");
                        return T_STRING;
                    }
                    // case "st" unknown
                    goto unknown;

                default:
                    // case "s" unknown
                    goto unknown;
            }

            // case "u"
        case 'u':
        case 'U':
            if (strncasecmp(t, "nsigned", 7) == 0) {
                // string "unsigned"
                DVPRINT("GetSimpleType: Found unsigned specifier!\n");
                t += 7;
                switch (*t) {
                    case '\0':
                        DVPRINT("GetSimpleType: Found unsigned!\n");
                        return T_UINT;

                    case ' ':
                        // here we recursively call this function to avoid
                        // having to duplicate cases. We return a value based on
                        // the return value of the recursive call. Make sure to
                        // preinc t to point past the ' '.
                        DVPRINT("GetSimpleType: Going recursive\n");
                        switch (GetSimpleType(++t)) {
                            case T_CHAR:
                                DVPRINT(
                                    "GetSimpleType: Found unsigned char!\n");
                                return T_UCHAR;
                            case T_SHORT:
                                DVPRINT(
                                    "GetSimpleType: Found unsigned short!\n");
                                return T_USHORT;
                            case T_INT:
                                DVPRINT("GetSimpleType: Found unsigned int!\n");
                                return T_UINT;
                            case T_LONG:
                                DVPRINT(
                                    "GetSimpletype: Found unsigned long!\n");
                                return T_ULONG;
#ifdef CC_HAS_LONG_LONG
                            case T_LONGLONG:
                                DVPRINT(
                                    "GetSimpleType: Found unsigned long "
                                    "long!\n");
                                return T_ULONGLONG;
#endif /* CC_HAS_LONG_LONG */
                            default:
                                DVPRINT("GetSimpleType: Unknown unsigned!\n");
                                goto unknown_invalid;
                        }

                    default:
                        // case "unsigned" unknown
                        goto unknown;
                }
            }

            if (strncasecmp(t, "int", 3) == 0) {
                t += 3;
                // check for stdint uint*_t types including null terminator
                if (strncasecmp(t, "8_t", 4) == 0) {
                    return T_UCHAR;
                } else if (strncasecmp(t, "16_t", 5) == 0) {
                    return T_USHORT;
                } else if (strncasecmp(t, "32_t", 5) == 0) {
                    return T_UINT;
                } else if (strncasecmp(t, "64_t", 5) == 0) {
                    return T_ULONGLONG;
                }
            }

            // case "u" unknown
            goto unknown;

            // case "v"
        case 'v':
        case 'V':
            switch (*t++) {
                    // case "vo"
                case 'o':
                case 'O':
                    switch (*t++) {
                            // case "voi"
                        case 'i':
                        case 'I':
                            switch (*t++) {
                                    // case "void"
                                case 'd':
                                case 'D':
                                    switch (*t) {
                                        case '\0':
                                            // string "void"
                                            DVPRINT(
                                                "GetSimpleType: Found void.\n");
                                            return T_VOID;

                                        case ' ':
                                            // string "void "
                                            t += 1;
                                            if (*t == '*') {
                                                // string "void *"
                                                DVPRINT(
                                                    "GetSimpleType: Found void "
                                                    "ptr.\n");
                                                return T_VOID_PTR;
                                            }
                                        default:
                                            // case "void " unknown
                                            goto unknown;
                                    }
                                    // case "void" unknown
                                default:
                                    goto unknown;
                            }
                            // case "voi" unknown
                            goto unknown;

                            // case "vol"
                        case 'l':
                        case 'L':
                            if (strncasecmp(t, "atile ", 6) == 0) {
                                // string "volatile "
                                // This gets rid of the volatile modifier
                                DVPRINT(
                                    "GetSimpleType: Found volatile "
                                    "modifier.\n");
                                t += 6;  // one past the white space
                                goto start;
                            }
                            // case "vol" unknown
                            goto unknown;

                        default:
                            // case "vo" unknown
                            goto unknown;
                    }

                default:
                    // case "v" unknown
                    goto unknown;
            }

        // We are finally at the default case for the first character in the
        // string. Note that we want to check if the string is a function if we
        // jumped to unknown or if its first letter was not a known type
        // specifier.
        default:
        unknown:
            if (type[len - 1] == ')') {
                DVPRINT("GetSimpleType: Found function!\n");
                return T_FUNCTION;
            }

        unknown_invalid:
            // this is the bottom of the parser. If we get here, the type is
            // truly unknown. This means that the parser probably has to be
            // expanded to handle the unknown type, or there was an error with
            // the type string provided by gdb.
            DVPRINT("GetSimpleType: Unknown type.\n");
            return T_UNKNOWN;
    }
}

/*
 * Attempt to determine the type of a variable from a type string. Assumes
 * that the type is structured. Returns T_UNKNOWN if the type can't be
 * determined.
 *
 * Possible return values:
 * 	T_ARRAY
 * 	T_POINTER
 * 	T_UNION
 * 	T_STRUCT
 * 	T_UNKNOWN
 */
static int GetComplexType(const char *type) {
    type = sanitize_type_string(type);
    int len = strlen(type);
    if (strncmp(type, "std::", 5) == 0) {
        type += 5;
        switch (*type) {
            case 'v':
                DVPRINT("GetComplexType: Found std::vector\n");
                return T_STD_VECTOR;
            case 'd':
                DVPRINT("GetComplexType: Found std::deque\n");
                return T_STD_DEQUE;
            case 's':
                DVPRINT("GetComplexType: Found std::stack\n");
                return T_STD_STACK;
            case 'p':
                DVPRINT("GetComplexType: Found std::priority_queue\n");
                return T_STD_PQUEUE;
            case 'q':
                DVPRINT("GetComplexType: Found std::queue\n");
                return T_STD_QUEUE;
        }
    }

    const char *t = &type[len - 1];
    const char *p;
    // Check for nvidia address space identifiers, we want to ignore these.
    if (gpu_flag && ((p = strrchr(type, '@')) != NULL)) {
        DVPRINT("GetComplexType: Pointing at \"%s\"\n", p);

        // ensure that the @... business is the last part of the string,
        // otherwise something else is going on
        if (NV_CHECK_ADDR_IDENT_L(p, CODE_S)) {
            DVPRINT("GetComplexType: Found %s address space identifier\n",
                    CODE_S);
            t = p - 2;
        }
        if (NV_CHECK_ADDR_IDENT_L(p, CONSTANT_S)) {
            DVPRINT("GetComplexType: Found %s address space identifier\n",
                    CONSTANT_S);
            t = p - 2;
        }
        if (NV_CHECK_ADDR_IDENT_L(p, GENERIC_S)) {
            DVPRINT("GetComplexType: Found %s address space identifier\n",
                    GENERIC_S);
            t = p - 2;
        }
        if (NV_CHECK_ADDR_IDENT_L(p, GLOBAL_S)) {
            DVPRINT("GetComplexType: Found %s address space identifier\n",
                    GLOBAL_S);
            t = p - 2;
        }
        if (NV_CHECK_ADDR_IDENT_L(p, PARAMETER_S)) {
            DVPRINT("GetComplexType: Found %s address space identifier\n",
                    PARAMETER_S);
            t = p - 2;
        }
        if (NV_CHECK_ADDR_IDENT_L(p, SHARED_S)) {
            DVPRINT("GetComplexType: Found %s address space identifier\n",
                    SHARED_S);
            t = p - 2;
        }
        if (NV_CHECK_ADDR_IDENT_L(p, TEXTURE_S)) {
            DVPRINT("GetComplexType: Found %s address space identifier\n",
                    TEXTURE_S);
            t = p - 2;
        }
        if (NV_CHECK_ADDR_IDENT_L(p, LOCAL_S)) {
            DVPRINT("GetComplexType: Found %s address space identifier\n",
                    LOCAL_S);
            t = p - 2;
        }
        if (NV_CHECK_ADDR_IDENT_L(p, REGISTER_S)) {
            DVPRINT("GetComplexType: Found %s address space identifier\n",
                    REGISTER_S);
            t = p - 2;
        }

        DVPRINT("GetComplexType: Pointing at \'%c\'\n", *t);
    }

    switch (*t) {
        case ']':
            DVPRINT("GetComplexType: Found array\n");
            return T_ARRAY;

        case '*':
            DVPRINT("GetComplexType: Found pointer\n");
            return T_POINTER;  // normal pointer

        case ')':
            // check to see if this is a derived type
            if (strncasecmp(type, "type(", strlen("type(")) == 0) {
                DVPRINT("GetComplexType: Found fortran derived type\n");
                return T_FTNDTYPE;
            }
            DVPRINT("GetComplexType: Found fortran array\n");
            return T_FTNARRAY;

        default:
            if (strncmp(type, "union", 5) == 0) {
                DVPRINT("GetComplexType: Found union\n");
                return T_UNION;
            }

            if (strncmp(type, "struct", 6) == 0) {
                DVPRINT("GetComplexType: Found struct\n");
                return T_STRUCT;
            }

            DVPRINT("GetComplexType: Unknown type %s\n", type);
            return T_UNKNOWN;
    }
}

static int GetType(char *type) {
    type = sanitize_type_string(type);
    int id = GetSimpleType(type);
    if (id == T_UNKNOWN) {
        id = GetComplexType(type);
    }

    return id;
}

/*
 * Get the base type of an array from it's type
 * definition string.
 */
static int GetArrayBaseType(MISession *session, MIVar *var, char **type_str,
                            int *type_len, struct_info_t **type_struct,
                            func_info_t **type_func) {
    if (type_struct)
        *type_struct = nullptr;
    if (type_func)
        *type_func = nullptr;
    
    char *vecStr = strdup(var->type);
    int done = 0;
    //DVPRINT("GetArrayBaseType: %s\n", var->type);

    if (strncmp(vecStr, "std::", 5) == 0) {
        vecStr += 5;
        while (!done) {
            switch ((*vecStr++)) {
                case '<':
                    if (strncmp(vecStr, "std", 3) == 0) {
                        break;
                    }

                    else {
                        vecStr = strtok(vecStr, ",");
                        done++;
                        break;
                    }
                default:
                    break;
            }
        }
        if (type_str != NULL) *type_str = strdup(vecStr);
        if (type_len != NULL) *type_len = GetTypeLength(session, vecStr);
        return GetType(vecStr);
    }

    char *str = strdup(var->type);
    int id;
    int index;
    char *p;
    done = 0;
    // grab the length of the string
    index = strlen(str) - 1;
    p = &str[index];

    // check if a valid end_delim matches the last character in the string
    switch (*p--) {
        case ARRAY_C_TYPE_END:
        case ARRAY_FTN_TYPE_END:
            DVPRINT("GetArrayBaseType: Valid array detected.\n");
            break;

        default:
            // unknown array specification
            DVPRINT("GetArrayBaseType: Unknown Array Type!\n");
            free(str);
            return T_UNKNOWN;
    }

    // loop through until we find the end of the array range information
    while (!done) {
        // bound check
        if (index == 0) {
            // unknown array specification
            DVPRINT("GetArrayBaseType: OutofBounds error!\n");
            free(str);
            return T_UNKNOWN;
        }

        // see if we are at the start of the range info
        switch (*p--) {
            case ARRAY_C_TYPE_START:
            case ARRAY_FTN_TYPE_START:
                // check to see if p[index-1] is a end delimiter, if so we
                // are not past the range information yet
                switch (*p) {
                    case ARRAY_C_TYPE_END:
                    case ARRAY_FTN_TYPE_END:
                        // we are not done
                        --index;
                        --p;
                        break;

                    default:
                        // we are done
                        // ignore array range information
                        *(p + 1) = '\0';
                        ++done;
                }
                break;

            default:
                // ignore it
                --index;
        }
    }

    // bound check
    if (index == 0) {
        // unknown array specification
        DVPRINT("GetArrayBaseType: OutofBounds error!\n");
        free(str);
        return T_UNKNOWN;
    }

    // remove whitespace
    while (*p == ' ' && index != 0) {
        *p-- = '\0';
        --index;
    }

    // bound check
    if (index == 0) {
        // unknown array specification
        DVPRINT("GetArrayBaseType: OutofBounds error!\n");
        free(str);
        return T_UNKNOWN;
    }

    DVPRINT("GetArrayBaseType: Base resolved to %s.\n", str);

    // get the base type
    id = GetType(str);

    if (type_str != NULL) *type_str = strdup(str);
    if (type_len != NULL) *type_len = GetTypeLength(session, str);

    // if its still unknown, try to use ptype
    while (id == T_UNKNOWN) {
        DVPRINT("GetArrayBaseType: Base still unknown, using whatis.\n");
        p = GetMIWhatisValue(session, str, NULL);
        if (!p) {
            break;
        } else if (str && strcmp(str,p)==0) {
            free(p);
            break;
        }
        
        id = GetType(p);
        if (type_str != NULL) {
            free(*type_str);
            *type_str = strdup(p);
        }
        if (type_len != NULL) *type_len = GetTypeLength(session, p);
        free(str);
        str = p;
    }

    if (id == T_UNKNOWN) {
        // C++ structs don't use the word "struct" so we need to just see if
        // GetStructInfo gives us a result
        DVPRINT("Still unknown, trying as a struct with: %s\n", str);
        if (struct_info_t* si = GetStructInfo(session,str)) {
            DVPRINT("Got a struct result\n");
            if (type_struct)
                *type_struct = si;
            else
                delete si;
            id = T_STRUCT;
        } else {
            DVPRINT("Did not get a struct result\n");
        }
    } else if (id == T_STRUCT && type_struct != NULL) {
        // If this is a C-style struct name, we need to resolve the struct info
        *type_struct = GetStructInfo(session, str);
    }

    // If this is a function, we need to resolve the func info
    if (id == T_FUNCTION && type_func != NULL) {
        *type_func = GetFunctionInfo(session, str);
    }

    free(str);

    return id;
}

// get the shape of an array
static arrayDimension_t *GetArrayDimensionShape(MISession *session,
                                                MIVar *var) {
    arrayDimension_t *shape;
    int done = 0;
    int isRange = 0;
    int isFortran = 0;
    int len = 0;
    int min = 0;
    int max = 0;
    char *p;
    char *str = strdup(var->type);

    DVPRINT("GetArrayDimensionShape: %s\n", var->type);

    // point to the last char in the string
    p = &str[strlen(str) - 1];

    // ensure we have valid dimensional information
    switch (*p) {
        case ARRAY_FTN_TYPE_END:
            isFortran = 1;
        case ARRAY_C_TYPE_END:
            break;
        default:
            // unknown array specification
            DVPRINT("GetArrayDimensionShape: Unknown Array Type!");
            free(str);
            return (arrayDimension_t *)NULL;
    }

    // get a new arrayDimension_t object from its constructor
    if ((shape = newArrayDimension()) == (arrayDimension_t *)NULL) {
        DVPRINT("GetArrayDimensionShape: arrayDimension_t ctor failed!");
        free(str);
        return (arrayDimension_t *)NULL;
    }
    shape->isFortran = isFortran;

    // set the end delimiter char to '\0'
    *p-- = '\0';

    // iterate while we are not pointing to the begining of the range info
    while (!done) {
        // iterate while we are not pointing to a start delimiter and separation
        // delimiter and str
        switch (*p) {
            case ARRAY_C_TYPE_START:
                // c arrays have a range of 0:N-1 where N is the typedef size of
                // the array
                len = atoi(p + 1);
                min = 0;
                max = len - 1;
                // add p+1 to the dimension definition list
                if (addArrayDimensionDef(shape, len, min, max)) {
                    DVPRINT(
                        "GetArrayDimensionShape: addArrayDimensionDef failed!");
                    free(str);
                    freeArrayDimension(shape);
                    return (arrayDimension_t *)NULL;
                }
                DVPRINT(
                    "GetArrayDimensionShape: Added dimension %d of length %d "
                    "with range %d to %d\n",
                    shape->dim, len, min, max);

                // check if there is a end delim right after this
                switch (*--p) {
                    case ARRAY_C_TYPE_END:
                        // post-dec p setting the end delim to '\0'
                        *p-- = '\0';
                        break;

                    default:
                        // done parsing the string
                        ++done;
                }
                break;

            // fortran represents this in a (1,2,..,n) way
            // fortran also allows for range definitions in a N:M way
            case ARRAY_TYPE_SEP:
                // only fortran allows for the range specification
                if (isRange) {
                    // this is the min index for the range
                    // remember that we are parsing backwards
                    min = atoi(p + 1);
                    // for a range of N:M the formula is M-N+1
                    len = max - min + 1;
                    isRange = 0;
                } else {
                    max = atoi(p + 1);
                    min = 1;
                    len = max;
                }

                // add p+1 to the dimension definition list
                if (addArrayDimensionDef(shape, len, min, max)) {
                    DVPRINT(
                        "GetArrayDimensionShape: addArrayDimensionDef failed!");
                    free(str);
                    freeArrayDimension(shape);
                    return (arrayDimension_t *)NULL;
                }

                // post-dec p setting the end delim to '\0'
                *p-- = '\0';

                DVPRINT(
                    "GetArrayDimensionShape: Added dimension %d of length %d "
                    "with range %d to %d\n",
                    shape->dim, len, min, max);
                break;

            // fortran is done when it finds a '(' character
            case ARRAY_FTN_TYPE_START:
                if (isRange) {
                    // this is the min index for the range
                    // remember that we are parsing backwards
                    min = atoi(p + 1);
                    // for a range of N:M the formula is M-N+1
                    len = max - min + 1;
                    isRange = 0;
                } else {
                    max = atoi(p + 1);
                    min = 1;
                    len = max;
                }
                // add p+1 to the dimension definition list
                if (addArrayDimensionDef(shape, len, min, max)) {
                    DVPRINT(
                        "GetArrayDimensionShape: addArrayDimensionDef failed!");
                    free(str);
                    freeArrayDimension(shape);
                    return (arrayDimension_t *)NULL;
                }
                DVPRINT(
                    "GetArrayDimensionShape: Added dimension %d of length %d "
                    "with range %d to %d\n",
                    shape->dim, len, min, max);
                // done parsing the string
                ++done;
                break;

            // fortran allows for the type information to be specified as a
            // range such as (0:5,1:3) we need to deal with this case
            case ARRAY_FTN_RANGE_SEP:
                // ensure isRange is not set. This would mean we encountered
                // multiple ":" in the range string.
                if (isRange) {
                    DVPRINT("GetArrayDimensionShape: isRange was already set!");
                    free(str);
                    freeArrayDimension(shape);
                    return (arrayDimension_t *)NULL;
                }
                // store the integer value of p+1 in len and set the isRange
                // flag.
                max = atoi(p + 1);
                ++isRange;
                // post-dec p setting the end delim to '\0'
                *p-- = '\0';
                break;

            default:
                // check if p is less than or equal to str
                if (p <= str) {
                    DVPRINT("GetArrayDimensionShape: OutofBounds error!");
                    free(str);
                    freeArrayDimension(shape);
                    return (arrayDimension_t *)NULL;
                }
                // decrement p
                --p;
        }
    }
    // free the str we were using
    free(str);

    DVPRINT("GetArrayDimensionShape: Total dimensions detected %d (%s)\n",
            shape->dim, isFortran?"fortran":"C");

    return shape;
}

/*
 * Get the name of a type (struct, union or enum) from the
 * MI type string. This now also works for any generic variable, for instance
 * int i would return i, char *c would return c.
 *
 * Returns the type name or NULL if the type is unnamed.
 */
static char *GetTypeName(char *type) {
    char *p;
    char *p2;

    DVPRINT("GetTypeName: %s\n", type);

    if ((p = strrchr(type, ' ')) != NULL) {
        p++;
        if (strncmp(p, "{...}", 5) == 0) {
            DVPRINT("GetTypeName: NULL type\n");
            return NULL;
        }

        while (*p == '*') {
            p++;
        }

        DVPRINT("GetTypeName: returning %s\n", p);

        return p;
    }

    // check for fortran derived type
    if (strncasecmp(type, "TYPE(", strlen("TYPE(")) == 0) {
        p = type;
        // point past type
        p += strlen("TYPE(");
        // set closing brace to null term
        if ((p2 = strchr(p, ')')) != NULL) {
            *p2 = '\0';
        }

        DVPRINT("GetTypeName: returning %s\n", p);

        return p;
    }

    DVPRINT("GetTypeName: returning %s\n", type);

    return type;
}

/*
** Remove the whitespace from the string represented by str.
*/
static void RemoveWhitespace(char *str) {
    char *p1 = str;
    char *p2 = str;

    if (*p1 == '\0') {
        return;
    }

    // blanket remove all leading spaces
    while (isspace(*p1) && (*p1 != '\0')) {
        ++p1;
    }

    while (*p1 != '\0') {
        if (isspace(*p1)
            // don't remove spaces between keywords (assume type doesn't
            // seperate with more than one space)
            &&
            ((p1[1] == '\0') || !(isalnum(*(p1 - 1)) && isalnum(*(p1 + 1))))) {
            ++p1;
        } else {
            *p2++ = *p1++;
        }
    }
    // set p2 to a final null terminator
    *p2 = '\0';
}

/*
** This function is used when creating a simple AIF type from a gdb string.
*/
static func_info_t *GetFunctionInfo(MISession *session, char *type_str) {
    DVPRINT("GetFunctionInfo: %s\n", type_str);

    // get the number of arguments and arguments of function
    char **argv;
    int argc = GetFunctionArguments(session, type_str, &argv);
    DVPRINT("GetFunctionInfo: Num args: %d\n", argc);

    // get the return type of the function
    char* rtn_type = GetFunctionReturnType(session, type_str);
    DVPRINT("GetFunctionInfo: Function return type: %s\n", rtn_type);

    // We need to ask for the length of an address on this system
    int addr_len = GetTypeLength(session, "void *");

    auto result = new func_info_t(addr_len, rtn_type, argc, argv);
    if (!result)
        DVPRINT("GetFunctionInfo: Malloc Failed.\n");

    return result;
}

/*
** This function returns the number of arguments of a function and writes the
** FDS type of the arg into the location of argv. Note that this function will
** malloc a new argv array and it is the callers responsibility to free it.
*/
static int GetFunctionArguments(MISession *session, char *type, char **argv[]) {
    char *start;
    char *end;
    char *res;
    char *p;
    char *q;
    int arg_type;
    char *arg;
    int num_args = 1;
    int i;

    DVPRINT("GetFunctionArguments: %s\n", type);

    // find the opening '('
    if ((start = strchr(type, FUNCTION_START)) == (char *)NULL) {
        // couldn't find opening '('
        DVPRINT("GetFunctionArguments: Couldn't find opening \"(\"\n");
        return 0;
    }

    // Check to see if this is a function pointer
    if (!strncmp(start, VOID_FUNCTION_STR, strlen(VOID_FUNCTION_STR))) {
        DVPRINT("GetFunctionArguments: Found function pointer!\n");
        // point start past the VOID_FUNCTION_STR string
        start += strlen(VOID_FUNCTION_STR);
    }

    // make start point past the '('
    ++start;

    // make a new copy of start
    res = strdup(start);

    // now find the end
    if ((end = strrchr(res, FUNCTION_END)) == (char *)NULL) {
        // couldn't find closing ')'
        DVPRINT("GetFunctionArguments: Couldn't find closing \")\"\n");
        free(res);
        return 0;
    }
    // make the closing ')' into the terminating null character
    *end = '\0';

    // get rid of any whitespace in the arguments
    RemoveWhitespace(res);

    DVPRINT("GetFunctionArguments: res is %s\n", res);

    // check for zero length arg string, or void
    if (strlen(res) == 0 || strncasecmp(res, "void", 4) == 0) {
        free(res);
        return 0;
    }

    // figure out the number of arguments
    p = res;
    while ((p = strchr(p, FUNCTION_ARG_SEP)) != (char *)NULL) {
        // advance p and num_args
        ++p;
        ++num_args;
    }

    // allocate the argv buffer
    if ((*argv = (char **)malloc(sizeof(char *) * num_args)) == (void *)0) {
        DVPRINT("GetFunctionArguments: malloc error\n");
        free(res);
        return 0;
    }

    DVPRINT("GetFunctionArguments: num_args is %d\n", num_args);

    // parse the args and convert to FDS format string
    p = res;
    for (i = 0; i < num_args; ++i) {
        // point q at the first ','
        // turn the first FUNCTION_ARG_SEP into a null term
        if ((q = strchr(p, FUNCTION_ARG_SEP)) != (char *)NULL) {
            *q = '\0';
        }

        // get the type id of p
        arg_type = GetType(p);

        if ((arg = MIToFDS(session, arg_type, p)) == NULL) {
            DVPRINT("GetFunctionArguments: Unknown farg_type!\n");
            free(res);
            return 0;
        }

        // get the FDS string, this needs to be strdup'ed since the
        // string is returned in a private buffer
        (*argv)[i] = strdup(arg);

        DVPRINT("GetFunctionArguments: Added arg %s\n", *argv[i]);

        // point p 1 past q and continue
        if (q != NULL) {
            p = q + 1;
        } else if (i + 1 != num_args) {
            DVPRINT("GetFunctionArguments: argv != num_args\n");
            free(res);
            return i;
        }
    }

    // free the res string
    free(res);

    return num_args;
}

/*
** This function returns the return type of a function from the type string of a
** MIVar represented by type. The return type is found by taking the string
*before
** the opening '(' and removing any whitespace.
*/
static char *GetFunctionReturnType(MISession *session, char *type) {
    char *str;
    char *end;
    int rtn_type;
    char *rtn;

    if (type == NULL) return (char *)NULL;

    DVPRINT("GetFunctionReturnType: %s\n", type);
    str = strdup(type);

    if ((end = strchr(str, FUNCTION_START)) == (char *)NULL) {
        // couldn't find opening '('
        free(str);
        return NULL;
    }
    // make the opening '(' into the null terminator
    *end = '\0';

    // get rid of any whitespace in the return type string
    RemoveWhitespace(str);

    // get the type id of str
    rtn_type = GetType(str);
    // get the FDS string, this needs to be strdup'ed since the
    // string is returned in a private buffer
    if ((rtn = MIToFDS(session, rtn_type, str)) == NULL) {
        free(str);
        return (char *)NULL;
    }

    return strdup(rtn);
}

/*
** This function returns the address of a function from the value string of a
** MIVar represented by value. It is expected that gdb/mi will represent the
** function address as a numerical string found before " <"
*/
static AIF *GetFunctionAddress(MISession *session, char *value) {
    char *start;
    char *end;
    long long int addr_loc;
    void *addr;
    int addr_len;

    DVPRINT("GetFunctionAddress: %s\n", value);

    if (value == NULL) return (AIF *)NULL;

    start = strdup(value);

    // find the first '<' in the start string
    if ((end = strchr(start, '<')) != (char *)NULL) {
        // make the opening '<' into the terminating null character
        *end = '\0';
    }

    RemoveWhitespace(start);

    // convert the address of the pointer to an integer value
    addr_loc = strtoll(start, NULL, 0);

    // Make the void pointer point at the memory location of the pointer
    addr = (void *)addr_loc;

    // We need to ask for the length of the address type for this particular
    // pointer.
    addr_len = GetTypeLength(session, "void *");

    // free the string
    free(start);

    // Create and return the AIF type representing the address
    return AddressToAIF(addr, addr_len);
}

/*
** This function returns the name of a function from the value string of a
** MIVar represented by value. It is expected that gdb/mi will represent the
** function name in a value string as being found between '<' and '>'
*/
static char *GetFunctionName(char *value) {
    char *start;
    char *end;
    char *res;

    CSAPRINT("GetFunctionName: %s\n", value);

    // find the final '<' in the value string
    if ((start = strrchr(value, '<')) == (char *)NULL) {
        // couldn't find final '<'
        return strdup("");
    }
    // point pass the '<'
    ++start;

    // make a new copy of start
    res = strdup(start);

    // find the final '>' in the value string
    if ((end = strrchr(res, '>')) == (char *)NULL) {
        // couldn't find closing '>'
        free(res);
        return strdup("");
    }
    // make the closing '>' into the terminating null character
    *end = '\0';

    RemoveWhitespace(res);

    return res;
}

/*
** Get a valid AIF format description string (FDS) for a MI type.
*/
static char *MIToFDS(MISession *session, int type_id, char *base_type) {
    char *fds_type = NULL;
    int len;

    DVPRINT("MIToFDS: base_type=>%s\n", base_type);

    switch (type_id) {
        case T_CHAR:
            fds_type = AIF_CHARACTER_TYPE();
            DVPRINT("MIToFDS: fds_type=>%s\n", fds_type);
            break;

        case T_STRING:
            len = GetTypeLength(session, base_type);
            fds_type = AIF_STRING_TYPE(len);
            DVPRINT("MIToFDS: fds_type=>%s\n", fds_type);
            break;

        case T_SHORT:
        case T_INT:
        case T_LONG:
        case T_LONGLONG:
            len = GetTypeLength(session, base_type);
            fds_type = AIF_INTEGER_TYPE(1, len);
            DVPRINT("MIToFDS: fds_type=>%s\n", fds_type);
            break;

        case T_USHORT:
        case T_UINT:
        case T_ULONG:
        case T_ULONGLONG:
            len = GetTypeLength(session, base_type);
            fds_type = AIF_INTEGER_TYPE(0, len);
            DVPRINT("MIToFDS: fds_type=>%s\n", fds_type);
            break;

        case T_FLOAT:
        case T_DOUBLE:
#ifdef HAVE_LONG_DOUBLE
        case T_LONGDOUBLE:
#endif
            len = GetTypeLength(session, base_type);
            fds_type = AIF_FLOATING_TYPE(len);
            DVPRINT("MIToFDS: fds_type=>%s\n", fds_type);
            break;

        case T_BOOLEAN:
            fds_type = AIF_BOOLEAN_TYPE();
            DVPRINT("MIToFDS: fds_type=>%s\n", fds_type);
            break;

        case T_INTEGER_1:
        case T_INTEGER_2:
        case T_INTEGER_4:
        case T_INTEGER_8:
            len = GetTypeLength(session, base_type);
            fds_type = AIF_FTN_INTEGER_TYPE(len);
            DVPRINT("MIToFDS: fds_type=>%s\n", fds_type);
            break;

        case T_REAL_4:
        case T_REAL_8:
        case T_REAL_16:
            len = GetTypeLength(session, base_type);
            fds_type = AIF_FTN_REAL_TYPE(len);
            DVPRINT("MIToFDS: fds_type=>%s\n", fds_type);
            break;

        case T_COMPLEX_4:
        case T_COMPLEX_8:
        case T_COMPLEX_16:
            len = GetTypeLength(session, base_type);
            fds_type = AIF_FTN_COMPLEX_TYPE(len);
            DVPRINT("MIToFDS: fds_type=>%s\n", fds_type);
            break;

        case T_LOGICAL_1:
        case T_LOGICAL_2:
        case T_LOGICAL_4:
        case T_LOGICAL_8:
            len = GetTypeLength(session, base_type);
            fds_type = AIF_FTN_LOGICAL_TYPE(len);
            DVPRINT("MIToFDS: fds_type=>%s\n", fds_type);
            break;

        case T_CHARACTER:
            len = GetTypeLength(session, base_type);
            fds_type = AIF_FTN_CHARACTER_TYPE(len);
            DVPRINT("MIToFDS: fds_type=>%s\n", fds_type);
            break;

        case T_VOID:
            // assume that void has a implicit length of 0
            // AIF_VOID type is confusing and poorly designed...
            fds_type = AIF_VOID_TYPE(0);
            DVPRINT("MIToFDS: fds_type=>%s\n", fds_type);
            break;

        case T_POINTER:
        case T_UNION:
        case T_ENUM:
        case T_ARRAY:
        case T_STRUCT:
        case T_CLASS:
        case T_FTNARRAY:
            /* FIXME */
        case T_UNKNOWN:
        default:
            DVPRINT("MIToFDS: unknown type.\n");
            break;
    }

    return fds_type;
}

static AIF *CreateSimpleAIF(int id, char *res, char *type_str, int type_len,
                            struct_info_t *type_struct,
                            func_info_t *type_func) {
    char *s1, *s2;
    char *t1, *t2;
    char *name;
    long double real, img;
    long long int addr_loc;
    void *addr;

    CSAPRINT("CreateSimpleAIF: %s\n", res);

    if (res == NULL) {
        return NULL;
    }
    switch (id) {
        case T_STRING:
            if (char* pch = strchr(res, ' ')) {
                pch++;
                CSAPRINT("CreateSimpleAIF: Creating string\n");
                return StringToAIF(pch);
            }
            // other type
            CSAPRINT("CreateSimpleAIF: Creating void\n");
            return VoidToAIF(0, 0);
        case T_CHAR:
            CSAPRINT("CreateSimpleAIF: Creating char\n");
            return CharToAIF((char)strtol(res, NULL, 10));
        case T_UCHAR:
            CSAPRINT("CreateSimpleAIF: Creating uchar\n");
            return UnsignedCharToAIF((unsigned char)strtol(res, NULL, 10));
        case T_SHORT:
            CSAPRINT("CreateSimpleAIF: Creating short\n");
            return ShortToAIF((short)strtol(res, NULL, 10));
        case T_USHORT:
            CSAPRINT("CreateSimpleAIF: Creating ushort\n");
            return UnsignedShortToAIF((unsigned short)strtoul(res, NULL, 10));
        case T_INT:
            CSAPRINT("CreateSimpleAIF: Creating int\n");
            return IntToAIF((int)strtol(res, NULL, 10));
        case T_UINT:
            CSAPRINT("CreateSimpleAIF: Creating uint\n");
            return UnsignedIntToAIF((unsigned int)strtoul(res, NULL, 10));
        case T_LONG:
            CSAPRINT("CreateSimpleAIF: Creating long\n");
            return LongToAIF(strtol(res, NULL, 10));
        case T_ULONG:
            CSAPRINT("CreateSimpleAIF: Creating ulong\n");
            return UnsignedLongToAIF((unsigned long)strtoul(res, NULL, 10));
#ifdef CC_HAS_LONG_LONG
        case T_LONGLONG:
            CSAPRINT("CreateSimpleAIF: Creating long long\n");
            return LongLongToAIF(strtoll(res, NULL, 10));
        case T_ULONGLONG:
            CSAPRINT("CreateSimpleAIF: Creating ulong long\n");
            return UnsignedLongLongToAIF((unsigned long long)strtoull(res, NULL, 10));
#endif
        case T_FLOAT:
            CSAPRINT("CreateSimpleAIF: Creating float\n");
            return FloatToAIF(strtof(res, NULL));
        case T_DOUBLE:
            CSAPRINT("CreateSimpleAIF: Creating double\n");
            return DoubleToAIF(strtod(res, NULL));
#ifdef HAVE_LONG_DOUBLE
        case T_LONGDOUBLE:
            CSAPRINT("CreateSimpleAIF: Creating long double\n");
            return LongDoubleToAIF(strtold(res, NULL));
#endif
        case T_BOOLEAN:
            CSAPRINT("CreateSimpleAIF: Creating bool\n");
            return BoolToAIF(strncmp(res, "true", 4) == 0 ? 1 : 0);
        case T_INTEGER_1:
            // gdb adds the actual char value so we need to get rid of that
            if (char* pch = strchr(res, ' ')) {
                pch++;
                if (*pch == '\'') {
                    pch--;
                    *pch = '\0';
                }
            }
            CSAPRINT("CreateSimpleAIF: Creating INTEGER1\n");
            return INTEGER1ToAIF((char)strtol(res, NULL, 10));
        case T_INTEGER_2:
            CSAPRINT("CreateSimpleAIF: Creating INTEGER2\n");
            return INTEGER2ToAIF((short)strtol(res, NULL, 10));
        case T_INTEGER_4:
            CSAPRINT("CreateSimpleAIF: Creating INTEGER4\n");
            return INTEGER4ToAIF((int)strtol(res, NULL, 10));
#ifdef CC_HAS_LONG_LONG
        case T_INTEGER_8:
            CSAPRINT("CreateSimpleAIF: Creating INTEGER8\n");
            return INTEGER8ToAIF(strtoll(res, NULL, 10));
#endif
        case T_REAL_4:
            CSAPRINT("CreateSimpleAIF: Creating REAL4\n");
            return REAL4ToAIF(strtof(res, NULL));
        case T_REAL_8:
            CSAPRINT("CreateSimpleAIF: Creating REAL8\n");
            return REAL8ToAIF(strtod(res, NULL));
#ifdef HAVE_LONG_DOUBLE
        case T_REAL_16:
            CSAPRINT("CreateSimpleAIF: Creating REAL16\n");
            return REAL16ToAIF(strtold(res, NULL));
#endif
        case T_COMPLEX_4:
            // ensure gdb gives us a sane result
            if (char* pch = strchr(res, ',')) { 
                // get rid of the final ')'
                res[strlen(res) - 1] = '\0';
                // get rid of the initial '('
                // res now holds the real part
                res++;
                // get rid of the ','
                *pch = '\0';
                // get the index to the complex part
                pch++;
                CSAPRINT("CreateSimpleAIF: Creating COMPLEX4\n");
                return COMPLEX4ToAIF(strtof(res, NULL), strtof(pch, NULL));
            } else {
                return VoidToAIF(0, 0);
            }
        case T_COMPLEX_8:
            // ensure gdb gives us a sane result
            if (char* pch = strchr(res, ',')) {
                // get rid of the final ')'
                res[strlen(res) - 1] = '\0';
                // get rid of the initial '('
                // res now holds the real part
                res++;
                // get rid of the ','
                *pch = '\0';
                // get the index to the complex part
                pch++;
                CSAPRINT("CreateSimpleAIF: Creating COMPLEX8\n");
                return COMPLEX8ToAIF(strtod(res, NULL), strtod(pch, NULL));
            } else {
                 return VoidToAIF(0, 0);
            }
#ifdef HAVE_LONG_DOUBLE
        case T_COMPLEX_16:
            // ensure gdb gives us a sane result
            if (char* pch = strchr(res, ',')) {
                // get rid of the final ')'
                res[strlen(res) - 1] = '\0';
                // get rid of the initial '('
                // res now holds the real part
                res++;
                // get rid of the ','
                *pch = '\0';
                // get the index to the complex part
                pch++;
                CSAPRINT("CreateSimpleAIF: Creating COMPLEX16\n");
                CSAPRINT("CreateSimpleAIF: real %s img %s\n", res, pch);
                real = strtold(res, NULL);
                img = strtold(pch, NULL);
                return COMPLEX16ToAIF(real, img);
            } else {
                return VoidToAIF(0, 0);
            }
#endif
        case T_LOGICAL_1:
            CSAPRINT("CreateSimpleAIF: Creating LOGICAL1\n");
            if (strncmp(res, ".TRUE.", 6) == 0) {
                return LOGICAL1ToAIF(1);
            } else {
                return LOGICAL1ToAIF(0);
            }
        case T_LOGICAL_2:
            CSAPRINT("CreateSimpleAIF: Creating LOGICAL2\n");
            if (strncmp(res, ".TRUE.", 6) == 0) {
                return LOGICAL2ToAIF(1);
            } else {
                return LOGICAL2ToAIF(0);
            }
        case T_LOGICAL_4:
            CSAPRINT("CreateSimpleAIF: Creating LOGICAL4\n");
            if (strncmp(res, ".TRUE.", 6) == 0) {
                return LOGICAL4ToAIF(1);
            } else {
                return LOGICAL4ToAIF(0);
            }
#ifdef CC_HAS_LONG_LONG
        case T_LOGICAL_8:
            CSAPRINT("CreateSimpleAIF: Creating LOGICAL8\n");
            if (strncmp(res, ".TRUE.", 6) == 0) {
                return LOGICAL8ToAIF(1);
            } else {
                return LOGICAL8ToAIF(0);
            }
#endif
        case T_CHARACTER:
            CSAPRINT("CreateSimpleAIF: Creating CHARACTER\n");
            // gdb adds in '...' around the string, we need to remove these
            if (*res == '\'') {
                ++res;
                // get rid of the final '
                if (char* pch = strrchr(res, '\'')) {
                    *pch = '\0';
                }
                CSAPRINT("CreateSimpleAIF: res is now \"%s\"\n", res);
            }
            return CHARACTERToAIF(res);

        case T_POINTER:
            CSAPRINT("CreateSimpleAIF: Creating pointer\n");
            if (type_str == NULL || type_len == 0) {
                CSAPRINT("CreateSimpleAIF: type_str NULL! Creating void\n");
                return VoidToAIF(0, 0);
            }
            CSAPRINT("1\n");
            addr_loc = strtoll(res, NULL, 0);
            CSAPRINT("2\n");
            addr = (void *)addr_loc;
            CSAPRINT("addr: $%p$\n", addr);
            CSAPRINT("type_len: $%d$\n", type_len);
            CSAPRINT("type_str: $%p$\n", type_str);
            return GenericPointerToAIF(AddressToAIF(addr, type_len), type_str);

        case T_FUNCTION: {
            AIF *ac;
            AIF *addr;
            long long int addr_loc;
            void *addr_ptr;
            char *name;

            CSAPRINT("CreateSimpleAIF: Creating function.\n");

            if (type_func == NULL) {
                DVPRINT("CreateSimpleAIF: type_func NULL! Creating void\n");
                return VoidToAIF(0, 0);
            }

            // first create an address of the res
            addr_loc = strtoll(res, NULL, 0);
            addr_ptr = (void *)addr_loc;
            addr = AddressToAIF(addr_ptr, type_func->getAddrLen());

            // get the function name
            name = GetFunctionName(res);

            // Now create the actual function pointer
            ac = FunctionToAIF(addr, name, type_func->getRtnType(), type_func->getArgc(),
                               type_func->getArgv());

            Free(name);
            AIFFree(addr);
            return ac;
        }

        case T_UNION:
            // NOTE: Unions in C are untagged, meaning that we have no intrinsic
            // language support to tell us what is currently placed inside the
            // union. We only know that one of the elements is allocated. To
            // deal with this, gdb will show the user the representation of each
            // field, with the assumption that all but the actual field
            // represents junk. In order to deal with this, we treat unions as
            // structs in this layer, that way the user will be able to see all
            // of the representations of what is currently placed in the union.
        case T_STRUCT: {
            // NOTE: This only works for c structures. Fortran types will need
            // to be added later.
            int index = 0;
            AIF *a;
            AIF *ac;

            CSAPRINT("CreateSimpleAIF: Creating struct\n");
            if (type_str == NULL) {
                DVPRINT("CreateSimpleAIF: type_str NULL! Creating void\n");
                return VoidToAIF(0, 0);
            }
            if (type_struct == NULL) {
                DVPRINT("CreateSimpleAIF: type_struct NULL! Creating void\n");
                return VoidToAIF(0, 0);
            }

            a = EmptyStructToAIF(type_struct->getName());
            char* pch = res;

            for (const auto& f : type_struct->getFields()) {
                // point at the fields "="
                if ((s1 = strchr(pch, '=')) == NULL) {
                    DVPRINT("CreateSimpleAIF: Could not find = in field.\n");
                    return VoidToAIF(0, 0);
                }
                // point n2 before the fields "="
                name = s1 - 1;
                // set this to null
                *name = '\0';
                // point to one before the next occurance of " " or '{'
                while (*(name - 1) != ' ' && *(name - 1) != '{') --name;

                // point past the fields "="
                ++s1;
                // if there is whitespace after the "=", ignore it.
                while (*s1 == ' ') {
                    ++s1;
                }
                // point s2 at the first occurance of ","
                if ((s2 = strchr(s1, ',')) == NULL) {
                    // if there is no "," then we are at the final entry, point
                    // at the closing bracket
                    if ((s2 = strchr(s1, '}')) == NULL) {
                        // not found, something is wrong...
                        DVPRINT(
                            "CreateSimpleAIF: Could not find final , or } in "
                            "field.\n");
                        return VoidToAIF(0, 0);
                    }
                    // ensure that there is not a opening bracket character,
                    // implying that s2 is pointing at the wrong bracket
                    if ((t1 = strchr(s1, '{')) != NULL) {
                        while (t1 < s2) {
                            // there is a opening bracket before the closing
                            // bracket that we found point t2 at the next
                            // bracket past s2
                            if ((t2 = strchr(++s2, '}')) == NULL) {
                                // not found, something is wrong...
                                DVPRINT(
                                    "CreateSimpleAIF: Detected no sep, found "
                                    "opening bracket, could not find second "
                                    "closing bracket.\n");
                                return VoidToAIF(0, 0);
                            }
                            s2 = t2;
                            if ((t2 = strchr(++t1, '{')) == NULL) {
                                // counld not find another opening bracket, so
                                // we must be done.
                                break;
                            }
                            t1 = t2;
                        }
                    }
                } else {
                    // ensure that there is not a '{' character, implying that
                    // s2 might be pointing at the wrong seperator.
                    if ((t1 = strchr(s1, '{')) != NULL) {
                        // if the opening bracket happens before the seperator,
                        // we need to point s2 at the closing bracket the first
                        // time through
                        if (t1 < s2) {
                            if ((s2 = strchr(t1, '}')) == NULL) {
                                // not found, something is wrong...
                                DVPRINT(
                                    "CreateSimpleAIF: Detected sep, found "
                                    "opening bracket, could not find first "
                                    "closing bracket.\n");
                                return VoidToAIF(0, 0);
                            }

                            // now check for another opening bracket, implying
                            // we are pointing at the wrong closer
                            if ((t1 = strchr(++t1, '{')) == NULL) {
                                // there are no more opening brackets, so we are
                                // done
                                t1 = s2;
                            }
                        }

                        // now loop through until we find the real closing
                        // bracket
                        while (t1 < s2) {
                            // there is an opening bracket before the sep that
                            // we found point t2 at the next bracket past s2
                            if ((t2 = strchr(++s2, '}')) == NULL) {
                                // not found, something is wrong...
                                DVPRINT(
                                    "CreateSimpleAIF: Detected sep, found "
                                    "opening bracket, could not find closing "
                                    "bracket.\n");
                                return VoidToAIF(0, 0);
                            }
                            s2 = t2;
                            if ((t2 = strchr(++t1, '{')) == NULL) {
                                // could not find another opening bracket, so
                                // we must be done.
                                break;
                            }
                            t1 = t2;
                        }

                        // need to point s2 past the closing bracket
                        ++s2;
                    }
                }

                // point pch past s2
                pch = s2 + 1;
                // set s2 to a null terminator
                *s2 = '\0';

                // now create a new AIF type from the res we just pieced out
                ac = CreateSimpleAIF(f.id, s1, f.type_str, f.type_len,
                                     f.inner_struct, f.type_func);

                CSAPRINT("CreateSimpleAIF: Adding AIF field for member %d\n", index);
                ++index;
                AIFAddFieldToStruct(a, strdup(name), ac);
                AIFFree(ac);
            }
            return a;
        }

        default:  // other type
            CSAPRINT("CreateSimpleAIF: Creating void\n");
            return VoidToAIF(0, 0);
    }
}

/*
** Create an AIF object that is a struct or class represented by the MIVar var.
*/
static AIFStream *GetStructAIF(MISession *session, MIVar *var) {
    DVPRINT("GetStructAIF: %s\n", var->name);

    // get the base type for this struct
    char* base_type = GetTypeName(var->type);
    DVPRINT("GetStructAIF: Base type: %s\n", base_type);

    AIF* a = EmptyStructToAIF(base_type);

    if (MIVarGetChildren(session,var)) {
        DVPRINT("MIVarGetChildren failed\n");
        return new AIFStream(a);
    }

    // The children of class objects are the visibility sections - as a first pass I will
    // make no attempt to capture these in the AIF
    for (int i=0; i<var->numchild; ++i) {
        auto child1 = var->children[i];
        if (child1->type == NULL) {
            // child1 is public|protected|private
            if (MIVarGetChildren(session,child1)) {
                continue;
            }
            DVPRINT("child %s has %d children\n", child1->name, child1->numchild);
            
            for (int j=0; j<child1->numchild; ++j) {
                auto child2 = child1->children[j];
                child2->expr = strdup(child2->name);

                std::shared_ptr<MIVar> sv(child2, [](MIVar*){});  
                if (AIFStream* ac = GetAIF(session, sv)) {
                    AIFAddFieldToStruct(a, child2->exp, ac->joinAIF());
                    delete ac;
                }
            }
        } else {
            std::shared_ptr<MIVar> sv(child1, [](MIVar*){});  
            if (AIFStream* ac = GetAIF(session, sv)) {
                AIFAddFieldToStruct(a, child1->exp, ac->joinAIF());
                delete ac;
            }
        }
    }

    return new AIFStream(a);
}


static struct_info_t *GetStructInfo(MISession *session, char *type_str) {
    MICommand *cmd;
    MIList *field_list;
    char *field;
    char *tmp_field = NULL;
    char *tmp;
    char *tmp2;
    char tmp_char;
    int id;
    int index;

    DVPRINT("GetStructInfo: %s\n", type_str);

    cmd = CLIPtype(type_str);
    SendMICommand(session, cmd);

    if (!MICommandResultOK(cmd)) {
        DVPRINT("GetStructInfo: CLIPtype call failed.\n");
        MICommandFree(cmd);
        return NULL;
    }

    field_list = CLIGetPtypeFieldList(cmd);
    MICommandFree(cmd);

    int numFields = MIListSize(field_list);
    if (!numFields) {
        DVPRINT("GetStructInfo: Zero fields detected.\n");
        MIListFree(field_list, NULL);
        return NULL;
    }

    // set the name of the struct
    const char* name = GetTypeName(type_str);
    struct_info_t* result = new struct_info_t(name, numFields);
    if (!result) {
        DVPRINT("allocating struct_info_t failed\n");
        return nullptr;
    }

    // Now fill in the information from the returned list
    index = 0;
    MIListSet(field_list);
    for (index = 0; (field = (char *)MIListGet(field_list)); ++index) {
        DVPRINT("GetStructInfo: field is %s\n", field);

        struct_info_t::field& sfield = result->getField(index);

        
        // The field contains the type including its name. For instance, field
        // will contain int i. We only want to point at the int part.
        // Note that we need to do special things for function pointers.
        if ((tmp = strrchr(field, ' ')) != NULL) {
            // ensure tmp+1 is not pointing at a '*', this is included in the
            // base type.
            while (*(tmp + 1) == '*') {
                ++tmp;
            }
            // one last time, need to point after the '*'
            if (*tmp == '*') {
                ++tmp;
            }

            // check to see if we are pointing at a named function pointer
            if (strncmp(tmp, "(*", 2) == 0 || strncmp(tmp, " (*", 3) == 0) {
                // we are pointing at a function pointer, we need to do
                // something special. start by strdup'ing the whole string
                tmp_field = strdup(field);
                // point at the final ' '
                tmp = strrchr(tmp_field, ' ');
                while (*(tmp + 1) == '*') {
                    ++tmp;
                }
                // one last time, need to point after the '*'
                if (*tmp == '*') {
                    ++tmp;
                }

                if (*tmp == ' ') ++tmp;

                // point past the "(*"
                tmp += 2;

                // point tmp2 at the closing ')'
                tmp2 = tmp;
                while (*tmp2 != ')') ++tmp2;

                // copy until null term
                while (*tmp2 != '\0') {
                    *tmp++ = *tmp2++;
                }
                *tmp = '\0';
                // strdup one last time for good measure, makes sure everything
                // is clean
                tmp2 = strdup(tmp_field);
                Free(tmp_field);
                tmp_field = tmp2;
                // Get the function info
                sfield.type_func = GetFunctionInfo(session, tmp_field);
            } else {
                // not a function pointer, proceed as usual
                tmp_char = *tmp;
                *tmp = '\0';
                tmp_field = strdup(field);
                // undo what we just did to the original field
                *tmp = tmp_char;
            }
        } else {
            tmp_field = strdup(field);
        }

        // use whatis to get the field type while its unknown
        while ((id = GetType(tmp_field)) == T_UNKNOWN) {
            tmp = GetMIWhatisValue(session, GetTypeName(field), NULL);
            if (tmp != NULL) {
                field = tmp;
                free(tmp_field);
                tmp_field = strdup(tmp);
            } else
                break;
        }

        sfield.id = id;
        sfield.type_str = strdup(tmp_field);
        sfield.type_len = GetTypeLength(session, tmp_field);

        DVPRINT("GetStructInfo: id is %d\n", sfield.id);
        DVPRINT("GetStructInfo: type_str is %s\n", sfield.type_str);
        DVPRINT("GetStructInfo: type_len is %d\n", sfield.type_len);

        // only need the struct_info_t stuff filled out if this is a struct
        if (id == T_STRUCT) {
            DVPRINT("GetStructInfo: Gathering inner struct info.\n");
            // recursively call this function with the field
            sfield.inner_struct = GetStructInfo(session, tmp_field);
        } else {
            sfield.inner_struct = nullptr;
        }

        DVPRINT("GetStructInfo: Done with field.\n");
    }

    // Remember to free the MIList and tmp_field.
    Free(tmp_field);
    MIListFree(field_list, (void (*)())Free);

    return result;
}

/*
** TODO: This function should not be used, it doesn't work properly. Since we
** have no way of knowing what is currently represented by the union (in C they
** are untagged) we cannot set the value of the union properly. Instead we
** should treat unions as structs in this layer.
*/
#if 0
static AIF *
GetUnionAIF(MISession *session, MIVar *var)
{
	MIVar *	v;
	AIF *	a;
	AIF *	ac;
	char *	base_type = NULL;
	int 	i;
	
	DVPRINT("GetUnionAIF: %s\n", var->name);
	
	base_type = GetTypeName(var->type);
	DVPRINT("GetUnionAIF: Base type: %s\n", base_type);
	
	a = EmptyUnionToAIF(base_type);
	
	// Resolve the child information for the var
	if (MIVarGetChildren(session, var))
	{
		AIFFree(a);
		return NULL;
	}

	DVPRINT("GetUnionAIF: Total number of children detected: %d\n", var->numchild);

	for (i=0; i < var->numchild; i++) 
	{
		DVPRINT("GetUnionAIF: Resolving member %d\n", i+1);
		v = var->children[i];
		// set the expr to the name of this member
		v->expr = strdup(v->name);
		if ((ac = GetAIF(session, v)) == NULL)
		{
			DVPRINT("GetUnionAIF: ERROR: Null aif object returned from GetAIF.\n");
			AIFFree(a);
			return NULL;
		}
		DVPRINT("GetUnionAIF: Adding AIF field for member %d.\n", i+1);
		if (AIFAddFieldToUnion(a, v->exp, AIF_FORMAT(ac)) < 0)
		{
			DVPRINT("GetUnionAIF: Could not add field to union.\n");
		}
		if (AIFSetUnion(a, v->exp, ac) < 0)
		{
			DVPRINT("GetUnionAIF: AIFError: %s\n", AIFErrorStr());
			DVPRINT("GetUnionAIF: ac field is %s\n", AIF_FORMAT(ac));
			DVPRINT("GetUnionAIF: a field is %s\n", AIF_FORMAT(a));
		}
		AIFFree(ac);
	}
	return a;
}
#endif

/* This function is used to hash the mivar befroe creating the AIF structure.
** This is to optimise the space required to build the immediate AIFArray
*structure
** since only the hashes that we are interested in
** Minh 17/10/2011
**/
static AIF *GetArrayAIFHash(MISession *session, MIVar *var, char *hashinfo,
                            int hashfunc, int precision_lower,
                            int precision_upper) {
    int id;
    char *buf;
    MICommand *cmd;
    arrayDimension_t *shape;
    AIF *a = NULL;
    AIF *ac;

    id = GetArrayBaseType(session, var, NULL, NULL, NULL, NULL);

    if (var->numchild <= 0)  // empty array
    {
        ac = CreateSimpleAIF(id, var->value, NULL, 0, NULL, NULL);
        a = EmptyArrayToAIF(0, 0, ac);
        AIFFree(ac);
        return a;
    } else {
        // first we need to get the data string
        DBEPERFLOG(SetTimerStart("GetArrayAIFHash: -data-evaluate-expression"));
        cmd = MIDataEvaluateExpression(var->expr);
        SendMICommand(session, cmd);
        if (!MICommandResultOK(cmd)) {
            if (MICommandResultClass(cmd) == MIResultRecordERROR) {
                if ((buf = MICommandResultErrorMessage(cmd)) != NULL) {
                    DVPRINT("GetArrayAIFHash: Debugger error: %s\n", buf);
                    EVENT_ERROR(dbg_error, DBGERR_DEBUGGER, buf);
                    free(buf);
                } else {
                    DVPRINT(
                        "GetArrayAIFHash: Error from GDB, but no message\n");
                    EVENT_ERROR(dbg_error, DBGERR_UNKNOWN,
                                "Error without message!");
                }
            } else {
                DVPRINT("GetArrayAIFHash: Bad response from GDB\n");
                EVENT_ERROR(dbg_error, DBGERR_UNKNOWN, "Bad response from GDB");
            }

            MICommandFree(cmd);

            return NULL;
        }
        DBEPERFLOG(
            SetTimerStopAndLog("GetArrayAIFHash: -data-evaluate-expression"));

        buf = MIGetDataEvaluateExpressionInfo(cmd);
        MICommandFree(cmd);
    }

    // Now lets get the shape of this array
    if ((shape = GetArrayDimensionShape(session, var)) ==
        (arrayDimension_t *)NULL) {
        // error already set
        DVPRINT("GetArrayAIFHash: GetArrayDimensionShape error\n");
        return NULL;
    }

    // we have the data string and the shape, we need to aply hashinfo to hash
    // the data
    a = PerformHashMI(buf, var->type, shape, hashinfo, hashfunc,
                      precision_lower, precision_upper);

    return a;
}

static char *GetArrayDataString(MISession *session, MIVar *var) {
    char *buf2;
    MICommand *cmd;
    char *buf;
    int last_resort = 0;
    DBEPERFLOG(SetTimerStart("GetArrayAIF: -data-evaluate-expression"));
    cmd = MIDataEvaluateExpression(var->expr);
    SendMICommand(session, cmd);
    while (!MICommandResultOK(cmd)) {
        if (MICommandResultClass(cmd) == MIResultRecordERROR) {
            if ((buf = MICommandResultErrorMessage(cmd)) != NULL) {
                DVPRINT("GetArrayAIF: Debugger error: %s\n", buf);
                EVENT_ERROR(dbg_error, DBGERR_DEBUGGER, buf);
                free(buf);
                // try to resolve the expression as a last resort and try again
                if (!last_resort) {
                    ++last_resort;
                    MICommandFree(cmd);
                    cmd = MIVarInfoPathExpression(var->expr);
                    SendMICommand(session, cmd);
                    if (MICommandResultOK(cmd)) {
                        // reset the error message
                        reset_err();
                        buf2 = MIGetVarInfoPathExpression(cmd);
                        MICommandFree(cmd);
                        cmd = MIDataEvaluateExpression(buf2);
                        SendMICommand(session, cmd);
                        free(buf2);
                        continue;
                    }
                }
            } else {
                DVPRINT("GetArrayAIF: Error from GDB, but no message\n");
                EVENT_ERROR(dbg_error, DBGERR_UNKNOWN,
                            "Error without message!");
            }
        } else {
            DVPRINT("GetArrayAIF: Bad response from GDB\n");
            EVENT_ERROR(dbg_error, DBGERR_UNKNOWN, "Bad response from GDB");
        }
        MICommandFree(cmd);

        return NULL;
    }
    DBEPERFLOG(SetTimerStopAndLog("GetArrayAIF: -data-evaluate-expression"));

    buf = MIGetDataEvaluateExpressionInfo(cmd);
    MICommandFree(cmd);
    return buf;
}

namespace {
class MIArrayToAIFParser {
public:
    // takes ownership of type_str, type_struct and type_func (but not shape)
    MIArrayToAIFParser(char* type_str, int type_len,
                       struct_info_t *type_struct, func_info_t *type_func,
                       int type_id, arrayDimension_t* shape);
    ~MIArrayToAIFParser();

    void parseSome(const char* str);

    AIF* getResult();

    // reset the parser to work on a new array - takes ownership of shape
    void setShape(arrayDimension_t* shape);
    
private:
    class Error {};
    
    AIF* createSimpleAIF();
    AIF* makeEmptyArrayAIF(int i, AIF* ac);
    void addArrayElement(int i, AIF* ac);

    void addElement();
    void openBracket(char bracket);
    void closeBracket(char bracket);
    void error(const std::string& message);
    
    char* m_type_str;     // type information from GetArrayBaseType
    int m_type_len;
    struct_info_t* m_type_struct;
    func_info_t* m_type_func;
    int m_type_id;
    arrayDimension_t* m_shape;  // dimension and index range of the array

    int m_dim;                  // dimension of the array
    std::vector<int> m_mins;    // minimum index in each direction (slowest to fastest)
    std::vector<int> m_maxs;    // maximum index in each direction (slowest to fastest)

    int m_depth;                // depth of current nesting (i.e. number of unclosed brackets)
    bool m_isC;                 // true if the data-type is a C/C++ array
    bool m_isFortran;           // true if the data-type is a fortran array
    bool m_hasError;            // true if the parser encountered a problem
    bool m_isFragment;          // true if the last block ended in the middle of an array
    std::vector<AIF*> m_abuild; // the multiple layers of the array m_abuild[0] is the outermost
    std::vector<int> m_idcs;    // the index of the next element to be filled in each dimension
    std::string m_element;      // the text of the current - potentially partial - element
};

// Define the type of object we are defining before we start partsing the actual
// text of the result.
// type_str, type_len, type_struct, type_func, and type_id collectively define the
// data type, while shape defined the dimension and indexing of the array.
MIArrayToAIFParser::MIArrayToAIFParser(char* type_str, int type_len,
                                       struct_info_t *type_struct,
                                       func_info_t *type_func,
                                       int type_id, arrayDimension_t* shape)
    : m_type_str(type_str), m_type_len(type_len), m_type_struct(type_struct),
      m_type_func(type_func), m_type_id(type_id), m_shape(shape ? dupArrayDimension(shape) : nullptr),
      m_dim(shape ? shape->dim : 0), m_mins(), m_maxs(),
      m_depth(0), m_isC(false), m_isFortran(false),
      m_hasError(false), m_isFragment(false), m_abuild(m_dim), m_idcs(m_dim, 0)
{
}

MIArrayToAIFParser::~MIArrayToAIFParser() {
    if (m_shape)
        freeArrayDimension(m_shape);
    delete m_type_struct;
    delete m_type_func;
}

// Translate the MI result text into AIF format.   \a str may contain either the
// data result, or a subset of the result.   In the case of a partitioned array
// it will concatenate adjacent array pieces.   That is "[0,1,2]" "[3,4,5]" will be
// understood as "[0,1,2,3,4,5]" Note that there is no comma between those arrays.
void MIArrayToAIFParser::parseSome(const char* str) {
    DVPRINT("In MIArrayToAIFParser::parseSome %s\n", str);

    if (m_hasError)
        return;

    try {
        if (!m_shape) {
            error("no array shape is specified\n");
        }
        
        for (const char* p = str; *p; ++p) {
            if (*p == '(' || *p == '{') {
                openBracket(*p);
            } else if (m_isFragment && !isspace(*p)) {
                error("insufficient data items in direction");
            } else if (*p == ')' || *p == '}') {
                closeBracket(*p);
            } else if (m_depth == m_dim && *p == ',') {
                addElement();
            } else if (!isspace(*p) || !m_element.empty()) {
                m_element += *p;
            }
        }
    } catch (Error&) {
        m_hasError = true;
    }

    DVPRINT("leaving MIArrayToAIFParser::parseSome m_hasError = %s\n", m_hasError ? "true" : "false");
}

// Handle a character that can be the beginniong of an array: '(' or '{' 
void MIArrayToAIFParser::openBracket(char bracket)
{
    if (bracket == '{' && !m_isFortran) {
        m_isC = true;
    } else if (bracket == '(' && !m_isC) {
        m_isFortran = true;
    } else {
        m_element += bracket;
        return;
    }

    if (m_depth >= m_dim) {
        m_element += bracket;
        ++m_depth;
        return;
    }

    if (m_mins.empty()) {
        // get the min and max elements the first time through.   Take care around
        // arrayDimension_t it is a bit of a goofy goober and used start-1 indexing, so
        // i=1, j=2, k=3, etc.   Also, the parser always orders from slowest changing to
        // fastest changing, so note the order reversal we do here for fortran.  (so we
        // don't need to mess with it again).   Actually - the dimension ordering in
        // arrayDimension_t is also perversely reversed, so reverse the logic again.
        for (int i=0; i<m_dim; ++i) {
            int d = (m_isFortran ? i+1 : m_dim-i);
            m_mins.push_back(getArrayDimensionRangeMin(m_shape,d));
            m_maxs.push_back(getArrayDimensionRangeMax(m_shape,d));
            
            DVPRINT("range in %d [%d,%d]\n", i, m_mins.back(), m_maxs.back());
        }
    }

    if (m_depth < m_dim && !m_isFragment)
        m_idcs[m_depth] = m_mins[m_depth];
    ++m_depth;
    m_isFragment = false;
    m_element.clear();
}

// Handle the potential end of an array
void MIArrayToAIFParser::closeBracket(char bracket)
{
    if ((bracket == '}' && !m_isC) ||
        (bracket == ')' && !m_isFortran)) {
        // the brackets don't match the language - treat like an ordinary character
        m_element += bracket;
    } else if (m_depth <= 0) {
        error("found a closing bracket with no matching open");
    } else if (m_depth > m_dim) {
        // this can happen with an array of structs or classes.   Pass into element
        // to give the element parser a crack at it.   We still need to update
        // the depth so we know when we've finished gettting the element.   We presume
        // the element has balanced brackets.
        m_element += bracket;
        --m_depth;
    } else {
        if (!m_element.empty())
            addElement();

        --m_depth;

        if (m_idcs[m_depth] != m_maxs[m_depth] + 1) {
            // with data chunking, we append adjacent arrays
            // e.g. {1,2,3,4,5,6} could arrive as {1,2,3}{4,5,6}.    If the next
            // non-space character is *not* an open bracket, the parsing will
            // generate a delayed error.
            m_isFragment = true;
        }
    }
}

// We have parsed through an array element, create the AIF and add it to the
// AIF structure.
void MIArrayToAIFParser::addElement()
{
    if (m_depth != m_dim) {
        error("trying to add an element at the wrong depth");
        return;
    }

    AIF* aif = createSimpleAIF();
    if (!aif)
        error("could not create element for " + m_element);

    m_element.clear();

    addArrayElement(m_depth-1, aif);
    AIFFree(aif);
}

// Create an empty array in direction \a i to contain type \a ac.
// i is ordered from slowest changing i=0 to fastest 
AIF* MIArrayToAIFParser::makeEmptyArrayAIF(int i, AIF* ac)
{
    AIF* aif = nullptr;
    if (m_isFortran) {
        aif = EmptyFtnArrayToAIF(m_mins[i], m_maxs[i], ac);
    } else {
        aif = EmptyArrayToAIF(m_mins[i], m_maxs[i], ac);
    }
    if (!aif)
        error("create empty AIF array failed");
    return aif;
}

// Add element \a ac to the current location in direction \a i.
void MIArrayToAIFParser::addArrayElement(int i, AIF* ac)
{
    if (m_idcs[i] > m_maxs[i]) {
        error("too many elements returned for direction");
        return;
    }

    if (m_idcs[i] == m_mins[i]) {
        m_abuild[i] = makeEmptyArrayAIF(i,ac);
    }

    if (m_isFortran) {
        AIFAddFtnArrayElement(m_abuild[i], m_idcs[i]++, ac);
    } else {
        AIFAddArrayElement(m_abuild[i], m_idcs[i]++, ac);
    }

    if (i && m_idcs[i] == m_maxs[i]+1) {
        addArrayElement(i-1, m_abuild[i]);
        AIFFree(m_abuild[i]);
        m_abuild[i] = nullptr;
    }
}

// Create a single element for the AIR result array
AIF* MIArrayToAIFParser::createSimpleAIF() {
    return CreateSimpleAIF(m_type_id, const_cast<char*>(m_element.c_str()),
                           m_type_str, m_type_len, m_type_struct, m_type_func);
}

// pull out the final result representing the entire array after all input
// has been parsed
AIF* MIArrayToAIFParser::getResult() {
    if (m_hasError)
        return nullptr;
    
    try {
        if (m_depth != 0)
            error("missing end bracket for array");
        else if (!m_abuild[0])
            error("expression ends prematurely");
        
    } catch (Error&) {}

    if (!m_hasError) {
        return m_abuild[0];
    }
    if (m_abuild[0])
        AIFFree(m_abuild[0]);
    return nullptr;
}

// reset the parser to work on a new array
void MIArrayToAIFParser::setShape(arrayDimension_t* shape)
{
    if (!shape) {
        DVPRINT("nullptr passed to AIFArrayToAIFParser::setShape\n");
        return;
    }

    arrayDimension_t* newShape = dupArrayDimension(shape);
    std::swap(newShape,m_shape);
    if (newShape)
        freeArrayDimension(newShape);

    // reset the processing state
    m_mins.clear();
    m_maxs.clear();
    m_dim = shape->dim;
    m_depth = 0;
    m_isC = m_isFortran = false;
    m_hasError = false;
    m_isFragment = false;
    m_abuild.resize(m_dim);
    fill_n(m_abuild.begin(), m_dim, nullptr);
    m_idcs.resize(m_dim);
    fill_n(m_idcs.begin(), m_dim, 0);
}

void MIArrayToAIFParser::error(const std::string& message) {
    m_hasError = true;
    DVPRINT("Parsing error %s\n", message.c_str());
    throw Error();
}

// Class to manage breaking the MI query for an array into smaller pieces.
// The calling sequence is simple:
// ArraySlice slicer(session, val, shape);
// while (char* buf = slicer.getNext()) {
//    ... process buf ...
// }
// The class is not omnicient, we currently only slice on the slowest changing
// dimension, so the actual block size could be larger than the nominal limit.
// Also, since we are plugging into the front end of gdb/mi (paging is disabled in this
// mode), we are limited to the set of types that can be split using a gdb expression.
class ArraySlicer {
public:
    ArraySlicer(MISession* session, std::shared_ptr<MIVar> var, arrayDimension_t* shape);
    ~ArraySlicer();

    // evaluate the next slice of the variable result.   Return the
    // result and the expected shape of the result.   The caller is
    // responsible for free the returned shape.
    char* getNext(arrayDimension_t** sliceShape, bool& isFinal);

    arrayDimension_t* getShape();
    const static size_t DEFAULT_BLOCK_SIZE = 1000000; // one million
    
private:
    void determineSlicing();
    arrayDimension_t* makeSubShape(long subRng[2]);

    ArraySlicer(const ArraySlicer&) = delete;
    ArraySlicer& operator=(const ArraySlicer&) = delete;    

    MISession* m_session;       // session to connect to gdb/MI
    std::shared_ptr<MIVar> m_var;  // MI variable of interest
    arrayDimension_t* m_shape;  // dimension and size of the array
    char* m_buf;                // allocated array for the result (contolled by this)
    char* m_array;              // interpretable region of m_buf

    size_t m_blockSize;         // maximum number of elements in each query
    std::string m_subExpr;      // encode expression to use for querying a chunk
    long m_subIdx;              // index for the current chunk
    long m_maxSubIdx;           // index for the highest chunk to return
    long m_slice;               // stride for the chunking index.
};

ArraySlicer::ArraySlicer(MISession* session, std::shared_ptr<MIVar> var,
                         arrayDimension_t* shape)
    : m_session(session), m_var(var), m_shape(shape ? dupArrayDimension(shape) : nullptr),
      m_buf(nullptr), m_array(nullptr), m_blockSize(DEFAULT_BLOCK_SIZE),
      m_subIdx(0), m_maxSubIdx(0), m_slice(0)
{
    static const char* blockSizeStr = ::getenv("GDB4HPC_BLOCK_ELEMENTS");
    if (blockSizeStr) {
        long lval = atol(blockSizeStr);
        if (lval > 0)
            m_blockSize = lval; 
    }

    if (m_blockSize) {
        DVPRINT("Slicing %s of type %s and dimension %d\n", var->expr, var->type,
                (!shape ? 0 : shape->dim));
        determineSlicing();
    }
}

ArraySlicer::~ArraySlicer()
{
    free(m_buf);
    if (m_shape)
        freeArrayDimension(m_shape);
}

// Paging and the like is unavailable in MI mode.   Instead, this class and this function
// specifically examines the data type and determines how it can be extracted in multiple
// data print calls that each pull out a portion of the data structure
void ArraySlicer::determineSlicing()
{
    if (!m_blockSize)
        return;
    
    std::string type(m_var->type);
    if (type.substr(0,12) == "std::vector<") {
        // C++ vector, we can treat this like an array if we can get at the size
        // and the first element.   We usually can, but gdb can get a little gimpy
        // in this area
        DVPRINT("Slicing: it is a vector");
        std::string sizeStr = std::string("(") + m_var->exp + ").size()";
        char* sizeRes = EvalExpression(m_session, sizeStr.c_str());
        if (!sizeRes)
            return;
        
        int size = atoi(sizeRes);
        DVPRINT("Slicing got a size %d\n", size);
        free(sizeRes);

        if (size <= ptrdiff_t(m_blockSize))
            return;
        
        // can I get the address of the first element?
        std::string frontStr = std::string("*&(") + m_var->expr + ")[0]";
        char* beginRes = EvalExpression(m_session, frontStr.c_str());
        if (!beginRes)
            return;

        DVPRINT("got begin address %s\n", beginRes);
        free(beginRes);

        m_shape = newArrayDimension();
        if (!m_shape) {
            DVPRINT("GetVectorLikeAIF: could not allocate shape\n");
            return;
        }
        addArrayDimensionDef(m_shape, size, 0, size-1);
        m_shape->isFortran = false;

        frontStr.erase(frontStr.end()-2, frontStr.end());
        frontStr += "$$from$$]@$$len$$";

        m_subExpr = frontStr;
        m_subIdx = 0;
        m_maxSubIdx = size-1;
        m_slice = m_blockSize;

        DVPRINT("Slicing vector %s %ld to %ld by %ld\n",
                m_subExpr.c_str(), m_subIdx, m_maxSubIdx, m_slice);

    } else if (m_shape && m_shape->isFortran) {
        // fortran array - we only try to split on the slowest changing dimension.
        // we can revisit if 100M x 2 arrays matter.
        int dim = m_shape->dim;
        size_t subArrayLen  = 1;
        for (int i=1; i<dim; ++i)
            subArrayLen *= getArrayDimension(m_shape,i);
        size_t numElems = subArrayLen * getArrayDimension(m_shape,dim);
        if (numElems <= m_blockSize)
            return;             // no need to divide

        std::ostringstream os;
        os << m_var->expr << "(";
        for (int i=1; i<dim; ++i) {
            int minIdx = getArrayDimensionRangeMin(m_shape,i);
            int maxIdx = getArrayDimensionRangeMax(m_shape,i);
            os << minIdx << ":" << maxIdx << ",";
        }
        os << "$$from$$:$$to$$)";

        m_subExpr = os.str();
        m_subIdx = getArrayDimensionRangeMin(m_shape,dim);
        m_maxSubIdx = getArrayDimensionRangeMax(m_shape,dim);
        m_slice = std::max(1, int(m_blockSize / subArrayLen));

        DVPRINT("Slicing: fortran type %s %ld to %ld by %ld\n",
                m_subExpr.c_str(), m_subIdx, m_maxSubIdx, m_slice);
    } else if (m_shape) {
        // C/C++ array, this may be real or partially artficial
        int dim = m_shape->dim;
        size_t subArrayLen  = 1;
        for (int i=2; i<=dim; ++i)
            subArrayLen *= getArrayDimension(m_shape,i);
        size_t numElems = subArrayLen * getArrayDimension(m_shape,1);
        if (numElems <= m_blockSize)
            return;             // no need to divide

        // we handle at least the simplest  artificial arrays
        m_subExpr = m_var->expr;
        size_t atLoc = m_subExpr.rfind("[0]@");
        if (atLoc != std::string::npos) {
            m_subExpr.erase(atLoc);
        }

        // transform into a series of artificial arrays for the sub-sets
        m_subExpr += "[$$from$$]@$$len$$";
        m_subIdx = 0;
        m_maxSubIdx = getArrayDimensionRangeMax(m_shape,1);
        m_slice = std::max(1, int(m_blockSize / subArrayLen));

        DVPRINT("Slicing: C array %s %ld to %ld by %ld\n",
                m_subExpr.c_str(), m_subIdx, m_maxSubIdx, m_slice);

    } else {
        DVPRINT("Slicing: no means to slice array\n");
    }
}

// Get the data shape, i.e. the dimensionality and range of the array indexing
// for the data type.  The function is entwined with this class because it
// needs understand the shape before it is sliced, and need to pass it along to
// the parsing after it is sliced
arrayDimension_t *ArraySlicer::getShape() {
    // in most important cases, ArraySlicer already has determined the shape
    // from the type.
    if (m_shape)
        return m_shape;

    // Though not always!   The remainder here is the fall-back position where
    // we could not cogitize the type information, so we just pull out the result
    // in a single glob and pull the shape information out of the result.
    m_buf = GetArrayDataString(m_session, m_var.get());

    // Now lets get the shape of this array
    int lengthTagLen = 6;
    char *lengthLoc = strstr(m_buf, "length"), *lengthEnd;

    char* withLoc = strstr(m_buf, "with");
    if (!lengthLoc || (withLoc && withLoc < lengthLoc)) {
        lengthLoc = withLoc;
        lengthTagLen = 4;
    }
    
    if (!lengthLoc ) {
        DVPRINT("GetVectorLikeAIF: could not find length\n");
        return nullptr;
    }
    lengthLoc += lengthTagLen;
    int length = strtol(lengthLoc, &lengthEnd, 10);
    if (lengthEnd == lengthLoc) {
        DVPRINT("GetVectorLikeAIF: could not parse length\n");
        return nullptr;
    }

    m_array = strchr(lengthEnd, '{');
    if (!m_array) {
        DVPRINT("GetVectorLikeAIF: could not find array in vector\n");
        return nullptr;
    }

    m_shape = newArrayDimension();
    if (!m_shape) {
        DVPRINT("GetVectorLikeAIF: could not allocate shape\n");
        return nullptr;
    }

    addArrayDimensionDef(m_shape, length, 0, length-1);
    m_shape->isFortran = false;

    return m_shape;
}

// Return either the next chunk of data, or nullptr if there is no more data to be
// had.
// The returned array is controlled by ArraySlicer and should not be freed by the
// caller.
char* ArraySlicer::getNext(arrayDimension_t** sliceShape, bool& isFinal)
{
    DVPRINT("In ArraySlicer::getNext\n");
    
    *sliceShape = nullptr;
    isFinal = true;
    
    if (m_subIdx > m_maxSubIdx)
        return nullptr;
    
    char* array = nullptr;
    if (m_array) {
        DVPRINT("In ArraySlicer::getNext no size information\n");
        // we were unable to get the size information for variable, so we
        // have already gotten the full data
        array = m_array;
        m_array = nullptr;
        ++m_subIdx;
    } else if (m_slice == 0 && !m_buf) {
        DVPRINT("In ArraySlicer::getNext not big enough to split\n");
        // splitting is not enabled, the array is not big enough to split,
        // or it's a form the splitter doesn't recognize, just return it
        // all
        array = m_buf = GetArrayDataString(m_session, m_var.get());
        ++m_subIdx;
    } else {
        DVPRINT("In ArraySlicer::getNext get next chunk\n");
        // get the next chunk of data
        std::swap(m_buf, array);
        if (array) {
            free(array);
            array = nullptr;
        }

        long maxIdx = std::min(m_maxSubIdx, m_subIdx+m_slice-1);
        long len = maxIdx - m_subIdx + 1;

        std::string subExpr = m_subExpr;
        std::string patterns[3] = { "$$from$$", "$$to$$", "$$len$$" };
        long vals[3] = { m_subIdx, maxIdx, len };
        for (int i=0; i<3; ++i) {
            size_t loc = subExpr.find(patterns[i]);
            if (loc != std::string::npos)
                subExpr.replace(loc, patterns[i].size(), std::to_string(vals[i]));
        }

        DVPRINT("Evaluating sub-expression %s\n", subExpr.c_str());
        
        array = EvalExpression(m_session,const_cast<char*>(subExpr.c_str()));
        if (array) {
            // success!
            DVPRINT("Succeeded getting sub-array value: %.400s\n", array);
            long subRng[2]= { m_subIdx, maxIdx };
            *sliceShape = makeSubShape(subRng);
            m_subIdx = maxIdx+1;
        } else {
            // failure!  - fall back to just evaluating the whole kielbasa
            array = GetArrayDataString(m_session, m_var.get());
            m_subIdx = m_maxSubIdx + 1;

            DVPRINT("Failed getting sub-array value, full result: %s\n", array);
        }
        m_buf = array;
    }

    DVPRINT("got an array value\n");
    if (array && !*sliceShape)
        *sliceShape = makeSubShape(nullptr);

    isFinal = (m_subIdx > m_maxSubIdx);

    DVPRINT("done makeSubShape\n");

    return array;
}

// Create a shape object corresponding to an array slice, or the full array if subRng is null.
// If subRng is specified it is the inclusive range of the outermost dimension of the arrray.
// Callers is responsible for freeing the returned object.
arrayDimension_t* ArraySlicer::makeSubShape(long subRng[2])
{
    arrayDimension_t* shape = getShape(); // size of the full array
    arrayDimension_t* subShape = newArrayDimension();
    subShape->isFortran = shape->isFortran;

    unsigned int dim = shape->dim;

    for (unsigned int i=1; i<=shape->dim; ++i) {
        int minIdx, maxIdx;
        if (subRng && ((shape->isFortran && i==dim) || (!shape->isFortran && i==1))) {
            minIdx = subRng[0];
            maxIdx = subRng[1];
        } else {
            minIdx = getArrayDimensionRangeMin(shape,i);
            maxIdx = getArrayDimensionRangeMax(shape,i);
        }
        addArrayDimensionDef(subShape, maxIdx-minIdx+1, minIdx, maxIdx);
    }

    return subShape;
}            

// Class for managing the extraction of an array data object via slices.   This object will
// be wrapped in an AIFStream, and ultimately allows us to query gdb/mi for slice data as the
// slices are ready to be consumed.   
class ArrayStream : public AIFStream::Slices {
public:
    // Constructor.   slicr and parser do the heavy lifting.
    ArrayStream(std::shared_ptr<ArraySlicer> slicer, std::shared_ptr<MIArrayToAIFParser> parser);

    // get the shape of the full array
    virtual arrayDimension_t* getFullShape() const override;

    // return the next partition of data
    virtual AIF* getNextSlice(bool& isFinal) override;

    // free an slice after it is done being processed
    virtual void freeSlice(AIF* aif) override;

private:
    std::shared_ptr<ArraySlicer> m_slicer;
    std::shared_ptr<MIArrayToAIFParser> m_parser;
};

ArrayStream::ArrayStream(std::shared_ptr<ArraySlicer> slicer, std::shared_ptr<MIArrayToAIFParser> parser)
    : m_slicer(slicer), m_parser(parser)
{}

// get the shape of the full array
arrayDimension_t* ArrayStream::getFullShape() const {
    return m_slicer->getShape();
}

// return the next partition of data
AIF* ArrayStream::getNextSlice(bool& isFinal) {
    DVPRINT("In ArrayStream::getNextSlice\n");
    arrayDimension_t* subShape = nullptr;
    AIF* res = nullptr;
    if (char* subResult = m_slicer->getNext(&subShape, isFinal)) {
        m_parser->setShape(subShape);
        freeArrayDimension(subShape);

        m_parser->parseSome(subResult);
        res = m_parser->getResult();
    }

    return res;
}

void ArrayStream::freeSlice(AIF* aif)
{
    if (aif)
        AIFFree(aif);
}
} // namespace


static AIFStream *GetArrayAIF(MISession *session, std::shared_ptr<MIVar> var) {
    int id;
    char *type_str;
    int type_len;
    struct_info_t *type_struct;
    func_info_t *type_func;
    DVPRINT("GetArrayAIF: %s\n", var->expr);

    if ((id = GetArrayBaseType(session, var.get(), &type_str, &type_len, &type_struct,
                               &type_func)) == T_UNKNOWN) {
        DVPRINT("GetArrayAIF: Unknown basetype for array!\n");
        return nullptr;
    }

    if (var->numchild <= 0) {
        DVPRINT("GetArrayAIF: No children detected.\n");
        AIF *ac = CreateSimpleAIF(id, var->value, type_str, type_len, type_struct,
                             type_func);
        AIF *a = EmptyArrayToAIF(0, 0, ac);
        AIFFree(ac);
        return new AIFStream(a);
    }
    
    DVPRINT("GetArrayAIF: %d children of type %d:%s(%d) detected.\n",
            var->numchild, id, type_str, type_len);

    // Get the shape of this array
    arrayDimension_t *shape = GetArrayDimensionShape(session, var.get());
    if (!shape) {
        // error already set
        DVPRINT("GetArrayAIF: GetArrayDimensionShape error\n");
        return nullptr;
    }

    // The array data could be enormous, so instead of getting it now, we just return a
    // data stream object which can access it in pieces
    auto slicer = std::make_shared<ArraySlicer>(session,var,shape);
    auto parser = std::make_shared<MIArrayToAIFParser>(type_str, type_len, type_struct,
                                                       type_func, id, shape);
    auto arrayStream = std::make_shared<ArrayStream>(slicer,parser);

    freeArrayDimension(shape);

    return new AIFStream(arrayStream);

}

static AIFStream *GetVectorLikeAIF(MISession *session, std::shared_ptr<MIVar> var) {
    DVPRINT("GetArrayAIF: %s\n", var->expr);

    char *type_str; int type_len; struct_info_t *type_struct; func_info_t *type_func;
    int id = GetArrayBaseType(session, var.get(), &type_str, &type_len, &type_struct, &type_func);

    if (id == T_UNKNOWN) {
        DVPRINT("GetArrayAIF: Unknown basetype for array!\n");
        return nullptr;
    }
    DVPRINT("id: %d\n", id);

    if (var->numchild <= 0) {
        DVPRINT("GetArrayAIF: No children detected.\n");
        AIF* ac = CreateSimpleAIF(id, var->value, type_str, type_len, type_struct,
                                  type_func);
        AIF* a = EmptyArrayToAIF(0, 0, ac);
        AIFFree(ac);
        return new AIFStream(a);
    } 

    DVPRINT("Slicing (vector-like) %s of type %s and dimension one\n", var->expr, var->type);

    DVPRINT("GetVectorLikeArrayAIF: %d children detected.\n", var->numchild);

    // The vector data could be enormous, so instead of getting it now, we just create an 
    // AIFStream which can access the data as it is needed.

    auto slicer = std::make_shared<ArraySlicer>(session,var,nullptr);

    arrayDimension_t *shape = slicer->getShape();                                                    
    if (!shape) {                                                                                   
        DVPRINT("could not get size of a vector like object");                                      
        return nullptr;                                                                             
    }                                                                                               
    
    auto parser = std::make_shared<MIArrayToAIFParser>(type_str, type_len, type_struct,
                                                       type_func, id, shape);
    auto arrayStream = std::make_shared<ArrayStream>(slicer,parser);

    return new AIFStream(arrayStream);
}

static AIFStream *GetPointerAIF(MISession *session, MIVar *var) {
    DVPRINT("GetPointerAIF: Pointer Type => %s\n", var->type);

    // get the address for the pointer, this is represented by the value
    // in the MI object.
    char *addr_str = GetVarValue(session, var->name);
    if (!addr_str) {
        // error already set
        return nullptr;
    }

    DVPRINT("GetPointerAIF: Pointer Addr => %s\n", addr_str);

    // convert the address of the pointer to an integer value
    long long int addr_loc = strtoll(addr_str, NULL, 0);

    // Make the void pointer point at the memory location of the pointer
    void* addr = (void *)addr_loc;

    // We need to ask for the length of the address type for this particular
    // pointer.
    int addr_len = GetTypeLength(session, var->type);

    DVPRINT("GetPointerAIF: Pointer Addr Length => %d\n", addr_len);

    // Create the AIF type representing the address
    AIF* addr_aif = AddressToAIF(addr, addr_len);

    // Now create the AIF generic pointer representing both the address that
    // we actually point to and the type string.
    AIF *rtn_aif = GenericPointerToAIF(addr_aif, var->type);

    AIFFree(addr_aif);

    return new AIFStream(rtn_aif);
}
