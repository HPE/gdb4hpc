--[[

    Module gdb4hpc

    (C) COPYRIGHT CRAY INC.
    UNPUBLISHED PROPRIETARY INFORMATION.
    ALL RIGHTS RESERVED.

]]--

-- local vars: define & assign --

-- template variables ----------------------------------------------------------
local INSTALL_ROOT       = "[@%PREFIX_PATH%@]"
local MOD_LEVEL          = "[@%MODULE_VERSION%@]"
--------------------------------------------------------------------------------

local NICKNAME  = "gdb4hpc"
local PE_DIR    = INSTALL_ROOT .. "/" .. NICKNAME .. "/" .. MOD_LEVEL

 -- module release info variables
local REL_FILE            = PE_DIR .. "/release_info"
local rel_info            = ""
if isFile(REL_FILE) then
    local f = io.open(REL_FILE, "r")
    local data = f:read("*all")
    f:close()
    if data ~= nil then rel_info = data end
end

 -- standered Lmod functions --

help ([[

The modulefile defines the system paths and
variables for the product gdb4hpc.

]] .. rel_info .. "\n" .. [[

===================================================================
To re-display ]] .. tostring(myModuleName()) .. "/" .. MOD_LEVEL .. [[ release information,
type:    less ]] .. REL_FILE .. "\n" .. [[
===================================================================

]])

whatis("Loads the Cray Line Mode Parallel Debugger.")

 -- environment modifications --

setenv (           "GDB4HPC_LEVEL",              MOD_LEVEL                   )
setenv (           "GDB4HPC_BASEDIR",            PE_DIR                      )
setenv (           "PE_GDB4HPC_MODULE_NAME",     myModuleName()              )
setenv (           "GDB4HPC_VERSION",            MOD_LEVEL                   )
setenv (           "GDB4HPC_INSTALL_DIR",        PE_DIR                      )

append_path   (    "PE_PRODUCT_LIST",            "GDB4HPC"                   )

prepend_path  (    "LD_LIBRARY_PATH",            PE_DIR .. "/lib"            )
prepend_path  (    "PATH",                       PE_DIR .. "/bin"            )
prepend_path  (    "MANPATH",                    PE_DIR .. "/man"            )
