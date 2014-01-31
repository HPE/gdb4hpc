/*********************************************************************************\
 * Main grammar include file.
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



#ifndef _GRAM_H
#define _GRAM_H

#include <stdarg.h>

#include "slice.h"
#include "perm.h"
#include "line.h"
#include "file.h"
#include "val.h"
#include "var.h"
#include "assign.h"
#include "func.h"
#include "invoke.h"
#include "expr.h"
#include "cmdtoken.h"
#include "blockmap.h"
#include "blockmap_util.h"
#include "procset.h"
#include "bitset.h"
#include "dbg_set_sync.h"
#include "hash.h"

#define	 PROCSET_STRUCT	"procset_struct"
#define	 PROCSET_SIZE		"procset_size"
#define	 PROCSET_RANK		"procset_rank"

enum CMD_MODE
{
    CMD_MODE_INTERACTIVE, /*Interactive command*/
    CMD_MODE_DEFERRED    /*Deferred command*/
};
typedef enum CMD_MODE CMD_MODE;

/*
** expr.h
*/
extern expr_t *			ParseExpr(char *);
extern void				FreeExpr(expr_t *);
extern expr_t *			AddExpr(expr_op, ...);
extern void				SaveTopElt(void);
extern expr_op			GetTopEltOp(void);
extern hentry_t *		GetTopEltSymVal(void);
extern void				RemoveTopElt(void);
extern int				EvalCompare(val_t **, val_t **, expr_t *, rec_set_t *);
extern int				EvalExpr(val_t **, expr_t *, rec_set_t *);
extern int				EvalExprList(list_t **, expr_t *, rec_set_t *);
extern int				EvalFunc(val_t **, func_t *, list_t *, rec_set_t *);
extern g_block_map_t *	EvaluateBlockMap(expr_t *, rec_set_t *, CMD_MODE);
extern var_t *			ReadVar_BlockMap(expr_t *e, rec_set_t *rec_set, CMD_MODE mode);
extern var_t *			ReadVarDecl(expr_t *, rec_set_t *);
extern var_t *			ReadVarImm(expr_t *, rec_set_t *);
extern int				AssignExprImm(expr_t *, expr_t *, rec_set_t *);
extern void				DumpExpr(expr_t *);
extern void				StartExprList(void);
extern void				IncExprList(void);
extern void				SetExprList(int);
extern int				EndExprList(void);
extern void				ClearUserSpecProcSet();
extern int				CheckUserSpecProcSet();

/*
** func.h
*/
extern void				InitFunc(void);
extern void				FinishFunc(void);
extern func_t *			NewFunc(hentry_t *, list_t *, expr_t *, int (*)(list_t *, val_t **), char *, int);
extern void				FreeFunc(func_t *);
extern func_t *			FindFunc(hentry_t *);
extern int				CheckFunc(hentry_t *);
extern void				SetFunc(void);
extern func_t *			GetFunc(void);
extern arg_t *			NewArg(int, ...);
extern void				DestroyArg(arg_t *);
extern int				FindArg(arg_t **, hentry_t *, func_t *);
extern void				PrintFunc(FILE *, func_t *);

/*
** invoke.h
*/
extern int				CreateInvokeGroupList(expr_t *, list_t *, rec_set_t *);
extern int				SetProcsetForInvokeGroup(invoke_group *, dbgprocset_t *);
extern invoke_t *		AddInvoke(char *, list_t *, char *, void *, void*, bitset*, int);
extern invoke_group *	FindInvokeGroup(hentry_t *);
extern int				SetInvoke(void);
extern invoke_t *		GetInvoke(void);
extern void				SetInvokeGroup(invoke_t *);
extern invoke_group *	GetInvokeGroup(invoke_t *);
extern void				FreeInvoke(invoke_t *);
extern void				FreeInvokeGroup(invoke_group *);
extern void				FreeInvokeGroupList(list_t *);
extern void				PrintProcSetInfo(FILE *fp, char *setname, bitset *set);
extern void			 	PrintProcSetInfoByOffset(FILE *fp, char *setname, bitset *set, int offset, int len);
extern void				PrintMsgWithSetInfo(FILE *fp, char* setname, bitset* group, char *f, ...);
extern void				PrintMsgWithSetInfoByOffset(FILE *fp, char* setname, bitset* group, int offset, int len, char *f, ...);

