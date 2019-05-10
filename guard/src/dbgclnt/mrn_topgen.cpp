/*********************************************************************************\
 * mrn_topgen.c
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



#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <arpa/inet.h>

#include "defs.h"
#include "dbg_client.h"
#include "mrn_cfg.h"
#include "dbg_mrn.h"
#include "Topology_C.h"

#define DEFAULT_TOP_FANOUT_DEGREE       32
#define DEFAULT_FILTER_TIMEOUT	        500
#define NUM_TIMEOUT_RETRY               1000
// line format is "%-7d %s %-5d %-6d\n"
// 3 bytes from ' ' seperators
// 7 bytes first field
// 8 bytes second field from "nidXXXXX"
// 5 bytes third field
// 6 bytes fourth field
// 1 byte from '\n'
// 30 bytes total

enum MRN_TOP_TYPE{
	MRN_TOP_BALANCED,
	MRN_TOP_KNOMIAL
};
typedef enum MRN_TOP_TYPE MRN_TOP_TYPE;

static int topology_type;

static int mrnetcfg_files_created = 1;
static	int fanout_degree = 0;
struct level_info {
	int num_nodes;
	char **node_name_list;
};
typedef struct level_info level_info_t;

static level_info_t *_level_list = NULL;
static int _level_number;

void
CreateLevelList(int num_level)
{
	_level_list = (level_info_t*)malloc(num_level*sizeof(level_info_t));
	_level_number = num_level;
}

int
SetLevelInfo(int level, int num_nodes)
{
	if(_level_list != NULL && level < _level_number)
	{
		_level_list[level].num_nodes = num_nodes;
		_level_list[level].node_name_list = (char **)malloc(num_nodes * sizeof(char*));	

		return 0;
	}
	
	return -1;
}

int
GetLevelNodeNum(int level)
{
	if(_level_list != NULL && level < _level_number)
	{
		return _level_list[level].num_nodes;
	}

	return -1;
}

int
SetLevelNodeInfo(int level, int node, char *node_name)
{
	if(_level_list != NULL && level < _level_number)
	{
		if(_level_list[level].num_nodes > node)
		{
			_level_list[level].node_name_list[node] = node_name;
			return 0;
		}
	}	

	return -1;
}

char*
GetLevelNodeInfo(int level, int node)
{
	if(_level_list != NULL && level < _level_number)
	{
		if(_level_list[level].num_nodes > node)
		{
			return _level_list[level].node_name_list[node];
		}
	}	

	return NULL;
}

void
DestroyLevelList(int attaching)
{
	int index;
	int node, num_nodes;
	int real_number;

	real_number = _level_number;
	if(attaching)
		--real_number;
	
	for(index = 0; index < real_number; ++index)
	{
		num_nodes = _level_list[index].num_nodes;
		for(node = 0; node < num_nodes; ++node)
		{
			free(_level_list[index].node_name_list[node]);
		}
	}

	free(_level_list);
}

struct hostID {
	char *hostname;
	int seq;
};
typedef struct hostID hostID_t;

static hostID_t *_hostID_header;
static int _hostID_index;
static int _hostID_number;

void
SetHostList(hostID_t *header, int number)
{
	_hostID_header = header;
	_hostID_index = 0;
	_hostID_number = number;
}

void
GetNextHost(char **name, int *id)
{
	*name = _hostID_header[_hostID_index].hostname;
	*id = _hostID_header[_hostID_index].seq++;

	_hostID_index++;
	if(_hostID_index >= _hostID_number)
		_hostID_index = 0;
}

int
PosInHostList(char *host)
{
	hostID_t *header = _hostID_header;

	int index = 0;
	for(index = 0; index < _hostID_number; ++index)
	{
		if(!strcmp(host, header[index].hostname))
			break;
	}

	if(index == _hostID_number)
		return -1;
	
	return index;
}

int
GetHostID(char *name)
{
	int id;
	int pos = PosInHostList(name);
	if(pos == -1)
		return 0;
	else
	{
		id = _hostID_header[_hostID_index].seq++;
		return id;
	}
}

int
line_count(FILE *file)
{
	int num_lines = 0;

	int ret_val;
	char ret_char;

	fseek(file, 0, SEEK_SET);

	while(1) {
		ret_val = fgetc(file);

		if(ret_val == EOF) {
			if(ferror(file))
				return -1;
			break;
		}

		ret_char = (char)ret_val;

		if(ret_char == '\n')
			num_lines++;
	}
	fseek(file, 0, SEEK_SET);

	return num_lines;
}

/* create a balanced tree based on pnumber and machines in machines_file
*  if(any_machine), leafnodes of the tree are exactlly located on the machines
*  otherwise, anymachines in machines_file could be selected
*/

