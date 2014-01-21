/*********************************************************************************\
 * calc2.c
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

void calcuvzh(jstart,jend,p,u,v,cu,cv,h,z,p_sync,u_sync,v_sync,cu_sync,cv_sync,h_sync,z_sync,fsdx,fsdy)
int jstart,jend;
float p	[o][m];
float u	[o][m];
float v	[o][m];
float cu[o][m];
float cv[o][m];
float h	[o][m];
float z	[o][m];
float p_sync[m];
float u_sync[m];
float v_sync[m];
float cu_sync[m];
float cv_sync[m];
float h_sync[m];
float z_sync[m];
float fsdx, fsdy;
{
  int	i,j,ip,jp;
  float p_temp[o+1][m];
  float u_temp[o+1][m];
  float v_temp[o+1][m];
  float cu_temp[o+1][m];
  float cv_temp[o+1][m];
  float h_temp[o+1][m];
  float z_temp[o+1][m];
/*
	for(i=0; i<m; i++)
		printf("%f ", p_sync[i]);
	printf("\n");
*/
  for(j=0; j<o+1; j++) {
    for(i=0; i<m; i++) {
	p_temp[j][i] = j<o? p[j][i] : p_sync[i];
	u_temp[j][i] = j<o? u[j][i] : u_sync[i];
	v_temp[j][i] = j<o? v[j][i] : v_sync[i];
	cu_temp[j][i] = j<o? cu[j][i] : cu_sync[i];
	cv_temp[j][i] = j<o? cv[j][i] : cv_sync[i];
	h_temp[j][i] = j<o? h[j][i] : h_sync[i];
	z_temp[j][i] = j<o? z[j][i] : z_sync[i];
    }
  }
	//printf("%d %f %f\n", o, fsdx, fsdy);
  for(j=0; j<o; j++) {
     jp = (j+1) % (o+1); // jp = (j+1) % (o+1);
    for (i = 0; i < m; i++){
       ip = (i+1) % m; // ip = (i+1) % m;
      cu_temp[j][ip] = 0.5*(p_temp[j][ip]+p_temp[j][i])*u_temp[j][ip];
      cv_temp[jp][i] = 0.5*(p_temp[jp][i]+p_temp[j][i])*v_temp[jp][i];
      z_temp[jp][ip] = (fsdx*(v_temp[jp][ip]-v_temp[jp][i])-fsdy*(u_temp[jp][ip]
	     -u_temp[j][ip]))/(p_temp[j][i]+p_temp[j][ip]+p_temp[jp][ip]+p_temp[jp][i]);
      h_temp[j][i] = p_temp[j][i]+0.25*(u_temp[j][ip]*u_temp[j][ip]+u_temp[j][i]*u_temp[j][i]
		   +v_temp[jp][i]*v_temp[jp][i]+v_temp[j][i]*v_temp[j][i]);
    }
  }
  for(j=0; j<o+1; j++) {
    for(i=0; i<m; i++) {
	if (j<o) {
		p[j][i] = p_temp[j][i];
		u[j][i] = u_temp[j][i];
		v[j][i] = v_temp[j][i];
		cu[j][i] = cu_temp[j][i];
		cv[j][i] = cv_temp[j][i];
		h[j][i] = h_temp[j][i];
		z[j][i] = z_temp[j][i];
	}
	else {
		p_sync[i] = p_temp[j][i];
		u_sync[i] = u_temp[j][i];
		v_sync[i] = v_temp[j][i];
		cu_sync[i] = cu_temp[j][i];
		cv_sync[i] = cv_temp[j][i];
		h_sync[i] = h_temp[j][i];
		z_sync[i] = z_temp[j][i];
	}
    }
  }
}
