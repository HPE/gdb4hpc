/*********************************************************************************\
 * Server MRNet dispatch routines
 *
 * Copyright (c) 1996-2012 by Monash Uni.
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
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



#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<sys/types.h>
#include	<sys/time.h>
#include	<sys/socket.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<netdb.h>

#include	"defs.h"
#include	"useful.h"
#include	"aif.h"
#include	"dbgevent.h"
#include	"dbgformat.h"
#include	"dbgsrv_proto.h"
#include	"dbgfn.h"
#include	"dbgsrv_defs.h"
#include	"dbgsrv_cmds.h"
#include 	"dbg_mrn_be.h"
#include	"mrn_cfg.h"
#include	"mrn_topo.h"
#include	"smart_bitset.h"
#include	"bitset.h"

#ifdef ALPS
#include	"common_tools_be.h"
#endif

/*
we use mpd to launch MRNet backends on non-Cray platform.
Not sure why "PMI_RANK" does not work.
We use "MPD_JRANK" instead.
*/
#ifndef ALPS
static char * MPIRankVars[] = {
    "OMPI_COMM_WORLD_RANK",     	/* Open MPI 1.6 */
    "OMPI_MCA_orte_ess_vpid",       /* Open MPI 1.3 */
    "OMPI_MCA_ns_nds_vpid",         /* Open MPI 1.2 */
    "PMI_RANK",                     /* MPICH2. PMI_RANK does not work for MPICH2. */
    "MPD_JRANK",                    /* MPICH2. MPD_JRANK works on MPICH2. */
    "MP_CHILD",                     /* IBM PE */
    "SLURM_PROCID",                 /* SLURM */
    NULL
};
#endif

// Static global vars
static MRNet_Stream *	stream;
static int				tag;
static int				my_mrn_rank;
static int				mrn_event_fd;
#ifdef PERFLOG
extern struct timeval	tv_srv;
extern struct timeval	tv_para;
extern struct timeval	tv_arch;
extern struct timeval	tv_init;
extern struct timeval	tv_io;
extern struct timeval	tv_end;
struct timeval			tv_att;
#endif

// Prototypes
static int			DbgSRVMRNInitProto(int, int, char **, char*, char*, int);
static void			DbgSRVMRNEndProto(void);
static fd_set *		DbgSRVMRNGetFDSet(void);
static void			DbgSRVMRNRunProto(fd_set *);
static int			InvokeMRNCallBack(char *, int, char *, int, int, int, dbgevent_t *);
static int			EventMRNCallBack(dbgevent_t *);
static int			DbgSRVMRNSetupIOChannel(int* fd, int* port);
static int			DbgSRVMRNTransferIO(int fd, char* buffer, int size);
static dbgevent_t *	DbgSRVMRNInvokeNetwork(int numbe);
static int			DbgSRVMRNSendReply(char* response);

// Proto definition
dbgsrv_proto	DbgSrvMRNProtoSW =
{
	DbgSRVMRNInitProto,
	DbgSRVMRNEndProto,
	DbgSRVMRNGetFDSet,
	DbgSRVMRNRunProto,
	InvokeMRNCallBack,
	EventMRNCallBack,
	DbgSRVMRNSetupIOChannel,
	DbgSRVMRNTransferIO,
	DbgSRVMRNInvokeNetwork,
	DbgSRVMRNSendReply
};

// Callbacks
static int
InvokeMRNCallBack(char *chost, int cport, char *host, int prog, int pnum, int port, dbgevent_t *e)
{
	//we have nothing to do here
	return 1;
}

static int
EventMRNCallBack(dbgevent_t *ev)
{
	int		res;
	char *	st_dres;
	
	st_dres = EventToSocProto(ev);

	DVPRINT("EventMRNCallBack is invoked: %s!\n", st_dres);

	res = mrn_be_send_event(st_dres);
	Free(st_dres);
	
	return res;
}

