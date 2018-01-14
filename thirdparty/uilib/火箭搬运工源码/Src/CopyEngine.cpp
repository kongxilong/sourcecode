
#include "stdafx.h"
#include "CopyEngine.h"


CCopyEngine::CCopyEngine(CCopyLog *pCopyLog)
{
	ASSERT(pCopyLog != NULL);

	m_bIsPause = FALSE;
	m_bIsAbort = FALSE;
	g_pCopyLog = pCopyLog;
	m_pCurPathObj = NULL;
	m_nNonbufMinSize = 0;
	m_pCurSrcFileSta = NULL;
	m_pReadWriteBuf = NULL;
	m_pMBufUsedOffset = NULL;
	m_pMBufFreeOffset = NULL;

	SetPrivilege(SE_BACKUP_NAME, TRUE);
	SetPrivilege(SE_RESTORE_NAME, TRUE);
	SetPrivilege(SE_CREATE_SYMBOLIC_LINK_NAME, TRUE);

	m_dwMaxReadSize = 0;

	int nFileStaSize = ALIGN_SIZE(sizeof(FileStat), 8);
	m_pCurSrcFileSta = (FileStat *)m_VBufList.NewVByte(nFileStaSize);

	m_pReadWriteBuf = m_VBufList.NewVBuf(READ_WRITE_BUF_LEN + PAGE_SIZE * 4);
	m_pMBufUsedOffset = m_pMBufFreeOffset = m_pReadWriteBuf->Buf();

	m_pSrcPath = (WCHAR *)m_VBufList.NewVByte(NTFS_MAX_PATH);
	m_pDstPath = (WCHAR *)m_VBufList.NewVByte(NTFS_MAX_PATH);

	m_hReadThread = m_hWriteThread = m_hStartThread = NULL;
}

CCopyEngine::~CCopyEngine()
{
	m_VBufList.ClearAll();
}

BOOL CCopyEngine::Copy(TCHAR *pFrom, TCHAR *pTo)
{
#ifdef UNICODE
	return CopyW(pFrom, pTo);
#else
	return CopyA(pFrom, pTo);
#endif
}

BOOL CCopyEngine::CopyA(char *pFrom, char *pTo)
{
	USES_CONVERSION;
	return CopyW(A2W(pFrom), A2W(pTo));
}

BOOL CCopyEngine::CopyW(WCHAR *pFrom, WCHAR *pTo)
{
	BOOL bRet = FALSE;
	BOOL bIsDir = FALSE;
	_int64 nFileSize = 0;
	int nFileCnts = 0;
	PathObj *pNewPathObj = NULL;
	WCHAR SrcRoot[MAX_PATH];
	WCHAR DstRoot[MAX_PATH];

	m_bIsAbort = false;

	if (m_PathList.IsEmpty())
		m_CopyInfo.clear();

	// 拷贝操作，目标必须为目录
	if (!PathIsDir(pTo, bIsDir) || !bIsDir)
		goto END;

	// 取得源路径是否为目录的信息
	if (!PathIsDir(pFrom, bIsDir))
		goto END;

	// 判断源文件和目标是否在同一磁盘
	memset(SrcRoot, 0, MAX_PATH);
	memset(DstRoot, 0, MAX_PATH);
	if (!GetRootDir(pFrom, SrcRoot))
		goto END;

	if (!GetRootDir(pTo, DstRoot))
		goto END;

	// 取得源路径的所有文件的长度总和
	if (bIsDir)
	{
		if (!GetDirSize(pFrom, nFileSize, nFileCnts, m_bIsAbort))
			goto END;
	}
	else
	{
		nFileCnts = 1;
		if (!FileSize(pFrom, nFileSize))
			goto END;
	}

	pNewPathObj = m_PathList.NewPathObj();
	if (pNewPathObj != NULL)
	{
		pNewPathObj->ActionType = PAT_COPY;
		pNewPathObj->bSrcIsDir = bIsDir;
		pNewPathObj->bDstIsDir = TRUE;
		pNewPathObj->bIsActionEnd = FALSE;
		pNewPathObj->dwLastErr = 0;
		pNewPathObj->nAllSrcFileSize = nFileSize;
		m_CopyInfo.AddFileSize(nFileSize, nFileCnts);

		::wcscpy_s(pNewPathObj->pScrPath, NTFS_MAX_PATH, pFrom);
		::wcscpy_s(pNewPathObj->pDstPath, NTFS_MAX_PATH, pTo);
		// 转换为无长度限制路径
		MakeUnlimitedPath(pNewPathObj->pScrPath);
		MakeUnlimitedPath(pNewPathObj->pDstPath);

		pNewPathObj->bIsSameDrv = IsSameDrive(SrcRoot, DstRoot);

		// 取得源文件磁盘的扇区大小
		pNewPathObj->nSrcSectorSize = GetSectorSize(SrcRoot);

		// 取得目标磁盘的扇区大小
		pNewPathObj->nDstSectorSize = pNewPathObj->nSrcSectorSize;
		if (!pNewPathObj->bIsSameDrv)
			pNewPathObj->nDstSectorSize = GetSectorSize(DstRoot);

		// 取得源文件磁盘的格式
		pNewPathObj->SrcFsType = GetFsType(SrcRoot);

		// 取得目标磁盘的格式
		pNewPathObj->DstFsType = pNewPathObj->SrcFsType;
		if (!pNewPathObj->bIsSameDrv)
			pNewPathObj->DstFsType = GetFsType(DstRoot);

		bRet = TRUE;
	}

END:
	if (!bRet)
	{
		if (pNewPathObj != NULL)
			m_PathList.DelPathObj(pNewPathObj);
	}
	else
	{
		Start();
	}

	return TRUE;
}

