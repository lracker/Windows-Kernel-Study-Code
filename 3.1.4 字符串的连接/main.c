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
	// 第一种连接：将一个UNICODE拼接一个字符串
	UNICODE_STRING dst;	// 目标字符串
	WCHAR DstBuff[256];	// 定义缓冲区
	UNICODE_STRING src = RTL_CONSTANT_STRING(L"My source string!");
	// 把目标字符串初始化为拥有缓冲区长度为256的UNICODE_STRING空串
	RtlInitEmptyUnicodeString(&dst, DstBuff, 256 * sizeof(WCHAR));
	RtlCopyUnicodeString(&dst, &src);	// 字符串拷贝
	// UNICODE_STRING拼接另一个字符串使用RtlAppendUnicodeToString
	RtlAppendUnicodeToString(&dst, L"my second string");
	DbgPrint("%wZ\n", &dst);
	// 第二种连接：将一个UNICODE_STRING连接另一个UNICODE_STRING
	UNICODE_STRING str1;
	UNICODE_STRING str2;
	WCHAR strBuff1[256];	
	WCHAR strBuff2[256];
	RtlInitEmptyUnicodeString(&str1, strBuff1, 256 * sizeof(WCHAR));
	RtlInitEmptyUnicodeString(&str2, strBuff2, 256 * sizeof(WCHAR));
	UNICODE_STRING src1 = RTL_CONSTANT_STRING(L"First");
	UNICODE_STRING src2 = RTL_CONSTANT_STRING(L"Second");
	RtlCopyUnicodeString(&str1, &src1);
	RtlCopyUnicodeString(&str2, &src2);
	//UNICODE_STRING拼接UNICODE_STRING使用RtlAppendUnicodeStringToString
	RtlAppendUnicodeStringToString(&str1, &str2);
	DbgPrint("%wZ\n", &str1);
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}