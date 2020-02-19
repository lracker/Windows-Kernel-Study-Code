#include <ntddk.h>

// 提供一个Unload函数只是为了让这个程序能动态卸载，方便调试
VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
	// 但是实际上我们什么都不做，只打印一句话
	DbgPrint("first：Our driver\r\n");
}

// DriverEntry，入口函数，相当于mian
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg_path)
{
#if DBG
	// 这里的 int3 是一条汇编语言，执行到它的时候程序会断下来，等于手工设置了一个断点。
	// 但是这样的代码如果不是调试状态执行就会直接蓝屏，所以加上宏 DBG 测试，保证只有调试版本呢
	// 这条语句才被编译
	_asm int 3
#endif
	pDriver;
	pReg_path;
	// 这里是内核模块的入口，可以在这里写入我们想要的东西
	DbgPrint("first: Hello, my salary!");
	PAGED_CODE();
	// 设置一个卸载函数，便于这个函数退出
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}