// 取得磁盘类型
DiskType CCopyEngine::GetFsType(WCHAR *pRootDir)
{
	DiskType Dt = DT_NETWORK;

	if (pRootDir != NULL)
	{
		if (::GetDriveTypeW(pRootDir) != DRIVE_REMOTE)
		{
			DWORD dwSerial = 0, dwFsFlags = 0, dwMaxFName = 0;
			WCHAR szVol[MAX_PATH], szFs[MAX_PATH];

			::GetVolumeInformationW(pRootDir, szVol, MAX_PATH, &dwSerial, &dwMaxFName, &dwFsFlags, szFs, MAX_PATH);

			Dt = (::lstrcmpiW(szFs, L"NTFS") == 0 ? DT_NTFS : DT_FAT);
		}
	}

	return Dt;
}

// 取得磁盘扇区大小
int CCopyEngine::GetSectorSize(WCHAR *pRootDir)
{
	DWORD dwSpc, dwBps, dwFc, dwCl;

	if (!::GetDiskFreeSpaceW(pRootDir, &dwSpc, &dwBps, &dwFc, &dwCl))
		return DEFAULT_SECTOR_SIZE;

	return dwBps;
}

// 判断是否为相同磁盘
BOOL CCopyEngine::IsSameDrive(WCHAR *pRoot1, WCHAR *pRoot2)
{
	if (GetChar(pRoot1, 1) != GetChar(pRoot2, 1) || ::GetDriveTypeW(pRoot1) != ::GetDriveTypeW(pRoot2))
		return	FALSE;

	return (::lstrcmpiW(pRoot1, pRoot2) == 0);
}

// 转换为无长度限制路径
int CCopyEngine::MakeUnlimitedPath(WCHAR *pPath)
{
	int nPrefixLen = 0;
	WCHAR *pPrefix;
	BOOL bIsUNC = (*pPath == '\\') ? TRUE : FALSE;

	pPrefix = bIsUNC ? PATH_UNC_PREFIX : PATH_LOCAL_PREFIX;
	nPrefixLen = bIsUNC ? PATH_UNC_PREFIX_LEN : PATH_LOCAL_PREFIX_LEN;

	memmove(pPath + nPrefixLen - (bIsUNC ? 1 : 0), pPath, (::wcslen(pPath) + 1) * WCHAR_SIZE);
	memcpy(pPath, pPrefix, nPrefixLen * WCHAR_SIZE);

	return nPrefixLen;
}

void CCopyEngine::Start()
{
	if (m_hStartThread == NULL)
	{
		u_int id = 0;
		m_hStartThread = (HANDLE)_beginthreadex(0, 0, CCopyEngine::StartThread, this, 0, &id);
	}
}

unsigned WINAPI CCopyEngine::StartThread(void *pCpEn)
{
	((CCopyEngine *)pCpEn)->StartThreadCore();
	return 0;
}