int
level_count(int num_be, int fanout)
{
	int num_level = 1;
	int num_curr_be = fanout;
	while(num_be > num_curr_be )
	{
		num_curr_be *= fanout;
		num_level++;
	}

	return num_level;
}

int
fanout_degree_count(int num_be, int num_level, int *up,  int *low)
{
	double fanout;

	if(num_be < 1 || num_level < 1)
		return -1;

	if(num_level == 1)
	{
		*up = num_be;
		*low = num_be;
	} else if(num_level == 2){
		fanout = sqrt((double)num_be);
		*up = (int)ceil(fanout);
		*low = (int)floor(fanout);
	} else {
		fanout = pow((double)num_be, 1.0/(double)num_level);
		*up = (int)ceil(fanout);
		*low = (int)floor(fanout);
	}	

	return 0;
}

int
fanout_count(int num_be, int num_level, int *p_up, int *p_low, int *p_up_count, int *p_low_count)
{
	int up = *p_up;
	int low = *p_low;
	int up_count = *p_up_count;
	int low_count = *p_low_count;
	int num_parent = 1;
	int done = 0;

	if(num_be < 1 || num_level < 0 || up < 1 || low < 1)
		return -1;

	while(--num_level)
	{
		num_parent *= up;
	};
	
	do{

		up_count = num_be - num_parent*low ; // low_count = num_be - up^(num_level -1 )
		low_count = num_parent - up_count;

		if(up_count < 0 || low_count < 0)
		{
			up--;
			low--;
		}
		else
			done = 1;

	} while(!done);

	*p_up = up;
	*p_low = low;
	*p_up_count = up_count;
	*p_low_count = low_count;

	return 0;
}

int
get_apid(int *nid)
{
	int apid;
	FILE *file;
	char aprunid_name[MAXPATHLEN];
	char *cfg_dir = getenv(GUARD_CFG_DIRECTORY);

	if(cfg_dir == NULL)
	{
		PrintOutError("ERROR: Cannot find %s! Please set up your MRNet configurations!\n", GUARD_CFG_DIRECTORY);
		return -1;
	}

	snprintf(aprunid_name, MAXPATHLEN-1, "%s/%s", cfg_dir, ALPS_APRUNID_FILE);

	file = fopen(aprunid_name, "r");
	if(file == NULL)
	{
		PrintOutError("ERROR: cannot open %s!\n", aprunid_name);
		return -1;
	}

	fscanf(file, "%d", &apid);
	fscanf(file, "%d", nid);
	fclose(file);

	return apid;
}

int
load_machines(char *machines_name, hostID_t **pHostID)
{
	FILE *machines;
	char hostname[MAXPATHLEN];
	int count, ret_val, index;

	machines = fopen(machines_name, "r");
	while(machines == NULL)
	{
		PrintOutError("ERROR: cannot open %s!\n", machines_name);
		return -1;
	}

	count = line_count(machines);
	if(count <= 0)
	{
		fclose(machines);
		return -1;
	}

	//load
	*pHostID = (hostID_t*)malloc(count*sizeof(hostID_t));
	index = 0;
	while(1)
	{
		ret_val = fscanf(machines, "%s\n", hostname);
		if(ret_val == EOF)
		{
			break;
		}

		(*pHostID)[index].hostname = strndup(hostname, (size_t)MAXPATHLEN);	
		(*pHostID)[index].seq = 0;
		index++;
	}
	fclose(machines);

	return count; 
}

