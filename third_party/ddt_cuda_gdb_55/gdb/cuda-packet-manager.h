/*
 * NVIDIA CUDA Debugger CUDA-GDB Copyright (C) 2007-2013 NVIDIA Corporation
 * Written by CUDA-GDB team at NVIDIA <cudatools@nvidia.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CUDA_PACKET_MANAGER_H
#define _CUDA_PACKET_MANAGER_H 1

#include "cudadebugger.h"

typedef enum {
    /* api */
    RESUME_DEVICE,
    SUSPEND_DEVICE,
    SINGLE_STEP_WARP,
    SET_BREAKPOINT,
    UNSET_BREAKPOINT,
    READ_GRID_ID,
    READ_BLOCK_IDX,
    READ_THREAD_IDX,
    READ_BROKEN_WARPS,
    READ_VALID_WARPS,
    READ_VALID_LANES,
    READ_ACTIVE_LANES,
    READ_CODE_MEMORY,
    READ_CONST_MEMORY,
    READ_GLOBAL_MEMORY,
    READ_PINNED_MEMORY,
    READ_PARAM_MEMORY,
    READ_SHARED_MEMORY,
    READ_TEXTURE_MEMORY,
    READ_TEXTURE_MEMORY_BINDLESS,
    READ_LOCAL_MEMORY,
    READ_REGISTER,
    READ_PC,
    READ_VIRTUAL_PC,
    READ_LANE_EXCEPTION,
    READ_CALL_DEPTH,
    READ_SYSCALL_CALL_DEPTH,
    READ_VIRTUAL_RETURN_ADDRESS,
    WRITE_GLOBAL_MEMORY,
    WRITE_PINNED_MEMORY,
    WRITE_PARAM_MEMORY,
    WRITE_SHARED_MEMORY,
    WRITE_LOCAL_MEMORY,
    WRITE_REGISTER,
    IS_DEVICE_CODE_ADDRESS,
    DISASSEMBLE,
    MEMCHECK_READ_ERROR_ADDRESS,
    GET_NUM_DEVICES,
    GET_GRID_STATUS,
    GET_GRID_INFO,
    GET_HOST_ADDR_FROM_DEVICE_ADDR,

    /* notification */
    NOTIFICATION_ANALYZE,
    NOTIFICATION_PENDING,
    NOTIFICATION_RECEIVED,
    NOTIFICATION_ALIASED_EVENT,
    NOTIFICATION_MARK_CONSUMED,
    NOTIFICATION_CONSUME_PENDING,

    /* event */
    QUERY_SYNC_EVENT,
    QUERY_ASYNC_EVENT,
    ACK_SYNC_EVENTS,

    /* other */
    UPDATE_GRID_ID_IN_SM,
    UPDATE_BLOCK_IDX_IN_SM,
    UPDATE_THREAD_IDX_IN_WARP,
    INITIALIZE_TARGET,
    QUERY_DEVICE_SPEC,
    QUERY_TRACE_MESSAGE,
    CHECK_PENDING_SIGINT,
    API_INITIALIZE,
    API_FINALIZE,
    CLEAR_ATTACH_STATE,
    REQUEST_CLEANUP_ON_DETACH,
    SET_OPTION,
    SET_ASYNC_LAUNCH_NOTIFICATIONS,
} cuda_packet_type_t;

extern int hex2bin (const char *hex, gdb_byte *bin, int count);
extern int bin2hex (const gdb_byte *bin, char *hex, int count);

void alloc_cuda_packet_buffer (void);
void free_cuda_packet_buffer (void *unused);

/* Device Execution Control */
CUDBGResult cuda_remote_api_suspend_device (uint32_t dev);
CUDBGResult cuda_remote_api_resume_device (uint32_t dev);
CUDBGResult cuda_remote_api_single_step_warp (uint32_t dev, uint32_t sm, uint32_t wp, uint64_t *warp_mask);

/* Breakpoints */
CUDBGResult cuda_remote_api_set_breakpoint (uint32_t dev, uint64_t addr);
CUDBGResult cuda_remote_api_unset_breakpoint (uint32_t dev, uint64_t addr);

/* Device State Inspection */
CUDBGResult cuda_remote_api_read_grid_id (uint32_t dev_id, uint32_t sm_id, uint32_t warp_id, uint64_t *grid_id);
CUDBGResult cuda_remote_api_read_block_idx (uint32_t dev_id, uint32_t sm_id, uint32_t warp_id, CuDim3 *block_idx);
CUDBGResult cuda_remote_api_read_thread_idx (uint32_t dev_id, uint32_t sm_id, uint32_t warp_id, uint32_t lane_id, CuDim3 *thread_idx);
CUDBGResult cuda_remote_api_read_broken_warps (uint32_t dev, uint32_t sm, uint64_t *broken_warps_mask);
CUDBGResult cuda_remote_api_read_valid_warps (uint32_t dev_id, uint32_t sm_id, uint64_t *valid_warps_mask);
CUDBGResult cuda_remote_api_read_valid_lanes (uint32_t dev_id, uint32_t sm_id, uint32_t wp_id, uint32_t *valid_lanes_mask);
CUDBGResult cuda_remote_api_read_active_lanes (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t *active_lanes);
CUDBGResult cuda_remote_api_read_code_memory (uint32_t dev, uint64_t addr, void *buf, uint32_t sz);
CUDBGResult cuda_remote_api_read_const_memory (uint32_t dev, uint64_t addr, void *buf, uint32_t sz);
CUDBGResult cuda_remote_api_read_global_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint64_t addr, void *buf, uint32_t sz);
CUDBGResult cuda_remote_api_read_pinned_memory (uint64_t addr, void *buf, uint32_t sz); 
CUDBGResult cuda_remote_api_read_param_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint64_t addr, void *buf, uint32_t sz);
CUDBGResult cuda_remote_api_read_shared_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint64_t addr, void *buf, uint32_t sz);
CUDBGResult cuda_remote_api_read_texture_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t id, uint32_t dim, uint32_t *coords, void *buf, uint32_t sz);
CUDBGResult cuda_remote_api_read_texture_memory_bindless (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t tex_symtab_index,
                                               uint32_t dim, uint32_t *coords, void *buf, uint32_t sz);
