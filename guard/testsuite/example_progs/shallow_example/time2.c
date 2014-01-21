/*********************************************************************************\
 * time2.c
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

#include "decs.h"

void timetend(jstart,jend,dpdt,dudt,dvdt,cu,cv,h,z, dvdt_sync, cu_sync, cv_sync, h_sync, z_sync)
int jstart,jend;
float dudt	[o][m];
float dvdt	[o][m];
float dpdt	[o][m];
float z		[o][m];
float cv	[o][m];
float cu	[o][m];
float h		[o][m];
float dvdt_sync	[m];
float z_sync	[m];
float cv_sync	[m];
float cu_sync	[m];
float h_sync	[m];
{
  int i,j,ip,jp;
  float invdx, invdy;
  float dvdt_temp[o+1][m];
  float z_temp	[o+1][m];
  float cv_temp	[o+1][m];
  float cu_temp	[o+1][m];
  float h_temp	[o+1][m];
  

  invdx = 1./dx; invdy=1./dy;

  for(j=0; j<o+1; j++) {
    for (i = 0; i < m; i++) {
        dvdt_temp[j][i] = j<o? dvdt[j][i] : dvdt_sync[i];
        cu_temp[j][i] = j<o? cu[j][i] : cu_sync[i];
        cv_temp[j][i] = j<o? cv[j][i] : cv_sync[i];
        h_temp[j][i] = j<o? h[j][i] : h_sync[i];
        z_temp[j][i] = j<o? z[j][i] : z_sync[i];
    }
  }

  for(j=0; j<o; j++) {
    jp = (j+1) % (o+1);
    for (i = 0; i < m; i++) {
      ip = (i+1) % m;
      /* ENERGY CONSERVING */
      dpdt[j][i] = -(cu_temp[j][ip]-cu_temp[j][i])*invdx - (cv_temp[jp][i]-cv_temp[j][i])*invdy;
      dudt[j][ip] =
	0.125 * (z_temp[jp][ip] * (cv_temp[jp][ip] + cv_temp[jp][i]) - z_temp[j][ip] *
	(cv_temp[j][ip]+cv_temp[j][i])) - (h_temp[j][ip] - h_temp[j][i]) * invdx;
      dvdt_temp[jp][i] =
	-0.125 * (z_temp[jp][ip] * (cu_temp[jp][ip] + cu_temp[j][ip]) + z_temp[jp][i] *
	(cu_temp[jp][i]+cu_temp[j][i])) - (h_temp[jp][i] - h_temp[j][i]) * invdy;
    }
  }

  for(j=0; j<o+1; j++) {
    for(i=0; i<m; i++) {
        if (j<o)
                dvdt[j][i] = dvdt_temp[j][i];
        else 
                dvdt_sync[i] = dvdt_temp[j][i];
    }
  }

}