void CCopyEngine::StartThreadCore()
{
	if (m_pCurPathObj == NULL && !m_PathList.IsEmpty())
	{
		m_bIsAbort = FALSE;
		m_CopyInfo.InitCopy();

		m_pCurPathObj = m_PathList.TopObj();
		while (m_pCurPathObj != NULL && !m_bIsAbort)
		{
			// 复制操作
			if (m_pCurPathObj->ActionType == PAT_COPY)
			{
				u_int id = 0;

				m_pWriteReqHeader = NULL;

				m_Condition.Initialize(2);
				m_WriteReqList.Init();

				if (m_hWriteThread == NULL && m_hReadThread == NULL)
				{
					if (!(m_hWriteThread = (HANDLE)_beginthreadex(0, 0, CCopyEngine::WriteThread, this, 0, &id))
					|| !(m_hReadThread  = (HANDLE)_beginthreadex(0, 0, CCopyEngine::ReadThread,  this, 0, &id)))
						break;
				}

				if (!m_Condition.WaitEnd(ONE_PATH_OBJ_END, m_bIsAbort))
					break;
			}

			EndOnePath(INFINITE);

			m_pCurPathObj->bIsActionEnd = TRUE;
			PATH_END_LOG(TRUE, m_pCurPathObj->pScrPath);

			PathObj *pTemp = m_pCurPathObj;
			m_pCurPathObj = m_PathList.NextObj(pTemp);
		}

		// 等待结束
		End();
		m_Condition.NotifyEnd(ALL_FILE_END);
	}
	m_PathList.ClearAll();
}

unsigned WINAPI CCopyEngine::ReadThread(void *pCpEn)
{
	return ((CCopyEngine *)pCpEn)->ReadThreadCore();
}

BOOL CCopyEngine::ReadFileAttr(WCHAR *pPath)
{
	WIN32_FIND_DATAW FindDat;
	HANDLE hFile = ::FindFirstFileW(pPath, &FindDat);
	if (!IS_SAVE_HANDLE(hFile))
		return FALSE;

	FdatToFileStat(&FindDat, m_pCurSrcFileSta);
	::FindClose(hFile);

	LARGE_INTEGER lnSize;
	lnSize.HighPart = FindDat.nFileSizeHigh;
	lnSize.LowPart = FindDat.nFileSizeLow;
	m_CopyInfo.InitCopyOneFile(lnSize.QuadPart);

	return TRUE;
}

// 打开指定文件
BOOL CCopyEngine::OpenFileProc()
{
	BOOL bRet = FALSE;

	DWORD dwFlg = ((OPEN_FILE_FLAGS & USE_OSCACHE_READ) ? 0 : FILE_FLAG_NO_BUFFERING) | FILE_FLAG_SEQUENTIAL_SCAN;

	m_pCurSrcFileSta->hFile = ::CreateFileW(m_pSrcPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, dwFlg, 0);
	if (IS_SAVE_HANDLE(m_pCurSrcFileSta->hFile))
		bRet = TRUE;
	else
		m_pCurSrcFileSta->dwLastError = ::GetLastError();

	m_nNonbufMinSize = ((m_pCurPathObj->DstFsType == DT_NTFS) ? NB_MIN_SIZE_NTFS : NB_MIN_SIZE_FAT);

	m_nMaxSectorSize = max(m_pCurPathObj->nSrcSectorSize, m_pCurPathObj->nDstSectorSize);
	m_nMaxSectorSize = max(m_nMaxSectorSize, MIN_SECTOR_SIZE);

	return	bRet;
}

BOOL CCopyEngine::ReadOneFileCore(WCHAR *pScrPath)
{
	BOOL bRet = TRUE;
	// 初始化
	m_pMBufUsedOffset = m_pMBufFreeOffset = m_pReadWriteBuf->Buf();
	m_dwMaxReadSize = m_dwMaxWriteSize = MAX_TRANS_SIZE;

	// 取得源路径
	::wsprintfW(m_pSrcPath, L"%s", pScrPath);

	if (!OpenFileProc())
	{
		ErrorEndOneFile();
		bRet = FALSE;
	}
	else
	{
		ReadFileProc();
	}

	SAFE_CLOSE_HANDLE(m_pCurSrcFileSta->hFile);

	return bRet;
}

