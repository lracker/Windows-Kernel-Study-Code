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
