/* interface of MRNet frontend for Guard client*/
#ifndef _GUARD_SCI_FRONTEND_H
#define _GUARD_SCI_FRONTEND_H

#include "dbg_sci.h"
#include "dbg_set_sync.h"
#include "bitset.h"

/* initialize data structures for SCI fontend*/
extern void	sci_fe_initialize(char* jobkey, char* agent_path, char* lib_path, char* hfile, char* be_path, char** beenvp);

/* launch SCI frontend without filter*/
extern int	sci_fe_create();

/* launch SCI frontend with filter*/
extern int	sci_fe_create_with_filter(char * cmd_filter_path, int cmd_filter_id, char* event_filter_path, int event_filter_id);

/*--------------------------------------------------------------------------*/
/*------ -The following APIs are valid only after SCI is launched --------*/
/*--------------------------------------------------------------------------*/


/* return the number of backends in SCI */
extern int			sci_fe_get_num_of_be();

/* send commands to backends via SCI groups*/
extern int			sci_fe_send_command(bitset *target, char *msg);

/* collect replies from backends via the command stream*/
//extern int 			sci_fe_recv_reply(bitset *target, char **msg, bitset **source);
extern int			sci_fe_recv_reply(bitset *target, rep_set_t ***rep, int* number);

/* set the callback handle events and I/O */
extern void 		sci_fe_set_callback(void (*callback)());

/* detect if any event or i/o messages arrive */
extern int			sci_fe_check_io_and_event();

/* consume waiting events */
extern int	 		sci_fe_handle_event();

/* consume waiting I/O */
extern int			sci_fe_handle_io();

/* called when invoke completes. To be compatible with the existing callback hierachy in Guard */
extern int			sci_fe_set_invoke_completed();

/* shutdown all SCI networks */
extern int 			sci_fe_shutdown();

#endif /* _GUARD_SCI_FRONTEND_H */
