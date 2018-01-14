#pragma once

class CSyncLocker
{
public:
	CSyncLocker(){ 
		::InitializeCriticalSection(&m_cs);
		m_bIsLocked = FALSE;
	}
	~CSyncLocker(){
		if(IsLocked())
		{
			UnLock();
		}
		::DeleteCriticalSection(&m_cs);
	}
    inline   void     Lock()
	{
		if(!m_bIsLocked)
		{
			EnterCriticalSection(&m_cs);
			m_bIsLocked = TRUE;
		}
	}
	inline   void     UnLock()
	{
		if(m_bIsLocked)
		{
			LeaveCriticalSection(&m_cs);
			m_bIsLocked = FALSE;
		}
	}
	inline   BOOL     IsLocked()
	{
		return m_bIsLocked == TRUE;
	}
private:
	CRITICAL_SECTION  m_cs;
	BOOL              m_bIsLocked;
};