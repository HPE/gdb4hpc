#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "defs.h"
#include "dbg_sci.h"
#include "dbg_sci_fe.h"
#include "sci_topgen.h"

#define NUM_TIMEOUT_RETRY 1000
//#define MAXPATHLEN 128

static int scicfg_files_created = 1;


int
get_apid(int *nid)
{
	int apid;
	FILE *file;
	char aprunid_name[MAXPATHLEN];
	char *cfg_dir = getenv(GUARD_SCI_CFG_DIRECTORY);

	if(cfg_dir == NULL)
	{
		fprintf(stderr, "ERROR: Cannot find %s! Please set up your SCI configurations!\n", GUARD_SCI_CFG_DIRECTORY);
		return -1;
	}

	snprintf(aprunid_name, MAXPATHLEN-1, "%s/%s", cfg_dir, ALPS_APRUNID_FILE);

	file = fopen(aprunid_name, "r");
	if(file == NULL)
	{
		fprintf(stderr, "ERROR: cannot open %s!\n", aprunid_name);
		return -1;
	}

	fscanf(file, "%d", &apid);
	fscanf(file, "%d", nid);
	fclose(file);

	return apid;
}


int
delete_file(char *file)
{
	struct stat file_stat;

	if(!stat(file, &file_stat))
	{
		if(remove(file))
		{
			fprintf(stderr, "failed to delete %s!\n", file);
			perror("Cannot delete file!");
			return -1;
		}
	}

	return 0;
}

/*
	I am called when a SIGUSR2 arrives from guard_starter_alps (via signal()). 
	This indicates that all the SCI configurations files have been created.
 */
static void
scicfg_callback(int sigNum)
{
/*TODO deal with the case that guard_sterter_alps crashes before sending signal */

  DPRINT("\nGuard received signal[%d]!\n", sigNum);
  scicfg_files_created = 1;
}

/* this is just for ALPS 
* we first delete all rubbish configuration files to prevent all possible confusability
* TODO: use IPC to make guard_starter_alps talk with dbgclnt_mrn
*/
int
prepare_scicfg_files()
{
	char aprunid_name[MAXPATHLEN];
	char machines_name[MAXPATHLEN];

	char *cfg_dir = getenv(GUARD_SCI_CFG_DIRECTORY);

	if(cfg_dir == NULL)
	{
		fprintf(stderr, "ERROR: Cannot find %s! Please set up your SCI configurations!\n", GUARD_SCI_CFG_DIRECTORY);
		return -1;
	}

	snprintf(machines_name, MAXPATHLEN-1, "%s/%s", cfg_dir, MACHINES_FILE);
	snprintf(aprunid_name, MAXPATHLEN-1, "%s/%s", cfg_dir, ALPS_APRUNID_FILE);

	if(delete_file(machines_name) || delete_file(aprunid_name))
		return -1;

	DPRINT("sci_topgen set handler for SIGUSR2!\n");
	signal(SIGUSR2, scicfg_callback);

	return 0;
}

/*
* confirm guard_starter_alps has created new configuration files
*/
int
check_scicfg_files()
{
	struct stat file_stat;
	char aprunid_name[MAXPATHLEN];
	char machines_name[MAXPATHLEN];

	char *cfg_dir = getenv(GUARD_SCI_CFG_DIRECTORY);

	if(cfg_dir == NULL)
	{
		fprintf(stderr, "ERROR: Cannot find %s! Please set up your SCI configurations!\n", GUARD_SCI_CFG_DIRECTORY);
		return -1;
	}

	snprintf(machines_name, MAXPATHLEN-1, "%s/%s", cfg_dir, MACHINES_FILE);
	snprintf(aprunid_name, MAXPATHLEN-1, "%s/%s", cfg_dir, ALPS_APRUNID_FILE);

	int counter = 0;
	while(stat(aprunid_name, &file_stat) || stat(machines_name, &file_stat))
	{
		fprintf(stderr, "INFO: %s or %s has not been created! errno of stat[%d]\n", machines_name, aprunid_name, errno);
		sleep(1);
		counter++;
	}

	DPRINT("check_scicfg_files waited %d seconds\n", counter);
	return 0;
}

int
create_placement_file(jobHostPlacement_t *jobPlacement)
{
	char *dir = NULL;
	char ap_name[PATH_MAX];
	char machines_name[PATH_MAX];
	char temp_name[PATH_MAX];
	FILE *ap = NULL;
	FILE *machines = NULL;

	int numHosts, index, i;
	hostPlacement_t* placement;

	if(jobPlacement == NULL)
		return -1;

	numHosts = jobPlacement->numHosts;
	placement = jobPlacement->hostPlacement;

	if(numHosts <= 0 || placement == NULL)
	{
		Free(jobPlacement);
		return -1;
	}

	dir = getenv(GUARD_SCI_CFG_DIRECTORY);
	if(dir == NULL)
	{
		fprintf(stderr, "ERROR: Cannot find GUARD_SCI_CFG_DIRECTORY\n");
	}

	//write ncList into machines.dat
	snprintf(machines_name, PATH_MAX-1, "%s/%s", dir, MACHINES_FILE);
	snprintf(temp_name, PATH_MAX-1, "%s/%s_", dir, MACHINES_FILE);
	machines = fopen(temp_name, "w");
	if(machines == NULL)
	{
		fprintf(stderr, "ERROR: create_placement_file Cannot open %s!\n", temp_name);
		exit(1);
	}	

	for(index = 0; index < numHosts; index++)
	{
		for(i = 0; i < placement[index].numProcs; i++)
			fprintf(machines, "%s\n", placement[index].hostname);
	}

	fclose(machines);

	if(rename(temp_name, machines_name))
	{
		fprintf(stderr, "ERROR: createMachinesFile cannot rename %s to %s\n", temp_name, machines_name);
		perror("rename failed!");

		return -1;
	}

	return 1;
}

void
destroyJobPlacement(jobHostPlacement_t* jobPlacement)
{
	int numHosts, index;
	hostPlacement_t* placement;

	if(jobPlacement == NULL)
		return;

	numHosts = jobPlacement->numHosts;
	placement = jobPlacement->hostPlacement;

	if(numHosts <= 0 || placement == NULL)
	{
		Free(jobPlacement);
		return;
	}

	for(index = 0; index < numHosts; index++)
	{
		Free(placement[index].hostname);
		placement[index].hostname = NULL;
	}

	jobPlacement->numHosts = 0;
	Free(jobPlacement->hostPlacement);
	Free(jobPlacement);
}
