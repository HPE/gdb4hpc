
/*************************************************************=
*****************\
 * C language test harness for basic types.
 * This is based off a modified mpi "hello world" application.
 *
 * =C2=A9 2012 Cray Inc.	All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and =
other laws.
 * Except as permitted by contract or express written permission of Cray =
Inc.,
 * no part of this work or its content may be used, reproduced or =
disclosed
 * in any form.
 *
 * $HeadURL: =
http://svn/svn/debugger-support/ccdb/trunk/extras/test_fixtures/c_type_te=
st.c $
 * $Date: 2012-04-25 16:30:37 -0500 (Wed, 25 Apr 2012) $
 * $Rev: 1648 $
 * $Author: andrewg $
 *
 
*************************************************************************=
*****/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

typedef void (*func_ptr_t)(int, int);
int test_stack(int x);

struct test_struct
{
	int 			i;
	double		d;
	char *		c_ptr;
	func_ptr_t	      func_ptr;
};

void 
test_func(int i, int j)
{
	int k;
	k = i+j;
}

int main(int argc, char* argv[])
{
	int myRank, numProcs;
	int source, dest=0, tag=0;
	char message[100];
	MPI_Status status;

	/* c primitives start below */
	char						c;
	signed char					sc;
	unsigned char			    uc;
	short						sh;
	short int					shi;
	signed short			    ssh;
	signed short int		    sshi;
	unsigned short			    ush;
	unsigned short int		    ushi;
	int							i;
	signed int					si;
	unsigned					u;
	unsigned int			    ui;
	long						l;
	long int					li;
	signed long					sl;
	signed long int			    sli;
	unsigned long			    ul;
	unsigned long int		    uli;
	long long					ll;
	long long int			    lli;
	signed long long		    sll;
	signed long long int	    slli;
	unsigned long long		    ull;
	unsigned long long int	    ulli;
	float						f;
	double						d;
	long double					ld;

	/* c arrays */
	int 			one_d_i_init[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int				one_d_i[10];
	int				two_d_i[10][20];
	int				three_d_i[10][20][30];
	long			one_d_l[10];
	long			two_d_l[10][20];
	long			three_d_l[10][20][30];
	float			one_d_f[10];
	float			two_d_f[10][20];
	float			three_d_f[10][20][30];
	double			one_d_d[10];
	double			two_d_d[10][20];
	double			three_d_d[10][20][30];
	long double		one_d_ld[10];
	long double		two_d_ld[10][20];
	long double		three_d_ld[10][20][30];

	/* c structs */
	struct test_struct		struct_test_1;

	/* c pointers */
	char *	char_ptr_test_1 = "12345678";
	void *	void_ptr;
	int *	int_ptr;
	int *	int_ptr_2;

	/* loop counters */
	int				ii,jj,kk;
	int				one_d_i_cnt = 0;
	int				two_d_i_cnt = 0;
	int				three_d_i_cnt = 0;
	long	        one_d_l_cnt = 0;
	long	       	two_d_l_cnt = 0;
	long	       	three_d_l_cnt = 0;
	float	        one_d_f_cnt = 0.1;
	float	        two_d_f_cnt = 0.1;
	float	        three_d_f_cnt = 0.1;
	double	     	one_d_d_cnt = 0.2;
	double	     	two_d_d_cnt = 0.2;
	double	     	three_d_d_cnt = 0.2;
	long double		one_d_ld_cnt = 0.3;
	long double		two_d_ld_cnt = 0.3;
	long double		three_d_ld_cnt = 0.3;
	

	/* other vars */
	unsigned long long int addr;

	MPI_Init(&argc,&argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

	if (myRank != 0) {
		sprintf(message,"Hello World! from process %d", myRank);
		MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
	} else {
		printf("Hello World! from process %d\n", myRank);

		for(source=1; source<numProcs; source++) {
			MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
			printf("%s\n", message);
		}
	}

    int	    assertTest = 5;


	/* initialize all the primitives */

	c		= 2;
	sc		= -3;
	uc		= 4;
	sh		= 5;
	shi		= 6;
	ssh		= -7;
	sshi	= -8;
	ush		= 9;
	ushi	= 10;
	i		= 11;
	si		= -12;
	u		= 13;
	ui		= 14;
	l		= 15;
	li		= 16;
	sl		= -17;
	sli		= -18;
	ul		= 19;
	uli		= 20;
	ll		= 21;
	lli		= 22;
	sll		=-23;
	slli	= -24;
	ull		= 25;
	ulli	= 26;
	f		= 27.0;
	d		= 28.1;
	ld		= 29.2;



	/* initialize all the arrays */

	for (ii=0; ii<10; ++ii)
	{
		one_d_i[ii] = one_d_i_cnt++;
		one_d_l[ii] = one_d_l_cnt++;
		one_d_f[ii] = one_d_f_cnt;
		one_d_f_cnt += 1.0;
		one_d_d[ii] = one_d_d_cnt;
		one_d_d_cnt += 1.0;
		one_d_ld[ii] = one_d_ld_cnt;
		one_d_ld_cnt += 1.0;
		for (jj=0; jj<20; ++jj)
		{
			two_d_i[ii][jj] = two_d_i_cnt++;
			two_d_l[ii][jj] = two_d_l_cnt++;
			two_d_f[ii][jj] = two_d_f_cnt++;
			two_d_d[ii][jj] = two_d_d_cnt++;
			two_d_ld[ii][jj] = two_d_ld_cnt++;
			for (kk=0; kk<30; ++kk)
			{
				three_d_i[ii][jj][kk] = three_d_i_cnt++;
				three_d_l[ii][jj][kk] = three_d_l_cnt++;
				three_d_f[ii][jj][kk] = three_d_f_cnt++;
				three_d_d[ii][jj][kk] = three_d_d_cnt++;
				three_d_ld[ii][jj][kk] = three_d_ld_cnt++;
			}
		}
	}


	int stackTest = 5;
	stackTest = test_stack(stackTest);

	/* initialize the structs */

	struct_test_1.i = 1;
	struct_test_1.d = 2.2;
	struct_test_1.c_ptr = char_ptr_test_1;
	struct_test_1.func_ptr = &test_func;

	/* initialize the pointers */

	void_ptr = malloc(8*sizeof(int));
	memset(void_ptr, 0, 8*sizeof(int));

	addr = strtoull(char_ptr_test_1, NULL, 0);
	int_ptr = (int *)&addr;

	int_ptr_2 = (int *)&one_d_i;

	MPI_Finalize();

	return 0;
}

int test_stack(int x){
	return x * 5;
}



