
#include "StdAfx.h"
#include "ICtrlContextMenuImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\ICommonFun.h"


ICtrlContextMenu::ICtrlContextMenu(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlContextMenuImpl::ICtrlContextMenuImpl(IUiFeatureKernel *pUiKernel) : ICtrlContextMenu(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_CONTEXTMENU);

	m_MenuWndVec.clear();
	m_pContextMenuWnd = NULL;
	m_pPropMenuPanelName = NULL;

	m_pPropItemHeight = NULL;
	m_pPropSeparatorHeight = NULL;

	m_pPropItemLeftSpace = NULL;
	m_pPropItemRightSpace = NULL;
	m_pPropItemTopSpace = NULL;
	m_pPropItemBottomSpace = NULL;
	m_pPropItemSpace = NULL;
	m_pPropPanelSpace = NULL;

	m_pNewNormalFont = NULL;
	m_pNewHoverFont = NULL;
	m_pNewDisableFont = NULL;
	m_wpTrackMenuParam = NULL;
}

ICtrlContextMenuImpl::~ICtrlContextMenuImpl()
{
	SAFE_DELETE(m_pContextMenuWnd);
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlContextMenuImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	m_pPropMenuPanelName = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "WindowObjectName", "要显示的菜单在Builder中的Window的ObjectName");

	m_pPropItemHeight = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "ItemHeight", "菜单项的高度");
	m_pPropSeparatorHeight = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "SeparatorHeight", "菜单分割线的高度");
	m_pPropSkinPassword = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "SkinPassword", "菜单所处皮肤包的密码");

	m_pPropItemLeftSpace = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "LeftSpace", "菜单项距离菜单面板左侧边距");
	m_pPropItemRightSpace = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "RightSpace", "菜单项距离菜单面板右侧边距");
	m_pPropItemTopSpace = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "TopSpace", "菜单项距离菜单面板顶部边距");
	m_pPropItemBottomSpace = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "BottomSpace", "菜单项距离菜单面板底部边距");
	m_pPropItemSpace = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "ItemSpace", "菜单项之间的距离");
	m_pPropPanelSpace = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "PanelSpace", "菜单面板之间的左右的间隔");

	if (bIsNewCtrl)
	{
		// 无绘制控件
		this->PP_SetNoDrawControl(true);
	}

	return true;
}

// 菜单项的高度
int ICtrlContextMenuImpl::GetItemHeight()
{
	if (m_pPropItemHeight == NULL)
		return 0;

	return m_pPropItemHeight->GetValue();
}

// 菜单分割线的高度
int ICtrlContextMenuImpl::GetSeparatorHeight()
{
	if (m_pPropSeparatorHeight == NULL)
		return 0;

	return m_pPropSeparatorHeight->GetValue();
}

// 菜单项距离菜单面板左侧边距
int ICtrlContextMenuImpl::GetLeftSpace()
{
	if (m_pPropItemLeftSpace == NULL)
		return 0;

	return m_pPropItemLeftSpace->GetValue();
}

// 菜单项距离菜单面板右侧边距
int ICtrlContextMenuImpl::GetRightSpace()
{
	if (m_pPropItemRightSpace == NULL)
		return 0;

	return m_pPropItemRightSpace->GetValue();
}

// 菜单项距离菜单面板顶部边距
int ICtrlContextMenuImpl::GetTopSpace()
{
	if (m_pPropItemTopSpace == NULL)
		return 0;

	return m_pPropItemTopSpace->GetValue();
}

// 菜单项距离菜单面板底部边距
int ICtrlContextMenuImpl::GetBottomSpace()
{
	if (m_pPropItemBottomSpace == NULL)
		return 0;

	return m_pPropItemBottomSpace->GetValue();
}

// 菜单项之间的距离
int ICtrlContextMenuImpl::GetItemSpace()
{
	if (m_pPropItemSpace == NULL)
		return 0;

	return m_pPropItemSpace->GetValue();
}

// 菜单面板之间的左右的间隔
int ICtrlContextMenuImpl::GetPanelSpace()
{
	if (m_pPropPanelSpace == NULL)
		return 0;

	return m_pPropPanelSpace->GetValue();
}


