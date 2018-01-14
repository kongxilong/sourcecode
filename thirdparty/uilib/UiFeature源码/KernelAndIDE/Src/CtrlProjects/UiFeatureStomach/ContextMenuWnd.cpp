
#include "StdAfx.h"
#include <windowsx.h>
#include "ContextMenuWnd.h"
#include "..\..\Inc\Control\ICtrlContextMenu.h"
#include "..\..\Inc\Control\ICtrlContextMenuItem.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\ICommonFun.h"

CContextMenuWnd::CContextMenuWnd()
{
	m_nMenuLevel = 0;
	m_pContextMenuCtrl = NULL;
	m_bHideDonotDestroy = FALSE;
	m_MenuItemList.clear();
}

CContextMenuWnd::~CContextMenuWnd()
{
}

// 设置菜单级别
void CContextMenuWnd::SetMenuLevel(int nLevel)
{
	m_nMenuLevel = nLevel;
}

// 取得菜单级别
int CContextMenuWnd::GetMenuLevel()
{
	return m_nMenuLevel;
}

bool CContextMenuWnd::CreateContextMenuWnd(ICtrlContextMenu* pContextMenuCtrl, const WCHAR* pszSkinPath, const WCHAR* pszWindowName, IUiFeatureKernel* pUiKernel, char * pszPassword, HWND hWnd)
{
	if (IS_SAFE_HANDLE(m_hWnd) || pContextMenuCtrl == NULL || pszWindowName == NULL || wcslen(pszWindowName) <= 0 || pUiKernel == NULL)
		return false;

	m_pContextMenuCtrl = pContextMenuCtrl;
	RECT WndRct = {0, 0, 0, 0};

	bool bNewThread = (m_nMenuLevel == 1);

	return CreateFeatureWindowByKernel(pszPassword, pUiKernel, pszSkinPath, pszWindowName, hWnd, WndRct, SW_HIDE, WS_POPUP, (LPARAM)this);
}

// 计算/取得菜单面板的大小
SIZE CContextMenuWnd::GetMenuWindowSize()
{
	SIZE menuSize;
	menuSize.cx = menuSize.cy = 0;
	if (m_pWindowBase == NULL)
		return menuSize;

	menuSize = m_pWindowBase->PP_GetWindowPropSize();
	return menuSize;
}

bool CContextMenuWnd::TrackPopupMenu(POINT pt)
{
	SIZE menuSize = GetMenuWindowSize();
	if (IS_INVALID_HANDLE(m_hWnd) || m_pWindowBase == NULL || menuSize.cx <= 0 || menuSize.cy <= 0)
		return false;

	RECT ScreenRect;
	INIT_RECT(ScreenRect);

	// 得到屏幕大小
	ScreenRect.right = GetSystemMetrics(SM_CXSCREEN);
	ScreenRect.bottom = GetSystemMetrics(SM_CYSCREEN);

	// 取得多屏整个宽度和高度
	int nCtns = GetSystemMetrics(SM_CMONITORS);
	if (nCtns > 1)
	{
		// 虚拟坐标的左上角起点
		ScreenRect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
		ScreenRect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);

		// 所有屏幕加在一起的大小
		ScreenRect.right = ScreenRect.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
		ScreenRect.bottom = ScreenRect.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
	}

	// 菜单不能超出屏幕右侧
	if ((pt.x + menuSize.cx) > ScreenRect.right)
		pt.x -= menuSize.cx;

	// 菜单不能超出屏幕下方
	if ((pt.y + menuSize.cy) > ScreenRect.bottom)
		pt.y -= menuSize.cy;

	// 菜单不能超出屏幕左侧
	if (pt.x < ScreenRect.left)
		pt.x = ScreenRect.left;

	// 菜单不能超出屏幕上方
	if (pt.y < ScreenRect.top)
		pt.y = ScreenRect.top;

	RECT wndRct = {pt.x, pt.y, pt.x + menuSize.cx, pt.y + menuSize.cy};
	this->MoveWindow(wndRct);
	this->ShowWindow(SW_SHOW);
	return true;
}

// 本窗口的消息处理函数
LRESULT CContextMenuWnd::WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	// 弹出窗口
	if (WM_MENU_TRACK == nMsgId)
	{
		POINT pt;
		pt.x = GET_X_LPARAM(wParam);
		pt.y = GET_Y_LPARAM(wParam);
		this->TrackPopupMenu(pt);
		return 0;
	}

	return CUiFeatureWindow::WndProc(nMsgId, wParam, lParam);
}

void CContextMenuWnd::OnKeyDown(int nVirtKey, int nFlag)
{
	if (nVirtKey == VK_ESCAPE && m_pContextMenuCtrl != NULL)
	{
		m_pContextMenuCtrl->DestroyMenu();
	}
}

// 销毁菜单
void CContextMenuWnd::DestroyMenu()
{
	if (IS_INVALID_HANDLE(m_hWnd))
		return;

	this->CloseWindow();

	m_nMenuLevel = 0;
	m_pContextMenuCtrl = NULL;
	m_MenuItemList.clear();
}

