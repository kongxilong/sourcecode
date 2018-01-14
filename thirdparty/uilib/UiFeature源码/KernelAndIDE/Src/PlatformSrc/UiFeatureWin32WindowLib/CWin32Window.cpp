
#include "stdafx.h"
#include <assert.h>
#include <shobjidl.h>
#include <process.h>
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\CWin32Window.h"

W32_Wnd_MAP CWin32Window::ms_Win32WndMap;

LRESULT CWin32Window::Win32WndProc(HWND hWnd, UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	switch (nMsgId)
	{
		// 创建窗口消息
	case WM_NCCREATE:
		{
			CWin32Window *pDlg = (CWin32Window *)(((LPCREATESTRUCT)lParam)->lpCreateParams);
			if (pDlg != NULL)
			{
				ms_Win32WndMap.insert(pair<HWND, CWin32Window*>(hWnd, pDlg));
				pDlg->SetSafeHandle(hWnd);
				return pDlg->WndProc(nMsgId, wParam, lParam);
			}
		}
		break;

		// 其他消息
	default:
		{
			W32_Wnd_MAP::iterator pDuiWnd = ms_Win32WndMap.find(hWnd);
			if (pDuiWnd != ms_Win32WndMap.end())
			{
				CWin32Window *pDlg = pDuiWnd->second;
				if (nMsgId == WM_NCDESTROY)
					ms_Win32WndMap.erase(pDuiWnd);

				if (pDlg != NULL)
				{
					LRESULT lRst = pDlg->WndProc(nMsgId, wParam, lParam);
					if (nMsgId == WM_NCDESTROY)
					{
						pDlg->SetSafeHandle(NULL);
					}
					return lRst;
				}
			}
		}
		break;
	}

	return ::DefWindowProc(hWnd, nMsgId, wParam, lParam);
}

CWin32Window::CWin32Window()
{
	::CoInitialize(NULL);

	m_hWnd = NULL;
	m_strWndText = L"Welcome UiFeature";
	m_strClassName = L"UiFeatureWindow";
	m_hParentWnd = NULL;
	m_lParam = NULL;

	INIT_RECT(m_rctCreate);
	m_nCreateShow = SW_SHOW;
	m_bIsCreateWithThread = false;
	m_bStopDrawWindow = false;
	m_hDlgIcon = NULL;
}

CWin32Window::~CWin32Window()
{
	CloseWindow();
	SAFE_DELETE_OBJECT(m_hDlgIcon)
}

// 创建窗口
bool CWin32Window::CreateWin32Window(HWND hParent, RECT WndRect, LPCWSTR pszWndText, int nShow, int nStyle, LPARAM lParam)
{
	m_bIsCreateWithThread = false;
	m_nCreateStyle = nStyle;
	m_hParentWnd = hParent;
	m_rctCreate = WndRect;
	m_nCreateShow = nShow;
	m_lParam = lParam;

	if (pszWndText != NULL)
		m_strWndText = pszWndText;

	RegisterClass();
	return InitInstance();
}

