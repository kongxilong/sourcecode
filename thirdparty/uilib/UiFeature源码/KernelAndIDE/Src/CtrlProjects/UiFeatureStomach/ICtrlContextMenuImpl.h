
#pragma once
#include "..\..\Inc\Control\ICtrlContextMenu.h"
#include "ContextMenuWnd.h"
#include "..\..\Inc\Control\ICtrlContextMenuItem.h"

class ICtrlContextMenuImpl : public ICtrlContextMenu
{
public:
	ICtrlContextMenuImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlContextMenuImpl();

	// 创建菜单面板
	virtual bool CreatePopupMenu();
	// 显示菜单面板，这个函数不会导致wait
	virtual bool TrackPopupMenu(POINT pt, WPARAM wMenuParam = NULL);
	// 销毁菜单
	virtual bool DestroyMenu();

	// 菜单项的高度
	virtual int GetItemHeight();
	// 菜单分割线的高度
	virtual int GetSeparatorHeight();

	// 菜单项距离菜单面板左侧边距
	virtual int GetLeftSpace();
	// 菜单项距离菜单面板右侧边距
	virtual int GetRightSpace();
	// 菜单项距离菜单面板顶部边距
	virtual int GetTopSpace();
	// 菜单项距离菜单面板底部边距
	virtual int GetBottomSpace();
	// 菜单项之间的距离
	virtual int GetItemSpace();
	// 菜单面板之间的左右的间隔
	virtual int GetPanelSpace();
	// 取得菜单面板的参数
	virtual WPARAM GetPopupMenuParam();

	// 设置菜单的新字体
	virtual void SetMenuFont(IPropertyFontBase* pNormalFont, IPropertyFontBase* pHoverFont, IPropertyFontBase* pDisableFont);

	// 取得指定子菜单的item
	virtual ICtrlContextMenuItem* GetMenuItem(WCHAR * pszMenuItemName);

protected:
	// 弹出菜单项的子菜单
	virtual void PopupSubMenu(ICtrlContextMenuItem* pMenuItem, CContextMenuWnd* pMenuWnd);
	// 销毁菜单项的子菜单
	virtual void DestroySubMenu(ICtrlContextMenuItem* pMenuItem);

	// 菜单项消息派发
	virtual void OnMenuItemMessage(IControlBase* pMenuItem, int nMsgId, int nMenuId);
	// 取得菜单对话框列表
	virtual MENN_WND_VEC* GetMenuWindowList();

	// 派生控件用于创建属于自己的控件属性
	// bIsNewCtrl：true时说明是Builder调用，新创建一个控件，需要初始化属性的各个默认值
	// false：说明整个皮肤包已经初始化完毕，属性创建完毕的时候，从xml中读取的属性值已经赋值完毕了，不能再初始化，否则xml中的值会被冲掉
	virtual bool CreateControlPropetry(bool bIsNewCtrl);
	// 初始化控件
	virtual void OnCreate();
	// 控件初始化完毕
	virtual void OnFinalCreate();
	// 销毁控件
	virtual void OnDestroy();
	// 绘制控件
	virtual void OnPaint(CDrawingBoard &DrawBoard);
	// 控件需要处理的一些自定义的消息，消息基本上会由对话框或者其他控件传入
	virtual int OnCtrlNotify(int nMsgId, WPARAM wParam, LPARAM lParam);
	// 取得菜单的新字体
	virtual void GetMenuFont(IPropertyFontBase** ppNormalFont, IPropertyFontBase** ppHoverFont, IPropertyFontBase** ppDisableFont);

	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);
private:
	bool OnDestroyMenuMessage();

private:
	// 菜单面板窗口
	CContextMenuWnd *m_pContextMenuWnd;

	IPropertyString* m_pPropMenuPanelName;
	IPropertyInt* m_pPropItemLeftSpace;
	IPropertyInt* m_pPropItemRightSpace;
	IPropertyInt* m_pPropItemTopSpace;
	IPropertyInt* m_pPropItemBottomSpace;
	IPropertyInt* m_pPropItemSpace;
	IPropertyInt* m_pPropPanelSpace;

	IPropertyInt* m_pPropItemHeight;
	IPropertyInt* m_pPropSeparatorHeight;
	IPropertyString* m_pPropSkinPassword;					// 皮肤包的密码

	// 所有菜单窗口列表
	MENN_WND_VEC m_MenuWndVec;

	IPropertyFontBase* m_pNewNormalFont;
	IPropertyFontBase* m_pNewHoverFont;
	IPropertyFontBase* m_pNewDisableFont;

	WPARAM m_wpTrackMenuParam;
};