CUDBGResult cuda_remote_api_read_local_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint64_t addr, void *buf, uint32_t sz);
CUDBGResult cuda_remote_api_read_register (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, int regno, uint32_t *value);
CUDBGResult cuda_remote_api_read_pc (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint64_t *pc);
CUDBGResult cuda_remote_api_read_virtual_pc (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint64_t *virtual_pc);
CUDBGResult cuda_remote_api_read_lane_exception (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, CUDBGException_t *exception);
CUDBGResult cuda_remote_api_read_call_depth (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint32_t *call_depth);
CUDBGResult cuda_remote_api_read_syscall_call_depth (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint32_t *syscall_call_depth);
CUDBGResult cuda_remote_api_read_virtual_return_address (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint32_t level, uint64_t *ra);

/* Device State Alteration */
CUDBGResult cuda_remote_api_write_global_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint64_t addr, const void *buf, uint32_t sz);
CUDBGResult cuda_remote_api_write_pinned_memory (uint64_t addr, const void *buf, uint32_t sz);
CUDBGResult cuda_remote_api_write_param_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint64_t addr, const void *buf, uint32_t sz);
CUDBGResult cuda_remote_api_write_shared_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint64_t addr, const void *buf, uint32_t sz);
CUDBGResult cuda_remote_api_write_local_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint64_t addr, const void *buf, uint32_t sz);
CUDBGResult cuda_remote_api_write_register (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, int regno, uint32_t val);

/* DWARF-related routines */
CUDBGResult cuda_remote_api_disassemble (uint32_t dev, uint64_t addr, uint32_t *inst_size, char *buf, uint32_t buf_size);
CUDBGResult cuda_remote_api_is_device_code_address (uint64_t addr, bool *is_device_address);

/* Grid Properties */
CUDBGResult cuda_remote_api_get_grid_status (uint32_t dev, uint64_t grid_id, CUDBGGridStatus *status);
CUDBGResult cuda_remote_api_get_grid_info (uint32_t dev, uint64_t grid_id, CUDBGGridInfo *info);

/* Device Properties */
CUDBGResult cuda_remote_api_get_num_devices (uint32_t *dev);
void cuda_remote_query_device_spec (uint32_t dev_id, uint32_t *num_sms, uint32_t *num_warps,
                                    uint32_t *num_lanes, uint32_t *num_registers, char **dev_type, char **sm_type);

/* Notifications */
bool cuda_remote_notification_pending (void);
bool cuda_remote_notification_received (void);
bool cuda_remote_notification_aliased_event (void);
void cuda_remote_notification_analyze (void);
void cuda_remote_notification_mark_consumed (void);
void cuda_remote_notification_consume_pending (void);

/* Events */
CUDBGResult cuda_remote_api_acknowledge_sync_events (void);
bool cuda_remote_query_sync_events (void);
bool cuda_remote_query_async_events (void);

/* Memcheck related */
CUDBGResult cuda_remote_api_memcheck_read_error_address (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln,
                                                  uint64_t *address, ptxStorageKind *storage);

/* Others */
void cuda_remote_update_grid_id_in_sm (uint32_t dev, uint32_t sm);
void cuda_remote_update_block_idx_in_sm (uint32_t dev, uint32_t sm);
void cuda_remote_update_thread_idx_in_warp (uint32_t dev, uint32_t sm, uint32_t wp);
void cuda_remote_initialize (CUDBGResult *get_debugger_api_res, CUDBGResult *set_callback_api_res, 
                             CUDBGResult *initialize_api_res, bool *cuda_initialized, 
                             bool *cuda_debugging_enabled, bool *driver_is_compatiable);
bool cuda_remote_check_pending_sigint (void);
CUDBGResult cuda_remote_api_initialize (void);
CUDBGResult cuda_remote_api_finalize (void);
CUDBGResult cuda_remote_api_clear_attach_state (void);
CUDBGResult cuda_remote_api_request_cleanup_on_detach (void);
CUDBGResult cuda_remote_api_get_host_addr_from_device_addr (uint32_t dev, uint64_t addr, uint64_t *hostaddr);
CUDBGResult cuda_remote_api_set_kernel_launch_notification_mode (CUDBGKernelLaunchNotifyMode);
void cuda_remote_set_option (void);
void cuda_remote_query_trace_message (void);
#endif
