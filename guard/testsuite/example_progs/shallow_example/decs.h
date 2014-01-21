/*********************************************************************************\
 * decs.h
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

#define	m	32	/* 31 Number of points in x direction */
#define	n	32	/* 15 Number of points in y direction */
#define slaves	8
#define	o	n/slaves /* data structure size for having 8 slaves */

#define	a	1.e6	/* Nominally the radius of the earth but here just 
			a length scale*/
#define	dt	90.0	/* Time step in seconds */
#define secs_pd	86400.	/* Seconds per day */
#define dx	1.e5	/* Grid spacing in x direction */
#define dy	1.e5	/* Grid spacing in y direction */
#define	alpha	0.001	/* Asselin time filter parameter */
#define itmax	950	/* Number of time steps in run */
#define mprint	50	/* Print diagnostics every mprint steps */

#define lower	1	/* low bound of range of processors */
#define upper	20	/* hi bound of range of processors */
#define version	0.6	/* version number of program */

/* constants used by dump() to determine what data structure to print */
#define one_dim		1
#define two_dim		2
#define p_label		0
#define u_label		1
#define v_label		2
#define pold_label	3
#define uold_label	4
#define vold_label	5
#define psi_label	6
#define cu_label	7
#define cv_label	8
#define h_label		9
#define z_label		10
#define dudt_label	11
#define dvdt_label	12
