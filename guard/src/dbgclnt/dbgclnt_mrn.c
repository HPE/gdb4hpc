/*********************************************************************************\
 * Debugger MRNet interface routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2014 Cray Inc.  All Rights Reserved.
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
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "defs.h"
#include "useful.h"
#include "aif.h"
#include "dbg_client.h"
#include "list.h"

#include "dbg_mrn_fe.h"

#ifdef ALPS
#include "cray_tools_fe.h"
#endif /* ALPS */

// Static global variables
static char *	wrapper_name = NULL;				// temporary mrnet commnode wrapper script that gets created
static char *	mrnet_port_base = NULL;				// Port base that MRNet should use - this is the default override with env var
													// which is read during the proto init step

// Static function prototypes
static int				DbgMRNInit();
static bitset *			DbgMRNSendCmd(dbginvoke_t *, bitset *, char *);
static proto_msg_set *	DbgMRNRecvReply(dbginvoke_t *, bitset *, bitset *);
static void				DbgMRNDispatchEvents(dbginvoke_t *, int ev_fd);
static int				DbgMRNAttach(	dbginvoke_t *, proto_session *, 
										int, char *, char *, int, int);
static int				DbgMRNInvoke(	dbginvoke_t *, proto_session *, 
										char *, char **, char *, char **, char *, 
										char *, char **, char *, int, int);
static int				DbgMRNExit(dbginvoke_t *, int);
static void				DbgMRNDestroyClient(dbginvoke_t *);

// protocol specific startup event callback functions
static int					cbMRNInvoke(dbginvoke_t *di, proto_msg_set *set);
static int					cbMRNAttach(dbginvoke_t *di, proto_msg_set *set);


dbgclnt_proto DbgClntMRNProtoSW =
{
	"mrnet",				/* sw_name */
	DbgMRNInit,				/* sw_init */
	NULL,					/* sw_finish */
	DbgMRNSendCmd,			/* sw_send_cmd */
	DbgMRNRecvReply,		/* sw_recv_reply */
	NULL,					/* sw_checkevents */
	DbgMRNDispatchEvents,	/* sw_dispatchevents */
	// MRNet does not implement sessions. These should only
	// be used inside of the proxy protocol.
	NULL,					/* sw_create_session */
	NULL,					/* sw_save_session */
	NULL,					/* sw_cleanup_session */
	DbgMRNAttach,			/* sw_attach */
	DbgMRNInvoke,			/* sw_invoke */
	DbgMRNExit,				/* sw_exit */
	DbgMRNDestroyClient		/* sw_destroyclnt */
};

// setup a mrnet_commnode_wrapper based on this sessions staging location
static int
create_mrnet_commnode_wrapper(cti_session_id_t sid)
{
	char *	lib_dir;
	char *	bin_dir;
	char **	lock_files;
	char **	ptr;
	char *	template_path;
	char *	cfg_dir;
	FILE *	sfp;
	int		ofd;
	FILE *	ofp;
	char *	line = NULL;
	size_t	len = 0;
	
	// get the backend library directory for this session
	if ((lib_dir = cti_getSessionLibDir(sid)) == NULL)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Could not determine lib dir for mrnet commnode wrapper.\n");
		return -1;
	}
	
	// get the backend binary directory for this session
	if ((bin_dir = cti_getSessionBinDir(sid)) == NULL)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Could not determine bin dir for mrnet commnode wrapper.\n");
		Free(lib_dir);
		return -1;
	}
	
	// get the null terminated list of lock files
	if ((lock_files = cti_getSessionLockFiles(sid)) == NULL)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Could not get lock files for mrnet commnode wrapper.\n");
		Free(lib_dir);
		Free(bin_dir);
		return -1;
	}

	// search for lgdb_mrnet_commnode_wrapper in path
	if ((template_path = pathFind("lgdb_mrnet_commnode_wrapper", NULL)) == NULL)
	{
		PrintOutError("Could not locate the lgdb_mrnet_commnode_wrapper script in path.\n");
		Free(lib_dir);
		Free(bin_dir);
		ptr = lock_files;
		while (*ptr != NULL) { Free(*ptr); ++ptr; }
		Free(lock_files);
		return -1;
	}
	
	// open the wrapper for reading
	if ((sfp = fopen(template_path, "r")) == NULL)
	{
		PrintOutError("Cannot open %s.\n", template_path);
		Free(template_path);
		Free(lib_dir);
		Free(bin_dir);
		ptr = lock_files;
		while (*ptr != NULL) { Free(*ptr); ++ptr; }
		Free(lock_files);
		return -1;
	}
	
	// cleanup
	Free(template_path);
	
	// cleanup existing wrapper_name
	if (wrapper_name != NULL)
	{
		remove(wrapper_name);
		Free(wrapper_name);
	}
	
	// allocate script name string
	wrapper_name = (char *)Alloc(MAXPATHLEN);
	
	// get configuration dir
	if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == NULL)
	{
		PrintOutError("Could not retrieve %s env var! Please check your environment settings.\n", GUARD_CFG_DIRECTORY);
		fclose(sfp);
		Free(wrapper_name);
		wrapper_name = NULL;
		Free(lib_dir);
		Free(bin_dir);
		ptr = lock_files;
		while (*ptr != NULL) { Free(*ptr); ++ptr; }
		Free(lock_files);
		return -1;
	}
	snprintf(wrapper_name, MAXPATHLEN-1, "%s/%s", cfg_dir, "mrnet_commnode_XXXXXX");
	
	// create a temporary file for the new wrapper script
	if ((ofd = mkstemp(wrapper_name)) < 0)
	{
		PrintOutError("mkstemp failed for %s.\n", wrapper_name);
		fclose(sfp);
		Free(wrapper_name);
		wrapper_name = NULL;
		Free(lib_dir);
		Free(bin_dir);
		ptr = lock_files;
		while (*ptr != NULL) { Free(*ptr); ++ptr; }
		Free(lock_files);
		return -1;
	}
	
	// Create a FILE * for this fd
	if ((ofp = fdopen(ofd, "w")) == NULL)
	{
		PrintOutError("Could not fdopen on wrapper script.\n");
		fclose(sfp);
		Free(wrapper_name);
		wrapper_name = NULL;
		Free(lib_dir);
		Free(bin_dir);
		ptr = lock_files;
		while (*ptr != NULL) { Free(*ptr); ++ptr; }
		Free(lock_files);
		return -1;
	}
	
	// copy each line in the template to the temporary file
	while (getline(&line, &len, sfp) >= 0)
	{
		// print this line to the new wrapper - no newline is needed
		fprintf(ofp, "%s", line);
		Free(line);
		line = NULL;
	}
	
	// close the template
	fclose(sfp);
	
	// write LD_LIBRARY_PATH command to the file
	fprintf(ofp, "export LD_LIBRARY_PATH=%s\n", lib_dir);
	Free(lib_dir);
	
	// write MRNET_COMM_PATH to file
	fprintf(ofp, "export MRNET_COMM_PATH=%s/mrnet_commnode\n", bin_dir);
	
	// write MRNET_PORT_BASE to file
	fprintf(ofp, "export MRNET_PORT_BASE=%s\n\n", mrnet_port_base);
	
	// write wait for lock files
	ptr = lock_files;
	while (*ptr != NULL)
	{
		fprintf(ofp, "while [ ! -e %s ]\n", *ptr);
		fprintf(ofp, "do\n");
		fprintf(ofp, "\tusleep 10000\n");
		fprintf(ofp, "done\n\n");
		Free(*ptr);
		++ptr;
	}
	Free(lock_files);
	
	// write exec statement
	fprintf(ofp, "exec %s/mrnet_commnode $@\n", bin_dir);
	Free(bin_dir);

	// flush the output file
	fflush(ofp);
	
	// close the file
	if (fclose(ofp))
	{
		PrintOutError("Could not fclose wrapper script\n");
		remove(wrapper_name);
		Free(wrapper_name);
		wrapper_name = NULL;
		return -1;
	}
	
	// Add execute permission to the file
	chmod(wrapper_name, S_IRWXU);
	
	// Set commnode env var
	setenv("MRNET_COMM_PATH", wrapper_name, 1);
	
	return 0;
}

