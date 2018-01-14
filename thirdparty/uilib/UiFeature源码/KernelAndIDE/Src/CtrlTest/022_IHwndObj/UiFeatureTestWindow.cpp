#include "StdAfx.h"
#include "UiFeatureTestWindow.h"
#include <assert.h>
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include <Shellapi.h>
#include <windows.h>
#include "..\..\HighQualitySkin\Win32MessageBox\Win32MessageBox.h"


CUiFeatureTestWindow::CUiFeatureTestWindow()
{
	m_dwMainThreadId = 0;
	m_pCheckRadio = NULL;

	m_pCockRadio = NULL;
	m_pClearRadio = NULL;
	m_pRehabRadio = NULL;
	m_pRepairRadio = NULL;
	m_pPcClear = NULL;
	m_pFunctionRadio = NULL;
	m_pSoftwareRadio = NULL;
	m_pHwndBody =NULL;
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
	CStringW strSkinPath = PathHelperW(_T("..\\SkinPackage\\022_IHwndObj\\022_IHwndObj.ufd"));

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

// 本窗口的消息处理函数
LRESULT CUiFeatureTestWindow::WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	return CUiFeatureWindow::WndProc(nMsgId, wParam, lParam);
}

// WM_CREATE：对话框初始化	
void CUiFeatureTestWindow::OnCreate()
{

	m_pCheckRadio = GET_CONTROL(ICtrlRadioBox,"CheckRadio");
	if (m_pCheckRadio == NULL)
	{
		assert(false);
		return;
	}

	m_pCockRadio = GET_CONTROL(ICtrlRadioBox,"CockRadio");
	if (m_pCockRadio == NULL)
	{
		assert(false);
		return;
	}

	m_pClearRadio = GET_CONTROL(ICtrlRadioBox,"ClearRadio");
	if (m_pClearRadio == NULL)
	{
		assert(false);
		return;
	}

	m_pRehabRadio = GET_CONTROL(ICtrlRadioBox,"RehabRadio");
	if (m_pRehabRadio == NULL)
	{
		assert(false);
		return;
	}

	m_pRepairRadio = GET_CONTROL(ICtrlRadioBox,"RepairRadio");
	if (m_pRepairRadio == NULL)
	{
		assert(false);
		return;
	}

	m_pPcClear = GET_CONTROL(ICtrlRadioBox,"PcClear");
	if (m_pPcClear == NULL)
	{
		assert(false);
		return;
	}

	m_pFunctionRadio = GET_CONTROL(ICtrlRadioBox,"FunctionRadio");
	if (m_pFunctionRadio == NULL)
	{
		assert(false);
		return;
	}

	m_pSoftwareRadio = GET_CONTROL(ICtrlRadioBox,"SoftwareRadio");
	if (m_pRepairRadio == NULL)
	{
		assert(false);
		return;
	}


	m_healthcheck.CreateUIWindow(this->GetSafeHandle());
	m_killHorse.CreateUIWindow(this->GetSafeHandle());
	m_clearCard.CreateUIWindow(this->GetSafeHandle());
	m_repairPC.CreateUIWindow(this->GetSafeHandle());
	m_fillHole.CreateUIWindow(this->GetSafeHandle());
	m_computerClear.CreateUIWindow(this->GetSafeHandle());
	m_fucntions.CreateUIWindow(this->GetSafeHandle());
	m_software.CreateUIWindow(this->GetSafeHandle());

	m_pHwndBody = GET_CONTROL(ICtrlHwndObj,"hwndbody");
	if (m_pHwndBody == NULL)
	{
		assert(false);
		return;
	}
	m_pHwndBody->Attach(m_healthcheck.GetSafeHandle());
	m_pHwndBody->Attach(m_killHorse.GetSafeHandle());
	m_pHwndBody->Attach(m_clearCard.GetSafeHandle());
	m_pHwndBody->Attach(m_repairPC.GetSafeHandle());
	m_pHwndBody->Attach(m_fillHole.GetSafeHandle());
	m_pHwndBody->Attach(m_computerClear.GetSafeHandle());
	m_pHwndBody->Attach(m_fucntions.GetSafeHandle());
	m_pHwndBody->Attach(m_software.GetSafeHandle());
	m_pHwndBody->ShowWindow(m_healthcheck.GetSafeHandle());
	
}

void CUiFeatureTestWindow::UnInitialized()
{
	if (m_pUiKernel != NULL)
		m_pUiKernel->UnInitialized();
}

LRESULT CUiFeatureTestWindow::OnCheckRadioClick(WPARAM wParam, LPARAM lParam)
{
	
	if (m_pCheckRadio->GetCheck())
	{
		m_pHwndBody->ShowWindow(m_healthcheck.GetSafeHandle());
		//m_pHwndBody->Detach(m_healthcheck.GetSafeHandle());
	}
	return S_OK;
}

LRESULT CUiFeatureTestWindow::OnCockRadioClick(WPARAM wParam, LPARAM lParam)
{

	if (m_pCockRadio->GetCheck())
	{
		m_pHwndBody->ShowWindow(m_killHorse.GetSafeHandle());
	}
	return S_OK;
}

LRESULT CUiFeatureTestWindow::OnClearRadioClick(WPARAM wParam, LPARAM lParam)
{

	if (m_pClearRadio->GetCheck())
	{
		m_pHwndBody->ShowWindow(m_clearCard.GetSafeHandle());
	}
	return S_OK;
}

LRESULT CUiFeatureTestWindow::OnRehabRadioClick(WPARAM wParam, LPARAM lParam)
{

	if (m_pRehabRadio->GetCheck())
	{
		m_pHwndBody->ShowWindow(m_fillHole.GetSafeHandle());
	}
	return S_OK;
}

LRESULT CUiFeatureTestWindow::OnRepairRadioClick(WPARAM wParam, LPARAM lParam)
{

	if (m_pRepairRadio->GetCheck())
	{
		m_pHwndBody->ShowWindow(m_repairPC.GetSafeHandle());
	}
	return S_OK;
}


LRESULT CUiFeatureTestWindow::OnPcClearClick(WPARAM wParam, LPARAM lParam)
{

	if (m_pPcClear->GetCheck())
	{
		m_pHwndBody->ShowWindow(m_computerClear.GetSafeHandle());
	}
	return S_OK;
}


LRESULT CUiFeatureTestWindow::OnFunctionRadioClick(WPARAM wParam, LPARAM lParam)
{

	if (m_pFunctionRadio->GetCheck())
	{
		m_pHwndBody->ShowWindow(m_fucntions.GetSafeHandle());
	}
	return S_OK;
}


LRESULT CUiFeatureTestWindow::OnSoftwareRadioClick(WPARAM wParam, LPARAM lParam)
{

	if (m_pSoftwareRadio->GetCheck())
	{
		m_pHwndBody->ShowWindow(m_software.GetSafeHandle());
	}
	return S_OK;
}



LRESULT CUiFeatureTestWindow::OnSysCommandClose(WPARAM wParam, LPARAM lParam)
{
	::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	return S_OK;
}
