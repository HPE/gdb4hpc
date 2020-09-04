/*********************************************************************************\
 * calc.c
 *
 * Copyright 2010-2013,2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
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

void calcuvzh(jstart,jend,p,u,v,cu,cv,h,z,fsdx,fsdy)
int jstart,jend;
float p[n][m];
float u[n][m];
float v[n][m];
float cu[n][m];
float cv[n][m];
float h[n][m];
float z[n][m];
float fsdx, fsdy;
{
  int	i,j,ip,jp;

  for(j=jstart;j<=jend;j++) {
    jp = (j+1) % n;
    for (i = 0; i < m; i++){
      ip = (i+1) % m;
      cu[j][ip] = 0.5*(p[j][ip]+p[j][i])*u[j][ip];
      cv[jp][i] = 0.5*(p[jp][i]+p[j][i])*v[jp][i];
      z[jp][ip] = (fsdx*(v[jp][ip]-v[jp][i])-fsdy*(u[jp][ip]
	     -u[j][ip]))/(p[j][i]+p[j][ip]+p[jp][ip]+p[jp][i]);
      h[j][i] = p[j][i]+0.25*(u[j][ip]*u[j][ip]+u[j][i]*u[j][i]
		   +v[jp][i]*v[jp][i]+v[j][i]*v[j][i]);
    }
  }
}

