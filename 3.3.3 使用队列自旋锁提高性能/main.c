#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

KSPIN_LOCK MyQueueSpinLock;
KLOCK_QUEUE_HANDLE MyLockQueueHandle;

VOID MySafeFunction()
{
	// 申请队列自旋锁,队列自旋锁的使用增加了一个 KLOCK_QUEUE_HANDLE 数据结构
	// 这个数据结构唯一地表示一个队列自旋锁
	KeAcquireInStackQueuedSpinLock(&MyQueueSpinLock, &MyLockQueueHandle);
	// To do something ...
	// 释放队列自旋锁。
	KeReleaseInStackQueuedSpinLock(&MyLockQueueHandle);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	NTSTATUS status;
	// KeInitializeSpinLock 函数没有返回值。
	KeInitializeSpinLock(&MyQueueSpinLock);
	MySafeFunction();
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}