BOOL CCopyEngine::ReadDirCore(WCHAR *pSrcDirPath, WCHAR *pDstDirPath)
{
	BOOL bRet = FALSE;
	WIN32_FIND_DATAW FindDat;
	WCHAR *pFindPath = new WCHAR[NTFS_MAX_PATH];
	WCHAR *pDstPath = new WCHAR[NTFS_MAX_PATH];

	if (pFindPath != NULL && pDstPath != NULL)
	{
		MakePath(pFindPath, pSrcDirPath, L"*.*");

		HANDLE hFile = ::FindFirstFileW(pFindPath, &FindDat);
		if (IS_SAVE_HANDLE(hFile))
		{
			bRet = TRUE;
			do
			{
				if (FindDat.cFileName[0] == '.')
					continue;

				MakePath(pFindPath, pSrcDirPath, FindDat.cFileName);
				if (IsDir(FindDat.dwFileAttributes))
				{
					// 创建目标文件夹
					MakePath(pDstPath, pDstDirPath, FindDat.cFileName);
					::CreateDirectoryW(pDstPath, NULL);

					bRet = ReadDirCore(pFindPath, pDstPath);

					if (bRet)
						continue;
					else
						break;
				}

				if (!ReadFileAttr(pFindPath))
					break;

				MakePath(m_pDstPath, pDstDirPath, FindDat.cFileName);

				bRet = ReadOneFileCore(pFindPath);
				if (!bRet)
					break;

			}while (::FindNextFileW(hFile, &FindDat) && !m_bIsAbort);
			::FindClose(hFile);
		}
	}
	SAFE_DELETE_MEM_LIST(pFindPath);
	SAFE_DELETE_MEM_LIST(pDstPath);

	return bRet;
}

BOOL CCopyEngine::ReadThreadCore(void)
{
	BOOL bRet = FALSE;

	if (m_pCurPathObj->bSrcIsDir)
	{
		bRet = ReadDirCore(m_pCurPathObj->pScrPath, m_pCurPathObj->pDstPath);
	}
	else
	{
		if (ReadFileAttr(m_pCurPathObj->pScrPath))
		{
			MakePath(m_pDstPath, m_pCurPathObj->pDstPath, m_pCurSrcFileSta->cFileName);
			bRet = ReadOneFileCore(m_pCurPathObj->pScrPath);
		}
	}

	RB_SendRequest(REQ_EOF);
	OnePathObjFinishNotify();

	return	bRet;
}

void CCopyEngine::ErrorEndOneFile()
{
	m_CopyInfo.ErrorOne();
}

BOOL CCopyEngine::ReadFileProc()
{
	BOOL bRet = TRUE;
	DWORD dwTransSize = 0;
	ReqBuf ReqBuffer;
	BOOL bIsOnlyData = FALSE;

	::SetFilePointer(m_pCurSrcFileSta->hFile, 0, NULL, FILE_BEGIN);

	for (_int64 nRemainSize = m_pCurSrcFileSta->lnFileSize.QuadPart; nRemainSize > 0 && !m_bIsAbort; nRemainSize -= dwTransSize)
	{
		dwTransSize = 0;
		while (true && !m_bIsAbort)
		{
			if (!(bRet = RB_PrepareReqBuf(offsetof(ReqHeader, FStat) + (bIsOnlyData ? 0 : m_pCurSrcFileSta->nFileStatMinSize), nRemainSize, &ReqBuffer)))
			{
				ErrorEndOneFile();
				break;
			}

			if ((bRet = ReadFileWithReduce(m_pCurSrcFileSta->hFile, ReqBuffer.pBuf, ReqBuffer.nBufSize, &dwTransSize, NULL)))
				break;
			else
				ErrorEndOneFile();
		}

		RB_SendRequest((bIsOnlyData ? WRITE_FILE : BEGIN_WRITE_FILE), &ReqBuffer, (bIsOnlyData ? NULL : m_pCurSrcFileSta));
		bIsOnlyData = TRUE;
	}

	return bRet;
}

