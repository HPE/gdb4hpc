/*********************************************************************************\
 * Assertion information for CompareData().
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2022 Hewlett Packard Enterprise Development LP.
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
 **********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <string.h>

#include "assertinfo.h"

ainfo_t **NewAssertInfoTable(void) {
    ainfo_t **ai_tab;

    ai_tab = (ainfo_t **)malloc(sizeof(ainfo_t *) * MAX_ASSERTS);
    memset((void *)ai_tab, 0, sizeof(ainfo_t *) * MAX_ASSERTS);

    return ai_tab;
}

ainfo_t *NewAssertInfoEntry(ainfo_t **ai_tab, int id) {
    ainfo_t *a;

    if (ai_tab == (ainfo_t **)NULL) return (ainfo_t *)NULL;

    if ((id < 0) || (id >= MAX_ASSERTS)) return (ainfo_t *)NULL;

    if (ai_tab[id] != (ainfo_t *)NULL) return ai_tab[id];

    a = (ainfo_t *)malloc(sizeof(ainfo_t));

    a->a_display = NULL;
    a->a_file = NULL;
    a->a_aifupper = NULL;
    a->a_aiflower = NULL;
    a->a_stop = 1;

    ai_tab[id] = a;

    return a;
}

void DestroyAssertInfo(ainfo_t **ai_tab, int ids) {
    ainfo_t *a;
    int i;

    if (ai_tab == (ainfo_t **)NULL) return;

    if (ids > MAX_ASSERTS) ids = MAX_ASSERTS;

    for (i = 0; i < ids; i++) {
        if ((a = ai_tab[i]) == (ainfo_t *)NULL) continue;

        if (a->a_display != NULL) free(a->a_display);
        if (a->a_file != NULL) free(a->a_file);
        if (a->a_aifupper != NULL) AIFFree(a->a_aifupper);
        if (a->a_aiflower != NULL) AIFFree(a->a_aiflower);

        free(a);
    }

    free(ai_tab);
}

ainfo_t *FindAssertInfo(ainfo_t **ai_tab, int id) {
    if (ai_tab == (ainfo_t **)NULL) return (ainfo_t *)NULL;

    if (id < 0 || id >= MAX_ASSERTS) return (ainfo_t *)NULL;

    return ai_tab[id];
}
