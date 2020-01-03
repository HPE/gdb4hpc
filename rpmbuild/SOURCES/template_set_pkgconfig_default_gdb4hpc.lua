--[[

    file  set_pkgconfig_default_cray-cti_version.lua
    Module gdb4hpc package default script for craype

    Copyright 2019 Cray Inc. All Rights Reserved.

]]--


-- local vars: define & assign --


-- template variables ----------------------------------------------------------
local INSTALL_ROOT   = "[@%PREFIX_PATH%@]"
local MOD_LEVEL      = "[@%MODULE_VERSION%@]"
--------------------------------------------------------------------------------

local NICKNAME       = "gdb4hpc"

local PE_PRODUCT_DIR = INSTALL_ROOT .. NICKNAME .. "/" .. MOD_LEVEL


-- environment modifications --


prepend_path (    "PE_PKG_CONFIG_PATH",    PE_PRODUCT_DIR .. "/lib/pkgconfig"    )