BOOL CCopyEngine::ReadFileWithReduce(HANDLE hFile, void *pBuf, DWORD dwSize, DWORD *pdwReads, OVERLAPPED *pOverWrap)
{
	DWORD dwTrans = 0;
	DWORD dwTotal = 0;

	while ((dwTrans = dwSize - dwTotal) > 0 && !m_bIsAbort)
	{
		DWORD dwTransed = 0;
		dwTrans = min(dwTrans, m_dwMaxReadSize);
		if (!::ReadFile(hFile, (BYTE *)pBuf + dwTotal, dwTrans, &dwTransed, pOverWrap))
		{
			if (::GetLastError() != ERROR_NO_SYSTEM_RESOURCES || min(dwSize, m_dwMaxReadSize) <= REDUCE_SIZE)
			{
				return	FALSE;
			}
			m_dwMaxReadSize -= REDUCE_SIZE;
			m_dwMaxReadSize = ALIGN_SIZE(m_dwMaxReadSize, REDUCE_SIZE);
			continue;
		}

		dwTotal += dwTransed;
		if (dwTransed != dwTrans)
			break;

		// 暂停等待
		m_Condition.WaitPause(m_bIsAbort);
	}
	*pdwReads = dwTotal;

	return	TRUE;
}

BOOL CCopyEngine::CheckDstRequest(void)
{
	return FALSE;
}

void CCopyEngine::OnePathObjFinishNotify(void)
{
	m_Condition.NotifyEnd(ONE_PATH_OBJ_END);
}

unsigned WINAPI CCopyEngine::WriteThread(void *pCpEn)
{
	return ((CCopyEngine *)pCpEn)->WriteThreadCore();
}

BOOL CCopyEngine::WriteThreadCore(void)
{
	BOOL bRet = WriteProc();

	m_Condition.Lock();
	m_pWriteReqHeader = NULL;
	m_WriteReqList.Init();
	m_Condition.Notify();
	m_Condition.UnLock();

	return	bRet;
}

BOOL CCopyEngine::WriteProc()
{
	BOOL	bRet = TRUE;

	while (!m_bIsAbort && (bRet = RB_RecvRequest()))
	{
		if (m_pWriteReqHeader->HeaderCommand == REQ_EOF)
			break;

		if (m_pWriteReqHeader->HeaderCommand == BEGIN_WRITE_FILE)
			bRet = WriteFileProc();
	}

	return	bRet && !m_bIsAbort;
}

HANDLE CCopyEngine::CreateFileWithRetry(WCHAR *path, DWORD mode, DWORD share, SECURITY_ATTRIBUTES *sa, DWORD cr_mode, DWORD flg, HANDLE hTempl, int retry_max)
{
	HANDLE	hFile = INVALID_HANDLE_VALUE;

	for (int i=0; i < retry_max && !m_bIsAbort; i++)
	{
		if ((hFile = ::CreateFileW(path, mode, share, sa, cr_mode, flg, hTempl)) != INVALID_HANDLE_VALUE)
			break;

		if (::GetLastError() != ERROR_SHARING_VIOLATION)
			break;

		::Sleep(i * i * 10);
	}

	return	hFile;
}

BOOL CCopyEngine::WrieFileWithReduce(HANDLE hFile, void *pBuf, DWORD size, DWORD *written, OVERLAPPED *pOverWrap)
{
	DWORD	dwTrans = 0;
	DWORD	dwTotal = 0;
	DWORD	maxWriteSizeSv = m_dwMaxWriteSize;

	while ((dwTrans = size - dwTotal) > 0 && !m_bIsAbort)
	{
		DWORD	dwTransed = 0;
		dwTrans = min(dwTrans, m_dwMaxWriteSize);
		if (!::WriteFile(hFile, (BYTE *)pBuf + dwTotal, dwTrans, &dwTransed, pOverWrap))
		{
			int nErr = ::GetLastError();
			if (nErr != ERROR_NO_SYSTEM_RESOURCES || min(size, m_dwMaxWriteSize) <= REDUCE_SIZE)
			{
				return	FALSE;
			}
			m_dwMaxWriteSize -= REDUCE_SIZE;
			m_dwMaxWriteSize = ALIGN_SIZE(m_dwMaxWriteSize, REDUCE_SIZE);
		}
		dwTotal += dwTransed;

		// 暂停等待
		m_Condition.WaitPause(m_bIsAbort);
	}
	*written = dwTotal;

	return	TRUE;
}

