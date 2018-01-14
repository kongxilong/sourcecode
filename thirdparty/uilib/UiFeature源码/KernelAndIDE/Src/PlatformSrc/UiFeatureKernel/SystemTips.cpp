
#include "stdafx.h"
#include "SystemTips.h"
#include <process.h>
#include "..\..\Inc\UiFeatureDefs.h"

#define TIPS_PROP_A_DATA			"uifeaturesystooltips"
#define TIPS_TIMER_ID				(1)
#define TIPS_TIMER_STEP				(100)
// Tips显示等待的时间
#define TIPS_WAIT_SHOW				(500)


LRESULT CALLBACK CSystemTips::TipsWndProc(HWND hWnd, UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	CSystemTips* pTipsWnd = (CSystemTips*)::GetPropA(hWnd, TIPS_PROP_A_DATA);
	if (pTipsWnd == NULL)
		return 0;

	if (nMsgId == WM_TIMER)
	{
		pTipsWnd->OnTimer((int)wParam);
		return 0;
	}

	return ::CallWindowProc(pTipsWnd->m_spOldWndProc, hWnd, nMsgId, wParam, lParam);
}

CSystemTips::CSystemTips()
{
	m_pTipsData = NULL;
	m_hTipsWnd = NULL;
	m_spOldWndProc = NULL;
	m_nShowTimerCtns = 0;
	m_nWaitTimerCtns = 0;
	m_bIsShow = false;
	m_nBeginShowCurPos.x = m_nBeginShowCurPos.y = -1;
	memset(&m_ToolInfo, 0, sizeof(TOOLINFO));

	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_BAR_CLASSES | ICC_TAB_CLASSES | ICC_WIN95_CLASSES;

	InitCommonControlsEx(&icc);
}

CSystemTips::~CSystemTips()
{
	DestroySysTips();
}

bool CSystemTips::ShowSysTips(HWND hParentWnd, LPCWSTR pszTips, bool bBaloon, int nShowTime)
{
	DestroySysTips();

	if (pszTips == NULL || wcslen(pszTips) <= 0 || nShowTime <= 0)
		return false;

	m_TipsInfo.bBaloon = bBaloon;
	m_TipsInfo.hParentWnd = hParentWnd;
	m_TipsInfo.nShowTime = nShowTime;
	m_TipsInfo.strTips = pszTips;

	DWORD dwStyle = WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP;
	if (bBaloon)
		dwStyle = WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP | TTS_BALLOON;

	// CREATE A TOOLTIP WINDOW
	m_hTipsWnd = ::CreateWindowEx(0, //WS_EX_TOPMOST,
		TOOLTIPS_CLASS, NULL,
		dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL);

	if (m_hTipsWnd == NULL)
	{
		DestroySysTips();
		return false;
	}

	// 去掉 Layered 属性，避免tips属性无法显示
	DWORD dwExStyle = ::GetWindowLong(m_hTipsWnd, GWL_EXSTYLE);
	dwExStyle &= (~WS_EX_LAYERED);
	::SetWindowLong(m_hTipsWnd, GWL_EXSTYLE, dwExStyle);

//////////////////////////////////////////////////////////////////////////
	m_spOldWndProc = (WNDPROC)::SetWindowLongPtr(m_hTipsWnd, GWL_WNDPROC, (LONG_PTR)TipsWndProc);
	if (m_spOldWndProc == NULL)
	{
		DestroySysTips();
		return false;
	}
	
	if (!::SetPropA(m_hTipsWnd, TIPS_PROP_A_DATA, (HANDLE)this))
	{
		DestroySysTips();
		return false;
	}	
//////////////////////////////////////////////////////////////////////////

	::SetTimer(m_hTipsWnd, TIPS_TIMER_ID, TIPS_TIMER_STEP, NULL);

	return true;
}

bool CSystemTips::ShowSysTipsInTimer()
{
	if (m_TipsInfo.strTips.GetLength() <= 0)
		return false;

	SAFE_DELETE_LIST(m_pTipsData);
	int nLen = m_TipsInfo.strTips.GetLength() + 1;
	m_pTipsData = new WCHAR[nLen];
	if (m_pTipsData == NULL)
		return false;

	memset(m_pTipsData, 0, nLen);
	wcscpy_s(m_pTipsData, nLen, m_TipsInfo.strTips);

	memset(&m_ToolInfo, 0, sizeof(TOOLINFO));
	// INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE
	m_ToolInfo.cbSize = sizeof(TTTOOLINFOW) - sizeof(void *);
	m_ToolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS; // TTF_TRACK;
	m_ToolInfo.hwnd = m_hTipsWnd;
	m_ToolInfo.hinst = (HINSTANCE)::GetModuleHandle(NULL);
	m_ToolInfo.uId = 0;
	m_ToolInfo.lpszText = m_pTipsData;

	// ToolTip control will cover the whole window
	INIT_RECT(m_ToolInfo.rect);

	// SEND AN ADDTOOL MESSAGE TO THE TOOLTIP CONTROL WINDOW
	LRESULT lRes = ::SendMessage(m_hTipsWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&m_ToolInfo);
	DWORD dwErr = ::GetLastError();

	lRes = ::SendMessage(m_hTipsWnd, TTM_TRACKPOSITION, 0, (LPARAM)(DWORD)MAKELONG(m_nBeginShowCurPos.x, m_nBeginShowCurPos.y));
	dwErr = ::GetLastError();

	lRes = ::SendMessage(m_hTipsWnd, TTM_TRACKACTIVATE, TRUE, (LPARAM)(LPTOOLINFO)&m_ToolInfo);
	dwErr = ::GetLastError();

	m_bIsShow = true;
	return true;
}

bool CSystemTips::DestroySysTips()
{
	if (::IsWindow(m_hTipsWnd))
	{
		::DestroyWindow(m_hTipsWnd);
		m_hTipsWnd = NULL;
	}

	SAFE_DELETE_LIST(m_pTipsData);
	memset(&m_ToolInfo, 0, sizeof(TOOLINFO));
	m_spOldWndProc = NULL;
	m_nShowTimerCtns = 0;
	m_nWaitTimerCtns = 0;
	m_nBeginShowCurPos.x = m_nBeginShowCurPos.y = -1;
	m_bIsShow = false;
	return true;
}

void CSystemTips::OnTimer(int nTimerId)
{
	if (nTimerId != TIPS_TIMER_ID)
		return;

	POINT comPos;
	::GetCursorPos(&comPos);
	if ((comPos.x != m_nBeginShowCurPos.x || comPos.y != m_nBeginShowCurPos.y) && !m_bIsShow)
	{
		m_nBeginShowCurPos = comPos;
		m_nWaitTimerCtns = 0;
		return;
	}

	m_nWaitTimerCtns += TIPS_TIMER_STEP;
	if (m_nWaitTimerCtns >= TIPS_WAIT_SHOW && !m_bIsShow)
	{
		ShowSysTipsInTimer();
		m_nShowTimerCtns = 0;
		return;
	}

	if (m_bIsShow)
	{
		m_nShowTimerCtns += TIPS_TIMER_STEP;
		if (m_nShowTimerCtns >= (m_TipsInfo.nShowTime * 1000))
		{
			DestroySysTips();
			return;
		}
	}
}
