/*
 * NVIDIA CUDA Debugger CUDA-GDB Modified by Allinea Software.
 * NVIDIA CUDA Debugger CUDA-GDB 
 * NVIDIA CUDA Debugger CUDA-GDB Copyright (C) Allinea Software Ltd. 2013
 * NVIDIA CUDA Debugger CUDA-GDB Copyright (C) 2007-2012 NVIDIA Corporation
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

#ifndef _CUDA_OPTIONS_H
#define _CUDA_OPTIONS_H 1

#include "cudadebugger.h"

void cuda_options_initialize (void);

int  cuda_options_debug_general (void);
bool cuda_options_debug_notifications (void);
bool cuda_options_debug_textures (void);
bool cuda_options_debug_libcudbg (void);
bool cuda_options_debug_siginfo (void);
bool cuda_options_debug_api (void);
bool cuda_options_memcheck (void);
bool cuda_options_coalescing (void);
bool cuda_options_break_on_launch_application(void);
bool cuda_options_break_on_launch_system (void);
bool cuda_options_disassemble_from_device_memory (void);
bool cuda_options_disassemble_from_elf_image (void);
bool cuda_options_hide_internal_frames (void);
bool cuda_options_show_kernel_events (void);
bool cuda_options_show_context_events (void);
bool cuda_options_launch_blocking (void);
bool cuda_options_thread_selection_logical (void);
bool cuda_options_thread_selection_physical (void);
bool cuda_options_api_failures_ignore (void);
bool cuda_options_api_failures_stop (void);
bool cuda_options_api_failures_hide (void);
void cuda_options_disable_break_on_launch (void);
bool cuda_options_notify_youngest (void);
bool cuda_options_notify_random (void);

const char* cuda_options_stepping (void);
extern const char cuda_stepping_warp[];
extern const char cuda_stepping_block[];
extern const char cuda_stepping_kernel[];
extern const char cuda_stepping_device[];
extern const char cuda_stepping_sm[];
extern const char cuda_stepping_all[];

#endif

