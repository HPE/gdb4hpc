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

#include "defs.h"
#include "dbg.h"
#include "mrn_cfg.h"
#include "dbg_mrn.h"
#include "Topology_C.h"
#include "dbgsrv_defs.h"

#define ATTACH_FILE_LINE_LEN            30
#define NUM_TIMEOUT_RETRY               1000

/*
** This file describes functions that are used on the compute node to interface
** with the mrnet config file.
*/

int
ProcRankToMRNRank(int procRank)
{
	return (procRank + ATTACHBE_RANK_SKEW);
}

int
get_myparent_info(int id, char *host, char *par_host, unsigned *par_port, unsigned *par_rank)
{
	FILE *          conn;
	int             dbgsrv_id;
	char            connections[MAXPATHLEN];
	char            hostname[MAXPATHLEN];
	unsigned        port;
	unsigned        rank;
	int	        found = 0;
	int             retval;

#ifdef ALPS
	// our cwd is in the toolhelper dir
	snprintf(connections, MAXPATHLEN-1, "./%s", MRNET_ATTACH_CONNECTIONS);
	if ((conn = fopen(connections, "r")) == (FILE *)NULL)
	{
	        PrintOutError("ERROR: get_myparent_info cannot open %s!\n", connections);
	        return -1;
	}
	
	DVPRINT("Reading backend attach info from %s\n", connections);
#else
	int retry = 0;
	char* dir = getenv(GUARD_CFG_DIRECTORY);
        
	if(dir == NULL){
		PrintOutError("ERROR: get_myparent_info cannot find %s! Please set up MRNet configurations!", GUARD_CFG_DIRECTORY); 	
		return -1;
	}	
	snprintf(connections, MAXPATHLEN-1, "%s/%s", dir, MRNET_ATTACH_CONNECTIONS);
	
	conn = fopen(connections, "r");	

	while(conn == NULL)
	{
		if(retry > NUM_TIMEOUT_RETRY)
			break;

		PrintOutError("INFO: get_myparent_info cannot open %s!\n", connections);
		sleep(1);

		conn = fopen(connections, "r");	
		retry++;
	}
	
	if(conn == NULL)
	{
		PrintOutError("ERROR: get_myparent_info cannot open %s!\n", connections);
		return -1;
	}
#endif
	
	// advance the file stream to our current rank's position in the file
	// this will greatly reduce read times for large numbers of runs
#ifdef ALPS
	if (fseek(conn, id*ATTACH_FILE_LINE_LEN, SEEK_SET))
	{
	        perror("fseek");
	}

	do{
		retval = fscanf(conn, "%d %s %d %d", &dbgsrv_id, hostname, &port, &rank);
		DVPRINT("INFO: get_myparent_info read %d:%s:%d:%d\n", dbgsrv_id, hostname, port, rank);
		if(id == dbgsrv_id)
		{
			found = 1;
			break;
		}
	} while(retval != EOF);
#else
	do{
		retval = fscanf(conn, "%d %s %d %d\n", &dbgsrv_id, hostname, &port, &rank);
		DVPRINT("INFO: get_myparent_info read %d:%s:%d:%d\n", dbgsrv_id, hostname, port, rank);
		if(id == dbgsrv_id)
		{
			found = 1;
			break;
		}
	} while(retval != EOF);
#endif
	
	fclose(conn);

	if(found)
	{
		sprintf(par_host, "%s", hostname);	
		*par_port = port;
		*par_rank = rank;

		return 0;
	}

	return -1;
}

