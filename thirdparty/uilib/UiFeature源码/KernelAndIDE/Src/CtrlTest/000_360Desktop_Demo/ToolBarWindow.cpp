#include "StdAfx.h"
#include "ToolBarWindow.h"
#include <assert.h>
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include <Shellapi.h>
#include <windows.h>

CToolBarWindow::CToolBarWindow()
{
	m_dwMainThreadId = 0;
	m_pDialogBkImgBox = NULL;
	m_pShowDirBtn = NULL;
}

CToolBarWindow::~CToolBarWindow()
{
}

bool CToolBarWindow::ShowToolBarWindow(DWORD dwMainThreadId)
{
	m_dwMainThreadId = dwMainThreadId;

	// 取得UI内核DLL的路径
	CStringW strKernelDllPath = PathHelperW(_T(NAME_KERNEL_DLL));
	// 取得皮肤包的路径
	CStringW strSkinPath = PathHelperW(_T("SkinPackage\\007_ICtrlEdit\\007_ICtrlEdit.ufd"));

	CRect WndRct(0, 0, 0, 0);
	// 根据皮肤包创建窗口，如果窗口大小为0，则会根据皮肤包中的大小自动设置窗口的大小。
	// 第一个参数为皮肤包密码
	return CreateFeatureWindow(NULL, strKernelDllPath, strSkinPath, L"TestWindow", NULL, WndRct, SW_SHOW, WS_OVERLAPPED | WS_THICKFRAME, NULL);
}

// 本窗口的消息处理函数
LRESULT CToolBarWindow::WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	return CUiFeatureWindow::WndProc(nMsgId, wParam, lParam);
}

// WM_CREATE：对话框初始化	
void CToolBarWindow::OnCreate()
{
	m_pDialogBkImgBox = GET_CONTROL(ICtrlImageBox, "testc");
	if (m_pDialogBkImgBox == NULL)
	{
		assert(false);
		return;
	}
}

void CToolBarWindow::UnInitialized()
{
	if (m_pUiKernel != NULL)
		m_pUiKernel->UnInitialized();
}

LRESULT CToolBarWindow::OnDirBtnClick(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}
