
/************************************************************************
*									*
* Commonwealth Scientific and Industrial Research Organisation (CSIRO)	*
*	- Division of Information Technology	(DIT)			*
*	- Division of Atmospheric Research	(DAR)			*
*									*
* Shallow water weather model - Distributed Memory Version		*
*									*
* Finite difference model of shallow water equations based on :-	*
* "The dynamics of finite difference models of the shallow water	*
* equations" by R. Sadourney, JAS, 32, 1975.				*
* Code from:-								*
* "An introduction to three-dimensional climate modelling"		*
* by Washington and Parkinson						*
*									*
* Programmers	= David Abramson	(DIT) rcoda@koel.co.rmit.oz	*
*		= Paul Whiting		(DIT) rcopw@koel.co.rmit.oz	*
*		= Martin Dix		(DAR) mrd@koel.co.rmit.oz	*
* Language	= BSD c using Argonne NL macros				*
* O/S		= Unix System V						*
* H/W		= Encore Multimax 320					*
*									*
************************************************************************/

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include "decs.h"

#define debug_data      0x1
#define debug_master    0x2
#define debug_slave     0x4
#define debug_call      0x8
#define debug           0x0

/* message types */
#define START_SIGNAL	0
#define END_SIGNAL	4
#define CALC1a		20
#define CALC1b		21
#define CALC1c		22
#define CALC2a		23
#define CALC2b		24
#define	TIME1a		30
#define	TIME1b		31
#define	TIME1c		32
#define	TIME1d		33
#define TIME2		34
#define P_ROW		50
#define U_ROW		51
#define V_ROW		52
#define PSI_ROW		53
#define POLD_ROW	54
#define UOLD_ROW	55
#define VOLD_ROW	56
#define H_ROW		57
#define Z_ROW		58

#define	PREV	0
#define	NEXT	1
#define	JSTART	2
#define	JEND	3

struct res
{
	float	row[m];
	int	indx;
};

MPI_Datatype *	setup_res();

void _print_data(float ds[n][m], int numrows);

