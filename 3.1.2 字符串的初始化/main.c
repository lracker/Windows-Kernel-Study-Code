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
	// 第一种方法
	UNICODE_STRING str1 = { 0 };
	WCHAR Buffer[128];
	str1.Buffer = Buffer;
	wcscpy_s(str1.Buffer, 128, L"hello 1" );
	str1.Length = str1.MaximumLength = (USHORT)wcslen(L"hello 1") * sizeof(WCHAR);
	DbgPrint("%wZ\n", &str1);
	// 第二种方法
	UNICODE_STRING str2 = { 0 };
	str2.Buffer = L"hello 2";
	str2.Length = str2.MaximumLength = (USHORT)wcslen(L"hello 2") * sizeof(WCHAR);
	DbgPrint("%wZ\n", &str2);
	// 第三种方法，使用WDK提供的API
	UNICODE_STRING str3 = { 0 };
	RtlInitUnicodeString(&str3, L"hello 3");
	DbgPrint("%wZ\n", &str3);
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}