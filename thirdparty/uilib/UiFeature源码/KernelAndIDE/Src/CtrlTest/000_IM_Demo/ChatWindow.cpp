#include "StdAfx.h"
#include "ChatWindow.h"
#include <assert.h>
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include <Shellapi.h>
#include <windows.h>

CChatWindow::CChatWindow()
{
	m_pDialogBkImgBox = NULL;
}

CChatWindow::~CChatWindow()
{
}

bool CChatWindow::ShowChatWindow()
{
	// 取得UI内核DLL的路径
	CStringW strKernelDllPath = PathHelperW(_T(NAME_KERNEL_DLL));
	// 取得皮肤包的路径
	CStringW strSkinPath = PathHelperW(_T("SkinPackage\\000_IM_Demo\\IM_ChatWindow.ufd"));

	CRect WndRct(0, 0, 0, 0);
	// 根据皮肤包创建窗口，如果窗口大小为0，则会根据皮肤包中的大小自动设置窗口的大小。
	// 第一个参数为皮肤包密码
	return CreateFeatureWindow(NULL, strKernelDllPath, strSkinPath, L"ChatWindow", NULL, WndRct, SW_SHOW, WS_OVERLAPPED | WS_THICKFRAME, NULL);
}

void CChatWindow::OnKeyDown(int nVirtKey, int nFlag)
{
}

// 控件发送给窗口的消息接口
LRESULT CChatWindow::OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam)
{
	// 子类的 OnCtrlMessage 函数必须先调用父类的 OnCtrlMessage 函数，再执行自己函数内部的操作
	CUiFeatureWindow::OnCtrlMessage(pCtrl, nMsgId, wParam, lParam);


	return 0;
}

// 本窗口的消息处理函数
LRESULT CChatWindow::WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	return CUiFeatureWindow::WndProc(nMsgId, wParam, lParam);
}

// WM_CREATE：对话框初始化	
void CChatWindow::OnCreate()
{
	m_pDialogBkImgBox = dynamic_cast<ICtrlImageBox*>(m_pWindowBase->GetControl("testc"));
	if (m_pDialogBkImgBox == NULL)
	{
		assert(false);
		return;
	}
}
