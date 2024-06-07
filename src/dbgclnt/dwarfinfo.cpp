/*********************************************************************************\
 * Copyright 2024 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#include <elfutils/libdw.h>
#include <elfutils/libdwfl.h>
#include <dwarf.h>

#include <stdexcept>

#include "dwarfinfo.h"

struct GetDieNameArgs
{
    int in_tag, in_attr;
    std::string out_name;
};

static int get_die_name_callback(Dwfl_Module *mod, void **, const char *, Dwarf_Addr, void *arg_void)
{
    auto args = (GetDieNameArgs*)arg_void;

    auto offset = Dwarf_Off{0};
    auto cu_die = dwfl_module_nextcu(mod, nullptr, &offset);

    do {
        auto child_die = Dwarf_Die{};
        if (dwarf_child(cu_die, &child_die) != 0) {
            continue;
        }

        do {
            if (dwarf_tag(&child_die) == args->in_tag) {

                Dwarf_Attribute attr;
                if (dwarf_attr(&child_die, args->in_attr, &attr) != nullptr) {

                    bool result;
                    if ((dwarf_formflag(&attr, &result) == 0) && result) {
                        if (auto func_name = dwarf_diename(&child_die)) {

                            args->out_name = func_name;
                            return DWARF_CB_OK;
                        }
                    }
                }
            }
        } while (dwarf_siblingof(&child_die, &child_die) == 0);
    } while ((cu_die = dwfl_module_nextcu(mod, cu_die, &offset)));

    return DWARF_CB_OK;
}

std::string DwarfInfo::getDieName(int tag, int attr)
{
    if (auto dwfl = (Dwfl*)m_dwfl) {

        auto args = GetDieNameArgs { .in_tag = tag, .in_attr = attr, .out_name = {} };

        if (dwfl_getmodules(dwfl, get_die_name_callback, (void*)&args, 0) != 0) {
            throw std::runtime_error("dwfl_getmodules failed:" + std::string{dwfl_errmsg(-1)});
        }

        return args.out_name;
        
    } else {
        throw std::logic_error("Called getDieName on uninitialized DwarfInfo");
    }
}

DwarfInfo::DwarfInfo(std::string const& binary)
{
    Dwfl_Callbacks callbacks = {
        .find_elf = dwfl_linux_proc_find_elf,
        .find_debuginfo = dwfl_standard_find_debuginfo,
        .section_address = {},
        .debuginfo_path = {}
    };

    auto dwfl = dwfl_begin(&callbacks);
    if (!dwfl) {
        throw std::runtime_error("dwfl_begin failed: " + std::string{dwfl_errmsg(-1)});
    }

    if (dwfl_report_offline(dwfl, binary.c_str(), binary.c_str(), -1) == nullptr) {
        throw std::runtime_error("dwfl_report_offline failed:" + std::string{dwfl_errmsg(-1)});
    }

    if (dwfl_report_end(dwfl, nullptr, nullptr) != 0) {
        throw std::runtime_error("dwfl_report_end failed:" + std::string{dwfl_errmsg(-1)});
    }

    m_dwfl = dwfl;
}

DwarfInfo::DwarfInfo(DwarfInfo&& expiring)
    : m_dwfl{expiring.m_dwfl}
{
    expiring.m_dwfl = nullptr;
}

DwarfInfo::~DwarfInfo()
{
    if (auto dwfl = (Dwfl*)m_dwfl) {
        dwfl_end(dwfl);
    }
}

std::string DwarfInfo::getMainSubprogram()
{
    return getDieName(DW_TAG_subprogram, DW_AT_main_subprogram);
}
