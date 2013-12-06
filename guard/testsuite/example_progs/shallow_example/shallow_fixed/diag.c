
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

#include <stdio.h>
#include "decs.h"

void diag(ncycle,time,p,u,v,h,z)
int ncycle;
float time;
float p[n][m];
float u[n][m];
float v[n][m];
float h[n][m];
float z[n][m];
/*
Calculate global integrals of kinetic and potential energy and
potential enstrophy
*/
{
  float	ptot,ketot,etot,enstot,ptime,pmean;
  int	i,j,ip,jp;

  ptot=0.; ketot=0.; etot=0.; enstot = 0.; pmean = 0.; 
  for (j = 0; j < n; j++) {
    for (i = 0; i < m; i++) {
      pmean = pmean+p[j][i];
    }
  }
  pmean = pmean/(m*n);
  for (j = 0; j < n; j++){
    jp = (j+1) % n;
    for (i = 0; i < m; i++){
      ip = (i+1) % m;
      ketot += p[j][i]*0.25*(u[j][ip]*u[j][ip]+u[j][i]*u[j][i]
		   +v[jp][i]*v[jp][i]+v[j][i]*v[j][i]);
	//printf("%f %f %f %f %f %12.3f\n", p[j][i], u[j][ip], u[j][i], v[jp][i], v[j][i], ketot);
      ptot += (p[j][i]-pmean)*(p[j][i]-pmean);
      etot += h[j][i];
      enstot += z[jp][ip]*z[jp][ip] * 0.25*
	     (p[j][i]+p[j][ip]+p[jp][ip]+p[jp][i]);
    }
  }
  ptot *= 0.5/(m*n);
  ketot /= (m*n);
  etot /= (m*n);
  enstot /= (m*n);
  ptime = time/secs_pd;

  if ( ncycle > 0 ) {
	  printf("Cycle number %5d    Model time in days %6.2f \n \
  Potential energy %12.3f  Kinetic Energy %12.3f \n \
  Total Energy     %12.3f  Pot. Enstrophy  %15.6e \n\n",
  ncycle,ptime,ptot,ketot,ptot+ketot,enstot);
  }
}
