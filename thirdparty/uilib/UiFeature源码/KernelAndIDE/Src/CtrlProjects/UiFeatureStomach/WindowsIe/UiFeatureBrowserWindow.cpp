
#include "StdAfx.h"
#include "UiFeatureBrowserWindow.h"

#include <atltypes.h>

CUiFeatureBrowserWindow::CUiFeatureBrowserWindow()
{
	m_strUrl = STR_URL_ABOUT_BLANK;
	m_hParent = NULL;
	this->SetClassName(L"UiFeatureWebBrowser");
}

CUiFeatureBrowserWindow::~CUiFeatureBrowserWindow()
{
}

HWND CUiFeatureBrowserWindow::CreateUiFeatureBrowserWindow(HWND hParent, INT nShow, LPCWSTR pszUrl)
{
	if (::IsWindow(m_hWnd))
		return m_hWnd;

	m_hParent = hParent;
	RECT wndRct = {0, 0, 0, 0};
	this->CreateWin32Window(hParent, wndRct, L"UiFeatureBrowser", nShow, WS_POPUP, (WPARAM)this);
	if (!::IsWindow(m_hWnd))
		return NULL;

	if (!CreateUiFeatureBrowserCtrl())
	{
		this->CloseWindow();
		return NULL;
	}

	if (pszUrl != NULL && wcslen(pszUrl) > 0)
		m_strUrl = pszUrl;

	return m_hWnd;
}

BOOL CUiFeatureBrowserWindow::CreateUiFeatureBrowserCtrl()
{
	RECT wndRct = {0, 0, 0, 0};
	m_IeCtrl.Create(NULL, m_strUrl, m_hWnd, &wndRct, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0,
		0U, NULL, DOCHOSTUIFLAG_DIALOG | DOCHOSTUIFLAG_DISABLE_HELP_MENU | /*DOCHOSTUIFLAG_SCROLL_NO |*/
		DOCHOSTUIFLAG_DISABLE_SCRIPT_INACTIVE | DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_NO3DOUTERBORDER |
		DOCHOSTUIFLAG_THEME | DOCHOSTUIFLAG_FLAT_SCROLLBAR);

	m_IeCtrl.ShowWindow(SW_SHOW);
	return TRUE;
}

void CUiFeatureBrowserWindow::DestroyUiFeatureBrowserCtrl()
{
	if (m_IeCtrl.IsWindow())
		m_IeCtrl.DestroyWindow();
}

VOID CUiFeatureBrowserWindow::OpenUrl(LPCWSTR pszUrl)
{
    if (NULL != pszUrl && wcslen(pszUrl) > 0 && m_IeCtrl.IsWindow())
	{
		m_strUrl = pszUrl;
        m_IeCtrl.Navigate(m_strUrl);
    }
}

void CUiFeatureBrowserWindow::OnSize(UINT nType, int cx, int cy)
{
	if (m_IeCtrl.IsWindow())
		::MoveWindow(m_IeCtrl.m_hWnd, 0, 0, cx, cy, TRUE);
}

void CUiFeatureBrowserWindow::OnSysCommand(WPARAM wParam, LPARAM lParam)
{

}

void CUiFeatureBrowserWindow::OnCloseWindowFromSysCommand()
{

}