int
create_balanced_topology(int num_be, char *root, char **hosts, int attaching)
{
	int num_level;
	int fanout, up_fanout, low_fanout;
	int up_count, low_count;
	int level_index;

	hostID_t *pHostID = NULL;
	int ret_val;
	int num_machines;
	char *nextHost;
	int nextID;

	FILE *topology;
	char machines_name[MAXPATHLEN];
	char topology_name[MAXPATHLEN];
	char *cfg_dir = getenv(GUARD_CFG_DIRECTORY);
	char *fanout_env = getenv(GUARD_MRNET_TOP_FANOUT);
	
	char **tmp;
	int index;
	

	if(cfg_dir == NULL)
	{
		PrintOutError("ERROR: Cannot find %s! Please set up your MRNet configurations!\n", GUARD_CFG_DIRECTORY);
		return -1;
	}
	snprintf(topology_name, MAXPATHLEN-1, "%s/%s", cfg_dir, MRNET_TOPOLOGY);
	
	if (hosts == (char **)NULL)
	{
		snprintf(machines_name, MAXPATHLEN-1, "%s/%s", cfg_dir, MACHINES_FILE);
		num_machines = load_machines(machines_name, &pHostID);
		if(num_machines <= 0)
		{
			PrintOutError("ERROR: create_balanced_topology calling load_machines returned errors!\n");
			return -1;
		}
	} else
	{
	        // count the number of hosts
	        num_machines = 0;
	        tmp = hosts;
	        while (*tmp++ != (char *)NULL)
	        {
	                ++num_machines;
	        }
	        
	        // allocate space for the pHostID
	        if ((pHostID = (hostID_t*)calloc(num_machines, sizeof(hostID_t))) == (void *)0)
	        {
	                PrintOutError("ERROR: calloc failed.\n");
	                return -1;
	        }
	        
	        // iterate through and populate the pHostID array
	        index = 0;
	        tmp = hosts;
	        while (*tmp != (char *)NULL)
	        {
	                pHostID[index].hostname = strdup(*tmp++);
	                pHostID[index].seq = 0;
	                ++index;
	        }
	}

	if(fanout_env != NULL)
	{
		fanout_degree = atoi(fanout_env);	
	}

	if(fanout_degree <= 0)
	{
		fanout_degree = DEFAULT_TOP_FANOUT_DEGREE;
	}

	num_level = level_count(num_be, fanout_degree);
	ret_val = fanout_degree_count(num_be, num_level, &up_fanout, &low_fanout);
	if(ret_val < 0)
	{
		PrintOutError("ERROR: create_balanced_topology calling fanout_degree returned errors! \
							num_be[%d], num_level[%d]\n", num_be, num_level);
		return -1;
	}
	fanout = up_fanout;

	ret_val = fanout_count(num_be, num_level, &up_fanout, &low_fanout, &up_count, &low_count);
	if(ret_val < 0)
	{
		PrintOutError("ERROR: create_balanced_topology calling fanout_count returned errors! \
							num_be[%d], num_level[%d]\n", num_be, num_level);
		return -1;
	}

	DPRINT("INFO: num_be[%d] num_level[%d] fanout[%d] up[%d]x%d low[%d]x%d\n", 
						num_be, num_level, fanout, up_fanout, up_count, low_fanout, low_count);

	topology = fopen(topology_name, "w");
	if(topology == NULL)
	{
		PrintOutError("ERROR: cannot open %s!\n", topology_name);
		return -1;
	}

	if(num_level == 1 && attaching)
	{
		if(root == NULL)
			fprintf(topology, "\t%s:%d ;", "localhost", 0);
		else
			fprintf(topology, "\t%s:%d ;", root, 0);
	}
	else{
		SetHostList(pHostID, num_machines);

		CreateLevelList(num_level);
		SetLevelInfo(0, 1);
		char *node_str = (char*)malloc(sizeof(char)*MAXPATHLEN);
		if(root == NULL)
			snprintf(node_str, MAXPATHLEN-1, "%s:%d", "localhost", GetHostID("localhost"));
		else
			snprintf(node_str, MAXPATHLEN-1, "%s:%d", root, GetHostID(root));
		SetLevelNodeInfo(0, 0, node_str);

		int num_parent, parent_index, node_index;
		int real_level = num_level;
		int real_fanout, real_fanout_count = 0;
		if(attaching)
			--real_level;	

		for(level_index = 0; level_index < real_level; ++level_index)
		{
			num_parent = GetLevelNodeNum(level_index);
			if(level_index+1 < real_level)
				SetLevelInfo(level_index+1, num_parent*fanout);
			for(parent_index = 0; parent_index < num_parent; ++parent_index)
			{
				fprintf(topology, "\t%s =>", GetLevelNodeInfo(level_index, parent_index));
				real_fanout = fanout;

				if(!attaching && (level_index+1)==real_level)
				{	
					real_fanout = up_fanout;
					real_fanout_count++;
					if(real_fanout_count > up_count)
						real_fanout = low_fanout;	
				}

				for(node_index = 0; node_index < real_fanout; ++node_index)
				{
					node_str = (char*)malloc(sizeof(char)*MAXPATHLEN);
					GetNextHost(&nextHost, &nextID);
					snprintf(node_str, MAXPATHLEN-1, "%s:%d", nextHost, nextID);

					fprintf(topology, "\n\t\t%s ", node_str);

					if(level_index+1 < real_level)
						SetLevelNodeInfo(level_index+1, (parent_index * real_fanout + node_index), node_str);
					else
						free(node_str);
				}
				fprintf(topology, ";\n\n");
			}
		}
		DestroyLevelList(attaching);
	}

	fclose(topology);
		
	if(pHostID != NULL)
	{
		for(index = 0; index < num_machines; ++index)
			if(pHostID[index].hostname != NULL)
				free(pHostID[index].hostname);

		free(pHostID);
	}
	
	return 0;
}

