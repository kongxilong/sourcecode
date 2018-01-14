
#include "StdAfx.h"
#include "ICtrlIEWebBrowserImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\ICommonFun.h"


ICtrlIEWebBrowser::ICtrlIEWebBrowser(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlIEWebBrowserImpl::ICtrlIEWebBrowserImpl(IUiFeatureKernel *pUiKernel) : ICtrlIEWebBrowser(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_IE_WEB_BROWSER);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;

	m_pPropUrl = NULL;
}

ICtrlIEWebBrowserImpl::~ICtrlIEWebBrowserImpl()
{
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlIEWebBrowserImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlIEWebBrowserImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	m_pPropUrl = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "URL", "网页地址或者本地网页路径");

	if (bIsNewCtrl)
	{
		if (m_pPropBase_NoDrawCtrl != NULL)
			m_pPropBase_NoDrawCtrl->SetValue(true);
	}
	return true;
}

VOID ICtrlIEWebBrowserImpl::OpenUrl(LPCWSTR pszUrl)
{
	if (pszUrl == NULL || wcslen(pszUrl) <= 0)
		return;

	if (m_pPropUrl == NULL)
		m_pPropUrl = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "URL", "网页地址或者本地网页路径", true);

	if (m_pPropUrl == NULL)
		return;

	m_pPropUrl->SetString(pszUrl);
	this->PostMessage(WM_IE_WEB_OPEN_URL, (WPARAM)this, NULL);
}

LPCWSTR ICtrlIEWebBrowserImpl::GetUrl()
{
	if (m_pPropUrl == NULL)
		return NULL;

	return m_pPropUrl->GetString();
}

// 初始化控件
void ICtrlIEWebBrowserImpl::OnCreate()
{
}

// 控件初始化完毕
void ICtrlIEWebBrowserImpl::OnFinalCreate()
{
	this->RegisterControlMessage(WM_IE_WEB_CREATE_UIF);
	this->RegisterControlMessage(WM_IE_WEB_OPEN_URL);
	this->RegisterControlMessage(WM_MOVE);
	this->RegisterControlMessage(WM_SHOWWINDOW);
	this->RegisterControlMessage(WM_SYSCOMMAND);
}

// 销毁控件
void ICtrlIEWebBrowserImpl::OnDestroy()
{
}

// 绘制控件
void ICtrlIEWebBrowserImpl::OnPaint(CDrawingBoard &DrawBoard)
{
}

// 这个接口会在整个对话框初始化完成的时候调用
// 控件通过派生这个接口，可以在这个接口函数里通过其他控件的名称取得当前对话框里的其他控件。
void ICtrlIEWebBrowserImpl::OnWindowFinalCreate()
{
	this->PostMessage(WM_IE_WEB_CREATE_UIF, (WPARAM)this, NULL);
}

// 附属对话框即将关闭
void ICtrlIEWebBrowserImpl::OnWindowClose()
{
	this->UnRegisterControlMessage(WM_IE_WEB_CREATE_UIF);
	this->UnRegisterControlMessage(WM_IE_WEB_OPEN_URL);
	this->UnRegisterControlMessage(WM_MOVE);
	this->UnRegisterControlMessage(WM_SHOWWINDOW);
	this->UnRegisterControlMessage(WM_SYSCOMMAND);
}

int ICtrlIEWebBrowserImpl::OnCreateUiFeatureIeWindow(WPARAM wParam, LPARAM lParam)
{
	if ((WPARAM)this != wParam)
		return S_FALSE;

	if (m_WebWindow.IsWindow() || m_pWindowBase == NULL)
		return S_FALSE;

	m_WebWindow.CreateUiFeatureBrowserWindow(m_pWindowBase->GetSafeHandle(), SW_SHOWNORMAL, GetUrl());
	SetIeWindowPostion();

	return S_OK;
}

int ICtrlIEWebBrowserImpl::OnOpenUrl(WPARAM wParam, LPARAM lParam)
{
	if ((WPARAM)this != wParam)
		return S_FALSE;

	CStringW strUrl = GetUrl();
	if (strUrl.GetLength() <= 0)
		return S_FALSE;

	if (!m_WebWindow.IsWindow() || m_pWindowBase == NULL)
		return S_FALSE;

	m_WebWindow.OpenUrl(strUrl);
	SetIeWindowPostion();
	
	return S_OK;
}

// 移动、设置控件位置
void ICtrlIEWebBrowserImpl::OnSize()
{
	SetIeWindowPostion();
}

void ICtrlIEWebBrowserImpl::SetIeWindowPostion()
{
	if (m_WebWindow.IsWindow() && ::IsWindowVisible(m_WebWindow.GetSafeHandle()))
	{
		RECT CtrlRct = this->GetWindowRect();
		RectClientToScreen(m_pWindowBase->GetSafeHandle(), CtrlRct);
		m_WebWindow.MoveWindow(CtrlRct);
	}
}

// 可见属性
void ICtrlIEWebBrowserImpl::SetVisible(bool bVisible, bool bSetChild)
{
	ICtrlIEWebBrowser::SetVisible(bVisible, bSetChild);

	if (bVisible)
	{
		m_WebWindow.ShowWindow(SW_SHOW);
		SetIeWindowPostion();
	}
	else
	{
		m_WebWindow.ShowWindow(SW_HIDE);
	}
}

int ICtrlIEWebBrowserImpl::OnWindowMove(WPARAM wParam, LPARAM lParam)
{
	SetIeWindowPostion();
	return S_OK;
}

BOOL ICtrlIEWebBrowserImpl::SetIeWindowShow(BOOL* pIsWndVisible)
{
	if (!m_WebWindow.IsWindow())
		return FALSE;

	BOOL bWndVisible = ::IsWindowVisible(m_pWindowBase->GetSafeHandle());
	if (pIsWndVisible != NULL)
		bWndVisible = *pIsWndVisible;

	INT nShow = SW_HIDE;
	if (bWndVisible && this->IsVisible())
		nShow = SW_SHOW;

	m_WebWindow.ShowWindow(nShow);

	return (nShow == SW_SHOW);
}

int ICtrlIEWebBrowserImpl::OnSysCommand(WPARAM wParam, LPARAM lParam)
{
	if (m_WebWindow.IsWindow())
	{
		if (wParam == SC_RESTORE)
		{
			DWORD dwStyle = ::GetWindowLong(m_WebWindow.GetSafeHandle(), GWL_STYLE);
			dwStyle &= (~WS_CHILD);
			dwStyle |= WS_POPUP;
			::SetWindowLong(m_WebWindow.GetSafeHandle(), GWL_STYLE, dwStyle);
		}
		else if (wParam == SC_MINIMIZE)
		{
			DWORD dwStyle = ::GetWindowLong(m_WebWindow.GetSafeHandle(), GWL_STYLE);
			dwStyle &= (~WS_POPUP);
			dwStyle |= WS_CHILD;
			::SetWindowLong(m_WebWindow.GetSafeHandle(), GWL_STYLE, dwStyle);
		}
	}

	return S_OK;
}
