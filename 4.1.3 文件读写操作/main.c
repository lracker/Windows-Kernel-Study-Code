#include <ntddk.h>
#define MEM_TAG 'MyTt'

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS MyCopyFile(PUNICODE_STRING TargetPath, PUNICODE_STRING SourcePath)
{
	NTSTATUS status;
	// 目标句柄和源句柄
	HANDLE TargetHandle, SourceHandle = NULL;
	// 定义IoStatusBlock这个结构
	IO_STATUS_BLOCK TarIoStatus, SouIoStatus;
	// 首先初始化含有文件路径的OBJECT_ATTRIBUTES
	OBJECT_ATTRIBUTES TarObjectAttributes, SouObjectAttributes;
	InitializeObjectAttributes(
		&SouObjectAttributes,
		SourcePath,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL
	);
	InitializeObjectAttributes(
		&TarObjectAttributes,
		TargetPath,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL
	);
	// 以FILE_OPEN方式打开源文件。如果文件不存在，则返回复制失败
	status = ZwCreateFile(
		&SourceHandle,
		GENERIC_READ,
		&SouObjectAttributes,
		&SouIoStatus,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN,
		FILE_NON_DIRECTORY_FILE | FILE_RANDOM_ACCESS | FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0
	);
	// 如果打开失败的话，就返回了
	if (!NT_SUCCESS(status))
	{
		KdPrint(("打开源文件失败\n"));
		return status;
	}
	// 以FILE_CREATE方式打开源文件。
	status = ZwCreateFile(
		&TargetHandle,
		GENERIC_WRITE,
		&TarObjectAttributes,
		&TarIoStatus,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_CREATE,
		FILE_NON_DIRECTORY_FILE | FILE_RANDOM_ACCESS | FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0
	);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("创建文件失败\n"));
		return status;
	}
	// 接下来读取文件内容到Buffer
	// 申请一端4kb的空间
	ULONG uLength = 4 * 1024;
	LARGE_INTEGER offset = { 0 };
	PVOID Buffer = ExAllocatePoolWithTag(NonPagedPool, 4 * 1024, MEM_TAG);
	do
	{
		if (Buffer == NULL)
			break;
		while (1)
		{
			RtlZeroMemory(Buffer, uLength);
			status = ZwReadFile(
				SourceHandle,
				NULL,
				NULL,
				NULL,
				&TarIoStatus,
				Buffer,
				uLength,
				&offset,
				NULL
			);
			if (!NT_SUCCESS(status))
			{
				// 如果状态为STATUS_END_OF_FILE， 
				// 则说明文件的拷贝已经成功地结束了。
				if (status == STATUS_END_OF_FILE)
					status = STATUS_SUCCESS;
				break;
			}
			// 获得实际读取到的长度
			uLength = TarIoStatus.Information;
			// 现在读取了内容。读出的长度为length，那么写入的长度也应该是length。
			// 写入必须成功，如果失败，则返回错误。
			status = ZwWriteFile(
				TargetHandle,
				NULL,
				NULL,
				NULL,
				&TarIoStatus,
				Buffer,
				uLength,
				&offset,
				NULL
			);
			if (!NT_SUCCESS(status))
				break;

			// offset 移动，然后继续，直到出现
			// STATUS_END_OF_FILE 的时候彩技术
			offset.QuadPart += uLength;
		}
	} while (0);
	// 在退出之前，释放资源，关闭所有的句柄。
	if (TargetHandle != NULL)
		ZwClose(TargetHandle);
	if (SourceHandle != NULL)
		ZwClose(SourceHandle);
	if (Buffer != NULL)
		ExFreePool(Buffer);
	return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg_path)
{
	pDriver;
	pReg_path;
	// 返回值
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING TargetPath = RTL_CONSTANT_STRING(L"\\??\\C:\\2.txt");
	UNICODE_STRING SourcePath = RTL_CONSTANT_STRING(L"\\??\\C:\\1.txt");
	status = MyCopyFile(&TargetPath, &SourcePath);
	pDriver->DriverUnload = DriverUnload;
	return status;
}