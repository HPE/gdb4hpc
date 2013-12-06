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

/* info of SCI group */
struct sci_group_info
{
	sci_group_t		group;
	bitset*			bs_full;
};
typedef struct sci_group_info sci_group_info;

/* messages from SCI BE */
struct sci_be_msg
{
	int						be_id;
	int						type;
	sci_group_t				group;	/* group_id > 0: only for replies*/
	int						seq_id; 	/* only for replies */
	char*					content;
};
typedef struct sci_be_msg sci_be_msg;

struct sci_type
{
	SCI_Handler				fe_pid; /* ID of SCI FE process */
	void*					tree;	/*compatable with existing callback*/
	int						num_be;		//number of dbgsrvs
	int						cmd_filter_id;
	int						event_filter_id;
	int						io_filter_id;
	bitset					*bitset_full;
	list_t					*group_list;	//list of groups 
	list_t					*reply_list;	//list of replies
	list_t					*event_list;	//list of events
	list_t					*io_list;		//list of I/O 
};
typedef struct sci_type sci_type;

struct agg_message
{
	void*		tree;
	sci_group_t	group;
	rep_set_t*	message;
};
typedef struct agg_message agg_message;

static sci_info_t 	info;
static sci_type*	my_sci = (sci_type*)NULL;
static void*	sci_tree = NULL;

/* TODO: remove */
static char*	waiting_io = NULL;
static bitset*	waiting_source = NULL;

static list_t*	rep_list = (list_t*)NULL; 	// list of grouped replies 
static list_t*	event_list = (list_t*)NULL; // list of grouped events
static list_t*	io_list = (list_t*)NULL;	// list of io messages

static int is_invoke = 0; //patch for calback. TODO: remove it?

void (*dbgclnt_event_callback)(void* tree, rep_set_t* set, int is_invoke) = NULL;

sci_be_msg*
new_sci_be_msg(int msg_type, sci_group_t group, int seq_id, char* content)
{
	sci_be_msg *msg = (sci_be_msg*)Alloc(sizeof(sci_be_msg));

	msg->type = msg_type;
	msg->group = group;
	msg->seq_id = seq_id;
	msg->content = content;

	return msg;
}

void
free_sci_be_msg(sci_be_msg* msg)
{
	if(msg != NULL)
	{
		Free(msg->content);
		msg->content = NULL;
		Free(msg);
	}
}

sci_group_info*
new_sci_group(sci_group_t group, bitset* group_bs)
{
	sci_group_info *info = (sci_group_info*)Alloc(sizeof(sci_group_info));

	info->group = group;
	info->bs_full = group_bs;

	return info;
}

void
free_sci_group_info(sci_group_info* info)
{
	if(info != NULL)
	{
		bitset_free(info->bs_full);
		info->bs_full = NULL;
		Free(info);
	}
}

sci_type*
new_sci_type(SCI_Handler handler, int num_be)
{
	sci_type *sci = (sci_type*)Alloc(sizeof(sci_type));

	sci->fe_pid = handler;
	sci->num_be = num_be;
	sci->cmd_filter_id = -1;
	sci->event_filter_id = -1;
	sci->io_filter_id = -1;
	sci->bitset_full = bitset_new(num_be);
	bitset_invert(sci->bitset_full);

	sci->group_list = NULL;

	sci->reply_list = NewList();
	sci->event_list = NewList();
	sci->io_list = NewList();

	sci->tree = sci_tree;	/*TODO: be fixed*/
	return sci;
}

void
free_sci_type(sci_type* my_sci)
{
	if(my_sci == NULL)
		return;

	bitset_free(my_sci->bitset_full);

	if(my_sci->group_list != NULL)
		DestroyList(my_sci->group_list, free_sci_group_info);

	if(my_sci->reply_list != NULL)
		DestroyList(my_sci->reply_list, free_sci_be_msg);

	if(my_sci->event_list != NULL)
		DestroyList(my_sci->io_list, free_sci_be_msg);

	if(my_sci->io_list != NULL)
		DestroyList(my_sci->io_list, free_sci_be_msg);

	Free(my_sci);
}

agg_message*
new_agg_message(void *tree, rep_set_t *msg, sci_group_t group)
{
	agg_message* agg = (agg_message*)Alloc(sizeof(agg_message));
	agg->tree = tree;
	agg->message = msg;
	agg->group = group;
	
	return agg;
}

void
free_agg_message(agg_message* agg)
{
	agg->tree = NULL;
	agg->message = NULL;

	Free(agg);
}