static int
DbgSRVMRNInitProto(int procnum, int argc, char **argv, char* key, char* host, int port)
{
	int	res;

#ifdef ALPS /* wth ALPS, dbgsrv is supposed to attach to mrnet */
	int		pmi_rank = procnum;
	int		be_argc = 6;
	char	* hostname;
	char	par_addr[MAXADDRLEN+1];
	char	par_port_str[64];
	char	par_rank_str[64];
	char	my_rank_str[64];
	char *	be_argv[6];
	unsigned par_port, par_rank;
	unsigned my_mrnrank;
	
	if ((hostname = cti_be_getNodeHostname()) == NULL)
	{
		fprintf(stderr, "DbgSRVMRNInitProto cannot get hostname\n");
		++Shutdown;
		
		return -1;
	}
	DVPRINT("INFO: hostname[%s] is attaching to MRNet network\n", hostname);

	//get parent information
	res = get_myparent_info(pmi_rank, hostname, par_addr, &par_port, &par_rank);
	if(res < 0)
	{
		fprintf(stderr, "DbgSRVMRNInitProto cannot find parent to attach!\n");
		++Shutdown;
		
		return -1;	
	}

	//calculate local rank
	my_mrnrank = (unsigned)ProcRankToMRNRank(pmi_rank);

	sprintf(par_port_str, "%d", par_port);
	sprintf(par_rank_str, "%d", par_rank);
	sprintf(my_rank_str, "%d", my_mrnrank);

	be_argv[0] = argv[0];
	be_argv[1] = par_addr;
	be_argv[2] = par_port_str;
	be_argv[3] = par_rank_str;
	be_argv[4] = hostname;
	be_argv[5] = my_rank_str;

	DVPRINT("DbgSRVMRNInitProto is attaching to: par_addr[%s] par_port[%d] par_rank[%d] my_rank[%d]\n", par_addr, par_port, par_rank, my_mrnrank);

	if (debug_flag)
	{
		fprintf(stderr, "BE argc: %d\n", be_argc);
		int i;
		for(i = 0; i < be_argc; i++)
		{
			fprintf(stderr, "BE argv[%d]=%s\n", i, be_argv[i]);
		}
	}

#ifdef PERFLOG
	gettimeofday(&tv_att, NULL);
#endif

DVPRINT("Creating be network");
	res = mrn_be_create_network( be_argc, be_argv );
  DVPRINT("Done Creating network");
	my_mrn_rank = pmi_rank; //my_mrn_rank is not rank in mrnet, but is used by client to identifiy dbgsrv 
	mrn_be_set_localrank(my_mrn_rank);
  DVPRINT("Done setting local rank");
#else /*end of ALPS*/

	int		c;
	char **	var;
	char *	envval;
	char *	cfg_env = NULL;
	char 	single_rank[16];
	int		be_argc = 6;
	char	hostname[MAXPATHLEN];
	char	par_addr[MAXADDRLEN+1];
	char	par_port_str[64];
	char	par_rank_str[64];
	char	my_rank_str[64];
	char *	be_argv[6];
	int		mpi_rank = -1;
	unsigned par_port, par_rank;
	unsigned my_mrnrank;

	optind = 0;
	while ( (c = getopt(argc, argv, "p:h:c:a:n:i:e:k:v:d:")) != EOF )
	{
		switch ( c )
		{
		case 'v':
			cfg_env = optarg;
			break;
		case 'p':
		case 'h':
		case 'c':
		case 'a':
		case 'n':
		case 'i':
		case 'e':
		case 'k':
		case 'd':
			break;
		}
	}

	if(cfg_env == NULL)
	{
		fprintf(stderr, "ERROR: GUARD_CFG_DIRECTORY argument is missing!\n");
		return -1;
	}

	DVPRINT("DbgSRVMRNInitProto GUARD_CFG_DIRECTORY=%s\n", cfg_env);
	setenv(GUARD_CFG_DIRECTORY, cfg_env, 1);

	for (var = MPIRankVars; *var != NULL; var++) {
	envval = getenv(*var);
	if (envval != NULL) {
		break;
	}
	}

	if(envval == NULL) /* we assume only single process is debugged */
	{
		/*
		fprintf(stderr, "ERROR: DbgSRVSCIInitProto cannot get MPIRank\n");
		return -1;
		*/
		sprintf(single_rank, "0");
		envval = single_rank;
		DVPRINT("DbgSRVMRNInitProto cannot get MPIRank. Debugging single process is assumed!\n");
	}

	sscanf(envval, "%d", &mpi_rank);

	gethostname(hostname, MAXPATHLEN);
	DVPRINT("INFO: hostname[%s]:mpirank[%d] is attaching to MRNet network\n", hostname, mpi_rank);

	//get parent information
	res = get_myparent_info(mpi_rank, hostname, par_addr, &par_port, &par_rank);
	if(res < 0)
	{
		fprintf(stderr, "dbgsrv[%d]: DbgSRVMRNInitProto cannot find parent to attach!\n", mpi_rank);
		++Shutdown;
		
		return -1;	
	}

	//calculate local rank
	my_mrnrank = (unsigned)ProcRankToMRNRank(mpi_rank);

	sprintf(par_port_str, "%d", par_port);
	sprintf(par_rank_str, "%d", par_rank);
	sprintf(my_rank_str, "%d", my_mrnrank);

	be_argv[0] = argv[0];
	be_argv[1] = par_addr;
	be_argv[2] = par_port_str;
	be_argv[3] = par_rank_str;
	be_argv[4] = hostname;
	be_argv[5] = my_rank_str;

	DVPRINT("DbgSRVMRNInitProto is attaching to: par_addr[%s] par_port[%d] par_rank[%d] my_rank[%d]\n", par_addr, par_port, par_rank, my_mrnrank);

	res = mrn_be_create_network( be_argc, be_argv );
	my_mrn_rank = mpi_rank; //my_mrn_rank is not rank in mrnet, but is used by client to identifiy dbgsrv 
	mrn_be_set_localrank(my_mrn_rank);
//	res = mrn_be_create_network( argc, argv );
//	my_mrn_rank = mrn_be_get_localrank();
#endif

	mrn_be_set_callback(dbgsrv_dispatch_cmds);
	
	// get the mrnet event fd to select on
	mrn_event_fd = mrn_be_get_event_fd();

	return res;
}

