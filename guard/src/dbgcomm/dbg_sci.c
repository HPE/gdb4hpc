#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include 	"defs.h"
#include	"dbgevent.h"
#include	"dbg_soc.h"
#include	"list.h"
#include	"alloc.h"
#include 	"bitset.h"

#include	"dbg_sci.h"
#include	"dbg_set_sync.h"


int
sci_get_polling_fd(int* polling_fd)
{
	int res;
	res = SCI_Query(POLLING_FD, polling_fd);
	if(res != SCI_SUCCESS)
	{
		fprintf(stderr, "ERROR: sci_get_polling_fd returned an ERROR[%d]\n", res);
		return -1;
	}

	return 0;
}

int
sci_pack_fe_msg(int* seq, char* msg, int* sizes, void** bufs)
{
	bufs[0] = seq;
	sizes[0] = sizeof(*seq);

	bufs[1] = msg;
	sizes[1] = strlen(msg) + 1;

	return 0;
}

int
sci_unpack_fe_msg(void* buffer, int size, int* seq, char** msg)
{
	char	*tmp;
	int		len;

	*seq = *(int *)buffer;
	tmp = (char *)((char *) buffer + sizeof(int));

	/* filter '\n'*/
	len = strlen(tmp);

	if((len + sizeof(int)) >= size)
	{
		return -1;
	}
	else 
	{
		if(tmp[len - 1] == '\n')
			tmp[len - 1] = '\0';

		*msg = strdup(tmp);
		return 0;
	}
}

int
//sci_pack_be_msg(int* be_id, int* type, int* seq, char* msg, int* sizes, void** bufs)
sci_pack_be_msg(char* header, int* bs_str_len, char* bs_str, int* type, int* seq, int* msg_len, char* msg, int* sizes, void** bufs)
{
	bufs[0] = header;
	sizes[0] = 1;

    bufs[1] = seq;
    sizes[1] = sizeof(*seq);

    bufs[2] = type;
    sizes[2] = sizeof(*type);

    bufs[3] = bs_str_len;
    sizes[3] = sizeof(int);

    bufs[4] = bs_str;
    sizes[4] = strlen(bs_str) + 1;

	bufs[5] = msg_len;	
	sizes[5] = sizeof(int);	

	bufs[6] = msg;
	sizes[6] = strlen(msg) + 1;

	DPRINT("sci_pack_be_msg: bs_str:%s, str_len:%d, type:%d, seq:%d, msg_len:%d, msg:%s \n", bs_str, *bs_str_len, *type, *seq, *msg_len, msg);
	return 0;
}

int
sci_unpack_be_msg_single(void* buffer, int size, bitset** bs, int* type, int* seq, char** msg)
{
	char	*tmp;
	int		msg_len, bs_str_len;
	char	*tmp_bs;

	tmp = (char*)buffer;

	*seq = *((int*)tmp);
	*type = *((int*)tmp + 1);

	bs_str_len = *((int*)tmp + 2);
	
	tmp = (char*)((int*)tmp + 3);
	if(strlen(tmp) != bs_str_len || bs_str_len + sizeof(int)*4 >= size)
	{
		return -1;
	} else {
		*bs = str_to_bitset(tmp, NULL);

		if(*bs == NULL)
			return -1;
	}
	tmp_bs = tmp;

	tmp += bs_str_len + 1;
	msg_len = *(int*)tmp;

    tmp = (char *)((int*)tmp + 1);
	if(strlen(tmp) != msg_len)
	{
		return -1;
	} else {
		*msg = strdup(tmp);
	}

	DPRINT("sci_unpack_be_msg_single: type:%d, seq:%d, msg_len:%d, msg:%s from %s\n", *type, *seq, msg_len, *msg, tmp_bs);

	return 0;
}

/*int
sci_unpack_be_msg(void* buffer, int size, char** bs_str, int* type, int* seq, char** msg)
{
	char	*tmp;
	int		msg_len, bs_str_len;

	bs_str_len = *((int *)buffer);
	tmp = (char*)((int*)buffer + 1);
	if(strlen(tmp) != bs_str_len || bs_str_len + sizeof(int)*4 >= size)
	{
		return -1;
	} else {
		*bs_str = strdup(tmp);
	}

	tmp += bs_str_len + 1;

	*type = *((int*)tmp);
	*seq = *((int*)tmp + 1);
	msg_len = *((int*)tmp + 2);

    tmp = (char *)((char *)tmp + sizeof(int)*3);

	if(strlen(tmp) != msg_len)
	{
		return -1;
	} else {
		*msg = strdup(tmp);
	}

	DPRINT("sci_unpack_be_msg: bs_str:%s, str_len:%d, type:%d, seq:%d, msg_len:%d, msg:%s \n", *bs_str, bs_str_len, *type, *seq, msg_len, *msg);

	return 0;
}*/
