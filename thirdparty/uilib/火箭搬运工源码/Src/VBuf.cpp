
#include "stdafx.h"
#include "VBuf.h"

CVBuf::CVBuf(int nSize)
{
	Init();
	AllocBuf(nSize);
}

CVBuf::~CVBuf()
{
	FreeBuf();
}

void CVBuf::Init(void)
{
	m_pVBuf = NULL;
	m_nSize = 0;
}

BOOL CVBuf::AllocBuf(int nSize)
{
	BOOL bAlloc = false;
	if (nSize > 0 && m_pVBuf == NULL)
	{
		// 保留虚拟地址空间以便以后提交。
		m_pVBuf = (BYTE *)::VirtualAlloc(NULL, nSize + PAGE_SIZE, MEM_RESERVE, PAGE_READWRITE);
		if (m_pVBuf == NULL)
		{
			Init();
		}
		else
		{
			// 在内存或者指定的磁盘页文件(虚拟内存文件)中分配一物理存储区域 函数初始化这个区域为0
			if (::VirtualAlloc(m_pVBuf, nSize, MEM_COMMIT, PAGE_READWRITE))
			{
				m_nSize = nSize;
				bAlloc = TRUE;
			}
			else
			{
				// 取消VirtualAlloc提交的页
				::VirtualFree(m_pVBuf, nSize + PAGE_SIZE, MEM_DECOMMIT);
				Init();
			}
		}
	}

	return bAlloc;
}

BOOL CVBuf::LockBuf(void)
{
	// 指定的内存页面始终保存在物理内存上，不许它交换到磁盘页文件中，对常用的内存页执行这个操作，可以节省时间
	return ::VirtualLock(m_pVBuf, m_nSize);
}

void CVBuf::FreeBuf(void)
{
	if (m_pVBuf != NULL)
	{
		// 释放指定页
		::VirtualFree(m_pVBuf, 0, MEM_RELEASE);
	}

	Init();
}
