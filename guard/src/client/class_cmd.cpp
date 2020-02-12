/******************************************************************************\
 * class_cmd.c - Class Command processing routines
 *
 * Copyright 2011-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

/*
 * Class command lists - External access
 */
cmdlist_t *InfoCmds;      /* Info command list */
cmdlist_t *MaintCmds;     /* Maint command list */
cmdlist_t *MaintSetCmds;  /* Maint Set command list */
cmdlist_t *MaintShowCmds; /* Maint Show command list */
cmdlist_t *SetCmds;       /* Set command list */
cmdlist_t *SetPrintCmds;  /* Set Print command list */
cmdlist_t *ShowCmds;      /* Show command list */
cmdlist_t *ShowPrintCmds; /* Show Print command list */
cmdlist_t *UnsetCmds;     /* Unset command list */
cmdlist_t *UsageCmds;     /* Usage command list */

/*
 * AddCommand functions for command classes - External access
 */

void AddInfoCommand(const char *name, cmdptype_t pt, int sub, int (*cmd)(),
                    const char *const help[], const char *desc) {
    AddClassSubCommand(InfoCmds->cmd_list, name, pt, sub, cmd, help, desc);
}

void AddMaintCommand(const char *name, cmdptype_t pt, int sub, int (*cmd)(),
                     const char *const help[], const char *desc) {
    AddClassSubCommand(MaintCmds->cmd_list, name, pt, sub, cmd, help, desc);
}

void AddMaintSetCommand(const char *name, cmdptype_t pt, int sub, int (*cmd)(),
                        const char *const help[], const char *desc) {
    AddClassSubCommand(MaintSetCmds->cmd_list, name, pt, sub, cmd, help, desc);
}

void AddMaintShowCommand(const char *name, cmdptype_t pt, int sub, int (*cmd)(),
                         const char *const help[], const char *desc) {
    AddClassSubCommand(MaintShowCmds->cmd_list, name, pt, sub, cmd, help, desc);
}

void AddSetCommand(const char *name, cmdptype_t pt, int sub, int (*cmd)(),
                   const char *const help[], const char *desc) {
    AddClassSubCommand(SetCmds->cmd_list, name, pt, sub, cmd, help, desc);
}

void AddSetPrintCommand(const char *name, cmdptype_t pt, int sub, int (*cmd)(),
                        const char *const help[], const char *desc) {
    AddClassSubCommand(SetPrintCmds->cmd_list, name, pt, sub, cmd, help, desc);
}

void AddShowCommand(const char *name, cmdptype_t pt, int sub, int (*cmd)(),
                    const char *const help[], const char *desc) {
    AddClassSubCommand(ShowCmds->cmd_list, name, pt, sub, cmd, help, desc);
}

void AddShowPrintCommand(const char *name, cmdptype_t pt, int sub, int (*cmd)(),
                         const char *const help[], const char *desc) {
    AddClassSubCommand(ShowPrintCmds->cmd_list, name, pt, sub, cmd, help, desc);
}

void AddUnsetCommand(const char *name, cmdptype_t pt, int sub, int (*cmd)(),
                     const char *const help[], const char *desc) {
    AddClassSubCommand(UnsetCmds->cmd_list, name, pt, sub, cmd, help, desc);
}

void AddUsageCommand(const char *name, cmdptype_t pt, int sub, int (*cmd)(),
                     const char *const help[], const char *desc) {
    AddClassSubCommand(UsageCmds->cmd_list, name, pt, sub, cmd, help, desc);
}

/*
 * DoCommand functions for command classes
 */

static int DoInfoCommand(dbgprocset_t *s, cmdtype_t *c, int argc, char *argv[],
                         int sub) {
    return DoClassSubCommand(InfoCmds, s, c, argc, argv, sub);
}

static int DoMaintCommand(dbgprocset_t *s, cmdtype_t *c, int argc, char *argv[],
                          int sub) {
    return DoClassSubCommand(MaintCmds, s, c, argc, argv, sub);
}

static int DoMaintSetCommand(dbgprocset_t *s, cmdtype_t *c, int argc,
                             char *argv[], int sub) {
    return DoClassSubCommand(MaintSetCmds, s, c, argc, argv, sub);
}

static int DoMaintShowCommand(dbgprocset_t *s, cmdtype_t *c, int argc,
                              char *argv[], int sub) {
    return DoClassSubCommand(MaintShowCmds, s, c, argc, argv, sub);
}

