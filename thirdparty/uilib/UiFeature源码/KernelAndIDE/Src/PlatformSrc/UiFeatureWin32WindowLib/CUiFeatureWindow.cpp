
#include "stdafx.h"
#include <assert.h>
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\CUiFeatureWindow.h"
#include "..\..\Inc\IUiFeatureKernel.h"
#include "..\..\Inc\ICommonFun.h"

CUiFeatureWindow::CUiFeatureWindow()
{
	m_hWnd = NULL;
	m_pWindowBase = NULL;
	m_strWndText = L"Welcome UiFeature";
	m_hParentWnd = NULL;
	m_lParam = NULL;

	m_nCreateShow = SW_SHOW;

	m_hKernelDll = NULL;
	m_pUiKernel = NULL;
	m_pSkinManager = NULL;
	m_strSkinPath = L"";
	m_strSkinWndName = "";
	m_strUserPassword = "";

	m_bIsStartShow = true;
	m_pStartAnimationCtrl = NULL;
	m_pEndAnimationCtrl = NULL;
}

CUiFeatureWindow::~CUiFeatureWindow()
{
	SAFE_FREE_LIBRARY(m_hKernelDll);
}

bool CUiFeatureWindow::CreateWin32Window(HWND hParent, RECT WndRect, LPCWSTR pszWndText, int nShow, int nStyle, LPARAM lParam)
{
	return false;
}

IWindowBase* CUiFeatureWindow::GetUiFeatureWindow()
{
	return m_pWindowBase;
}

bool CUiFeatureWindow::InitInstance()
{
	m_hWnd = ::CreateWindowEx(0, m_strClassName, m_strWndText, m_nCreateStyle,
		m_rctCreate.left, m_rctCreate.top, RECT_WIDTH(m_rctCreate), RECT_HEIGHT(m_rctCreate), m_hParentWnd, NULL, ::GetModuleHandle(NULL), this);

	// 设置创建对话框参数
	SetCreateWindowParam(m_lParam);

	// 对话框初始化
	if (IS_SAFE_HANDLE(m_hWnd))
	{
		IControlMessage *pCtrlMsg = dynamic_cast<IControlMessage*>(this);
		if (pCtrlMsg == NULL)
		{
			assert(false);
			return false;
		}

		m_pWindowBase = m_pUiKernel->PG_InitFeatureSkin(m_hWnd, m_strSkinPath, m_strSkinWndName, (char*)m_strUserPassword.c_str());
		if (m_pWindowBase == NULL)
		{
			assert(false);
			return false;
		}

		// 设置控件回调
		m_pWindowBase->SetCtrlMsgCallBack(pCtrlMsg);
		if (IS_RECT_EMPTY(m_rctCreate))
		{
			SIZE WndSize = m_pWindowBase->PP_GetWindowPropSize();
			INIT_RECT(m_rctCreate);
			m_rctCreate.right = WndSize.cx;
			m_rctCreate.bottom = WndSize.cy;
			this->MoveWindow(m_rctCreate);
			this->CenterWindow();
		}
		else
		{
			this->MoveWindow(m_rctCreate);
		}

		OnCreate();
		OnCreateEnd();
	}

	return IS_SAFE_HANDLE(m_hWnd);
}

void CUiFeatureWindow::OnCreateEnd()
{
	this->MoveWindow(m_rctCreate);
	this->ShowWindow(m_nCreateShow);
}

LRESULT CUiFeatureWindow::WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	switch (nMsgId)
	{
		// UI内核内部自定义消息值
	case UI_FEATURE_KERNEL_MSG:
		{
			switch((int)wParam)
			{
				// 初始化对话框错误
			case WID_INIT_WINDOW_ERROR:
				OnInitWindowEnd((int)lParam);
				break;

				// 初始化对话框成功
			case WID_INIT_WINDOW_SUCCESS:
				OnInitWindowEnd(0);
				break;

			default:
				break;
			}
		}
		break;

	default:
		break;
	}

	return CWin32Window::WndProc(nMsgId, wParam, lParam);
}

// 初始化对话框结束
void CUiFeatureWindow::OnInitWindowEnd(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		// 成功
	}
	else
	{
		// 失败
		char szError[MAX_PATH];
		memset(szError, 0, MAX_PATH);
		sprintf_s(szError, MAX_PATH, "创建Feature窗口错误，错误码：%d", nErrorCode);

		if (nErrorCode == -2)
		{
			memset(szError, 0, MAX_PATH);
			sprintf_s(szError, MAX_PATH, "创建Feature窗口错误，错误码：%d (亲，有可能皮肤路径错误，或者创建的对话框的名字没有写对奥！)", nErrorCode);
		}
		else if (nErrorCode == -3)
		{
			memset(szError, 0, MAX_PATH);
			sprintf_s(szError, MAX_PATH, "创建Feature窗口错误，错误码：%d (亲，创建控件居然失败了，看看控件DLL都在不在！)", nErrorCode);
		}
		else if (nErrorCode == -4)
		{
			memset(szError, 0, MAX_PATH);
			sprintf_s(szError, MAX_PATH, "创建Feature窗口错误，错误码：%d (亲，密码错误呀！难道亲忘记密码了吗？)", nErrorCode);
		}

#ifdef _DEBUG
		MessageBoxA(NULL, szError, "UiFeatureKernel", MB_OK | MB_ICONERROR);
#else
		DebugInfoOutput(szError);
#endif // _DEBUG
	}
}