/*
** blockmap.h
*/
extern g_block_map_t *	FindBlockMap(char *);
extern int				CheckBlockMap(hentry_t *);
extern int				VerifyBlockMap(hentry_t *);
extern void				SetBlockMap(void);
extern g_block_map_t *	GetBlockMap(void);
extern g_block_map_t *	NewBlockMap(hentry_t *);
extern void				DestroyBlockMap(g_block_map_t *);
extern AIF *			ApplyBlockMap(g_block_map_t *, char *, int, AIF **, char *);
extern void				PrintBlockMap(FILE *, g_block_map_t *);
extern void				FinishBlockMap(void);

/*
** val.h
*/
extern void				InitVal(void);
extern void				FinishVal(void);
extern val_t *			MakeTransitoryLval(hentry_t *);
extern int				RetrieveContents(val_t *, rec_set_t *);
extern rval_type		GetActualRvalType(val_t *);
extern AIF *			GetActualAIF(val_t *);
extern dbgprocset_t *	GetActualProcset(val_t *);
extern val_t *			NewVal(lval_type, ...);
extern void				DestroyRval(rval_type, rval_val);
extern void				DestroyVal(val_t *);
extern void				DestroyInternalVal(val_t *);
extern rval_val			CopyRval(rval_type, rval_val);
extern val_t *			CopyVal(val_t *);
extern val_t *			GetVal(hentry_t *);
extern int				AssignVal(val_t *, val_t *, rec_set_t *);
extern void				RemoveVal(hentry_t *);

/*
** cmdtoken.h
*/
extern char *			SymLookup(hentry_t *);
// andrewg: The data types in the hash table really should be void *. The hash
// table implementation is a common interface used with many different types of
// data. 
extern void *			SymLookupData(hentry_t *);
extern void				SetSymData(hentry_t *, void *);
extern hentry_t *		AddSym(char *);
extern void				RemoveSym(hentry_t *);
extern hentry_t *		FindSym(char *);
extern ct_type			FindCmdToken(char *);
extern ct_type			FindCmdTokenStr(char *, ct_val *);
extern ct_type			ScanString(ct_val *, char **);
extern int				InitCmdToken(void);
extern void				FinishCmdToken(void);

/*
** procset.h
*/
extern void				InitProcSet(void);
extern void				FinishProcSet(void);
extern void				SetProcSet(void);
extern dbgprocset_t *	GetProcSet(void);
extern dbgprocset_t *	NewProcSet(char *);
extern void				FreeProcSet(dbgprocset_t *);
extern void				SetCurrentProcSet(dbgprocset_t *);
extern dbgprocset_t *	FindProcSet(char *name);
extern dbgprocset_t *	CurrentProcSet(void);
extern int				ParseProcSet(dbgprocset_t *, char *, rec_set_t *);
extern void				PrintProcSet(FILE *, dbgprocset_t *);
extern void				AddOverlaySetToProcSet(dbgprocset_t *, invoke_t *, invoke_group*, bitset *, int, int, int, dbgset_type);
extern list_t*			GetOverlaySetListByTree(void *);
extern list_t*			GetOverlaySetListByDbgInvoke(void *);
extern void				SetOverlaySetForProcSet(dbgprocset_t *);
extern dbg_over_set_t*	GetNextOverlaySetForProcSet(dbgprocset_t *);
extern dbg_over_set_t*	GetOverlaySetForProcSet(dbgprocset_t *);
 
/* 
**Output routines defined by client/server interface 
*/
extern void				PrintOutError(char *, ...);
extern void				PrintOutMsg(char *, ...);
extern void				PrintToFd(FILE *, char *, ...);
extern void				vPrintToFd(FILE *, char *, va_list);
 
#endif /* !_GRAM_H */