static int DoSetCommand(dbgprocset_t *s, cmdtype_t *c, int argc, char *argv[],
                        int sub) {
    return DoClassSubCommand(SetCmds, s, c, argc, argv, sub);
}

static int DoSetPrintCommand(dbgprocset_t *s, cmdtype_t *c, int argc,
                             char *argv[], int sub) {
    return DoClassSubCommand(SetPrintCmds, s, c, argc, argv, sub);
}

static int DoShowCommand(dbgprocset_t *s, cmdtype_t *c, int argc, char *argv[],
                         int sub) {
    return DoClassSubCommand(ShowCmds, s, c, argc, argv, sub);
}

static int DoShowPrintCommand(dbgprocset_t *s, cmdtype_t *c, int argc,
                              char *argv[], int sub) {
    return DoClassSubCommand(ShowPrintCmds, s, c, argc, argv, sub);
}

static int DoUnsetCommand(dbgprocset_t *s, cmdtype_t *c, int argc, char *argv[],
                          int sub) {
    return DoClassSubCommand(UnsetCmds, s, c, argc, argv, sub);
}

static int DoUsageCommand(dbgprocset_t *s, cmdtype_t *c, int argc, char *argv[],
                          int sub) {
    return DoClassSubCommand(UsageCmds, s, c, argc, argv, sub);
}

/*
 * FindCommand functions for command classes
 */

static cmdtype_t *FindInfoCommand(char *cmd) {
    cmdtype_t *rtn;

    rtn = FindSubCommand(InfoCmds, cmd);

    return rtn;
}

static cmdtype_t *FindMaintCommand(char *cmd) {
    return FindSubCommand(MaintCmds, cmd);
}

static cmdtype_t *FindMaintSetCommand(char *cmd) {
    return FindSubCommand(MaintSetCmds, cmd);
}

static cmdtype_t *FindMaintShowCommand(char *cmd) {
    return FindSubCommand(MaintShowCmds, cmd);
}

static cmdtype_t *FindSetCommand(char *cmd) {
    return FindSubCommand(SetCmds, cmd);
}

static cmdtype_t *FindSetPrintCommand(char *cmd) {
    return FindSubCommand(SetPrintCmds, cmd);
}

static cmdtype_t *FindShowCommand(char *cmd) {
    return FindSubCommand(ShowCmds, cmd);
}

static cmdtype_t *FindShowPrintCommand(char *cmd) {
    return FindSubCommand(ShowPrintCmds, cmd);
}

static cmdtype_t *FindUnsetCommand(char *cmd) {
    return FindSubCommand(UnsetCmds, cmd);
}

static cmdtype_t *FindUsageCommand(char *cmd) {
    return FindSubCommand(UsageCmds, cmd);
}

/*
 * Command not found behavior for command classes
 */

static void InfoCmdNotFound(const char *cmd) {
    PrintOutError("Undefined info command: \"%s\".  Try \"help info\".\n", cmd);
}

static void MaintCmdNotFound(const char *cmd) {
    PrintOutError("Undefined maint command: \"%s\". Try \"help maint\".\n",
                  cmd);
}

static void MaintSetCmdNotFound(const char *cmd) {
    PrintOutError(
        "Undefined maint set command: \"%s\". Try \"help maint set\".\n", cmd);
}

static void MaintShowCmdNotFound(const char *cmd) {
    PrintOutError(
        "Undefined maint show command: \"%s\". Try \"help maint show\".\n",
        cmd);
}

static void SetCmdNotFound(const char *cmd) {
    PrintOutError("Undefined set command: \"%s\".  Try \"help set\".\n", cmd);
}

static void SetPrintCmdNotFound(const char *cmd) {
    PrintOutError(
        "Undefined set print command: \"%s\".  Try \"help set print\".\n", cmd);
}

static void ShowCmdNotFound(const char *cmd) {
    PrintOutError("Undefined show command: \"%s\". Try \"help show\".\n", cmd);
}

static void ShowPrintCmdNotFound(const char *cmd) {
    PrintOutError(
        "Undefined show print command: \"%s\". Try \"help show print\".\n",
        cmd);
}

static void UnsetCmdNotFound(const char *cmd) {
    PrintOutError("Undefined unset command: \"%s\".  Try \"help unset\".\n",
                  cmd);
}

static void UsageCmdNotFound(const char *cmd) {
    PrintOutError("Undefined usage command: \"%s\". Try \"help usage\".\n",
                  cmd);
}

/*
 * Ambiguous command behavior for command classes
 */