BOOL CCopyEngine::WriteFileProc()
{
	BOOL	bRet = FALSE;
	HANDLE	hFile = INVALID_HANDLE_VALUE;
	FileStat *pStat = &m_pWriteReqHeader->FStat;
	_int64	nFileSize = pStat->lnFileSize.QuadPart;
	_int64	nRemainSize = nFileSize;
	DWORD	dwTransSize = 0;
	Command	command = m_pWriteReqHeader->HeaderCommand;
	BOOL bIsNonbuf = (m_pCurPathObj->DstFsType != DT_NETWORK)
					&& (nFileSize >= m_nNonbufMinSize || (nFileSize % m_pCurPathObj->nDstSectorSize) == 0)
					&& ((OPEN_FILE_FLAGS & USE_OSCACHE_WRITE) == 0);
	// 写入正确的文件尾标志用(无缓冲并且文件长度不是扇区对齐)
	BOOL bIsReopen = bIsNonbuf && (nFileSize % m_pCurPathObj->nDstSectorSize);
	DWORD dwFlags = (bIsNonbuf ? FILE_FLAG_NO_BUFFERING : 0) | FILE_FLAG_SEQUENTIAL_SCAN;

	hFile = ::CreateFileW(m_pDstPath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, dwFlags, 0);
	if (!IS_SAVE_HANDLE(hFile))
	{
		::SetFileAttributesW(m_pDstPath, FILE_ATTRIBUTE_NORMAL);
		hFile = ::CreateFileW(m_pDstPath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, dwFlags, 0);
	}

	if (!IS_SAVE_HANDLE(hFile))
		goto END;

	// 创建文件
	if (nFileSize > m_pWriteReqHeader->nHeaderBufSize)
	{
		// 创建文件的大小为目标磁盘扇区大小对齐。
		_int64	nAllocSize = bIsNonbuf ? ALIGN_SIZE(nFileSize, m_pCurPathObj->nDstSectorSize) : nFileSize;
		LONG	lHighSize = (LONG)(nAllocSize >> 32);
		::SetFilePointer(hFile, (LONG)nAllocSize, &lHighSize, FILE_BEGIN);
		if (!::SetEndOfFile(hFile) && GetLastError() == ERROR_DISK_FULL)
			goto END;

		::SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	}

	while (nRemainSize > 0 && !m_bIsAbort)
	{
		DWORD dwWriteSize = (nRemainSize >= m_pWriteReqHeader->nHeaderBufSize)
			? m_pWriteReqHeader->nHeaderBufSize
			: (DWORD)(bIsNonbuf ? ALIGN_SIZE(nRemainSize, m_pCurPathObj->nDstSectorSize) : nRemainSize);

		dwTransSize = 0;
		if (!(bRet = WrieFileWithReduce(hFile, m_pWriteReqHeader->pHeaderBuf, dwWriteSize, &dwTransSize, NULL)) || dwWriteSize != dwTransSize)
			goto END;

		// 计算速度
		m_CopyInfo.AddCopiedSize(dwTransSize);

		dwTransSize = dwWriteSize;
		if ((nRemainSize -= dwTransSize) > 0 && !m_bIsAbort)
		{
			if (RB_RecvRequest() == FALSE || m_pWriteReqHeader->HeaderCommand != WRITE_FILE)
				goto END;
		}
	}

	if (bIsReopen)
	{
		// 计算速度
		m_CopyInfo.AddCopiedSize(nRemainSize);

		SAFE_CLOSE_HANDLE(hFile);

		dwFlags &= ~FILE_FLAG_NO_BUFFERING;
		hFile = ::CreateFileW(m_pDstPath, GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, dwFlags, 0);
		if (!IS_SAVE_HANDLE(hFile))
		{
			hFile = CreateFileWithRetry(m_pDstPath, GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, dwFlags, 0, 10);
			if (!IS_SAVE_HANDLE(hFile))
				goto END;
		}
	}

	if (bRet)
	{
		::SetFilePointer(hFile, pStat->lnFileSize.LowPart, (LONG *)&(pStat->lnFileSize.HighPart), FILE_BEGIN);
		if (!(bRet = ::SetEndOfFile(hFile)))
			goto END;

		::SetFileTime(hFile, &pStat->ftCreationTime, &pStat->ftLastAccessTime, &pStat->ftLastWriteTime);
		::SetFileAttributesW(m_pDstPath, pStat->dwFileAttributes);
	}

	bRet = TRUE;

END:
	if (!bRet)
	{
		m_CopyInfo.ErrorOne();
		m_pCurSrcFileSta->dwLastError = ::GetLastError();
	}
	else
	{
		m_CopyInfo.SuccessOne();
	}

	SAFE_CLOSE_HANDLE(hFile);
	return bRet;
}