main (argc, argv)
	int	argc;
	char *	argv[];
{
	float 	p[n][m];	/* Pressure (or free surface height) */
	float 	u[n][m];	/* Zonal wind */
	float 	v[n][m];	/* Meridional wind */
	float 	psi[n][m];	/* Velocity streamfunction */
	float 	pold[n][m];
	float 	uold[n][m];
	float 	vold[n][m];
	float	h[n][m];
	float	z[n][m];
	float	dummy1[m];
	float	pi=4.*(float)atan((double)1.);
	float	tpi=pi+pi;
	float	di=tpi/(float)m;
	float	dj=tpi/(float)n;
	int	i, j, chunk_size, nxt, prv;

	int	master_packet[4];
	float	p_start[m];
	float	u_start[m];
	float	v_start[m];
	float	psi_start[m];
	float	pold_start[m];
	float	uold_start[m];
	float	vold_start[m];
	int	proc_cnt;
	int	tid;
	MPI_Datatype *	res_type;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_cnt);
	MPI_Comm_rank(MPI_COMM_WORLD, &tid);

	//sleep(10);

	if ( proc_cnt < 2 )
	{
		fprintf(stderr, "must have at least 2 processes, not %d\n", proc_cnt);
		MPI_Finalize();
		exit(1);
	}

	if ( (n % (proc_cnt - 1)) != 0 )
	{
		if ( tid == 0 )
			fprintf(stderr, "(number of processes - 1) must be a multiple of %d\n", n);

		MPI_Finalize();
		exit(1);
	}

	if (tid != 0) {
		slave();
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Finalize();
		exit(0);
	}

	/* master process */

	chunk_size = n / (proc_cnt - 1);

	for (i = 1; i < proc_cnt; i++) {
		/* calculate each slave's boundary */
		master_packet[JSTART] = (i - 1) * chunk_size;

		if (i == proc_cnt - 1)
			master_packet[JEND] = n - 1;
		else
			master_packet[JEND] = i * chunk_size - 1;

		if (i == 1)
			prv = proc_cnt-1;
		else
			prv = i-1;

		master_packet[PREV] = prv;

		if (i == proc_cnt - 1)
			nxt = 1;
		else
			nxt = i+1;

		master_packet[NEXT] = nxt;

		MPI_Send(&master_packet, 4, MPI_INT, i, START_SIGNAL,
			MPI_COMM_WORLD);

	printf("jstart=%d, jend=%d, next=%d, prev=%d\n", 
		master_packet[JSTART],
		master_packet[JEND],
		master_packet[NEXT],
		master_packet[PREV]);
	}


	/*
	initialise data structures and construct packets to be sent to slaves
	*/

	initialise(p, u, v, psi, pold, uold, vold, di, dj);
	diag(1, 0., p, u, v, h, z);

	for (i = 1; i < proc_cnt; i++) {
		for (j = (i-1)*o; j < o+((i-1)*o); j++) {
			acopy_two_to_one(p, p_start, j);
			MPI_Send(&p_start, m, MPI_FLOAT, i, P_ROW, 
				MPI_COMM_WORLD);

			acopy_two_to_one(u, u_start, j);
			MPI_Send(&u_start, m, MPI_FLOAT, i, U_ROW, 
				MPI_COMM_WORLD);

			acopy_two_to_one(v, v_start, j);
			MPI_Send(&v_start, m, MPI_FLOAT, i, V_ROW, 
				MPI_COMM_WORLD);

			acopy_two_to_one(psi, psi_start, j);
			MPI_Send(&psi_start, m, MPI_FLOAT, i, PSI_ROW, 
				MPI_COMM_WORLD);

			acopy_two_to_one(pold, pold_start, j);
			MPI_Send(&pold_start, m, MPI_FLOAT, i, POLD_ROW, 
				MPI_COMM_WORLD);

			acopy_two_to_one(uold, uold_start, j);
			MPI_Send(&uold_start, m, MPI_FLOAT, i, UOLD_ROW, 
				MPI_COMM_WORLD);

			acopy_two_to_one(vold, vold_start, j);
			MPI_Send(&vold_start, m, MPI_FLOAT, i, VOLD_ROW, 
				MPI_COMM_WORLD);
		}
	}

	/*
	receive packets back from the slaves
	*/
	res_type = setup_res();

	if ( debug & debug_master )
		printf("receiving P\n");

	update_global_ds(res_type, P_ROW, p);

	if ( debug & debug_master )
		printf("receiving U\n");

	update_global_ds(res_type, U_ROW, u);

	if ( debug & debug_master )
		printf("receiving V\n");

	update_global_ds(res_type, V_ROW, v);

	if ( debug & debug_master )
		printf("receiving H\n");

	update_global_ds(res_type, H_ROW, h);

	if ( debug & debug_master )
		printf("receiving Z\n");

	update_global_ds(res_type, Z_ROW, z);

	for (i = 1; i < proc_cnt; i++){
		MPI_Send(&master_packet, 4, MPI_INT, i, END_SIGNAL,
			MPI_COMM_WORLD);
	}

	/* wait for all slaves to end */
	MPI_Barrier(MPI_COMM_WORLD);

	diag(itmax, itmax*dt, p, u, v, h, z);

	MPI_Finalize();
	exit(0);
}

void _print_data(float ds[n][m], int numrows)
{
        int i, j;
        for(i=0; i<numrows; i++) {
                for(j=0; j<m; j++)
                        printf("%f ", ds[i][j]);
                printf("\n");
        }
        printf("\n");
        return;
}



MPI_Datatype *
setup_res()
{
	struct res		res;
	MPI_Aint		res_disp[2];
	static int		res_done = 0;
	static int		res_len[2] = { m, 1 };
	static MPI_Datatype	res_old[2] = { MPI_FLOAT, MPI_INT };
	static MPI_Datatype	res_type;

	if ( res_done )
		return &res_type;

	res_done++;
	MPI_Address(&res.row[0], &res_disp[0]);
	MPI_Address(&res.indx, &res_disp[1]);
	res_disp[1] -= res_disp[0];
	res_disp[0] = 0;
	MPI_Type_struct(2, res_len, res_disp, res_old, &res_type);
	MPI_Type_commit(&res_type);

	return &res_type;
}

