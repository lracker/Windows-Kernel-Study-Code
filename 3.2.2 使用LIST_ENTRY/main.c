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
