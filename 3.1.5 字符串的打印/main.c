#include <ntddk.h>
#include <ntstrsafe.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	WCHAR DstBuf[512] = { 0 };
	UNICODE_STRING dst;
	NTSTATUS status;
	UNICODE_STRING FilePath = RTL_CONSTANT_STRING(L"\\??\\c:\\winddk\\7600.16385.1\\inc\\cifs.h");
	USHORT FileSize = 1024;
	// 字符串初始化为空串。缓冲区长度为*sizeof(WCHAR)
	RtlInitEmptyUnicodeString(&dst, DstBuf, 512 * sizeof(WCHAR));
	// 调用RtlStringCbPrintfW来进行打印
	// 这个函数跟sprintfw一样，并不是在终端输出，而是打印到这个字符串里
	status = RtlStringCbPrintfW(dst.Buffer, 512 * sizeof(WCHAR), L"file path = %wZ file size = %d \r\n", &FilePath, FileSize);
#if DBG
	_asm int 3
#endif
	// 这里调用wcslen没问题，这是因为RtlStringCbPrintfW打印的
	// 字符串是以空结束的
	dst.Length = (USHORT)wcslen(dst.Buffer) * sizeof(WCHAR);
	DbgPrint("%wZ", &dst);
	pDriver->DriverUnload = DriverUnload;
	return status;
}