/*********************************************************************************\
 * dbg_mrn_be.h
 *
 * © 2010-2019 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/



/* interface of MRNet backend for Guard server*/
#ifndef _GUARD_MRNET_BACKEND_H
#define _GUARD_MRNET_BACKEND_H

#include "dbg_mrn.h"

extern int			mrn_be_create_network(int argc, char* argv[]);

extern void			mrn_be_set_size(int size);

extern void			mrn_be_set_localrank(int rank);
extern int			mrn_be_get_localrank();

extern void			mrn_be_set_callback(void (*callback)(char*));
extern void			mrn_be_handle_msg();

extern void			mrn_be_set_cmd_stream(MRNet_Stream *stream);
extern void			mrn_be_set_event_stream(MRNet_Stream *stream);
extern void			mrn_be_set_event_stream_from_id(unsigned int streamId);
extern void			mrn_be_set_io_stream(MRNet_Stream *stream);
extern int			mrn_be_get_event_fd();
extern void			mrn_be_clear_event_fd();
extern unsigned int	mrn_be_num_events_pending();
extern void			mrn_be_ack_event();
extern void			mrn_be_clear_events();

extern int			mrn_be_recv(struct MRNet_Stream **stream, int *tag, int blocking);

extern int			mrn_be_send_reply(MRNet_Stream *stream, char* msg);
extern int			mrn_be_send_event(char* msg);
extern int			mrn_be_send_io(char* msg);

extern int			mrn_be_is_shutdown();
extern void			mrn_be_waitfor_shutdown();

extern int			mrn_be_shutdown();

#endif /* _GUARD_MRNET_BACKEND_H */
