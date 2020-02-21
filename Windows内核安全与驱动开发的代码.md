# 第一章

### 1.1.2 编写第一个C 文件

```cpp
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

	// 设置一个卸载函数，便于这个函数退出
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}
```



### 2.2.3 字符串的使用

```c
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
```



### 3.1.2 字符串的初始化

```c
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
```



### 3.1.3 字符串的拷贝

```c
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
```



### 3.1.4 字符串的连接

```c
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
    NTSTATUS status;
	// 第一种连接：将一个UNICODE拼接一个字符串
	UNICODE_STRING dst;	// 目标字符串
	WCHAR DstBuff[256];	// 定义缓冲区
	UNICODE_STRING src = RTL_CONSTANT_STRING(L"My source string!");
	// 把目标字符串初始化为拥有缓冲区长度为256的UNICODE_STRING空串
	RtlInitEmptyUnicodeString(&dst, DstBuff, 256 * sizeof(WCHAR));
	RtlCopyUnicodeString(&dst, &src);	// 字符串拷贝
	// UNICODE_STRING拼接另一个字符串使用RtlAppendUnicodeToString
	status = RtlAppendUnicodeToString(&dst, L"my second string");
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
	status = RtlAppendUnicodeStringToString(&str1, &str2);
	DbgPrint("%wZ\n", &str1);
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}
```



### 3.1.5 字符串的打印

```c
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
	dst.Length = (USHORT)wcslen(dst.Buffer)*sizeof(WCHAR);
	DbgPrint("%wZ", &dst);
	pDriver->DriverUnload = DriverUnload;
	return status;
}
```



### 3.2.1 内存的分配与释放

```c
#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	NTSTATUS status;
	// 定义一个内存分配标记
#define MEM_TAG 'MyTt'
	// 目标字符串，接下来它需要分配空间
	UNICODE_STRING dst = { 0 };
	UNICODE_STRING src = RTL_CONSTANT_STRING(L"My source string!");
	// 根据源字符串的长度，分配空间给目标字符串
	dst.Buffer = (PWCHAR)ExAllocatePoolWithTag(NonPagedPool, src.Length, MEM_TAG);
	if (dst.Buffer == NULL)
	{
		// 错误处理
		status = STATUS_INSUFFICIENT_RESOURCES;
	}
	else
	{
		dst.Length = dst.MaximumLength = src.Length;
		RtlCopyUnicodeString(&dst, &src);
		KdPrint(("%wZ", &dst));
		status = STATUS_SUCCESS;
	}
	pDriver->DriverUnload = DriverUnload;
	return status;
}
```



### 3.2.2 使用LIST_ENTRY

```c
#include <ntddk.h>
#define MEM_TAG 'ddd'
// 链表头
LIST_ENTRY MyListHead;

// 链表头初始化。一般在程序入口处调用一下
void MyFileInforInit()
{
	InitializeListHead(&MyListHead);
}

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

// 链表节点，里面保存一个文件名和一个文件长度信息
typedef struct 
{
	LIST_ENTRY ListEntry;
	PFILE_OBJECT FileObject;
	PUNICODE_STRING FileName;
	LARGE_INTEGER FileLength;
}MYFILEINFO, *PMYFILEINFO;

// 追加一条信息，也就是增加一个链表结点。请注意，FileName是外面分配的
// 内存由使用者管理，本链表并不管理它。
NTSTATUS MyFileInforAppendNode(
	PFILE_OBJECT FileObject,
	PUNICODE_STRING FileName,
	LARGE_INTEGER FileLength
)
{
	PMYFILEINFO MyFileInfor = (PMYFILEINFO)ExAllocatePoolWithTag(PagedPool, sizeof(MYFILEINFO), MEM_TAG);
	if (MyFileInfor == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;
	// 填写数据成员
	MyFileInfor->FileObject = FileObject;
	MyFileInfor->FileName = FileName;
	MyFileInfor->FileLength = FileLength;
	
	// 插入到链表末尾。请注意这里没有使用任何锁，所以，这个函数不是
	// 多线程安全的。在自旋锁的使用中讲解如何保证多线程安全性
	InsertHeadList(&MyListHead, (PLIST_ENTRY)&MyFileInfor);
	return STATUS_SUCCESS;
}
```



### 3.2.3 使用长长整型数据

```c
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
		KdPrint(("b.QuadPart > 1000，LowPart = %x HighPart = %x", b.LowPart, b.HighPart));
	}
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}
```



### 3.3.1 使用自旋锁

```c
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
```





### 3.3.2 在双向链表中使用自旋锁

```c
#include <ntddk.h>
#define MEM_TAG 'ddd'
LIST_ENTRY MyListHead;	// 链表头
KSPIN_LOCK MyListLock;	// 链表的锁

// 链表头初始化。一般在程序入口处调用一下
void MyFileInforInit()
{
	InitializeListHead(&MyListHead);
	KeInitializeSpinLock(&MyListHead);
}

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

// 链表节点，里面保存一个文件名和一个文件长度信息
typedef struct
{
	LIST_ENTRY ListEntry;
	PFILE_OBJECT FileObject;
	PUNICODE_STRING FileName;
	LARGE_INTEGER FileLength;
}MYFILEINFO, * PMYFILEINFO;

// 追加一条信息，也就是增加一个链表结点。请注意，FileName是外面分配的
// 内存由使用者管理，本链表并不管理它。
NTSTATUS MyFileInforAppendNode(
	PFILE_OBJECT FileObject,
	PUNICODE_STRING FileName,
	LARGE_INTEGER FileLength
)
{
	PMYFILEINFO MyFileInfor = (PMYFILEINFO)ExAllocatePoolWithTag(PagedPool, sizeof(MYFILEINFO), MEM_TAG);
	if (MyFileInfor == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;
	// 填写数据成员
	MyFileInfor->FileObject = FileObject;
	MyFileInfor->FileName = FileName;
	MyFileInfor->FileLength = FileLength;

	// 使用这个加锁的插入操作来代替普通的操作
	ExInterlockedInsertHeadList(&MyListHead, (PLIST_ENTRY)&MyFileInfor, &MyListLock);
	// 使用这个加锁的删除操作
	MyFileInfor = ExInterlockedRemoveHeadList(&MyListHead, &MyListLock);
	return STATUS_SUCCESS;
}
```