int	
sci_create_group(bitset* bs, sci_group_t* group)
{
	int		index, be_index;
	int		size;
	int 	num;
	int*	be_list;

	if(bs == NULL)
		return -1;	

	size = bitset_size(bs);
	num = bitset_count(bs);	
	if(num == 0)
		return -1;

	be_list = (int*)Alloc(num*sizeof(int));
	be_index = 0;
	for(index = 0; index < size; index++)
	{
		if(bitset_test(bs, index))
		{
			be_list[be_index++] = index;
		}
	}

	return SCI_Group_create(num, be_list, group);
}

int
get_sci_group(bitset *target, sci_group_t* group)
{
	list_t			*group_list = NULL;
	sci_group_info	*info = NULL;
	sci_group_t		sci_group;
	int				res;

	if(my_sci == NULL)
		return -1;

	if(target == NULL || bitset_eq(my_sci->bitset_full, target)) //is it a broadcast to all?
	{
		*group = SCI_GROUP_ALL;
		return 0;
	}

	if(my_sci->group_list == NULL)
	{
		my_sci->group_list = NewList();
	}
	group_list = my_sci->group_list;

	//search group_list
	SetList(group_list);
	while((info = (sci_group_info*)GetListElement(group_list)) != (sci_group_info*)NULL)
	{
		if(bitset_eq(info->bs_full, target))
			break;
	}

	if(info != NULL) //sci_group is found
	{
		*group = info->group;
		return 0;
	}
	else //create an SCI group
	{
		res = sci_create_group(target, &sci_group);
		if(res != SCI_SUCCESS) {
			fprintf(stderr, "sci_create_group returned ERROR[%d]", res);
			return -1;
		}
 
		info =  new_sci_group(sci_group, bitset_dup(target));
		AddToList(group_list, group);

		*group = sci_group;
		return 0;
	}
}

int
put_msg_to_list(int seq, int type, bitset* bs, char* msg, sci_group_t group)
{
	group_reply_t* reply;
	rep_set_t *agg_set;
	agg_message* agg_msg;

	agg_set = NewRepSet(my_sci->tree);
	reply = NewGroupReply(bs, msg);
	AddReplyToSet(agg_set, reply);
	agg_msg = new_agg_message(my_sci->tree, agg_set, group);

	if(type == DBG_SCI_BE_MSG_EVENT )
		AddToList(event_list, agg_msg);
	else if (type == DBG_SCI_BE_MSG_REPLY )
		AddToList(rep_list, agg_msg);

	return 1;
}

int
process_be_msg(void * buffer, int size, sci_group_t group)
{
	int		seq;
	int		type;
	bitset*	bs = NULL;
	char*	msg = NULL;

	char* tmp = (char*)buffer;
	int index, num, len, res;

	if(size < 1)
		return -1;

	if(*tmp == DBG_SCI_MSG_SINGLE)
	{
		tmp++;
		res = sci_unpack_be_msg_single(tmp, size - 1, &bs, &type, &seq, &msg);
		if(res < 0)
		{
			if(bs != NULL)
				bitset_free(bs);

			return res;
		}

		put_msg_to_list(seq, type, bs, msg, group);
	} 
	else if (*tmp == DBG_SCI_MSG_CONCAT)
	{
		tmp++;
		num = *(int*)tmp;
		tmp += sizeof(int);	

		for(index = 0; index < num; index++)
		{
			len = *(int*)tmp;
			tmp += sizeof(int);	

			res = sci_unpack_be_msg_single(tmp, len, &bs, &type, &seq, &msg);
			if(res < 0)
			{
				if(bs != NULL)
					bitset_free(bs);

				return res;
			}
			tmp += len;

			put_msg_to_list(seq, type, bs, msg, group);
		}
	} 

	return 0;
}

void sci_fe_handler(void *user_param, sci_group_t group, void *buffer, int size)
{
	int res;

	res = process_be_msg(buffer, size, group);
	if(res < 0)
	{
		fprintf(stderr, "ERROR: sci_fe_handler reveived invalid msg[%s]\n", buffer);
		return;
	}
}

/*
** load filter
*/
int
load_sci_filter(char *filter_path, int filter_id)
{
	int res;
	sci_filter_info_t filter_info;

    bzero(&filter_info, sizeof(filter_info));
    filter_info.filter_id = filter_id;
    filter_info.so_file = filter_path;
    res = SCI_Filter_load(&filter_info);
    if (res != SCI_SUCCESS) {
		return -1;
    }

	return 0;
}

