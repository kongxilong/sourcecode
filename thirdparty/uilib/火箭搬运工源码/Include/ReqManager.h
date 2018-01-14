
#pragma once

#include <Windows.h>
#include "define.h"
#include "TList.h"

// 控制命令
enum Command
{
	// 可以开始写入文件
	BEGIN_WRITE_FILE,
	// 写文件
	WRITE_FILE,
	// 拷贝文件结束
	REQ_EOF
};

struct FileStat
{
	LARGE_INTEGER lnFileSize;
	FILETIME	ftCreationTime;
	FILETIME	ftLastAccessTime;
	FILETIME	ftLastWriteTime;
	DWORD		dwFileAttributes;
	HANDLE		hFile;
	DWORD		dwLastError;
	int			nFileStatMinSize;

	WCHAR		cFileName[MAX_PATH];
};

struct ReqHeader : public ListObj
{
	// 操作命令
	Command		HeaderCommand;
	BYTE		*pHeaderBuf;
	int			nHeaderBufSize;
	int			nHeaderReqSize;
	FileStat	FStat;
};

struct ReqBuf
{
	// 存储读写数据用的缓存VBuf
	BYTE		*pBuf;
	// 以最大扇区对齐的缓存的长度
	int			nBufSize;
	ReqHeader	*pReqHeader;
	int			nReqHeaderSize;
};


class TReqList : public CTList
{
public:
	TReqList(void) {};

	ReqHeader *TopObj(void)
	{
		return (ReqHeader *)CTList::TopObj();
	};

	ReqHeader *NextObj(ReqHeader *pReqObj)
	{
		return (ReqHeader *)CTList::NextObj(pReqObj);
	};
};
