#include "StdAfx.h"
#include "DockBarWindow.h"
#include <assert.h>
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include <Shellapi.h>
#include <windows.h>

CDockBarWindow::CDockBarWindow()
{
	m_pDialogBkImgBox = NULL;
	m_pShowDirBtn = NULL;
}

CDockBarWindow::~CDockBarWindow()
{
}

bool CDockBarWindow::ShowDockBarWindow()
{
	// 取得UI内核DLL的路径
	CStringW strKernelDllPath = PathHelperW(_T(NAME_KERNEL_DLL));
	// 取得皮肤包的路径
	CStringW strSkinPath = PathHelperW(_T("SkinPackage\\000_360Desktop_Demo\\DockBarWindow.ufd"));

	CRect WndRct(0, 0, 0, 0);
	// 根据皮肤包创建窗口，如果窗口大小为0，则会根据皮肤包中的大小自动设置窗口的大小。
	// 第一个参数为皮肤包密码
	return CreateFeatureWindow(NULL, strKernelDllPath, strSkinPath, L"DockBarWnd", NULL, WndRct, SW_SHOW, WS_OVERLAPPED | WS_THICKFRAME, NULL);
}

// WM_CREATE：对话框初始化	
void CDockBarWindow::OnCreate()
{
	m_pDialogBkImgBox = GET_CONTROL(ICtrlImageBox, "testc");
	if (m_pDialogBkImgBox == NULL)
	{
		assert(false);
		return;
	}
}

LRESULT CDockBarWindow::OnDirBtnClick(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}
