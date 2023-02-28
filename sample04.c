#include <ntddk.h>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    // Open a handle to the current process
    HANDLE currentProcess = PsGetCurrentProcess();

    // Set the process protection level to "Protected Process"
    ULONG protectionLevel = PROTECTION_LEVEL_WINTCB;
    NTSTATUS status = ZwSetInformationProcess(currentProcess, ProcessProtectionLevel, &protectionLevel, sizeof(protectionLevel));
    if (!NT_SUCCESS(status)) {
        DbgPrint("Failed to set process protection level.\n");
        return status;
    }

    return STATUS_SUCCESS;
}
