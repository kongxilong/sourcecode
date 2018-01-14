
#include "stdafx.h"
#include "Condition.h"

Condition::Condition(void)
{
	m_ahEvents = NULL;
	m_pOnePathEndEvent = NULL;
	m_pAllFileEndEvent = NULL;
	m_pPauseEvent = NULL;
}

Condition::~Condition(void)
{
	UnInitialize();
}

BOOL Condition::Initialize(int _nMaxThreads)
{
	UnInitialize();

	m_nMaxThreads = _nMaxThreads;

	m_aWaitEventsType = new WaitEvent [m_nMaxThreads];
	m_ahEvents = new HANDLE [m_nMaxThreads];

	for (int nWaitId = 0; nWaitId < m_nMaxThreads; nWaitId++)
	{
		// 当事件被一个等待线程释放以后，系统将会自动将事件状态复原为无信号状态。
		// 初始状态为无信号状态
		m_ahEvents[nWaitId] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if (m_ahEvents[nWaitId] == NULL)
			return	FALSE;

		m_aWaitEventsType[nWaitId] = CLEAR_EVENT;
	}

	::InitializeCriticalSection(&m_cs);

	m_nWaitCnt = 0;

	// 单个文件操作结束事件
	if (m_pOnePathEndEvent == NULL)
		m_pOnePathEndEvent = m_EventList.NewEventObj(ONE_PATH_OBJ_END);

	// 所有文件操作结束事件
	if (m_pAllFileEndEvent == NULL)
		m_pAllFileEndEvent = m_EventList.NewEventObj(ALL_FILE_END);

	// 暂停事件
	if (m_pPauseEvent == NULL)
		m_pPauseEvent = m_EventList.NewEventObj(PAUSE_OP, TRUE);

	if (m_pOnePathEndEvent != NULL || m_pAllFileEndEvent != NULL ||  m_pPauseEvent != NULL)
		return FALSE;

	return	TRUE;
}

void Condition::UnInitialize(void)
{
	if (m_ahEvents != NULL)
	{
		while (--m_nMaxThreads >= 0)
			::CloseHandle(m_ahEvents[m_nMaxThreads]);

		delete [] m_ahEvents;
		delete [] m_aWaitEventsType;

		m_ahEvents = NULL;
		m_aWaitEventsType = NULL;

		m_nWaitCnt = 0;

		::DeleteCriticalSection(&m_cs);
	}
}

BOOL Condition::Wait(DWORD dwTimeout)
{
	int nWaitId = 0;

	for (nWaitId = 0; ((nWaitId < m_nMaxThreads) && (m_aWaitEventsType[nWaitId] != CLEAR_EVENT)); nWaitId++)
		;

	if (nWaitId >= m_nMaxThreads)
		return	FALSE;

	m_aWaitEventsType[nWaitId] = WAIT_EVENT;
	m_nWaitCnt++;

	UnLock();

	DWORD dwStatus = ::WaitForSingleObject(m_ahEvents[nWaitId], dwTimeout);

	Lock();

	--m_nWaitCnt;
	m_aWaitEventsType[nWaitId] = CLEAR_EVENT;

	return ((dwStatus == WAIT_TIMEOUT) ? FALSE : TRUE);
}

void Condition::Notify(void)
{
	if (m_nWaitCnt > 0)
	{
		for (int nWaitId = 0, nDoneCnt = 0; nWaitId < m_nMaxThreads; nWaitId++)
		{
			if (m_aWaitEventsType[nWaitId] == WAIT_EVENT)
			{
				::SetEvent(m_ahEvents[nWaitId]);
				m_aWaitEventsType[nWaitId] = DONE_EVENT;

				if (++nDoneCnt >= m_nWaitCnt)
					break;
			}
		}
	}
}

BOOL Condition::WaitEnd(WaitEvent EventType, BOOL &bIsAbort)
{
	EventObj *pEventObj = NULL;
	if (EventType == ONE_PATH_OBJ_END)
		pEventObj = m_pOnePathEndEvent;
	else if (EventType == ALL_FILE_END)
		pEventObj = m_pAllFileEndEvent;

	DWORD dwStatus = WAIT_TIMEOUT;
	BOOL bRet = FALSE;
	while (pEventObj != NULL && !bIsAbort && !bRet)
	{
		dwStatus = ::WaitForSingleObject(pEventObj->hEvents, 100);
		bRet = ((dwStatus == WAIT_TIMEOUT || dwStatus == WAIT_FAILED) ? FALSE : TRUE);
	}

	return bRet;
}

void Condition::NotifyEnd(WaitEvent EventType)
{
	EventObj *pEventObj = NULL;
	if (EventType == ONE_PATH_OBJ_END)
		pEventObj = m_pOnePathEndEvent;
	else if (EventType == ALL_FILE_END)
		pEventObj = m_pAllFileEndEvent;

	if (pEventObj != NULL)
		::SetEvent(pEventObj->hEvents);
}

BOOL Condition::WaitPause(BOOL &bIsAbort)
{
	DWORD dwStatus = WAIT_TIMEOUT;
	BOOL bRet = FALSE;
	while (m_pPauseEvent != NULL && !bIsAbort)
	{
		dwStatus = ::WaitForSingleObject(m_pPauseEvent->hEvents, 0);
		bRet = ((dwStatus == WAIT_TIMEOUT || dwStatus == WAIT_FAILED) ? FALSE : TRUE);

		if (bRet)
			Sleep(200);
		else
			break;
	}

	return bRet;
}

void Condition::NotifyPause(BOOL bIsPause)
{
	if (m_pPauseEvent != NULL)
	{
		if (bIsPause)
			::SetEvent(m_pPauseEvent->hEvents);
		else
			::ResetEvent(m_pPauseEvent->hEvents);
	}
}
