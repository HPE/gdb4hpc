/******************************************************************************\
 * type_cmd.h - Types for command routines.
 *
 * Copyright 2011-2013,2020 Hewlett Packard Enterprise Development LP.
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
 ******************************************************************************/

#ifndef _TYPE_CMD_H
#define _TYPE_CMD_H

#include "dbg_client.h"
#include "list.h"

enum cmdres { cmd_ok, cmd_notfound, cmd_ambig, cmd_ambig_set, cmd_ambig_info };
typedef enum cmdres cmdres_t;

enum cmdptype { proc_required, proc_optional, proc_optional_mi, proc_ignore };
typedef enum cmdptype cmdptype_t;

struct cmdtype {
    const char *c_name;
    const char *c_alias;
    cmdptype_t c_ptype;
    int c_hassub;
    int c_multi;
    const char *c_desc;
    char **c_help;
    int (*c_cmd)(rec_set_t *, int, char **);
};
typedef struct cmdtype cmdtype_t;

struct cmdlist {
    list_t *cmd_list;
    void (*cmd_notfound)(const char *);
    void (*cmd_ambig)(const char *);
    void (*cmd_failed)(void);
    struct cmdtype *cmd_default;
};
typedef struct cmdlist cmdlist_t;

struct cmdhelp {
    const char *c_name;
    cmdtype_t *(*c_find)(char *);
};
typedef struct cmdhelp cmdhelp_t;

#endif /* _TYPE_CMD_H */