static int
DbgMRNInit()
{
	struct stat		st;
	char *			mrnet_cfg_dir;
	char *			mrnet_port_base_env;
	
	// grab the config dir environment variable
	if ((mrnet_cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == (char *)NULL)
	{
		PrintOutError("Could not retrieve %s env var! Please check your environment settings.\n", GUARD_CFG_DIRECTORY);
		return 0;
	}
	
	// try to stat the directory
	if (stat(mrnet_cfg_dir, &st))
	{
		// XXX: This gets created in main. This should not fail, but it is good practice just to check
		PrintOutError("Could not find the config directory! Ensure you have write permissions in the %s path.\n", mrnet_cfg_dir);
		return 0;
	}
	
	// Try to grab the port base environment variable
	if ((mrnet_port_base_env = getenv(GUARD_MRNET_PORT)) != NULL)
	{
		// This was set, so we don't want to use our default value
		mrnet_port_base = strdup(mrnet_port_base_env);
	} else
	{
		// Use the default choice
		mrnet_port_base = strdup(DEFAULT_MRNET_PORT_BASE);
	}
	
	// Set the MRNET_PORT_BASE env var
	setenv("MRNET_PORT_BASE", mrnet_port_base, 1);

	return 1;
}

/*
** Attach to user program and build an MRNet tree.
*/
static int
DbgMRNAttach(
	dbginvoke_t *di,
	proto_session *sess,
	int pid,
	char *arch,
	char *backend,
	int gpu_flag,
	int debug_flag
)
{
	char *				root;
	char *				cfg_dir;
	char				topology_name[MAXPATHLEN];
	char				connections[MAXPATHLEN];
	int					num_be;
	int					rtn;
	MRNet_Topology *	mrn_topology;
	mrnet_type *		network;
	char *				request;
	char *				env_var_str;
	// Set this to the number of environment variables we need to ship over.
	char *				env_vars[5] = {NULL,NULL,NULL,NULL,NULL};
	int					env_var_cnt = 0;
	int					i;
	// init this to the default timeout
	unsigned long int	mrnet_timeout = GUARD_DEFAULT_TIMEOUT;
	char **				ap;
	int					event_fd;
	int *				ev_fd_ptr;
#ifdef ALPS
	int					dbg=debug_flag;
	char **				machine_hosts;
	cti_manifest_id_t	mid;
	cti_session_id_t	sid;
#endif /* ALPS */
	char * 				env_lib_dir;
	char *				python_lib_dir;
	char *				gdb_lib_dir;

#ifdef DEBUG
	dbg = 1;
#endif

	// ensure the dbginvoke_t object was passed down
	if (di == NULL)
		return 1;
	
	// Set the initial protocol event callback to use our custom event handler
	// for launching
	di->d_proto_ev_callback = cbMRNAttach;

	// get mrnet configuration information
	if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == (char *)NULL)
	{
		PrintOutError("ERROR: Cannot find %s. Please set MRNet environment variables!\n", GUARD_CFG_DIRECTORY);
		return 1;
	}
	snprintf(topology_name, MAXPATHLEN-1, "%s/%s", cfg_dir, MRNET_TOPOLOGY);
	snprintf(connections,	MAXPATHLEN-1, "%s/%s", cfg_dir, MRNET_ATTACH_CONNECTIONS);

	/*
	** using the tools interface, we can interact with the compute nodes directly
	** this means that there is no longer any need for a starter application on the
	** front end.
	*/
	PrintOutMsg("Attaching to application, please wait...\n");
	
	// if PERFLOG is turned on, we need to create a string to the location to 
	// create the log file.
#ifdef PERFLOG
	char *envDir;
	char logfile[PATH_MAX];
	
	if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)NULL)
	{
		// user didn't set the environment variable, so lets
		// read the HOME variable
		if ((envDir = getenv("HOME")) == (char *)NULL)
		{
			// couldn't read HOME, so lets default to tmp
			envDir = "/tmp";
		}
	}
	
	// create the fullpath string to the log file using PATH_MAX plus a null term
	snprintf(logfile, PATH_MAX+1, "%s/mrnet_perf_frontend.log", envDir);
	
	DPERFLOG(CreateTimerLog(logfile));
	DPERFLOG(SetLogInfo("Attaching to processes\n"));
	DPERFLOG(SetTimerStart("Attach total"));
