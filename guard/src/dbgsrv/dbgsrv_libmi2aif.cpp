/******************************************************************************\
 * libmi to aif conversion routines
 *
 * © 2011-2019 Cray Inc.	All Rights Reserved.
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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "defs.h"
#include "useful.h"
#include "dbgsrv_libmi2aif.h"
#include "dbgsrv_util.h"
#include "dbgsrv_defs.h"
#include "dbgfn.h"
#include "dbgevent.h"

/* The following is used to turn on debug printing for the CreateSimpleAIF func.
** It generates a ton of output for large arrays, which is why we want to
** normally leave it off.
*/
#define CreateSimpleAIF_PRINT

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
typedef struct func_info
{
	int		addr_len;
	char *	rtn_type;
	int		argc;
	char **	argv;
} func_info_t;

typedef struct struct_info
{
	char *					name;
	int 					numFields;
	// The following are arrays of size numFields
	int *					id;
	char **					type_str;
	int *					type_len;
	struct func_info **		type_func;
	struct struct_info **	inner_struct;
} struct_info_t;

// external prototypes
extern void			 		SendMICommand(MISession *, MICommand *);

/*
** Static prototypes
*/
static MIVar *				CreateMIVar(MISession *, char *);
static void					DeleteMIVar(MISession *, char *);
static int					MIVarGetChildren(MISession *, MIVar *);
static char *				GetMIWhatisValue(MISession *, char *, MIVar *);
static int					GetTypeLength(MISession *, char *);
static int					GetSimpleType(char *);
static int					GetComplexType(char *);
static int					GetType(char *);
static int					GetArrayBaseType(MISession *, MIVar *, char **, int *, struct_info_t **, func_info_t **);
static arrayDimension_t *	GetArrayDimensionShape(MISession *, MIVar *);
static char *				GetTypeName(char *);
static void					RemoveWhitespace(char *);
static func_info_t *		GetFunctionInfo(MISession *, char *);
static int					GetFunctionArguments(MISession *, char *, char **[]);
static char *				GetFunctionReturnType(MISession *session, char *);
static AIF *				GetFunctionAddress(MISession *, char *);
static char *				GetFunctionName(char *);
static AIF *				CreateSimpleAIF(int, char *, char *, int, struct_info_t *, func_info_t *);
static char *				MIToFDS(MISession *, int, char *);
static char *				GetVarValue(MISession *, char *);
static AIF *				GetAIF(MISession *, MIVar *);
static AIF *				GetSimpleAIF(MISession *, MIVar *);
static AIF *				GetComplexAIF(MISession *, MIVar *);
static struct_info_t *		GetStructInfo(MISession *, char *);
static AIF *				GetStructAIF(MISession *, MIVar *);
//static AIF *				GetUnionAIF(MISession *, MIVar *);
static AIF *				GetArrayAIF(MISession *, MIVar *);
static AIF *                GetVectorLikeAIF(MISession *, MIVar *);
static AIF *                GetVectorAIF(MISession *, MIVar *);
static AIF *                GetDequeAIF(MISession *, MIVar *);
static AIF *                GetStackAIF(MISession *, MIVar *);
static AIF *                GetPQueueAIF(MISession *, MIVar *);
static AIF *                GetQueueAIF(MISession *, MIVar *);
static AIF *				GetArrayAIFHash(MISession *, MIVar *, char *, int, int, int);
static AIF *				GetPointerAIF(MISession *, MIVar *);
static char * 				GetArrayDataString(MISession *, MIVar *);
//static arrayDimension_t *   oneDArray(int, int, int);
static vectorLikeDimension_t * 	GetVectorLikeDimensionShape(char *);
static void					fixTypeString(char *, const char *, char *);

const char * _aif_ignored_type_specifiers[] = {
	"const",
	"restrict",
	NULL
};


// Internal globals
static dbgevent_t *	dbg_error = NULL;

void printArray(unsigned int * arr, int size){
	fprintf(stderr, "[");
	int i;
	for(i=0; i<size; i++){
		if(i != size-1)
			fprintf(stderr, "%d, ", arr[i]);
	}
	fprintf(stderr, "]");

}

static void
reset_err(void)
{
	if (dbg_error != NULL)
	{
		FreeEvent(dbg_error);
	}
	dbg_error = NULL;
}

static dbgevent_t *
get_err(void)
{
	dbgevent_t *	rtn = NULL;

	if (dbg_error != NULL)
	{
		rtn = dbg_error;
		dbg_error = NULL;
	}
	
	return rtn;
}

// This is the "main" function that should be called in this interface...
// It returns NULL on success, otherwise it returns an error event.
dbgevent_t *
GetAIFVar(MISession *session, char *var, AIF **val, char **type)
{
	AIF * res;
	MIVar *mivar;

	// reset the error var
	reset_err();

	DVPRINT("GetAIFVar: Resolving variable %s.\n", var);

	mivar = CreateMIVar(session, var);
	
	if (mivar == NULL) 
	{
		DVPRINT("GetAIFVar: CreateMIVar returned null.\n");
		if (dbg_error == NULL)
		{
			EVENT_ERROR(dbg_error, DBGERR_INTERNAL, "CreateMIVar failed.");
		}
		return get_err();
	}

	if ((res = GetAIF(session, mivar)) == NULL) 
	{
		DVPRINT("GetAIFVar: GetAIF returned null.\n");
		if (dbg_error == NULL)
		{
			EVENT_ERROR(dbg_error, DBGERR_INTERNAL, "Parser failed to interpret gdb variable.");
		}
		DeleteMIVar(session, mivar->name);
		MIVarFree(mivar);
		return get_err();
	}
	*type = strdup(mivar->type);
	*val = res;

	DeleteMIVar(session, mivar->name);
	MIVarFree(mivar);

	return NULL;
}

// TODO: Merge this with the above somehow?
dbgevent_t *
GetAIFVarHash(MISession *session, char *var, AIF **val, char *hashinfo,
		int hashfunc, int precision_lower, int precision_upper)
{
	AIF *	res;
	AIF *	a = NULL;
	MIVar *	mivar;
	int		id;
	char *	pt;
	int		cnt = 0;
	
	// reset the error var
	reset_err();

	mivar = CreateMIVar(session, var);
	
	if (mivar == NULL) 
	{
		DVPRINT("GetAIFVarHash: Unknown variable\n");
		if (dbg_error == NULL)
		{
			EVENT_ERROR(dbg_error, DBGERR_INTERNAL, "CreateMIVar failed.");
		}
		return get_err();
	}

	if (strcmp(mivar->type, "<text variable, no debug info>") == 0) 
	{
		DVPRINT("GetAIFVarHash: No symbols\n");
		EVENT_ERROR(dbg_error, DBGERR_NOSYMS, NULL);
		return get_err();
	}

	while ((id = GetComplexType(mivar->type)) == T_UNKNOWN)
	{
		pt = GetMIWhatisValue(session, NULL, mivar);
		if (pt != NULL)
		{
			if (mivar->type != NULL)
			{
				free(mivar->type);
			}
			mivar->type = pt;
			if (++cnt >= LOOP_PREVENTION)
			{
				EVENT_ERROR(dbg_error, DBGERR_INTERNAL, "GetArrayAIFHash failed.");
				return get_err();
			}
		} else
			break;
	}

	if ( mivar->numchild==0 || !(id==T_ARRAY || id==T_FTNARRAY) )
	{
		//simple type
		a = GetAIF(session, mivar);
		res	= PerformHash(a, hashinfo, hashfunc, precision_lower, precision_upper);
	}
	else
	{
		// of type array
		if((res = GetArrayAIFHash(session, mivar, hashinfo, hashfunc, precision_lower, precision_upper)) == NULL)
		{
			DVPRINT("GetAIFVarHash: Unknown type\n");
			DeleteMIVar(session, mivar->name);
			MIVarFree(mivar);
			if (dbg_error == NULL)
			{
				EVENT_ERROR(dbg_error, DBGERR_INTERNAL, "GetArrayAIFHash failed.");
			}
			return get_err();
		}
	}

	*val = res;

	DeleteMIVar(session, mivar->name);
	MIVarFree(mivar);

	return 0;
}

/*
 * Create a MI variable in gdb and get its MIVar type.
 * Returns null on error.
 */
static MIVar *
CreateMIVar(MISession *session, char *expr)
{
	MICommand *	cmd;
	MIVar *		var;
	char *		p;

	cmd = MIVarCreate("-", "*", expr);
	SendMICommand(session, cmd);
	// If this fails, it means the symbol doesn't exist.
	if (!MICommandResultOK(cmd)) 
	{
		EVENT_ERROR(dbg_error, DBGERR_NOSYM, expr);
		MICommandFree(cmd);
		return NULL;
	}
	
	var = MIGetVarCreateInfo(cmd);
	MICommandFree(cmd);
	
	if (var->exp == NULL) {
		var->exp = strdup(expr);
	}
	
	var->expr = strdup(expr);
	
	// remove newline
	if (var->expr[strlen(expr)-1] == '\n')
	{
		var->expr[strlen(expr)-1] = '\0';
	}
	
	// Remove whitespace and the final '&' for reference types.
	// ptype breaks with '&'.
	p = &(var->type)[strlen(var->type)-1];
	
	while ((p >= var->type) && isspace(*p))
	{
		*p-- = '\0';
	}
	
	if (*p == '&')
	{
		*p = '\0';
	}
	
	return var;
}

static void
DeleteMIVar(MISession *session, char *name)
{
	MICommand *cmd;
	
	cmd = MIVarDelete(name);
	SendMICommand(session, cmd);
	MICommandFree(cmd);
}