BOOL CCopyEngine::RB_AllocReqBuf(int nReqSize, _int64 n64DataSize, ReqBuf *pReqBuf)
{
	// 当前共享缓存中剩余的可用缓存数
	__w64 int nMaxFree = m_pReadWriteBuf->Buf() + m_pReadWriteBuf->Size() - m_pMBufUsedOffset;
	// 最大读取数据量(当前读取的数据量)
	int nMaxTrans = (n64DataSize > m_dwMaxReadSize) ? m_dwMaxReadSize : (int)n64DataSize;
	// 读写缓存长度：取得当前数据以最大磁盘扇区空间对齐的字节数
	int nSectorDataSize = ALIGN_SIZE(nMaxTrans, m_nMaxSectorSize);
	// 读写控制缓存长度：取得当前数据以8字节对齐的字节数
	int nReqDataSize = ALIGN_SIZE(nReqSize, 8);
	// 当前需要的 reg buffer 的大小
	int nAllNeedBufSize = nSectorDataSize + nReqDataSize;
	// 取得当前空闲缓存中，以磁盘扇区大小对齐的内存地址
	BYTE *pAlignOffset = ((nMaxTrans > 0) ? (BYTE *)ALIGN_SIZE((u_int)m_pMBufUsedOffset, m_nMaxSectorSize) : m_pMBufUsedOffset);
	// 计算最后剩余的缓存空间
	nMaxFree -= (pAlignOffset - m_pMBufUsedOffset);

	// 需要的缓存的数量不允许低于一个磁盘扇区的大小
	if (nMaxTrans > 0 && nAllNeedBufSize < nSectorDataSize)
		nAllNeedBufSize = nSectorDataSize;

	// 需要的空间超过剩余空间，就从头开始。
	if (nAllNeedBufSize > nMaxFree)
		pAlignOffset = m_pReadWriteBuf->Buf();

	pReqBuf->pBuf = pAlignOffset;
	pReqBuf->nBufSize = nSectorDataSize;
	pReqBuf->pReqHeader = (ReqHeader *)(pAlignOffset + nSectorDataSize);
	pReqBuf->nReqHeaderSize = nReqDataSize;

	while (!m_WriteReqList.IsEmpty() && !m_bIsAbort)
	{
		if (pReqBuf->pBuf == m_pReadWriteBuf->Buf())
		{
			if (m_pMBufFreeOffset < m_pMBufUsedOffset && (m_pMBufFreeOffset - m_pReadWriteBuf->Buf()) >= nAllNeedBufSize)
				break;
		}
		else
		{
			if (m_pMBufFreeOffset < m_pMBufUsedOffset || pReqBuf->pBuf + nAllNeedBufSize <= m_pMBufFreeOffset)
				break;
		}
		m_Condition.Wait(CV_WAIT_TICK);
	}

	m_pMBufUsedOffset = pReqBuf->pBuf + nAllNeedBufSize;
	return !m_bIsAbort;
}

// 准备通信用的 reg buffer
BOOL CCopyEngine::RB_PrepareReqBuf(int nReqSize, _int64 n64DataSize, ReqBuf *pReqBuf)
{
	m_Condition.Lock();
	BOOL bRet = RB_AllocReqBuf(nReqSize, n64DataSize, pReqBuf);
	m_Condition.UnLock();
	return	bRet;
}