#endif /* PERFLOG */

	// Register the aprun pid with the tool interface
	if ((di->d_appid = cti_registerApid(pid)) == 0)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to register aprun pid with CTI.\n");
		
		return 1;
	}

	// TODO: If we really wanted to, we could attach to the existing aprun
	// process here and redirect its /proc stdout and stderr to our output stream
	// handler.
	
	// ship over the binaries to the compute nodes
	DPERFLOG(SetTimerStart("transfer binaries"));
	
	// Create a new manifest to hold the stuff we want to ship
	if ((mid = cti_createNewManifest(0)) == 0)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to create a cti_manifest_id_t.\n");
		
		return 1;
	}
	
	// Add gdb-guard to the manifest
	if (cti_addManifestBinary(mid, "gdb-guard"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add gdb-guard to manifest.\n");
		cti_destroyManifest(mid);
		
		return 1;
	}
	
	// This checks the setting of gpu_flag to figure out if cuda-gdb should be
	// sent over
	if (gpu_flag)
	{
		// The backend code will determine which version of cuda-gdb to use
		if (cti_addManifestBinary(mid, "cuda50-gdb-guard"))
		{
			PrintOutError("CTI error: %s\n", cti_error_str());
			PrintOutError("Failed to add cuda50-gdb-guard to manifest.\n");
			cti_destroyManifest(mid);
			
			return 1;
		}
		if (cti_addManifestBinary(mid, "cuda55-gdb-guard"))
		{
			PrintOutError("CTI error: %s\n", cti_error_str());
			PrintOutError("Failed to add cuda55-gdb-guard to manifest.\n");
			cti_destroyManifest(mid);
			
			return 1;
		}
	}

	// Add dbgsrv to the manifest
	if (cti_addManifestBinary(mid, "dbgsrv"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add dbgsrv to manifest.\n");
		cti_destroyManifest(mid);
		
		return 1;
	}
	
	// Add mrnet_commnode to the manifest
	if (cti_addManifestBinary(mid, "mrnet_commnode"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add mrnet_commnode to manifest.\n");
		cti_destroyManifest(mid);
		
		return 1;
	}
	
	// Get the libdir env var
	if ((env_lib_dir = getenv(GUARD_LIB_DIRECTORY)) == NULL)
	{
		PrintOutError("Failed to getenv on %s. Please check your environment setup.\n", GUARD_LIB_DIRECTORY);
		cti_destroyManifest(mid);
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	// Add the python modules
	Asprintf(&python_lib_dir, "%s/python2.6", env_lib_dir);
	if (cti_addManifestLibDir(mid, python_lib_dir))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add python2.6 to manifest.\n");
		cti_destroyManifest(mid);
		
		return 1;
	}
	Free(python_lib_dir);
	
	// Now add the gdb pretty printers
	Asprintf(&gdb_lib_dir, "%s/gdb", env_lib_dir);
	if (cti_addManifestLibDir(mid, gdb_lib_dir))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add gdb pretty printers to manifest.\n");
		cti_destroyManifest(mid);
		
		return 1;
	}
	Free(gdb_lib_dir);
	
#if 1	// RWM
	// send over the uda plugin - only if gpu mode is currently turned off
	if (!gpu_flag)
	{
		if (cti_addManifestLibrary(mid, "uda-plugin-cray-upc.so"))
		{
			PrintOutError("CTI error: %s\n", cti_error_str());
			PrintOutError("Failed to transfer uda-plugin-cray-upc.so.\n");
			cti_killApp(di->d_appid, SIGKILL);
			sigrelse(SIGCHLD);
		
			return 1;
		}
	}
// End upc addition
#endif

#if 0	// use with strace for libmi
	if (cti_addManifestBinary(mid, "strace"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add strace to manifest.\n");
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
#endif

	// Add libthread_db otherwise gdb will be messed up with thread
	// support
	if (cti_addManifestLibrary(mid, "libthread_db.so.1"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add libthread_db.so.1 to manifest.\n");
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
#ifdef BE_VALGRIND
	// Add valgrind to the manifest
	if (cti_addManifestBinary(mid, "valgrind"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add valgrind to manifest.\n");
		cti_destroyManifest(mid);
		
		return 1;
	}
#endif
	
	// Add the aggregation filter to the manifest
	if (cti_addManifestLibrary(mid, AGGREGATION_FILTER))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add %s to manifest.\n", AGGREGATION_FILTER);
		cti_destroyManifest(mid);
		
		return 1;
	}
	
	// Stage this manifest on the compute node, we need to remember the sid for
	// later.
	if ((sid = cti_sendManifest(di->d_appid, mid, dbg)) == 0)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to ship the manifest!\n");
		
		return 1;
	}
	
	DPERFLOG(SetTimerStopAndLog("transfer binaries"));
	
	DPRINT("Aprun session initialized.\n");
	PrintOutMsg("Creating MRNet communication network...\n"); 
	
	DPRINT("MRNet is creating a tree that the dbgsrvs can attach to.\n");
	
	// get the number of processes associated with this aprun
	if ((di->d_nproc = cti_getNumAppPEs(di->d_appid)) == 0)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to retrieve nproc.\n");
		cti_destroyManifest(mid);
		
		return 1;
	}
	
	// create the root hostname for mrnet
	if ((root = cti_getHostname()) == NULL)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to retrieve node hostname.\n");
		
		return 1;
	}
	
	DPRINT("Application has apid[%d]\n", di->d_appid);
	DPRINT("Root has a hostname of [%s]\n", root);
	
	// get a list of the nid hostnames for topology creation
	if ((machine_hosts = cti_getAppHostsList(di->d_appid)) == (char **)NULL)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to retrieve nid hostname list.\n");
		
		return 1;
	}
	
#ifdef DEBUG
	char **tmp;
	DPRINT("Application has the following nid hostnames list:\n");
	tmp = machine_hosts;
	while (*tmp != (char *)NULL)
	{
		DPRINT("%s ", *tmp++);
	}
	DPRINT("\n");
#endif /* DEBUG */

	// setup a mrnet_commnode_wrapper based on this sessions staging location
	if (create_mrnet_commnode_wrapper(sid))
	{
		PrintOutError("ERROR: DbgMRNAttach failed to create MRNet topology file!\n"); 
		
		return 1;
	}

	// create the topology file used to create the mrnet front end network
	if(create_mrnet_topology(di->d_nproc, root, machine_hosts, 1))
	{
		PrintOutError("ERROR: DbgMRNAttach failed to create MRNet topology file!\n"); 
		
		return 1;
	}

	DPRINT("MRNet is going to create a tree with topology[%s]\n", topology_name);
	
	DPERFLOG(SetTimerStart("Create mrnet network"));
	
	network = mrn_fe_create_network_no_be(topology_name, &mrn_topology, pid);
	
	DPERFLOG(SetTimerStopAndLog("Create mrnet network"));
	
	// create the backend attach file which the compute nodes will use to hook up to the network
	if (create_parents_info(mrn_topology, di->d_nproc) < 1)
	{
		PrintOutError("ERROR: DbgMRNAttach cannot create infomation for MRNet attachment!\n");
		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		
		return 1;
	}
	
	// Create a new manifest based on the existing session to send additional stuff
	if ((mid = cti_createNewManifest(sid)) == 0)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to create a cti_manifest_id_t.\n");
		
		return 1;
	}
	
	// Add the be_attach.dat file to the manifest
	if (cti_addManifestFile(mid, connections))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add backend attach information to manifest.\n");
		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		
		return 1;
	}
	
	DPRINT("MRNet network initialized.\n");
	DPRINT("Launching debug servers onto compute nodes.\n");
	
	// Transfer and execute dbgsrv_alps with call line info:
	// dbgsrv_alps -h <cbhost> -c <cbport> -a <arch> -p <proto> -e <backend>
	// first create the argv for dbgsrv_alps
	// This now also depends on gpu_flag
	if (dbg)
	{
		if (gpu_flag)
		{
			ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p", di->d_proto->sw_name, "-e", backend, "--gpu", "--debug", NULL);
		} else
		{
			ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p", di->d_proto->sw_name, "-e", backend, "--debug", NULL);
		}
	} else
	{
		if (gpu_flag)
		{
			ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p", di->d_proto->sw_name, "-e", backend, "--gpu", NULL);
		} else
		{
			ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p", di->d_proto->sw_name, "-e", backend, NULL);
		}
	}
	
	
#ifdef DEBUG 
	DPRINT("Starting dbgsrv_alps with argv: \n");
	tmp = ap;
	while (*tmp != (char *)NULL)
	{
		DPRINT("%s ", *tmp++);
	}
	DPRINT("\n");
	
	// set dbg to 1 if debug is on
	dbg = 1;
