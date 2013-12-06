#ifndef _GUARD_MRNET_STREAM_H
#define _GUARD_MRNET_STREAM_H

#include "MRNet_C.h"


void stream_delete(MRNet_Stream *stream);

int stream_send(MRNet_Stream* stream, int tag, char *format_str, ... );

int stream_send_with_data(MRNet_Stream* stream, int tag, void **data, char *format_str);

int stream_flush(MRNet_Stream* stream );

int stream_recv(MRNet_Stream* stream, int *otag, MRNet_Packet** packet, int blocking);

int stream_get_DataNotificationFd(MRNet_Stream *stream);
void stream_clear_DataNotificationFd(MRNet_Stream *stream);
void stream_close_DataNotificationFd(MRNet_Stream *stream);

#endif /* _GUARD_MRNET_STREAM_H */