LRESULT CWin32Window::WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	if (!::IsWindow(m_hWnd))
		return 0;

	switch (nMsgId)
	{
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		OnKeyDown((int)wParam, (int)lParam);
		break;

	case WM_NCDESTROY:
		break;

	case WM_SHOWWINDOW:
		OnShowWindow((bool)(wParam != 0), (int)lParam);
		break;

		// WM_TIMER：定时器	
	case WM_TIMER:
		OnTimer((UINT)wParam);
		break;

	case WM_KILLFOCUS:
		{
			LRESULT lRes = ::DefWindowProc(m_hWnd, nMsgId, wParam, lParam);
			OnKillFocus();
			return lRes;
		}
		break;

	case WM_SETFOCUS:
		OnSetFocus();
		break;

		// WM_DESTROY：销毁窗口
	case WM_DESTROY:
		OnDestroy();
		break;

	case WM_SIZE:
		{
			LRESULT lRes = ::DefWindowProc(m_hWnd, nMsgId, wParam, lParam);
			OnSize((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
			return lRes;
		}
		break;

	case WM_ENTERSIZEMOVE:
		OnEnterSizeMove();
		break;

	case WM_EXITSIZEMOVE:
		OnExitSizeMove();
		break;

	case WM_PAINT:
		{
			if (!m_bStopDrawWindow)
			{
				PAINTSTRUCT ps;
				HDC hDc = ::BeginPaint(m_hWnd, &ps);
				OnPaint(hDc);
				::EndPaint(m_hWnd, &ps);
				return TRUE;
			}
		}
		break;

	case WM_NCCALCSIZE:
		return FALSE;

	case WM_ERASEBKGND:
		if (OnEraseBkgnd((HDC)wParam))
			return TRUE;
		break;

	case WM_NCPAINT:
	case WM_NCACTIVATE:
	case 0x00AE:	// WM_NCUAHDRAWCAPTION
	case 0x00AF:	// WM_NCUAHDRAWFRAME
		return WM_PAINT;

	default:
		break;
	}

	return ::DefWindowProc(m_hWnd, nMsgId, wParam, lParam);
}

bool CWin32Window::OnEraseBkgnd(HDC hDc)
{
	return true;
}

void CWin32Window::OnPaint(HDC hDc)
{
}

// WM_ENTERSIZEMOVE：进入移动、拉伸窗口操作
void CWin32Window::OnEnterSizeMove()
{
}

// WM_EXITSIZEMOVE：退出移动、拉伸窗口操作
void CWin32Window::OnExitSizeMove()
{
}

void CWin32Window::OnSize(UINT nType, int cx, int cy)
{
	//HRGN hRgn = ::CreateRectRgn(0, 0, cx, cy);
	//if (IS_SAFE_HANDLE(hRgn))
	//{
	//	::SetWindowRgn(m_hWnd, hRgn, TRUE);
	//	::DeleteObject(hRgn);
	//}
}

// 显示窗口
void CWin32Window::OnShowWindow(bool bIsShow, int nShowStatus)
{

}

// 取得窗口句柄
HWND CWin32Window::GetSafeHandle()
{
	return m_hWnd;
}

// 得到窗口屏幕坐标
RECT CWin32Window::GetWindowRect()
{
	RECT RetRect;
	INIT_RECT(RetRect);

	if (IS_SAFE_HANDLE(m_hWnd))
		::GetWindowRect(m_hWnd, &RetRect);

	return RetRect;
}

// 得到客户端坐标
RECT CWin32Window::GetClientRect()
{
	RECT RetRect;
	INIT_RECT(RetRect);

	if (IS_SAFE_HANDLE(m_hWnd))
	{
		::GetWindowRect(m_hWnd, &RetRect);
		int nW = RECT_WIDTH(RetRect);
		int nH = RECT_HEIGHT(RetRect);

		INIT_RECT(RetRect);
		RetRect.right = nW;
		RetRect.bottom = nH;
	}

	return RetRect;
}

// 同WindowsAPI的IsWindow
bool CWin32Window::IsWindow()
{
	return (::IsWindow(m_hWnd) == TRUE);
}

// 取得桌面工作区域大小
RECT CWin32Window::GetWorkAreaRect()
{
	RECT WorkArea;
	INIT_RECT(WorkArea);
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);
	return WorkArea;
}

// 将窗口移到屏幕中央
void CWin32Window::CenterWindow()
{
	RECT WndRect = GetClientRect();
	int cx = RECT_WIDTH(WndRect);
	int cy = RECT_HEIGHT(WndRect);

	if (IS_SAFE_HANDLE(m_hWnd) && cx > 0 && cy > 0)
	{
		// 设置默认大小
		RECT WorkArea = GetWorkAreaRect();
		INIT_RECT(m_rctCreate);

		m_rctCreate.left = WorkArea.left + (RECT_WIDTH(WorkArea) - cx) / 2;
		m_rctCreate.right = m_rctCreate.left + cx;
		m_rctCreate.top = WorkArea.top + (RECT_HEIGHT(WorkArea) - cy) / 2;
		m_rctCreate.bottom = m_rctCreate.top + cy;

		this->MoveWindow(m_rctCreate, TRUE);
	}
}

