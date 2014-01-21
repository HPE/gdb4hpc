/*********************************************************************************\
 * copy2.c
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

#include <string.h>
#include "decs.h"

twod_acopy(src,dest)
float	src[n][m];
float	dest[n][m];
{
  int	i, j;

  /*
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      dest[j][i] = src[j][i];
    }
  }
  */
  for (j = 0; j < n; j++)
    bcopy(src[j], dest[j], sizeof(src[j]));
}

twod_acopy_column(src,dest,column)
float	src[n][m];
float	dest[n][m];
int 	column;
/*
This now does a ROW COPY and not a column copy
*/
{
  int	i;

  /*
  for (i = 0; i < m; i++)
    dest[column][i] = src[column][i];
  */
  bcopy(src[column], dest[column], sizeof(src[column]));
}

acopy_two_to_one(twodim,onedim,column)
float	twodim[n][m];
float	onedim[m];
int	column;
/*
This now does a ROW COPY and not a column copy
*/
{
  int	i;

  /*
  for (i = 0; i < m; i++)
    onedim[i] = twodim[column][i];
  */
  bcopy(twodim[column], onedim, sizeof(float)*m);
}

acopy_two_to_one_slave(twodim,onedim,column)
float	twodim[o][m];
float	onedim[m];
int	column;
{
  int	i;
  bcopy(twodim[column], onedim, sizeof(float)*m);
}

acopy_one_to_two(onedim,twodim,column)
float	twodim[n][m];
float	onedim[m];
int	column;
{
  int	i;

  /*
  for (i = 0; i < m; i++)
    twodim[column][i] = onedim[i];
  */
  bcopy(onedim, twodim[column], sizeof(float)*m);
}

acopy_one_to_two_slave(onedim,twodim,column)
float	twodim[o][m];
float	onedim[m];
int	column;
{
  int	i;
  bcopy(onedim, twodim[column], sizeof(float)*m);
}
