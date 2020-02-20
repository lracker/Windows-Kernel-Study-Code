#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	NTSTATUS status;
	// 定义一个内存分配标记
#define MEM_TAG 'MyTt'
	// 目标字符串，接下来它需要分配空间
	UNICODE_STRING dst = { 0 };
	UNICODE_STRING src = RTL_CONSTANT_STRING(L"My source string!");
	// 根据源字符串的长度，分配空间给目标字符串
	dst.Buffer = (PWCHAR)ExAllocatePoolWithTag(NonPagedPool, src.Length, MEM_TAG);
	if (dst.Buffer == NULL)
	{
		// 错误处理
		status = STATUS_INSUFFICIENT_RESOURCES;
	}
	else
	{
		dst.Length = dst.MaximumLength = src.Length;
		RtlCopyUnicodeString(&dst, &src);
		KdPrint(("%wZ", &dst));
		status = STATUS_SUCCESS;
	}
	pDriver->DriverUnload = DriverUnload;
	return status;
}