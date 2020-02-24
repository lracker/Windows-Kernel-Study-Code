#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS WriteRegister(PUNICODE_STRING pKeyPath, PUNICODE_STRING pName, PWCHAR pValue)
{
	NTSTATUS status;
	HANDLE MyKeyHandle = NULL;
	OBJECT_ATTRIBUTES MyObjAttr = { 0 };
	// 初始化OBJECT_ATTRIBUTES
	InitializeObjectAttributes(
		&MyObjAttr,
		pKeyPath,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL
	);
	status = ZwOpenKey(&MyKeyHandle, KEY_READ, &MyObjAttr);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("打开注册表失败"));
		return status;
	}
	// 写入数据。数据长度之所以要将字符串长度加上1，是为了把最后一个空结束符
	// 写入。笔者不确定如果不写入空结束符会不会有错。有兴趣可以测试一下。
	// （实际测试了一下，并不会出错）
	status = ZwSetValueKey(MyKeyHandle, pName, 0, REG_SZ, pValue, (wcslen(pValue) + 1) * sizeof(WCHAR));
	if (!NT_SUCCESS(status))
	{
		KdPrint(("写入注册表失败"));
		return status;
	}
	return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg_path)
{
	pDriver;
	pReg_path;
	NTSTATUS status;
	UNICODE_STRING KeyPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
	UNICODE_STRING Name = RTL_CONSTANT_STRING(L"Test");
	PWCHAR pValue = { L"My Test Value" };
	status = WriteRegister(&KeyPath, &Name, pValue);
	pDriver->DriverUnload = DriverUnload;
	return status;
}