// 取得窗口标题文字
LPCWSTR CWin32Window::GetWindowText()
{
	return m_strWndText;
}

// 设置定时器
void CWin32Window::SetTimer(int nId, UINT uElapse)
{
	if (IS_SAFE_HANDLE(m_hWnd))
		::SetTimer(m_hWnd, nId, uElapse, NULL);
}

// 取消定时器
void CWin32Window::KillTimer(int nId)
{
	if (IS_SAFE_HANDLE(m_hWnd))
		::KillTimer(m_hWnd, nId);
}

// 设置对话框位置
void CWin32Window::MoveWindow(RECT MoveRect, BOOL bRepaint)
{
	m_rctCreate = MoveRect;
	if (IS_SAFE_HANDLE(m_hWnd))
		::MoveWindow(m_hWnd, MoveRect.left, MoveRect.top, RECT_WIDTH(MoveRect), RECT_HEIGHT(MoveRect), bRepaint);
}

// 显示/隐藏窗口，参数为SW_SHOW之类
void CWin32Window::ShowWindow(int nShow)
{
	m_nCreateShow = nShow;
	if (IS_SAFE_HANDLE(m_hWnd))
		::ShowWindow(m_hWnd, nShow);
}

// 设置重绘区域
void CWin32Window::InvalidateRect(RECT *pInvRect)
{
	if (IS_SAFE_HANDLE(m_hWnd))
		::InvalidateRect(m_hWnd, pInvRect, FALSE);
}

// 更新窗口，立即进行绘制
void CWin32Window::UpdateWindow()
{
	if (IS_SAFE_HANDLE(m_hWnd))
		::UpdateWindow(m_hWnd);
}

// 立即重绘窗口
void CWin32Window::RedrawWindow(RECT *pDrawRect)
{
	if (IS_SAFE_HANDLE(m_hWnd))
		::RedrawWindow(m_hWnd, pDrawRect, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// 发送消息:Send方式
LRESULT CWin32Window::SendMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	LRESULT lpRes = -1;
	if (IS_SAFE_HANDLE(m_hWnd))
		lpRes = ::SendMessage(m_hWnd, nMsgId, wParam, lParam);

	return lpRes;
}

// 发送消息:Post方式
bool CWin32Window::PostMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	bool bRet = false;
	if (IS_SAFE_HANDLE(m_hWnd))
		bRet = (::PostMessage(m_hWnd, nMsgId, wParam, lParam) == TRUE);

	return bRet;
}

// 设置窗体的透明特性
void CWin32Window::SetWindowTransparence(bool bIsTrans)
{
	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if (bIsTrans)
	{
		// 透明
		if ((dwExStyle & WS_EX_LAYERED) != WS_EX_LAYERED)
		{
			dwExStyle |= WS_EX_LAYERED;
			::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle);
		}
	}
	else
	{
		// 不透明
		if ((dwExStyle & WS_EX_LAYERED) == WS_EX_LAYERED)
		{
			dwExStyle &= (~WS_EX_LAYERED);
			::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle);
		}
	}
}

void CWin32Window::SetCreateWindowParam(LPARAM lParam)
{
	if (IS_SAFE_HANDLE(m_hWnd))
		::SetPropA(m_hWnd, "uifeaturewndparam", (HANDLE)lParam);
}

LPARAM CWin32Window::GetCreateWindowParam()
{
	if (IS_SAFE_HANDLE(m_hWnd))
		return (LPARAM)::GetPropA(m_hWnd, "uifeaturewndparam");

	return NULL;
}

// 窗口是否被销毁
bool CWin32Window::IsWindowEnd()
{
	return (!IS_SAFE_HANDLE(m_hWnd) || !::IsWindow(m_hWnd));
}

ATOM CWin32Window::RegisterClass()
{
	WNDCLASSEX wcex;
	if (!::GetClassInfoEx(::GetModuleHandle(NULL), m_strClassName, &wcex)) 
	{
		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcex.lpfnWndProc	= Win32WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= ::GetModuleHandle(NULL);
		wcex.hIcon			= NULL;
		wcex.hCursor		= ::LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= m_strClassName;
		wcex.hIconSm		= NULL;

		return ::RegisterClassEx(&wcex);
	}

	return TRUE;
}