int
create_knominal_topology(int num_be, char *root, char **hosts, int attaching)
{
	char** tmp;
	int	L, N, K; //the number of leaves, the total number of tree nodes, and fanout
	int num_machines, num_procs;
	char machines_file[MAXPATHLEN];
	char topology_file[MAXPATHLEN];
	char *cfg_dir = getenv(GUARD_CFG_DIRECTORY);
	char *fanout_env = getenv(GUARD_MRNET_TOP_FANOUT);
	char topgen_cmd[1024];

	FILE*	machines = NULL;

	if(cfg_dir == NULL)
	{
		PrintOutError("ERROR: Cannot find %s! Please set up your MRNet configurations!\n", GUARD_CFG_DIRECTORY);
		return -1;
	}
	snprintf(machines_file, MAXPATHLEN-1, "%s/%s", cfg_dir, MACHINES_FILE);
	snprintf(topology_file, MAXPATHLEN-1, "%s/%s", cfg_dir, MRNET_TOPOLOGY);

	machines = fopen(machines_file, "w");
	if(machines == NULL)
	{
		PrintOutError("ERROR: Cannot create %s! Please set up your MRNet configurations!\n", machines_file);
		return -1;
	}

	if(fanout_env != NULL)
	{
		fanout_degree = atoi(fanout_env);	
	}

	if(fanout_degree <= 0)
	{
		fanout_degree = DEFAULT_TOP_FANOUT_DEGREE;
	}

	K = fanout_degree;
	L = ceil((float)num_be / K);
	N = ceil((float)(L * K)/(K-1));

	//create machine file
	num_machines = 0;
	tmp = hosts;
	while (*tmp++ != (char *)NULL)
		num_machines++;

	fprintf(machines, "%s\n", root);
	num_procs = ceil((float)N / num_machines);
	tmp = hosts;
	while (*tmp != (char *)NULL)
	{
		fprintf(machines, "%s:%d\n", *tmp++, num_procs);
	}
	fclose(machines);

	//invoke mrnet_topgen to create k-nomial topology
	sprintf(topgen_cmd, "%s --host=%s --topology=k:%d@%d -o %s", "mrnet_topgen", machines_file, K, N, topology_file);
	DPRINT("calling mrnet_topgen to create k-nomial topology: \n\t%s\n", topgen_cmd);
	return system(topgen_cmd);
}

int
create_mrnet_topology(int num_be, char *root, char **hosts, int attaching)
{
	char *	type = NULL;
	int	res = 0;

	topology_type = MRN_TOP_BALANCED;
	type = getenv(GUARD_MRNET_TOP_TYPE);
	if(type != NULL && strcmp(type, "K") == 0)
		topology_type = MRN_TOP_KNOMIAL;

	switch (topology_type)
	{
	case MRN_TOP_BALANCED:
		res = create_balanced_topology(num_be, root, hosts, attaching);
		break;
	case MRN_TOP_KNOMIAL:
		res = create_knominal_topology(num_be, root, hosts, attaching);
		break;
	default:
		res = -1;
	}	

	return res;
}

/* TODO: check apid */

