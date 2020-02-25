#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

VOID MyGetTickCount(PULONG pMsec)
{
	LARGE_INTEGER TickCount;
	ULONG MyInc = KeQueryTimeIncrement();
	KeQueryTickCount(&TickCount);
	TickCount.QuadPart *= MyInc;
	TickCount.QuadPart /= 10000;
	*pMsec = TickCount.LowPart;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	NTSTATUS status = STATUS_SUCCESS;
	ULONG msec = 0;
	MyGetTickCount(&msec);
	msec /= 1000;
	KdPrint(("%d\n", msec));
	pDriver->DriverUnload = DriverUnload;
	return status;
}