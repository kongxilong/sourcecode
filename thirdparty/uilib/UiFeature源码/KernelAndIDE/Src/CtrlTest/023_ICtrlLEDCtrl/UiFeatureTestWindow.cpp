#include "StdAfx.h"
#include "UiFeatureTestWindow.h"
#include <assert.h>
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include <Shellapi.h>
#include <windows.h>
#include <atltime.h>

CUiFeatureTestWindow::CUiFeatureTestWindow()
{
	m_dwMainThreadId = 0;
	m_pShowTime = NULL;
}

CUiFeatureTestWindow::~CUiFeatureTestWindow()
{
	::KillTimer(this->GetSafeHandle(),IDC_Timer);
}

bool CUiFeatureTestWindow::ShowUiFeatureTestWindow(DWORD dwMainThreadId)
{
	m_dwMainThreadId = dwMainThreadId;

	// 取得UI内核DLL的路径
	CStringW strKernelDllPath = PathHelperW(_T(NAME_KERNEL_DLL));
	// 取得皮肤包的路径
	CStringW strSkinPath = PathHelperW(_T("..\\SkinPackage\\023_ICtrlLEDCtrl\\023_ICtrlLEDCtrl.ufd"));

	CRect WndRct(0, 0, 0, 0);
	// 根据皮肤包创建窗口，如果窗口大小为0，则会根据皮肤包中的大小自动设置窗口的大小。
	// 第一个参数为皮肤包密码
	return CreateFeatureWindow(NULL, strKernelDllPath, strSkinPath, L"Main", NULL, WndRct, SW_SHOW, WS_OVERLAPPED | WS_THICKFRAME, NULL);
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
	m_pShowTime = GET_CONTROL(ICtrlLEDCtrl, "ShowTime");
	if (m_pShowTime == NULL)
	{
		assert(false);
		return;
	}
	::SetTimer(this->GetSafeHandle(),IDC_Timer,1000,NULL);	
}

void CUiFeatureTestWindow::UnInitialized()
{
	if (m_pUiKernel != NULL)
		m_pUiKernel->UnInitialized();
}

LRESULT CUiFeatureTestWindow::OnSysCommandClose(WPARAM wParam, LPARAM lParam)
{
	::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	return S_OK;
}


LRESULT CUiFeatureTestWindow::OnTimer(WPARAM wParam, LPARAM lParam, BOOL bContinue)
{
	CTime Current= CTime::GetCurrentTime();	//取得目前的系统时间
	long hour, minute, second;	//声明储存时间数据的变量
	hour = Current.GetHour();	//取得目前系统时间的小时
	minute = Current.GetMinute();	//取得目前系统时间的分钟数据
	second = Current.GetSecond();	//取得目前系统时间的秒数据
	if (m_pShowTime!=NULL)
	{
		CStringW szText;
		szText.Format(_T("%02d:%02d:%02d"),hour,minute,second);
		
		m_pShowTime->SetText(szText,true);
	}
	return S_OK;
}