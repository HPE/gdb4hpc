/*********************************************************************************\
 * Assertion info defines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Cray Inc.
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
 **********************************************************************************/

#ifndef _AINFO_H
#define _AINFO_H

#include "aif.h"
#include "gvar.h"
#include "list.h"

#define MAX_ASSERTS 512

struct ainfo_t {
    double a_lower;
    double a_upper;
    AIF *a_aifupper;
    AIF *a_aiflower;
    int a_relative;
    int a_force;
    int a_stop;
    char *a_display;
    char *a_file;
    dfmode_t a_dfmode;
    dfmode_t a_logging;
    char *a_logfile;
};
typedef struct ainfo_t ainfo_t;

extern ainfo_t **NewAssertInfoTable(void);
extern ainfo_t *NewAssertInfoEntry(ainfo_t **, int);
extern ainfo_t *FindAssertInfo(ainfo_t **, int);
extern void DestroyAssertInfo(ainfo_t **, int);

#endif /* !_AINFO_H */
