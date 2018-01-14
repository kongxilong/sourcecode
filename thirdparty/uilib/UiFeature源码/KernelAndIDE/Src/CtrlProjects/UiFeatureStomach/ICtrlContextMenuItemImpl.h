
#pragma once
#include "..\..\Inc\Control\ICtrlContextMenuItem.h"
#include "ContextMenuWnd.h"

class ICtrlContextMenuItemImpl : public ICtrlContextMenuItem
{
public:
	ICtrlContextMenuItemImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlContextMenuItemImpl();

protected:
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
	// 鼠标进入
	virtual void OnMouseEnter(POINT pt);
	// 鼠标移出
	virtual void OnMouseLeave(POINT pt);
	// 鼠标左键点击
	virtual void OnLButtonDown(POINT pt);
	// 鼠标左键抬起
	virtual void OnLButtonUp(POINT pt);
	// 设置引导控件
	virtual void SetContextMenu(ICtrlContextMenu* pContextMenu);
	// 设置菜单的新字体
	virtual void SetMenuFont(IPropertyFontBase* pNormalFont, IPropertyFontBase* pHoverFont, IPropertyFontBase* pDisableFont);
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);
private:
	typedef enum enumMeunItemState
	{
		MeunItemStateInvalid = -1,
		MeunItemStateNormal,
		MeunItemStateHover,
		MeunItemStateDisable,
		//在此之前增加MeunItem的状态
		MeunItemStateNum
	}MENUITEMSTATE;

	typedef enum enumPadding
	{
		Invalid = -1,
		Top,
		Left,
		Bottom,
		Right,
		PaddingNum
	}PADDING;

private:
	// 变更MenuITem显示的风格
	void ChangeMenuItemStyle(MENUITEMSTATE state);
	bool HaveSubMenu();

private:
	// 子菜单面板窗口
	CContextMenuWnd*	m_pSubContextMenuWnd;

	// 窗口名称
	IPropertyString*	m_pPropMenuPanelName;
	// 是否启用
	IPropertyBool*		m_pPropEnabled;
	// 是否分隔符
	IPropertyBool*		m_pPropIsSeparator;
	// 分隔符背景图
	IPropertyImage*		m_pPropImageSeparator;
	// 是否有子菜单
	IPropertyBool*		m_pPropHasChild;
	// 菜单项背景图
	IPropertyImage*		m_pPropImages[MeunItemStateNum];
	// 菜单项Icon图标
	IPropertyImage*		m_pPropIcons[MeunItemStateNum];
	// 菜单项标题字体
	IPropertyFont*		m_pPropFonts[MeunItemStateNum];
	// 菜单项箭头图标
	IPropertyImage*		m_pPropArrows[MeunItemStateNum];
	// 菜单项Icon的位置
	IPropertyInt*		m_pPropArrowPosition[2];
	// 菜单项Icon的大小
	IPropertyInt*		m_pPropArrowSize[2];
	// 菜单项文言的边距
	IPropertyInt*		m_pPropTextPadding[PaddingNum];
	// 菜单项Arrow的位置
	IPropertyInt*		m_pPropIconPosition[2];
	// 菜单项Arrow的大小
	IPropertyInt*		m_pPropIconSize[2];

	IPropertyInt*		m_pPropSeparatorPosition[2];
	IPropertyInt*		m_pPropSeparatorSize[2]; 


	// 菜单项顺序号(从0开始)
	IPropertyInt*		m_pPropIndex;
	// 菜单项标题
	IPropertyString*	m_pPropText;
	// 子菜单
	ICtrlContextMenu*	m_pContextMenu;

	MENUITEMSTATE		m_enumMenuItemState;
	ULONG				m_mouseFlag;
};
