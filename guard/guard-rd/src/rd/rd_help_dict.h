/******************************************************************************\
 * rd_help_dict.h - RD command help string defintion dictionary interface.
 *
 * © 2011-2013 Cray Inc.  All Rights Reserved.
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
 
#ifndef _RD_HELP_DICT_H
#define _RD_HELP_DICT_H

/**********************************************
 * Regular rd command help definitions
 **********************************************/
extern const char *cmd_blockmap_help[];
extern const char *cmd_build_help[];
extern const char *cmd_start_help[];
extern const char *cmd_stop_help[];

/**********************************************
 * Alt rd command help definitions
 **********************************************/
extern const char *cmd_blockmap_alt_dimension_help[];
extern const char *cmd_blockmap_alt_distribute_help[];
extern const char *cmd_blockmap_alt_proc_grid_help[];
extern const char *cmd_blockmap_alt_dim_order_help[];
extern const char *cmd_blockmap_alt_end_help[];
extern const char *cmd_graph_alt_assert_help[]; 
extern const char *cmd_graph_alt_assign_help[];
extern const char *cmd_graph_alt_end_help[];

/**********************************************
 * Command class rd sub-command help definitions
 **********************************************/
/*
 * Class info subcommands
 */
extern const char *class_info_subcmd_blockmap_help[];
extern const char *class_info_subcmd_diff_help[];
extern const char *class_info_subcmd_build_help[];
extern const char *class_info_subcmd_script_help[];

/*
 * Class maint subcommands
 */
extern const char *class_maint_subcmd_dump_help[];

/*
 * Class set subcommands
 */
extern const char *class_set_subcmd_dfmode_help[];
extern const char *class_set_subcmd_display_help[];
extern const char *class_set_subcmd_force_help[];
extern const char *class_set_subcmd_halt_help[];
extern const char *class_set_subcmd_logfile_help[];
extern const char *class_set_subcmd_logging_help[];
extern const char *class_set_subcmd_output_help[];
extern const char *class_set_subcmd_reduce_help[];
extern const char *class_set_subcmd_stop_help[];
extern const char *class_set_subcmd_p2p_help[];

/*
 * Class show subcommands
 */
extern const char *class_show_subcmd_dfmode_help[];
extern const char *class_show_subcmd_display_help[];
extern const char *class_show_subcmd_force_help[];
extern const char *class_show_subcmd_halt_help[];
extern const char *class_show_subcmd_logfile_help[];
extern const char *class_show_subcmd_logging_help[];
extern const char *class_show_subcmd_output_help[];
extern const char *class_show_subcmd_reduce_help[];
extern const char *class_show_subcmd_stop_help[];
extern const char *class_show_subcmd_p2p_help[];

/*
 * Class usage subcommands
 */
extern const char *class_usage_subcmd_assert_help[];
extern const char *class_usage_subcmd_assign_help[];
extern const char *class_usage_subcmd_decomposition_help[];

#endif /* _RD_HELP_DICT_H */
