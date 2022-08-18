/*********************************************************************************\
 * Global definitions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/
#pragma once

#include "config.h"

#ifdef ZMQNET
#define DBG_PROTO "zmqnet"
#elif MRNET
#define DBG_PROTO "mrnet"
#else
#define DBG_PROTO "socket" /* "socket" or "mrnet" or "zmqnet" */
#endif

// I don't think this is used anymore - pretty sure it related to the old
// starter architecture.
#define DBG_ARCH "alps"

#define DBG_BACKEND "gdbmi"

#define GUARD_ENV_SETUP "GDB4HPC_IS_SETUP"
#define PROXY_ENV_SETUP "GDB4HPC_PROXY_IS_SETUP"
#define GUARD_CFG_DIRECTORY "GDB4HPC_CFG_DIRECTORY"
#define GUARD_INST_DIRECTORY "GDB4HPC_INSTALL_DIR"
#define LIBALPS_ENABLE_DSL "LIBALPS_ENABLE_DSL"
#define GUARD_LIBALPS_ENABLE_DSL "GDB4HPC_LIBALPS_ENABLE_DSL"
#define GUARD_MRNET_PORT "GUARD_MRNET_PORT_BASE"
#define GDB4HPC_DEFAULT_NUM_RANKS "GDB4HPC_DEFAULT_NUM_RANKS"
#define DEFAULT_MRNET_PORT_BASE "20000"
#define GDB4HPC_BE_SUPPRESSIONS "gdb4hpc_be_suppressions.txt"

#ifdef ZPL
#define LANG_ZPL 7
#define ZPL_TAG "zcmap"
#endif /* ZPL */

#ifdef PERFLOG
#define DPERFLOG(s) s
#else               /* PERFLOG */
#define DPERFLOG(s) /**/
#endif              /* PERFLOG */

#ifdef BE_PERFLOG
#define DBEPERFLOG(s) s
#else                 /* BE_PERFLOG */
#define DBEPERFLOG(s) /**/
#endif                /* BE_PERFLOG */

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__)
#else               /* DEBUG */
// Dummy DPRINT that will be optimized out, but still be syntax/type checked
#define DPRINT(...) while (0) fprintf(stderr, __VA_ARGS__)
#endif              /* DEBUG */

/*
** Include for MIN & MAX
*/
#if defined(sun)
#include <sys/sysmacros.h>
#elif defined(__sgi) || defined(__linux)
#include <sys/param.h>
#else
#ifndef MAX
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#endif /* MAX */

#ifndef MIN
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#endif /* MIN */
#endif

/* !WIN32 && !APPLE */
#include <values.h>

#ifdef CC_HAS_LONG_LONG
typedef long long int LONGEST;
#else
typedef long int LONGEST;
#endif /* CC_HAS_LONG_LONG */

#ifdef HAVE_LONG_DOUBLE
typedef long double DOUBLEST;
#else
typedef double DOUBLEST;
#endif /* HAVE_LONG_DOUBLE */

#define SOCKET int
#define CLOSE_SOCKET(s) (void)close(s)
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

// can hold maximum length of IPv4 address, hostname
#define MAXADDRLEN 15
#define MAXADDRLEN_FMT "15"

