/******************************************************************************\
 * rd.h - Relative debugger common interface defines.
 *
 * © 2013 Cray Inc.  All Rights Reserved.
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
 ******************************************************************************/

#ifndef _RD_H
#define _RD_H

/*
 * disp prototypes
 */
extern int CompareData(graph_t *, int, char *, AIF *, char *, AIF *, int);
extern int DisplayData(char *, char *, char *, AIF *);
extern int DisplayP2P(graph_t *, int, char *, AIF *);

#endif /* _RD_H */
