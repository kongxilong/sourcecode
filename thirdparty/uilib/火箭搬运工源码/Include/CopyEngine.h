
#pragma once

#include <Windows.h>
#include <process.h>
#include <stddef.h>
#include <stdio.h>
#include <AtlBase.h>
#include <atlstr.h>
#include <math.h>

#include "define.h"
#include "Function.h"
#include "VBuf.h"
#include "Condition.h"
#include "ReqManager.h"
#include "PathList.h"
#include "CopyLog.h"

struct COPY_STA 
{
	// 所有需要拷贝文件总数量
	int nAllFileCnts;
	// 已经被成功拷贝的文件数量
	int nSuccessFileCnts;
	// 已经被拷贝但失败的文件数量
	int nErrorFileCnts;
	// 所有需要拷贝文件的长度
	_int64 nAllCopySize;
	// 已经被拷贝的长度
	_int64 nCopiedSize;
	// 操作开始时间
	DWORD dwBeginTime;
	// 操作的所有时间，单位 [s]
	double dbCopiedTime;
	// 总体的拷贝的平均速度，单位 [00.00 MB/s]
	double dbAvgSpeed;
	// 上次操作结束时间
	DWORD dwPreEndTime;
	// 当前的拷贝速度，单位 [00.00 MB/s]
	double dbCurSpeed;
	// 当前文件长度
	_int64 nCurFileSize;
	// 当前文件已经拷贝长度
	_int64 nCurFileCopiedSize;
	// 当前操作源文件全路径
	WCHAR *pSrcPath;
	// 当前操作目标文件全路径
	WCHAR *pDstPath;
	// 暂停时间，单位 [毫秒]
	DWORD dwPauseTime;
};

class CCopyEngine;

class CCopyInf
{
	friend class CCopyEngine;
private:
	COPY_STA m_CopySta;

public:
	CCopyInf()
	{
		clear();
	};

	~CCopyInf()
	{
	};

	void clear()
	{
		m_CopySta.nAllCopySize = 0;
		m_CopySta.nCopiedSize = 0;
		m_CopySta.dwBeginTime = 0;
		m_CopySta.dwPreEndTime = 0;
		m_CopySta.dbCopiedTime = 0.0;
		m_CopySta.dbAvgSpeed = 0.0;
		m_CopySta.dbCurSpeed = 0.0;
		m_CopySta.dwPauseTime = 0;
		m_CopySta.nCurFileSize = 0;
		m_CopySta.nCurFileCopiedSize = 0;
		m_CopySta.pSrcPath = NULL;
		m_CopySta.pDstPath = NULL;
		m_CopySta.nAllFileCnts = 0;
		m_CopySta.nSuccessFileCnts = 0;
		m_CopySta.nErrorFileCnts = 0;
	};

	void AddFileSize(_int64 nSize, int nFileCnts)
	{
		m_CopySta.nAllFileCnts += nFileCnts;
		m_CopySta.nAllCopySize += nSize;
	};

	void InitCopy()
	{
		m_CopySta.dwBeginTime = ::GetTickCount();
		m_CopySta.dwPreEndTime = m_CopySta.dwBeginTime;
	};

	void InitCopyOneFile(_int64 nFileSize)
	{
		m_CopySta.nCurFileSize = nFileSize;
		m_CopySta.nCurFileCopiedSize = 0;
	};

	void BeginPause()
	{
		m_CopySta.dwPauseTime = ::GetTickCount();
	};

	void EndPause()
	{
		DWORD dwEndTime = ::GetTickCount();
		m_CopySta.dwPauseTime = GetTickDiff(m_CopySta.dwPauseTime, dwEndTime);
	};

	inline void AddCopiedSize(_int64 nSize)
	{
		m_CopySta.nCopiedSize += nSize;
		m_CopySta.nCurFileCopiedSize += nSize;

		if (nSize > 0)
		{
			DWORD dwEndTime = ::GetTickCount();

			m_CopySta.dwBeginTime += m_CopySta.dwPauseTime;
			m_CopySta.dwPauseTime = 0;

			m_CopySta.dbCopiedTime = (double)((double)GetTickDiff(m_CopySta.dwBeginTime, dwEndTime) / 1000.0);
			double dbCopiedMB = (double)(((double)m_CopySta.nCopiedSize) / ((double)(1024 * 1024)));
			m_CopySta.dbAvgSpeed = dbCopiedMB / m_CopySta.dbCopiedTime;

			double dbCurTime = (double)((double)GetTickDiff(m_CopySta.dwPreEndTime, dwEndTime) / 1000.0);
			dbCopiedMB = (double)(((double)nSize) / ((double)(1024 * 1024)));
			if (dbCurTime <= 0.0)
				m_CopySta.dbCurSpeed = m_CopySta.dbAvgSpeed;
			else
				m_CopySta.dbCurSpeed = dbCopiedMB / dbCurTime;
			m_CopySta.dwPreEndTime = dwEndTime;
		}
	};

	inline void GetSpeedInfo(COPY_STA &CopyInf)
	{
		CopyInf.nAllFileCnts = m_CopySta.nAllFileCnts;
		CopyInf.nSuccessFileCnts = m_CopySta.nSuccessFileCnts;
		CopyInf.nErrorFileCnts = m_CopySta.nErrorFileCnts;
		CopyInf.nAllCopySize = m_CopySta.nAllCopySize;
		CopyInf.nCopiedSize = m_CopySta.nCopiedSize;
		CopyInf.dwBeginTime = m_CopySta.dwBeginTime;
		CopyInf.dbCopiedTime = m_CopySta.dbCopiedTime;
		CopyInf.dbAvgSpeed = m_CopySta.dbAvgSpeed;
		CopyInf.dwPreEndTime = m_CopySta.dwPreEndTime;
		CopyInf.dbCurSpeed = m_CopySta.dbCurSpeed;
		CopyInf.nCurFileSize = m_CopySta.nCurFileSize;
		CopyInf.nCurFileCopiedSize = m_CopySta.nCurFileCopiedSize;
	};

