
#include "StdAfx.h"
#include "ICtrlWindowTitleBarImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\ICommonFun.h"

ICtrlWindowTitleBar::ICtrlWindowTitleBar(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlWindowTitleBarImpl::ICtrlWindowTitleBarImpl(IUiFeatureKernel *pUiKernel) : ICtrlWindowTitleBar(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_WINDOW_TITLE_BAR);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;

	m_pMinimizeBtn = NULL;
	m_pMaximizeBtn = NULL;
	m_pRestoreBtn = NULL;
	m_pClostBtn = NULL;
	m_pWndBasePanelCtrl = NULL;

	m_pPropNoMinimizeBtn = NULL;
	m_pPropNoMaximizeBtn = NULL;
	m_pPropWndBasePanelName = NULL;
	m_pPropSetDftRgn = NULL;

	m_bOldFullScreenStyle = false;
}

ICtrlWindowTitleBarImpl::~ICtrlWindowTitleBarImpl()
{
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlWindowTitleBarImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlWindowTitleBarImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	m_pPropNoMinimizeBtn = (IPropertyBool*)CreatePropetry(NULL, OTID_BOOL, "NoMinimizeButton", "没有最小化按钮");
	m_pPropNoMaximizeBtn = (IPropertyBool*)CreatePropetry(NULL, OTID_BOOL, "NoMaximizeButton", "没有最大化和还原按钮");
	m_pPropWndBasePanelName = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "WndBasePanelName", "当前对话框最底层的控件，一个窗口如果支持最大化，必须要有一个底层的控件（一般为ICtrlPanel），这个控件包含当前窗口的所有控件。此处输入这个控件的名称。");
	m_pPropSetDftRgn = (IPropertyBool*)CreatePropetry(NULL, OTID_BOOL, "UseDefaultRgn", "使用控件内置的窗口的RGN区域，默认为何窗口大小一样的矩形RGN");

	if (bIsNewCtrl)
	{
		m_pPropBase_NoDrawCtrl->SetValue(true);
		// 撑满上方
		this->SetControlLayoutType(CL_L_TOP);
	}

	return true;
}

bool ICtrlWindowTitleBarImpl::UseDefaultRgn()
{
	if (m_pPropSetDftRgn == NULL)
		return false;

	return m_pPropSetDftRgn->GetValue();
}

// 初始化控件
void ICtrlWindowTitleBarImpl::OnCreate()
{
}

// 控件初始化完毕
void ICtrlWindowTitleBarImpl::OnFinalCreate()
{
	if (m_pWindowBase == NULL || m_pWindowBase->IsDesignMode() || !::IsWindow(GetOwnerWindowHwnd()) || m_pPropWndBasePanelName == NULL)
		return;

	this->RegisterControlMessage(UI_FEATURE_KERNEL_MSG);
	this->RegisterControlMessage(WM_WINDOWPOSCHANGED);
	this->RegisterControlMessageSync(WM_SIZE);

	DWORD dwStyle = ::GetWindowLong(GetOwnerWindowHwnd(), GWL_STYLE);

	dwStyle |= WS_OVERLAPPEDWINDOW;

	if (!HaveMinimizeButton())
		dwStyle &= (~WS_MINIMIZEBOX);

	if (!HaveMaximizeButton())
		dwStyle &= (~WS_MAXIMIZEBOX);

	// WS_SIZEBOX 会让在win7下有贴边放大的效果
	if (!m_pWindowBase->PP_IsCanStretchWindow())
		dwStyle &= (~WS_SIZEBOX);

	::SetWindowLong(GetOwnerWindowHwnd(), GWL_STYLE, dwStyle);

	m_pWndBasePanelCtrl = m_pWindowBase->GetControl(m_pPropWndBasePanelName->GetString());

	// 判断操作系统，Win7 以下的没有最大化后，直接移动标题栏还原的效果
	if (HaveMaximizeButton())
		m_pWindowBase->SetMaximizeWndDragMove(IsWin7Later());
	else
		m_pWindowBase->SetMaximizeWndDragMove(false);

	this->PostMessage(UI_FEATURE_KERNEL_MSG, WID_SET_WINDOW_RGN, (LPARAM)this);
}

bool ICtrlWindowTitleBarImpl::HaveMinimizeButton()
{
	if (m_pPropNoMinimizeBtn == NULL)
		return false;

	return !m_pPropNoMinimizeBtn->GetValue();
}

