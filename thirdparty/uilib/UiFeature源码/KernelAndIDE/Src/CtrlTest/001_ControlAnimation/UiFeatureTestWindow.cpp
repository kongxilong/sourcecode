#include "StdAfx.h"
#include "UiFeatureTestWindow.h"
#include <assert.h>
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\Control\ICtrlLink.h"
#include <Shellapi.h>
#include <windows.h>

CUiFeatureTestWindow::CUiFeatureTestWindow()
{
	m_dwMainThreadId = 0;
	m_nWndAnimationId = 1;

	m_pBeginBtn = NULL;
	m_pAnimationPanel = NULL;
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
	CStringW strSkinPath = PathHelperW(_T("..\\SkinPackage\\001_ControlAnimation\\001_ControlAnimation.ufd"));

	CRect WndRct(0, 0, 0, 0);
	// 根据皮肤包创建窗口，如果窗口大小为0，则会根据皮肤包中的大小自动设置窗口的大小。
	// 第一个参数为皮肤包密码
	return CreateFeatureWindow(NULL, strKernelDllPath, strSkinPath, L"TestWindow", NULL, WndRct, SW_SHOW, WS_OVERLAPPED | WS_THICKFRAME, NULL);
}

void CUiFeatureTestWindow::OnKeyDown(int nVirtKey, int nFlag)
{
	// 测试程序按下【Esc】就退出程序
	if (nVirtKey == VK_ESCAPE)
		EndDialog();
}

void CUiFeatureTestWindow::EndDialog()
{
	this->CloseWindow();
	::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
}

// 控件发送给窗口的消息接口
LRESULT CUiFeatureTestWindow::OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam)
{
	// 子类的 OnCtrlMessage 函数必须先调用父类的 OnCtrlMessage 函数，再执行自己函数内部的操作
	CUiFeatureWindow::OnCtrlMessage(pCtrl, nMsgId, wParam, lParam);

	if (nMsgId == CM_LINK_UP)
	{
		ICtrlLink* pLinkCtrl = dynamic_cast<ICtrlLink*>(pCtrl);
		if (pLinkCtrl == NULL)
			return -1;

		CStringW strUrl = (LPCWSTR)wParam;
		if (strUrl.IsEmpty())
			return -1;

		HINSTANCE hOpen = ::ShellExecute(NULL, _T("open"), strUrl, NULL, NULL, SW_SHOWNORMAL);
		if (hOpen == NULL)
			return -1;
	}
	else if (nMsgId == CM_BUTTON_UP && m_pBeginBtn->CompareControl(pCtrl))
	{
		if (m_nWndAnimationId == 1)
			SetWindowAnimation();
	}
	else if (nMsgId == KCM_SYS_COMMAND_CLOSE)
	{
		::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	}

	return 0;
}

void CUiFeatureTestWindow::SetWindowAnimation()
{
	if (m_pAnimationPanel == NULL || m_pWindowBase == NULL)
		return;

	if (m_nWndAnimationId > 81)
	{
		m_nWndAnimationId = 1;
		::MessageBox(m_hWnd, _T("亲！已经为您播放了79个动画！"), _T("动画"), MB_OK);
		return;
	}

	m_nWndAnimationId++;

	// 设置控件动画类型
	m_pAnimationPanel->PP_SetCtrlAnimationType(m_nWndAnimationId);

	// 向窗口加入动画元素
	m_pWindowBase->AppendAnimationControl(m_pAnimationPanel);

	// 开始动画
	m_pWindowBase->DoControlAnimation();
}

// 本窗口的消息处理函数
LRESULT CUiFeatureTestWindow::WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	if (UI_FEATURE_KERNEL_MSG == nMsgId && WID_ANIMATION_THREAD_END == wParam)
	{
		// 窗口接受到动画线程结束的消息
		SetWindowAnimation();
	}

	return CUiFeatureWindow::WndProc(nMsgId, wParam, lParam);
}

// WM_CREATE：对话框初始化	
void CUiFeatureTestWindow::OnCreate()
{
	GET_CONTROL_EX(m_pAnimationPanel, ICtrlImageBox, "CtrlAniImgBox");
	GET_CONTROL_EX(m_pBeginBtn, ICtrlShadowButton, "BeginBtn");
	if ( m_pAnimationPanel == NULL || m_pBeginBtn == NULL)
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