bool CWin32Window::InitInstance()
{
	m_hWnd = ::CreateWindowExW(0, m_strClassName, m_strWndText, m_nCreateStyle,
		m_rctCreate.left, m_rctCreate.top, RECT_WIDTH(m_rctCreate), RECT_HEIGHT(m_rctCreate), m_hParentWnd, NULL, ::GetModuleHandle(NULL), this);

	// 设置创建对话框参数
	SetCreateWindowParam(m_lParam);

	// 对话框初始化
	if (IS_SAFE_HANDLE(m_hWnd))
	{
		//this->CenterWindow();
		OnCreate();
		this->ShowWindow(m_nCreateShow);
	}

	return IS_SAFE_HANDLE(m_hWnd);
}

// WM_TIMER：定时器	
void CWin32Window::OnTimer(UINT nTimerId)
{
}

// WM_CREATE：对话框初始化	
void CWin32Window::OnCreate()
{
}

// WM_DESTROY：销毁窗口
void CWin32Window::OnDestroy()
{
	m_lParam = NULL;
	m_hWnd = m_hParentWnd = NULL;
}

void CWin32Window::OnKillFocus()
{
}

void CWin32Window::OnSetFocus()
{
}

void CWin32Window::OnKeyDown(int nVirtKey, int nFlag)
{
}

// 类似MFC的PreTranslateMessage，如果返回的是true，不再继续往下派发消息
bool CWin32Window::PreTranslateMessage(MSG msg)
{
	return false;
}

// 设置窗口句柄
void CWin32Window::SetSafeHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

// 设置窗口标题文字
void CWin32Window::SetWindowText(LPCWSTR pstrWndText)
{
	if (pstrWndText == NULL)
		return;

	if (IS_SAFE_HANDLE(m_hWnd))
	{
		m_strWndText = pstrWndText;
		::SetWindowTextW(m_hWnd, pstrWndText);
	}
}

void CWin32Window::CloseWindow()
{
	if (::IsWindow(m_hWnd))
		this->PostMessage(WM_CLOSE, NULL, NULL);
}

// 在任务栏上隐藏主窗口按钮
void CWin32Window::HideInTaskbar()
{
	HRESULT hr = NULL;
	ITaskbarList* pTaskbarList = NULL;

	hr = ::CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList, (void**)&pTaskbarList);
	if (hr == S_OK && pTaskbarList != NULL)
	{
		pTaskbarList->HrInit();

		::SetWindowTextA(this->GetSafeHandle(), "");

		DWORD dwLong = ::GetWindowLong(this->GetSafeHandle(), GWL_STYLE);
		dwLong &= (~WS_CAPTION);
		::SetWindowLong(this->GetSafeHandle(), GWL_STYLE, dwLong);

		dwLong = ::GetWindowLong(this->GetSafeHandle(), GWL_EXSTYLE);
		dwLong &= (~WS_EX_APPWINDOW);
		dwLong |= WS_EX_TOOLWINDOW;
		::SetWindowLong(this->GetSafeHandle(), GWL_EXSTYLE, dwLong);

		pTaskbarList->DeleteTab(this->GetSafeHandle());
		pTaskbarList->Release();
	}
}

void CWin32Window::StopDrawWin32Window(bool bStop)
{
	m_bStopDrawWindow = bStop;
}

// 设置任务栏图标
void CWin32Window::SetDialogIcon(int nIconId)
{
	if (m_hDlgIcon == NULL)
		m_hDlgIcon = ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(nIconId));

	if (m_hDlgIcon != NULL)
	{
		BOOL bBigIcon = TRUE;
		::SendMessage(m_hWnd, WM_SETICON, bBigIcon, (LPARAM)m_hDlgIcon);
	}
}

VOID CWin32Window::SetClassName(LPCWSTR pszClassName)
{
	if (pszClassName != NULL && wcslen(m_strClassName) > 0)
		m_strClassName = pszClassName;
}
