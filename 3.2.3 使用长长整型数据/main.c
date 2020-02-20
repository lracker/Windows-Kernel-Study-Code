#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg_path)
{
	pDriver;
	pReg_path;
	LARGE_INTEGER a, b;
	a.QuadPart = 100;
	a.QuadPart *= 100;
	b.QuadPart = a.QuadPart;
	if (b.QuadPart > 1000)
	{
		KdPrint(("b.QuadPart > 1000£¬LowPart = %x HighPart = %x", b.LowPart, b.HighPart));
	}
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}