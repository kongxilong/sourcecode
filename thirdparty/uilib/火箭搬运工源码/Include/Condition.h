
#pragma once

#include "define.h"
#include <windows.h>
#include "TList.h"

enum WaitEvent
{
	CLEAR_EVENT = 0,
	DONE_EVENT,
	WAIT_EVENT,
	ONE_PATH_OBJ_END,
	ALL_FILE_END,
	PAUSE_OP
};

struct EventObj : public ListObj
{
	HANDLE hEvents;
	WaitEvent EventsType;
};

class CEventList : public CTList
{
public:
	CEventList(void) {};
	~CEventList(void) { ClearAll(); };

	EventObj *TopObj(void)
	{
		return (EventObj *)CTList::TopObj();
	};

	EventObj *NextObj(EventObj *pReqObj)
	{
		return (EventObj *)CTList::NextObj(pReqObj);
	};

	EventObj *NewEventObj(WaitEvent EventsType, BOOL bManualReset = FALSE)
	{
		EventObj *pNewObj = new EventObj;
		if (pNewObj != NULL)
		{
			// 当事件被一个等待线程释放以后，系统将会自动将事件状态复原为无信号状态，初始状态为无信号状态
			pNewObj->hEvents = ::CreateEvent(NULL, bManualReset, FALSE, NULL);
			if (pNewObj->hEvents == NULL)
			{
				SAFE_DELETE(pNewObj);
			}
			else
			{
				pNewObj->EventsType = EventsType;
				this->AddObj(pNewObj);
			}
		}

		return pNewObj;
	};

	void DelEventObj(EventObj *pReqObj)
	{
		if (pReqObj != NULL)
		{
			this->DelObj(pReqObj);
			SAFE_CLOSE_HANDLE(pReqObj->hEvents);
			SAFE_DELETE(pReqObj);
		}
	};

	void ClearAll()
	{
		while (!this->IsEmpty())
		{
			EventObj *pPathObj = this->TopObj();
			DelEventObj(pPathObj);
		}
	};
};

class Condition
{
protected:
	// 各种事件列表
	CEventList m_EventList;
	// 单个文件操作结束事件
	EventObj *m_pOnePathEndEvent;
	// 所有文件操作结束事件
	EventObj *m_pAllFileEndEvent;
	// 暂停事件
	EventObj *m_pPauseEvent;

	HANDLE *m_ahEvents;
	WaitEvent *m_aWaitEventsType;

	CRITICAL_SECTION m_cs;
	int m_nMaxThreads;
	int m_nWaitCnt;

public:
	Condition(void);
	~Condition();

	BOOL Initialize(int _nMaxThreads);
	void UnInitialize(void);

	void Lock(void)
	{
		::EnterCriticalSection(&m_cs);
	};

	void UnLock(void)
	{
		::LeaveCriticalSection(&m_cs);
	};

	int WaitThreads()
	{
		return m_nWaitCnt;
	};

	int IsWait()
	{
		return (m_nWaitCnt ? TRUE : FALSE);
	};

	void DetachThread()
	{
		m_nMaxThreads--;
	};

	int  MaxThreads()
	{
		return m_nMaxThreads;
	};

	BOOL WaitEnd(WaitEvent EventType, BOOL &bIsAbort);
	void NotifyEnd(WaitEvent EventType);

	BOOL WaitPause(BOOL &bIsAbort);
	void NotifyPause(BOOL bIsPause);

	BOOL Wait(DWORD dwTimeout = INFINITE);
	void Notify(void);
};
