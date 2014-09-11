/******************************************************************************\
 * help_dict.h - Command help string defintion dictionary interface.
 *
 * © 2011-2014 Cray Inc.  All Rights Reserved.
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
 
#ifndef _HELP_DICT_H
#define _HELP_DICT_H

/**********************************************
 * Regular command help definitions
 **********************************************/
extern const char *cmd_attach_help[];
extern const char *cmd_assign_help[];
extern const char *cmd_backtrace_help[];
extern const char *cmd_break_help[];
extern const char *cmd_compare_help[];
extern const char *cmd_continue_help[];
extern const char *cmd_run_help[];
extern const char *cmd_defset_help[];
extern const char *cmd_delete_help[];
extern const char *cmd_directmode_help[];
extern const char *cmd_disable_help[];
extern const char *cmd_down_help[];
extern const char *cmd_enable_help[];
extern const char *cmd_finish_help[];
extern const char *cmd_focus_help[];
extern const char *cmd_frame_help[];
extern const char *cmd_halt_help[];
extern const char *cmd_help_help[];
extern const char *cmd_invoke_help[];
extern const char *cmd_kill_help[];
extern const char *cmd_list_help[];
extern const char *cmd_next_help[];
extern const char *cmd_print_help[];
extern const char *cmd_quit_help[];
extern const char *cmd_release_help[];
extern const char *cmd_session_help[];
extern const char *cmd_source_help[];
extern const char *cmd_step_help[];
extern const char *cmd_tbreak_help[];
extern const char *cmd_up_help[];
extern const char *cmd_viewset_help[];
extern const char *cmd_watch_help[];
extern const char *cmd_whatis_help[];

/**********************************************
 * Command class help definitions
 **********************************************/
extern const char *class_info_help[];
extern const char *class_maint_help[];
extern const char *class_maint_set_help[];
extern const char *class_maint_show_help[];
extern const char *class_set_help[];
extern const char *class_show_help[];
extern const char *class_unset_help[];
extern const char *class_usage_help[];

/**********************************************
 * Command class sub-command help definitions
 **********************************************/
/*
 * Class info subcommands
 */
extern const char *class_info_subcmd_application_help[];
extern const char *class_info_subcmd_breakpoints_help[];
extern const char *class_info_subcmd_locals_help[];
extern const char *class_info_subcmd_scope_help[];
extern const char *class_info_subcmd_sources_help[];
extern const char *class_info_subcmd_status_help[];

/*
 * Class maint subcommands
 */
extern const char *class_maint_subcmd_echo_help[];
extern const char *class_maint_subcmd_sleep_help[];
extern const char *class_maint_subcmd_direct_help[];
extern const char *class_maint_subcmd_system_help[];

/*
 * Class set subcommands
 */
extern const char *class_set_subcmd_directory_help[];
extern const char *class_set_subcmd_error_help[];
extern const char *class_set_subcmd_environment_help[];
extern const char *class_set_subcmd_function_help[];

/*
 * Class maint set subcommands
 */
extern const char *class_maint_set_subcmd_unsafe_help[];
extern const char *class_maint_set_subcmd_sync_help[];

/*
 * Class show subcommands
 */
extern const char *class_show_subcmd_directories_help[];
extern const char *class_show_subcmd_error_help[];
extern const char *class_show_subcmd_function_help[];
extern const char *class_show_subcmd_environment_help[];
extern const char *class_show_subcmd_version_help[];

/*
 * Class maint show subcommands
 */
extern const char *class_maint_show_subcmd_unsafe_help[];
extern const char *class_maint_show_subcmd_sync_help[];

/*
 * Class unset subcommands
 */
extern const char *class_unset_subcmd_environment_help[];

#endif /* _HELP_DICT_H */