int
create_parents_info_with_balanced_top(MRNet_Topology *topology, int num_be)
{
        int	        num_nodes, num_level, up_fanout, low_fanout;
	int             up_count, low_count;
	int             index, inner, up_index = 0;
	int             dbgsrv_id = 0;
	int             loop;
	char *          dir;
	char            connections[MAXPATHLEN];
	char            temp_conn[MAXPATHLEN];
	FILE *          pfile;
	MRNet_Node_t *  leaf_nodes;

	dir = getenv(GUARD_CFG_DIRECTORY);
	
	if(dir == NULL)
	{
		PrintOutError("ERROR: create_parents_info cannot find GUARD_MRNET_CFG! Please set up MRNet configurations!"); 	
		return -1;
	}

	snprintf(connections, MAXPATHLEN-1, "%s/%s", dir, MRNET_ATTACH_CONNECTIONS);
	snprintf(temp_conn, MAXPATHLEN-1, "%s/%s_", dir, MRNET_ATTACH_CONNECTIONS);

	/* one MRNet leafnode on each machine
	num_nodes = topology_print_leaves(topology, temp_conn);
	if(num_nodes > 0)
	{
		if(rename(temp_conn, connections))
		{
			PrintOutError("ERROR: create_parents_info cannot rename %s to %s\n", temp_conn, connections);
			perror("rename failed!");
			return -1;
		}

		return num_nodes;
	}
	*/
	
	num_level = level_count(num_be, fanout_degree);
	
	if (fanout_degree_count(num_be, num_level, &up_fanout, &low_fanout) < 0)
	{
		PrintOutError("ERROR: create_parents_info calling fanout_degree returned errors! num_be[%d], num_level[%d]\n", num_be, num_level);
		return -1;
	}

	if (fanout_count(num_be, num_level, &up_fanout, &low_fanout, &up_count, &low_count) < 0)
	{
		PrintOutError("ERROR: create_parents_info calling fanout_count returned errors! num_be[%d], num_level[%d]\n", num_be, num_level);
	}

	num_nodes = topology_get_leaves(topology, &leaf_nodes);

	DPRINT("INFO: create_parents_info num_nodes[%d], up_fanout[%d]x[%d], low_fanout[%d]x[%d]\n", num_nodes, up_fanout, up_count, low_fanout, low_count);

	if(num_nodes != (up_count + low_count))
	{
		PrintOutError("WARNING: create_parents_info num_nodes mismatch! num_nodes[%d], up_fanout[%d]x[%d], low_fanout[%d]x[%d]\n", num_nodes, up_fanout, up_count, low_fanout, low_count);
	}

	if ((pfile = fopen(temp_conn, "w")) == (FILE *)NULL)
	{
                PrintOutError("ERROR: cannot open %s to print MRNet leaves!\n", temp_conn);
                return 0;
        }

	for(index = 0; index < num_nodes; ++index)
	{
		loop = up_fanout;
		++up_index;
		if(up_index > up_count)
			loop = low_fanout;	

		for(inner = 0; inner < loop; ++inner)	
		{
			// resolve the node hostname to its IPv4 address
			struct addrinfo hints;
			hints.ai_family = AF_INET;
			struct addrinfo *infoptr = NULL;
			if (getaddrinfo(leaf_nodes[index].hostname, NULL, &hints, &infoptr) || !infoptr) {
				PrintOutError("ERROR: failed to resolve frontend hostname %s\n", leaf_nodes[index].hostname);
				perror("getaddrinfo");
				return -1;
			}

			// extract the IP address string
			char node_ipaddr[MAXIPADDRLEN];
			getnameinfo(infoptr->ai_addr, infoptr->ai_addrlen, node_ipaddr, sizeof(node_ipaddr), NULL, 0, NI_NUMERICHOST);
			freeaddrinfo(infoptr);

			// we should make this a constant width so we can speed up the parsing of the be_attach.dat file
			// 7 digit width for dbgsrv_id - meaning we can have up to 9,999,999 nodes, should be good enough into the distant future
			// 15 character width for ip address - XXX.XXX.XXX.XXX
			// 5 digit width for portnum - ok as long as they don't change the standard on us (65535)
			// 6 digit mrnet rank - this is highest dbgsrv_id divided by fanout
			fprintf(pfile, "%-7d %15s %-5d %-6d\n", dbgsrv_id, node_ipaddr, leaf_nodes[index].port, leaf_nodes[index].rank);
			++dbgsrv_id;
		}
	}
	fclose(pfile);

	if(rename(temp_conn, connections))
	{
		PrintOutError("ERROR: create_parents_info cannot rename %s to %s\n", temp_conn, connections);
		perror("rename failed!");
		return -1;
	}
 
	release_nodes(leaf_nodes, num_nodes);

	return num_nodes;
}