int
create_sci_fe(sci_info_t* info, int* num_be)
{
	int	res;

	if(info == NULL)
		return -1;

	DPRINT("<*> INFO: enterring create_sci_fe!\n");
	res = SCI_Initialize(info);
    if (res != SCI_SUCCESS) {
        fprintf(stderr, "ERROR: create_sci_fe() SCI_Initialize returned ERROR[%d]\n", res);
		return -1;
    }

	DPRINT("<*> INFO: SCI_Initialize() completed!\n");

    res = SCI_Query(NUM_BACKENDS, num_be);
    if (res != SCI_SUCCESS) {
    	DPRINT("ERROR: create_sci_fe() SCI_Query returned ERROR[%d]. num_bes:[%d] \n", res, num_be);
		return -1;
    }
	
	is_invoke = 1;
	DPRINT("<*> INFO: leaving create_sci_fe!\n");

	return 0;
}


/*------------------------------------------ ---------------------*/
/*----------------------APIs start from here----------------------*/
/*------------------------------------------- --------------------*/

void
sci_fe_initialize(char* jobkey, char* agent_path, char* lib_path, char* hfile, char* be_path, char** beenvp)
{
	/* set environment variables for SCI FE */
	setenv(SCI_USE_EXTLAUNCHER, USE_EXTLAUNCHER_VALUE, 1);
//	setenv(SCI_REMOTE_SHELL, REMOTE_SHELL_VALUE, 1);
	setenv(SCI_EMBED_AGENT, EMBED_AGENT_VALUE, 1);
	setenv(SCI_LISTENER_PORT, DEFAULT_LISTENER_PORT, 1); //TODO: doesnot work with multiple SCI
	setenv(SCI_JOB_KEY, jobkey, 1); 
	setenv(SCI_AGENT_PATH, agent_path, 1);
	setenv(SCI_LIB_PATH, lib_path, 1); 

	/* set sci_info for SCI FE */
	bzero(&info, sizeof(info));
    info.type = SCI_FRONT_END;
    info.fe_info.mode = SCI_POLLING;
    info.fe_info.hostfile = hfile;
    info.fe_info.bepath = be_path;
    info.fe_info.beenvp = beenvp;
    info.fe_info.hndlr = (SCI_msg_hndlr *)&sci_fe_handler;
    info.fe_info.param = NULL;
	info.disable_sshauth == 1;

	/*TODO: remove the following code*/
	int key;
	sscanf(jobkey, "%d", &key);
	sci_tree = (void*)((long)key);

	rep_list = NewList();
	event_list = NewList();
}

/*
** create sci FE with filter
*/
int
sci_fe_create_with_filter(char* cmd_filter_path, int cmd_filter_id, char* event_filter_path, int event_filter_id)
{
	int num_be;
	int	res;

	res = create_sci_fe(&info, &num_be);

	if(res < 0)
		return res;

	my_sci = new_sci_type(0, num_be);

	res = load_sci_filter(cmd_filter_path, cmd_filter_id);

	if(res >= 0)
	{
		res = load_sci_filter(event_filter_path, event_filter_id);
	}

	return res;
}

/*
** create sci FE
*/
int
sci_fe_create()
{
	int num_be;
	int	res;

	res = create_sci_fe(&info, &num_be);

	if(res == 0)
		my_sci = new_sci_type(0, num_be);

	return res;
}

/*
** initialize frontend for attaching backend programs
#ifdef ALPS
MRNet_Network*
mrn_fe_create_network_no_be(char *topology_file, int *top_size, MRNet_Topology **p_topology, int apid)
{
	MRNet_Network *network = NULL;

	if(topology_file == NULL)
	{
		fprintf(stderr, "ERROR: mrn_fe_create_network_no_be topology_file is NULL!\n");
		return NULL;
	}

	network = network_create_FE_with_apid(topology_file, apid);

	if( network_has_error(network) ) 
	{
		network_print_error(network, "ERROR: mrn_fe_create_network_no_be failed to create MRNet network!\n");
		return NULL;
	}

	MRNet_Topology *topology = network_get_topology(network);
	if(topology == NULL)
	{
		fprintf(stderr, "ERROR: mrn_fe_create_network_no_be cannot get topology for network[%p]\n", network);
		network_delete(network);
		return NULL;
	}

	*top_size = topology_get_nodes_number(topology);
	*p_topology = topology;

	network_set_failure_recovery(network, 0); 

	return network;
}

int
mrn_fe_create_streams(MRNet_Network *network)
{
	return 0;
}
#endif
*/

