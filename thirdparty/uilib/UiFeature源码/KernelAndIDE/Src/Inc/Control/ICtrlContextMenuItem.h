
#pragma once
#include "..\ICtrlInterface.h"
#include ".\..\IPropertyFontBase.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_CONTEXTMENUITEM						(L"ContextMenuItem")

enum CTRL_MSG_MENU_ITEM
{
	// MenuItem消息Base，此消息值不允许被发送，只是作为消息值的Base
	CM_MENU_ITEM_BASE	= 0x10012000,
	// MenuItem Down消息
	CM_MENU_ITEM_DOWN,
	// MenuItem Up消息
	CM_MENU_ITEM_UP
};

class ICtrlContextMenu;

class _declspec(novtable) ICtrlContextMenuItem : public ICtrlInterface
{
public:
	ICtrlContextMenuItem(IUiFeatureKernel *pUiKernel);
	// 设置引导控件
	virtual void SetContextMenu(ICtrlContextMenu* pContextMenu) = 0;
	// 设置菜单的新字体
	virtual void SetMenuFont(IPropertyFontBase* pNormalFont, IPropertyFontBase* pHoverFont, IPropertyFontBase* pDisableFont) = 0;
};
