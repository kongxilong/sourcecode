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
	m_pCloneBtn = NULL;
	m_pTitleBar = NULL;
	m_nAddHeight = 40;
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
	CStringW strSkinPath = PathHelperW(_T("..\\SkinPackage\\010_CloneControl\\010_CloneControl.ufd"));

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
	GET_CONTROL_EX(m_pCloneBtn, ICtrlShadowButton, "CloneBtn");
	GET_CONTROL_EX(m_pTitleBar, ICtrlWindowTitleBar, "TestTitleBar");
	if (m_pCloneBtn == NULL || m_pTitleBar == NULL)
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

LRESULT CUiFeatureTestWindow::OnCloneBtn(WPARAM wParam, LPARAM lParam)
{
	if (m_pTitleBar == NULL)
		return S_FALSE;

	IControlBase* pCloneCtrl = m_pTitleBar->Clone(NULL, L"", TRUE, NULL);
	ICtrlWindowTitleBar* pNewTitle = dynamic_cast<ICtrlWindowTitleBar*>(pCloneCtrl);

	if (pNewTitle == NULL)
		return S_FALSE;

	RECT fromRct = this->GetWindowRect();

	RECT ctrlRct = pNewTitle->GetWindowRect();
	int nH = RECT_HEIGHT(ctrlRct);
	int nW = RECT_WIDTH(ctrlRct);

	ctrlRct.top = m_nAddHeight;
	ctrlRct.bottom = ctrlRct.top + nH;

	pNewTitle->SetWindowRectLayoutWithChild(ctrlRct);

	this->RedrawWindow();

	m_nAddHeight += 40;
	return S_OK;
}

LRESULT CUiFeatureTestWindow::OnSysCommandClose(WPARAM wParam, LPARAM lParam)
{
	::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	return S_OK;
}
