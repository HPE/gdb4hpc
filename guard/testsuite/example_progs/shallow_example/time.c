/*********************************************************************************\
 * time.c
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

void timetend(jstart,jend,dpdt,dudt,dvdt,cu,cv,h,z)
int jstart,jend;
float dudt[n][m];
float dvdt[n][m];
float dpdt[n][m];
float z[n][m];
float cv[n][m];
float cu[n][m];
float h[n][m];
{
  int i,j,ip,jp;
  float invdx, invdy;

  invdx = 1./dx; invdy=1./dy;
  for(j=jstart;j<=jend;j++) {
    jp = (j+1) % n;
    for (i = 0; i < m; i++) {
      ip = (i+1) % m;
      /* ENERGY CONSERVING */
      dpdt[j][i] = -(cu[j][ip]-cu[j][i])*invdx - (cv[jp][i]-cv[j][i])*invdy;
      dudt[j][ip] =
	0.125 * (z[jp][ip] * (cv[jp][ip] + cv[jp][i]) - z[j][ip] *
	(cv[j][ip]+cv[j][i])) - (h[j][ip] - h[j][i]) * invdx;
      dvdt[jp][i] =
	-0.125 * (z[jp][ip] * (cu[jp][ip] + cu[j][ip]) + z[jp][i] *
	(cu[jp][i]+cu[j][i])) - (h[jp][i] - h[j][i]) * invdy;
    }
  }
}
