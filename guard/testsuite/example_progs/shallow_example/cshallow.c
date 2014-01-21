/*********************************************************************************\
 * cshallow.c
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



/*
program shallow

  Finite difference model of shallow water equations based on
  "The dynamics of finite-difference models of the shallow-water equations"
  by R. Sadourny, JAS, 32, 1975.
  Code from "An introduction to three-dimensional climate modelling"
  by Washington and Parkinson.
*/

#include <stdio.h>
#include <math.h>
#include "mpi.h" // rwm

#define	m	32     /* Number of points in x direction */
#define	n	32     /* Number of points in y direction */
#define	a	1.e6   /* Nominally the radius of the earth but here just 
                          a length scale*/
#define	dt	90.0   /* Time step in seconds */
#define dx	1.e5   /* Grid spacing in x direction */
#define dy	1.e5   /* Grid spacing in y direction */
#define	alpha	0.001  /* Asselin time filter parameter */
#define itmax	950     /* Number of time steps in run */
#define mprint	50      /* Print diagnostics every mprint steps */

float	u [m][n];      /* Zonal wind */
float	v [m][n];      /* Meridional wind */
float	p [m][n];      /* Pressure (or free surface height) */
float	unew [m][n];
float	vnew [m][n];
float	pnew [m][n];
float	uold [m][n];
float	vold [m][n];
float	pold [m][n];
float	cu [m][n];     /* Mass weighted u */
float	cv [m][n];     /* Mass weighted v */
float	z [m][n];      /* Potential enstrophy */
float	h [m][n];       
float	psi [m][n];    /* Velocity streamfunction */
float   dudt [m][n];   /* Time tendency of u */
float   dvdt [m][n];
float   dpdt [m][n];

void initialise(), diag(), calcuvzh(), tstep(), timetend();
void dump();

main(int argc, char **argv)
{
int i,j,ip,jp,ncycle,jstart,jend,firststep=1;

float tdt=dt;
float time=0.;
float pi=4.*(float)atan((double)1.);
float tpi=pi+pi;
float di=tpi/(float)m;
float dj=tpi/(float)n;
float fsdx = 4./dx;
float fsdy = 4./dy;

MPI_Init(&argc, &argv); // rwm
initialise(u,v,p,psi,uold,vold,pold,di,dj);
diag(1,time,p,u,v,h,z);

for (ncycle = 0; ncycle < itmax; ncycle++ ) { /* Main time stepping loop */

 /* Loop over latitudes calculating U,V,z and h*/
/*ASSERT test u[0..99][0..99]*/
    jstart = 0; jend = n-1;   
 /* Do the block of latitudes from jstart to jend inclusive*/
    calcuvzh(jstart,jend,p,u,v,cu,cv,z,h,fsdx,fsdy);

/*  Calculate time tendencies of u, v and p */
    jstart = 0; jend = n-1;   
    timetend(jstart,jend,dudt,dvdt,dpdt,z,cv,cu,h);

    if(ncycle%mprint==0) diag(ncycle,time,p,u,v,h,z);

    time += dt;

    jstart = 0; jend = n-1;   
    tstep(jstart,jend,uold,vold,pold,u,v,p,unew,vnew,pnew,dudt,dvdt,dpdt,
          firststep,tdt);
    if ( firststep ) {
      tdt = tdt+tdt;  /* Double tdt because all future steps are leapfrog */
      firststep = 0;
    }

  }  /* End of time step loop */

  diag(itmax, itmax*dt, p, u, v, h, z);
}

void initialise(u,v,p,psi,uold,vold,pold,di,dj)
float	u [m][n];
float	v [m][n];
float	p [m][n];
float	psi [m][n];
float	uold [m][n];
float	vold [m][n];
float	pold [m][n];
float	di,dj;

