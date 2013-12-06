/*
** Debug protocol definitions
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

// TODO: This interface could use proper documentation since it's an abstraction

#ifndef _DBGCLNT_PROTO_H
#define _DBGCLNT_PROTO_H

#include "bitset.h"
#include "proto_message.h"

// forward declare struct dbginvoke - Note that because the struct dbginvoke is
// first encountered inside of a list of parameters for the function pointer, we
// *must* forward declare it otherwise the compiler will mess up.
struct dbginvoke;

typedef void *	proto_network;	// This is data used by the protocol layer 
                                // that is allocated on a per dbginvoke basis.
       
// Used to keep track of sessions associated with the proxy based dbgclnt_proto implementation                             
struct proto_session
{
	struct dbgclnt_proto *	sess_proto;
	void *					sess_obj;	// This is data used by the protocol layer
										// that is allocated on a per session basis.
};
typedef struct proto_session	proto_session;

// Some protocols will interact with the workload manager. Known workload managers
// are defined in the following enumeration. Note that the name cooresponds to
// the job submission command used by the WLM.
enum WLM_type
{
	WLM_none,
	WLM_qsub,
	WLM_sbatch
};
typedef enum WLM_type WLM_type;

// main protocol definition
struct dbgclnt_proto
{
	char *					sw_name;												/* Name of the protocol */
	
	int						(*sw_init)(void);										/* protocol init function */
	void					(*sw_finish)(void);										/* protocol finish function */
	
	bitset *				(*sw_send_cmd)(proto_network, bitset *, char *);		/* protocol send command function */
	struct proto_msg_set *	(*sw_recv_reply)(proto_network, bitset *, bitset *);	/* protocol receive reply function */
	
	int						(*sw_checkevents)(struct dbginvoke *);					/* protocol check for ready event function */
	void					(*sw_dispatchevents)(struct dbginvoke *, int);			/* protocol event dispatch function */

	/*
	** Client protocol routines.
	*/
	proto_session *			(*sw_create_session)(	struct dbgclnt_proto *, 
													WLM_type, 
													char *, 
													char *, 
													char *);						/* protocol create session function */
	void					(*sw_save_session)(proto_session *);
	void					(*sw_cleanup_session)(proto_session *);					/* protocol cleanup session function - called when a session is no longer valid */
	struct dbginvoke *		(*sw_attach)(	struct dbgclnt_proto *, 
											proto_session *,
											int,
											char *, 
											char *, 
											int,
											int);									/* protocol attach function */
	struct dbginvoke *		(*sw_invoke)(	struct dbgclnt_proto *,
											proto_session *,
											char *,
											char **,
											int,
											char *,
											char **,
											char *,
											char *,
											char **,
											char *,
											int,
											int);									/* protocol launch function */
	int						(*sw_exit)(struct dbginvoke *, int detach);				/* protocol exit function */
	void					(*sw_destroyclnt)(struct dbginvoke *);					/* protocol destroy dbginvoke specific instance */
};
typedef struct dbgclnt_proto		dbgclnt_proto;

/*
** Function definitions
*/
extern void				SetClntProto(void);
extern dbgclnt_proto *	GetClntProto(void);
extern dbgclnt_proto *	FindClntProto(char *);

extern int				WaitForProtoStartupEvent(struct dbginvoke *);

#endif /* !_DBGCLNT_PROTO_H */
