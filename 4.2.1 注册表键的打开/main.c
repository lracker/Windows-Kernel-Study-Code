#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS OpenRegister(PUNICODE_STRING pKeyPath)
{
	NTSTATUS status;
	HANDLE MyKey = NULL;
	OBJECT_ATTRIBUTES MyObjAttr = { 0 };
	// 初始化OBJECT_ATTRIBUTES
	InitializeObjectAttributes(
		&MyObjAttr,
		pKeyPath,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL
	);
	status = ZwOpenKey(&MyKey, KEY_READ, &MyObjAttr);
	if (!NT_SUCCESS(status))
	{
		// 失败处理
		//……
	}
	return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg_path)
{
	pDriver;
	pReg_path;
	NTSTATUS status;
	UNICODE_STRING KeyPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
	status = OpenRegister(&KeyPath);
	pDriver->DriverUnload = DriverUnload;
	return status;
}