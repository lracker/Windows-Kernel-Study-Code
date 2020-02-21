#include <ntddk.h>

VOID DriveUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	// 要返回的文件句柄
	HANDLE FileHandle = NULL;
	// 返回值
	NTSTATUS status;
	// 定义IoStatusBlock这个结构
	IO_STATUS_BLOCK io_status;
	// 首先初始化含有文件路径的OBJECT_ATTRIBUTES
	OBJECT_ATTRIBUTES ObjectAttributes;
	UNICODE_STRING uFileName = RTL_CONSTANT_STRING(L"\\??\\C:\\1.txt");
	InitializeObjectAttributes(
		&ObjectAttributes,
		&uFileName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL);
	// 以FILE_OPEN_IF方式打开文件
	status = ZwCreateFile(
		&FileHandle,
		GENERIC_READ | GENERIC_WRITE,
		&ObjectAttributes,
		&io_status,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN_IF,
		FILE_NON_DIRECTORY_FILE | FILE_RANDOM_ACCESS | FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);
	ZwClose(FileHandle);
	pDriver->DriverUnload = DriveUnload;
	return status;
}