// 初始化控件
void ICtrlContextMenuImpl::OnCreate()
{
}

// 控件初始化完毕
void ICtrlContextMenuImpl::OnFinalCreate()
{
}

// 销毁控件
void ICtrlContextMenuImpl::OnDestroy()
{
}

// 绘制控件
void ICtrlContextMenuImpl::OnPaint(CDrawingBoard &DrawBoard)
{
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlContextMenuImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 取得菜单对话框列表
MENN_WND_VEC* ICtrlContextMenuImpl::GetMenuWindowList()
{
	return &m_MenuWndVec;
}

// 取得菜单面板的参数
WPARAM ICtrlContextMenuImpl::GetPopupMenuParam()
{
	return m_wpTrackMenuParam;
}

// 菜单项消息派发
void ICtrlContextMenuImpl::OnMenuItemMessage(IControlBase* pMenuItem, int nMsgId, int nMenuId)
{
	if (nMsgId == CM_MENU_ITEM_UP && m_pWindowBase != NULL)
	{
		MENU_PARAM_DATA menuParam;
		menuParam.pContextMenu = dynamic_cast<ICtrlContextMenu*>(this);
		menuParam.pMenuItem = dynamic_cast<ICtrlContextMenuItem*>(pMenuItem);

		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), nMsgId, (WPARAM)nMenuId, (LPARAM)&menuParam);
		this->PostMessage(WM_MENU_DESTROY, (WPARAM)this, NULL);
	}
}

bool ICtrlContextMenuImpl::CreatePopupMenu()
{
	if (m_pWindowBase == NULL || m_pWindowBase->GetUiKernel() == NULL || m_pPropMenuPanelName == NULL || m_pPropMenuPanelName->GetString() == NULL || wcslen(m_pPropMenuPanelName->GetString()) <= 0 || m_pPropSkinPassword == NULL)
		return false;

	if (m_pContextMenuWnd == NULL)
		m_pContextMenuWnd = new CContextMenuWnd;

	if (m_pContextMenuWnd == NULL)
		return false;

	if (IS_SAFE_HANDLE(m_pContextMenuWnd->GetSafeHandle()) || m_MenuWndVec.size() > 0)
		return false;

	m_pContextMenuWnd->SetMenuLevel(1);

	CStringA  strPassword = m_pPropSkinPassword->GetString();
	
	bool bRet = m_pContextMenuWnd->CreateContextMenuWnd(dynamic_cast<ICtrlContextMenu*>(this), m_pWindowBase->GetSkinFileItem()->strSkinFilePath, m_pPropMenuPanelName->GetString(), m_pWindowBase->GetUiKernel(), strPassword.GetBuffer(), m_pWindowBase->GetSafeHandle());
	strPassword.ReleaseBuffer();

	MENU_WND_ITEM MenuItem;
	MenuItem.pMenuItem = NULL;
	MenuItem.pMenuWnd = m_pContextMenuWnd;
	m_MenuWndVec.push_back(MenuItem);
	DEBUG_INFO("m_MenuWndVec.push_back");

	if (bRet)
	{
		this->RegisterControlMessage(WM_MENU_DESTROY);
	}
	else
	{
		DestroyMenu();
	}

	return bRet;
}

bool ICtrlContextMenuImpl::TrackPopupMenu(POINT pt, WPARAM wMenuParam)
{
	m_wpTrackMenuParam = wMenuParam;
	if (m_pContextMenuWnd == NULL)
		return false;

	if (!m_pContextMenuWnd->PostMessage(WM_MENU_TRACK, MAKEWPARAM(pt.x,pt.y), NULL))
	{
		// 此处有可能发送消息失败，原因是窗口没有完全建立完整
		DestroyMenu();
		return false;
	}

	return true;
}

bool ICtrlContextMenuImpl::DestroyMenu()
{
	return OnDestroyMenuMessage();
//	return this->PostMessage(WM_MENU_DESTROY, (WPARAM)this, NULL);
}

