#pragma once

typedef enum _PROCESSINFOCLASS {
    ProcessBasicInformation = 0,
    ProcessQuotaLimits = 1,
    ProcessIoCounters = 2,
    ProcessVmCounters = 3,
    ProcessTimes = 4,
    ProcessBasePriority = 5,
    ProcessRaisePriority = 6,
    ProcessDebugPort = 7,
    ProcessExceptionPort = 8,
    ProcessAccessToken = 9,
    ProcessLdtInformation = 10,
    ProcessLdtSize = 11,
    ProcessDefaultHardErrorMode = 12,
    ProcessIoPortHandlers = 13,
    ProcessPooledUsageAndLimits = 14,
    ProcessWorkingSetWatch = 15,
    ProcessUserModeIOPL = 16,
    ProcessEnableAlignmentFaultFixup = 17,
    ProcessPriorityClass = 18,
    ProcessWx86Information = 19,
    ProcessHandleCount = 20,
    ProcessAffinityMask = 21,
    ProcessPriorityBoost = 22,
    ProcessDeviceMap = 23,
    ProcessSessionInformation = 24,
    ProcessForegroundInformation = 25,
    ProcessWow64Information = 26,
    ProcessImageFileName = 27,
    ProcessLUIDDeviceMapsEnabled = 28,
    ProcessBreakOnTermination = 29,
    ProcessDebugObjectHandle = 30,
    ProcessDebugFlags = 31,
    ProcessHandleTracing = 32,
    ProcessIoPriority = 33,
    ProcessExecuteFlags = 34,
    ProcessTlsInformation = 35,
    ProcessCookie = 36,
    ProcessImageInformation = 37,
    ProcessCycleTime = 38,
    ProcessPagePriority = 39,
    ProcessInstrumentationCallback = 40,
    ProcessThreadStackAllocation = 41,
    ProcessWorkingSetWatchEx = 42,
    ProcessImageFileNameWin32 = 43,
    ProcessImageFileMapping = 44,
    ProcessAffinityUpdateMode = 45,
    ProcessMemoryAllocationMode = 46,
    ProcessGroupInformation = 47,
    ProcessTokenVirtualizationEnabled = 48,
    ProcessOwnerInformation = 49,
    ProcessWindowInformation = 50,
    ProcessHandleInformation = 51,
    ProcessMitigationPolicy = 52,
    ProcessDynamicFunctionTableInformation = 53,
    ProcessHandleCheckingMode = 54,
    ProcessKeepAliveCount = 55,
    ProcessRevokeFileHandles = 56,
    ProcessWorkingSetControl = 57,
    ProcessHandleTable = 58,
    ProcessCheckStackExtentsMode = 59,
    ProcessCommandLineInformation = 60,
    ProcessProtectionInformation = 61,
    ProcessMemoryExhaustion = 62,
    ProcessFaultInformation = 63,
    ProcessTelemetryIdInformation = 64,
    ProcessCommitReleaseInformation = 65,
    ProcessDefaultCpuSetsInformation = 66,
    ProcessAllowedCpuSetsInformation = 67,
    ProcessSubsystemProcess = 68,
    ProcessJobMemoryInformation = 69,
    ProcessInPrivate = 70,
    ProcessRaiseUMExceptionOnInvalidHandleClose = 71,
    ProcessIumChallengeResponse = 72,
    ProcessChildProcessInformation = 73,
    ProcessHighGraphicsPriorityInformation = 74,
    ProcessSubsystemInformation = 75,
    ProcessEnergyValues = 76,
    ProcessActivityThrottleState = 77,
    ProcessActivityThrottlePolicy = 78,
    ProcessWin32kSyscallFilterInformation = 79,
    ProcessDisableSystemAllowedCpuSets = 80,
    ProcessWakeInformation = 81,
    ProcessEnergyTrackingState = 82,
    ProcessManageWritesToExecutableMemory = 83,
    ProcessCaptureTrustletLiveDump = 84,
    ProcessTelemetryCoverage = 85,
    ProcessEnclaveInformation = 86,
    ProcessEnableReadWriteVmLogging = 87,
    ProcessUptimeInformation = 88,
    ProcessImageSection = 89,
    ProcessDebugAuthInformation = 90,
    ProcessSystemResourceManagement = 91,
    ProcessSequenceNumber = 92,
    ProcessLoaderDetour = 93,
    ProcessSecurityDomainInformation = 94,
    ProcessCombineSecurityDomainsInformation = 95,
    ProcessEnableLogging = 96,
    ProcessLeapSecondInformation = 97,
    ProcessFiberShadowStackAllocation = 98,
    ProcessFreeFiberShadowStackAllocation = 99,
    ProcessAltSystemCallInformation = 100,
    ProcessDynamicEHContinuationTargets = 101,
    MaxProcessInfoClass
} PROCESSINFOCLASS;

typedef  NTSTATUS(_stdcall* pNtQueryInformationProcess)(
    IN HANDLE ProcessHandle,
    IN PROCESSINFOCLASS InformationClass,
    OUT PVOID ProcessInformation,
    IN ULONG ProcessInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );
pNtQueryInformationProcess NtQueryInformationProcess = NULL;

typedef NTSTATUS(_stdcall* pNtRemoveProcessDebug)(
    IN HANDLE               ProcessHandle,
    IN HANDLE               DebugObjectHandle);
pNtRemoveProcessDebug NtRemoveProcessDebug;

typedef VOID(_stdcall* pDbgUiSetThreadDebugObject)(_In_ HANDLE DebugObject);
pDbgUiSetThreadDebugObject DbgUiSetThreadDebugObject = NULL;

typedef NTSTATUS(_stdcall* pNtDuplicateObject)(
    HANDLE      SourceProcessHandle,
    HANDLE      SourceHandle,
    HANDLE      TargetProcessHandle,
    PHANDLE     TargetHandle,
    ACCESS_MASK DesiredAccess,
    ULONG       HandleAttributes,
    ULONG       Options
    );
pNtDuplicateObject NtDuplicateObject = NULL;

typedef NTSTATUS(_stdcall* pNtClose)(
    HANDLE Handle
    );
pNtClose NtClose = NULL;

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)