/*
this function waits for all the slaves to return the packets of
a particular type and then updates the master's copy of the same type
*/
update_global_ds(res_type, indx, ds)
	MPI_Datatype *	res_type;
	int		indx;
	float		ds[n][m];

{
	int		i;
	int		row;
	struct res	res;
	MPI_Status	status;

	for (i = 0; i < n; i++) {
		MPI_Recv(&res, 1, *res_type, MPI_ANY_SOURCE, indx,
			MPI_COMM_WORLD, &status);

		acopy_one_to_two(res.row, ds, res.indx);
	}

}

slave()
{
	float 	p	[o][m];	/* Pressure (or free surface height) */
	float 	u	[o][m];	/* Zonal wind */
	float 	v	[o][m];	/* Meridional wind */
	float 	psi	[o][m];	/* Velocity streamfunction */
	float 	pold	[o][m];
	float 	uold	[o][m];
	float 	vold	[o][m];
	float	pnew	[o][m];
	float	unew	[o][m];
	float	vnew	[o][m];
	float	dpdt	[o][m];
	float	dudt	[o][m];	/* Time tendency of u */
	float	dvdt	[o][m];
	float	cu	[o][m];	/* Mass weighted u */
	float	cv	[o][m];	/* Mass weighted v */
	float	h	[o][m];       
	float	z	[o][m];	/* Potential enstrophy */
	float	dummy1[m];
	float	fsdx = 4./dx;
	float	fsdy = 4./dy;
	int	i,j,ip,jp,jstart,jend;
	int	prv;
	int	nxt;
	int	firststep, ncycle;
	float	tdt, time;

	int	msg_type;
	int	master_id, my_id;
	int	nprocs;
	int	nbytes;

	int		slave[4];
	float		p_start[m];
	float		u_start[m];
	float		v_start[m];
	float		psi_start[m];
	float		pold_start[m];
	float		uold_start[m];
	float		vold_start[m];

	float	p_sync[m];
	float	u_sync[m];
	float	v_sync[m];
	float	cu_sync[m];
	float	cv_sync[m];
	float	h_sync[m];
	float	z_sync[m];
	float	dvdt_sync[m];

	MPI_Datatype *	res_type;
	MPI_Status	status;

	/*
	initialise control variables
	*/

	firststep = 1;
	ncycle = 0;
	tdt = dt;
	time = 0.;

	/*
	set up environment for slave
	*/

	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

	MPI_Recv(&slave, 4, MPI_INT, MPI_ANY_SOURCE, START_SIGNAL,
		MPI_COMM_WORLD, &status);

	prv = slave[PREV];
	nxt = slave[NEXT];
	jstart = slave[JSTART];
	jend = slave[JEND];

	/*
	receive initialisation packets from master
	*/

	for (i = 0; i < o; i++){
		MPI_Recv(&p_start, m, MPI_FLOAT, 0, P_ROW, 
			MPI_COMM_WORLD, &status);
		acopy_one_to_two(p_start, p, i);

		MPI_Recv(&u_start, m, MPI_FLOAT, 0, U_ROW, 
			MPI_COMM_WORLD, &status);
		acopy_one_to_two(u_start, u, i);

		MPI_Recv(&v_start, m, MPI_FLOAT, 0, V_ROW, 
			MPI_COMM_WORLD, &status);
		acopy_one_to_two(v_start, v, i);

		MPI_Recv(&psi_start, m, MPI_FLOAT, 0, PSI_ROW, 
			MPI_COMM_WORLD, &status);
		acopy_one_to_two(psi_start, psi, i);

		MPI_Recv(&pold_start, m, MPI_FLOAT, 0, POLD_ROW, 
			MPI_COMM_WORLD, &status);
		acopy_one_to_two(pold_start, pold, i);

		MPI_Recv(&uold_start, m, MPI_FLOAT, 0, UOLD_ROW, 
			MPI_COMM_WORLD, &status);
		acopy_one_to_two(uold_start, uold, i);

		MPI_Recv(&vold_start, m, MPI_FLOAT, 0, VOLD_ROW, 
			MPI_COMM_WORLD, &status);
		acopy_one_to_two(vold_start, vold, i);
	}

	while (ncycle < itmax) {
		if ((debug & debug_data) && (my_id == 1)) {
			dump(p_label, two_dim, dummy1, p);
			dump(u_label, two_dim, dummy1, u);
			dump(v_label, two_dim, dummy1, v);
			dump(pold_label, two_dim, dummy1, pold);
			dump(psi_label, two_dim, dummy1, psi);
			dump(uold_label, two_dim, dummy1, uold);
			dump(vold_label, two_dim, dummy1, vold);
		}

		/*
		loop over latitudes calculating U, V, z and h
		do the block of latitudes from jstart to jend inclusive
		*/
		calc_load(prv, nxt, my_id, jstart, jend, p, u,v,p_sync,u_sync,v_sync);
		calcuvzh(jstart, jend, p, u, v, cu, cv, h, z, p_sync, u_sync, v_sync, cu_sync, cv_sync, h_sync, z_sync, fsdx, fsdy);
		calc_unload(prv, nxt, my_id, jstart, jend, cv, z, cv_sync, z_sync);

		// Calculate time tendencies of p, u and v
		time_load(prv, nxt, my_id, jstart, jend, cu, cv, h, z, cu_sync, cv_sync, h_sync, z_sync);
		timetend(jstart, jend, dpdt, dudt, dvdt, cu, cv, h, z, dvdt_sync, cu_sync, cv_sync, h_sync, z_sync);
		time_unload(prv, nxt, my_id, jstart, jend, dvdt, dvdt_sync);
		if ((my_id == 1) && (ncycle%mprint==0))
			diag_slave(ncycle, time, p, u, v, h, z);

		time += dt;

		tstep(jstart, jend, pold, uold, vold, p, u, v, pnew,
			unew, vnew, dpdt, dudt, dvdt, firststep, tdt);

		if ( firststep ) {
			tdt = tdt+tdt;  
			/* Double tdt because all future steps are leapfrog */
			firststep = 0;
		}

		ncycle++;
	}  /* End of time step loop */

	/*
	send local data structures (results) back to master
	*/

	res_type = setup_res();

	send_updated_ds(res_type, jstart, jend, p, P_ROW, 0);
	send_updated_ds(res_type, jstart, jend, u, U_ROW, 0);
	send_updated_ds(res_type, jstart, jend, v, V_ROW, 0);
	send_updated_ds(res_type, jstart, jend, h, H_ROW, 0);
	send_updated_ds(res_type, jstart, jend, z, Z_ROW, 0);

	MPI_Recv(&slave, 4, MPI_INT, 0, END_SIGNAL,
		MPI_COMM_WORLD, &status);

	if (debug & debug_call) {
		printf("slave %d sent TIDY_UP to master\n", my_id);
		printf("slave %d got END_SIGNAL from master\n", my_id);
	}
}

