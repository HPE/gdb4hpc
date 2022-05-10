/******************************************************************************\
 * help_dict.h - Command help string defintion dictionary interface.
 *
 * Copyright 2011-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifndef _HELP_DICT_H
#define _HELP_DICT_H

/**********************************************
 * Regular command help definitions
 **********************************************/
extern const char* const cmd_attach_help[];
extern const char* const cmd_assign_help[];
extern const char* const cmd_backtrace_help[];
extern const char* const cmd_break_help[];
extern const char* const cmd_catch_help[];
extern const char* const cmd_checkpoint_help[];
extern const char* const cmd_compare_help[];
extern const char* const cmd_continue_help[];
extern const char* const cmd_cuda_help[];
extern const char* const cmd_run_help[];
extern const char* const cmd_defset_help[];
extern const char* const cmd_delete_help[];
extern const char* const cmd_directmode_help[];
extern const char* const cmd_disable_help[];
extern const char* const cmd_down_help[];
extern const char* const cmd_enable_help[];
extern const char* const cmd_finish_help[];
extern const char* const cmd_focus_help[];
extern const char* const cmd_frame_help[];
extern const char* const cmd_halt_help[];
extern const char* const cmd_help_help[];
extern const char* const cmd_launch_help[];
extern const char* const cmd_run_help[];
extern const char* const cmd_kill_help[];
extern const char* const cmd_list_help[];
extern const char* const cmd_next_help[];
extern const char* const cmd_ompd_help[];
extern const char* const cmd_print_help[];
extern const char* const cmd_ptype_help[];
extern const char* const cmd_quit_help[];
extern const char* const cmd_release_help[];
extern const char* const cmd_restart_help[];
extern const char* const cmd_session_help[];
extern const char* const cmd_source_help[];
extern const char* const cmd_step_help[];
extern const char* const cmd_tbreak_help[];
extern const char* const cmd_thread_help[];
extern const char* const cmd_up_help[];
extern const char* const cmd_viewset_help[];
extern const char* const cmd_watch_help[];
extern const char* const cmd_whatis_help[];
extern const char* const cmd_blockmap_help[];
extern const char* const cmd_build_help[];
extern const char* const cmd_start_help[];
extern const char* const cmd_stop_help[];

extern const char* const cmd_blockmap_alt_dimension_help[];
extern const char* const cmd_blockmap_alt_distribute_help[];
extern const char* const cmd_blockmap_alt_proc_grid_help[];
extern const char* const cmd_blockmap_alt_dim_order_help[];
extern const char* const cmd_blockmap_alt_end_help[];
extern const char* const cmd_script_alt_assert_help[];
extern const char* const cmd_script_alt_end_help[];
extern const char* const cmd_script_alt_set_help[];
extern const char* const cmd_script_alt_show_help[];

/**********************************************
 * Command class help definitions
 **********************************************/
extern const char* const class_info_help[];
extern const char* const class_maint_help[];
extern const char* const class_maint_set_help[];
extern const char* const class_maint_show_help[];
extern const char* const class_set_help[];
extern const char* const class_set_print_help[];
extern const char* const class_set_break_help[];
extern const char* const class_show_help[];
extern const char* const class_show_print_help[];
extern const char* const class_show_break_help[];
extern const char* const class_unset_help[];
extern const char* const class_usage_help[];

/**********************************************
 * Command class sub-command help definitions
 **********************************************/
/*
 * Class info subcommands
 */
extern const char* const class_info_subcmd_application_help[];
extern const char* const class_info_subcmd_breakpoints_help[];
extern const char* const class_info_subcmd_checkpoints_help[];
extern const char* const class_info_subcmd_locals_help[];
extern const char* const class_info_subcmd_scope_help[];
extern const char* const class_info_subcmd_sources_help[];
extern const char* const class_info_subcmd_status_help[];
extern const char* const class_info_subcmd_threads_help[];
extern const char* const class_info_subcmd_blockmap_help[];
extern const char* const class_info_subcmd_diff_help[];
extern const char* const class_info_subcmd_script_help[];

/*
 * Class maint subcommands
 */
extern const char* const class_maint_subcmd_echo_help[];
extern const char* const class_maint_subcmd_sleep_help[];
extern const char* const class_maint_subcmd_direct_help[];
extern const char* const class_maint_subcmd_system_help[];

/*
 * Class set subcommands
 */
extern const char* const class_set_subcmd_directory_help[];
extern const char* const class_set_subcmd_error_help[];
extern const char* const class_set_subcmd_environment_help[];
extern const char* const class_set_subcmd_function_help[];
extern const char* const class_set_subcmd_scheduler_locking_help[];
extern const char* const class_set_subcmd_non_stop_help[];
extern const char* const set_subcmd_args_help[];

/*
 * Class set print subcommands
 */
extern const char* const class_set_print_subcmd_entryframe_help[];
extern const char* const class_set_print_subcmd_sourceonstep_help[];
extern const char* const class_set_print_subcmd_elements_help[];

/*
 * Class set breakpoint subcommands
 */
extern const char* const class_set_break_subcmd_pending_help[];

/*
 * Class maint set subcommands
 */
extern const char* const class_maint_set_subcmd_earlyentry_help[];
extern const char* const class_maint_set_subcmd_unsafe_help[];
extern const char* const class_maint_set_subcmd_sync_help[];

/*
 * Class show subcommands
 */
extern const char* const class_show_subcmd_directories_help[];
extern const char* const class_show_subcmd_error_help[];
extern const char* const class_show_subcmd_function_help[];
extern const char* const class_show_subcmd_environment_help[];
extern const char* const class_show_subcmd_version_help[];
extern const char* const class_show_subcmd_scheduler_locking_help[];
extern const char* const class_show_subcmd_non_stop_help[];
extern const char* const show_subcmd_args_help[];

/*
 * Class show print subcommands
 */
extern const char* const class_show_print_subcmd_entryframe_help[];
extern const char* const class_show_print_subcmd_sourceonstep_help[];
extern const char* const class_show_print_subcmd_elements_help[];

/*
 * Class show break subcommands
 */
extern const char* const class_show_break_subcmd_pending_help[];

/*
 * Class maint show subcommands
 */
extern const char* const class_maint_show_subcmd_earlyentry_help[];
extern const char* const class_maint_show_subcmd_unsafe_help[];
extern const char* const class_maint_show_subcmd_sync_help[];

/*
 * Class unset subcommands
 */
extern const char* const class_unset_subcmd_environment_help[];

/*
 * Class usage subcommands
 */
extern const char* const class_usage_subcmd_assert_help[];
extern const char* const class_usage_subcmd_decomposition_help[];

/*
 * Dummy help - used when a help cmd isn't actually required
 */
extern const char* const dummy_help[];

#endif /* _HELP_DICT_H */