{
int	i,j,ip,jp;
  /* *** initialise values of the streamfunction */
  for (j = 0; j < n; j++){
    for (i = 0; i < m; i++){
      float sin1 = sin((double)((i+0.5)*di));
      float sin2 = sin((double)((j+0.5)*dj));
      psi[i][j] = a*sin1*sin2;
    }
  }

  /* *** initialise velocities */
  for (j = 0; j < n; j++){
    jp = (j+1) % n;
    for (i = 0; i < m; i++){
        ip = (i+1) % m;
	//u[ip][j] = -(psi[ip][jp]-psi[ip][j])/dy;
	//v[i][jp] = -(psi[ip][jp]-psi[i][jp])/dx;
	//printf("u[%d][%d] = -(psi[%d][%d] - psi[%d][%d])/%f = %f\n", ip, j, ip, jp, ip, j, dy, u[ip][j]);
	u[i][jp] = -(psi[ip][jp]-psi[i][jp])/dy;
	v[ip][j] = -(psi[ip][jp]-psi[ip][j])/dx;
    }
  }

  for (j = 0; j < n; j++) {
    for (i = 0; i < m; i++) {
      uold[i][j] = u[i][j];
      vold[i][j] = v[i][j];
      pold[i][j] = 50000.;   /* Free surface height * gravitational accel'n */
      p[i][j] = 50000.;
    }
  }

printf(" Number of points in the X direction  %8d \n \
 Number of points in the Y direction  %8d \n \
 Grid spacing in the X direction      %8.0f \n \
 Grid spacing in the Y direction      %8.0f \n \
 Time step                            %8.3f \n \
 Time filter parameter                %8.3f \n ",
	 n,m,dx,dy,dt,alpha );
fflush(stdout);
} 

void diag(ncycle,time,p,u,v,h,z)
/* Calculate global integrals of kinetic and potential energy and potential
   enstrophy */
float p[m][n];
float u[m][n];
float v[m][n];
float h[m][n];
float z[m][n];
float time;
int   ncycle;
{
      float ptot,ketot,etot,enstot,ptime,pmean;
      int i,j,ip,jp;
      ptot=0.; ketot=0.; etot=0.; enstot = 0.; pmean = 0.; 
      for (j = 0; j < n; j++) {
        for (i = 0; i < m; i++) {
  	  pmean = pmean+p[i][j];
	}
      }
      pmean = pmean/(m*n);
      for (j = 0; j < n; j++){
	jp = (j+1) % n;
	for (i = 0; i < m; i++){
	  ip = (i+1) % m;
          ketot += p[i][j]*0.25*(u[i][jp]*u[i][jp]+u[i][j]*u[i][j]
                       +v[ip][j]*v[ip][j]+v[i][j]*v[i][j]);
	  ptot += (p[i][j]-pmean)*(p[i][j]-pmean);
          etot += h[i][j];
	  enstot += z[ip][jp]*z[ip][jp] * 0.25*
                 (p[i][j]+p[i][jp]+p[ip][jp]+p[ip][j]);
	}
      }
      ptot *= 0.5/(m*n);
      ketot /= (m*n);
      etot /= (m*n);
      enstot /= (m*n);
      ptime = time/86400.;
      if (ncycle > 0) {
	      printf(" Cycle number %5d    Model time in days %6.2f \n \
 Potential energy  %12.3f  Kinetic Energy %12.3f \n \
 Total Energy      %12.3f  Pot. Enstrophy  %15.6e \n\n",
 ncycle,ptime,ptot,ketot,ptot+ketot,enstot);
 fflush(stdout);
      }
}

void calcuvzh(jstart,jend,p,u,v,cu,cv,z,h,fsdx,fsdy)
int jstart,jend;
float p[m][n];
float u[m][n];
float v[m][n];
float cu[m][n];
float cv[m][n];
float z[m][n];
float h[m][n];
float fsdx, fsdy;
{
     int i,j,ip,jp;

     for(j=jstart;j<=jend;j++) {
	jp = (j+1) % n;
	for (i = 0; i < m; i++){
	  ip = (i+1) % m;
          cu[i][jp] = 0.5*(p[i][jp]+p[i][j])*u[i][jp];
	  cv[ip][j] = 0.5*(p[ip][j]+p[i][j])*v[ip][j];
	  z[ip][jp] = (fsdx*(v[ip][jp]-v[ip][j])-fsdy*(u[ip][jp]
                 -u[i][jp]))/(p[i][j]+p[i][jp]+p[ip][jp]+p[ip][j]);
          h[i][j] = p[i][j]+0.25*(u[i][jp]*u[i][jp]+u[i][j]*u[i][j]
                       +v[ip][j]*v[ip][j]+v[i][j]*v[i][j]);
	}
      }
}

