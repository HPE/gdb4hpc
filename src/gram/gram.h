/*********************************************************************************\
 * Main grammar include file.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2014,2020-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
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

#include "bitset.h"
#include "blockmap.h"
#include "blockmap_util.h"
#include "cmdtoken.h"
#include "dbg_client.h"
#include "expr.h"
#include "func.h"
#include "hash.h"
#include "procset.h"
#include "slice.h"
#include "val.h"
#include "var.h"

enum CMD_MODE {
    CMD_MODE_INTERACTIVE,  // Interactive command
    CMD_MODE_DEFERRED      // Deferred command
};
typedef enum CMD_MODE CMD_MODE;

/*
** expr.h
*/
extern expr_t *ParseExpr(const char *);
extern void FreeExpr(expr_t *);
extern expr_t *AddExpr(expr_op, ...);    // called by yacc expr.y.in
extern void SaveTopElt(void);            // called by yacc expr.y.in
extern expr_op GetTopEltOp(void);        // called by yacc expr.y.in
extern hentry_t *GetTopEltSymVal(void);  // called by yacc expr.y.in
extern void RemoveTopElt(void);          // called by yacc expr.y.in
extern int EvalCompare(val_t **, val_t **, const expr_t *, rec_set_t *);
extern int EvalExpr(val_t **, const expr_t *, rec_set_t *);
extern int EvalExprList(list_t **, expr_t *, rec_set_t *);
extern g_block_map_t *EvaluateBlockMap(const expr_t *, rec_set_t *, CMD_MODE);
extern var_t *ReadVarDecl(expr_t *, rec_set_t *);
extern int AssignExpr(expr_t *, const expr_t *, rec_set_t *);
extern void StartExprList(void);  // called by yacc expr.y.in
extern void IncExprList(void);    // called by yacc expr.y.in
extern void SetExprList(int);     // called by yacc expr.y.in
extern int EndExprList(void);     // called by yacc expr.y.in

/*
** func.h
*/
extern void InitFunc(void);
extern void FinishFunc(void);
extern func_t *NewFunc(hentry_t *, list_t *, expr_t *,
                       int (*)(list_t *, val_t **), const char *, int);
extern void FreeFunc(func_t *);
extern func_t *FindFunc(hentry_t *);
extern int CheckFunc(hentry_t *);
extern void SetFunc(void);
extern func_t *GetFunc(void);
extern arg_t *NewArg(int, ...);
extern void DestroyArg(arg_t *);
extern int FindArg(arg_t **, hentry_t *, func_t *);
class cli_out_t;
extern void PrintFunc(cli_out_t&, func_t *);

/*
** blockmap.h
*/
extern g_block_map_t *FindBlockMap(const char *);
extern int CheckBlockMap(hentry_t *);
extern int VerifyBlockMap(hentry_t *);
extern void SetBlockMap(void);
extern g_block_map_t *GetBlockMap(void);
extern g_block_map_t *NewBlockMap(hentry_t *);
extern void DestroyBlockMap(g_block_map_t *);
extern void PrintBlockMap(FILE *, g_block_map_t *);
extern void FinishBlockMap(void);

/*
** val.h
*/
extern void InitVal(void);
extern void FinishVal(void);
extern val_t *MakeTransitoryLval(hentry_t *);
extern int RetrieveContents(val_t *, rec_set_t *);
extern rval_type GetActualRvalType(val_t *);
extern AIFStream *GetActualAIF(val_t *);
extern dbgprocset_t *GetActualProcset(val_t *);
extern val_t *NewVal(lval_type, ...);
extern void DestroyRval(rval_type, rval_val);
extern void DestroyVal(val_t *);
extern void DestroyInternalVal(val_t *);
extern rval_val CopyRval(rval_type, rval_val);
extern val_t *CopyVal(val_t *);
extern val_t *GetVal(hentry_t *);
extern int AssignVal(val_t *, val_t *, rec_set_t *);
extern void RemoveVal(hentry_t *);
hentry_t* FindVal(const char* name);

/*
** cmdtoken.h
*/
extern char *SymLookup(hentry_t *);
// andrewg: The data types in the hash table really should be void *. The hash
// table implementation is a common interface used with many different types of
// data.
extern void *SymLookupData(hentry_t *);
extern void SetSymData(hentry_t *, void *);
extern hentry_t *AddSym(char *);
extern void RemoveSym(hentry_t *);
extern hentry_t *FindSym(char *);
extern ct_type FindCmdToken(char *);
extern ct_type FindCmdTokenStr(char *, ct_val *);
extern ct_type ScanString(ct_val *, const char **);
extern int InitCmdToken(void);
extern void FinishCmdToken(void);

/*
** procset.h
*/
extern void InitProcSet(void);
extern void FinishProcSet(void);
dbgprocset_t *CreateTempProcSetSlice(dbgprocset_t *, slice_t *);
extern int CreateProcSet(expr_t *, expr_t *);
extern dbgprocset_t *CreateProcSetForInvoke(expr_t *e, dbgprocset_t*& isInUse);
extern int FinalizeProcSetForInvoke(dbgprocset_t *ps, DbgInvoke *di);
extern rec_set_t *CreateRecSetForProcset(const dbgprocset_t *ps);
extern void SetProcSet(void);
extern dbgprocset_t *GetProcSet(void);
extern void FreeProcSet(dbgprocset_t *);
extern void SetCurrentProcSet(dbgprocset_t *);
extern dbgprocset_t *FindProcSet(const char *name);
extern dbgprocset_t *CurrentProcSet(void);
extern void PrintProcSet(dbgprocset_t *);
extern void SetOverlaySetForProcSet(dbgprocset_t *);
extern dbg_over_set_t *GetNextOverlaySetForProcSet(dbgprocset_t *);
extern void PrintMsgWithSetInfo(const char *setname, const Bitset *group,
                                const char *f, ...) __attribute__((format (printf,3,4)));
extern void PrintMsgWithSetAndThreadInfo(const char *setname, const Bitset *group,
                                         const char* threads,
                                         const char *f, ...) __attribute__((format (printf,4,5)));

extern void PrintErrorWithSetInfo(const char *setname, const Bitset *group,
                                  const char *f, ...) __attribute__((format (printf,3,4)));
extern void PrintErrorWithSetAndThreadInfo(const char *setname, const Bitset *group,
                                           const char* threads,
                                           const char *f, ...) __attribute__((format (printf,4,5)));


/*
** slice.h
*/

#endif /* !_GRAM_H */