bool ICtrlWindowTitleBarImpl::HaveMaximizeButton()
{
	if (m_pPropNoMaximizeBtn == NULL)
		return false;

	return !m_pPropNoMaximizeBtn->GetValue();
}

// 这个接口会在整个对话框初始化完成的时候调用
// 控件通过派生这个接口，可以在这个接口函数里通过其他控件的名称取得当前对话框里的其他控件。
void ICtrlWindowTitleBarImpl::OnWindowFinalCreate()
{
}

// 是否为Win7以后的操作系统
bool ICtrlWindowTitleBarImpl::IsWin7Later()
{
	OSVERSIONINFOEX os;
	::ZeroMemory(&os, sizeof(os));
	os.dwOSVersionInfoSize = sizeof(os);
	::GetVersionEx(reinterpret_cast<LPOSVERSIONINFO>(&os));

	// Win8.............
	if (os.dwMajorVersion > 6)
		return true;

	// Win7.............
	if (os.dwMajorVersion >= 6 && os.dwMinorVersion >= 1)
		return true;

	return false;
}

// 销毁控件
void ICtrlWindowTitleBarImpl::OnDestroy()
{
	this->UnRegisterControlMessage(UI_FEATURE_KERNEL_MSG);
	this->UnRegisterControlMessage(WM_WINDOWPOSCHANGED);
	this->UnRegisterControlMessage(WM_SIZE);
}

// 当一个控件包含了其他子控件的时候，系统会发送创建或加载子控件的消息
void ICtrlWindowTitleBarImpl::OnCreateSubControl(bool bIsCreate)
{
	m_pMinimizeBtn = dynamic_cast<ICtrlShadowButton*>(m_pWindowBase->CreateSubControl(dynamic_cast<IControlMessage*>(this),
		dynamic_cast<IControlBase*>(this), CTRL_NAME_SHADOW_BUTTON, L"TB_MinimizeBtn", bIsCreate, 1));
	m_pMaximizeBtn = dynamic_cast<ICtrlShadowButton*>(m_pWindowBase->CreateSubControl(dynamic_cast<IControlMessage*>(this),
		dynamic_cast<IControlBase*>(this), CTRL_NAME_SHADOW_BUTTON, L"TB_MaximizeBtn", bIsCreate, 2));
	m_pRestoreBtn = dynamic_cast<ICtrlShadowButton*>(m_pWindowBase->CreateSubControl(dynamic_cast<IControlMessage*>(this),
		dynamic_cast<IControlBase*>(this), CTRL_NAME_SHADOW_BUTTON, L"TB_RestoreBtn", bIsCreate, 3));
	m_pClostBtn = dynamic_cast<ICtrlShadowButton*>(m_pWindowBase->CreateSubControl(dynamic_cast<IControlMessage*>(this),
		dynamic_cast<IControlBase*>(this), CTRL_NAME_SHADOW_BUTTON, L"TB_ClostBtn", bIsCreate, 4));

	// 配合那个克隆的例子做了一下小修正
	OnAfterClone(NULL, NULL);
}

// pCloneCtrl 是最终克隆出来的控件，可以在这个消息响应函数中再克隆一些控件私有的属性
void ICtrlWindowTitleBarImpl::OnAfterClone(IControlBase* pCloneCtrl, WPARAM lParam)
{
	if (m_pWindowBase != NULL && !m_pWindowBase->IsDesignMode())
	{
		if (m_pMinimizeBtn != NULL && !HaveMinimizeButton())
			m_pMinimizeBtn->SetVisible(false);

		if (m_pMaximizeBtn != NULL && !HaveMaximizeButton())
			m_pMaximizeBtn->SetVisible(false);

		if (m_pRestoreBtn != NULL && !HaveMaximizeButton())
			m_pRestoreBtn->SetVisible(false);
	}
}

// 绘制控件
void ICtrlWindowTitleBarImpl::OnPaint(CDrawingBoard &DrawBoard)
{
}

ICtrlShadowButton* ICtrlWindowTitleBarImpl::GetMinimizeButton()
{
	return m_pMinimizeBtn;
}

ICtrlShadowButton* ICtrlWindowTitleBarImpl::GetMaximizeButton()
{
	return m_pMaximizeBtn;
}

