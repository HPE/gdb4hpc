// designed to test edge cases of the list command.

#include <stdio.h>
#include "mpi.h"
static void printIt();
int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

    printIt();

	MPI_Finalize();
}

static void printIt()
{
    for (int i=0; i<3; i++) {
		for (int j=0; j<i; j++) {
			int foo	=	j;
			foo		+=	i;
			// try to be as nasty as possible with backslashes...
			printf("\tfoo is: \"\t\t\\\t\t%d\n", foo);	// print foo \
				end comment
		}
	}
}