static void AmbiguousInfoCmd(const char *cmd) {
    PrintOutError("Ambiguous info command \"%s\": ", cmd);
    AmbiguousClassSubCmd(InfoCmds->cmd_list, cmd);
}

static void AmbiguousMaintCmd(const char *cmd) {
    PrintOutError("Ambiguous maint command \"%s\": ", cmd);
    AmbiguousClassSubCmd(MaintCmds->cmd_list, cmd);
}

static void AmbiguousMaintSetCmd(const char *cmd) {
    PrintOutError("Ambiguous maint set command \"%s\": ", cmd);
    AmbiguousClassSubCmd(MaintSetCmds->cmd_list, cmd);
}

static void AmbiguousMaintShowCmd(const char *cmd) {
    PrintOutError("Ambiguous maint show command \"%s\": ", cmd);
    AmbiguousClassSubCmd(MaintShowCmds->cmd_list, cmd);
}

static void AmbiguousSetCmd(const char *cmd) {
    PrintOutError("Ambiguous set command \"%s\": ", cmd);
    AmbiguousClassSubCmd(SetCmds->cmd_list, cmd);
}

static void AmbiguousSetPrintCmd(const char *cmd) {
    PrintOutError("Ambiguous set print command \"%s\": ", cmd);
    AmbiguousClassSubCmd(SetPrintCmds->cmd_list, cmd);
}

static void AmbiguousShowCmd(const char *cmd) {
    PrintOutError("Ambiguous show command \"%s\": ", cmd);
    AmbiguousClassSubCmd(ShowCmds->cmd_list, cmd);
}

static void AmbiguousShowPrintCmd(const char *cmd) {
    PrintOutError("Ambiguous show print command \"%s\": ", cmd);
    AmbiguousClassSubCmd(ShowPrintCmds->cmd_list, cmd);
}

static void AmbiguousUnsetCmd(const char *cmd) {
    PrintOutError("Ambiguous unset command \"%s\": ", cmd);
    AmbiguousClassSubCmd(UnsetCmds->cmd_list, cmd);
}

static void AmbiguousUsageCmd(const char *cmd) {
    PrintOutError("Ambiguous usage command \"%s\": ", cmd);
    AmbiguousClassSubCmd(UsageCmds->cmd_list, cmd);
}

/*
 * Command classes are defined below - External visibility for custom command
 * lists
 */

int class_info_cmd(rec_set_t *rec_set, int argc, char **argv) {
    cmdtype_t *c;
    dbgprocset_t *s = CurrentProcSet();

    if (argc < 2) {
        SetList(InfoCmds->cmd_list);

        while ((c = (cmdtype_t *)GetListElement(InfoCmds->cmd_list)) !=
               (cmdtype_t *)NULL) {
            if (c->c_cmd != NULL) {
                PrintOutMsg("info %-20s %s\n", c->c_name, c->c_desc);
            }
        }
        return 1;
    }

    if ((c = FindInfoCommand(argv[1])) == (cmdtype_t *)NULL) return 0;

    return DoInfoCommand(s, c, --argc, &argv[1], 1);
}

int class_maint_cmd(rec_set_t *rec_set, int argc, char **argv) {
    cmdtype_t *c;
    dbgprocset_t *s = CurrentProcSet();

    if (argc < 2) {
        SetList(MaintCmds->cmd_list);

        while ((c = (cmdtype_t *)GetListElement(MaintCmds->cmd_list)) !=
               (cmdtype_t *)NULL) {
            if (c->c_cmd != NULL) {
                PrintOutMsg("maint %-20s %s\n", c->c_name, c->c_desc);
            }
        }
        return 1;
    }

    if ((c = FindMaintCommand(argv[1])) == (cmdtype_t *)NULL) return 0;

    return DoMaintCommand(s, c, --argc, &argv[1], 1);
}

int class_maint_set_cmd(rec_set_t *rec_set, int argc, char **argv) {
    cmdtype_t *c;
    dbgprocset_t *s = CurrentProcSet();

    if (argc < 2) {
        SetList(MaintSetCmds->cmd_list);

        while ((c = (cmdtype_t *)GetListElement(MaintSetCmds->cmd_list)) !=
               (cmdtype_t *)NULL) {
            if (c->c_cmd != NULL) {
                PrintOutMsg("maint set %-20s %s\n", c->c_name, c->c_desc);
            }
        }
        return 1;
    }

    if ((c = FindMaintSetCommand(argv[1])) == (cmdtype_t *)NULL) return 0;

    return DoMaintSetCommand(s, c, --argc, &argv[1], 1);
}

