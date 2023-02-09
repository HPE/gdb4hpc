/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */
/*    modified 10/16/2006 by Jim Hoekstra - ISU
 *      changed test to follow mtest_init/mtest_finalize convention
 */
/*    modified 08/29/2008 by Jim Hoekstra - ISU
 *      changed test to add check on dDuration and errs++
 */
/*    modified 02/02/2010 by Jim Hoekstra - ISU
 *      changed test to add ifdef for check on dDuration
 */
/*    modified 5/10/2010 by Jim Hoekstra - ISU
 *      added #include "mpitest.h"
 */
#include "mpi.h"

#ifdef IS_LOCAL_MPI
int main(int argc, char *argv[])
{
    MPI_Init( &argc, &argv);
    MPI_Finalize();
    return 0;
}
#else

#include "mpitest.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	int i;
    int errs = 0;
	double dStart, dFinish, dDuration;

	MTest_Init( &argc, &argv );
	MPI_Comm_rank(MPI_COMM_WORLD, &i);

	dStart = MPI_Wtime();
	MTestSleep(1);
	dFinish = MPI_Wtime();
	dDuration = dFinish - dStart;

	printf(" start:%20.16e\nfinish:%20.16e\nduration:%g\n", dStart, dFinish, dDuration);
#ifdef __x86_64__
    if (( dDuration < .99 || dDuration > 1.01 )) {
#else
    if (( dDuration < .97 || dDuration > 1.03 )) {
#endif
        printf("Fail - dDuration = %g\n",dDuration);
        errs++;
    }

    MTest_Finalize( errs );
	MPI_Finalize();
	return 0;
}
#endif
