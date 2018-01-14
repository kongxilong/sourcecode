
#pragma once
#include "..\ICtrlInterface.h"
#include ".\..\IPropertyFontBase.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_CONTEXTMENU						(L"ContextMenu")

class ICtrlContextMenu;
class ICtrlContextMenuItemImpl;
class ICtrlContextMenuItem;
class CContextMenuWnd;

// 一个菜单窗口的信息
struct MENU_WND_ITEM
{
	ICtrlContextMenuItem* pMenuItem;
	// 内部菜单窗口对话框指针
	CContextMenuWnd* pMenuWnd;
};
typedef vector<MENU_WND_ITEM>	MENN_WND_VEC;

// 一个菜单项点击后发出的消息参数
struct MENU_PARAM_DATA
{
	ICtrlContextMenu* pContextMenu;
	ICtrlContextMenuItem* pMenuItem;
};

class _declspec(novtable) ICtrlContextMenu : public ICtrlInterface
{
	friend class CContextMenuWnd;
	friend class ICtrlContextMenuItemImpl;

public:
	ICtrlContextMenu(IUiFeatureKernel *pUiKernel);
	// 创建菜单面板
	virtual bool CreatePopupMenu() = 0;
	// 显示菜单面板，这个函数不会导致wait
	virtual bool TrackPopupMenu(POINT pt, WPARAM wMenuParam = NULL) = 0;
	// 销毁菜单
	virtual bool DestroyMenu() = 0;

	// 菜单项的高度
	virtual int GetItemHeight() = 0;
	// 菜单分割线的高度
	virtual int GetSeparatorHeight() = 0;

	// 菜单项距离菜单面板左侧边距
	virtual int GetLeftSpace() = 0;
	// 菜单项距离菜单面板右侧边距
	virtual int GetRightSpace() = 0;
	// 菜单项距离菜单面板顶部边距
	virtual int GetTopSpace() = 0;
	// 菜单项距离菜单面板底部边距
	virtual int GetBottomSpace() = 0;
	// 菜单项之间的距离
	virtual int GetItemSpace() = 0;
	// 菜单面板之间的左右的间隔
	virtual int GetPanelSpace() = 0;

	// 设置菜单的新字体
	virtual void SetMenuFont(IPropertyFontBase* pNormalFont, IPropertyFontBase* pHoverFont, IPropertyFontBase* pDisableFont) = 0;

	// 取得菜单面板的参数
	virtual WPARAM GetPopupMenuParam() = 0;

	// 取得指定子菜单的item
	virtual ICtrlContextMenuItem* GetMenuItem(WCHAR * pszMenuItemName) = 0;

protected:
	// 弹出菜单项的子菜单
	virtual void PopupSubMenu(ICtrlContextMenuItem* pMenuItem, CContextMenuWnd* pMenuWnd) = 0;
	// 销毁菜单项的子菜单
	virtual void DestroySubMenu(ICtrlContextMenuItem* pMenuItem) = 0;

	// 菜单项消息派发
	virtual void OnMenuItemMessage(IControlBase* pMenuItem, int nMsgId, int nMenuId) = 0;
	// 取得菜单对话框列表
	virtual MENN_WND_VEC* GetMenuWindowList() = 0;
	// 取得菜单的新字体
	virtual void GetMenuFont(IPropertyFontBase** ppNormalFont, IPropertyFontBase** ppHoverFont, IPropertyFontBase** ppDisableFont) = 0;
};
