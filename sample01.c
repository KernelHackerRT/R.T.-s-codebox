#include <ntddk.h>
#include <ntstrsafe.h>

#define FILE_DEVICE_MYDRIVER 0x00008001
#define MYDRIVER_IOCTL_DELETE_FILE CTL_CODE(FILE_DEVICE_MYDRIVER, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _FILE_NAME {
    UNICODE_STRING FileName;
} FILE_NAME, *PFILE_NAME;

NTSTATUS DeleteFile(PUNICODE_STRING FileName)
{
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;
    HANDLE fileHandle;
    NTSTATUS status;

    // Initialize the object attributes structure
    InitializeObjectAttributes(&objectAttributes, FileName, OBJ_KERNEL_HANDLE, NULL, NULL);

    // Open the file for deletion
    status = ZwCreateFile(&fileHandle, DELETE, &objectAttributes, &ioStatusBlock, NULL, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, FILE_OPEN, FILE_DELETE_ON_CLOSE, NULL, 0);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    // Close the handle to the file to delete it
    ZwClose(fileHandle);

    return STATUS_SUCCESS;
}

NTSTATUS DispatchIoctl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    NTSTATUS status;
    PIO_STACK_LOCATION stackLocation;
    ULONG ioControlCode;
    PFILE_NAME fileName = NULL;

    stackLocation = IoGetCurrentIrpStackLocation(Irp);
    ioControlCode = stackLocation->Parameters.DeviceIoControl.IoControlCode;

    switch (ioControlCode) {
    case MYDRIVER_IOCTL_DELETE_FILE:
        // Get a pointer to the input buffer
        fileName = (PFILE_NAME)Irp->AssociatedIrp.SystemBuffer;
        if (fileName == NULL) {
            status = STATUS_INVALID_PARAMETER;
            break;
        }

        // Delete the specified file
        status = DeleteFile(&fileName->FileName);
        break;

    default:
        status = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }

    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return status;
}