#endif /* DEBUG */

	// launch dbgsrv_alps onto the compute nodes

	DPERFLOG(SetTimerStart("mrnet backend attach"));

	// For the attach case, we might need to send over the current value of 
	// GUARD_MRNET_STARTUP_TIMEOUT in case it isn't set in the environment of
	// the application. Note that we want the environment inside the cray tool
	// interface to be set to this value.
	if ((env_var_str = getenv(GUARD_MRNET_STARTUP_TIMEOUT)) != NULL)
	{
		Asprintf(&env_vars[env_var_cnt++], "%s=%s", CTI_ATTRIBS_TIMEOUT_ENV_VAR, env_var_str);
	}
	
	// ditto for GUARD_ADDITIONAL_SLEEP, we want to use the actual env var that
	// inside the cray tool interface again.
	if ((env_var_str = getenv(GUARD_ADDITIONAL_SLEEP)) != NULL)
	{
		Asprintf(&env_vars[env_var_cnt++], "%s=%s", CTI_EXTRA_SLEEP_ENV_VAR, env_var_str);
	}
	
	// We should set the DBG_LOG_ENV_VAR only if debug flag is true
	if (dbg)
	{
		if ((env_var_str = getenv(DBG_LOG_ENV_VAR)) != NULL)
		{
			Asprintf(&env_vars[env_var_cnt++], "%s=%s", DBG_LOG_ENV_VAR, env_var_str);
		}
	}
	
	// Setup PYTHONHOME
	char * beDirStr = cti_getSessionRootDir(sid);
	if (beDirStr == NULL)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to launch dbgsrv_alps onto compute nodes!\n");
		FreeArgv(ap);
		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		
		return 1;
	}
	Asprintf(&env_vars[env_var_cnt++], "PYTHONHOME=%s", beDirStr);
	Free(beDirStr);
	
	if ((sid = cti_execToolDaemon(di->d_appid, mid, sid, "dbgsrv_alps", ap, env_vars, dbg)) == 0)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to launch dbgsrv_alps onto compute nodes!\n");
		FreeArgv(ap);
		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		
		return 1;
	}
	
	// done with the argv array and env_vars
	FreeArgv(ap);
	for (i=0; i < env_var_cnt; ++i)
	{
		Free(env_vars[i]);
	}

	// wait for the debug servers to connect to the mrnet tree
	rtn = mrn_fe_attach_be(network, di->d_nproc, mrnet_timeout);
	if (rtn)
	{
		if (rtn > 0)
		{
			PrintOutError("ERROR: DbgMRNAttach failed to attach dbgsrvs before timeout!\n");
			mrn_fe_debug_print_network_topology(network);
			mrn_fe_delete_network(network);
			// clear the network pointer
			di->d_proto_net = NULL;
		} else
		{
			PrintOutError("ERROR: DbgMRNAttach had null network information!\n");
		}
		
		return 1;
	}
	
	DPERFLOG(SetTimerStopAndLog("mrnet backend attach"));
	
	DPRINT("Debug server launch completed.\n");
	DPRINT("MRNet is going to create communication streams for the debugging network...\n");
	
	if (mrn_fe_create_streams_and_load_filter(network, AGGREGATION_FILTER, AGGREGATION_FUNC))
	{
		PrintOutError("ERROR: DbgMRNAttach failed to load filter functions and create streams for MRNet!\n");
		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		
		return 1;
	}
	
	DPRINT("Debugging network startup completed.\n");
	PrintOutMsg("Finalizing setup...\n");
	
	mrn_fe_enable_failure_callback(network, di);
	
	if ((num_be = mrn_fe_get_num_of_be(network)) != di->d_nproc)
	{
		PrintOutError("ERROR: MRNet has an incorrect number of debug servers.\n");
		PrintOutError("\tExpected [%d] found [%d]\n", di->d_nproc, num_be);

		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		
		return 1;
	}
	
	DPRINT("num_be is %d\n", num_be);

	di->d_bitset_full = bitset_new(num_be);
	bitset_invert(di->d_bitset_full);

	di->d_bitset_launched = bitset_new(num_be);

	di->d_proto_net = network;

	DPRINT("DbgMRNAttach: MRNet initialization succeded! network:[%p]\n", network);
	
	// create the d_evfds list for the di
	di->d_evfds = NewList();
	
	// setup the event notification fd for the io stream
	event_fd = mrn_fe_get_io_notificationfd(network);
	
	// create an int to hold the io stream fd. Note that this will get free'ed
	// later on when the dbg invoke gets cleaned up.
	ev_fd_ptr = (int *)Alloc(sizeof(int));
	*ev_fd_ptr = event_fd;
	
	// Add the io_stream fd to the d_evfds list
	// This allows DbgCheckEvents to select on MRNet protocol events
	AddToList(di->d_evfds, (void *)ev_fd_ptr);
	
	// setup the event notification fd for the event stream
	event_fd = mrn_fe_get_event_notificationfd(network);
	
	// create an int to hold the event stream fd.
	ev_fd_ptr = (int *)Alloc(sizeof(int));
	*ev_fd_ptr = event_fd;
	
	// Add the event_stream fd to the d_evfds list
	// This allows DbgCheckEvents to select on MRNet protocol events
	AddToList(di->d_evfds, (void *)ev_fd_ptr);

	/*broadcast an INVOKE command to complete the attach callback*/
	request = CommandToSocProto(DBGCMD_INVOKE, num_be);

	if(mrn_fe_setup_streams(network, request))
	{
		PrintOutError("ERROR: MRNet cannot setup broadcast channels with dbgsrvs!\n");
		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		
		return 1;
	}
	
	// wait for the DBG_OK response from the dbgsrv's indicating they have finished
	// starting up
	
	DPRINT("MRNet setup finalized.\n");
	DPRINT("Waiting for dbgsrv callbacks...\n");

	// final cleanup
	Free(request);

	DPERFLOG(SetTimerStopAndLog("Attach total"));
	DPRINT("DbgMRNAttach completed!\n");
	
	// Here we want to wait for the dbginvoke to start as long as the callback
	// is still valid
	while ( di->d_started == 0 )
	{
		if (WaitForProtoStartupEvent(di) == -1)
		{
			PrintOutError("Could not receive launch callbacks!\n");
			mrn_fe_delete_network(network);
			// clear the network pointer
			di->d_proto_net = NULL;
			
			return 1;
		}
	}
	
	// cleanup wrapper_name
	remove(wrapper_name);
	Free(wrapper_name);
	wrapper_name = NULL;
	
	// All done. Everything is ready to go at this point.

	return 0;
}

/*
** Compatible callback routine for attaching with mrnet.
*/
static int
cbMRNAttach(dbginvoke_t *di, proto_msg_set *set)
{
	proto_message *	msg;
	dbgevent_t * 	e = NULL;
	int 			err = 0;

	if(set == NULL)
		return 0;

	while ((msg = NextProtoMsgInSet(set)) != NULL)
	{
		// convert this protocol msg string to an event
		e = SocProtoToEvent(msg->message);

		bitset_oreq(di->d_bitset_launched, msg->group);

		if(e->ev_event != DBGEV_OK)
		{
			err = 1;
			PrintOutError("Received unknown event %s.\n", DbgevToStr(e->ev_event));
		}
		
		// Done with the event, free it
		FreeEvent(e);
	}

	if (err)
		return 1;

	DPRINT("cbMRNAttach received responses [%s]!\n", bitset_to_str(di->d_bitset_launched));

	// Check if we have received a message from every rank
	if(bitset_eq(di->d_bitset_launched, di->d_bitset_full))
	{
		// Reset the protocol event callback to the dbgclient default
		di->d_proto_ev_callback = DbgDispatchProtoEvents;
		
		// Set the started bit in the di
		di->d_started = 1;
		
		DPERFLOG(SetTimerStopAndLog("stream_init"));
		
		DPRINT("dbgsrv callback complete.\n");
		DPRINT("Releasing application from the startup barrier...\n");

		PrintOutMsg("Attach complete.\n");
	}

	return 0;
}

