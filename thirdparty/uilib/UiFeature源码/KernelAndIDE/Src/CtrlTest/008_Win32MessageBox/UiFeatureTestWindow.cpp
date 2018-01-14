#include "StdAfx.h"
#include "UiFeatureTestWindow.h"
#include <assert.h>
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include <Shellapi.h>
#include <windows.h>
#include "..\..\HighQualitySkin\Win32MessageBox\Win32MessageBox.h"

CUiFeatureTestWindow::CUiFeatureTestWindow()
{
	m_dwMainThreadId = 0;
	m_pDialogBkImgBox = NULL;
	m_pShowDirBtn = NULL;
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
	CStringW strSkinPath = PathHelperW(_T("..\\SkinPackage\\008_Win32MessageBox\\008_Win32MessageBox.ufd"));

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

// 本窗口的消息处理函数
LRESULT CUiFeatureTestWindow::WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	return CUiFeatureWindow::WndProc(nMsgId, wParam, lParam);
}

// WM_CREATE：对话框初始化	
void CUiFeatureTestWindow::OnCreate()
{
	m_pShowDirBtn = GET_CONTROL(ICtrlShadowButton, "OpenMsgBox");
	if (m_pShowDirBtn == NULL)
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

LRESULT CUiFeatureTestWindow::OnDirBtnClick(WPARAM wParam, LPARAM lParam)
{
	CWin32MessageBox msgBox;

	// 取得皮肤包的路径
	CStringW strSkinPath = PathHelperW(_T("..\\SkinPackage\\999_HighQualitySkin\\Win32MessageBox.ufd"));

	msgBox.MessageBox(m_hWnd, m_pUiKernel, strSkinPath, L"我的朋友，UiFeature提醒您：", L"欢迎加入UiFeature的大家庭！\n我们将持续不断的更新UiFeature，让大家用上高效、易用的UI引擎！", MB_OKCANCEL | MB_ICONWARNING);

	return S_OK;
}

LRESULT CUiFeatureTestWindow::OnSysCommandClose(WPARAM wParam, LPARAM lParam)
{
	::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	return S_OK;
}
