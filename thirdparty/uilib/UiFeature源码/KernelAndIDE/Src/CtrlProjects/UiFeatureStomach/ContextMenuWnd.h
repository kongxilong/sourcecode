
#pragma once
#include "..\..\Inc\CUiFeatureWindow.h"
#include "..\..\Inc\Control\ICtrlContextMenuItem.h"

// 弹出菜单
#define WM_MENU_TRACK					(WM_APP + 0x0300 + 0x0181)
// 销毁菜单
#define WM_MENU_DESTROY					(WM_APP + 0x0300 + 0x0182)

typedef vector<ICtrlContextMenuItem*>			MENU_ITEM_CTRL_VEC;


class ICtrlContextMenu;
class CContextMenuWnd : public CUiFeatureWindow
{
public:
	CContextMenuWnd();
	virtual ~CContextMenuWnd();

	// 创建菜单面板
	bool CreateContextMenuWnd(ICtrlContextMenu* pContextMenuCtrl, const WCHAR* pszSkinPath, const WCHAR* pszWindowName, IUiFeatureKernel* pUiKernel, char * pszPassword, HWND hWnd);
	// 销毁菜单
	void DestroyMenu();

	// 设置菜单级别
	void SetMenuLevel(int nLevel);
	// 取得菜单级别
	int GetMenuLevel();
	// 取得菜单项列表
	MENU_ITEM_CTRL_VEC* GetMenuItemList();

	// 显示菜单面板，这个函数不会导致wait
	bool TrackPopupMenu(POINT pt);

	// 取得指定子菜单的item
	ICtrlContextMenuItem* GetMenuItem(WCHAR * pszMenuItemName);

protected:
	// 控件发送给窗口的消息接口
	virtual LRESULT OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam);
	// 本窗口的消息处理函数
	virtual LRESULT WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnKeyDown(int nVirtKey, int nFlag);
	virtual void OnKillFocus();

private:
	// 计算/取得菜单面板的大小
	SIZE GetMenuWindowSize();
	void GetMenuItemVec(CHILD_CTRLS_VEC* pCtrlVEC);
	// 设置字体
	void SetMenuItemFont();

private:
	int m_nMenuLevel;
	ICtrlContextMenu* m_pContextMenuCtrl;
	MENU_ITEM_CTRL_VEC m_MenuItemList;
	BOOL				m_bHideDonotDestroy;
};