int
sci_fe_get_be_num()
{
	int num_be = 0;

	if(my_sci != NULL)
		num_be = my_sci->num_be;

	return num_be;
}

int 
sci_fe_send_command(bitset *target, char *msg)
{
	static int seq = 0;
	void	*bufs[NUM_FE_BUFS];
	int		sizes[NUM_FE_BUFS];

	sci_group_t group;
	int		res;
	if(my_sci == NULL || msg == NULL)
		return -1;

	res = get_sci_group(target, &group);
	if(res != 0)
	{
		fprintf(stderr, "ERROR: sci_fe_send_command cannot find group for [%s]\n", bitset_to_set(target));
		return -1;
	}

	DPRINT("INFO: sci_fe_send_command is sending cmd[%s] to %s\n", msg, bitset_to_set(target));

	sci_pack_fe_msg(&seq, msg, sizes, bufs);
	seq++;

	if( SCI_Bcast(SCI_FILTER_NULL, group, NUM_FE_BUFS, bufs, sizes) != SCI_SUCCESS ){
		fprintf( stderr, "ERROR: sci_fe_send_command SCI_Bcast() failed!\n");
		return -1;
	}

	return 0;
}

int 
sci_fe_recv_reply(bitset *target, rep_set_t ***rep, int* number)
{
	int res;
	int seq;
	rep_set_t **repArr;
	int	rank;
	sci_group_t group;
	int		msgNum, index;
	agg_message* agg;

	DPRINT("enterring sci_fe_recv_reply\n");

	res = get_sci_group(target, &group);
	if(res != 0)
	{
		fprintf(stderr, "ERROR: sci_fe_send_command cannot find group for [%s]\n", bitset_to_set(target));
		return -1;
	}

	res = SCI_Poll(50);//blocked reading, TODO: handling failures
	if(res != SCI_SUCCESS)
	{
		fprintf(stderr, "ERROR: sci_fe_recv_reply() SCI_Poll returned an ERROR[%d]\n", res);
		return -1;
	}

	msgNum = SizeOfList(rep_list);
	if(msgNum == 0)
	{
		fprintf(stderr, "ERROR: sci_fe_handler() didn't receive any message after calling SCI_Poll()!\n");
		return 0;
	}

	repArr = (rep_set_t**)Alloc(sizeof(rep_set_t*)*msgNum);

	SetList(rep_list);
	index = 0;
	while((agg = (agg_message*)GetListElement(rep_list)) != (agg_message*)NULL)
	{
		if(agg->group != group) // it is not supposed to happen
			continue;

		rep_set_t*	reply = agg->message;
		repArr[index] = agg->message;

		index++;
	}
	
	while(SizeOfList(rep_list) > 0)
		RemoveFirst(rep_list);

	*rep = repArr;
	*number = msgNum;

	DPRINT("leaving sci_fe_recv_reply.\n");

	return 1;
}

int 
sci_fe_check_io_and_event()
{
	int res;
/*	char **strArr;
	int	strLen, msgNum, index;
	group_reply_t* reply;
	rep_set_t *event_set;*/
	char* msg = NULL;
	char* rank = NULL;
	int tag, retval;
	int	ok = 0;

	res = SCI_Poll(100);
	if(res != SCI_SUCCESS)
	{
		return 0;
	}

	return SizeOfList(event_list);
}

void
sci_fe_set_callback(void (*callback)())
{
	dbgclnt_event_callback = callback;
	return;
}

int
sci_fe_handle_io()
{
	if(waiting_io == NULL)
		return 0;

	char* res = waiting_io;
	waiting_io = NULL;

	bitset* bs = waiting_source;
	waiting_source = NULL;

	fprintf(stderr, "IO: %s from %s\n", bitset_to_set(bs));

	return 1;
}

int
sci_fe_handle_event()
{
	char* msg;
	sci_group_t group;
	bitset*	be_rank;
	group_reply_t* reply;
	agg_message* agg_msg;
	rep_set_t *event_set;

//	DPRINT("sci_fe_handle_event is invoked: SizeOfList(event_list)[%d]\n", SizeOfList(event_list));

	while(SizeOfList(event_list) > 0)
	{	
		agg_msg = (agg_message*)RemoveFirst(event_list);	
		event_set = agg_msg->message;
		dbgclnt_event_callback(my_sci->tree, event_set, is_invoke);
		//TODO: free event_set ?	
	}

	return 1;
}

void 
sci_fe_set_invoke_completed()
{
	is_invoke = 0;
}

int 
sci_fe_shutdown()
{
	SCI_Terminate();
	free_sci_type(my_sci);
}
