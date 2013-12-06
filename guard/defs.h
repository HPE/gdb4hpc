/*
** Global definitions.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _DEFS_H
#define _DEFS_H

#include "version.h"

#ifdef MRNET
#define DBG_PROTO	"mrnet"
#else
#define DBG_PROTO	"socket" /* "socket" or "mrnet" */
#endif

// I don't think this is used anymore - pretty sure it related to the old
// starter architecture.
#define DBG_ARCH "alps"

#ifdef GDB_BACKEND
#define DBG_BACKEND	"gdb"
#elif defined(GDBMI_BACKEND)
#define DBG_BACKEND	"gdbmi"
#elif defined(IDB_BACKEND)
#define DBG_BACKEND	"idb"
#endif

#define	 GUARD_CFG_DIRECTORY		"GUARD_MRNET_CFG_DIRECTORY"
#define GUARD_MRNET_PORT			"GUARD_MRNET_PORT_BASE"
#define DEFAULT_MRNET_PORT_BASE	"20000"

#ifdef ZPL
#define LANG_ZPL        7
#define ZPL_TAG         "zcmap"
#endif /* ZPL */

#ifdef PERFLOG
#define DPERFLOG(s) s
#else /* PERFLOG */
#define DPERFLOG(s) /**/
#endif /* PERFLOG */

#ifdef BE_PERFLOG
#define DBEPERFLOG(s) s
#else /* BE_PERFLOG */
#define DBEPERFLOG(s) /**/
#endif /* BE_PERFLOG */

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__)
#else /* DEBUG */
#define DPRINT(...) /**/
#endif /* DEBUG */

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
#include        <values.h>

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

#define SOCKET		int
#define CLOSE_SOCKET(s)	(void)close(s)
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1

#endif /* !_DEFS_H */
