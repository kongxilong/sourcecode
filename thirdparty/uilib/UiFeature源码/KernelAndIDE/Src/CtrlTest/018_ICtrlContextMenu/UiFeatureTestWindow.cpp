#include "StdAfx.h"
#include "UiFeatureTestWindow.h"
#include <assert.h>
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include <Shellapi.h>
#include <windows.h>

CUiFeatureTestWindow::CUiFeatureTestWindow()
{
	m_dwMainThreadId = 0;
	m_pDialogBkImgBox = NULL;
	m_pShowMenuBtn = NULL;
}

CUiFeatureTestWindow::~CUiFeatureTestWindow()
{
}

bool CUiFeatureTestWindow::ShowUiFeatureTestWindow(DWORD dwMainThreadId)
{
	m_dwMainThreadId = dwMainThreadId;

	// 取得UI内核DLL的路径
	CStringW strKernelDllPath = PathHelperW(_T(NAME_KERNEL_DLL));
	// 取得皮肤包的路径
	CStringW strSkinPath = PathHelperW(_T("..\\SkinPackage\\018_ContextMenu\\018_ContextMenu.ufd"));

	CRect WndRct(0, 0, 0, 0);
	// 根据皮肤包创建窗口，如果窗口大小为0，则会根据皮肤包中的大小自动设置窗口的大小。
	// 第一个参数为皮肤包密码
	return CreateFeatureWindow(NULL, strKernelDllPath, strSkinPath, L"TestWindow", NULL, WndRct, SW_SHOW, WS_OVERLAPPED | WS_THICKFRAME, NULL);
}

void CUiFeatureTestWindow::OnKeyDown(int nVirtKey, int nFlag)
{
	// 测试程序按下【Esc】就退出程序
	if (nVirtKey == VK_ESCAPE)
	{
		this->CloseWindow();
		::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	}
}

// WM_CREATE：对话框初始化	
void CUiFeatureTestWindow::OnCreate()
{
	m_pContextMenu = GET_CONTROL(ICtrlContextMenu, "m");
	m_pShowMenuBtn = GET_CONTROL(ICtrlShadowButton, "testbtn");
	if (m_pContextMenu == NULL || m_pShowMenuBtn == NULL)
	{
		assert(false);
		return;
	}
}

void CUiFeatureTestWindow::UnInitialized()
{
	if (m_pUiKernel != NULL)
		m_pUiKernel->UnInitialized();
}

LRESULT CUiFeatureTestWindow::OnBtnClick(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	::GetCursorPos(&pt);

	if (m_pContextMenu->CreatePopupMenu())
	{
		m_pContextMenu->TrackPopupMenu(pt);
	}

	return S_OK;
}

LRESULT CUiFeatureTestWindow::OnMenuItem(WPARAM wParam, LPARAM lParam)
{

	int id = (int)wParam;
	if(wParam == 0)
	{
		::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	}
	else if(wParam == 1)
	{
	
		
	}
	else if(wParam == 2)
	{

	}

	return S_OK;
}

LRESULT CUiFeatureTestWindow::OnSysCommandClose(WPARAM wParam, LPARAM lParam)
{
	::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	return S_OK;
}