ICtrlShadowButton* ICtrlWindowTitleBarImpl::GetRestoreButton()
{
	return m_pRestoreBtn;
}

ICtrlShadowButton* ICtrlWindowTitleBarImpl::GetClostButton()
{
	return m_pClostBtn;
}

// 鼠标是否落在了控件指定的RGN区域，默认返回true，整个控件就是RGN区域
bool ICtrlWindowTitleBarImpl::OnCheckMouseInRgn(POINT pt, INT nMouseMsgId)
{
	if (nMouseMsgId == WM_LBUTTONDOWN)
	{
		POINT wndPt = pt;
		RECT wndRct = this->GetWindowRect();
		wndPt.x += wndRct.left;
		wndPt.y += wndRct.top;

		if (PointInSubCtrl(wndPt, dynamic_cast<IControlBase*>(m_pMinimizeBtn)))
			return true;

		if (PointInSubCtrl(wndPt, dynamic_cast<IControlBase*>(m_pMaximizeBtn)))
			return true;

		if (PointInSubCtrl(wndPt, dynamic_cast<IControlBase*>(m_pRestoreBtn)))
			return true;

		if (PointInSubCtrl(wndPt, dynamic_cast<IControlBase*>(m_pClostBtn)))
			return true;

		return false;
	}

	return true;
}

// 鼠标是否落在了子控件范围内
BOOL ICtrlWindowTitleBarImpl::PointInSubCtrl(POINT pt, IControlBase* pCtrlBase)
{
	if (pCtrlBase == NULL || !pCtrlBase->IsVisible())
		return FALSE;

	RECT ctrlRct = pCtrlBase->GetWindowRect();
	return ::PtInRect(&ctrlRct, pt);
}

// 鼠标左键双击
void ICtrlWindowTitleBarImpl::OnLButtonDbClick(POINT pt)
{
	if (m_pWindowBase == NULL || !HaveMaximizeButton())
		return;
	
	m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_TITLEBAR_DBCLICK, NULL, NULL);

	if (m_pWindowBase->IsFullScreen())
		OnRestoreBtnClick(NULL, NULL);
	else
		OnMaximizeBtnClick(NULL, NULL);
}

LRESULT ICtrlWindowTitleBarImpl::OnMinimizeBtnClick(WPARAM wParam, LPARAM lParam)
{
	if (m_pWindowBase == NULL || !HaveMinimizeButton())
		return S_FALSE;

	this->PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, NULL);
	m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_TITLEBAR_MINIMIZEBTN_CLICK, NULL, NULL);
	return S_OK;
}

LRESULT ICtrlWindowTitleBarImpl::OnMaximizeBtnClick(WPARAM wParam, LPARAM lParam)
{
	if (m_pWindowBase == NULL || !HaveMaximizeButton())
		return S_FALSE;

	this->PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, NULL);
	m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_TITLEBAR_MAXIMIZEBTN_CLICK, NULL, NULL);
	return S_OK;
}

LRESULT ICtrlWindowTitleBarImpl::OnRestoreBtnClick(WPARAM wParam, LPARAM lParam)
{
	if (m_pWindowBase == NULL || !HaveMaximizeButton())
		return S_FALSE;

	this->PostMessage(WM_SYSCOMMAND, SC_RESTORE, NULL);
	m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_TITLEBAR_RESTOREBTN_CLICK, NULL, NULL);
	return S_OK;
}

LRESULT ICtrlWindowTitleBarImpl::OnClostBtnClick(WPARAM wParam, LPARAM lParam)
{
	if (m_pWindowBase == NULL)
		return S_FALSE;

	this->PostMessage(WM_SYSCOMMAND, SC_CLOSE, NULL);
	m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_TITLEBAR_CLOSEBTN_CLICK, NULL, NULL);
	return S_OK;
}

int ICtrlWindowTitleBarImpl::OnWindowSize(WPARAM wParam, LPARAM lParam)
{
	SetDefaultWindowRgn();
	return S_OK;
}

int ICtrlWindowTitleBarImpl::OnSetWindowRgn(WPARAM wParam, LPARAM lParam)
{
	if (m_pWindowBase == NULL || wParam != WID_SET_WINDOW_RGN || lParam != (LPARAM)this)
		return S_FALSE;

	// 初始化一下最大化状态
	m_bOldFullScreenStyle = !m_pWindowBase->IsFullScreen();
	OnWindowPosChanged(NULL, NULL);

	SetDefaultWindowRgn();
	return S_OK;
}

