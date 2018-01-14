#include "StdAfx.h"
#include "WtlTestWindow.h"
#include <assert.h>
#include "..\..\Inc\UiFeatureDefs.h"
#include <Shellapi.h>
#include <windows.h>
#include "resource.h"

CWtlTestWindow::CWtlTestWindow()
{
	m_dwMainThreadId = 0;
	m_pModule = NULL;
	m_DlgIcon = NULL;

	m_pWindowbkPanel = NULL;
}

CWtlTestWindow::~CWtlTestWindow()
{
}

bool CWtlTestWindow::ShowWtlTestWindow(CAppModule *_pModule, DWORD dwMainThreadId)
{
	m_dwMainThreadId = dwMainThreadId;
	m_pModule = _pModule;

	CStringW strKernelDllPath = PathHelperW(_T(NAME_KERNEL_DLL));

	// 取得皮肤包的路径
	CStringW strSkinPath = PathHelperW(_T("..\\SkinPackage\\002_WindowAnimation\\002_WindowAnimation.ufd"));

	// 使用皮肤包中的对话框皮肤信息初始化当前的对话框
	RECT WndRct = {0, 0, 0, 0};
	return (CreateWTLFeatureWindow(m_pModule, strKernelDllPath, strSkinPath, L"TestWindow", NULL,
		NULL, WndRct, SW_SHOW, WS_OVERLAPPED | WS_THICKFRAME, 0, NULL) == TRUE);
}

LRESULT CWtlTestWindow::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int nVirtKey = (int)wParam;

	// 按下esc退出窗口
	if (nVirtKey == VK_ESCAPE)
		CloseTestWindow();

	return 0;
}

void CWtlTestWindow::CloseTestWindow()
{
	// 关闭窗口
	this->PostMessage(WM_CLOSE);
	// 退出主线程
	::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
}

LRESULT CWtlTestWindow::OnUiFeatureMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	return 0;
}

// 对话框初始化	
VOID CWtlTestWindow::OnWTLWindowCreate()
{
//////////////////////////////////////////////////////////////////////////
	// 设置任务栏图标
	m_DlgIcon = ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_UIFEATURERUN));
	if (m_DlgIcon != NULL)
		this->SetIcon(m_DlgIcon);
//////////////////////////////////////////////////////////////////////////

	if (m_pWindowBase == NULL)
		return;

	// 取得界面上需要使用的控件指针
	m_pWindowbkPanel = GET_CONTROL(ICtrlImageBox, "DlgBk");
	if (m_pWindowbkPanel == NULL)
	{
		assert(false);
		return;
	}
}

void CWtlTestWindow::UnInitialized()
{
	if (m_pUiKernel != NULL)
		m_pUiKernel->UnInitialized();
}

LRESULT CWtlTestWindow::OnSysCommandClose(WPARAM wParam, LPARAM lParam)
{
	::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	return S_OK;
}