/*
** Invoke user program and build an MRNet tree.
*/
static int
DbgMRNInvoke(
	dbginvoke_t *di,
	proto_session *sess,
	char *prog,
	char **argv,
	char *arch,
	char **arch_args,
	char *arch_input,
	char *backend,
	char **env,
	char *cwd,
	int gpu_flag,
	int debug_flag
)
{
	char				sn[80];
	char *				root;
	char *				cfg_dir;
	char				topology_name[MAXPATHLEN];
	char				connections[MAXPATHLEN];
	int					num_be;
	int					rtn;
	MRNet_Topology *	mrn_topology;
	mrnet_type *		network;
	char *				request;
	char **				ap;
	char **				tmp;
	int 				event_fd;
	int *				ev_fd_ptr;
	g_progoutp_t *		po = NULL;
	int					prog_outp = -1;
	unsigned long int	mrnet_timeout = GUARD_DEFAULT_TIMEOUT;
	cti_aprunProc_t *	aprun_info;
	char *				env_var_str;
	int					dbg=debug_flag;
	char **				machine_hosts;
	cti_manifest_id_t	mid;
	cti_session_id_t	sid;
	int					redirectInput = 0;
	char * 				env_lib_dir;
	char *				python_lib_dir;
	char *				gdb_lib_dir;

#ifdef DEBUG
	dbg = 1;
#endif

	// ensure the dbginvoke_t object was passed down
	if (di == NULL)
		return 1;

	// Set the initial protocol event callback to use our custom event handler
	// for launching
	di->d_proto_ev_callback = cbMRNInvoke;

	// get mrnet configuration information
	if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == (char *)NULL)
	{
		PrintOutError("ERROR: Cannot find %s. Please set MRNet environment variables!\n", GUARD_CFG_DIRECTORY);
		return 1;
	}
	snprintf(topology_name, MAXPATHLEN-1, "%s/%s", cfg_dir, MRNET_TOPOLOGY);
	snprintf(connections,	MAXPATHLEN-1, "%s/%s", cfg_dir, MRNET_ATTACH_CONNECTIONS);

	/*
	** There is a race between returning a pid and
	** receiving a SIGCHLD if the new process exits
	** very quickly (such as if the executable can't
	** be found). We need to hold the signal until we've
	** made sure we've got the pid.
	*/
	sighold(SIGCHLD);

	/*
	** using the tools interface, we can interact with the compute nodes directly
	** this means that there is no longer any need for a starter application on the
	** front end.
	*/
	PrintOutMsg("Starting alps application, please wait...\n");
	
	// if PERFLOG is turned on, we need to create a string to the location to 
	// create the log file.
#ifdef PERFLOG
	char *envDir;
	char logfile[PATH_MAX];
	
	if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)NULL)
	{
		// user didn't set the environment variable, so lets
		// read the HOME variable
		if ((envDir = getenv("HOME")) == (char *)NULL)
		{
			// couldn't read HOME, so lets default to tmp
			envDir = "/tmp";
		}
	}
	
	// create the fullpath string to the log file using PATH_MAX plus a null term
	snprintf(logfile, PATH_MAX+1, "%s/mrnet_perf_frontend.log", envDir);
	
	DPERFLOG(CreateTimerLog(logfile));
	DPERFLOG(SetLogInfo("Invoking %d processes, GPU is %s\n", di->d_nproc, gpu_flag ? "on":"off"));
	DPERFLOG(SetTimerStart("Invoke total"));
#endif /* PERFLOG */
	
	// create the argv for aprun note that here argv[0] should not be the program name
	// this is to address the fact that guard should be ignorant of the starter applications
	// real name.
	snprintf(sn, sizeof(sn), "%d", di->d_nproc);
#ifdef DEBUG
	ap = NewArgv("-n", sn, NULL);
#else
	// we want aprun to be silent when not in debug mode.
	ap = NewArgv("-q", "-n", sn, NULL);
#endif /* DEBUG */

	// add the launcher args if present
	if (arch_args != NULL)
	{
		tmp = &arch_args[0];
		while (*tmp != (char *)NULL)
		{
			ap = AppendArgs(ap, *tmp++, NULL);
		}
	}
	
	ap = AppendArgs(ap, prog, NULL); // and the prog 
	ap = AppendArgv(ap, argv); // plus its args

#ifdef DEBUG
	DPRINT("aprun args: ");
	tmp = ap;
	while (*tmp != (char *)NULL)
	{
		DPRINT("%s ", *tmp++);
	}
	DPRINT("\n");
