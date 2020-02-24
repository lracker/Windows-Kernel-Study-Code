#include <ntddk.h>
#define MEM_TAG1 'ddd'
#define MEM_TAG2 'eee'

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS ReadRegister(PUNICODE_STRING pKeyPath, PUNICODE_STRING pKeyName)
{
	NTSTATUS status;
	HANDLE MyKeyHandle = NULL;
	OBJECT_ATTRIBUTES MyObjAttr = { 0 };
	// 用来试探大小的KeyInfor
	KEY_VALUE_PARTIAL_INFORMATION KeyInfor;
	// 最后实际用到的KeyInfor指针。内存分配在堆中
	PKEY_VALUE_PARTIAL_INFORMATION AcKeyInfor;
	ULONG uAcLength;

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
	status = ZwQueryValueKey(
		MyKeyHandle,
		pKeyName,
		KeyValuePartialInformation,
		&KeyInfor,
		sizeof(KEY_VALUE_PARTIAL_INFORMATION),
		&uAcLength);
	if (!NT_SUCCESS(status) &&
		status != STATUS_BUFFER_OVERFLOW &&
		status != STATUS_BUFFER_TOO_SMALL)
	{
		KdPrint(("读取注册表键值失败"));
		return status;
	}
	// 如果没有失败，那么分配足够的空间，再次读取。
	AcKeyInfor = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePoolWithTag(NonPagedPool, uAcLength, MEM_TAG1);
	if (AcKeyInfor == NULL)
	{
		KdPrint(("分配失败"));
		status = STATUS_INSUFFICIENT_RESOURCES;
		return status;
	}
	status = ZwQueryValueKey(
		MyKeyHandle,
		pKeyName,
		KeyValuePartialInformation,
		AcKeyInfor,
		uAcLength,
		&uAcLength);
	// 到此位置，如果status为STATUS_SUCCESS，则要读取的数据已经
	// 在AcKeyInfor->Data里面了
	PUCHAR Buffer = (PUCHAR)ExAllocatePoolWithTag(NonPagedPool, AcKeyInfor->DataLength, MEM_TAG2);
	RtlCopyMemory(Buffer, AcKeyInfor->Data, AcKeyInfor->DataLength);
	UNICODE_STRING Buff;
	Buff.Buffer = (PWCHAR)Buffer;
	Buff.Length = Buff.MaximumLength = (USHORT)AcKeyInfor->DataLength;
	KdPrint(("%wZ", &Buff));
	return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg_path)
{
	pDriver;
	pReg_path;
	NTSTATUS status;
	UNICODE_STRING KeyPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
	UNICODE_STRING KeyName = RTL_CONSTANT_STRING(L"SystemRoot");
	status = ReadRegister(&KeyPath, &KeyName);
	pDriver->DriverUnload = DriverUnload;
	return status;
}