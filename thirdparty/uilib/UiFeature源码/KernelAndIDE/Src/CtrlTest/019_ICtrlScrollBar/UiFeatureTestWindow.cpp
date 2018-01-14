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
	m_pTestSc = NULL;
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
	CStringW strSkinPath = PathHelperW(_T("..\\SkinPackage\\019_ScrollBar\\019_ICtrlScrollBar.ufd"));

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
	m_pTestSc = GET_CONTROL(ICtrlScrollBar, "sc");
	if (m_pTestSc == NULL)
	{
		assert(false);
		return;
	}


	SCROLLINFO si;
	m_pTestSc->GetScrollInfo(&si);
	si.nPage = 500;
	si.cbSize = sizeof(SCROLLINFO);
	si.nMin = 0;
	si.nMax = 1000;
	si.fMask = SIF_ALL;

	m_pTestSc->SetScrollInfo(&si);
	m_pTestSc->ShowScrollBar(true);
	m_pTestSc->RedrawControl(true);
}

void CUiFeatureTestWindow::UnInitialized()
{
	if (m_pUiKernel != NULL)
		m_pUiKernel->UnInitialized();
}

LRESULT CUiFeatureTestWindow::OnDirBtnClick(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CUiFeatureTestWindow::OnSysCommandClose(WPARAM wParam, LPARAM lParam)
{
	::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	return S_OK;
}

LRESULT CUiFeatureTestWindow::OnMouseWheel(WPARAM wParam, LPARAM lParam, BOOL bContinue)
{
	bContinue = TRUE;
	if(!m_pTestSc->IsVisible())
	{
		return bContinue;
	}

	UINT nline;
	int fwKeys = GET_KEYSTATE_WPARAM(wParam);
	short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	if (zDelta == 0)
		return bContinue;

	SystemParametersInfo(SPI_GETWHEELSCROLLLINES,0,&nline,0);

	if(MK_CONTROL == fwKeys)
	{
		return bContinue;
	}

	int minPos, maxPos;
	m_pTestSc->GetScrollRange(&minPos, &maxPos);
	int nowPos = m_pTestSc->GetScrollPos() - zDelta/abs(zDelta) * 20;

	nowPos = min(nowPos, maxPos);
	nowPos = max(minPos, nowPos);

	m_pTestSc->SetScrollPos(nowPos);

	return S_OK;
}