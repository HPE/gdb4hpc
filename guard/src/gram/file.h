/*********************************************************************************\
 * File info defines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013,2020 Cray Inc.
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

#ifndef _FILE_H
#define _FILE_H

#include "list.h"

struct file_t {
    char *f_name;
    list_t *f_lines;
    int f_ref;
};
typedef struct file_t file_t;

extern file_t *AddFile(list_t **, char *);
extern void DestroyFile(file_t *);
#endif /* !_FILE_H */
