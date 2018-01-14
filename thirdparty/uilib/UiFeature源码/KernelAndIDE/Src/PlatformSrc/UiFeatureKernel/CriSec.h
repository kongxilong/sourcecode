
#pragma once
#include <Windows.h>

struct CCriSec
{
	CCriSec() { ::InitializeCriticalSection(&cs); }
	~CCriSec() { ::DeleteCriticalSection(&cs); }
	operator CRITICAL_SECTION*() { return &cs; }
	CRITICAL_SECTION cs;
};

struct CSimpleLock
{
	CSimpleLock(CRITICAL_SECTION* cs) { if (m_cs = cs) ::EnterCriticalSection(m_cs); }
	~CSimpleLock() { if (m_cs) ::LeaveCriticalSection(m_cs); }
	CRITICAL_SECTION* m_cs;
};

// Àý£º
//CCriSec KernelSec;
// CSimpleLock simpleLock(KernelSec)
