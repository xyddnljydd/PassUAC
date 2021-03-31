

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for boo.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __boo_h_h__
#define __boo_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __boo_INTERFACE_DEFINED__
#define __boo_INTERFACE_DEFINED__

/* interface boo */
/* [version][uuid] */ 

typedef struct _MONITOR_POINT
    {
    long MonitorLeft;
    long MonitorRight;
    } 	MONITOR_POINT;

typedef struct _APP_STARTUP_INFO
    {
    wchar_t *lpszTitle;
    long dwX;
    long dwY;
    long dwXSize;
    long dwYSize;
    long dwXCountChars;
    long dwYCountChars;
    long dwFillAttribute;
    long dwFlags;
    short wShowWindow;
    struct _MONITOR_POINT MonitorPoint;
    } 	APP_STARTUP_INFO;

typedef struct _APP_PROCESS_INFORMATION
    {
    unsigned __int3264 ProcessHandle;
    unsigned __int3264 ThreadHandle;
    long ProcessId;
    long ThreadId;
    } 	APP_PROCESS_INFORMATION;

/* [async] */ void  RAiLaunchAdminProcess( 
    /* [in] */ PRPC_ASYNC_STATE RAiLaunchAdminProcess_AsyncHandle,
    handle_t hBinding,
    /* [string][unique][in] */ wchar_t *ExecutablePath,
    /* [string][unique][in] */ wchar_t *CommandLine,
    /* [in] */ long StartFlags,
    /* [in] */ long CreationFlags,
    /* [string][in] */ wchar_t *CurrentDirectory,
    /* [string][in] */ wchar_t *WindowStation,
    /* [in] */ struct _APP_STARTUP_INFO *StartupInfo,
    /* [in] */ unsigned __int3264 hWnd,
    /* [in] */ long Timeout,
    /* [out] */ struct _APP_PROCESS_INFORMATION *ProcessInformation,
    /* [out] */ long *ElevationType);



extern RPC_IF_HANDLE boo_v1_0_c_ifspec;
extern RPC_IF_HANDLE boo_v1_0_s_ifspec;
#endif /* __boo_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


