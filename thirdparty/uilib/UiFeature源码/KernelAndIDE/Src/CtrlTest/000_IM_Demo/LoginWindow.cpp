#include "StdAfx.h"
#include "LoginWindow.h"
#include <assert.h>
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include <Shellapi.h>
#include <windows.h>

CLoginWindow::CLoginWindow()
{
	m_dwMainThreadId = 0;
	m_pDialogBkImgBox = NULL;
}

CLoginWindow::~CLoginWindow()
{
}

bool CLoginWindow::ShowLoginWindow(DWORD dwMainThreadId)
{
	m_dwMainThreadId = dwMainThreadId;

	// 取得UI内核DLL的路径
	CStringW strKernelDllPath = PathHelperW(_T(NAME_KERNEL_DLL));
	// 取得皮肤包的路径
	CStringW strSkinPath = PathHelperW(_T("SkinPackage\\000_IM_Demo\\IM_LoginWindow.ufd"));

	CRect WndRct(0, 0, 0, 0);
	// 根据皮肤包创建窗口，如果窗口大小为0，则会根据皮肤包中的大小自动设置窗口的大小。
	// 第一个参数为皮肤包密码
	return CreateFeatureWindow(NULL, strKernelDllPath, strSkinPath, L"LoginWindow", NULL, WndRct, SW_SHOW, WS_OVERLAPPED | WS_THICKFRAME, NULL);
}

void CLoginWindow::OnKeyDown(int nVirtKey, int nFlag)
{
}

// 控件发送给窗口的消息接口
LRESULT CLoginWindow::OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam)
{
	// 子类的 OnCtrlMessage 函数必须先调用父类的 OnCtrlMessage 函数，再执行自己函数内部的操作
	CUiFeatureWindow::OnCtrlMessage(pCtrl, nMsgId, wParam, lParam);


	return 0;
}

// 本窗口的消息处理函数
LRESULT CLoginWindow::WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	return CUiFeatureWindow::WndProc(nMsgId, wParam, lParam);
}

// WM_CREATE：对话框初始化	
void CLoginWindow::OnCreate()
{
	m_pDialogBkImgBox = dynamic_cast<ICtrlImageBox*>(m_pWindowBase->GetControl("DlgBk"));
	if (m_pDialogBkImgBox == NULL)
	{
		assert(false);
		return;
	}
}

void CLoginWindow::UnInitialized()
{
	if (m_pUiKernel != NULL)
		m_pUiKernel->UnInitialized();
}
