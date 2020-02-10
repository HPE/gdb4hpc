/******************************************************************************\
 * class_cmd.h - Class command routine defines.
 *
 * © 2011-2020 Cray Inc.
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

#ifndef _CLASS_CMD_H
#define _CLASS_CMD_H

#include "dbg_client.h"
#include "type_cmd.h"

/*
 * External class cmd objects
 */
extern cmdlist_t *InfoCmds;      /* Info command list */
extern cmdlist_t *MaintCmds;     /* Maint command list */
extern cmdlist_t *MaintSetCmds;  /* Maint Set command list */
extern cmdlist_t *MaintShowCmds; /* Maint Show command list */
extern cmdlist_t *SetCmds;       /* Set command list */
extern cmdlist_t *SetPrintCmds;  /* Set Print command list */
extern cmdlist_t *ShowCmds;      /* Show command list */
extern cmdlist_t *ShowPrintCmds; /* Show Print command list */
extern cmdlist_t *UnsetCmds;     /* Unset command list */
extern cmdlist_t *UsageCmds;     /* Usage command list */

/*
 * External AddClassCommand interface
 */
extern void AddInfoCommand(const char *, cmdptype_t, int, int (*)(),
                           const char *const[], const char *);
extern void AddMaintCommand(const char *, cmdptype_t, int, int (*)(),
                            const char *const[], const char *);
extern void AddMaintSetCommand(const char *, cmdptype_t, int, int (*)(),
                               const char *const[], const char *);
extern void AddMaintShowCommand(const char *, cmdptype_t, int, int (*)(),
                                const char *const[], const char *);
extern void AddSetCommand(const char *, cmdptype_t, int, int (*)(),
                          const char *const[], const char *);
extern void AddSetPrintCommand(const char *, cmdptype_t, int, int (*)(),
                               const char *const[], const char *);
extern void AddShowCommand(const char *, cmdptype_t, int, int (*)(),
                           const char *const[], const char *);
extern void AddShowPrintCommand(const char *, cmdptype_t, int, int (*)(),
                                const char *const[], const char *);
extern void AddUnsetCommand(const char *, cmdptype_t, int, int (*)(),
                            const char *const[], const char *);
extern void AddUsageCommand(const char *, cmdptype_t, int, int (*)(),
                            const char *const[], const char *);

/*
 * External class cmd interface
 */
extern int class_info_cmd(rec_set_t *, int, char **);
extern int class_maint_cmd(rec_set_t *, int, char **);
extern int class_maint_cmd(rec_set_t *, int, char **);
extern int class_maint_set_cmd(rec_set_t *, int, char **);
extern int class_maint_show_cmd(rec_set_t *, int, char **);
extern int class_set_cmd(rec_set_t *, int, char **);
extern int class_set_print_cmd(rec_set_t *, int, char **);
extern int class_show_cmd(rec_set_t *, int, char **);
extern int class_show_print_cmd(rec_set_t *, int, char **);
extern int class_unset_cmd(rec_set_t *, int, char **);
extern int class_usage_cmd(rec_set_t *p, int, char **);

/*
 * External class command init funcs
 */
extern void InitCmd_class(void);
extern void DestroyCmd_class(void);

#endif /* _CLASS_CMD_H */