int class_maint_show_cmd(rec_set_t *rec_set, int argc, char **argv) {
    cmdtype_t *c;
    dbgprocset_t *s = CurrentProcSet();

    if (argc < 2) {
        SetList(MaintShowCmds->cmd_list);

        while ((c = (cmdtype_t *)GetListElement(MaintShowCmds->cmd_list)) !=
               (cmdtype_t *)NULL) {
            if (c->c_cmd != NULL) {
                PrintOutMsg("maint show %-20s %s\n", c->c_name, c->c_desc);
            }
        }
        return 1;
    }

    if ((c = FindMaintShowCommand(argv[1])) == (cmdtype_t *)NULL) return 0;

    return DoMaintShowCommand(s, c, --argc, &argv[1], 1);
}

int class_set_cmd(rec_set_t *rec_set, int argc, char **argv) {
    cmdtype_t *c;
    dbgprocset_t *s = CurrentProcSet();

    if (argc < 2) {
        SetList(SetCmds->cmd_list);

        while ((c = (cmdtype_t *)GetListElement(SetCmds->cmd_list)) !=
               (cmdtype_t *)NULL)
            if (c->c_cmd != NULL)
                PrintOutMsg("set %-20s %s\n", c->c_name, c->c_desc);
        return 1;
    }

    if ((c = FindSetCommand(argv[1])) == (cmdtype_t *)NULL) return 0;

    return DoSetCommand(s, c, --argc, &argv[1], 1);
}

int class_set_print_cmd(rec_set_t *rec_set, int argc, char **argv) {
    cmdtype_t *c;
    dbgprocset_t *s = CurrentProcSet();

    if (argc < 2) {
        SetList(SetPrintCmds->cmd_list);

        while ((c = (cmdtype_t *)GetListElement(SetPrintCmds->cmd_list)) !=
               (cmdtype_t *)NULL)
            if (c->c_cmd != NULL)
                PrintOutMsg("set print %-20s %s\n", c->c_name, c->c_desc);
        return 1;
    }

    if ((c = FindSetPrintCommand(argv[1])) == (cmdtype_t *)NULL) return 0;

    return DoSetPrintCommand(s, c, --argc, &argv[1], 1);
}

int class_show_cmd(rec_set_t *rec_set, int argc, char **argv) {
    cmdtype_t *c;
    dbgprocset_t *s = CurrentProcSet();

    if (argc < 2) {
        SetList(ShowCmds->cmd_list);

        while ((c = (cmdtype_t *)GetListElement(ShowCmds->cmd_list)) !=
               (cmdtype_t *)NULL)
            if (c->c_cmd != NULL)
                PrintOutMsg("show %-20s %s\n", c->c_name, c->c_desc);
        return 1;
    }

    if ((c = FindShowCommand(argv[1])) == (cmdtype_t *)NULL) return 0;

    return DoShowCommand(s, c, --argc, &argv[1], 1);
}

int class_show_print_cmd(rec_set_t *rec_set, int argc, char **argv) {
    cmdtype_t *c;
    dbgprocset_t *s = CurrentProcSet();

    if (argc < 2) {
        SetList(ShowPrintCmds->cmd_list);

        while ((c = (cmdtype_t *)GetListElement(ShowPrintCmds->cmd_list)) !=
               (cmdtype_t *)NULL)
            if (c->c_cmd != NULL)
                PrintOutMsg("show print %-20s %s\n", c->c_name, c->c_desc);
        return 1;
    }

    if ((c = FindShowPrintCommand(argv[1])) == (cmdtype_t *)NULL) return 0;

    return DoShowPrintCommand(s, c, --argc, &argv[1], 1);
}

int class_unset_cmd(rec_set_t *rec_set, int argc, char **argv) {
    cmdtype_t *c;
    dbgprocset_t *s = CurrentProcSet();

    if (argc < 2) {
        SetList(UnsetCmds->cmd_list);

        while ((c = (cmdtype_t *)GetListElement(UnsetCmds->cmd_list)) !=
               (cmdtype_t *)NULL)
            if (c->c_cmd != NULL)
                PrintOutMsg("unset %-20s %s\n", c->c_name, c->c_desc);
        return 1;
    }

    if ((c = FindUnsetCommand(argv[1])) == (cmdtype_t *)NULL) return 0;

    return DoUnsetCommand(s, c, --argc, &argv[1], 1);
}