void CContextMenuWnd::OnCreate()
{
	if (m_pWindowBase == NULL || m_pContextMenuCtrl == NULL)
		return;

	CHILD_CTRLS_VEC* pChildCtrlVEC = m_pWindowBase->GetChildControlsVec();
	if (pChildCtrlVEC == NULL)
		return;

	m_MenuItemList.clear();
	GetMenuItemVec(pChildCtrlVEC);
	SetMenuItemFont();
}

// 设置字体
void CContextMenuWnd::SetMenuItemFont()
{
	if (m_pContextMenuCtrl == NULL)
		return;

	IPropertyFontBase* pNormalFont = NULL;
	IPropertyFontBase* pHoverFont = NULL;
	IPropertyFontBase* pDisableFont = NULL;
	m_pContextMenuCtrl->GetMenuFont(&pNormalFont, &pHoverFont, &pDisableFont);

	for (MENU_ITEM_CTRL_VEC::iterator pCtrlItem = m_MenuItemList.begin(); pCtrlItem != m_MenuItemList.end(); pCtrlItem++)
	{
		ICtrlContextMenuItem* pMenuItem = *pCtrlItem;
		if (pMenuItem == NULL)
			continue;

		pMenuItem->SetMenuFont(pNormalFont, pHoverFont, pDisableFont);
	}
}

// 取得指定子菜单的item
ICtrlContextMenuItem* CContextMenuWnd::GetMenuItem(WCHAR * pszMenuItemName)
{
	if (pszMenuItemName == NULL || wcslen(pszMenuItemName) <= 0)
		return NULL;

	for (MENU_ITEM_CTRL_VEC::iterator pCtrlItem = m_MenuItemList.begin(); pCtrlItem != m_MenuItemList.end(); pCtrlItem++)
	{
		ICtrlContextMenuItem* pMenuItem = *pCtrlItem;
		if (pMenuItem == NULL)
			continue;

		if (lstrcmpi(pMenuItem->GetObjectName(), pszMenuItemName) == 0)
			return pMenuItem;
	}

	return NULL;
}

void CContextMenuWnd::GetMenuItemVec(CHILD_CTRLS_VEC* pCtrlVEC)
{
	if (pCtrlVEC == NULL || m_pContextMenuCtrl == NULL)
		return;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pCtrlVEC->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pCtrlVEC->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		if (lstrcmpi(pCtrl->GetObjectType(), CTRL_NAME_CONTEXTMENUITEM) == 0)
		{
			ICtrlContextMenuItem *pMenuItem = dynamic_cast<ICtrlContextMenuItem*>(pCtrl);
			if (pMenuItem == NULL)
				continue;

			pMenuItem->SetContextMenu(m_pContextMenuCtrl);

			// 菜单Item
			m_MenuItemList.push_back(pMenuItem);
			continue;
		}

		CHILD_CTRLS_VEC* pChildCtrlVEC = pCtrl->GetChildControlsVec();
		if (pChildCtrlVEC == NULL)
			continue;

		GetMenuItemVec(pChildCtrlVEC);
	}
}

// 取得菜单项列表
MENU_ITEM_CTRL_VEC* CContextMenuWnd::GetMenuItemList()
{
	return &m_MenuItemList;
}

// 控件发送给窗口的消息接口
LRESULT CContextMenuWnd::OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam)
{
	if ((nMsgId == CM_MENU_ITEM_DOWN || nMsgId == CM_MENU_ITEM_UP) && m_pContextMenuCtrl != NULL)
	{
		m_bHideDonotDestroy = TRUE;
		ShowWindow(SW_HIDE);
		m_bHideDonotDestroy = FALSE;

		m_pContextMenuCtrl->OnMenuItemMessage(pCtrl, nMsgId, (int)wParam);
		
		return 0;
	}

	return CUiFeatureWindow::OnCtrlMessage(pCtrl, nMsgId, wParam, lParam);
}

void CContextMenuWnd::OnKillFocus()
{
	if (m_pContextMenuCtrl == NULL || m_bHideDonotDestroy)
		return;

	MENN_WND_VEC* pWndVec = m_pContextMenuCtrl->GetMenuWindowList();
	if (pWndVec == NULL)
		return;

	// 失去的焦点如果还是在菜单上，不销毁窗口
	HWND hActWnd = ::GetActiveWindow();

	for (MENN_WND_VEC::iterator pWndItem = pWndVec->begin(); pWndItem != pWndVec->end(); pWndItem++)
	{
		MENU_WND_ITEM& wndItem = *pWndItem;
		if (wndItem.pMenuWnd == NULL)
			continue;

		// 失去的焦点在菜单上，不销毁窗口
		if (hActWnd == wndItem.pMenuWnd->GetSafeHandle())
			return;
	}

	m_pContextMenuCtrl->DestroyMenu();

	m_nMenuLevel = 0;
	m_pContextMenuCtrl = NULL;
	m_MenuItemList.clear();
}
