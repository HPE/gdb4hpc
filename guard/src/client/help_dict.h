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
extern const char * const cmd_attach_help[];
extern const char * const cmd_assign_help[];
extern const char * const cmd_backtrace_help[];
extern const char * const cmd_break_help[];
extern const char * const cmd_compare_help[];
extern const char * const cmd_continue_help[];
extern const char * const cmd_run_help[];
extern const char * const cmd_defset_help[];
extern const char * const cmd_delete_help[];
extern const char * const cmd_directmode_help[];
extern const char * const cmd_disable_help[];
extern const char * const cmd_down_help[];
extern const char * const cmd_enable_help[];
extern const char * const cmd_finish_help[];
extern const char * const cmd_focus_help[];
extern const char * const cmd_frame_help[];
extern const char * const cmd_halt_help[];
extern const char * const cmd_help_help[];
extern const char * const cmd_invoke_help[];
extern const char * const cmd_kill_help[];
extern const char * const cmd_list_help[];
extern const char * const cmd_next_help[];
extern const char * const cmd_print_help[];
extern const char * const cmd_quit_help[];
extern const char * const cmd_release_help[];
extern const char * const cmd_session_help[];
extern const char * const cmd_source_help[];
extern const char * const cmd_step_help[];
extern const char * const cmd_tbreak_help[];
extern const char * const cmd_up_help[];
extern const char * const cmd_viewset_help[];
extern const char * const cmd_watch_help[];
extern const char * const cmd_whatis_help[];

/**********************************************
 * Command class help definitions
 **********************************************/
extern const char * const class_info_help[];
extern const char * const class_maint_help[];
extern const char * const class_maint_set_help[];
extern const char * const class_maint_show_help[];
extern const char * const class_set_help[];
extern const char * const class_show_help[];
extern const char * const class_unset_help[];
extern const char * const class_usage_help[];

/**********************************************
 * Command class sub-command help definitions
 **********************************************/
/*
 * Class info subcommands
 */
extern const char * const class_info_subcmd_application_help[];
extern const char * const class_info_subcmd_breakpoints_help[];
extern const char * const class_info_subcmd_locals_help[];
extern const char * const class_info_subcmd_scope_help[];
extern const char * const class_info_subcmd_sources_help[];
extern const char * const class_info_subcmd_status_help[];

/*
 * Class maint subcommands
 */
extern const char * const class_maint_subcmd_echo_help[];
extern const char * const class_maint_subcmd_sleep_help[];
extern const char * const class_maint_subcmd_direct_help[];
extern const char * const class_maint_subcmd_system_help[];

/*
 * Class set subcommands
 */
extern const char * const class_set_subcmd_directory_help[];
extern const char * const class_set_subcmd_error_help[];
extern const char * const class_set_subcmd_environment_help[];
extern const char * const class_set_subcmd_function_help[];

/*
 * Class maint set subcommands
 */
extern const char * const class_maint_set_subcmd_unsafe_help[];
extern const char * const class_maint_set_subcmd_sync_help[];

/*
 * Class show subcommands
 */
extern const char * const class_show_subcmd_directories_help[];
extern const char * const class_show_subcmd_error_help[];
extern const char * const class_show_subcmd_function_help[];
extern const char * const class_show_subcmd_environment_help[];
extern const char * const class_show_subcmd_version_help[];

/*
 * Class maint show subcommands
 */
extern const char * const class_maint_show_subcmd_unsafe_help[];
extern const char * const class_maint_show_subcmd_sync_help[];

/*
 * Class unset subcommands
 */
extern const char * const class_unset_subcmd_environment_help[];

#endif /* _HELP_DICT_H */
