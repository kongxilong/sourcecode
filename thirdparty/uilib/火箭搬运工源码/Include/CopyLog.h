
#pragma once

class CCopyLog
{
public:
	CCopyLog(void) {};
	~CCopyLog() {};

	virtual void PrintLog(LPCTSTR lpszFormat, ...) = 0;
	virtual void PathEnd(BOOL bIsSuccess, WCHAR *pPath) = 0;
};

extern CCopyLog *g_pCopyLog;

#define OUT_LOG						g_pCopyLog->PrintLog
#define PATH_END_LOG				g_pCopyLog->PathEnd
