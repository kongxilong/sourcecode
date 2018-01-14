
#pragma once

#include "define.h"
#include "TList.h"

// 默认页内存的大小
#define PAGE_SIZE							(4 * 1024)

class CVBuf : public ListObj
{
protected:
	BYTE	*m_pVBuf;
	int		m_nSize;
	void	Init();

public:
	CVBuf(int nSize = 0);
	~CVBuf();
	BOOL AllocBuf(int nSize);
	BOOL LockBuf();
	void FreeBuf();
	BYTE *Buf() { return m_pVBuf; }
	int Size() { return m_nSize; }
};

class CVBufList : public CTList
{
public:
	CVBufList(void) {};
	~CVBufList(void) { ClearAll(); };

	CVBuf *TopObj(void)
	{
		return (CVBuf *)CTList::TopObj();
	};

	CVBuf *NextObj(CVBuf *pReqObj)
	{
		return (CVBuf *)CTList::NextObj(pReqObj);
	};

	BYTE *NewVByte(int nBufLen)
	{
		BYTE *pAlloc = NULL;
		CVBuf *pVBuf = new CVBuf;
		if (pVBuf != NULL)
		{
			if (pVBuf->AllocBuf(nBufLen))
			{
				this->AddObj(pVBuf);
				pAlloc = pVBuf->Buf();
			}
			else
			{
				SAFE_DELETE(pVBuf);
			}
		}

		return pAlloc;
	};

	CVBuf *NewVBuf(int nBufLen)
	{
		CVBuf *pVBuf = new CVBuf;
		if (pVBuf != NULL)
		{
			if (pVBuf->AllocBuf(nBufLen))
			{
				this->AddObj(pVBuf);
			}
			else
			{
				SAFE_DELETE(pVBuf);
			}
		}

		return pVBuf;
	};

	void ClearAll()
	{
		while (!this->IsEmpty())
		{
			CVBuf *pVBuf = this->TopObj();
			if (pVBuf != NULL)
			{
				pVBuf->FreeBuf();
				this->DelObj(pVBuf);
				SAFE_DELETE(pVBuf);
			}
		}
	};
};
