#include <ntddk.h>

// 提供一个Unload函数只是为了让这个程序能动态卸载，方便调试
VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
	// 但是实际上我们什么都不做，只打印一句话
	DbgPrint("first：Our driver\r\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg_path)
{
#if DBG
	_asm int 3
#endif
	pDriver;
	pReg_path;
	// 这里是内核模块的入口，可以在这里写入我们想要的东西
	DbgPrint("first: Hello, my salary!");

	// 这里使用UNICODE_STRING来保存字符串并且打印
	UNICODE_STRING str = RTL_CONSTANT_STRING(L"first: Hello, my salary! ");
	DbgPrint("%wZ", &str);
	// 设置一个卸载函数，便于这个函数退出
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}