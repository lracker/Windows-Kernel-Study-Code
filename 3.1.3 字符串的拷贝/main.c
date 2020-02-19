#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
	DbgPrint("Unload\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	UNICODE_STRING dst;	// 目标字符串
	WCHAR DstBuff[256];	// 先定义缓冲区
	UNICODE_STRING src = RTL_CONSTANT_STRING(L"My source string!");
	// 把目标字符串初始化为拥有缓冲区长度为256的UNICODE_STRING空串
	RtlInitEmptyUnicodeString(&dst, DstBuff, 256 * sizeof(WCHAR));
	RtlCopyUnicodeString(&dst, &src);
	DbgPrint("%wZ", &dst);
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}