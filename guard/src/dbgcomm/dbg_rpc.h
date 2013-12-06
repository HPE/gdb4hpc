/*
** RPC utility routines.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _DBG_RPC_H
#define _DBG_RPC_H

extern u_long		FindRPCProgNum(SVCXPRT *, u_long, u_long, void (*) ());
extern int		CreateSvc(SVCXPRT **, u_long *, void (*)(struct svc_req *, SVCXPRT *), u_long);
extern char *		StrDup(char *);
extern bpinfo *		EventBPToRes(dbgbp_t *);
extern frameentry *	EventFrameToRes(dbgframe_t *);
extern location *	EventLocToRes(dbgloc_t *);
extern sclist *		EventListToRes(dbglist_t *);
extern void		EventToRes(dbg_res *, dbgevent_t *);
extern dbgloc_t *	ResToLoc(location *);
extern dbglist_t *	ResToList(sclist *);
extern dbgbp_t *	ResToBP(bpinfo *);
extern dbgframe_t *	ResToFrame(frameentry *);
extern dbgevent_t *	ResToEvent(dbg_res *);

#endif /* ! _DBG_RPC_H */
