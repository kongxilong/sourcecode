#include "StdAfx.h"
#include "CloneWindow.h"
#include <assert.h>
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include <Shellapi.h>
#include <windows.h>

CCloneWindow::CCloneWindow()
{
	m_pShowDirBtn = NULL;
	m_pClonePanel = NULL;
	m_pNewClonePanel = NULL;
}

CCloneWindow::~CCloneWindow()
{
}

bool CCloneWindow::ShowCloneWindow(IUiFeatureKernel* pUiKernel, HWND hParentWnd)
{
	if (pUiKernel == NULL)
		return false;

	// 取得皮肤包的路径
	CStringW strSkinPath = PathHelperW(_T("..\\SkinPackage\\CloneControl_Test\\CloneControl_Test.ufd"));

	CRect WndRct(0, 0, 0, 0);
	// 根据皮肤包创建窗口，如果窗口大小为0，则会根据皮肤包中的大小自动设置窗口的大小。
	// 第一个参数为皮肤包密码
	return CreateFeatureWindowByKernel(NULL, pUiKernel, strSkinPath, L"clonewnd", hParentWnd, WndRct, SW_SHOW, WS_POPUP, NULL);
}

void CCloneWindow::OnKeyDown(int nVirtKey, int nFlag)
{
	// 测试程序按下【Esc】就退出程序
	if (nVirtKey == VK_ESCAPE)
	{
		this->CloseWindow();
	}
}

// WM_CREATE：对话框初始化	
void CCloneWindow::OnCreate()
{
	m_pShowDirBtn = GET_CONTROL(ICtrlShadowButton, "CloneBtn");
	m_pClonePanel = GET_CONTROL(ICtrlPanel, "clonepanel");
	if (m_pShowDirBtn == NULL || m_pClonePanel == NULL)
	{
		assert(false);
		return;
	}
}

LRESULT CCloneWindow::OnDirBtnClick(WPARAM wParam, LPARAM lParam)
{
	if (m_pClonePanel == NULL)
		return S_FALSE;

	if (m_pNewClonePanel == NULL)
	{
		m_pNewClonePanel = (ICtrlPanel*)m_pClonePanel->Clone(NULL, L"newPanel", FALSE, NULL);
		if (m_pNewClonePanel != NULL)
		{
			m_pNewClonePanel->SetDeleteCloneProp(true);
			RECT wndRct = m_pNewClonePanel->GetWindowRect();
			int nH = RECT_HEIGHT(wndRct);
			wndRct.top = wndRct.bottom;
			wndRct.bottom = wndRct.top + nH;
			m_pNewClonePanel->SetWindowRect(wndRct, true);
		}
	}
	else
	{
		if (!this->m_pWindowBase->DeleteSubControl(dynamic_cast<IControlBase*>(m_pNewClonePanel)))
			MessageBoxW(NULL, L"删除控件错误了！", L"error", MB_OK);

		m_pNewClonePanel = NULL;
	}
	this->RedrawWindow();

	return S_OK;
}