bool ICtrlContextMenuImpl::OnDestroyMenuMessage()
{
	if (m_pContextMenuWnd == NULL)
		return false;

	for (int i = (int)m_MenuWndVec.size() - 1; i >= 0; i--)
	{
		MENU_WND_ITEM &WndItem = m_MenuWndVec[i];
		if (WndItem.pMenuWnd == NULL)
			continue;

		WndItem.pMenuWnd->CloseWindow();
		//WndItem.pMenuWnd->SendMessage(WM_CLOSE, NULL, NULL);
	}
	m_MenuWndVec.clear();
	DEBUG_INFO("OnDestroyMenuMessage m_MenuWndVec.clear();");

	return true;
}

// 控件需要处理的一些自定义的消息，消息基本上会由对话框或者其他控件传入
int ICtrlContextMenuImpl::OnCtrlNotify(int nMsgId, WPARAM wParam, LPARAM lParam)
{
	if (WM_MENU_DESTROY == nMsgId && wParam == (WPARAM)this)
		OnDestroyMenuMessage();

	return S_OK;
}

// 弹出菜单项的子菜单
void ICtrlContextMenuImpl::PopupSubMenu(ICtrlContextMenuItem* pMenuItem, CContextMenuWnd* pMenuWnd)
{
	if (pMenuItem == NULL || pMenuWnd == NULL)
		return;

	for (int i = 0 ; i < (int)m_MenuWndVec.size(); i++)
	{
		MENU_WND_ITEM &WndItem = m_MenuWndVec[i];
		if (WndItem.pMenuItem == pMenuItem || WndItem.pMenuWnd == pMenuWnd)
			return;
	}

	MENU_WND_ITEM WndItem;
	WndItem.pMenuItem = pMenuItem;
	WndItem.pMenuWnd = pMenuWnd;

	m_MenuWndVec.push_back(WndItem);
	DEBUG_INFO("m_MenuWndVec.push_back(WndItem);");
}

// 销毁菜单项的子菜单
void ICtrlContextMenuImpl::DestroySubMenu(ICtrlContextMenuItem* pMenuItem)
{
	if (pMenuItem == NULL)
		return;

	for (MENN_WND_VEC::iterator pWndItem = m_MenuWndVec.begin(); pWndItem != m_MenuWndVec.end(); pWndItem++)
	{
		MENU_WND_ITEM &WndItem = *pWndItem;
		if (WndItem.pMenuItem == pMenuItem)
		{
			WndItem.pMenuWnd->CloseWindow();
			break;
		}
	}
}

// 设置菜单的新字体
void ICtrlContextMenuImpl::SetMenuFont(IPropertyFontBase* pNormalFont, IPropertyFontBase* pHoverFont, IPropertyFontBase* pDisableFont)
{
	m_pNewNormalFont = pNormalFont;
	m_pNewHoverFont = pHoverFont;
	m_pNewDisableFont = pDisableFont;
}

// 取得菜单的新字体
void ICtrlContextMenuImpl::GetMenuFont(IPropertyFontBase** ppNormalFont, IPropertyFontBase** ppHoverFont, IPropertyFontBase** ppDisableFont)
{
	if (ppNormalFont != NULL)
		*ppNormalFont = m_pNewNormalFont;

	if (ppHoverFont != NULL)
		*ppHoverFont = m_pNewHoverFont;

	if (ppDisableFont != NULL)
		*ppDisableFont = m_pNewDisableFont;
}

// 取得指定子菜单的item
ICtrlContextMenuItem* ICtrlContextMenuImpl::GetMenuItem(WCHAR * pszMenuItemName)
{
	if (pszMenuItemName == NULL || wcslen(pszMenuItemName) <= 0)
		return NULL;

	for (MENN_WND_VEC::iterator pWndItem = m_MenuWndVec.begin(); pWndItem != m_MenuWndVec.end(); pWndItem++)
	{
		MENU_WND_ITEM &WndItem = *pWndItem;
		if (WndItem.pMenuWnd == NULL)
			continue;

		ICtrlContextMenuItem* pMenuItem = WndItem.pMenuWnd->GetMenuItem(pszMenuItemName);
		if (pMenuItem != NULL)
			return pMenuItem;
	}

	return NULL;
}
