
#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <atlstr.h>
using namespace ATL;

struct TIPS_INFO
{
	HWND hParentWnd;
	CStringW strTips;
	bool bBaloon;
	int nShowTime;
};

class CSystemTips
{
public:
	CSystemTips();
	~CSystemTips();

	bool ShowSysTips(HWND hParentWnd, LPCWSTR pszTips, bool bBaloon, int nShowTime);
	bool DestroySysTips();

protected:
	static LRESULT CALLBACK TipsWndProc(HWND hWnd, UINT nMsgId, WPARAM wParam, LPARAM lParam);
	void OnTimer(int nTimerId);
	bool ShowSysTipsInTimer();

private:
	HWND m_hTipsWnd;
	TOOLINFO m_ToolInfo;
	WCHAR *m_pTipsData;
	WNDPROC m_spOldWndProc;
	int m_nShowTimerCtns;
	int m_nWaitTimerCtns;
	TIPS_INFO m_TipsInfo;
	bool m_bIsShow;
	POINT m_nBeginShowCurPos;
};