### 3.3.3 使用队列自旋锁提高性能

```c
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
```



### 4.1.2 打开和关闭文件

```c
#include <ntddk.h>

VOID DriveUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	// 要返回的文件句柄
	HANDLE FileHandle = NULL;
	// 返回值
	NTSTATUS status;
	// 定义IoStatusBlock这个结构
	IO_STATUS_BLOCK io_status;
	// 首先初始化含有文件路径的OBJECT_ATTRIBUTES
	OBJECT_ATTRIBUTES ObjectAttributes;
	UNICODE_STRING uFileName = RTL_CONSTANT_STRING(L"\\??\\C:\\1.txt");
	InitializeObjectAttributes(
		&ObjectAttributes,
		&uFileName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL);
	// 以FILE_OPEN_IF方式打开文件
	status = ZwCreateFile(
		&FileHandle,
		GENERIC_READ | GENERIC_WRITE,
		&ObjectAttributes,
		&io_status,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN_IF,
		FILE_NON_DIRECTORY_FILE | FILE_RANDOM_ACCESS | FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);
    ZwClose(FileHandle);
	pDriver->DriverUnload = DriveUnload;
	return status;
}
```



### 4.1.3 文件读写操作

```c
#include <ntddk.h>
#define MEM_TAG 'MyTt'

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS MyCopyFile(PUNICODE_STRING TargetPath, PUNICODE_STRING SourcePath)
{
	NTSTATUS status;
	// 目标句柄和源句柄
	HANDLE TargetHandle, SourceHandle = NULL;
	// 定义IoStatusBlock这个结构
	IO_STATUS_BLOCK TarIoStatus, SouIoStatus;
	// 首先初始化含有文件路径的OBJECT_ATTRIBUTES
	OBJECT_ATTRIBUTES TarObjectAttributes, SouObjectAttributes;
	InitializeObjectAttributes(
		&SouObjectAttributes,
		SourcePath,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL
	);
	InitializeObjectAttributes(
		&TarObjectAttributes,
		TargetPath,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL
	);
	// 以FILE_OPEN方式打开源文件。如果文件不存在，则返回复制失败
	status = ZwCreateFile(
		&SourceHandle,
		GENERIC_READ,
		&SouObjectAttributes,
		&SouIoStatus,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN,
		FILE_NON_DIRECTORY_FILE | FILE_RANDOM_ACCESS | FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0
	);
	// 如果打开失败的话，就返回了
	if (!NT_SUCCESS(status))
	{
		KdPrint(("打开源文件失败\n"));
		return status;
	}
	// 以FILE_CREATE方式打开源文件。
	status = ZwCreateFile(
		&TargetHandle,
		GENERIC_WRITE,
		&TarObjectAttributes,
		&TarIoStatus,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_CREATE,
		FILE_NON_DIRECTORY_FILE | FILE_RANDOM_ACCESS | FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0
	);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("创建文件失败\n"));
		return status;
	}
	// 接下来读取文件内容到Buffer
	// 申请一端4kb的空间
	ULONG uLength = 4 * 1024;
	LARGE_INTEGER offset = { 0 };
	PVOID Buffer = ExAllocatePoolWithTag(NonPagedPool, 4 * 1024, MEM_TAG);
	do
	{
		if (Buffer == NULL)
			break;
		while (1)
		{
			RtlZeroMemory(Buffer, uLength);
			status = ZwReadFile(
				SourceHandle,
				NULL,
				NULL,
				NULL,
				&TarIoStatus,
				Buffer,
				uLength,
				&offset,
				NULL
			);
			if (!NT_SUCCESS(status))
			{
				// 如果状态为STATUS_END_OF_FILE， 
				// 则说明文件的拷贝已经成功地结束了。
				if (status == STATUS_END_OF_FILE)
					status = STATUS_SUCCESS;
				break;
			}
			// 获得实际读取到的长度
			uLength = TarIoStatus.Information;
			// 现在读取了内容。读出的长度为length，那么写入的长度也应该是length。
			// 写入必须成功，如果失败，则返回错误。
			status = ZwWriteFile(
				TargetHandle,
				NULL,
				NULL,
				NULL,
				&TarIoStatus,
				Buffer,
				uLength,
				&offset,
				NULL
			);
			if (!NT_SUCCESS(status))
				break;

			// offset 移动，然后继续，直到出现
			// STATUS_END_OF_FILE 的时候彩技术
			offset.QuadPart += uLength;
		}
	} while (0);
	// 在退出之前，释放资源，关闭所有的句柄。
	if (TargetHandle != NULL)
		ZwClose(TargetHandle);
	if (SourceHandle != NULL)
		ZwClose(SourceHandle);
	if (Buffer != NULL)
		ExFreePool(Buffer);
	return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg_path)
{
	pDriver;
	pReg_path;
	// 返回值
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING TargetPath = RTL_CONSTANT_STRING(L"\\??\\C:\\2.txt");
	UNICODE_STRING SourcePath = RTL_CONSTANT_STRING(L"\\??\\C:\\1.txt");
	status = MyCopyFile(&TargetPath, &SourcePath);
	pDriver->DriverUnload = DriverUnload;
	return status;
}
```

