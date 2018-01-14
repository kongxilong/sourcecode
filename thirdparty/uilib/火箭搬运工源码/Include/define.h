
#pragma once

#pragma warning(disable:4311)
#pragma warning(disable:4312)

#include <windows.h>

// read write 缓存大小 32MB
#define READ_WRITE_BUF_LEN					(32 * 1024 *1024)

// 默认扇区大小
#define DEFAULT_SECTOR_SIZE					(2048)

// 无长度限制路径前缀
#define PATH_LOCAL_PREFIX					(L"\\\\?\\")
#define PATH_LOCAL_PREFIX_LEN				(4)
#define PATH_UNC_PREFIX						(L"\\\\?\\UNC")
#define PATH_UNC_PREFIX_LEN					(7)

// WCHAR 的宽度
#define WCHAR_SIZE							(sizeof(WCHAR))

// 取得安全权限
#ifndef SE_CREATE_SYMBOLIC_LINK_NAME
#define SE_CREATE_SYMBOLIC_LINK_NAME		TEXT("SeCreateSymbolicLinkPrivilege")
#endif

// 将 all_size 以 blcok_size 对齐
#define ALIGN_SIZE(all_size, block_size)	((((all_size) + (block_size) -1) / (block_size)) * (block_size))

// FILE_FLAG_NO_BUFFERING 禁止对磁盘进行缓冲处理，文件只能写入磁盘卷的扇区块
// NTFS 当文件长度 < 65535(64KB) 字节时，使用 FILE_FLAG_NO_BUFFERING 来写入文件。
#define NB_MIN_SIZE_NTFS					(64 * 1024)
// FAT 当文件长度 < 131072(128KB) 字节时，使用 FILE_FLAG_NO_BUFFERING 来写入文件。
#define NB_MIN_SIZE_FAT						(128 * 1024)

// 每次读取或者写入的最大长度 8388608(8MB)
#define MAX_TRANS_SIZE						(8 * 1024 * 1024)

// TBD 打开文件是的标志 (VERIFY_MD5 | AUTOSLOW_IOLIMIT | SAMEDIR_RENAME)
#define OPEN_FILE_FLAGS						(0x02000110)

// 等待队列操作结束
#define CV_WAIT_TICK						(200)

// 最小扇区大小
#define MIN_SECTOR_SIZE						(512)

// 最大读取/写入长度减少的尺寸
#define REDUCE_SIZE							(1024 * 1024)

// NTFS支持的最长的文件路径长度 32768(32KB)
#define NTFS_MAX_PATH						(32768 + MAX_PATH)
#define MAX_PATH_EX							(MAX_PATH * 8)

#ifndef IS_SAVE_HANDLE
// 是否为安全句柄
#define IS_SAVE_HANDLE(hHandle)				(((hHandle) != NULL) && ((hHandle) != INVALID_HANDLE_VALUE))
#endif

// 安全删除宏
#ifndef SAFE_DELETE_MEM_LIST
#define SAFE_DELETE_MEM_LIST(pMemBuf)		{ if ((pMemBuf) != NULL) { delete [] (pMemBuf); } (pMemBuf) = NULL; }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(pMemBuf)				{ if ((pMemBuf) != NULL) { delete (pMemBuf); } (pMemBuf) = NULL; }
#endif

#ifndef SAFE_CLOSE_HANDLE
#define SAFE_CLOSE_HANDLE(hHandle)			{ if (IS_SAVE_HANDLE(hHandle)) { ::CloseHandle(hHandle); } (hHandle) = NULL; }
#endif

// 是否为文件夹
#define IsDir(dwFileAttr)					(dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)
#define IsReparse(dwFileAttr)				(dwFileAttr & FILE_ATTRIBUTE_REPARSE_POINT)
#define IsNoReparseDir(dwFileAttr)			(IsDir(dwFileAttr) && !IsReparse(dwFileAttr))


// 磁盘类型
enum DiskType
{
	// 未知
	DT_NONE,
	// NTFS
	DT_NTFS,
	// FAT
	DT_FAT,
	// 网络磁盘
	DT_NETWORK
};

enum Flags
{
	CREATE_OPENERR_FILE	=	0x00000001,
	USE_OSCACHE_READ	=	0x00000002,
	USE_OSCACHE_WRITE	=	0x00000004,
	PRE_SEARCH			=	0x00000008,
	SAMEDIR_RENAME		=	0x00000010,
	SKIP_EMPTYDIR		=	0x00000020,
	FIX_SAMEDISK		=	0x00000040,
	FIX_DIFFDISK		=	0x00000080,
	AUTOSLOW_IOLIMIT	=	0x00000100,
	WITH_ACL			=	0x00000200,
	WITH_ALTSTREAM		=	0x00000400,
	OVERWRITE_DELETE	=	0x00000800,
	OVERWRITE_DELETE_NSA=	0x00001000,
	FILE_REPARSE		=	0x00002000,
	DIR_REPARSE			=	0x00004000,
	COMPARE_CREATETIME	=	0x00008000,
	SERIAL_MOVE			=	0x00010000,
	SERIAL_VERIFY_MOVE	=	0x00020000,
	USE_OSCACHE_READVERIFY=	0x00040000,
	RESTORE_HARDLINK	=	0x00080000,
	DEL_BEFORE_CREATE	=	0x00200000,
	DELDIR_WITH_FILTER	=	0x00400000,
	LISTING				=	0x01000000,
	VERIFY_MD5			=	0x02000000,
	OVERWRITE_PARANOIA	=	0x04000000,
	VERIFY_FILE			=	0x08000000,
	LISTING_ONLY		=	0x10000000,
	REPORT_ACL_ERROR	=	0x20000000,
	REPORT_STREAM_ERROR	=	0x40000000
};