#endif /* DEBUG */

	// create the output stream to redirect stdout/stderr to
	// Note that this will be a g_progoutp_t in this proto definition
	if ((di->d_proto_outp = DbgCreateGenericProgOutp()) != NULL)
	{
		po = (g_progoutp_t *)di->d_proto_outp;
		prog_outp = po->p_o_wfd;
		// Set the protocol program output stream callback
		di->d_proto_outp_callback = DbgDispatchProtoOutput;
	}
	
	// check to see if arch_input is null, if not we need to try and redirect
	// aprun stdin to it.
	if ( arch_input != (char *)NULL )
	{
		redirectInput = 1;
	}
	
	// Try to read the GUARD_MRNET_STARTUP_TIMEOUT and set the appropriate 
	// CTI environment variables for the application.
	if ((env_var_str = getenv(GUARD_MRNET_STARTUP_TIMEOUT)) != NULL)
	{
		mrnet_timeout = strtoul(env_var_str, NULL, 10);
		// set the pmi_attribs parser timeout to the same value as mrnet timeout.
		setenv(CTI_ATTRIBS_TIMEOUT_ENV_VAR, env_var_str, 1);
	}
	
	// ditto for GUARD_ADDITIONAL_SLEEP, we want to use the actual env var that
	// inside the cray tool interface again.
	if ((env_var_str = getenv(GUARD_ADDITIONAL_SLEEP)) != NULL)
	{
		setenv(CTI_ATTRIBS_TIMEOUT_ENV_VAR, env_var_str, 1);
	}
	
	DPERFLOG(SetTimerStart("aprun launch"));

	// launch aprun using the tool interface
	// Note that the prog_outp is the WRITE END OF THE PIPE!!!!
	if ((di->d_appid = cti_launchAppBarrier(ap, 1, redirectInput, prog_outp, prog_outp, arch_input, cwd, env)) == 0)
	{
		int 	n=0, r;
		char *	read_buf;
		
		PrintOutError("Aprun launch failed.\n");
		
		// Check to see if we can read from the p_o_rfd fd. If we can, lets
		// print each line out as an error since it might contain useful
		// information for the user.
		if (ioctl(po->p_o_rfd, FIONREAD, &n) == 0)
		{
			if (n != 0)
			{
				// There is stuff for us to read.
				read_buf = (char *)Alloc(n+1);
				memset(read_buf, '\0', n+1);
				r = read(po->p_o_rfd, read_buf, n);
				if (r > 0)
				{
					// TODO: Make the GUI parse for this since it is important.
					PrintOutError("\nThe following error was returned from aprun:\n%s\n", read_buf);
				} else
				{
					PrintOutError("CTI error: %s\n", cti_error_str());
				}
				Free(read_buf);
			} else
			{
				PrintOutError("CTI error: %s\n", cti_error_str());
			}
		} else
		{
			PrintOutError("CTI error: %s\n", cti_error_str());
		}
	
		FreeArgv(ap);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	DPERFLOG(SetTimerStopAndLog("aprun launch"));
	
	// done with this ap
	FreeArgv(ap);
	
	// ship over the binaries to the compute nodes
	DPERFLOG(SetTimerStart("transfer binaries"));
	
	// Create a new manifest to hold the stuff we want to ship
	if ((mid = cti_createNewManifest(0)) == 0)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to create a cti_manifest_id_t.\n");
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	// first add gdb-guard to manifest
	if (cti_addManifestBinary(mid, "gdb-guard"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add gdb-guard to manifest.\n");
		cti_destroyManifest(mid);
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	// This checks the setting of gpu_flag to figure out if cuda-gdb should be
	// sent over
	if (gpu_flag)
	{
		// The backend code will determine which version of cuda-gdb to use
		if (cti_addManifestBinary(mid, "cuda50-gdb-guard"))
		{
			PrintOutError("CTI error: %s\n", cti_error_str());
			PrintOutError("Failed to add cuda50-gdb-guard to manifest.\n");
			cti_destroyManifest(mid);
			cti_killApp(di->d_appid, SIGKILL);
			sigrelse(SIGCHLD);
			
			return 1;
		}
		if (cti_addManifestBinary(mid, "cuda55-gdb-guard"))
		{
			PrintOutError("CTI error: %s\n", cti_error_str());
			PrintOutError("Failed to add cuda55-gdb-guard to manifest.\n");
			cti_destroyManifest(mid);
			cti_killApp(di->d_appid, SIGKILL);
			sigrelse(SIGCHLD);
			
			return 1;
		}
	}

	// add the dbgsrv
	if (cti_addManifestBinary(mid, "dbgsrv"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add dbgsrv to manifest.\n");
		cti_destroyManifest(mid);
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	// finally add the mrnet_commnode
	if (cti_addManifestBinary(mid, "mrnet_commnode"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add mrnet_commnode to manifest.\n");
		cti_destroyManifest(mid);
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	// Get the libdir env var
	if ((env_lib_dir = getenv(GUARD_LIB_DIRECTORY)) == NULL)
	{
		PrintOutError("Failed to getenv on %s. Please check your environment setup.\n", GUARD_LIB_DIRECTORY);
		cti_destroyManifest(mid);
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	// Add the python modules
	Asprintf(&python_lib_dir, "%s/python2.6", env_lib_dir);
	if (cti_addManifestLibDir(mid, python_lib_dir))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add python2.6 to manifest.\n");
		cti_destroyManifest(mid);
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	Free(python_lib_dir);
	
	// Now add the gdb pretty printers
	Asprintf(&gdb_lib_dir, "%s/gdb", env_lib_dir);
	if (cti_addManifestLibDir(mid, gdb_lib_dir))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add gdb pretty printers to manifest.\n");
		cti_destroyManifest(mid);
		
		return 1;
	}
	Free(gdb_lib_dir);
	
#if 1	// RWM
	// send over the uda plugin - only if gpu mode is currently turned off
	if (!gpu_flag)
	{
		if (cti_addManifestLibrary(mid, "uda-plugin-cray-upc.so"))
		{
			PrintOutError("CTI error: %s\n", cti_error_str());
			PrintOutError("Failed to add uda-plugin-cray-upc.so to manifest.\n");
			cti_destroyManifest(mid);
			cti_killApp(di->d_appid, SIGKILL);
			sigrelse(SIGCHLD);
		
			return 1;
		}
	}
// End upc addition
#endif

#if 0	// use with strace for libmi
	if (cti_addManifestBinary(mid, "strace"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add strace to manifest.\n");
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
#endif

	// Add libthread_db otherwise gdb will be messed up with thread
	// support
	if (cti_addManifestLibrary(mid, "libthread_db.so.1"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add libthread_db.so.1 to manifest.\n");
		cti_destroyManifest(mid);
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
#ifdef BE_VALGRIND
	// add valgrind
	if (cti_addManifestBinary(mid, "valgrind"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add valgrind to manifest.\n");
		cti_destroyManifest(mid);
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
#endif
	
	// add the aggregation filter
	if (cti_addManifestLibrary(mid, AGGREGATION_FILTER))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add %s to manifest.\n", AGGREGATION_FILTER);
		cti_destroyManifest(mid);
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	// Stage this manifest on the compute node, we need to remember the sid for
	// later.
	if ((sid = cti_sendManifest(di->d_appid, mid, dbg)) == 0)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to ship the manifest!\n");
		cti_destroyManifest(mid);
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	DPERFLOG(SetTimerStopAndLog("transfer binaries"));
	
	DPRINT("Aprun session initialized.\n");
	PrintOutMsg("Creating MRNet communication network...\n"); 
	
	DPRINT("MRNet is creating a tree that the dbgsrvs can attach to.\n");
	
	// create the root hostname for mrnet
	if ((root = cti_getHostname()) == NULL)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to retrieve node hostname.\n");
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	DPRINT("Application has apid[%d]\n", (int)di->d_appid);
	DPRINT("Root has a hostname of [%s]\n", root);
	
	// get a list of the nid hostnames for topology creation
	if ((machine_hosts = cti_getAppHostsList(di->d_appid)) == (char **)NULL)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to retrieve nid hostname list.\n");
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
#ifdef DEBUG
	DPRINT("Application has the following nid hostnames list:\n");
	tmp = machine_hosts;
	while (*tmp != (char *)NULL)
	{
		DPRINT("%s ", *tmp++);
	}
	DPRINT("\n");
#endif /* DEBUG */

	// setup a mrnet_commnode_wrapper based on this sessions staging location
	if (create_mrnet_commnode_wrapper(sid))
	{
		PrintOutError("ERROR: DbgMRNAttach failed to create MRNet topology file!\n"); 
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}

	// create the topology file used to create the mrnet front end network
	if(create_mrnet_topology(di->d_nproc, root, machine_hosts, 1))
	{
		PrintOutError("ERROR: DbgMRNInvoke failed to create MRNet topology file!\n"); 
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	// get the apid for MRNet
	if ((aprun_info = cti_getAprunInfo(di->d_appid)) == NULL)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("ERROR: DbgMRNInvoke failed to query the apid!\n"); 
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}

	DPRINT("MRNet is going to create a tree with topology[%s]\n", topology_name);
	
	DPERFLOG(SetTimerStart("Create mrnet network"));
	
	network = mrn_fe_create_network_no_be(topology_name, &mrn_topology, aprun_info->apid);
	
	Free(aprun_info);
	
	DPERFLOG(SetTimerStopAndLog("Create mrnet network"));
	
	// create the backend attach file which the compute nodes will use to hook up to the network
	if (create_parents_info(mrn_topology, di->d_nproc) < 1)
	{
		PrintOutError("ERROR: DbgMRNInvoke cannot create infomation for MRNet attachment!\n");
		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	// Create a new manifest to hold the stuff we want to ship based on the existing sid
	if ((mid = cti_createNewManifest(sid)) == 0)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to create a cti_manifest_id_t.\n");
		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	// Add the be_attach.dat file to the manifest
	if (cti_addManifestFile(mid, connections))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to transfer backend attach information.\n");
		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	DPRINT("MRNet network initialized.\n");
	DPRINT("Launching debug servers onto compute nodes.\n");
	
	// Transfer and execute dbgsrv_alps with call line info:
	// dbgsrv_alps -h <cbhost> -c <cbport> -a <arch> -p <proto> -e <backend> prog
	// first create the argv for dbgsrv_alps
	// This now also depends on gpu_flag
	if (dbg)
	{
		if (gpu_flag)
		{
			ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p", di->d_proto->sw_name, "-e", backend, "--gpu", "--debug", "--", prog, NULL);
		} else
		{
			ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p", di->d_proto->sw_name, "-e", backend, "--debug", "--", prog, NULL);
		}
	} else
	{
		if (gpu_flag)
		{
			ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p", di->d_proto->sw_name, "-e", backend, "--gpu", "--", prog, NULL);
		} else
		{
			ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p", di->d_proto->sw_name, "-e", backend, "--", prog, NULL);
		}
	}
	
#ifdef DEBUG 
	DPRINT("Starting dbgsrv_alps with argv: \n");
	tmp = ap;
	while (*tmp != (char *)NULL)
	{
		DPRINT("%s ", *tmp++);
	}
	DPRINT("\n");
	
	// set dbg to 1 if debug is on
	dbg = 1;
#endif /* DEBUG */

	// launch dbgsrv_alps onto the compute nodes

	DPERFLOG(SetTimerStart("mrnet backend attach"));

	// Setup PYTHONHOME
	char ** args = malloc(2*sizeof(char*));
	char * beDirStr = cti_getSessionRootDir(sid);
	if (beDirStr == NULL)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to launch dbgsrv_alps onto compute nodes!\n");
		FreeArgv(ap);
		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	Asprintf(&args[0], "PYTHONHOME=%s", beDirStr);
	Free(beDirStr);
	args[1] = (char *)NULL;
	
	if ((sid = cti_execToolDaemon(di->d_appid, mid, sid, "dbgsrv_alps", ap, args, dbg)) == 0)
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to launch dbgsrv_alps onto compute nodes!\n");
		FreeArgv(ap);
		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	free(args[0]);
	free(args);
	
	// done with the argv array
	FreeArgv(ap);

	// wait for the debug servers to connect to the mrnet tree
	rtn = mrn_fe_attach_be(network, di->d_nproc, mrnet_timeout);
	if (rtn)
	{
		if (rtn > 0)
		{
			PrintOutError("ERROR: DbgMRNInvoke failed to attach dbgsrvs before timeout!\n");
			mrn_fe_debug_print_network_topology(network);
			mrn_fe_delete_network(network);
			// clear the network pointer
			di->d_proto_net = NULL;
		} else
		{
			PrintOutError("ERROR: DbgMRNInvoke had null network information!\n");
		}
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	DPERFLOG(SetTimerStopAndLog("mrnet backend attach"));
	
	DPRINT("Debug server launch completed.\n");
	DPRINT("MRNet is going to create communication streams for the debugging network...\n");
	
	if (mrn_fe_create_streams_and_load_filter(network, AGGREGATION_FILTER, AGGREGATION_FUNC))
	{
		PrintOutError("ERROR: DbgMRNInvoke failed to load filter functions and create streams for MRNet!\n");
		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	DPRINT("Debugging network startup completed.\n");
	PrintOutMsg("Finalizing setup...\n");
	
	mrn_fe_enable_failure_callback(network, di);
	
	if ((num_be = mrn_fe_get_num_of_be(network)) != di->d_nproc)
	{
		PrintOutError("ERROR: MRNet has an incorrect number of debug servers.\n");
		PrintOutError("\tExpected [%d] found [%d]\n", di->d_nproc, num_be);

		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	DPRINT("num_be is %d\n", num_be);

	// Create the full bitset based on the number of ranks
	di->d_bitset_full = bitset_new(num_be);
	bitset_invert(di->d_bitset_full);

	// Create the empty launched bitset
	di->d_bitset_launched = bitset_new(num_be);
	
	// Set the network object in the dbginvoke
	di->d_proto_net = network;

	DPRINT("DbgMRNInvoke: MRNet initialization succeded! network:[%p]\n", network);
	
	// create the d_evfds list for the di
	di->d_evfds = NewList();
	
	// setup the event notification fd for the io stream
	event_fd = mrn_fe_get_io_notificationfd(network);
	
	// create an int to hold the io stream fd. Note that this will get free'ed
	// later on when the dbg invoke gets cleaned up.
	ev_fd_ptr = (int *)Alloc(sizeof(int));
	*ev_fd_ptr = event_fd;
	
	// Add the io_stream fd to the d_evfds list
	// This allows DbgCheckEvents to select on MRNet protocol events
	AddToList(di->d_evfds, (void *)ev_fd_ptr);
	
	// setup the event notification fd for the event stream
	event_fd = mrn_fe_get_event_notificationfd(network);
	
	// create an int to hold the event stream fd.
	ev_fd_ptr = (int *)Alloc(sizeof(int));
	*ev_fd_ptr = event_fd;
	
	// Add the event_stream fd to the d_evfds list
	// This allows DbgCheckEvents to select on MRNet protocol events
	AddToList(di->d_evfds, (void *)ev_fd_ptr);
	
	// setup the program output stream handler if we are handling output
	if (po != NULL)
	{
		ev_fd_ptr = (int *)Alloc(sizeof(int));
		*ev_fd_ptr = po->p_o_rfd;
		
		// Add the p_o_rfd to the d_evfds list
		// This allows DbgCheckEvents to select on application output events
		AddToList(di->d_evfds, (void *)ev_fd_ptr);
	}

	/*broadcast an INVOKE command to complete the invoke callback*/
	request = CommandToSocProto(DBGCMD_INVOKE, num_be);

	if(mrn_fe_setup_streams(network, request))
	{
		PrintOutError("ERROR: MRNet cannot setup broadcast channels with dbgsrvs!\n");
		mrn_fe_delete_network(network);
		// clear the network pointer
		di->d_proto_net = NULL;
		cti_killApp(di->d_appid, SIGKILL);
		sigrelse(SIGCHLD);
		
		return 1;
	}
	
	// wait for the DBG_OK response from the dbgsrv's indicating they have finished
	// starting up
	
	DPRINT("MRNet setup finalized.\n");
	DPRINT("Waiting for dbgsrv callbacks...\n");

	// final cleanup
	Free(request);
	sigrelse(SIGCHLD);

	DPERFLOG(SetTimerStopAndLog("Invoke total"));
	DPRINT("DbgMRNInvoke completed!\n");
	
	// Here we want to wait for the dbginvoke to start as long as the callback
	// is still valid
	while ( di->d_started == 0 )
	{
		if (WaitForProtoStartupEvent(di) == -1)
		{
			PrintOutError("Could not receive launch callbacks!\n");
			mrn_fe_delete_network(network);
			// clear the network pointer
			di->d_proto_net = NULL;
			cti_killApp(di->d_appid, SIGKILL);
			
			return 1;
		}
	}
	
	// cleanup wrapper_name
	remove(wrapper_name);
	Free(wrapper_name);
	wrapper_name = NULL;
	
	// All done. Everything is ready to go at this point.

	return 0;
}

/*
** Compatible callback routine for invoking with mrnet.
*/
static int
cbMRNInvoke(dbginvoke_t *di, proto_msg_set *set)
{
	proto_message *	msg;
	dbgevent_t * 	e = NULL;
	int 			err = 0;

	if(set == NULL)
		return 0;

	while ((msg = NextProtoMsgInSet(set)) != NULL)
	{
		// convert this protocol msg string to an event
		e = SocProtoToEvent(msg->message);

		bitset_oreq(di->d_bitset_launched, msg->group);

		if(e->ev_event != DBGEV_OK)
		{
			err = 1;
			PrintOutError("Received unknown event %s.\n", DbgevToStr(e->ev_event));
		}
		
		// Done with the event, free it
		FreeEvent(e);
	}
	
	if (err)
		return 1;

	DPRINT("cbMRNInvoke received responses [%s]!\n", bitset_to_str(di->d_bitset_launched));

	// Check if we have received a message from every rank
	if(bitset_eq(di->d_bitset_launched, di->d_bitset_full))
	{
		// Reset the protocol event callback to the dbgclient default
		di->d_proto_ev_callback = DbgDispatchProtoEvents;
		
		// Set the started bit in the di
		di->d_started = 1;
		
		DPERFLOG(SetTimerStopAndLog("stream_init"));
			
		DPRINT("dbgsrv callback complete.\n");
		DPRINT("Releasing application from the startup barrier...\n");
			
#ifdef ALPS
		// tell aprun to release the application from the startup barrier
		if (cti_releaseAppBarrier(di->d_appid))
		{
			PrintOutError("CTI error: %s\n", cti_error_str());
			PrintOutError("Could not release application from the startup barrier.\n");
			cti_killApp(di->d_appid, SIGKILL);
			return 1;
		}
#endif

		PrintOutMsg("Launch complete.\n");
	}

	return 0;
}

/*
**********************************************************
**********************************************************
************ WRAPPERS FOR MRNet ROUTINES *****************
**********************************************************
**********************************************************
*/

static void
DbgMRNDispatchEvents(dbginvoke_t *di, int ev_fd)
{
	mrnet_type *		network = (mrnet_type *)di->d_proto_net;
	g_progoutp_t *		po = (g_progoutp_t *)di->d_proto_outp;
	proto_msg_set *		io_res;
	proto_msg_set *		ev_res;
	proto_message *		msg;
	char *				set_str;
	
	// dispatch application output
	if ((po != NULL) && (po->p_o_rfd == ev_fd))
	{
		// Call the protocol specific callback function to handle the output on the stream
		(*(di->d_proto_outp_callback))(di, po->p_o_stream);
		return;
	}
	
	// dispatch io
	// FIXME: This isn't used on a Cray, but if it is this should be calling
	// a callback registered in the dbginvoke instead of handling it here. It's
	// not appropriate to handle client events in this layer.
	if ((io_res = mrn_fe_dispatch_io(network, ev_fd)) != NULL)
	{
		// process each io message in the list
		while ((msg = NextProtoMsgInSet(io_res)) != NULL)
		{
			set_str = bitset_to_set(msg->group);
			PrintOutMsg("IO from %s: %s\n", set_str, msg->message);
			Free(set_str);
		}
		
		FreeProtoMsgSet(io_res);
		return;
	}
	
	// dispatch events
	if ((ev_res = mrn_fe_dispatch_event(network, ev_fd)) != NULL)
	{
		// Call the protocol specific callback function to handle this event msg
		(*(di->d_proto_ev_callback))(di, ev_res);
		
		// Free the message set
		FreeProtoMsgSet(ev_res);
		return;
	}
}

static bitset *
DbgMRNSendCmd(dbginvoke_t *di, bitset *target, char *request)
{
	proto_network	network = di->d_proto_net;
	bitset *		received = NULL;
	
	// send command only if not already in a shutdown state
	if (di->d_isShutdown)
	{
		return NULL;
	}
	
	mrn_fe_send_command((mrnet_type *)network, target, request);

	received = bitset_new(bitset_size(target));
	
	DPRINT("INFO: Sending cmd to set[%s]\n", bitset_to_set(target));

	return received;
}

// Here received means the bitset that represents the ranks we sent the initial
// command to. Change this to something that makes more sense.
static proto_msg_set *
DbgMRNRecvReply(dbginvoke_t *di, bitset *target, bitset *received)
{
	proto_network		network = di->d_proto_net;
	int					full = 0;
	proto_msg_set *		msg_set = NULL;
	proto_message *		msg;
	proto_msg_set *		result = NULL;
	
	// recv reply only if not already in a shutdown state
	if (di->d_isShutdown)
	{
		return NULL;
	}

	while(!full)
	{
		if ((msg_set = mrn_fe_recv_reply((mrnet_type *)network, target)) == NULL)
		{
			PrintOutError("Could not recv reply from MRNet network.\n");
			ResetProtoMsgSet(result);
			return result;
		}
		
		// If this is the first message set, then use this as our return type,
		// otherwise merge the set with our return type.
		if (result == NULL)
		{
			result = msg_set;
			// process the msg_set to get the received set
			while ((msg = NextProtoMsgInSet(msg_set)) != NULL)
			{
				// add this group to the received group
				bitset_oreq(received, msg->group);
			}
		} else
		{
			// add each message to the result
			while ((msg = RemoveProtoMsgInSet(msg_set)) != NULL)
			{
				// add this group to the received group
				bitset_oreq(received, msg->group);
				AddProtoMsgSet(result, msg);
			}
		}
		
		DPRINT("INFO: Recieved replies from set[%s]\n", bitset_to_str(received));
		
		// Check to see if we have received all responses
		full = bitset_eq(target, received);
	}
	
	ResetProtoMsgSet(result);
	
	return result;
}

/*
** DBG_EXIT: Tell server to quit immediately.
** If detach is true, then we detach from application instead of killing it.
*/
static int
DbgMRNExit(dbginvoke_t *di, int detach)
{
	mrnet_type *	network = (mrnet_type *)di->d_proto_net;
	g_progoutp_t *	po = (g_progoutp_t *)di->d_proto_outp;

	// shutdown mrnet only if not already in a shutdown state
	if (!di->d_isShutdown)
	{
		mrn_fe_delete_network(network);
	}
	
	// clear the network pointer
	di->d_proto_net = NULL;
	
#ifdef ALPS
	if (!detach)
	{
		// send a kill signal using apkill to the apid of the aprun pid
		if (cti_killApp(di->d_appid, SIGKILL))
		{
			PrintOutError("CTI error: %s\n", cti_error_str());
			PrintOutError("Failed to kill aprun process!\n");
			return -1;
		}
	}
#endif

	// Destroy the program output object
	DbgFreeGenericProgOutp(po);
	
	// clear the program output object
	di->d_proto_outp = NULL;

	return 0;
}

static void
DbgMRNDestroyClient(dbginvoke_t *di)
{
	mrnet_type *	network = (mrnet_type *)di->d_proto_net;	
	g_progoutp_t *	po = (g_progoutp_t *)di->d_proto_outp;
	
	// Check to see if the network type has been destroyed
	if (network != NULL)
	{
		// call the exit function to force the application to quit
		DbgMRNExit(di, 0);
	}
	
	// Check to see if the output stream has been destroyed
	if (po != NULL)
	{
		// Destroy the program output object
		DbgFreeGenericProgOutp(po);
	
		// clear the program output object
		di->d_proto_outp = NULL;
	}
}

