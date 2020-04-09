#include <login.h>

DWORD Login::SampleCreateVirtualDisk(_In_ LPCWSTR VirtualDiskPath, _In_opt_ LPCWSTR ParentPath, _In_ CREATE_VIRTUAL_DISK_FLAG Flags, _In_ ULONGLONG FileSize,
                                     _In_ DWORD BlockSize, _In_ DWORD LogicalSectorSize, _In_ DWORD PhysicalSectorSize)
{
        VIRTUAL_STORAGE_TYPE storageType;
        CREATE_VIRTUAL_DISK_PARAMETERS parameters;
        HANDLE vhdHandle = INVALID_HANDLE_VALUE;
        DWORD opStatus;
        GUID uniqueId;
        if (RPC_S_OK != UuidCreate((UUID*)&uniqueId))
            {
                opStatus = ERROR_NOT_ENOUGH_MEMORY;
               // wprintf(L"error = %u\n", opStatus);
                if (vhdHandle != INVALID_HANDLE_VALUE) CloseHandle(vhdHandle);

                return opStatus;
            }

            storageType.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_VHD;
            storageType.VendorId = VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT;

            memset(&parameters, 0, sizeof(parameters));

            parameters.Version = CREATE_VIRTUAL_DISK_VERSION_2;
            parameters.Version2.UniqueId = uniqueId;
            parameters.Version2.MaximumSize = FileSize;
            parameters.Version2.BlockSizeInBytes = BlockSize;
            parameters.Version2.SectorSizeInBytes = LogicalSectorSize;
            parameters.Version2.PhysicalSectorSizeInBytes = PhysicalSectorSize;
            parameters.Version2.ParentPath = ParentPath;

            opStatus = CreateVirtualDisk(&storageType, VirtualDiskPath, VIRTUAL_DISK_ACCESS_NONE, NULL, Flags, 0,
                                         &parameters, NULL, &vhdHandle);

            if (vhdHandle != INVALID_HANDLE_VALUE)
               {
                   CloseHandle(vhdHandle);
               }

               return opStatus;
}

unsigned long Login::create_v_disk(LPCWSTR virtualDiskPath, ULONGLONG fileSize_mb) {
    LPCWSTR path = virtualDiskPath;
    LPCWSTR parentPath = NULL;
    ULONGLONG fileSize = fileSize_mb * int64_t(1024) * int64_t(1024);                                           // MUST BE NOT LESS 3 MB
    DWORD blockSize = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_BLOCK_SIZE;
    DWORD logicalSectorSize = 512;
    DWORD physicalSectorSize = 512;

    auto rc =SampleCreateVirtualDisk(path,parentPath,CREATE_VIRTUAL_DISK_FLAG_FULL_PHYSICAL_ALLOCATION,fileSize,
                                      blockSize,logicalSectorSize,physicalSectorSize);
    return rc;
}

void Login::generate_diskpart_script(string script, string path) {
    ofstream f_script;
    f_script.open(path + "dpscript.txt");
    f_script << script;
    f_script.close();
}

void Login::run_diskpart_script(string path) {
    string path_str_script = path;
    path_str_script += "dpscript.txt";
    char buf[1024] = "diskpart /s \"";
    sprintf(buf + 13, "%s\"", path_str_script.data());
    system(buf);
}

void Login::format_v_disk(string path_str, string diskName) {
    string script = "select vdisk file=\"";
    script += path_str + diskName;
    script += "\"\nattach vdisk\nformat quick fs=fat label=\"My Vault\"\nassign letter=Z";

    Login::generate_diskpart_script(script, path_str);
    Login::run_diskpart_script(path_str);

}

void Login::attach_v_disk(string path_str, string diskName) {
    string script = "select vdisk file=\"";
    script += path_str + diskName;
    script += "\"\nattach vdisk\nassign letter=Z";
    Login::generate_diskpart_script(script, path_str);
    Login::run_diskpart_script(path_str);
}

void Login::detach_v_disk(string path_str, string diskName) {
    string script = "select vdisk file=\"";
    script += path_str + diskName;
    script += "\"\ndetach vdisk";
    Login::generate_diskpart_script(script, path_str);
    Login::run_diskpart_script(path_str);
}

void Login::init_v_disk(string path, string diskName) {
    string script = "select vdisk file=\"";
    script += path + diskName;
    script += "\"\nattach vdisk\ncreate partition primary\nformat quick fs=ntfs label=\"My Vault\"\nassign letter=Z";
    Login::generate_diskpart_script(script, path);
    Login::run_diskpart_script(path);
}