send_updated_ds(res_type, jstart, jend, ds, indx, master_id)
	MPI_Datatype *	res_type;
	int		jstart;
	int		jend;
	float		ds[o][m];
	int		indx;
	int		master_id;
{
	int		j;
	struct res	res;
	MPI_Request	rq[2];
	MPI_Status	stat[2];

	for (j = jstart; j <= jend; j++) {
		acopy_two_to_one(ds, res.row, j-jstart);
		res.indx = j;

		MPI_Send(&res, 1, *res_type, master_id, indx,
			MPI_COMM_WORLD);
	}
}

/*
this procedure does all the message passing before the call to _calcuvzh_
*/
calc_load(prv,nxt,my_id,jstart,jend,p,u,v, p_sync, u_sync, v_sync)
	int	prv;
	int	nxt;
	int	my_id;
	int	jstart;
	int	jend;
	float	p[o][m];
	float	u[o][m];
	float	v[o][m];
	float	p_sync[m];
	float	u_sync[m];
	float	v_sync[m];
{
	float tmp_ds[m];
	int i;
	acopy_two_to_one(p, tmp_ds, 0);
	neighbour_send(prv, my_id, CALC1a, tmp_ds);
	acopy_two_to_one(u, tmp_ds, 0);
	neighbour_send(prv, my_id, CALC1b, tmp_ds);
	acopy_two_to_one(v, tmp_ds, 0);
	neighbour_send(prv, my_id, CALC1c, tmp_ds);
	neighbour_receive(nxt, my_id, CALC1a, p_sync);
	neighbour_receive(nxt, my_id, CALC1b, u_sync);
	neighbour_receive(nxt, my_id, CALC1c, v_sync);
}

