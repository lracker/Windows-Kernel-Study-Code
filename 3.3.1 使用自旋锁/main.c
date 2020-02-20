#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

KSPIN_LOCK MySpinLock;

VOID MySafeFunction()
{
	KIRQL irql;
	// 申请自旋锁，irql用于保存当前的中断请求级
	KeAcquireSpinLock(&MySpinLock, &irql);
	// To do something ...
	// 释放自旋锁，irql保存当前的中断请求级别，注意，这个跟上面那个申请自旋锁不一样，不是指针变量。
	KeReleaseSpinLock(&MySpinLock, irql);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	NTSTATUS status;
	// KeInitializeSpinLock 函数没有返回值。
	KeInitializeSpinLock(&MySpinLock);
	MySafeFunction();
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}