void ICtrlWindowTitleBarImpl::SetDefaultWindowRgn()
{
	if (m_pWindowBase == NULL)
		return;

	// 告诉窗口，该设置RGN了
	m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_TITLEBAR_SET_WND_RGN, NULL, NULL);

	// 直接使用默认的RGN
	if (!UseDefaultRgn())
		return;

	RECT WndRect = { 0, 0, 0, 0 };
	WndRect = m_pWindowBase->GetClientRect();

	if (m_pWindowBase->IsFullScreen())
	{
		// 最大化的状态下要裁掉边框
		int nCxOffset = 0, nCyOffset = 0;
		GetMaxWndRgnOffset(nCxOffset, nCyOffset);
		WndRect.left += nCxOffset;
		WndRect.right -= nCxOffset;
		WndRect.top += nCyOffset;
		WndRect.bottom -= nCyOffset;
	}

	CRgn Rgn;
	Rgn.CreateRectRgnIndirect(&WndRect);
	::SetWindowRgn(GetOwnerWindowHwnd(), (HRGN)Rgn, TRUE);
}

int ICtrlWindowTitleBarImpl::OnWindowPosChanged(WPARAM wParam, LPARAM lParam)
{
	if (m_pWindowBase == NULL || m_pRestoreBtn == NULL || m_pMaximizeBtn == NULL || !HaveMaximizeButton())
		return S_FALSE;

	bool bFullScreen = m_pWindowBase->IsFullScreen();
	if (bFullScreen == m_bOldFullScreenStyle)
		return S_OK;

	m_bOldFullScreenStyle = bFullScreen;
	m_pRestoreBtn->SetVisible(m_bOldFullScreenStyle);
	m_pMaximizeBtn->SetVisible(!m_bOldFullScreenStyle);

	if (m_pWndBasePanelCtrl != NULL)
	{
		RECT wndRct = m_pWindowBase->GetClientRect();
		if (m_bOldFullScreenStyle)
		{
			// 此控件的布局属性必须为撑满全部
			m_pWndBasePanelCtrl->SetControlLayoutType(CL_L_MIDDLE);

			int nCxOffset = 0, nCyOffset = 0;
			GetMaxWndRgnOffset(nCxOffset, nCyOffset);
			wndRct.left += nCxOffset;
			wndRct.right -= nCxOffset;
			wndRct.top += nCyOffset;
			wndRct.bottom -= nCyOffset;
			// 当前为最大化状态，需要设置窗口中所有控件的偏移
			m_pWndBasePanelCtrl->SetWindowRectLayoutWithChild(wndRct, true);
			m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_TITLEBAR_WILL_MAXIMIZE, NULL, NULL);
		}
		else
		{
			// 此控件的布局属性必须为撑满全部
			m_pWndBasePanelCtrl->SetControlLayoutType(CL_L_ALL);
			m_pWndBasePanelCtrl->SetWindowRectLayoutWithChild(wndRct, true);
			m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_TITLEBAR_WILL_RESTOREBTN, NULL, NULL);
		}
	}

	return S_OK;
}

void ICtrlWindowTitleBarImpl::GetMaxWndRgnOffset(int &nCxOffset, int &nCyOffset)
{
	nCxOffset = nCyOffset = 0;
	DWORD dwStyle = ::GetWindowLong(GetOwnerWindowHwnd(), GWL_STYLE);
	if ((dwStyle & WS_CAPTION) != 0)
	{
		if ((dwStyle & WS_SIZEBOX) != 0)
		{
			// 存在拉伸边框操作的对话框
			nCxOffset = ::GetSystemMetrics(SM_CXSIZEFRAME);
			nCyOffset = ::GetSystemMetrics(SM_CYSIZEFRAME);
		}
		else if ((dwStyle & WS_MAXIMIZEBOX) != 0)
		{
			// 具有标题但无法改变尺寸的窗口（通常是一些对话框）的边框的厚度
			nCxOffset = ::GetSystemMetrics(SM_CXFIXEDFRAME);
			nCyOffset = ::GetSystemMetrics(SM_CYFIXEDFRAME);
		}
	}
}
