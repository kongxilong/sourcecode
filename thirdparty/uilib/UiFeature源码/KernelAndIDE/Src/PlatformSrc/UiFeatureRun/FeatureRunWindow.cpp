#include "StdAfx.h"
#include "FeatureRunWindow.h"
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"

#define _RUN_PASSWORD_WORD_				"(0xFEFDFCFB)-*-(0xFEFDFCFB)!@#fangshunbao@163.com"

CFeatureRunWindow::CFeatureRunWindow()
{
	m_dwMainThreadId = 0;
	this->SetClassName(L"UiFeatureRunWindow");
}

CFeatureRunWindow::~CFeatureRunWindow()
{
}

bool CFeatureRunWindow::ShowFeatureRunWindow(LPCWSTR pCmdLine, DWORD dwMainThreadId)
{
	if (pCmdLine == NULL || wcslen(pCmdLine) <= 0)
		return false;
	m_dwMainThreadId = dwMainThreadId;

//	MessageBox(NULL, A2W_F(pCmdLine), _T("预览窗口"), MB_OK | MB_ICONERROR);

	CStringW strKernelDllPath = PathHelperW(_T(NAME_KERNEL_DLL));

	CStringW strCmd = pCmdLine;
	CStringW strFirst = strCmd.Mid(1, strCmd.GetLength() - 1);
	strCmd = strFirst;
	strFirst = strCmd.Left(strCmd.Find('\"'));
	CStringW strSecond = strCmd.Mid(strFirst.GetLength() + 3, strCmd.GetLength() - strFirst.GetLength() - 3 - 1);

	CRect WndRct(0, 0, 0, 0);
	bool bRet = CreateFeatureWindow(_RUN_PASSWORD_WORD_, strKernelDllPath, strFirst, strSecond, NULL, WndRct, SW_SHOW);
	return bRet;
}

void CFeatureRunWindow::OnKeyDown(int nVirtKey, int nFlag)
{
	if (nVirtKey == VK_ESCAPE)
		OnExitWindow();
}

void CFeatureRunWindow::OnCreate()
{
	if (m_pWindowBase != NULL)
		m_pWindowBase->SetAnimationTimer();

	this->CenterWindow();
}

void CFeatureRunWindow::UnInitialized()
{
	if (m_pUiKernel != NULL)
		m_pUiKernel->UnInitialized();
}

LRESULT CFeatureRunWindow::OnUiFeatureKernelMsg(WPARAM wParam, LPARAM lParam, BOOL &bContinue)
{
	//// 标题栏控件发出的控制rgn的消息
	//if (wParam != WID_SET_WINDOW_RGN || m_pWindowBase == NULL)
	//	return -1;

	//RECT WndRect = { 0, 0, 0, 0 };
	//WndRect = this->GetClientRect();
	//CRgn Rgn;
	//Rgn.CreateRectRgnIndirect(&WndRect);
	//::SetWindowRgn(m_hWnd, (HRGN)Rgn, TRUE);

	return 0;
}

// 关闭对话框的消息，来源于系统消息：WM_SYSCOMMAND - SC_CLOSE
void CFeatureRunWindow::OnExitWindow()
{
	this->CloseWindow();
	::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
}

LRESULT CFeatureRunWindow::OnSysCommandClose(WPARAM wParam, LPARAM lParam)
{
	::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	return S_OK;
}