int
create_parents_info_with_knomial_top(MRNet_Topology *topology, int num_be)
{
	int             dbgsrv_id = 0;
	int				inner, index, loop;
	int				num_nodes, num_attach_floor, num_attach_ceil, num_floor, num_ceil;
	float			num_attach;
	MRNet_Node_t*	leaf_nodes;

	char*			dir;
	char			connections[MAXPATHLEN];
	char			temp_conn[MAXPATHLEN];
	FILE*			pfile;

	dir = getenv(GUARD_CFG_DIRECTORY);
	if(dir == NULL)
	{
		PrintOutError("ERROR: create_parents_info cannot find GUARD_MRNET_CFG! Please set up MRNet configurations!"); 	
		return -1;
	}

	snprintf(connections, MAXPATHLEN-1, "%s/%s", dir, MRNET_ATTACH_CONNECTIONS);
	snprintf(temp_conn, MAXPATHLEN-1, "%s/%s_", dir, MRNET_ATTACH_CONNECTIONS);

	if ((pfile = fopen(temp_conn, "w")) == (FILE *)NULL)
	{
		PrintOutError("ERROR: cannot open %s to print MRNet leaves!\n", temp_conn);
		return 0;
	}

	num_nodes = topology_get_leaves(topology, &leaf_nodes);

	num_attach = (float)num_be/num_nodes;	
	num_attach_ceil = ceil(num_attach);
	num_attach_floor = floor(num_attach);

	num_ceil = num_be - num_attach_floor * num_nodes;
	num_floor = num_nodes - num_ceil;

	dbgsrv_id = 0;
	for(index = 0; index < num_nodes; ++index)
	{
		loop = num_attach_ceil;
		if(index > num_ceil)
			loop = num_attach_floor;

		for(inner = 0; inner < loop; ++inner)
		{
			// resolve the node hostname to its IPv4 address
			struct addrinfo hints;
			hints.ai_family = AF_INET;
			struct addrinfo *infoptr = NULL;
			if (getaddrinfo(leaf_nodes[index].hostname, NULL, &hints, &infoptr) || !infoptr) {
				PrintOutError("ERROR: failed to resolve frontend hostname %s\n", leaf_nodes[index].hostname);
				perror("getaddrinfo");
				return -1;
			}

			// extract the IP address string
			char node_ipaddr[MAXIPADDRLEN];
			getnameinfo(infoptr->ai_addr, infoptr->ai_addrlen, node_ipaddr, sizeof(node_ipaddr), NULL, 0, NI_NUMERICHOST);
			freeaddrinfo(infoptr);

			// we should make this a constant width so we can speed up the parsing of the be_attach.dat file
			// 7 digit width for dbgsrv_id - meaning we can have up to 9,999,999 nodes, should be good enough into the distant future
			// 15 character width for ip address - XXX.XXX.XXX.XXX
			// 5 digit width for portnum - ok as long as they don't change the standard on us (65535)
			// 6 digit mrnet rank - this is highest dbgsrv_id divided by fanout
			fprintf(pfile, "%-7d %15s %-5d %-6d\n", dbgsrv_id, node_ipaddr, leaf_nodes[index].port, leaf_nodes[index].rank);
			++dbgsrv_id;
		}
	}

	fclose(pfile);

	if(rename(temp_conn, connections))
	{
		PrintOutError("ERROR: create_parents_info cannot rename %s to %s\n", temp_conn, connections);
		perror("rename failed!");
		return -1;
	}
 
	release_nodes(leaf_nodes, num_nodes);

	return num_nodes;
}

int
create_parents_info(MRNet_Topology *topology, int num_be)
{
	int res = 0;

	switch (topology_type)
	{
	case MRN_TOP_BALANCED:
		res = create_parents_info_with_balanced_top(topology, num_be);
		break;
	case MRN_TOP_KNOMIAL:
		res = create_parents_info_with_knomial_top(topology, num_be);
		break;
	default:
		res = -1;
	}

	return res;
}

