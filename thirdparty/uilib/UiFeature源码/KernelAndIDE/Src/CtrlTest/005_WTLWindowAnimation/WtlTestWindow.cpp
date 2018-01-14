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
	m_pWindowbkPanel = NULL;

	m_pAnimationBtn = NULL;
	m_nWndAnimationId = 1;
	m_pModule = NULL;
	m_DlgIcon = NULL;
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
	CStringW strSkinPath = PathHelperW(_T("..\\SkinPackage\\005_WTLWindowAnimation\\005_WTLWindowAnimation.ufd"));

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

// 控件发送给窗口的消息接口
LRESULT CWtlTestWindow::OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam)
{
	if (nMsgId == CM_LINK_UP)
	{
		// 按下了超链接控件
		ICtrlLink* pLinkCtrl = dynamic_cast<ICtrlLink*>(pCtrl);
		if (pLinkCtrl == NULL)
			return -1;

		// 取得超链接的链接地址
		CStringW strUrl = (WCHAR*)wParam;
		if (strUrl.GetLength() <= 0)
			return -1;

		// 打开网页
		HINSTANCE hOpen = ::ShellExecute(NULL, _T("open"), strUrl, NULL, NULL, SW_SHOWNORMAL);
		if (hOpen == NULL)
			return -1;
	}
	else if (m_pAnimationBtn != NULL && nMsgId == CM_BUTTON_UP && m_pAnimationBtn->CompareControl(pCtrl))
	{
		// 按下开始动画按钮
		SetWindowAnimation();
	}
	else if (nMsgId == KCM_SYS_COMMAND_CLOSE)
	{
		::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	}

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
	if (UI_FEATURE_KERNEL_MSG == uMsg && WID_ANIMATION_THREAD_END == wParam)
	{
		// 窗口接受到动画线程结束的消息
		// 因为连续72个动画，需要在每一个动画结束的时候继续下一个动画
		SetWindowAnimation();
	}

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

	GET_CONTROL_EX(m_pWindowbkPanel, ICtrlImageBox, "DlgInBk");
	GET_CONTROL_EX(m_pAnimationBtn, ICtrlShadowButton, "BeginBtn");
	if (m_pWindowbkPanel == NULL || m_pAnimationBtn == NULL)
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

void CWtlTestWindow::SetWindowAnimation()
{
	if (m_pWindowbkPanel == NULL || m_pWindowBase == NULL)
		return;

	if (m_nWndAnimationId > 81)
	{
		m_nWndAnimationId = 1;
		::MessageBox(m_hWnd, _T("亲！已经为您播放了79个动画！"), _T("亲，动画完了："), MB_OK);
		return;
	}

	// 这个是动画类型的ID，支持从 2-73
	m_nWndAnimationId++;

	// 设置控件动画类型
	m_pWindowbkPanel->PP_SetCtrlAnimationType(m_nWndAnimationId);

	// 向窗口加入动画元素
	m_pWindowBase->AppendAnimationControl(m_pWindowbkPanel);

	// 开始动画
	m_pWindowBase->DoControlAnimation();
}