// 创建一个窗口，参数：
// char* pszDllDir：UiFeatureKernel.dll的全路径
// char* pszSkinPath：*.ufd皮肤文件的全路径
// char *pszWndName：*.ufd皮肤文件中对话框的名字
// HWND hParent：父窗口
// RECT WndRect：窗口显示位置，坐标为屏幕位置
// int nShow：显示模式，如：SW_SHOW、SW_HIDE
// LPARAM lParam：创建窗口时传入的参数，可以通过 GetCreateWindowParam 函数取得
bool CUiFeatureWindow::CreateFeatureWindow(char* pszPassword, LPCWSTR pszKernelDllPath, LPCWSTR pszSkinPath, LPCWSTR pszSkinWndName, HWND hParent, RECT WndRect, int nShow, int nStyle, LPARAM lParam)
{
	if (pszKernelDllPath == NULL || pszSkinPath == NULL || pszSkinWndName == NULL)
		return false;

	m_strSkinPath = pszSkinPath;
	m_strSkinWndName = pszSkinWndName;
	m_strUserPassword = "";
	if (pszPassword != NULL && strlen(pszPassword) > 0)
		m_strUserPassword = pszPassword;

	m_hKernelDll = ::LoadLibraryW(pszKernelDllPath);
	if (m_hKernelDll == NULL)
	{
		DWORD dwErr = ::GetLastError();
		return false;
	}

	GETUIKERNELINTERFACE GetKernel = (GETUIKERNELINTERFACE)::GetProcAddress(m_hKernelDll, "GetUiKernelInterface");
	if (GetKernel == NULL)
	{
		DWORD dwErr = ::GetLastError();
		SAFE_FREE_LIBRARY(m_hKernelDll);
		return false;
	}

	m_pUiKernel = GetKernel();
	if (m_pUiKernel == NULL)
	{
		DWORD dwErr = ::GetLastError();
		SAFE_FREE_LIBRARY(m_hKernelDll);
		return false;
	}

	m_pSkinManager = m_pUiKernel->GetSkinManager();
	if (m_pSkinManager == NULL)
	{
		m_pUiKernel = NULL;
		SAFE_FREE_LIBRARY(m_hKernelDll);
		return false;
	}

	return CWin32Window::CreateWin32Window(hParent, WndRect, L"Welcome UiFeature", nShow, nStyle, lParam);
}

bool CUiFeatureWindow::CreateFeatureWindowByKernel(char* pszPassword, IUiFeatureKernel* pUiKernel, LPCWSTR pszSkinPath, LPCWSTR pszSkinWndName, HWND hParent, RECT WndRect, int nShow, int nStyle, LPARAM lParam)
{
	if (pUiKernel == NULL || pszSkinPath == NULL || pszSkinWndName == NULL)
		return false;

	m_pUiKernel = pUiKernel;
	m_strSkinPath = pszSkinPath;
	m_strSkinWndName = pszSkinWndName;
	m_strUserPassword = "";
	if (pszPassword != NULL && strlen(pszPassword) > 0)
		m_strUserPassword = pszPassword;

	m_pSkinManager = m_pUiKernel->GetSkinManager();
	if (m_pSkinManager == NULL)
	{
		m_pUiKernel = NULL;
		return false;
	}

	return CWin32Window::CreateWin32Window(hParent, WndRect, L"Welcome UiFeature", nShow, nStyle, lParam);
}

LPCWSTR CUiFeatureWindow::GetSkinPath()
{
	return m_strSkinPath;
}

// 设置对话框启动的时候的动画控件
void CUiFeatureWindow::ShowWindowWithAnimation(IControlBase* pAnimationCtrl)
{
	m_pStartAnimationCtrl = pAnimationCtrl;
}

// 显示窗口
void CUiFeatureWindow::OnShowWindow(bool bIsShow, int nShowStatus)
{
	if (m_pWindowBase != NULL && m_pStartAnimationCtrl != NULL && m_bIsStartShow && bIsShow)
	{
		// 开始启动对话框动画
		m_bIsStartShow = false;
		m_pWindowBase->AppendAnimationControl(m_pStartAnimationCtrl);
		m_pWindowBase->DoControlAnimation();
	}
}

// 播放关闭对话框动画，并关闭窗口
bool CUiFeatureWindow::CloseWindowWithAnimation(IControlBase* pAnimationCtrl)
{
	m_pEndAnimationCtrl = pAnimationCtrl;
	if (m_pEndAnimationCtrl == NULL)
		return false;

	m_pWindowBase->AppendAnimationControl(m_pEndAnimationCtrl);
	m_pWindowBase->DoControlAnimation();
	return true;
}

// 控件发送消息接口
LRESULT CUiFeatureWindow::OnCloseWindowWithAnimation(WPARAM wParam, LPARAM lParam)
{
	m_pEndAnimationCtrl = NULL;
	// 关闭对话框动画结束，并关闭窗口
	this->CloseWindow();

	return S_OK;
}