int class_usage_cmd(rec_set_t *rec_set, int argc, char **argv) {
    cmdtype_t *c;
    dbgprocset_t *s = CurrentProcSet();

    if (argc < 2) {
        SetList(UsageCmds->cmd_list);

        while ((c = (cmdtype_t *)GetListElement(UsageCmds->cmd_list)) !=
               (cmdtype_t *)NULL)
            if (c->c_cmd != NULL)
                PrintOutMsg("usage %-20s %s\n", c->c_name, c->c_desc);
        return 1;
    }

    if ((c = FindUsageCommand(argv[1])) == (cmdtype_t *)NULL) return 0;

    return DoUsageCommand(s, c, --argc, &argv[1], 1);
}

/*
 * Init functions for command classes - External visibility
 */

void InitCmd_class(void) {
    InfoCmds =
        NewCommandList(InfoCmdNotFound, AmbiguousInfoCmd, (void (*)(void))NULL);
    MaintCmds = NewCommandList(MaintCmdNotFound, AmbiguousMaintCmd,
                               (void (*)(void))NULL);
    MaintSetCmds = NewCommandList(MaintSetCmdNotFound, AmbiguousMaintSetCmd,
                                  (void (*)(void))NULL);
    MaintShowCmds = NewCommandList(MaintShowCmdNotFound, AmbiguousMaintShowCmd,
                                   (void (*)(void))NULL);
    SetCmds =
        NewCommandList(SetCmdNotFound, AmbiguousSetCmd, (void (*)(void))NULL);
    SetPrintCmds = NewCommandList(SetPrintCmdNotFound, AmbiguousSetPrintCmd,
                                  (void (*)(void))NULL);
    ShowCmds =
        NewCommandList(ShowCmdNotFound, AmbiguousShowCmd, (void (*)(void))NULL);
    ShowPrintCmds = NewCommandList(ShowPrintCmdNotFound, AmbiguousShowPrintCmd,
                                   (void (*)(void))NULL);
    UnsetCmds = NewCommandList(UnsetCmdNotFound, AmbiguousUnsetCmd,
                               (void (*)(void))NULL);
    UsageCmds = NewCommandList(UsageCmdNotFound, AmbiguousUsageCmd,
                               (void (*)(void))NULL);

    // class commands
    AddClassCommand(
        "info", NULL, proc_ignore, 1, (int (*)())class_info_cmd,
        FindInfoCommand, class_info_help,
        "Display information about the application being debugged.");
    AddClassCommand("maint", NULL, proc_ignore, 1, (int (*)())class_maint_cmd,
                    FindMaintCommand, class_maint_help,
                    "Commands for use by debugger maintainers.");
    AddClassCommand("set", NULL, proc_ignore, 1, (int (*)())class_set_cmd,
                    FindSetCommand, class_set_help,
                    "Set information about the debugger environment.");
    AddClassCommand("show", NULL, proc_ignore, 1, (int (*)())class_show_cmd,
                    FindShowCommand, class_show_help,
                    "Show information about the debugger environment.");
    AddClassCommand("unset", NULL, proc_ignore, 1, (int (*)())class_unset_cmd,
                    FindUnsetCommand, class_unset_help,
                    "Unset information in the debugger environment.");
    AddClassCommand("usage", NULL, proc_ignore, 1, (int (*)())class_usage_cmd,
                    FindUsageCommand, class_usage_help,
                    "Display usage information about script mode commands.");

    // Set class commands
    AddSetCommand("print", proc_ignore, 1, (int (*)())class_set_print_cmd,
                  class_set_print_help,
                  "Set information about how things get printed.");

    // Show class commands
    AddShowCommand("print", proc_ignore, 1, (int (*)())class_show_print_cmd,
                   class_show_print_help,
                   "Show information about how things get printed.");

    // Maint class commands
    AddMaintCommand("set", proc_ignore, 1, (int (*)())class_maint_set_cmd,
                    class_maint_set_help,
                    "Set information about the debugger environment.");
    AddMaintCommand("show", proc_ignore, 1, (int (*)())class_maint_show_cmd,
                    class_maint_show_help,
                    "Show information about the debugger environment.");
}

void DestroyCmd_class(void) {
    DestroyCommandList(InfoCmds);
    DestroyCommandList(MaintCmds);
    DestroyCommandList(MaintSetCmds);
    DestroyCommandList(MaintShowCmds);
    DestroyCommandList(SetCmds);
    DestroyCommandList(SetPrintCmds);
    DestroyCommandList(ShowCmds);
    DestroyCommandList(ShowPrintCmds);
    DestroyCommandList(UnsetCmds);
}