int
delete_file(char *file)
{
	struct stat file_stat;

	if(!stat(file, &file_stat))
	{
		if(remove(file))
		{
			PrintOutError("failed to delete %s!\n", file);
			perror("Cannot delete file!");
			return -1;
		}
	}

	return 0;
}

/*
	I am called when a SIGUSR2 arrives from guard_starter_alps (via signal()). 
	This indicates that all the MRNet configurations files have been created.
 */
static void
mrncfg_callback(int sigNum)
{
/*TODO deal with the case that guard_sterter_alps crashes before sending signal */

  DPRINT("\nGuard received signal[%d]!\n", sigNum);
  mrnetcfg_files_created = 1;
}

/* this is just for ALPS 
* we first delete all rubbish configuration files to prevent all possible confusability
* TODO: use IPC to make guard_starter_alps talk with dbgclnt_mrn
*/
int
prepare_mrnetcfg_files()
{
	char aprunid_name[MAXPATHLEN];
	char machines_name[MAXPATHLEN];
	char topology_name[MAXPATHLEN];
	char attach_name[MAXPATHLEN];

	char *cfg_dir = getenv(GUARD_CFG_DIRECTORY);

	if(cfg_dir == NULL)
	{
		PrintOutError("ERROR: Cannot find %s! Please set up your MRNet configurations!\n", GUARD_CFG_DIRECTORY);
		return -1;
	}

	snprintf(topology_name, MAXPATHLEN-1, "%s/%s", cfg_dir, MRNET_TOPOLOGY);
	snprintf(machines_name, MAXPATHLEN-1, "%s/%s", cfg_dir, MACHINES_FILE);
	snprintf(attach_name, MAXPATHLEN-1, "%s/%s", cfg_dir, MRNET_ATTACH_CONNECTIONS);
	snprintf(aprunid_name, MAXPATHLEN-1, "%s/%s", cfg_dir, ALPS_APRUNID_FILE);

	if(delete_file(machines_name) || delete_file(topology_name) || delete_file(attach_name) || delete_file(aprunid_name))
		return -1;

	DPRINT("mrn_topgen set handler for SIGUSR2!\n");
	signal(SIGUSR2, mrncfg_callback);

	return 0;
}

/*
* confirm guard_starter_alps has created new configuration files
*/
int
check_mrnetcfg_files()
{
	struct stat file_stat;
	char aprunid_name[MAXPATHLEN];
	char machines_name[MAXPATHLEN];

	char *cfg_dir = getenv(GUARD_CFG_DIRECTORY);

	if(cfg_dir == NULL)
	{
		PrintOutError("ERROR: Cannot find %s! Please set up your MRNet configurations!\n", GUARD_CFG_DIRECTORY);
		return -1;
	}

	snprintf(machines_name, MAXPATHLEN-1, "%s/%s", cfg_dir, MACHINES_FILE);
	snprintf(aprunid_name, MAXPATHLEN-1, "%s/%s", cfg_dir, ALPS_APRUNID_FILE);

	int counter = 0;

	while(stat(aprunid_name, &file_stat) || stat(machines_name, &file_stat))
	{
		PrintOutError("INFO: %s or %s has not been created! errno of stat[%d]\n", machines_name, aprunid_name, errno);
		//return -1;
		sleep(1);
		counter++;
	}

	DPRINT("check_mrnetcfg_files waited %d seconds\n", counter);
	return 0;
}

/*
* wait guard_starter_alps to create new configuration files
*/
int
wait_for_creating_mrnetcfg_files()
{
    while(!mrnetcfg_files_created);

    return check_mrnetcfg_files();
}

/*Each dbgsrv's mrn_rank = ATTACHBE_RANK_SKEW + mpi_rank*/
int
MRNRankToProcRank(int mrnRank)
{
	return (mrnRank - ATTACHBE_RANK_SKEW);
}

int
get_mrn_filter_timeout()
{
	int filter_timeout = DEFAULT_FILTER_TIMEOUT;
	char *timeout_str = getenv(GUARD_MRNET_FILTER_TIMEOUT);
	if(timeout_str != NULL)
		filter_timeout = atoi(timeout_str);
	
	return filter_timeout;
}

