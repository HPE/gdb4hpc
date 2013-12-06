/*
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
** See COPYRIGHT in top-level directory.
**
** Guard debugger
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>
#include <sys/wait.h>

#include	"aif.h"
#include	"defs.h"
#include	"unistd.h"
#include	"fds.h"

#ifdef OPENDX
void _visualize_opendx(AIF *);
#endif /* OPENDX */

void 
waitforchild(int sig)
{ 
	int stat;
	wait(&stat);
}

void 
GuardVisualization(AIF *a, char *type)
{
#ifdef OPENDX
	if (strcmp(type, "vis_opendx") == 0)
		_visualize_opendx(a);
#endif /* OPENDX */
}

#ifdef OPENDX
void
_visualize_opendx(AIF *a)
{
	int atype;
	int *loc;
	AIFIndex *ix;
	AIF *ax;
	int x, y, z;
	FILE *outfile;

	signal(SIGCHLD, waitforchild);

	switch (atype = AIFType(a))
	{
	case AIF_ARRAY:
		ix = AIFArrayIndexInit(a);
		loc = (int *) _aif_alloc(sizeof(int) * ix->i_rank);

		if (ix->i_rank == 1)
		{
			outfile = fopen("opendx_array.data", "w");
			for (x = ix->i_min[0]; x <= ix->i_max[0]; x++)
			{
				PrintToFd(outfile, "%d ", x);
				ax = AIFArrayRef(a, ix->i_rank, &x);
				AIFPrint(outfile, 0, ax);
				PrintToFd(outfile, "\n");
			}
		}
		else if (ix->i_rank == 2)
		{
			outfile = fopen("opendx_array.data", "w");
			for (x = ix->i_min[0]; x <= ix->i_max[0]; x++)
			{
				for (y = ix->i_min[1]; y <= ix->i_max[1]; y++)
				{
					PrintToFd(outfile, "%d %d ", x, y);
					loc[0] = x; 
					loc[1] = y;
					ax = AIFArrayRef(a, ix->i_rank, loc);
					AIFPrint(outfile, 0, ax);
					PrintToFd(outfile, "\n");
				}
			}
		}
		else if (ix->i_rank == 3)
		{
			outfile = fopen("opendx_array.data", "w");
			for (x = ix->i_min[0]; x <= ix->i_max[0]; x++)
			{
				for (y = ix->i_min[1]; y <= ix->i_max[1]; y++)
				{
					for (z = ix->i_min[2]; z <= ix->i_max[2]; z++)
					{
						PrintToFd(outfile, "%d %d %d ", x, y, z);
						loc[0] = x; 
						loc[1] = y;
						loc[2] = z;
						ax = AIFArrayRef(a, ix->i_rank, loc);
						AIFPrint(outfile, 0, ax);
						PrintToFd(outfile, "\n");
					}
				}
			}
		}
		else
		{
			printf("Error\n");
		}

		AIFArrayIndexFree(ix);
		_aif_free(loc);
		fclose(outfile);
	
		break;

	default:
		printf("Unknown\n");
	}



	switch (atype)
	{
	case AIF_ARRAY:

		if (fork() == 0)
		{
			(void) close(0);
			execl(OPENDX, "dx", "-program opendx_array.net -host localhost -image", (char *) 0);
			exit(1);
		}

		break;

	default:
		printf("Unknown\n");
	}
}
#endif /* OPENDX */
