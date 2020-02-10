/*********************************************************************************\
 * File info routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>

#include "defs.h"
#include "gram.h"
#include "useful.h"

file_t *FindFile(list_t *, char *);

file_t *NewFile(char *fn) {
    file_t *f;

    f = (file_t *)Alloc(sizeof(file_t));

    f->f_name = strdup(fn);
    f->f_lines = (list_t *)NULL;
    f->f_ref = 0;

    return f;
}

file_t *AddFile(list_t **fl, char *fn) {
    file_t *f;

    if (*fl == (list_t *)NULL) *fl = NewList();

    if ((f = FindFile(*fl, fn)) == (file_t *)NULL) {
        f = NewFile(fn);
        AddToList(*fl, (void *)f);
    }
    f->f_ref++;

    return f;
}

file_t *FindFile(list_t *fl, char *fn) {
    file_t *f;

    SetList(fl);

    while ((f = (file_t *)GetListElement(fl)) != (file_t *)NULL)
        if (strcmp(f->f_name, fn) == 0) return f;

    return (file_t *)NULL;
}

void DestroyFile(file_t *f) {
    f->f_ref--;

    if (f->f_ref >= 0) return;

    DestroyList(f->f_lines, (void (*)(void *))DestroyLine);
    Free(f->f_name);
    Free(f);
}