static int
MIVarGetChildren(MISession *session, MIVar *var)
{
	MICommand		*cmd;
	
	if (session == NULL || var == NULL)
		return 1;
		
	cmd = MIVarListChildren(var->name);
	SendMICommand(session, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		if (MICommandResultClass(cmd) == MIResultRecordERROR) 
		{
			char *err = MICommandResultErrorMessage(cmd);
			if (err != NULL) 
			{
				DVPRINT("MIVarGetChildren: Debugger error: %s\n", err);
				EVENT_ERROR(dbg_error, DBGERR_DEBUGGER, err);
				free(err);
			} else 
			{
				DVPRINT("MIVarGetChildren: Error from GDB, no error message.\n");
				EVENT_ERROR(dbg_error, DBGERR_UNKNOWN, "Error without message!");
			}
		} else 
		{
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
static char *
GetMIWhatisValue(MISession *session, char *expr, MIVar *var)
{
	MICommand * cmd;
	char *		type;

	DVPRINT("GetMIWhatisValue: (%s, %s)\n", expr != NULL ? expr : "NULL", var != NULL ? var->type : "NULL");
	
	if (var != NULL)
	{
		cmd = CLIWhatis(var->type);
		SendMICommand(session, cmd);
		if (!MICommandResultOK(cmd))
		{
			if (expr == NULL)
			{
				DVPRINT("CLIWhatis failed and NULL expr\n");
				MICommandFree(cmd);
				return NULL;
			}
		
			// As a last resort, try to get the whatis based on the provided expr
			cmd = CLIWhatis(expr);
			SendMICommand(session, cmd);
			if (!MICommandResultOK(cmd))
			{
				DVPRINT("CLIWhatis failed for both\n");
				MICommandFree(cmd);
				return NULL;
			}
		}
	} else if (expr != NULL)
	{
		cmd = CLIWhatis(expr);
		SendMICommand(session, cmd);
		if (!MICommandResultOK(cmd))
		{
			DVPRINT("CLIWhatis failed for expr\n");
			MICommandFree(cmd);
			return NULL;
		}
	} else
	{
		DVPRINT("CLIWhatis both expr and var are NULL!\n");
		return NULL;
	}
	
	type = CLIGetWhatisInfo(cmd);
	
	DVPRINT("Type is \"%s\"\n", type);
	
	MICommandFree(cmd);
	
	return type;
}

static void
fixTypeString(char *p, const char *s, char *orig)
{
	int	extra_char = 0;	// used for extra characters to account for

	if (p == NULL || s == NULL || orig == NULL)
	{
		DVPRINT("fixTypeString: bad args\n");
		return;
	}
	
	DVPRINT("fixTypeString: p is \"%s\"\n", p);
	DVPRINT("fixTypeString: s is \"%s\"\n", s);

	// If this is the last stuff in the string, we simply null terminate.
	if (strlen(p) == strlen(s))
	{
		DVPRINT("fixTypeString: null terminate method.\n");
		// Check for a preceeding " "
		if (*(p-1) == ' ')
		{
			*--p = '\0';
		} else				
		{
			*p = '\0';
		}
	} else
	{
		DVPRINT("fixTypeString: memmove method.\n");
		// If we are not at the start of the string, there might be a
		// preceeding space to deal with
		if (p > orig)
		{
			// check for a preceeding " "
			if (*(p-1) == ' ')
			{
				// decrement p to pick up the space
				--p;
				++extra_char;
			}
		}
		
		// Check for space after string
		if (*(p+strlen(s)) == ' ')
		{
			// point past the ' ' character
			memmove(p, p+(strlen(s)+extra_char+1), (strlen(p)+1)-(strlen(s)+extra_char+1));
		} else
		{
			// no ' ' character
			memmove(p, p+strlen(s)+extra_char, (strlen(p)+1)-(strlen(s)+extra_char));
		}
	}
	
	DVPRINT("fixTypeString: New string is \"%s\"\n", p);
}

static char* sanitize_type_string(char* input){
	// first we need to sanitize the stupid string if gpu debugging is turned on
	// I don't know why Nvidia decided to be dumb by putting the address space
	// idents in but then making the gdb parser not understand them...
	char* clean_str;
	char* p;
	int found;
	clean_str = strdup(input);
	
	if (gpu_flag)
	{
		while ((p = strchr(clean_str, '@')) != NULL)
		{
			// reset found
			found = 0;
			if (NV_CHECK_ADDR_IDENT(p, CODE_S))
			{
				DVPRINT("GetTypeLength: Found %s address space identifier\n", CODE_S);
				++found;
				fixTypeString(p, CODE_S, clean_str);
			}
			if (NV_CHECK_ADDR_IDENT(p, CONSTANT_S))
			{
				DVPRINT("GetTypeLength: Found %s address space identifier\n", CONSTANT_S);
				++found;
				fixTypeString(p, CONSTANT_S, clean_str);
			}
			if (NV_CHECK_ADDR_IDENT(p, GENERIC_S))
			{
				DVPRINT("GetTypeLength: Found %s address space identifier\n", GENERIC_S);
				++found;
				fixTypeString(p, GENERIC_S, clean_str);
			}
			if (NV_CHECK_ADDR_IDENT(p, GLOBAL_S))
			{
				DVPRINT("GetTypeLength: Found %s address space identifier\n", GLOBAL_S);
				++found;
				fixTypeString(p, GLOBAL_S, clean_str);
			}
			if (NV_CHECK_ADDR_IDENT(p, PARAMETER_S))
			{
				DVPRINT("GetTypeLength: Found %s address space identifier\n", PARAMETER_S);
				++found;
				fixTypeString(p, PARAMETER_S, clean_str);
			}
			if (NV_CHECK_ADDR_IDENT(p, SHARED_S))
			{
				DVPRINT("GetTypeLength: Found %s address space identifier\n", SHARED_S);
				++found;
				fixTypeString(p, SHARED_S, clean_str);
			}
			if (NV_CHECK_ADDR_IDENT(p, TEXTURE_S))
			{
				DVPRINT("GetTypeLength: Found %s address space identifier\n", TEXTURE_S);
				++found;
				fixTypeString(p, TEXTURE_S, clean_str);
			}
			if (NV_CHECK_ADDR_IDENT(p, LOCAL_S))
			{
				DVPRINT("GetTypeLength: Found %s address space identifier\n", LOCAL_S);
				++found;
				fixTypeString(p, LOCAL_S, clean_str);
			}
			if (NV_CHECK_ADDR_IDENT(p, REGISTER_S))
			{
				DVPRINT("GetTypeLength: Found %s address space identifier\n", REGISTER_S);
				++found;
				fixTypeString(p, REGISTER_S, clean_str);
			}
			
			// ensure found triggered, otherwise just break to avoid infinite loop
			// this will cause an error later on though.
			if (!found)
			{
				DVPRINT("GetTypeLength: Avoiding infinite loop!\n");
				break;
			}
		}
	}

	const char** current = _aif_ignored_type_specifiers;

	while(*current != NULL){
		while ((p = strstr(clean_str, *current)) != NULL){
			fixTypeString(p, *current, clean_str);
		}
		current++;
	}

	return clean_str;
}

static int
GetTypeLength(MISession *session, char *base_type)
{
	char *		res;
	MICommand *	cmd;
	char *		buf;
	int			type_len;		// in bytes
	char *		p;
	char *		clean_str;		// string without nvidia crap
	int			found;
	
	DVPRINT("GetTypeLength: Resolving length of %s\n", base_type);
	
	// sanity
	if (base_type == NULL)
		return 0;
		
	base_type = sanitize_type_string(base_type);
		
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
	if (gpu_flag)
	{
		free(base_type);
	}
	
	return type_len;
}

static char *
GetVarValue(MISession *session, char *var)
{
	char *		res;
	MICommand *	cmd;

	DVPRINT("GetVarValue: Evaluate %s.\n", var);

	DBEPERFLOG(SetTimerStart("GetVarValue: -data-evaluate-expression"));
	cmd = MIVarEvaluateExpression(var);
	DBEPERFLOG(SetTimerStopAndLog("GetVarValue: -data-evaluate-expression"));

	SendMICommand(session, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		DVPRINT("GetVarValue: Error\n");
		EVENT_ERROR(dbg_error, DBGERR_INTERNAL, "GetAIF failed.");
		MICommandFree(cmd);
		return NULL;
	}
	res = MIGetVarEvaluateExpressionInfo(cmd);
	MICommandFree(cmd);
	
	// Check to see if the value was optimized out
	if (strcmp(res, "<value optimized out>") == 0)
	{
		EVENT_ERROR(dbg_error, DBGERR_OPT, NULL);
		free(res);
		return NULL;
	}
	
	return res;
}

static AIF *
GetAIF(MISession *session, MIVar *var)
{
	DVPRINT("GetAIF: %s\n", var->expr);

	if (strcmp(var->type, "<text variable, no debug info>") == 0) {
		DVPRINT("GetAIF: No symbols\n");
		EVENT_ERROR(dbg_error, DBGERR_NOSYMS, NULL);
		return NULL;
	}
	if (var->numchild == 0) 
	{ 
		//simple type
		DVPRINT("GetAIF: Resolved to simple type.\n");
		return GetSimpleAIF(session, var);
	}
	
	//complex type
	DVPRINT("GetAIF: Resolved to complex type.\n");
	return GetComplexAIF(session, var);
}

static AIF *
GetSimpleAIF(MISession *session, MIVar *var)
{
	int	 id;
	AIF *	a = NULL;
	AIF *	ac;
	AIF *	addr;
	char *	pt;
	char *	v;
	char ** argv;
	int	 argc;
	char *	rtn;
	char *	name;
	int		cnt = 0;

	DVPRINT("GetSimpleAIF: %s\n", var->type);
	
	while ((id = GetSimpleType(var->type)) == T_UNKNOWN)
	{
		// default to using whatis
		pt = GetMIWhatisValue(session, NULL, var);
		if (pt != NULL) 
		{
			if (var->type != NULL) 
			{
				free(var->type);
			}
			var->type = pt;
			if (++cnt >= LOOP_PREVENTION)
			{
				break;
			}
		} else
			break;
	}
	
	switch (id) 
	{
	
		case T_FUNCTION:
			DVPRINT("GetSimpleAIF: Creating AIF function.\n");			
			argc = GetFunctionArguments(session, var->type, &argv);
			if (debug_flag)
			{
				int i;
				DVPRINT("GetSimpleAIF: Num args: %d\n", argc);
				for (i=0; i<argc; ++i)
				{
					DVPRINT("GetSimpleAIF: argv[%d]=\"%s\"\n", i, argv[i]);
				}
			}
			rtn = GetFunctionReturnType(session, var->type);
			DVPRINT("GetSimpleAIF: Function return type: %s\n", rtn);
			if ((v = GetVarValue(session, var->name)) == NULL)
			{
				// error already set
				free(rtn);
				return NULL;
			}
			name = GetFunctionName(v);
			DVPRINT("GetSimpleAIF: Function name: %s\n", name);
			addr = GetFunctionAddress(session, v);
			if (debug_flag)
			{
				char *aif_str;
				AIFToStr(&aif_str, 0, addr);
				DVPRINT("GetSimpleAIF: Function address: %s func_name %s func_rtn_fmt %p argc %d\n", aif_str, name, rtn, argc);
				Free(aif_str);
			}
			ac = FunctionToAIF(addr, name, rtn, argc, argv);
			if (debug_flag)
			{
				char *aif_str;
				AIFToStr(&aif_str, 0, ac);
				DVPRINT("GetSimpleAIF: FDS string is: %s\n", AIF_FORMAT(ac));
				DVPRINT("GetSimpleAIF: Function AIF size is: %d\n", AIF_LEN(ac));
				DVPRINT("GetSimpleAIF: Function AIF type is: %s\n", aif_str);
				Free(aif_str);
			}
			free(v);
			free(rtn);
			free(name);
			AIFFree(addr);
			return ac;
		
		case T_ENUM:
			DVPRINT("GetSimpleAIF: Creating AIF enum.\n");
			return EmptyEnumToAIF(GetTypeName(var->type));
			
		case T_VOID_PTR:
			// void ptr's don't have any children and are considered simple types
			DVPRINT("GetComplexAIF: Creating AIF void pointer.\n");
			a = GetPointerAIF(session, var);
			return a;
		
		case T_UNKNOWN:
			// Unrecognized type.
			DVPRINT("GetSimpleAIF: Unknown type. Returning error.\n");
			EVENT_ERROR(dbg_error, DBGERR_BADTYPE, NULL);
			return NULL;
		
		default:
			DVPRINT("GetSimpleAIF: Creating simple type.\n");
			if ((v = GetVarValue(session, var->name)) == NULL)
			{
				// error already set
				return NULL;
			}
			a = CreateSimpleAIF(id, v, NULL, 0, NULL, NULL);
			free(v);
			return a;
	}
}

static AIF *
GetComplexAIF(MISession *session, MIVar *var)
{
	int	 	id;
	char *	pt;
	AIF *	a = NULL;
	int		cnt = 0;

	while ((id = GetComplexType(var->type)) == T_UNKNOWN) 
	{
		pt = GetMIWhatisValue(session, NULL, var);
		if (pt != NULL) 
		{
			if (var->type != NULL) 
			{
				free(var->type);
			}
			var->type = pt;
			if (++cnt >= LOOP_PREVENTION)
			{
				break;
			}
		} else
			break;
	}

	switch (id) 
	{
		case T_ARRAY:
		case T_FTNARRAY:
			DVPRINT("GetComplexAIF: Creating AIF array.\n");
			a = GetArrayAIF(session, var);
			break;
		
		case T_POINTER:
			DVPRINT("GetComplexAIF: Creating AIF pointer.\n");
			a = GetPointerAIF(session, var);
			break;
		
		case T_UNION:
			// NOTE: Unions in C are untagged, meaning that we have no intrinsic
			// language support to tell us what is currently placed inside the
			// union. We only know that one of the elements is allocated. To deal
			// with this, gdb will show the user the representation of each field,
			// with the assumption that all but the actual field represents junk.
			// In order to deal with this, we treat unions as structs in this layer,
			// that way the user will be able to see all of the representations
			// of what is currently placed in the union.
			DVPRINT("GetComplexAIF: Creating AIF struct to represent union type.\n");
			a = GetStructAIF(session, var);
			break;
		
		case T_STRUCT:
		// Pretend a derived type is a struct
		case T_FTNDTYPE:
			DVPRINT("GetComplexAIF: Creating AIF struct.\n");
			a = GetStructAIF(session, var);
			break;
		case T_STD_VECTOR:
			DVPRINT("GetComplexAIF: Creating AIF Vector.\n");
			a = GetVectorAIF(session, var);
			break;
		case T_STD_DEQUE:
			DVPRINT("GetComplexAIF: Creating AIF Deque.\n");
			a = GetDequeAIF(session, var);
			break;
		case T_STD_STACK:
			DVPRINT("GetComplexAIF: Creating AIF Stack.\n");
			a = GetStackAIF(session, var);
			break;
		case T_STD_QUEUE:
			DVPRINT("GetComplexAIF: Creating AIF Queue.\n");
			a = GetQueueAIF(session, var);
			break;
		case T_STD_PQUEUE:
			DVPRINT("GetComplexAIF: Creating AIF Priority Queue.\n");
			a = GetPQueueAIF(session, var);
			break;
		default:
			/*
			* Maybe it was simple all alongg
			*/
			DVPRINT("GetComplexAIF: Defaulting to simple aif.\n");
			a = GetSimpleAIF(session, var);
	}
	
	return a;
}

static int
GetSimpleType(char *type)
{
	char *	t = sanitize_type_string(type);
	int	len = strlen(type);
	
	DVPRINT("GetSimpleType: %s\n", type);
	
	// instead of strncmp on everything, lets jump in char by char
	// Note that we need to compare case insensitive since every compiler generates different
	// type information.
start:
	switch (*t++)
	{
// This is for Nvidia address space identifiers that may or may not be present
// case "@"
	case '@':
		switch (*t++)
		{
	// case "@c"
		case 'c':
		case 'C':
			switch (*t++)
			{
		// case "@co"
			case 'o':
			case 'O':
				switch (*t++)
				{
			// case "@cod"
				case 'd':
				case 'D':
					if (strncasecmp(t, "e ", 2) == 0)
					{
					// string "@code "
					DVPRINT("GetSimpleType: Found @code address space identifier!\n");
					t += 2;
					// we ignore these address space identifiers, so simply 
					// return the value of the recursive call to this function
					return GetSimpleType(t);
					}
					// case "@cod" unknown
					goto unknown;
			
			// case "@con"
				case 'n':
				case 'N':
					if (strncasecmp(t, "stant ", 6) == 0)
					{
					// string "@constant "
					DVPRINT("GetSimpleType: Found @constant address space identifier!\n");
					t += 6;
					// we ignore these address space identifiers, so simply 
					// return the value of the recursive call to this function
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
			switch (*t++)
			{
		// case "@ge"
			case 'e':
			case 'E':
				if (strncasecmp(t, "neric ", 6) == 0)
				{
				// string "@generic "
				DVPRINT("GetSimpleType: Found @generic address space identifier!\n");
				t += 6;
				// we ignore these address space identifiers, so simply 
				// return the value of the recursive call to this function
				return GetSimpleType(t);
				}
				// case "@ge" unknown
				goto unknown;
				
		// case "@gl"
			case 'l':
			case 'L':
				if (strncasecmp(t, "obal ", 5) == 0)
				{
				// string "@global "
				DVPRINT("GetSimpleType: Found @global address space identifier!\n");
				t += 5;
				// we ignore these address space identifiers, so simply 
				// return the value of the recursive call to this function
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
			if (strncasecmp(t, "arameter ", 9) == 0)
			{
			// string "@parameter "
			DVPRINT("GetSimpleType: Found @parameter address space identifier!\n");
			t += 9;
			// we ignore these address space identifiers, so simply 
			// return the value of the recursive call to this function
			return GetSimpleType(t);
			}
	// case "@p" unknown
			goto unknown;
			
	// case "@s"
		case 's':
		case 'S':
			if (strncasecmp(t, "hared ", 6) == 0)
			{
			// string "@shared "
			DVPRINT("GetSimpleType: Found @shared address space identifier!\n");
			t += 6;
			// we ignore these address space identifiers, so simply 
			// return the value of the recursive call to this function
			return GetSimpleType(t);
			}
	// case "@s" unknown
			goto unknown;
			
	// case "@t"
		case 't':
		case 'T':
			if (strncasecmp(t, "exture ", 7) == 0)
			{
			// string "@texture "
			DVPRINT("GetSimpleType: Found @texture address space identifier!\n");
			t += 7;
			// we ignore these address space identifiers, so simply 
			// return the value of the recursive call to this function
			return GetSimpleType(t);
			}
	// case "@t" unknown
			goto unknown;
			
	// case "@l"
		case 'l':
		case 'L':
			if (strncasecmp(t, "ocal ", 5) == 0)
			{
			// string "@local "
			DVPRINT("GetSimpleType: Found @local address space identifier!\n");
			t += 5;
			// we ignore these address space identifiers, so simply 
			// return the value of the recursive call to this function
			return GetSimpleType(t);
			}
	// case "@l" unknown
			goto unknown;
			
	// case "@r"
		case 'r':
		case 'R':
			if (strncasecmp(t, "egister ", 8) == 0)
			{
			// string "@register "
			DVPRINT("GetSimpleType: Found @register address space identifier!\n");
			t += 8;
			// we ignore these address space identifiers, so simply 
			// return the value of the recursive call to this function
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
		switch (*t++)
		{
	// case "bo"
		case 'o':
		case 'O':
			if (strncasecmp(t, "ol", 2) == 0)
			{
			// string "bool"
			DVPRINT("GetSimpleType: Found boolean!\n");
			return T_BOOLEAN;
			}
	// case "bo" unknown
			goto unknown;
			
	// case "by"
		case 'y':
		case 'Y':
			if (strncasecmp(t, "te", 2) == 0)
			{
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
		switch (*t++)
		{
	// case "ch"
		case 'h':
		case 'H':
		{
			if (strncasecmp(t, "ar", 2) == 0)
			{
			// string "char"
			t += 2;
			switch (*t)
			{
				case '\0':
				// case "char"
				DVPRINT("GetSimpleType: Found char!\n");
				return T_CHAR;
				
				case 'a':
				case 'A':
				// case "chara"
 					// NOTE: fortran only defines one type of character (kind=1)
 					++t;
 					if (strncasecmp(t, "cter", 4) == 0)
 					{
 					// string "character"
 					DVPRINT("GetSimpleType: Found fortran character!\n");
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
		case 'O':
		{
			switch (*t++)
			{
	// case "con"
			case 'n':
			case 'N':
				if (strncasecmp(t, "st ", 3) == 0)
				{
				// string "const "
					// This gets rid of the const modifier
					DVPRINT("GetSimpleType: Found const modifier.\n");
					t += 3; // one past the white space
					goto start;
				}
	// case "con" unknown
				goto unknown;
			
	// case "com"
				case 'm':
				case 'M':
				if (strncasecmp(t, "plex", 4) == 0)
				{
					// string "complex"
					DVPRINT("GetSimpleType: Found fortran complex!\n");
 					t += 4;
				switch (*t)
				{
					case '\0':
					// pgi defaults to kind = 4
					DVPRINT("GetSimpleType: Fortran kind=4\n");
					return T_COMPLEX_4;
					
					case '(':
					// note that type[len-1] is the closing bracket or ')'
					// intel compiler gives us COMPLEX(n) where n is the kind
					// gnu compiler gives us complex(kind=n)
					switch (type[len-2]) 
					{
						case '4':
						DVPRINT("GetSimpleType: Fortran kind=4\n");
						return T_COMPLEX_4;
						
						case '8':
						DVPRINT("GetSimpleType: Fortran kind=8\n");
						return T_COMPLEX_8;
						
						case '6':
						DVPRINT("GetSimpleType: Fortran kind=16\n");
						return T_COMPLEX_16;
						
						default:
						DVPRINT("GetSimpleType: Unsupported kind!\n");
						goto unknown;
					}
					
					case '*':
					// cray compiler uses this. COMPLEX*n where n is the kind.
					switch (*++t)
					{
						case '4':
						DVPRINT("GetSimpleType: Fortran kind=4\n");
						return T_COMPLEX_4;
						
						case '8':
						DVPRINT("GetSimpleType: Fortran kind=8\n");
						return T_COMPLEX_8;
						
						case '1':
						if (*++t == '6')
						{
							DVPRINT("GetSimpleType: Fortran kind=16\n");
							return T_COMPLEX_16;
						}
						
						default:
						DVPRINT("GetSimpleType: Unsupported kind!\n");
						goto unknown;
					}
					
					default:
					DVPRINT("GetSimpleType: Unsupported kind!\n");
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
		if (strncasecmp(t, "ouble", 5) == 0)
		{
		// string "double"
		t += 5;
		switch (*t)
		{
			case '\0':
			// case "double"
			DVPRINT("GetSimpleType: Found double!\n");
				return T_DOUBLE;
				
				case ' ':
				// case "double "
				switch (*++t)
				{
					case 'c':
					case 'C':
					++t;
					if (strncasecmp(t, "omplex", 6) == 0)
					{
						// string "double complex"
						// pgi uses double complex
						DVPRINT("GetSimpleType: Found fortran double complex!\n");
 					return T_COMPLEX_8;
					}
					// case "double c" unknown
					// this is not a valid function!
					goto unknown_invalid;
					
					case 'p':
					case 'P':
					++t;
					if (strncasecmp(t, "recision", 8) == 0)
					{
						// string "double precision"
						DVPRINT("GetSimpleType: Found fortran double precision!\n");
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
		if (strncasecmp(t, "num", 3) == 0)
		{
		//string "enum"
		DVPRINT("GetSimpleType: Found enum!\n");
			return T_ENUM;
		}
// case "e" unknown
		goto unknown;
		
// case "f"
	case 'f':
	case 'F':
		if (strncasecmp(t, "loat", 4) == 0)
		{
		// string "float"
		DVPRINT("GetSimpleType: Found float!\n");
			return T_FLOAT;
		}
// case "f" unknown
		goto unknown;

// case "i"
	case 'i':
	case 'I':
		if (strncasecmp(t, "nt", 2) == 0)
		{
		// string "int"
		t += 2;
		switch (*t)
		{
			case '\0':
				DVPRINT("GetSimpleType: Found int!\n");
				return T_INT;
				
			case 'e':
			case 'E':
				++t;
				if (strncasecmp(t, "ger", 3) == 0)
				{
					// string "integer"
					DVPRINT("GetSimpleType: Found fortran integer!\n");
					t += 3;
					switch (*t)
					{
						case '\0':
 						// pgi defaults to kind = 4
 						DVPRINT("GetSimpleType: Fortran kind=4\n");
 						return T_INTEGER_4;
 						
 						case '(':
 						// note that type[len-1] is the closing bracket or ')'
 						// intel compiler gives us INTEGER(n) where n is the kind
 						// gnu compiler gives us integer(kind=n)
 						switch (type[len-2]) 
 						{
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
 							DVPRINT("GetSimpleType: Unsupported kind!\n");
 							goto unknown;
 						}
 				
						case '*':
 						// cray compiler uses this INTEGER*n where n is the kind
 						switch (*++t) 
 						{
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
 							DVPRINT("GetSimpleType: Unsupported kind!\n");
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
				// check for stdint int*_t types including null terminator
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
		switch (*t++)
		{
	// case "lo"
		case 'o':
		case 'O':
			switch (*t++)
			{
	// case "lon"
			case 'n':
			case 'N':
				switch (*t++)
				{
		// case "long"
				case 'g':
				case 'G':
					switch (*t)
					{
					case '\0':
						// case "long"
						DVPRINT("GetSimpleType: Found long!\n");
							return T_LONG;
							
						case ' ':
							// here we recursively call this function to avoid having
							// to duplicate cases. We return a value based on the return
							// value of the recursive call. Make sure to preinc t to point
							// past the ' '.
							DVPRINT("GetSimpleType: Going recursive\n");
							switch (GetSimpleType(++t))
							{
							case T_INT:
								DVPRINT("GetSimpleType: Found long!\n");
								return T_LONG;
							case T_UINT:
								DVPRINT("GetSimpleType: Found unsigned long!\n");
								return T_ULONG;
#ifdef CC_HAS_LONG_LONG
							case T_LONG:
								DVPRINT("GetSimpleType: Found long long!\n");
								return T_LONGLONG;
							case T_ULONG:
								DVPRINT("GetSimpleType: Found unsigned long long!\n");
								return T_ULONGLONG;
#endif /* CC_HAS_LONG_LONG */
#ifdef HAVE_LONG_DOUBLE
							case T_DOUBLE:
								DVPRINT("GetSimpleType: Found long double!\n");
								return T_LONGDOUBLE;
#endif
							default:
								// case "long " unknown, this is not a valid func name
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
				if (strncasecmp(t, "ical", 4) == 0)
				{
				// string "logical"
				DVPRINT("GetSimpleType: Found fortran logical!\n");
				t += 4;
				
				switch (*t)
				{
					case '\0':
 						// pgi defaults to kind = 4
 						DVPRINT("GetSimpleType: Fortran kind=4\n");
 						return T_LOGICAL_4;
 				
 					case '(':
 						// note that type[len-1] is the closing bracket or ')'
 						// intel compiler gives us LOGICAL(n) where n is the kind
 						// gnu compiler gives us logical(kind=n)
 						switch (type[len-2]) 
 						{
 						case '1':
 							DVPRINT("GetSimpleType: Fortran kind=1\n");
 							return T_LOGICAL_1;
 						case '2':
 							DVPRINT("GetSimpleType: Fortran kind=2\n");
 							return T_LOGICAL_2;
 						case '4':
 							DVPRINT("GetSimpleType: Fortran kind=4\n");
 							return T_LOGICAL_4;
 						case '8':
 							DVPRINT("GetSimpleType: Fortran kind=8\n");
 							return T_LOGICAL_8;
 						default:
 							DVPRINT("GetSimpleType: Unsupported kind!\n");
 							goto unknown;
 						}
 						
 					case '*':
 						// cray compiler uses this LOGICAL*n where n is the kind
 						switch (*++t) 
 						{
 						case '1':
 							DVPRINT("GetSimpleType: Fortran kind=1\n");
 							return T_LOGICAL_1;
 						case '2':
 							DVPRINT("GetSimpleType: Fortran kind=2\n");
 							return T_LOGICAL_2;
 						case '4':
 							DVPRINT("GetSimpleType: Fortran kind=4\n");
 							return T_LOGICAL_4;
 						case '8':
 							DVPRINT("GetSimpleType: Fortran kind=8\n");
 							return T_LOGICAL_8;
 						default:
 							DVPRINT("GetSimpleType: Unsupported kind!\n");
 							goto unknown;
 						}
 						
 					default:
 						DVPRINT("GetSimpleType: Unsupported kind!\n");
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
		if (strncasecmp(t, "eal", 3) == 0)
		{
		// string "real"
		DVPRINT("GetSimpleType: Found fortran real!\n");
		t += 3;
		switch (*t)
		{
			case '\0':
			// pgi defaults to kind = 4
 				DVPRINT("GetSimpleType: Fortran kind=4\n");
 				return T_REAL_4;
 				
 			case '(':
 				// note that type[len-1] is the closing bracket or ')'
 				// intel compiler gives us REAL(n) where n is the kind
 				// gnu compiler gives us real(kind=n)
 				switch (type[len-2]) 
 				{
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
 				switch (*++t) 
 				{
 				case '4':
 					DVPRINT("GetSimpleType: Fortran kind=4\n");
 					return T_REAL_4;
 				case '8':
 					DVPRINT("GetSimpleType: Fortran kind=8\n");
 					return T_REAL_8;
 				case '1':
 					if (*++t == '6')
 					{
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
		switch (*t++)
		{
	// case "sh"
		case 'h':
		case 'H':
			if (strncasecmp(t, "ort", 3) == 0)
			{
				t += 3;
				switch (*t)
				{
				case ' ':
					// string "short "
					DVPRINT("GetSimpleType: Found short specifier!\n");
					t += 1;
					// here we recursively call this function to avoid having
					// to duplicate cases. We return a value based on the return
					// value of the recursive call.
					DVPRINT("GetSimpleType: Going recursive\n");
					switch (GetSimpleType(t))
					{
						case T_INT:
							DVPRINT("GetSimpleType: Found short!\n");
							return T_SHORT;
						case T_UINT:
							DVPRINT("GetSimpleType: Found unsigned short!\n");
							return T_USHORT;
						default:
							DVPRINT("GetSimpleType: Unknown short!\n");
							goto unknown_invalid;
					}
					
				case '\0':
					// type short
					DVPRINT("GetSimpleType: short!\n");
					return T_SHORT;
				}
			}
			if (strncasecmp(t, "ared ", 3) == 0)
			{
			// string "shared "
				// This gets rid of the shared modifier
				DVPRINT("GetSimpleType: Found shared modifier.\n");
				t += 5; // one past the white space
				goto start;
			}
	// case "sh" unknown
			goto unknown;
			
	// case "si"
		case 'i':
		case 'I':
			if (strncasecmp(t, "gned ", 5) == 0)
			{
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
			if (strncasecmp(t, "ring", 4) == 0)
			{
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
		if (strncasecmp(t, "nsigned", 7) == 0)
		{
		// string "unsigned"
		DVPRINT("GetSimpleType: Found unsigned specifier!\n");
		t += 7;
		switch (*t)
		{
			case '\0':
			DVPRINT("GetSimpleType: Found unsigned!\n");
			return T_UINT;
			
			case ' ':
				// here we recursively call this function to avoid having
				// to duplicate cases. We return a value based on the return
				// value of the recursive call. Make sure to preinc t to point
				// past the ' '.
				DVPRINT("GetSimpleType: Going recursive\n");
				switch (GetSimpleType(++t))
				{
					case T_CHAR:
						DVPRINT("GetSimpleType: Found unsigned char!\n");
						return T_UCHAR;
					case T_SHORT:
						DVPRINT("GetSimpleType: Found unsigned short!\n");
						return T_USHORT;
					case T_INT:
						DVPRINT("GetSimpleType: Found unsigned int!\n");
						return T_UINT;
					case T_LONG:
						DVPRINT("GetSimpletype: Found unsigned long!\n");
						return T_ULONG;
#ifdef CC_HAS_LONG_LONG
					case T_LONGLONG:
						DVPRINT("GetSimpleType: Found unsigned long long!\n");
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
		switch (*t++)
		{
	// case "vo"
		case 'o':
		case 'O':
			switch (*t++)
			{
	// case "voi"
			case 'i':
			case 'I':
				switch (*t++)
				{
		// case "void"
				case 'd':
				case 'D':
					switch (*t)
					{
					case '\0':
						// string "void"
						DVPRINT("GetSimpleType: Found void.\n");
						return T_VOID;
					
					case ' ':
						// string "void "
						t += 1;
						if (*t == '*')
						{
							// string "void *"
							DVPRINT("GetSimpleType: Found void ptr.\n");
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
				if (strncasecmp(t, "atile ", 6) == 0)
				{
				// string "volatile "
				// This gets rid of the volatile modifier
					DVPRINT("GetSimpleType: Found volatile modifier.\n");
					t += 6; // one past the white space
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
		
	// We are finally at the default case for the first character in the string.
	// Note that we want to check if the string is a function if we jumped to unknown
	// or if its first letter was not a known type specifier.
	default:
unknown:
	if (type[len-1] == ')')
	{
		DVPRINT("GetSimpleType: Found function!\n");
		return T_FUNCTION;
	}
	
unknown_invalid:
	// this is the bottom of the parser. If we get here, the type is truly
	// unknown. This means that the parser probably has to be expanded to handle
	// the unknown type, or there was an error with the type string provided by
	// gdb.
	DVPRINT("GetSimpleType: Unknown type.\n");
	return	T_UNKNOWN;
	}
}

/*
 * Attempt to determine the type of a variable from a type string. Assumes
 * that the type is structured. Returns T_UNKNOWN if the type can't be determined.
 *
 * Possible return values:
 * 	T_ARRAY
 * 	T_POINTER
 * 	T_UNION
 * 	T_STRUCT
 * 	T_UNKNOWN
 */
static int
GetComplexType(char *type)
{
	type = sanitize_type_string(type);
	int len = strlen(type);
	if(strncmp(type, "std::",5)==0)
	{
		type+=5;
		switch(*type)
		{
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
	char *t = &type[len - 1];
	char* p;	
	// Check for nvidia address space identifiers, we want to ignore these.
	if (gpu_flag && ((p = strrchr(type, '@')) != NULL))
	{
		DVPRINT("GetComplexType: Pointing at \"%s\"\n", p);
		
		// ensure that the @... business is the last part of the string, otherwise something else is
		// going on
		if (NV_CHECK_ADDR_IDENT_L(p, CODE_S))
		{
			DVPRINT("GetComplexType: Found %s address space identifier\n", CODE_S);
			t = p-2;
		}
		if (NV_CHECK_ADDR_IDENT_L(p, CONSTANT_S))
		{
			DVPRINT("GetComplexType: Found %s address space identifier\n", CONSTANT_S);
			t = p-2;
		}
		if (NV_CHECK_ADDR_IDENT_L(p, GENERIC_S))
		{
			DVPRINT("GetComplexType: Found %s address space identifier\n", GENERIC_S);
			t = p-2;
		}
		if (NV_CHECK_ADDR_IDENT_L(p, GLOBAL_S))
		{
			DVPRINT("GetComplexType: Found %s address space identifier\n", GLOBAL_S);
			t = p-2;
		}
		if (NV_CHECK_ADDR_IDENT_L(p, PARAMETER_S))
		{
			DVPRINT("GetComplexType: Found %s address space identifier\n", PARAMETER_S);
			t = p-2;
		}
		if (NV_CHECK_ADDR_IDENT_L(p, SHARED_S))
		{
			DVPRINT("GetComplexType: Found %s address space identifier\n", SHARED_S);
			t = p-2;
		}
		if (NV_CHECK_ADDR_IDENT_L(p, TEXTURE_S))
		{
			DVPRINT("GetComplexType: Found %s address space identifier\n", TEXTURE_S);
			t = p-2;
		}
		if (NV_CHECK_ADDR_IDENT_L(p, LOCAL_S))
		{
			DVPRINT("GetComplexType: Found %s address space identifier\n", LOCAL_S);
			t = p-2;
		}
		if (NV_CHECK_ADDR_IDENT_L(p, REGISTER_S))
		{
			DVPRINT("GetComplexType: Found %s address space identifier\n", REGISTER_S);
			t = p-2;
		}
		
		DVPRINT("GetComplexType: Pointing at \'%c\'\n", *t);
	}

	switch (*t) 
	{
		case ']':
			DVPRINT("GetComplexType: Found array\n");
			return T_ARRAY;
		
		case '*':
			DVPRINT("GetComplexType: Found pointer\n");
			return T_POINTER; //normal pointer
		
		case ')':
			// check to see if this is a derived type
			if (strncasecmp(type, "type(", strlen("type(")) == 0)
			{
				DVPRINT("GetComplexType: Found fortran derived type\n");
				return T_FTNDTYPE;
			}
			DVPRINT("GetComplexType: Found fortran array\n");
			return T_FTNARRAY;
	
		default:
			if (strncmp(type, "union", 5) == 0) 
			{
				DVPRINT("GetComplexType: Found union\n");
				return T_UNION;
			}
		
			if (strncmp(type, "struct", 6) == 0) 
			{
				DVPRINT("GetComplexType: Found struct\n");
				return T_STRUCT;
			}
		
			DVPRINT("GetComplexType: Unknown type %s\n", type);
			return T_UNKNOWN;
	}
}

static int
GetType(char *type)
{
	type = sanitize_type_string(type);
	int id = GetSimpleType(type);
	if (id == T_UNKNOWN) 
	{
		id = GetComplexType(type);
	}
	
	return id;
}

/*
 * Get the base type of an array from it's type
 * definition string.
 */
static int
GetArrayBaseType(	MISession *session, 
					MIVar *var, 
					char **type_str, 
					int *type_len, 
					struct_info_t **type_struct,
					func_info_t **type_func)
{
	char *	vecStr = strdup(var->type);
	int	 done = 0;
	//DVPRINT("GetArrayBaseType: %s\n", str);
	
	if(strncmp(vecStr, "std::",5)==0)
	{
		vecStr+=5;
		while(!done){
			switch((*vecStr++)){
				case '<':
					if(strncmp(vecStr, "std",3)==0){
							break;
					}
					
					else{
						vecStr = strtok(vecStr, ",");
						done++;
						break;
					}
				default:
					break;

			}

		}
		if (type_str != NULL)
			*type_str = strdup(vecStr);
		if (type_len != NULL)
			*type_len = GetTypeLength(session, vecStr);
		return GetType(vecStr);	
	
	}

	char *	str = strdup(var->type);
	int	id;
	int	 index;
	char *	p;
	done=0;
	// grab the length of the string
	index = strlen(str)-1;
	p = &str[index];
	
	// check if a valid end_delim matches the last character in the string
	switch (*p--)
	{
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
	while (!done)
	{
		// bound check
		if (index==0)
		{
			// unknown array specification
			DVPRINT("GetArrayBaseType: OutofBounds error!\n");
			free(str);
			return T_UNKNOWN;
		}
		
		// see if we are at the start of the range info
		switch (*p--)
		{
			case ARRAY_C_TYPE_START:
			case ARRAY_FTN_TYPE_START:
				// check to see if p[index-1] is a end delimiter, if so we 
				// are not past the range information yet
				switch (*p)
				{
					case ARRAY_C_TYPE_END:
					case ARRAY_FTN_TYPE_END:
						// we are not done
						--index;
						--p;
						break;
						
					default:
						// we are done
						// ignore array range information
						*(p+1) = '\0';
						++done;
				}
				break;
				
			default:
				//ignore it
				--index;
		}
	}
	
	// bound check
	if (index==0)
	{
		// unknown array specification
		DVPRINT("GetArrayBaseType: OutofBounds error!\n");
		free(str);
		return T_UNKNOWN;
	}
	
	// remove whitespace
	while (*p == ' ' && index != 0) 
	{
		*p-- = '\0';
		--index;
	}
	
	// bound check
	if (index==0)
	{
		// unknown array specification
		DVPRINT("GetArrayBaseType: OutofBounds error!\n");
		free(str);
		return T_UNKNOWN;
	}
	
	DVPRINT("GetArrayBaseType: Base resolved to %s.\n", str);

	// get the base type
	id = GetType(str);
	
	if (type_str != NULL)
		*type_str = strdup(str);
	if (type_len != NULL)
		*type_len = GetTypeLength(session, str);
	
	// if its still unknown, try to use ptype
	while (id == T_UNKNOWN) 
	{
		DVPRINT("GetArrayBaseType: Base still unknown, using whatis.\n");
		p = GetMIWhatisValue(session, str, NULL);
		if (p != NULL) {
			id = GetType(p);
			if (type_str != NULL)
			{
				free(*type_str);
				*type_str = strdup(p);
			}
			if (type_len != NULL)
				*type_len = GetTypeLength(session, p);
			free(str);
			str = p;
		} else
			break;
	}
	
	// If this is a struct, we need to resolve the struct info
	if (id == T_STRUCT && type_struct != NULL)
	{
		*type_struct = GetStructInfo(session, str);
	}
	
	// If this is a function, we need to resolve the func info
	if (id == T_FUNCTION && type_func != NULL)
	{
		*type_func = GetFunctionInfo(session, str);
	}

	free(str);

	return id;
}

static
vectorLikeDimension_t * GetVectorLikeDimensionShape(char * buf){
	char * buf1 = strdup(buf);
	char * bufOrig = buf1;
	int * atCoords = NULL;
	int atCoordsSize = 0;
	vectorLikeDimension_t * result = newVectorLikeDimension();
	int lengthToAdd = 0;
	char * lengthStr = NULL;
	int depth = -1;
	char * newBuf1 = NULL;
	char * newBuf1Orig = NULL;
	
	//Error null args
	if(!buf1){
		DVPRINT("GetVectorLikeDimensionShape: buf is NULL");
		return NULL;
	}
	
	//Main loop to parse the data string into the dimension object
	while(*buf1){
		switch(*buf1){
			//Get the length, add it to the dimension object and skip the comma
			case 'l':
				if(strncmp(buf1,"length",6)==0){
					buf1 = buf1+7;
					newBuf1 = strdup(buf1);
					newBuf1Orig = newBuf1;
					lengthStr=strtok(newBuf1,",");
					DVPRINT("lengthStr: $%s$", lengthStr);
					buf1 = buf1+strlen(lengthStr);
					lengthToAdd = atoi(lengthStr);
					addVectorLikeDimensionDef(result, atCoords, atCoordsSize, lengthToAdd);
					free(newBuf1Orig);
				}
				break;
			//Make the current coordinates (atCoords) have another dimension, starting at 0
			case '{':
				atCoords = (int*)realloc(atCoords, (atCoordsSize+1)*sizeof(int));
				atCoords[atCoordsSize] = 0;
				atCoordsSize++;
				depth++;
				break;
				
			//Check if the array being closed is the biggest of its parent's children.
			//If so, store this size in the parent. Shrink our coordinates.
			case '}':
				if(atCoordsSize >=1){
					if(result && result->sizes && ((atCoords[atCoordsSize-1]+1) > result->sizes[depth]))
					{
						result->sizes[depth] = atCoords[atCoordsSize-1]+1;
					}
				}
				atCoords = (int*)realloc(atCoords, (atCoordsSize-1)*sizeof(int));
				atCoordsSize--;		
				--depth;		
				break;
			
			//Commas increment the latest coordinate
			case ',':
				atCoords[atCoordsSize-1]++;
				break;
			
			//Handle containers that use 'with' in their string to describe length
			case 'w':
				if(strncmp(buf1,"with",4)==0){
					buf1 = buf1+5;
					newBuf1 = strdup(buf1);
					newBuf1Orig = newBuf1;
					lengthStr=strtok(newBuf1," ");
					buf1 = buf1+strlen(lengthStr);
					lengthToAdd = atoi(lengthStr);
					addVectorLikeDimensionDef(result, atCoords, atCoordsSize, lengthToAdd);
					free(newBuf1Orig);
				}
				break;
			
			//Otherwise just skip the character
			default:
				break;
				}
		//Go to the next character
		buf1++;


		}
	//free memory
	free(atCoords);
	free(bufOrig);
	
	//return the dimension object we just constructed
	return result;
}

// get the shape of an array
static arrayDimension_t *
GetArrayDimensionShape(MISession *session, MIVar *var)
{
	arrayDimension_t *		shape;
	int			done = 0;
	int			isRange = 0;
	int			isFortran = 0;
	int			len = 0;
	int			min = 0;
	int			max = 0;
	char *		p;
	char *		str = strdup(var->type);
	
	DVPRINT("GetArrayDimensionShape: %s\n", var->type);
	
	// point to the last char in the string
	p = &str[strlen(str) - 1];
	
	// ensure we have valid dimensional information
	switch (*p)
	{
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
	if ((shape = newArrayDimension()) == (arrayDimension_t *)NULL)
	{
		DVPRINT("GetArrayDimensionShape: arrayDimension_t ctor failed!");
		free(str);
		return (arrayDimension_t *)NULL;
	}
	
	// set the end delimiter char to '\0'
	*p-- = '\0';
	
	// iterate while we are not pointing to the begining of the range info
	while (!done)
	{
		// iterate while we are not pointing to a start delimiter and seperation delimiter and str
		switch (*p)
		{
			case ARRAY_C_TYPE_START:
				// c arrays have a range of 0:N-1 where N is the typedef size of the array
				len = atoi(p+1);
				min = 0;
				max = len - 1;
				// add p+1 to the dimension definition list
				if (addArrayDimensionDef(shape, len, min, max))
				{
					DVPRINT("GetArrayDimensionShape: addArrayDimensionDef failed!");
					free(str);
					freeArrayDimension(shape);
					return (arrayDimension_t *)NULL;
				}
				DVPRINT("GetArrayDimensionShape: Added dimension %d of length %d with range %d to %d\n", shape->dim, len, min, max);
				
				// check if there is a end delim right after this
				switch (*--p)
				{
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
				if (isRange)
				{
					// this is the min index for the range
					// remember that we are parsing backwards
					min = atoi(p+1);
					// for a range of N:M the formula is M-N+1
					len = max - min + 1;
					isRange = 0;
				} else
				{
					max = atoi(p+1);
					min = 1;
					len = max;
				}
				
				// add p+1 to the dimension definition list
				if (addArrayDimensionDef(shape, len, min, max))
				{
					DVPRINT("GetArrayDimensionShape: addArrayDimensionDef failed!");
					free(str);
					freeArrayDimension(shape);
					return (arrayDimension_t *)NULL;
				}
				
				// post-dec p setting the end delim to '\0'
				*p-- = '\0';
				
				DVPRINT("GetArrayDimensionShape: Added dimension %d of length %d with range %d to %d\n", shape->dim, len, min, max);
				break;
				
			// fortran is done when it finds a '(' character
			case ARRAY_FTN_TYPE_START:
				if (isRange)
				{
					// this is the min index for the range
					// remember that we are parsing backwards
					min = atoi(p+1);
					// for a range of N:M the formula is M-N+1
					len = max - min + 1;
					isRange = 0;
				} else
				{
					max = atoi(p+1);
					min = 1;
					len = max;
				}
				// add p+1 to the dimension definition list
				if (addArrayDimensionDef(shape, len, min, max))
				{
					DVPRINT("GetArrayDimensionShape: addArrayDimensionDef failed!");
					free(str);
					freeArrayDimension(shape);
					return (arrayDimension_t *)NULL;
				}
				DVPRINT("GetArrayDimensionShape: Added dimension %d of length %d with range %d to %d\n", shape->dim, len, min, max);
				// done parsing the string
				++done;
				break;
				
			// fortran allows for the type information to be specified as a range such as (0:5,1:3)
			// we need to deal with this case
			case ARRAY_FTN_RANGE_SEP:
				// ensure isRange is not set. This would mean we encountered multiple ":" in the range string.
				if (isRange)
				{
					DVPRINT("GetArrayDimensionShape: isRange was already set!");
					free(str);
					freeArrayDimension(shape);
					return (arrayDimension_t *)NULL;
				}
				// store the integer value of p+1 in len and set the isRange flag.
				max = atoi(p+1);
				++isRange;
				// post-dec p setting the end delim to '\0'
				*p-- = '\0';
				break;
				
			default:
				// check if p is less than or equal to str
				if (p <= str)
				{
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
	
	DVPRINT("GetArrayDimensionShape: Total dimensions detected %d\n", shape->dim);
	
	return shape;
}

/*
 * Get the name of a type (struct, union or enum) from the
 * MI type string. This now also works for any generic variable, for instance
 * int i would return i, char *c would return c.
 *
 * Returns the type name or NULL if the type is unnamed.
 */
static char *
GetTypeName(char *type)
{
	char *	p;
	char *	p2;
	
	DVPRINT("GetTypeName: %s\n", type);

	if ((p = strrchr(type, ' ')) != NULL) 
	{
		p++;
		if (strncmp(p, "{...}", 5) == 0) 
		{
			DVPRINT("GetTypeName: NULL type\n");
			return NULL;
		}
		
		while (*p == '*')
		{
			p++;
		}
		
		DVPRINT("GetTypeName: returning %s\n", p);
		
		return p;
	}
	
	// check for fortran derived type
	if (strncasecmp(type, "TYPE(", strlen("TYPE(")) == 0)
	{
		p = type;
		// point past type
		p += strlen("TYPE(");
		// set closing brace to null term
		if ((p2 = strchr(p, ')')) != NULL)
		{
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
static void
RemoveWhitespace(char *str)
{
	char *p1 = str;
	char *p2 = str;

	if (*p1 == '\0')
	{
		return;
	}

	// blanket remove all leading spaces
	while (isspace(*p1) && (*p1 != '\0'))
	{
		++p1;
	}

	while (*p1 != '\0')
	{
		if (isspace(*p1)
		// don't remove spaces between keywords (assume type doesn't seperate with more than one space)
		 && ((p1[1] == '\0') || !(isalnum(*(p1 - 1)) && isalnum(*(p1 + 1)))))
		{
			++p1;
		} else
		{
			*p2++ = *p1++;
		}
	}
	// set p2 to a final null terminator
	*p2 = '\0';
}

/*
** This function is used when creating a simple AIF type from a gdb string.
*/
static func_info_t *
GetFunctionInfo(MISession *session, char *type_str)
{
	func_info_t *	result;

	DVPRINT("GetFunctionInfo: %s\n", type_str);
	
	// malloc the return type
	if ((result = (func_info_t*)malloc(sizeof(func_info_t))) == (void *)0)
	{
		DVPRINT("GetFunctionInfo: Malloc Failed.\n");
		return NULL;
	}
	
	// get the number of arguments and arguments of function
	result->argc = GetFunctionArguments(session, type_str, &result->argv);
	DVPRINT("GetFunctionInfo: Num args: %d\n", result->argc);
	
	// get the return type of the function
	result->rtn_type = GetFunctionReturnType(session, type_str);
	DVPRINT("GetFunctionInfo: Function return type: %s\n", result->rtn_type);
	
	// We need to ask for the length of an address on this system
	result->addr_len = GetTypeLength(session, "void *");
	
	return result;
}

/*
** This function returns the number of arguments of a function and writes the
** FDS type of the arg into the location of argv. Note that this function will
** malloc a new argv array and it is the callers responsibility to free it.
*/
static int
GetFunctionArguments(MISession *session, char *type, char **argv[])
{
	char *	start;
	char *	end;
	char *	res;
	char *	p;
	char *	q;
	int	 arg_type;
	char *	arg;
	int	 num_args = 1;
	int	 i;
	
	DVPRINT("GetFunctionArguments: %s\n", type);
	
	// find the opening '('
	if ((start = strchr(type, FUNCTION_START)) == (char *)NULL)
	{
		// couldn't find opening '('
		DVPRINT("GetFunctionArguments: Couldn't find opening \"(\"\n");
		return 0;
	}
	
	// Check to see if this is a function pointer
	if (!strncmp(start, VOID_FUNCTION_STR, strlen(VOID_FUNCTION_STR)))
	{
		DVPRINT("GetFunctionArguments: Found function pointer!\n");
		// point start past the VOID_FUNCTION_STR string
		start += strlen(VOID_FUNCTION_STR);
	}
	
	// make start point past the '('
	++start;
	
	// make a new copy of start
	res = strdup(start);
	
	// now find the end
	if ((end = strrchr(res, FUNCTION_END)) == (char *)NULL)
	{
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
	if (strlen(res) == 0 || strncasecmp(res, "void", 4) == 0)
	{
		free(res);
		return 0;
	}
	
	// figure out the number of arguments
	p = res;
	while ((p = strchr(p, FUNCTION_ARG_SEP)) != (char *)NULL)
	{
		// advance p and num_args
		++p;
		++num_args;
	}
	
	// allocate the argv buffer
	if ((*argv = (char**)malloc(sizeof(char *) * num_args)) == (void *)0)
	{
		DVPRINT("GetFunctionArguments: malloc error\n");
		free(res);
		return 0;
	}
	
	DVPRINT("GetFunctionArguments: num_args is %d\n", num_args);
	
	// parse the args and convert to FDS format string
	p = res;
	for (i=0; i < num_args; ++i)
	{
		// point q at the first ','
		// turn the first FUNCTION_ARG_SEP into a null term
		if ((q = strchr(p, FUNCTION_ARG_SEP)) != (char *)NULL)
		{
			*q = '\0';
		}
		
		// get the type id of p
		arg_type = GetType(p);
		
		if ((arg = MIToFDS(session, arg_type, p)) == NULL)
		{
			DVPRINT("GetFunctionArguments: Unknown farg_type!\n");
			free(res);
			return 0;
		}
		
		// get the FDS string, this needs to be strdup'ed since the
		// string is returned in a private buffer
		(*argv)[i] = strdup(arg);
		
		DVPRINT("GetFunctionArguments: Added arg %s\n", *argv[i]);
		
		// point p 1 past q and continue
		if (q != NULL)
		{
			p = q+1;
		} else if (i+1 != num_args)
		{
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
** MIVar represented by type. The return type is found by taking the string before
** the opening '(' and removing any whitespace.
*/
static char *
GetFunctionReturnType(MISession *session, char *type)
{
	char *	str;
	char *	end;
	int	 rtn_type;
	char *	rtn;
	
	if (type == NULL)
		return (char *)NULL;
	
	DVPRINT("GetFunctionReturnType: %s\n", type);
	str = strdup(type);
	
	if ((end = strchr(str, FUNCTION_START)) == (char *)NULL)
	{
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
	if ((rtn = MIToFDS(session, rtn_type, str)) == NULL)
	{
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
static AIF *
GetFunctionAddress(MISession *session, char *value)
{
	char *			start;
	char *			end;
	long long int	addr_loc;
	void *			addr;
	int				addr_len;
	
	DVPRINT("GetFunctionAddress: %s\n", value);
	
	if (value == NULL)
		return (AIF *)NULL;
	
	start = strdup(value);
	
	// find the first '<' in the start string
	if ((end = strchr(start, '<')) != (char *)NULL)
	{
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
static char *
GetFunctionName(char *value)
{
	char *	start;
	char *	end;
	char *	res;
	
	CSAPRINT("GetFunctionName: %s\n", value);
	
	// find the final '<' in the value string
	if ((start = strrchr(value, '<')) == (char *)NULL)
	{
		// couldn't find final '<'
		return strdup("");
	}
	// point pass the '<'
	++start;
	
	// make a new copy of start
	res = strdup(start);
	
	// find the final '>' in the value string
	if ((end = strrchr(res, '>')) == (char *)NULL)
	{
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
static char *
MIToFDS(MISession *session, int type_id, char *base_type)
{
	char *	fds_type = NULL;
	int	 len;
	
	DVPRINT("MIToFDS: base_type=>%s\n", base_type);
	
	switch (type_id)
	{
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

static AIF *
CreateSimpleAIF(int id, 
				char *res, 
				char *type_str, 
				int type_len, 
				struct_info_t * type_struct,
				func_info_t * type_func)
{
	char *pch;
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
			if ((pch = strchr(res, ' ')) != NULL) 
			{
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
			if ((pch = strchr(res, ' ')) != NULL)
			{
				pch++;
				if (*pch == '\'')
				{
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
			if ((pch = strchr(res, ',')) == NULL)
				return VoidToAIF(0,0);
			// get rid of the final ')'
			res[strlen(res)-1] = '\0';
			// get rid of the initial '('
			// res now holds the real part
			res++;
			// get rid of the ','
			*pch = '\0';
			// get the index to the complex part
			pch++;
			CSAPRINT("CreateSimpleAIF: Creating COMPLEX4\n");
			return COMPLEX4ToAIF(strtof(res, NULL), strtof(pch, NULL));
		case T_COMPLEX_8:
			// ensure gdb gives us a sane result
			if ((pch = strchr(res, ',')) == NULL)
				return VoidToAIF(0,0);
			// get rid of the final ')'
			res[strlen(res)-1] = '\0';
			// get rid of the initial '('
			// res now holds the real part
			res++;
			// get rid of the ','
			*pch = '\0';
			// get the index to the complex part
			pch++;
			CSAPRINT("CreateSimpleAIF: Creating COMPLEX8\n");
			return COMPLEX8ToAIF(strtod(res, NULL), strtod(pch, NULL));
#ifdef HAVE_LONG_DOUBLE
		case T_COMPLEX_16:
			// ensure gdb gives us a sane result
			if ((pch = strchr(res, ',')) == NULL)
				return VoidToAIF(0,0);
			// get rid of the final ')'
			res[strlen(res)-1] = '\0';
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
#endif
		case T_LOGICAL_1:
			CSAPRINT("CreateSimpleAIF: Creating LOGICAL1\n");
			if (strncmp(res, ".TRUE.", 6) == 0)
			{
				return LOGICAL1ToAIF(1);
			} else
			{
				return LOGICAL1ToAIF(0);
			}
		case T_LOGICAL_2:
			CSAPRINT("CreateSimpleAIF: Creating LOGICAL2\n");
			if (strncmp(res, ".TRUE.", 6) == 0)
			{
				return LOGICAL2ToAIF(1);
			} else
			{
				return LOGICAL2ToAIF(0);
			}
		case T_LOGICAL_4:
			CSAPRINT("CreateSimpleAIF: Creating LOGICAL4\n");
			if (strncmp(res, ".TRUE.", 6) == 0)
			{
				return LOGICAL4ToAIF(1);
			} else
			{
				return LOGICAL4ToAIF(0);
			}
#ifdef CC_HAS_LONG_LONG
		case T_LOGICAL_8:
			CSAPRINT("CreateSimpleAIF: Creating LOGICAL8\n");
			if (strncmp(res, ".TRUE.", 6) == 0)
			{
				return LOGICAL8ToAIF(1);
			} else
			{
				return LOGICAL8ToAIF(0);
			}
#endif
		case T_CHARACTER:
			CSAPRINT("CreateSimpleAIF: Creating CHARACTER\n");
			// gdb adds in '...' around the string, we need to remove these
			if (*res == '\'')
			{
				++res;
				// get rid of the final '
				if ((pch = strrchr(res, '\'')) != NULL)
				{
					*pch = '\0';
				}
				CSAPRINT("CreateSimpleAIF: res is now \"%s\"\n", res);
			}
			return CHARACTERToAIF(res);
			
		case T_POINTER:
			CSAPRINT("CreateSimpleAIF: Creating pointer\n");
			if (type_str == NULL || type_len == 0)
			{
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
		
		case T_FUNCTION:
		{
			AIF *			ac;
			AIF *			addr;
			long long int	addr_loc;
			void *			addr_ptr;
			char *			name;
			
			CSAPRINT("CreateSimpleAIF: Creating function.\n");
			
			if (type_func == NULL)
			{
				DVPRINT("CreateSimpleAIF: type_func NULL! Creating void\n");
				return VoidToAIF(0,0);
			}
			
			// first create an address of the res
			addr_loc = strtoll(res, NULL, 0);
			addr_ptr = (void *)addr_loc;
			addr = AddressToAIF(addr_ptr, type_func->addr_len);
			
			// get the function name
			name = GetFunctionName(res);
			
			// Now create the actual function pointer
			ac = FunctionToAIF(addr, name, type_func->rtn_type, type_func->argc, type_func->argv);

			Free(name);
			AIFFree(addr);
			return ac;
		}
		
		case T_UNION:
			// NOTE: Unions in C are untagged, meaning that we have no intrinsic
			// language support to tell us what is currently placed inside the
			// union. We only know that one of the elements is allocated. To deal
			// with this, gdb will show the user the representation of each field,
			// with the assumption that all but the actual field represents junk.
			// In order to deal with this, we treat unions as structs in this layer,
			// that way the user will be able to see all of the representations
			// of what is currently placed in the union.
		case T_STRUCT:
		{
			// NOTE: This only works for c structures. Fortran types will need
			// to be added later.
			int index = 0;
			AIF * a;
			AIF * ac;
			
			CSAPRINT("CreateSimpleAIF: Creating struct\n");
			if (type_str == NULL)
			{
				DVPRINT("CreateSimpleAIF: type_str NULL! Creating void\n");
				return VoidToAIF(0, 0);
			}
			if (type_struct == NULL)
			{
				DVPRINT("CreateSimpleAIF: type_struct NULL! Creating void\n");
				return VoidToAIF(0,0);
			}
			
			a = EmptyStructToAIF(type_struct->name);
			pch = res;
			
			while (index < type_struct->numFields)
			{
				// point at the fields "="
				if ((s1 = strchr(pch, '=')) == NULL)
				{
					DVPRINT("CreateSimpleAIF: Could not find = in field.\n");
					return VoidToAIF(0,0);
				}
				// point n2 before the fields "="
				name = s1-1;
				// set this to null
				*name = '\0';
				// point to one before the next occurance of " " or '{'
				while (*(name-1) != ' ' && *(name-1) != '{')
					--name;
				
				// point past the fields "="
				++s1;
				// if there is whitespace after the "=", ignore it.
				while (*s1 == ' ')
				{
					++s1;
				}
				// point s2 at the first occurance of ","
				if ((s2 = strchr(s1, ',')) == NULL)
				{
					// if there is no "," then we are at the final entry, point
					// at the closing bracket
					if ((s2 = strchr(s1, '}')) == NULL)
					{
						// not found, something is wrong...
						DVPRINT("CreateSimpleAIF: Could not find final , or } in field.\n");
						return VoidToAIF(0, 0);
					}
					// ensure that there is not a opening bracket character, implying
					// that s2 is pointing at the wrong bracket
					if ((t1 = strchr(s1, '{')) != NULL)
					{
						while (t1 < s2)
						{
							// there is a opening bracket before the closing bracket that we found
							// point t2 at the next bracket past s2
							if ((t2 = strchr(++s2, '}')) == NULL)
							{
								// not found, something is wrong...
								DVPRINT("CreateSimpleAIF: Detected no sep, found opening bracket, could not find second closing bracket.\n");
								return VoidToAIF(0, 0);
							}
							s2 = t2;
							if ((t2 = strchr(++t1, '{')) == NULL)
							{
								// counld not find another opening bracket, so
								// we must be done.
								break;
							}
							t1 = t2;
						}
					}
				} else
				{
					// ensure that there is not a '{' character, implying that
					// s2 might be pointing at the wrong seperator.
					if ((t1 = strchr(s1, '{')) != NULL)
					{
						// if the opening bracket happens before the seperator,
						// we need to point s2 at the closing bracket the first
						// time through
						if (t1 < s2)
						{
							if ((s2 = strchr(t1, '}')) == NULL)
							{
								// not found, something is wrong...
								DVPRINT("CreateSimpleAIF: Detected sep, found opening bracket, could not find first closing bracket.\n");
								return VoidToAIF(0, 0);
							}
							
							// now check for another opening bracket, implying we
							// are pointing at the wrong closer
							if ((t1 = strchr(++t1, '{')) == NULL)
							{
								// there are no more opening brackets, so we are
								// done
								t1 = s2;
							}
						}
						
						// now loop through until we find the real closing bracket
						while (t1 < s2)
						{
							// there is an opening bracket before the sep that we found
							// point t2 at the next bracket past s2
							if ((t2 = strchr(++s2, '}')) == NULL)
							{
								// not found, something is wrong...
								DVPRINT("CreateSimpleAIF: Detected sep, found opening bracket, could not find closing bracket.\n");
								return VoidToAIF(0, 0);
							}
							s2 = t2;
							if ((t2 = strchr(++t1, '{')) == NULL)
							{
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
				pch = s2+1;
				// set s2 to a null terminator
				*s2 = '\0';
				
				// now create a new AIF type from the res we just pieced out
				ac = CreateSimpleAIF(	type_struct->id[index], 
										s1, 
										type_struct->type_str[index], 
										type_struct->type_len[index], 
										type_struct->inner_struct[index],
										type_struct->type_func[index]);
				
				CSAPRINT("CreateSimpleAIF: Adding AIF field for member %d.\n", index+1);
				AIFAddFieldToStruct(a, strdup(name), ac);
				AIFFree(ac);
				++index;
			}
			return a;
		}
		
		default://other type
			CSAPRINT("CreateSimpleAIF: Creating void\n");
			return VoidToAIF(0, 0);
	}
}

/*
** Create an AIF object that is the struct represented by the MIVar var.
*/
static AIF *
GetStructAIF(MISession *session, MIVar *var)
{
	MIVar *		v;
	AIF *		a;
	AIF *		ac;
	char *		base_type = NULL;
	int		 i;

	DVPRINT("GetStructAIF: %s\n", var->name);
	
	// get the base type for this struct
	base_type = GetTypeName(var->type);
	DVPRINT("GetStructAIF: Base type: %s\n", base_type);
	
	a = EmptyStructToAIF(base_type);

	// Resolve the child information for the var
	if (MIVarGetChildren(session, var))
	{
		// error already set
		AIFFree(a);
		return NULL;
	}
	
	DVPRINT("GetStructAIF: Total number of children detected: %d\n", var->numchild);

	for (i=0; i < var->numchild; i++)
	{
		DVPRINT("GetStructAIF: Resolving member %d\n", i+1);
		v = var->children[i];
		// set the expr to the name of this member
		v->expr = strdup(v->name);
		if ((ac = GetAIF(session, v)) == NULL)
		{
			DVPRINT("GetStructAIF: ERROR: Null aif object returned from GetAIF.\n");
			AIFFree(a);
			return NULL;
		}
		DVPRINT("GetStructAIF: Adding AIF field for member %d.\n", i+1);
		AIFAddFieldToStruct(a, v->exp, ac);
		AIFFree(ac);
	}
	
	return a;
}

static struct_info_t *
GetStructInfo(MISession *session, char *type_str)
{
	MICommand *		cmd;
	MIList *		field_list;
	char *			field;
	char *			tmp_field = NULL;
	char *			tmp;
	char *			tmp2;
	char			tmp_char;
	int				id;
	struct_info_t *	result;
	int				index;
	
	DVPRINT("GetStructInfo: %s\n", type_str);
	
	cmd = CLIPtype(type_str);
	SendMICommand(session, cmd);
	
	if (!MICommandResultOK(cmd))
	{
		DVPRINT("GetStructInfo: CLIPtype call failed.\n");
		MICommandFree(cmd);
		return NULL;
	}
	
	field_list = CLIGetPtypeFieldList(cmd);
	MICommandFree(cmd);
	
	// malloc the return type
	if ((result = (struct_info_t*)malloc(sizeof(struct_info_t))) == (void *)0)
	{
		DVPRINT("GetStructInfo: Malloc Failed.\n");
		return NULL;
	}
	
	if ((result->numFields = MIListSize(field_list)) == 0)
	{
		DVPRINT("GetStructInfo: Zero fields detected.\n");
		MIListFree(field_list, NULL);
		free(result);
		return NULL;
	}
	
	// set the name of the struct
	result->name = strdup(GetTypeName(type_str));
	
	// malloc each of the arrays inside the struct_info_t
	if ((result->id = (int*)malloc(sizeof(int) * result->numFields)) == (void *)0)
	{
		DVPRINT("GetStructInfo: Malloc Failed.\n");
		return NULL;
	}
	
	if ((result->type_str = (char**)malloc(sizeof(char *) * result->numFields)) == (void *)0)
	{
		DVPRINT("GetStructInfo: Malloc Failed.\n");
		return NULL;
	}
	
	if ((result->type_len = (int*)malloc(sizeof(int) * result->numFields)) == (void *)0)
	{
		DVPRINT("GetStructInfo: Malloc Failed.\n");
		return NULL;
	}
	
	if ((result->type_func = (func_info_t**)malloc(sizeof(func_info_t *) * result->numFields)) == (void *)0)
	{
		DVPRINT("GetStructInfo: Malloc Failed.\n");
		return NULL;
	}
	// memset this to 0
	memset(result->type_func, 0, sizeof(func_info_t *) * result->numFields);
	
	if ((result->inner_struct = (struct_info_t**)malloc(sizeof(struct_info_t *) * result->numFields)) == (void *)0)
	{
		DVPRINT("GetStructInfo: Malloc Failed.\n");
		return NULL;
	}
	
	// Now fill in the information from the returned list
	index = 0;
	MIListSet(field_list);
	while ((field = (char *)MIListGet(field_list)) != NULL) 
	{
		DVPRINT("GetStructInfo: field is %s\n", field);
		// The field contains the type including its name. For instance, field
		// will contain int i. We only want to point at the int part.
		// Note that we need to do special things for function pointers.
		if ((tmp = strrchr(field, ' ')) != NULL) 
		{
			// ensure tmp+1 is not pointing at a '*', this is included in the
			// base type.
			while (*(tmp+1) == '*')
			{
				++tmp;
			}
			// one last time, need to point after the '*'
			if (*tmp == '*')
			{
				++tmp;
			}
			
			// check to see if we are pointing at a named function pointer
			if (strncmp(tmp, "(*", 2) == 0 || strncmp(tmp, " (*", 3) == 0)
			{
				// we are pointing at a function pointer, we need to do something
				// special.
				// start by strdup'ing the whole string
				tmp_field = strdup(field);
				// point at the final ' '
				tmp = strrchr(tmp_field, ' ');
				while (*(tmp+1) == '*')
				{
					++tmp;
				}
				// one last time, need to point after the '*'
				if (*tmp == '*')
				{
					++tmp;
				}
				
				if (*tmp == ' ')
					++tmp;
				
				// point past the "(*"
				tmp += 2;
				
				// point tmp2 at the closing ')'
				tmp2 = tmp;
				while (*tmp2 != ')')
					++tmp2;
				
				// copy until null term
				while (*tmp2 != '\0')
				{
					*tmp++ = *tmp2++;
				}
				*tmp = '\0';
				// strdup one last time for good measure, makes sure everything
				// is clean
				tmp2 = strdup(tmp_field);
				Free(tmp_field);
				tmp_field = tmp2;
				// Get the function info 
				result->type_func[index] = GetFunctionInfo(session, tmp_field);
			} else
			{
				// not a function pointer, proceed as usual
				tmp_char = *tmp;
				*tmp = '\0';
				tmp_field = strdup(field);
				// undo what we just did to the original field
				*tmp = tmp_char;
			}
		} else
		{
			tmp_field = strdup(field);
		}
	
		// use whatis to get the field type while its unknown
		while ((id = GetType(tmp_field)) == T_UNKNOWN)
		{
			tmp = GetMIWhatisValue(session, GetTypeName(field), NULL);
			if (tmp != NULL)
			{
				field = tmp;
				free(tmp_field);
				tmp_field = strdup(tmp);
			} else
				break;
		}
		
		result->id[index] = id;
		result->type_str[index] = strdup(tmp_field);
		result->type_len[index] = GetTypeLength(session, tmp_field);
		
		DVPRINT("GetStructInfo: id is %d\n", result->id[index]);
		DVPRINT("GetStructInfo: type_str is %s\n", result->type_str[index]);
		DVPRINT("GetStructInfo: type_len is %d\n", result->type_len[index]);
		
		// only need the struct_info_t stuff filled out if this is a struct
		if (id == T_STRUCT)
		{
			DVPRINT("GetStructInfo: Gathering inner struct info.\n");
			// recursively call this function with the field
			result->inner_struct[index] = GetStructInfo(session, tmp_field);
		} else
		{
			result->inner_struct[index] = NULL;
		}
		
		DVPRINT("GetStructInfo: Done with field.\n");
		
		// increment index
		++index;
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
** This is to optimise the space required to build the immediate AIFArray structure
** since only the hashes that we are interested in
** Minh 17/10/2011
**/
static AIF *
GetArrayAIFHash(MISession *session, MIVar *var, char *hashinfo,
		int hashfunc, int precision_lower, int precision_upper)
{
	int					id;
	char *				buf;
	MICommand *			cmd;
	arrayDimension_t *	shape;
	AIF *				a = NULL;
	AIF *				ac;

	id = GetArrayBaseType(session, var, NULL, NULL, NULL, NULL);

	if (var->numchild <= 0)	// empty array
	{
		ac = CreateSimpleAIF(id, var->value, NULL, 0, NULL, NULL);
		a = EmptyArrayToAIF(0, 0, ac);
		AIFFree(ac);
		return a;
	}
	else
	{
		// first we need to get the data string
		DBEPERFLOG(SetTimerStart("GetArrayAIFHash: -data-evaluate-expression"));
		cmd = MIDataEvaluateExpression(var->expr);
		SendMICommand(session, cmd);
		if (!MICommandResultOK(cmd))
		{
			if (MICommandResultClass(cmd) == MIResultRecordERROR)
			{
				if ((buf = MICommandResultErrorMessage(cmd)) != NULL)
				{
					DVPRINT("GetArrayAIFHash: Debugger error: %s\n", buf);
					EVENT_ERROR(dbg_error, DBGERR_DEBUGGER, buf);
					free(buf);
				} else
				{
					DVPRINT("GetArrayAIFHash: Error from GDB, but no message\n");
					EVENT_ERROR(dbg_error, DBGERR_UNKNOWN, "Error without message!");
				}
			} else
			{
				DVPRINT("GetArrayAIFHash: Bad response from GDB\n");
				EVENT_ERROR(dbg_error, DBGERR_UNKNOWN, "Bad response from GDB");
			}

			MICommandFree(cmd);

			return NULL;
		}
		DBEPERFLOG(SetTimerStopAndLog("GetArrayAIFHash: -data-evaluate-expression"));
		
		buf = MIGetDataEvaluateExpressionInfo(cmd);
		MICommandFree(cmd);
	}


	// Now lets get the shape of this array
	if ((shape = GetArrayDimensionShape(session, var)) == (arrayDimension_t *)NULL)
	{
		// error already set
		DVPRINT("GetArrayAIFHash: GetArrayDimensionShape error\n");
		return NULL;
	}

	// we have the data string and the shape, we need to aply hashinfo to hash the data
	a = PerformHashMI(buf, var->type, shape, hashinfo, hashfunc, precision_lower, precision_upper);
	
	return a;
}

static char * GetArrayDataString(MISession *session, 
								MIVar *var)
{
	char *				buf2;
	MICommand *		 	cmd;
	char *				buf;
	int					last_resort = 0;
	DBEPERFLOG(SetTimerStart("GetArrayAIF: -data-evaluate-expression"));
			cmd = MIDataEvaluateExpression(var->expr);
			SendMICommand(session, cmd);
			while (!MICommandResultOK(cmd))
			{
				if (MICommandResultClass(cmd) == MIResultRecordERROR) 
				{
					if ((buf = MICommandResultErrorMessage(cmd)) != NULL)
					{
						DVPRINT("GetArrayAIF: Debugger error: %s\n", buf);
						EVENT_ERROR(dbg_error, DBGERR_DEBUGGER, buf);
						free(buf);
						// try to resolve the expression as a last resort and try again
						if (!last_resort)
						{
							++last_resort;
							MICommandFree(cmd);
							cmd = MIVarInfoPathExpression(var->expr);
							SendMICommand(session, cmd);
							if (MICommandResultOK(cmd))
							{
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
					} else 
					{
						DVPRINT("GetArrayAIF: Error from GDB, but no message\n");
						EVENT_ERROR(dbg_error, DBGERR_UNKNOWN, "Error without message!");
					}
				} else
				{
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

static AIF * parseMIVectorLikeContToAIF(
								char *type_str, 
								int type_len, 
								struct_info_t * type_struct, 
								func_info_t * type_func,
								char * dataBuf,
								int id,
								vectorLikeDimension_t *	shape)
{
	char * buf1 = strdup(dataBuf);
	char * buf2 = strdup(dataBuf);
	char * buf1Orig = buf1;
	char * buf2Orig = buf2;
	char * buf1dup = NULL;
	char * buf2dup = NULL;
	char * buf1DupOrig = NULL;
	char * buf2DupOrig = NULL;
	int * atCoords = NULL;
	int atCoordsSize = 0;
	int done = 0;
	char * lengthStr = NULL;
	char * currElem = NULL;
	unsigned int currElemLength = 0;
	int maxDepth = 0;
	int depth=0;
	char * currElemShort = NULL;
	vectorLikeDimension_t * tempVecDim = NULL;
	AIF * ac = NULL;
	printArray(shape->sizes, shape->sizesSize);

	/*
		Why do we need a two pass parser? Turns out that you can only initiialize an empty AIF array if you have a representative element
		(any element, it's not really used only it's size is) so that the size can be inferred. 
		
		The first pass gets a string representation of the first base element it can find and exits, storing it in currElemShort
		
		The second pass proceeds to parse the entire multidimensional structure into AIF 
	*/
	while(!done){
		DVPRINT("buf1: %s\n", buf1);
		DVPRINT("maxDepth: %d\n", maxDepth);
		switch(*buf1){
			//Ignore all spaces
			case ' ':
				break;
			//A way to skip over commas
			case 'l':
				if(strncmp(buf1,"length",6)==0){
					buf1 = buf1+7;
					buf1dup = strdup(buf1);
					buf1DupOrig = buf1dup;
					lengthStr=strtok(buf1dup," ");
					buf1 = buf1+strlen(lengthStr);
					free(buf1DupOrig);
				}
				break;
			//Avoid useless commas, part 2
			case 'w':
				if(strncmp(buf1,"with",4)==0)
				{
					buf1 = buf1+4;
					buf1dup = strdup(buf1);
					buf1DupOrig = buf1dup;
					lengthStr=strtok(buf1dup," ");
					buf1 = buf1 +strlen(lengthStr);
					free(buf1DupOrig);
				}
				break;
			case '{':
				//currElem keeps track of the current base type element
				currElem = buf1+1;
				currElemLength = 0;
				++depth;
				if(depth > maxDepth){
					maxDepth = depth;
					DVPRINT("maxDepth incremented. It is now %d\n", maxDepth);
				}
				break;
			//Carve out only the relevant part of the string into currElemShort and exit
			case '}':
				currElemShort=(char*)realloc(currElemShort, currElemLength+1);
				strncpy(currElemShort,  currElem, currElemLength);
				currElemShort[currElemLength] = '\0';
				DVPRINT("parseMIVectorLikeContToAIF: Got the representative element (%s)\n", currElemShort);

				currElem = buf1+1;
				currElemLength = 0;
				done++;
				--depth;
				break;
			//Carve out only the relevant part of the string into currElemShort and exit
			case ',':
				currElemShort=(char*)realloc(currElemShort, currElemLength+1);
				strncpy(currElemShort,  currElem, currElemLength);
				currElemShort[currElemLength] = '\0';
				DVPRINT("parseMIVectorLikeContToAIF: Got the representative element (%s)\n", currElemShort);
				currElem = buf1+2;
				currElemLength = 0;
				done++;
				break;
			default:
				currElemLength++;
				break;
				}
		buf1++;
	}
	DVPRINT("Attempting to build simple AIF with $%s$", currElemShort);
	//Now have the representative element (currElemShort) to pass in
	//when building empty arrays
	ac = CreateSimpleAIF(id, currElemShort, type_str, type_len, type_struct, type_func);
	DVPRINT("Built simple AIF");
	
	//Build empty arrays with all the right coordinates using the shape of the array
	createDummyAIF(shape, ac);
	DVPRINT("parseMIVectorLikeContToAIF: Built the dummies\n");
	
	depth=0;
	DVPRINT("maxDepth is: %d\n", maxDepth);
	
	printVectorLikeDimension(shape);
	while(*buf2){
		//DVPRINT("buf2: %s\n", buf2);
		switch(*buf2){
			//Ignore all spaces
			case ' ':
				break;
			//A way to skip over commas
			case 'l':
				if(strncmp(buf2,"length",6)==0){
					buf2 = buf2+7;
					buf2dup = strdup(buf2);
					buf2DupOrig = buf2dup;
					lengthStr=strtok(buf2dup," ");
					buf2 = buf2+strlen(lengthStr);
					DVPRINT("parseMIVectorLikeContToAIF: Skipping commas length\n");
					free(buf2DupOrig);
				}
				break;
			//{ means we've gone down a level. Make a new AIF array and prepare it to start adding elements
			case '{':	
				DVPRINT("parseMIVectorLikeContToAIF: Open bracket\n");	
				//Append a 0 to atCoords
				atCoords = (int*)realloc(atCoords, (atCoordsSize+1)*sizeof(int));
				atCoords[atCoordsSize] = 0;
				atCoordsSize++;

				//currElem keeps track of the current base type element
				currElem = buf2+1;
				currElemLength = 0;
				
				++depth;
				break;
			case '}':
				DVPRINT("parseMIVectorLikeContToAIF: Closing bracket.\n");
				if(depth==maxDepth){
					//currElemShort is the relevant base type element string
					currElemShort=(char*)realloc(currElemShort, currElemLength+1);
					strncpy(currElemShort,  currElem, currElemLength);
					currElemShort[currElemLength] = '\0';

					DVPRINT("parseMIVectorLikeContToAIF: Closing bracket. Found element: (%s)\n", currElemShort);
					//For some reason the parser gets to this point with valid but empty strings. Only do stuff when the string is not empty
					if((*currElemShort != '\0'))
					{
						//Add AIF of the element we saw to AIF Array
						ac = CreateSimpleAIF(id, currElemShort, type_str, type_len, type_struct, type_func);
						tempVecDim = getVectorLikeDimensionStruct(shape, atCoords, atCoordsSize-1);
						printVectorLikeDimension(tempVecDim);
						AIFAddArrayElement(tempVecDim->aif, atCoords[atCoordsSize-1], ac);	
						tempVecDim->dimdef[atCoords[atCoordsSize-1]].aif = ac;
					}
				}
				if(atCoordsSize >= 1)
					atCoords = (int*)realloc(atCoords, (atCoordsSize-1)*sizeof(int));
				atCoordsSize--;

				currElem = buf2+1;
				currElemLength = 0;
				
				--depth;
				break;
			case ',':
				DVPRINT("parseMIVectorLikeContToAIF: Comma.\n");
				if(depth==maxDepth)
				{
					//currElemShort is the relevant base type element string
					currElemShort=(char*)realloc(currElemShort, currElemLength+1);
					strncpy(currElemShort,  currElem, currElemLength);
					currElemShort[currElemLength] = '\0';

					DVPRINT("parseMIVectorLikeContToAIF: Comma. Found element: (%s)\n", currElemShort);
					//For some reason the parser gets to this point with valid but empty strings. Only do stuff when the string is not empty
					if(strcmp("",currElemShort))
					{
						//Add AIF of the element we saw to AIF Array
						ac = CreateSimpleAIF(id, currElemShort, type_str, type_len, type_struct, type_func);
						tempVecDim = getVectorLikeDimensionStruct(shape, atCoords, atCoordsSize-1);
						printVectorLikeDimension(tempVecDim);
						AIFAddArrayElement(tempVecDim->aif, atCoords[atCoordsSize-1], ac);	
						tempVecDim->dimdef[atCoords[atCoordsSize-1]].aif = ac;
					}
					
				}

				//Fix atCoords and currElem appropriately
				atCoords[atCoordsSize-1]++;
				currElem = buf2+2;
				currElemLength = 0;
				break;
			
			//Avoid useless commas, part 2
			case 'w':
				if(strncmp(buf2,"with",4)==0)
				{
					buf2 = buf2+5;
					buf2dup = strdup(buf2);
					buf2DupOrig = buf2dup;
					lengthStr=strtok(buf2dup," ");
					buf2 = buf2 +strlen(lengthStr);
					DVPRINT("parseMIVectorLikeContToAIF: Skipping commas with\n");
					free(buf2DupOrig);
				}
				currElemLength++;
				break;

			//All other cases just increment currElemLength (element might have numerous digits/characters)
			default:
				currElemLength++;
				break;
				}
		//Move on to the next character
		buf2++;


		}
		
	DVPRINT("got here, returning...\n");
	connectAIFArrays(shape);
	fprintf(stderr, "shape->aif: %p\n", shape->aif);
	AIFPrint(stderr, 20, shape->aif);
	AIF * retval = shape->aif;
	
	//free memory
	free(buf1Orig);
	free(buf2Orig);
	free(atCoords);
	freeVectorLikeDimension(shape);
	
	return retval;
	
}

static AIF * parseMIArrayToAIF(	MISession *session, 
								MIVar *var,
								char *type_str, 
								int type_len, 
								struct_info_t * type_struct, 
								func_info_t * type_func,
								char * buf,
								int id,
								arrayDimension_t *	shape,
								AIF * a,
								AIF * ac)
{
	int			 		isComplex = 0;
	int					dcnt;
	int *				mcnt;
	int			 		depth = 0;
	int			 		done = 0;
	int			 		isFortran = 0;
	int			 		endCnt = 0;
	AIF **				abuild;
	char				*p1, *p2;

		// check if we are dealing with complex types here, by complex we mean
		// that gdb is going to enclose the basetype with curly braces (i.e. {...})
		// This presents a problem because these curly braces are indistinguishable
		// from the start of a new array. If the type is complex, then we use a
		// lookahead to ensure we have a base type.
		switch(id)
		{
			case T_COMPLEX_4:
			case T_COMPLEX_8:
			case T_COMPLEX_16:
			case T_UNION:
			case T_STRUCT:
				++isComplex;
			default:
				break;
		}
	// now that we have the string and array shape, we need to start
		// parsing it, find the first array member, and create a simple
		// aif object out of it.
		p1 = buf;
		
		/*
		 * Note that in the below switch statements we are going to check for
		 * any of the START, SEP, or END delimiters that are defined. We do this
		 * because switch statements require constant integers in their case 
		 * definitions, so we must check for any of the valid cases. 
		 */
		
		// ensure the first character is a start delimiter
		switch (*p1++)
		{
			// Fortran uses row major ordering
			case ARRAY_FTN_START:
				++isFortran;
			case ARRAY_C_START:
				break;
				
			case C_STRING_START:
				/*
				 * This is to handle the way gdb prints out arrays of chars. It will
				 * try to print it out in string format i.e. \"blah\". We need to
				 * handle this case.
				 */
				if(buf[strlen(buf)-1] == C_STRING_END)
				{
					buf[strlen(buf)-1] = '\0';
				}
				// Create the string directly
				a = StringToAIF(p1);
				// free the buf string
				free(buf);
				// free the array dimension info
				freeArrayDimension(shape);	
				return a;
				
			default:
				DVPRINT("GetArrayAIF: unknown array format.\n");
				freeArrayDimension(shape);
				return	(AIF *)NULL;
		}
		++depth;
		
		// now lets proceed into the string, ignoring ' ' and counting
		// depth along the way
		while (!done)
		{
			switch (*p1)
			{
				case ' ':
					// ignore it
					++p1;
					break;
					
				case ARRAY_C_START:
				case ARRAY_FTN_START:
					// Note that complex data values printed by gdb are of the form (r,i) and the start delimiter
					// is indistinguishable from the ARRAY_FTN_START delimiter. Therefore we need to have a look-ahead
					// to see if the next character is not ARRAY_FTN_START. The same is true for c with structs, unions,
					// etc but it is in the form {...}
					if (isComplex)
					{
						switch (*(p1+1))
						{
							case ARRAY_C_START:
							case ARRAY_FTN_START:
							case ' ':
								// still need to go deeper
								++depth;
								++p1;
								break;
								
							default:
								// we are pointing to the start of the (r,i) pair
								++done;
								break;
						}
						break;
					}
					
					// increment depth and continue
					++depth;
					++p1;
					break;
					
				default:
					// something other than ' ' or start_delim so we are done
					++done;
					break;
			}
		}
		
		// we now need to iterate through with p2 until we find a ',' or end delimiter
		p2 = p1+1;
		done = 0;
		
		while (!done)
		{
			switch (*p2)
			{
				case ARRAY_SEP:
					// only if we are complex
					if (isComplex)
					{
						// if we have not seen a ')' or '}' which ends the 
						// complex type, ignore the seperator.
						if (endCnt == 0)
						{
							//ignore it
							++p2;
							break;
						}
					}
					// at the end of this member...
					*p2 = '\0';
					++done;
					break;
					
				case ARRAY_C_END:
				case ARRAY_FTN_END:
					// special case if we are complex
					if (isComplex)
					{
						++endCnt;
						if (endCnt == 1)
						{
							// ignore it
							++p2;
							break;
						}
					}
					// at the end of this member and dimension
					*p2 = '\0';
					--depth;
					++done;
					break;
					
				default:
					// ignore it
					++p2;
					break;
			}
		}
		
		// we can now create the first data member's aif object and use
		// it to create our entire array regardless of dimension.
		ac = CreateSimpleAIF(id, p1, type_str, type_len, type_struct, type_func);

		// we need to allocate space for the abuild array
		// this will hold a list of each of our array dimensions which
		// are used while building the aif representation of the array
		if ((abuild = (AIF**)malloc(sizeof(AIF *) * shape->dim)) == (void *)0)
		{
			DVPRINT("GetArrayAIF: malloc error\n");
			free(buf);
			freeArrayDimension(shape);
			AIFFree(ac);
			AIFFree(a);
			return (AIF *)NULL;
		}
		
		// we need to allocate space for the mcnt array
		// this will hold a list of ints which is used to keep track of
		// the next open member index for each dimension
		if ((mcnt = (int*)malloc(sizeof(int) * shape->dim)) == (void *)0)
		{
			DVPRINT("GetArrayAIF: malloc error\n");
			free(abuild);
			free(buf);
			freeArrayDimension(shape);
			AIFFree(ac);
			AIFFree(a);
			return (AIF *)NULL;
		}
		// set the mcnt array to zero since we are currently at the
		// zeroeth index for each dimension
		memset(mcnt, 0, sizeof(int) * shape->dim);
			
		// now that we have adequate space to hold our aif array pointers
		// for each dimension, we can create the aif arrays.
		
		// !!!IMPORTANT!!! Fortran is col-major, meaning we need to transpose the array.
		
		// we start by creating the innermost array first, since this
		// is going to have entries of type ac
		if (isFortran)
		{
			DVPRINT("GetArrayAIF: building inner array of size %d at abuild index %d\n", getArrayDimension(shape, 1), shape->dim - 1);
			if ((abuild[shape->dim - 1] = EmptyFtnArrayToAIF(getArrayDimensionRangeMin(shape, 1), getArrayDimensionRangeMax(shape, 1), ac)) == (AIF *)NULL)
			{
				DVPRINT("GetArrayAIF: EmptyFtnArrayToAIF error\n");
				free(abuild);
				free(buf);
				freeArrayDimension(shape);
				AIFFree(ac);
				AIFFree(a);
				return (AIF *)NULL;
			}
			
			// now we create each of the outer dimensions using its inner dimension
			// child as its aif ref type. This will ensure that we have enough space
			// in our aif object the the child arrays.
			for (dcnt=2; dcnt <= shape->dim; ++dcnt)
			{
				DVPRINT("GetArrayAIF: building outter array of size %d at abuild index %d\n", getArrayDimension(shape, dcnt), shape->dim - dcnt);
				// the abuild index at dcnt is a 0..n array of type abuild[shape->dim-dcnt+1]
				abuild[shape->dim - dcnt] = EmptyFtnArrayToAIF(getArrayDimensionRangeMin(shape, dcnt), 
											getArrayDimensionRangeMax(shape, dcnt), 
											abuild[shape->dim - dcnt + 1]);
				if ( abuild[shape->dim - dcnt] == (AIF *)NULL)
				{
					DVPRINT("GetArrayAIF: EmptyArrayToAIF error\n");
					dcnt = shape->dim;
					while (--dcnt >= 0)
					{
						AIFFree(abuild[dcnt]);
					}
					free(abuild);
					free(buf);
					freeArrayDimension(shape);
					AIFFree(ac);
					AIFFree(a);
					return (AIF *)NULL;
				}
			}
			
			// set the mcnt array to the actual min range value for each dimension. Fortran allows for custom ranges to be used
			for (dcnt = 1; dcnt <= shape->dim; ++dcnt)
			{
				mcnt[shape->dim - dcnt] = getArrayDimensionRangeMin(shape, dcnt);
				DVPRINT("GetArrayAIF: Set mcnt[%d] to %d\n", shape->dim - dcnt, mcnt[shape->dim - dcnt]);
			}
			
			// since we already created the first data member, lets add it
			// to the abuild array at the previous depth if depth is non-zero
			// or else at zero, meaning we had only one element in our array.
			if (0 < depth)
			{
				AIFAddFtnArrayElement(abuild[depth-1], mcnt[depth-1]++, ac);
			} else
			{
				// we are done building the array. There is only 1 element.
				AIFAddFtnArrayElement(abuild[0], mcnt[0]++, ac);
			}
		} else
		{		
			DVPRINT("GetArrayAIF: building inner array of size %d at abuild index %d\n", getArrayDimension(shape, shape->dim), shape->dim - 1);
			if ((abuild[shape->dim - 1] = EmptyArrayToAIF(0, getArrayDimension(shape, shape->dim)-1, ac)) == (AIF *)NULL)
			{
				DVPRINT("GetArrayAIF: EmptyArrayToAIF error\n");
				free(abuild);
				free(buf);
				freeArrayDimension(shape);
				AIFFree(ac);
				AIFFree(a);
				return (AIF *)NULL;
			}
		
			// now we create each of the outer dimensions using its inner dimension
			// child as its aif ref type. This will ensure that we have enough space
			// in our aif object the the child arrays.
			for (dcnt=shape->dim - 2; dcnt >= 0; --dcnt)
			{
				DVPRINT("GetArrayAIF: building outter array of size %d at abuild index %d\n", getArrayDimension(shape, dcnt+1), dcnt);
				// the abuild index at dcnt is a 0..n array of type abuild[dcnt+1]
				if ((abuild[dcnt] = EmptyArrayToAIF(0, getArrayDimension(shape, dcnt+1)-1, abuild[dcnt+1])) == (AIF *)NULL)
				{
					DVPRINT("GetArrayAIF: EmptyArrayToAIF error\n");
					while (--dcnt >= 0)
					{
						AIFFree(abuild[dcnt]);
					}
					free(abuild);
					free(buf);
					freeArrayDimension(shape);
					AIFFree(ac);
					AIFFree(a);
					return (AIF *)NULL;
				}
			}
			
			// since we already created the first data member, lets add it
			// to the abuild array at the previous depth if depth is non-zero
			// or else at zero, meaning we had only one element in our array.
			if (0 < depth)
			{
				AIFAddArrayElement(abuild[depth-1], mcnt[depth-1]++, ac);
			} else
			{
				// we are done building the array. There is only 1 element.
				AIFAddArrayElement(abuild[0], mcnt[0]++, ac);
			}
		}
		
		// free the first member, we are done with it
		AIFFree(ac);
		
		while (0 < depth)
		{
			// point p1 one past p2
			// p2 points to the '\0' at the end of the last string
			p1 = ++p2;
			done = 0;
			
			while (!done)
			{
				switch(*p1)
				{
					case ' ':
						// ignore it
						++p1;
						break;			
						
					case ARRAY_SEP:
						// ignore it
						++p1;
						break;
						
					case ARRAY_C_START:
					case ARRAY_FTN_START:
						// special case for complex
						if (isComplex)
						{
							switch (*(p1+1))
							{
								case ARRAY_C_START:
								case ARRAY_FTN_START:
								case ' ':
									// still need to go deeper
									++depth;
									++p1;
									break;
								
								default:
									// we are pointing to the start of (r,i) or {...}
									++done;
									break;
							}
							break;
						}
						// increment depth and continue
						++depth;
						++p1;
						break;
						
					case ARRAY_C_END:
					case ARRAY_FTN_END:
						// we just finished adding the final entry for the
						// current dimension
						
						// ensure that depth is greater than 0
						if (--depth == 0)
						{
							DVPRINT("GetArrayAIF: Done building.\n");
							++done;
							break;
						}
						
//						DVPRINT("GetArrayAIF: Done with %d\n", mcnt[depth-1]);
						
						if (isFortran)
						{
							// lets add it to its parent
							AIFAddFtnArrayElement(abuild[depth-1], mcnt[depth-1]++, abuild[depth]);
							
							// reset the current depth mcnt
							// note that fortran needs to reverse the dimensionality hence the dim minus current depth
//							DVPRINT("GetArrayAIF: p1 depth is %d\n", depth);
							mcnt[depth] = getArrayDimensionRangeMin(shape, shape->dim - depth);
						} else
						{
							// lets add it to its parent
							AIFAddArrayElement(abuild[depth-1], mcnt[depth-1]++, abuild[depth]);
						
							// reset the current depths mcnt
							mcnt[depth] = 0;
						}
						
						// continue parsing
						++p1;
						break;
						
					default:
						// something other than ' ' ',' start_delim or end_delim
						// start of a new value
						++done;
						break;	 
				}
			}
			
			// ensure depth is not zero
			if (depth == 0)
				continue;
				
			// reset done and set p2 to p1+1
			p2 = p1+1;
			done = 0;
			
			// reset the complex counters
			endCnt = 0;
			
			while (!done)
			{
				switch (*p2)
				{
					case ARRAY_SEP:
						// only if we are complex
						if (isComplex)
						{
							if (endCnt == 0)
							{
								//ignore it
								++p2;
								break;
							}
						}
						// at the end of this member
						*p2 = '\0';	
						
						// lets create the simple aif variable from it
						ac = CreateSimpleAIF(id, p1, type_str, type_len, type_struct, type_func);
						
						// now we add it to the current abuild at mcnt
						if (isFortran)
						{
							AIFAddFtnArrayElement(abuild[depth-1], mcnt[depth-1]++, ac);
						} else
						{
							AIFAddArrayElement(abuild[depth-1], mcnt[depth-1]++, ac);
						}
						
						// free the temporary aif object
						AIFFree(ac);
						
						// we are done
						++done;
						break;
						
					case ARRAY_C_END:
					case ARRAY_FTN_END:
						// special case if we are complex
						if (isComplex)
						{
							++endCnt;
							if (endCnt == 1)
							{
								// ignore it
								++p2;
								break;
							}
						}
						// at the last member in the current dimension
						*p2 = '\0';
						
						// lets create the simple aif variable from it
						ac = CreateSimpleAIF(id, p1, type_str, type_len, type_struct, type_func);
						
						// now we add it to the current abuild at mcnt
						if (isFortran)
						{
							AIFAddFtnArrayElement(abuild[depth-1], mcnt[depth-1], ac);
							
							// reset mcnt for this dimension since we are done with it
							// note that fortran can have non-standard ranges
//							DVPRINT("GetArrayAIF: p2 depth is %d\n", depth);
							mcnt[depth-1] = getArrayDimensionRangeMin(shape, shape->dim - depth + 1);
						} else
						{
							AIFAddArrayElement(abuild[depth-1], mcnt[depth-1], ac);
							
							// reset mcnt for this dimension since we are done with it
							mcnt[depth-1] = 0;
						}
						
						// free the temporary aif object
						AIFFree(ac);
						
						// ensure that depth is greater than 0
						if (--depth == 0)
						{
							++done;
							break;
						}
						
						// lets add it to its parent
						if (isFortran)
						{
							AIFAddFtnArrayElement(abuild[depth-1], mcnt[depth-1]++, abuild[depth]);
						} else
						{
							AIFAddArrayElement(abuild[depth-1], mcnt[depth-1]++, abuild[depth]);
						}
						
//						DVPRINT("GetArrayAIF: Done with %d\n", mcnt[depth-1]);
						
						// we are done
						++done;
						break;
						
					default:
						// ignore it
						++p2;
						break;
				}
			}
		}
		
		// our final return array is going to be contained in abuild[0]
		a = abuild[0];
		
		// cleanup the other abuild entries, we are done with them
		for (dcnt = 1; dcnt < shape->dim; ++dcnt)
		{
			AIFFree(abuild[dcnt]);
		}
		// free abuild
		free(abuild);
		// free mcnt
		free(mcnt);
		// free the buf string
		free(buf);
		// free the array dimension info
		freeArrayDimension(shape);		
	return a;
}

static AIF *
GetArrayAIF(MISession *session, MIVar *var)
{
	int					id;
	char *				type_str;
	int					type_len;
	struct_info_t *		type_struct;
	func_info_t *		type_func;
	char *				buf = NULL;
	arrayDimension_t *	shape = NULL;
	AIF *				a = NULL;
	AIF *				ac = NULL;
	
	DVPRINT("GetArrayAIF: %s\n", var->expr);
	
	if ((id = GetArrayBaseType(session, var, &type_str, &type_len, &type_struct, &type_func)) == T_UNKNOWN)
	{
		DVPRINT("GetArrayAIF: Unknown basetype for array!\n");
		return (AIF *)NULL;
	}

	if (var->numchild <= 0) 
	{
		DVPRINT("GetArrayAIF: No children detected.\n");
		ac = CreateSimpleAIF(id, var->value, type_str, type_len, type_struct, type_func);
		a = EmptyArrayToAIF(0, 0, ac);
		AIFFree(ac);
	} else 
	{
		DVPRINT("GetArrayAIF: %d children detected.\n", var->numchild);
		
		// first we need to get the data string
		buf = GetArrayDataString(session, var);

		// Now lets get the shape of this array
		if ((shape = GetArrayDimensionShape(session, var)) == (arrayDimension_t *)NULL)
		{
			// error already set
			DVPRINT("GetArrayAIF: GetArrayDimensionShape error\n");
			return NULL;
		}
	}
	return parseMIArrayToAIF(session, var, type_str, type_len, type_struct, type_func, buf, id, shape, a, ac);
}

static AIF *
GetVectorLikeAIF(MISession *session, MIVar *var)
{
	int					id;
	char *				type_str;
	int					type_len;
	struct_info_t *		type_struct;
	func_info_t *		type_func;
	char *				buf = NULL;
	vectorLikeDimension_t *	shape = NULL;
	AIF * ac = NULL;
	AIF * a = NULL;
	
	DVPRINT("GetArrayAIF: %s\n", var->expr);
	
	if ((id = GetArrayBaseType(session, var, &type_str, &type_len, &type_struct, &type_func)) == T_UNKNOWN)
	{
		DVPRINT("GetArrayAIF: Unknown basetype for array!\n");
		return (AIF *)NULL;
	}
	DVPRINT("id: %d\n", id);
	if (var->numchild <= 0) 
	{
		DVPRINT("GetArrayAIF: No children detected.\n");
		ac = CreateSimpleAIF(id, var->value, type_str, type_len, type_struct, type_func);
		a = EmptyArrayToAIF(0, 0, ac);
		AIFFree(ac);
	} else 
	{
		DVPRINT("GetArrayAIF: %d children detected.\n", var->numchild);
		
		// first we need to get the data string
		buf = GetArrayDataString(session, var);

		// Now lets get the shape of this array
		if ((shape = GetVectorLikeDimensionShape(buf)) == (vectorLikeDimension_t *)NULL)
		{
			// error already set
			DVPRINT("GetArrayAIF: GetArrayDimensionShape error\n");
		
	return NULL;
		}
	}
	buf = strtok(buf, "=");
	buf = strtok(NULL, "");
	buf++;
	return parseMIVectorLikeContToAIF(type_str, type_len, type_struct, type_func, buf, id, shape);
}

static AIF * 
GetVectorAIF(MISession *session, MIVar *var)
{
	return GetVectorLikeAIF(session, var);


}

static AIF * 
GetDequeAIF(MISession *session, MIVar *var)
{
	return GetVectorLikeAIF(session, var);
}

static AIF * 
GetStackAIF(MISession *session, MIVar *var)
{
	return GetVectorLikeAIF(session, var);
}

static AIF * 
GetQueueAIF(MISession *session, MIVar *var)
{
	return GetVectorLikeAIF(session, var);
}

static AIF * 
GetPQueueAIF(MISession *session, MIVar *var)
{
	return GetVectorLikeAIF(session, var);
}

static AIF *
GetPointerAIF(MISession *session, MIVar *var)
{
	void *			addr;
	char *			addr_str;
	long long int	addr_loc;
	int				addr_len;
	AIF *			addr_aif;
	AIF *			rtn_aif;
	
	DVPRINT("GetPointerAIF: Pointer Type => %s\n", var->type);

	// get the address for the pointer, this is represented by the value
	// in the MI object.
	if ((addr_str = GetVarValue(session, var->name)) == NULL)
	{
		// error already set
		return NULL;
	}
	
	DVPRINT("GetPointerAIF: Pointer Addr => %s\n", addr_str);
	
	// convert the address of the pointer to an integer value
	addr_loc = strtoll(addr_str, NULL, 0);

	// Make the void pointer point at the memory location of the pointer
	addr = (void *)addr_loc;

	// We need to ask for the length of the address type for this particular
	// pointer.
	addr_len = GetTypeLength(session, var->type);
	
	DVPRINT("GetPointerAIF: Pointer Addr Length => %d\n", addr_len);

	// Create the AIF type representing the address
	addr_aif = AddressToAIF(addr, addr_len);

	// Now create the AIF generic pointer representing both the address that
	// we actually point to and the type string.
	rtn_aif = GenericPointerToAIF(addr_aif, var->type);

	AIFFree(addr_aif);
	
	return rtn_aif;
}

