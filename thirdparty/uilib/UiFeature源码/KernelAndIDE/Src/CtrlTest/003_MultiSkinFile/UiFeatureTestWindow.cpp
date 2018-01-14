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
	m_pUpBtn = NULL;
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
	CStringW strSkinPath = PathHelperW(_T("..\\SkinPackage\\003_MultiSkinFile\\Skin_01.ufd"));

	CRect WndRct(0, 0, 0, 0);
	// 根据皮肤包创建窗口，如果窗口大小为0，则会根据皮肤包中的大小自动设置窗口的大小。
	// 第一个参数为皮肤包密码
	return CreateFeatureWindow("123", strKernelDllPath, strSkinPath, L"Dlg_01", NULL, WndRct, SW_SHOW, WS_OVERLAPPED | WS_THICKFRAME, NULL);
}

void CUiFeatureTestWindow::OnKeyDown(int nVirtKey, int nFlag)
{
	// 测试程序按下【Esc】就退出程序
	if (nVirtKey == VK_ESCAPE)
	{
		::DestroyWindow(m_TestWnd2.GetSafeHandle());
		this->CloseWindow();
		::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	}
}

// 控件发送给窗口的消息接口
LRESULT CUiFeatureTestWindow::OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam)
{
	// 子类的 OnCtrlMessage 函数必须先调用父类的 OnCtrlMessage 函数，再执行自己函数内部的操作
	CUiFeatureWindow::OnCtrlMessage(pCtrl, nMsgId, wParam, lParam);

	if (m_pUpBtn != NULL && m_pUpBtn->CompareControl(pCtrl) && nMsgId == CM_BUTTON_UP)
	{
		m_TestWnd2.ShowTestWindow2();
	}
	else if (nMsgId == KCM_SYS_COMMAND_CLOSE)
	{
		::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	}

	return 0;
}

// 本窗口的消息处理函数
LRESULT CUiFeatureTestWindow::WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	return CUiFeatureWindow::WndProc(nMsgId, wParam, lParam);
}

// WM_CREATE：对话框初始化	
void CUiFeatureTestWindow::OnCreate()
{
	GET_CONTROL_EX(m_pUpBtn, ICtrlShadowButton, "BeginBtn");
	if (m_pUpBtn == NULL)
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
