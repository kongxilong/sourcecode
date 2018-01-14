
#pragma once
#include "UiFeatureBrowserCtrl.h"
#include "..\..\..\Inc\CWin32Window.h"

//////////////////////////////////////////////////////////////////////////
//
class CUiFeatureBrowserWindow : public CWin32Window
{
public:
    CUiFeatureBrowserWindow();
    ~CUiFeatureBrowserWindow();

	HWND CreateUiFeatureBrowserWindow(HWND hParent, INT nShow, LPCWSTR pszUrl);
	VOID OpenUrl(LPCWSTR pszUrl);

protected:
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnSysCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCloseWindowFromSysCommand();


private:
    BOOL CreateUiFeatureBrowserCtrl();
    void DestroyUiFeatureBrowserCtrl();

private:
    CUiFeatureBrowserCtrl m_IeCtrl;
    CString m_strUrl;
	HWND m_hParent;
};