BOOL CCopyEngine::RB_SendRequest(Command command, ReqBuf *pReqBuf, FileStat *pStat)
{
	BOOL bRet = TRUE;
	ReqBuf TmpBuf;

	m_Condition.Lock();

	if (pReqBuf == NULL)
	{
		pReqBuf = &TmpBuf;
		bRet = RB_AllocReqBuf(offsetof(ReqHeader, FStat) + (pStat ? pStat->nFileStatMinSize : 0), 0, pReqBuf);
	}

	if (bRet)
	{
		ReqHeader *pReadReq = pReqBuf->pReqHeader;
		pReadReq->nHeaderReqSize = pReqBuf->nReqHeaderSize;
		pReadReq->HeaderCommand = command;
		pReadReq->pHeaderBuf = pReqBuf->pBuf;
		pReadReq->nHeaderBufSize = pReqBuf->nBufSize;

		if (pStat != NULL)
			memcpy(&pReadReq->FStat, pStat, pStat->nFileStatMinSize);

		m_WriteReqList.AddObj(pReadReq);
		m_Condition.Notify();
	}

	m_Condition.UnLock();

	return	bRet;
}

BOOL CCopyEngine::RB_RecvRequest(void)
{
	m_Condition.Lock();

	if (m_pWriteReqHeader != NULL)
		WriteReqDone();

	CheckDstRequest();

	while (m_WriteReqList.IsEmpty() && !m_bIsAbort)
	{
		m_Condition.Wait(CV_WAIT_TICK);
		CheckDstRequest();
	}
	m_pWriteReqHeader = m_WriteReqList.TopObj();

	m_Condition.UnLock();

	return	m_pWriteReqHeader && !m_bIsAbort;
}

void CCopyEngine::WriteReqDone(void)
{
	m_WriteReqList.DelObj(m_pWriteReqHeader);

	m_pMBufFreeOffset = (BYTE *)m_pWriteReqHeader + m_pWriteReqHeader->nHeaderReqSize;
	if (m_WriteReqList.IsEmpty())
		m_Condition.Notify();

	m_pWriteReqHeader = NULL;
}

void CCopyEngine::EndOnePath(DWORD dwWait)
{
	if (IS_SAVE_HANDLE(m_hReadThread))
	{
		if (::WaitForSingleObject(m_hReadThread, dwWait) != WAIT_TIMEOUT)
		{
			SAFE_CLOSE_HANDLE(m_hReadThread);
		}
	}

	if (IS_SAVE_HANDLE(m_hWriteThread))
	{
		if (::WaitForSingleObject(m_hWriteThread, dwWait) != WAIT_TIMEOUT)
		{
			SAFE_CLOSE_HANDLE(m_hWriteThread);
		}
	}
}

void CCopyEngine::End(void)
{
	m_PathList.ClearAll();
	m_bIsAbort = TRUE;

	while (m_hWriteThread || m_hReadThread)
	{
		m_Condition.Lock();
		m_Condition.Notify();
		m_Condition.UnLock();

		EndOnePath();
	}

	SAFE_CLOSE_HANDLE(m_hStartThread);
}

void CCopyEngine::FdatToFileStat(WIN32_FIND_DATAW *FindDat, FileStat *pStat)
{
	pStat->ftCreationTime = FindDat->ftCreationTime;
	pStat->ftLastAccessTime = FindDat->ftLastAccessTime;
	pStat->ftLastWriteTime = FindDat->ftLastWriteTime;
	pStat->lnFileSize.LowPart = FindDat->nFileSizeLow;
	pStat->lnFileSize.HighPart = FindDat->nFileSizeHigh;
	pStat->dwFileAttributes = FindDat->dwFileAttributes;
	pStat->hFile = INVALID_HANDLE_VALUE;
	pStat->dwLastError = 0;

	int	nLen = (::swprintf_s((WCHAR*)pStat->cFileName, MAX_PATH, L"%s", (WCHAR*)(FindDat->cFileName)) + 1) * WCHAR_SIZE;
	nLen = nLen + offsetof(FileStat, cFileName);
	pStat->nFileStatMinSize = ALIGN_SIZE(nLen, 8);
}

BOOL CCopyEngine::WaitEnd(void)
{
	return m_Condition.WaitEnd(ALL_FILE_END, m_bIsAbort);
}

void CCopyEngine::PauseContinue(void)
{
	if (IsRuning())
	{
		m_bIsPause = !m_bIsPause;
		m_Condition.NotifyPause(m_bIsPause);

		if (m_bIsPause)
			m_CopyInfo.BeginPause();
		else
			m_CopyInfo.EndPause();
	}
}

void CCopyEngine::Stop(void)
{
	if (IsRuning())
	{
		m_bIsAbort = true;
		m_bIsPause = FALSE;
		m_Condition.NotifyPause(m_bIsPause);
	}
}