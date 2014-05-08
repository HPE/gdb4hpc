/*
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


#ifndef _CUDA_KERNEL_H
#define _CUDA_KERNEL_H 1

#include "cuda-defs.h"

uint64_t        kernel_get_id             (kernel_t kernel);
uint32_t        kernel_get_dev_id         (kernel_t kernel);
uint32_t        kernel_get_grid_id        (kernel_t kernel);
const char*     kernel_get_name           (kernel_t kernel);
uint64_t        kernel_get_virt_code_base (kernel_t kernel);
context_t       kernel_get_context        (kernel_t kernel);
module_t        kernel_get_module         (kernel_t kernel);
CuDim3          kernel_get_grid_dim       (kernel_t kernel);
CuDim3          kernel_get_block_dim      (kernel_t kernel);
const char*     kernel_get_dimensions     (kernel_t kernel);
CUDBGKernelType kernel_get_type           (kernel_t kernel);
bool            kernel_has_launched       (kernel_t kernel);
bool            kernel_is_present         (kernel_t kernel);

uint32_t        kernel_compute_sms_mask   (kernel_t kernel);
void            kernel_load_elf_images    (kernel_t kernel);
void            kernel_print              (kernel_t kernel);
const char*     kernel_disassemble        (kernel_t kernel, uint64_t pc,
                                           uint32_t *inst_size);


kernels_t kernels_new                     (uint32_t dev_id);
void      kernels_delete                  (kernels_t kernels);
uint32_t  kernels_get_dev_id              (kernels_t kernels);
uint32_t  kernels_get_num_kernels         (kernels_t kernels);
uint32_t  kernels_get_num_present_kernels (kernels_t kernels);
void      kernels_print                   (kernels_t kernels);

kernel_t  kernels_find_kernel_by_grid_id (kernels_t kernels, uint64_t
                                          grid_id);
void      kernels_remove_kernels_for_module  (kernels_t kernels,
                                              module_t module);

void      kernels_update_kernels   (kernels_t kernels);
void      kernels_terminate_kernel (kernels_t kernels, kernel_t kernel);
void      kernels_start_kernel     (kernels_t kernels, uint64_t grid_id,
                                    uint64_t virt_code_base,
                                    uint64_t context_id, uint64_t module_id,
                                    CuDim3 grid_dim, CuDim3 block_dim,
                                    CUDBGKernelType type);

uint64_t  cuda_latest_launched_kernel_id (void);

#endif
