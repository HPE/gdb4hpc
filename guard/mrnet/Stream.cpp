/*********************************************************************************\
 * <Stream.cpp>
 *
 * © 2011-2013 Cray Inc.  All Rights Reserved.
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



#include "mrnet/Network.h" //for resolving cyclic dependency betwen Network.h and Stream.h
#include "mrnet/Stream.h"
#include "mrnet/Types.h"
#include "MRNet_type_wrapper.h"

using namespace MRN;

extern "C" void stream_delete(MRNet_Stream *stream)
{
	if(stream != NULL)
	{
		Stream *pStream = (Stream*)stream;
		delete pStream;
	}
}

extern "C" int stream_send(MRNet_Stream* stream, int tag, const char *format_str, ... )
{
//	fprintf(*err_stream_ptr, "[%d] entering stream_send()! stream: [%p]\n", getpid(), stream);
	Stream *pStream = (Stream*)stream;
	va_list arg_list;

	va_start(arg_list, format_str);
	int res = pStream->send(format_str, arg_list, tag);
	va_end(arg_list);
//	fprintf(*err_stream_ptr, "[%d] leaving stream_send()! stream: [%p], res[%d]\n", getpid(), stream, res);

	return res;
}

/*
extern "C" int stream_send_with_data(MRNet_Stream* stream, int tag, void **data, char *format_str)
{
	Stream *pStream = (Stream*)stream;
	
//	return pStream->send(tag, data, format_str);
	return 0;
}
*/

extern "C" int stream_flush(MRNet_Stream* stream )
{
	Stream *pStream = (Stream*)stream;
	
	return pStream->flush();
}

extern "C" int stream_recv(MRNet_Stream* stream, int *otag, MRNet_Packet** packet, int blocking)
{
	int res;
	Stream *pStream = (Stream*)stream;
	PacketPtr * stream_packet_ptr = new PacketPtr;
	
	*packet = NULL;
	
	if(blocking)
		res = pStream->recv(otag, *stream_packet_ptr, true);
	else
		res = pStream->recv(otag, *stream_packet_ptr, false);
	
	// if res is greater than 0, we recieved a packet, otherwise delete the PacketPtr
	if (res > 0)
	{
		PacketPtr stream_packet = *stream_packet_ptr;
		stream_packet->set_DestroyData(true);
		*packet = reinterpret_cast< MRNet_Packet * >(stream_packet_ptr);
	} else
		delete stream_packet_ptr;

	return res;
}

extern "C" int stream_get_DataNotificationFd(MRNet_Stream *stream)
{
	Stream *pStream = (Stream *)stream;
	
	return pStream->get_DataNotificationFd();
}

extern "C" void stream_clear_DataNotificationFd(MRNet_Stream *stream)
{
	Stream *pStream = (Stream *)stream;
	
	pStream->clear_DataNotificationFd();
}

extern "C" void stream_close_DataNotificationFd(MRNet_Stream *stream)
{
	Stream *pStream = (Stream *)stream;
	
	pStream->close_DataNotificationFd();
}

extern "C" unsigned int stream_get_Id(MRNet_Stream *stream)
{
	Stream *pStream = (Stream *)stream;
	return pStream->get_Id();
}


