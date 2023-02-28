#include <ntddk.h>
#include <ntstrsafe.h>

#define FILE_DEVICE_MYDRIVER 0x00008001
#define MYDRIVER_IOCTL_READ_FILE CTL_CODE(FILE_DEVICE_MYDRIVER, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define MYDRIVER_IOCTL_WRITE_FILE CTL_CODE(FILE_DEVICE_MYDRIVER, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _FILE_BUFFER {
    UNICODE_STRING FileName;
    PVOID Buffer;
    ULONG Length;
} FILE_BUFFER, *PFILE_BUFFER;

NTSTATUS ReadFile(PFILE_OBJECT FileObject, PFILE_BUFFER FileBuffer)
{
    NTSTATUS status;
    IO_STATUS_BLOCK ioStatusBlock;
    LARGE_INTEGER byteOffset;
    PVOID buffer = NULL;
    ULONG length = 0;

    // Allocate a buffer to hold the contents of the file
    buffer = ExAllocatePoolWithTag(PagedPool, FileBuffer->Length, 'MYDR');
    if (buffer == NULL) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    // Read the contents of the file into the buffer
    byteOffset.QuadPart = 0;
    status = ZwReadFile(FileObject, NULL, NULL, NULL, &ioStatusBlock, buffer, FileBuffer->Length, &byteOffset, NULL);
    if (!NT_SUCCESS(status)) {
        ExFreePoolWithTag(buffer, 'MYDR');
        return status;
    }

    // Return the contents of the file to the caller
    FileBuffer->Buffer = buffer;
    FileBuffer->Length = ioStatusBlock.Information;

    return STATUS_SUCCESS;
}

NTSTATUS WriteFile(PFILE_OBJECT FileObject, PFILE_BUFFER FileBuffer)
{
    NTSTATUS status;
    IO_STATUS_BLOCK ioStatusBlock;
    LARGE_INTEGER byteOffset;

    // Write the contents of the buffer to the file
    byteOffset.QuadPart = 0;
    status = ZwWriteFile(FileObject, NULL, NULL, NULL, &ioStatusBlock, FileBuffer->Buffer, FileBuffer->Length, &byteOffset, NULL);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    return STATUS_SUCCESS;
}

NTSTATUS DispatchIoctl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    NTSTATUS status;
    PIO_STACK_LOCATION stackLocation;
    ULONG ioControlCode;
    PFILE_BUFFER fileBuffer = NULL;
    UNICODE_STRING fileName;
    PFILE_OBJECT fileObject = NULL;
    HANDLE fileHandle = NULL;

    stackLocation = IoGetCurrentIrpStackLocation(Irp);
    ioControlCode = stackLocation->Parameters.
