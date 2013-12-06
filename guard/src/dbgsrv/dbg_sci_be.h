/* interface of MRNet backend for Guard server*/
#ifndef _GUARD_SCI_BACKEND_H
#define _GUARD_SCI_BACKEND_H

#include "dbg_sci.h"

extern int		sci_be_create_debug();//TODO: remove me

extern int 		sci_be_create();

extern void		sci_be_set_size(int size);

//extern void	 	sci_be_set_localrank(int rank);
extern int	 	sci_be_get_localrank();

extern void 	sci_be_set_callback(void (*callback)());
extern int	 	sci_be_handle_msg();

extern int 		sci_be_recv();
extern int		sci_get_polling_fd(int* polling_fd);

extern int		sci_be_send_reply(sci_group_t group, int seq, char *msg);
extern int		sci_be_send_event(char* msg);
extern int		sci_be_send_io(char* msg);

extern int		sci_be_is_shutdown();

extern int 		sci_be_shutdown();

#endif /* _GUARD_SCI_BACKEND_H */
