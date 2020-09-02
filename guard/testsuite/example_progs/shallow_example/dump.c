/*********************************************************************************\
 * dump.c
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
#include <stdio.h>
#include "decs.h"

void dump(indx, one_or_two, onedim, twodim)
int	indx;
int	one_or_two;
float	onedim[m];
float	twodim[n][m];
{
  int	i, j;

  printf("\n");
  switch (indx) {
    case p_label :
      printf("dumping p:\n");
      break;
    case u_label :
      printf("dumping u:\n");
      break;
    case v_label :
      printf("dumping v:\n");
      break;
    case pold_label :
      printf("dumping pold:\n");
      break;
    case uold_label :
      printf("dumping uold:\n");
      break;
    case vold_label :
      printf("dumping vold:\n");
      break;
    case psi_label :
      printf("dumping psi:\n");
      break;
    case cu_label :
      printf("dumping cu:\n");
      break;
    case cv_label :
      printf("dumping cv:\n");
      break;
    case h_label :
      printf("dumping h:\n");
      break;
    case z_label :
      printf("dumping z:\n");
      break;
    case dudt_label :
      printf("dumping dudt:\n");
      break;
    case dvdt_label :
      printf("dumping dvdt:\n");
      break;
  }

  if (one_or_two == 1) {
    for (i = 0; i < m; i++) {
      printf("%d %f", i, onedim[i]);
      printf("\n");
    }
  }
  else {
    for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++) {
	printf("%d,%d %f", i, j, twodim[j][i]);
	printf("\n");
      }
    }
  }
}