static void
DbgSRVMRNEndProto(void)
{ 
	/*todo: delete network? */
	DVPRINT("Waiting for MRNet network shutdown.\n");
	mrn_be_waitfor_shutdown();
}

static fd_set *
DbgSRVMRNGetFDSet(void)
{
	static fd_set	fds;
	
	FD_ZERO(&fds);
	FD_SET(mrn_event_fd, &fds);
	
	return &fds;
}

static int
DbgSRVMRNSetupIOChannel(int* fd, int* port)
{
	return 0;
}

static int
DbgSRVMRNTransferIO(int fd, char* buffer, int size)
{
	buffer[size] = '\0';

	DVPRINT("DbgSRVMRNTransferIO: %s\n", buffer);

	mrn_be_send_io(buffer);
	
	return 0;
}

/*
* This is the real part checking and delivering any packets from MRNet frontend
*/
static void
DbgSRVMRNRunProto(fd_set *fds)
{
	int res;
	
	DVPRINT("INFO: enterring DbgSRVMRNRunProto.\n");

	if(mrn_be_is_shutdown())
	{
		DVPRINT("DbgSRVMRNRunProto found mrnet is in a shutdown state!\n");
		DVPRINT("Quiting...\n");
		DbgQuit(0);

		return;
	}
	
	// make sure event fd is ready to read on
	if (!FD_ISSET(mrn_event_fd, fds))
	{
		return;
	}
	
	FD_CLR(mrn_event_fd, fds);
	
	// clear the mrnet event fd
	mrn_be_clear_event_fd();
	
	// service all pending events, otherwise the event fd might not trigger
	while (mrn_be_num_events_pending())
	{
		DVPRINT("INFO: Servicing MRNet event.\n");
		
		// Acknowledge the event
		mrn_be_ack_event();
		
		// receive the event
		res = mrn_be_recv(&stream, &tag, MRNET_UNBLOCKING);
		
		if( res <0 ) {
			fprintf(stderr, "DbgSRVMRNRunProto() mrn_be_recv() failed!\n");
			exit(1);
			}
		if(res > 0)	
		{
			DVPRINT("Recieved event on stream[%d]\n", tag);
			mrn_be_handle_msg();
		}
		
		DVPRINT("INFO: Finished servicing MRNet Event.\n");
	}

	DVPRINT("INFO: exiting DbgSRVMRNRunProto.\n");
}

// FIXME: This should be returning an event for each stream setup so that the
// frontend knows that setup completed on each of the streams
// TODO: This is calling the send event instead of send reply function, so its
//        incompatible with the caller...
static dbgevent_t *
DbgSRVMRNInvokeNetwork(int numbe)
{
	if(tag == DBG_MRNet_PRO_COMMAND)
	{
		mrn_be_set_cmd_stream(stream);
		DVPRINT("[dbgsrv] Received SetupNetwork on command stream.\n");
		return NULL;
	}

	if(tag == DBG_MRNet_PRO_IO)
	{
		mrn_be_set_io_stream(stream);
		DVPRINT("[dbgsrv] Received SetupNetwork on io stream!\n");
		return NULL;
	}

	if(tag == DBG_MRNet_PRO_EVENT) 
	{
		dbgevent_t *	e;
		char *			resp;
		
		mrn_be_set_event_stream(stream);
		DVPRINT("[dbgsrv] Received SetupNetwork on event stream!\n");
	
		mrn_be_set_size(numbe);
		
		e = NewEvent(DBGEV_OK);
		resp = EventToSocProto(e);
		
		mrn_be_send_event(resp);
		Free(resp);
		
		return NULL;
	}
	
	return NULL;
}

static int
DbgSRVMRNSendReply(char* response)
{
	return mrn_be_send_reply(stream, response);
}