/*
this procedure does all the message passing after the call to _calcuvzh_
*/
calc_unload(prv,nxt,my_id,jstart,jend,cv,z,cv_sync, z_sync)
	int	prv;
	int	nxt;
	int	my_id;
	int	jstart;
	int	jend;
	float	cv[o][m];
	float	z[o][m];
	float	cv_sync[m];
	float	z_sync[m];
{
	neighbour_send(nxt, my_id, CALC2a, cv_sync);
	neighbour_send(nxt, my_id, CALC2b, z_sync);
	neighbour_receive(prv, my_id, CALC2a, cv[0]);
	neighbour_receive(prv, my_id, CALC2b, z[0]);
}

/*
this procedure does all the message passing before the call to _timetend_
*/
time_load(prv,nxt,my_id,jstart,jend,cu,cv,h,z, cu_sync, cv_sync, h_sync, z_sync)
	int	prv;
	int	nxt;
	int	my_id;
	int	jstart;
	int	jend;
	float	cu[o][m];
	float	cv[o][m];
	float	h[o][m];
	float	z[o][m];
	float	cu_sync[m];
	float	cv_sync[m];
	float	h_sync[m];
	float	z_sync[m];
{
	neighbour_send(prv, my_id, TIME1a, cu[0]);
	neighbour_send(prv, my_id, TIME1b, cv[0]);
	neighbour_send(prv, my_id, TIME1c, h[0]);
	neighbour_send(prv, my_id, TIME1d, z[0]);
	neighbour_receive(nxt, my_id, TIME1a, cu_sync);
	neighbour_receive(nxt, my_id, TIME1b, cv_sync);
	neighbour_receive(nxt, my_id, TIME1c, h_sync);
	neighbour_receive(nxt, my_id, TIME1d, z_sync);
}

/*
this procedure does all the message passing after the call to _timetend_
*/
time_unload(prv,nxt,my_id,jstart,jend,dvdt, dvdt_sync)
	int	prv;
	int	nxt;
	int	my_id;
	int	jstart;
	int	jend;
	float	dvdt[o][m];
	float	dvdt_sync[m];
{
	neighbour_send(nxt, my_id, TIME2, dvdt_sync);
	neighbour_receive(prv, my_id, TIME2, dvdt[0]);
}

/*
this is a general purpose function for sending packets b/w slaves
*/
neighbour_send(ns_neighbour,ns_my_id,ns_rec_id,ns_ds)
	int	ns_neighbour;
	int	ns_my_id;
	int	ns_rec_id;
	float	ns_ds[m];
{
	MPI_Request	rq;
	float		ns_rec[m];
	int i;
	for(i=0; i<m; i++)
		ns_rec[i] = ns_ds[i];
	MPI_Isend(&ns_rec, m, MPI_FLOAT, ns_neighbour, ns_rec_id,
		MPI_COMM_WORLD, &rq);

	if (debug & debug_slave)
		printf("slave %d sent packet %d to slave %d\n", ns_my_id,
			ns_rec_id, ns_neighbour);
}

/*
this is a general purpose function for receiving packets b/w slaves
*/
neighbour_receive(nr_neighbour,nr_my_id,nr_rec_id,nr_ds)
	int	nr_neighbour;
	int	nr_my_id;
	int	nr_rec_id;
	float	nr_ds[m];
{
	MPI_Status	status;
	float           nr_rec[m];
	int i;

	MPI_Recv(&nr_rec, m, MPI_FLOAT, nr_neighbour, nr_rec_id,
		MPI_COMM_WORLD, &status);
	for(i=0; i<m; i++)
		nr_ds[i] = nr_rec[i];
	
}
