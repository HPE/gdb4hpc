/*
 Copyright (C) 2012 by Intel Corporation.
*/

/*myo_dbl.h*/

#ifndef MYO_DBL_H
#define MYO_DBL_H

#define _MYOD_error_code int
#define _MYOD_node_handle int
#define _MYOD_VA void * /*this is for address in different process*/
#define _MYOD_size_t int
#define _MYOD_byte unsigned char
#define _MYOD_string char *
#define _MYOD_TargetAddress _MYOD_VA
#define _MYOD_Version int

#define MYOD_ERROR -1
#define MYOD_ERR_MY_NOT_OWN  1
#define MYOD_ERR_OUR_NOT_OWN  2
#define MYOD_ERR_NOT_OWN 3
#define MYOD_ERR_NONSHARED 4
#define MYOD_WARN_READONLY 5
#define MYOD_SUCCESS 0



typedef enum DbgAccessEnumTagType {
    MYO_READONLY = 1,
    MYO_READWRITE
}DbgAccessType;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WINDOWS
#       if defined _EXPORT_SYMBOLS
#          define MYOACCESSAPI __declspec(dllexport)
#       elif defined _IMPORT_SYMBOLS
#          define MYOACCESSAPI __declspec(dllimport)
#       else
#          define MYOACCESSAPI /* nothing */
#       endif
#else
#       define MYOACCESSAPI /* nothing */
#endif

/*callback type defs */
typedef	_MYOD_error_code (*_MYOD_GetCurrentDebuggerCB)(
        _MYOD_node_handle      *owner
	);

typedef _MYOD_error_code (*_MYOD_MemReadCB)(
        _MYOD_VA                dbgCtx,
        _MYOD_node_handle       owner,
        _MYOD_VA                address,
        _MYOD_size_t            bytes,
        _MYOD_byte*             buffer
        );

typedef	_MYOD_error_code (*_MYOD_MemWriteCB)(
        _MYOD_VA                dbgCtx,
        _MYOD_node_handle       owner,
	_MYOD_VA		address,
	_MYOD_size_t	        bytes,
	_MYOD_byte*		buffer
	);

typedef	_MYOD_error_code (*_MYOD_GetSymbolAddressCB)(
       _MYOD_VA                 dbgCtx,
       _MYOD_node_handle	owner,
       const _MYOD_string	name,
       _MYOD_VA 	       *address
	);

typedef _MYOD_error_code (*_MYOD_initRuntime)(
      _MYOD_VA                    dbgCtx,
      _MYOD_Version              *version,
      _MYOD_MemReadCB             readCB,
      _MYOD_MemWriteCB            writeCB,
      _MYOD_GetSymbolAddressCB    symbolCB,
      _MYOD_string                signalSEGVtlsSymbolName,
      _MYOD_TargetAddress        *sharedAddressBase,
      _MYOD_TargetAddress        *sharedAddressEnd

);

typedef _MYOD_error_code (*_MYOD_bindOwner)(_MYOD_node_handle owner);

typedef _MYOD_error_code (*_MYOD_checkAddress)(
    _MYOD_VA              dbgCtx,
    _MYOD_VA              address,
    _MYOD_size_t          size,
    _MYOD_size_t         *ownedBytes,
    _MYOD_node_handle    *owner,
    DbgAccessType         mode);

typedef const char** (*_MYOD_getSymbolList)();

typedef struct{
    _MYOD_GetCurrentDebuggerCB getCurrentDebugger;
    _MYOD_MemReadCB read;
    _MYOD_MemWriteCB write;
    _MYOD_GetSymbolAddressCB getSymbolAddr;
} DBG_CB;

MYOACCESSAPI const char** getSymbolList();
/*MYO DBL APIs*/
MYOACCESSAPI _MYOD_error_code initRuntime(
      _MYOD_VA                    dbgCtx,
      _MYOD_Version              *version,
      _MYOD_MemReadCB             readCB,
      _MYOD_MemWriteCB            writeCB,
      _MYOD_GetSymbolAddressCB    symbolCB,
      _MYOD_string                signalSEGVtlsSymbolName, /*out parameter, tell debugger MYO tls var name*/
      _MYOD_TargetAddress        *sharedAddressBase, /*out parameter, tell debugger MYO shared addr range*/
      _MYOD_TargetAddress        *sharedAddressEnd /*out parameter, tell debugger MYO shared addr range*/

);
MYOACCESSAPI _MYOD_error_code bindOwner(_MYOD_node_handle owner);
MYOACCESSAPI _MYOD_error_code unBindOwner(_MYOD_node_handle owner);

MYOACCESSAPI _MYOD_error_code checkAddress(
    _MYOD_VA              dbgCtx,
    _MYOD_VA              address,
    _MYOD_size_t          size,
    _MYOD_size_t         *ownedBytes,
    _MYOD_node_handle    *owner,
    DbgAccessType         mode
);

#ifdef __cplusplus
}
#endif

#endif /*MYO_DBL_H*/