void timetend(jstart,jend,dudt,dvdt,dpdt,z,cv,cu,h)
int jstart,jend;
float dudt[m][n];
float dvdt[m][n];
float dpdt[m][n];
float z[m][n];
float cv[m][n];
float cu[m][n];
float h[m][n];
{
      int i,j,ip,jp;
      float invdx, invdy;

      invdx = 1./dx; invdy=1./dy;
      for(j=jstart;j<=jend;j++) {
	jp = (j+1) % n;
	for (i = 0; i < m; i++){
          ip = (i+1) % m;
/* *** ENERGY CONSERVING */
          dudt[i][jp] = 0.125 * ( z[ip][jp]*(cv[ip][jp]+cv[ip][j]) +
                                z[i][jp]*(cv[i][jp]+cv[i][j]) )
                        - (h[i][jp]-h[i][j])*invdx;
          dvdt[ip][j] = - 0.125 * ( z[ip][jp]*(cu[ip][jp]+cu[i][jp]) +
                                  z[ip][j]*(cu[ip][j]+cu[i][j]) )
                         - (h[ip][j]-h[i][j])*invdy;
          dpdt[i][j] = -(cu[i][jp]-cu[i][j])*invdx - (cv[ip][j]-cv[i][j])*invdy;
	}
      }
}

void tstep(jstart,jend,uold,vold,pold,u,v,p,unew,vnew,pnew,dudt,dvdt,dpdt,firststep,tdt)
int jstart,jend;
float uold[m][n];
float vold[m][n];
float pold[m][n];
float u[m][n];
float v[m][n];
float p[m][n];
float unew[m][n];
float vnew[m][n];
float pnew[m][n];
float dudt[m][n];
float dvdt[m][n];
float dpdt[m][n];
float tdt;
int firststep;
{

      int i,j;
      for (j = jstart; j <= jend; j++){
	for (i = 0; i < m; i++){
	  unew[i][j] = uold[i][j] + tdt*dudt[i][j];
	  vnew[i][j] = vold[i][j] + tdt*dvdt[i][j];
	  pnew[i][j] = pold[i][j] + tdt*dpdt[i][j];
        }
      }

      if ( !firststep ) {  /* Don't apply time filter on first step */
        for (j = jstart; j <= jend; j++){
	  for (i = 0; i < m; i++){
            uold[i][j] = u[i][j]+alpha*(unew[i][j]-2.*u[i][j]+uold[i][j]);
            vold[i][j] = v[i][j]+alpha*(vnew[i][j]-2.*v[i][j]+vold[i][j]);
            pold[i][j] = p[i][j]+alpha*(pnew[i][j]-2.*p[i][j]+pold[i][j]);
	  }
        }
      }
#ifdef STRIDEERROR
      for (j = jstart; j <= jend; j = j + 4){
        for (i = 0; i < m; i = i + 4){
#else
#ifdef BOUNDARYERROR
      for (j = jstart; j <= jend-10; j++){
        for (i = 0; i < m; i++){
#else
      for (j = jstart; j <= jend; j++){
        for (i = 0; i < m; i++){
#endif
#endif
	  u[i][j] = unew[i][j];
	  v[i][j] = vnew[i][j];
	  p[i][j] = pnew[i][j];
        }
      }
}

void dump(twodim)
float	twodim[m][n];
{
  int	i, j;

  for (j = 0; j < n; j++){
    for (i = 0; i < m; i++){
	printf("%d,%d %f", i, j, twodim[i][j]);
	printf("\n");
      }
    }
}