	void SuccessOne() { m_CopySta.nSuccessFileCnts++; };
	void ErrorOne() { m_CopySta.nErrorFileCnts++; };
};

class CCopyEngine
{
public:
	CCopyEngine(CCopyLog *pCopyLog);
	virtual ~CCopyEngine();

	void Stop(void);

	BOOL Copy(TCHAR *pFrom, TCHAR *pTo);
	BOOL WaitEnd(void);
	inline void GetCopyInfo(COPY_STA &CopyInf)
	{
		m_CopyInfo.GetSpeedInfo(CopyInf);

		CopyInf.pSrcPath = m_pSrcPath;
		CopyInf.pDstPath = m_pDstPath;
	};

	void PauseContinue(void);

	BOOL IsPause(void) { return m_bIsPause; }
	BOOL IsRuning(void) { return (IS_SAVE_HANDLE(m_hStartThread)); }
	BOOL IsEnd(void) { return (!IsRuning()); };

private:
	// 暂停状态
	BOOL m_bIsPause;
	// 需要操作的所有路径信息
	CPathList m_PathList;
	// 当前操作的路径信息
	PathObj *m_pCurPathObj;
	// 互斥事件管理类
	Condition m_Condition;
	// 强制退出标志
	BOOL m_bIsAbort;
	// 不需要系统缓存存取数据时，文件的最大尺寸
	int m_nNonbufMinSize;
	// 拷贝的实时信息
	CCopyInf m_CopyInfo;
	// 当前操作的源磁盘和目标磁盘的最大扇区值
	int m_nMaxSectorSize;

	TReqList m_WriteReqList;
	ReqHeader *m_pWriteReqHeader;

	DWORD m_dwMaxReadSize;
	DWORD m_dwMaxWriteSize;

	HANDLE m_hReadThread;
	HANDLE m_hWriteThread;
	HANDLE m_hStartThread;

	// VBuf 列表
	CVBufList m_VBufList;
	// 操作文件的源路径 -> into VBufList
	WCHAR *m_pSrcPath;
	// 操作文件的目标路径 -> into VBufList
	WCHAR *m_pDstPath;
	// 当前拷贝文件状态 -> into VBufList
	FileStat *m_pCurSrcFileSta;
	// 读写数据的主缓存 -> into VBufList
	CVBuf *m_pReadWriteBuf;
	// 当前在主缓存中用户需要使用的内存的偏移量
	BYTE *m_pMBufUsedOffset;
	// 当前在主缓存中剩余可用的缓存的开始偏移量
	BYTE *m_pMBufFreeOffset;

private:
	void Start();
	void End(void);
	void EndOnePath(DWORD dwWait = CV_WAIT_TICK);

	BOOL CopyA(char *pFrom, char *pTo);
	BOOL CopyW(WCHAR *pFrom, WCHAR *pTo);

	static unsigned WINAPI StartThread(void *pCpEn);
	static unsigned WINAPI ReadThread(void *pCpEn);
	static unsigned WINAPI WriteThread(void *pCpEn);

	void StartThreadCore();

	int MakeUnlimitedPath(WCHAR *pPath);
	void OnePathObjFinishNotify(void);

	DiskType GetFsType(WCHAR *pRootDir);
	BOOL IsSameDrive(WCHAR *pRoot1, WCHAR *pRoot2);
	int GetSectorSize(WCHAR *pRootDir);

	BOOL ReadDirCore(WCHAR *pSrcDirPath, WCHAR *pDstDirPath);
	BOOL ReadOneFileCore(WCHAR *pScrPath);
	BOOL ReadThreadCore(void);
	BOOL OpenFileProc();
	BOOL ReadFileAttr(WCHAR *pPath);
	BOOL ReadFileProc();
	BOOL ReadFileWithReduce(HANDLE hFile, void *pBuf, DWORD dwSize, DWORD *pdwReads, OVERLAPPED *pOverWrap);
	HANDLE CreateFileWithRetry(WCHAR *path, DWORD mode, DWORD share, SECURITY_ATTRIBUTES *sa, DWORD cr_mode, DWORD flg, HANDLE hTempl, int retry_max=10);

	BOOL WriteThreadCore(void);
	BOOL WriteProc();
	BOOL WrieFileWithReduce(HANDLE hFile, void *pBuf, DWORD size, DWORD *written, OVERLAPPED *pOverWrap);
	void WriteReqDone(void);

	void ErrorEndOneFile();

	BOOL CheckDstRequest(void);

	void FdatToFileStat(WIN32_FIND_DATAW *FindDat, FileStat *stat);
	BOOL WriteFileProc();

	BOOL RB_RecvRequest(void);
	BOOL RB_SendRequest(Command command, ReqBuf *pReqBuf = NULL, FileStat *pStat = NULL);
	BOOL RB_AllocReqBuf(int nReqSize, _int64 n64DataSize, ReqBuf *pReqBuf);
	BOOL RB_PrepareReqBuf(int nReqSize, _int64 n64DataSize, ReqBuf *pReqBuf);
};
