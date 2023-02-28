#include <ntddk.h>

typedef NTSTATUS (*ZwCreateFileProc)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, PLARGE_INTEGER, ULONG, ULONG, ULONG, ULONG, PVOID, ULONG);
ZwCreateFileProc OriginalZwCreateFile = NULL;

NTSTATUS HookedZwCreateFile(PHANDLE FileHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, PLARGE_INTEGER AllocationSize, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PVOID EaBuffer, ULONG EaLength)
{
    DbgPrint("ZwCreateFile hook function called.\n");

    // Call the original ZwCreateFile function
    return OriginalZwCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNICODE_STRING targetFunctionName;
    PVOID targetFunctionAddress;

    // Get the address of the original ZwCreateFile function
    RtlInitUnicodeString(&targetFunctionName, L"ZwCreateFile");
    targetFunctionAddress = MmGetSystemRoutineAddress(&targetFunctionName);
    if (targetFunctionAddress == NULL) {
        DbgPrint("Failed to get the address of ZwCreateFile.\n");
        return STATUS_UNSUCCESSFUL;
    }
    OriginalZwCreateFile = (ZwCreateFileProc)targetFunctionAddress;

    // Replace the original ZwCreateFile function with our hook function
    __writemsr(0xC0000082, (ULONG64)HookedZwCreateFile);

    return STATUS_SUCCESS;
}
