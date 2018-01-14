
#include "StdAfx.h"
#include "ICtrlContextMenuItemImpl.h"
#include "..\..\Inc\Control\ICtrlContextMenu.h"
#include "..\..\Inc\UiFeatureDefs.h"

// 鼠标按下标志
#define MOUSE_FLAG_DOWN								(0x0000001UL)
ICtrlContextMenuItem::ICtrlContextMenuItem(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlContextMenuItemImpl::ICtrlContextMenuItemImpl(IUiFeatureKernel *pUiKernel) : ICtrlContextMenuItem(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_CONTEXTMENUITEM);

	m_pSubContextMenuWnd = NULL;
	m_pPropEnabled = NULL;
	m_pPropIsSeparator = NULL;
	m_pPropImageSeparator = NULL;
	m_pPropText = NULL;
	m_pPropIndex = NULL;
	m_pContextMenu = NULL;
	m_pPropMenuPanelName = NULL;
	m_enumMenuItemState = MeunItemStateNormal;

	for (int i = 0; i < MeunItemStateNum; i++)
	{
		m_pPropImages[i] = NULL;
		m_pPropIcons[i] = NULL;
		m_pPropFonts[i] = NULL;
		m_pPropArrows[i] = NULL;
	}
	for(int i = 0; i< 2 ;i++)
	{
		m_pPropIconPosition[i] = 0;
		m_pPropIconSize[i] = 0;

		m_pPropArrowPosition[i] = 0;
		m_pPropArrowSize[i] = 0;

		m_pPropSeparatorPosition[i] = 0;
		m_pPropSeparatorSize[i] = 0;

	}

	for(int i = 0; i< PaddingNum ;i++)
	{
		m_pPropTextPadding[i] = 0;
	}

	m_mouseFlag = 0;
}

ICtrlContextMenuItemImpl::~ICtrlContextMenuItemImpl()
{
	SAFE_DELETE(m_pSubContextMenuWnd);
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlContextMenuItemImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlContextMenuItemImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	m_pPropMenuPanelName = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "SubMenuWndObjName", "子菜单的窗口名称：Builder中的Window的ObjectName");
	m_pPropEnabled			= (IPropertyBool*)CreatePropetry(NULL, OTID_BOOL, "Enabled", "是否启用");
	m_pPropIsSeparator		= (IPropertyBool*)CreatePropetry(NULL, OTID_BOOL, "IsSeparator", "是否分隔符");
	m_pPropText = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "Text", "MenuItem上显示的文言");
	if (m_pPropText != NULL && bIsNewCtrl)
		m_pPropText->SetString(L"显示的文本");
	m_pPropIndex = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "Order", "菜单项顺序号(从0开始)");

	//设置MenuItem背景图片
	IPropertyGroup* pImagePropGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "Image", "MenuItem的背景图片Group");
	m_pPropImages[MeunItemStateNormal] = (IPropertyImage*)CreatePropetry(pImagePropGroup, OTID_IMAGE, "NormalImage", "Normal状态下MenuItem的背景图片");
	m_pPropImages[MeunItemStateHover] = (IPropertyImage*)CreatePropetry(pImagePropGroup, OTID_IMAGE, "HoverImage", "Hover状态下MenuItem的背景图片");
	m_pPropImages[MeunItemStateDisable] = (IPropertyImage*)CreatePropetry(pImagePropGroup, OTID_IMAGE, "DisableImage", "禁用状态下MenuItem的背景图片");

	//设置MenuItem上Icon
	IPropertyGroup* pIconPropGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "Icon", "Icon Group");
	m_pPropIcons[MeunItemStateNormal] = (IPropertyImage*)CreatePropetry(pIconPropGroup, OTID_IMAGE, "NormalIcon", "Normal状态下MenuItem上的Icon");
	m_pPropIcons[MeunItemStateHover] = (IPropertyImage*)CreatePropetry(pIconPropGroup, OTID_IMAGE, "HoverIcon", "Hover状态下MenuItem上的Icon");
	m_pPropIcons[MeunItemStateDisable] = (IPropertyImage*)CreatePropetry(pIconPropGroup, OTID_IMAGE, "DisableIcon", "禁用状态下MenuItem上的Icon");
	m_pPropIconPosition[0] = (IPropertyInt*)CreatePropetry(pIconPropGroup, OTID_INT, "Top", "上边距");
	m_pPropIconPosition[1] = (IPropertyInt*)CreatePropetry(pIconPropGroup, OTID_INT, "Left", "左边距");
	m_pPropIconSize[0] = (IPropertyInt*)CreatePropetry(pIconPropGroup, OTID_INT, "Width", "宽");
	m_pPropIconSize[1] = (IPropertyInt*)CreatePropetry(pIconPropGroup, OTID_INT, "Height", "高");


	//设置MenuItem上Arrow
	IPropertyGroup* pArrowPropGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "Arrow", "MenuItem上的Arrow Group");
	m_pPropArrows[MeunItemStateNormal] = (IPropertyImage*)CreatePropetry(pArrowPropGroup, OTID_IMAGE, "NormalArrow", "Normal状态下MenuItem上的Arrow");
	m_pPropArrows[MeunItemStateHover] = (IPropertyImage*)CreatePropetry(pArrowPropGroup, OTID_IMAGE, "HoverArrow", "Hover状态下MenuItem上的Arrow");
	m_pPropArrows[MeunItemStateDisable] = (IPropertyImage*)CreatePropetry(pArrowPropGroup, OTID_IMAGE, "DisableArrow", "禁用状态下MenuItem上的Arrow");
	m_pPropArrowPosition[0] = (IPropertyInt*)CreatePropetry(pArrowPropGroup, OTID_INT, "Top", "上边距");
	m_pPropArrowPosition[1] = (IPropertyInt*)CreatePropetry(pArrowPropGroup, OTID_INT, "Right", "右边距");
	m_pPropArrowSize[0] = (IPropertyInt*)CreatePropetry(pArrowPropGroup, OTID_INT, "Width", "宽");
	m_pPropArrowSize[1] = (IPropertyInt*)CreatePropetry(pArrowPropGroup, OTID_INT, "Height", "高");

	//设置MenuItem上文言的字体
	IPropertyGroup* pFontPropGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "Font", "MenuItem上文言的字体 Group");
	m_pPropFonts[MeunItemStateNormal] = (IPropertyFont*)CreatePropetry(pFontPropGroup, OTID_FONT, "NormalFont", "Normal状态下MenuItem上的文言的字体");
	m_pPropFonts[MeunItemStateHover] = (IPropertyFont*)CreatePropetry(pFontPropGroup, OTID_FONT, "HoverFont", "Hover状态下MenuItem上的文言的字体");
	m_pPropFonts[MeunItemStateDisable] = (IPropertyFont*)CreatePropetry(pFontPropGroup, OTID_FONT, "DisableFont", "禁用状态下MenuItem上的文言的字体");
	m_pPropTextPadding[Top] = (IPropertyInt*)CreatePropetry(pFontPropGroup, OTID_INT, "Top", "上边距");
	m_pPropTextPadding[Left] = (IPropertyInt*)CreatePropetry(pFontPropGroup, OTID_INT, "Left", "左边距");
	m_pPropTextPadding[Bottom] = (IPropertyInt*)CreatePropetry(pFontPropGroup, OTID_INT, "Bottom", "下边距");
	m_pPropTextPadding[Right] = (IPropertyInt*)CreatePropetry(pFontPropGroup, OTID_INT, "Right", "右边距");

	//设置Separator
	IPropertyGroup* pSeparatorPropGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "Separator", "Separator Group");
	m_pPropImageSeparator	= (IPropertyImage*)CreatePropetry(pSeparatorPropGroup, OTID_IMAGE, "SeparatorImage", "分隔符背景图");
	m_pPropSeparatorPosition[0] = (IPropertyInt*)CreatePropetry(pSeparatorPropGroup, OTID_INT, "Top", "上边距");
	m_pPropSeparatorPosition[1] = (IPropertyInt*)CreatePropetry(pSeparatorPropGroup, OTID_INT, "Left", "左边距");
	m_pPropSeparatorSize[0] = (IPropertyInt*)CreatePropetry(pSeparatorPropGroup, OTID_INT, "Width", "宽");
	m_pPropSeparatorSize[1] = (IPropertyInt*)CreatePropetry(pSeparatorPropGroup, OTID_INT, "Height", "高");

	return true;
}

// 初始化控件
void ICtrlContextMenuItemImpl::OnCreate()
{
}

// 控件初始化完毕
void ICtrlContextMenuItemImpl::OnFinalCreate()
{
}

// 销毁控件
void ICtrlContextMenuItemImpl::OnDestroy()
{
}

// 绘制控件
void ICtrlContextMenuItemImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	//绘制分割条
	if(NULL != m_pPropIsSeparator)
	{
		if(m_pPropIsSeparator->GetValue())
		{
			if (NULL != m_pPropImageSeparator)
			{
				RECT rect = GetClientRect();
				rect.top += m_pPropSeparatorPosition[0]->GetValue();
				rect.left += m_pPropSeparatorPosition[1]->GetValue();
				rect.bottom = rect.top + m_pPropSeparatorSize[1]->GetValue();
				rect.right =rect.left + m_pPropSeparatorSize[0]->GetValue();
				m_pPropImageSeparator->DrawImage(DrawBoard, GetClientRect());
			}
			return;
		}
	}

	if (IsEnable()) 
	{
		if (m_enumMenuItemState == MeunItemStateDisable)
			m_enumMenuItemState = MeunItemStateNormal;
	}
	else
	{
		m_enumMenuItemState = MeunItemStateDisable;
	}


	//绘制背景图
	if (NULL != m_pPropImages[m_enumMenuItemState])
		m_pPropImages[m_enumMenuItemState]->DrawImage(DrawBoard, GetClientRect());

	//绘制左侧的icon
	if (NULL != m_pPropIcons[m_enumMenuItemState])
	{
		RECT rect = GetClientRect();
		rect.top += m_pPropIconPosition[0]->GetValue();
		rect.left += m_pPropIconPosition[1]->GetValue();
		rect.bottom = rect.top + m_pPropIconSize[1]->GetValue();
		rect.right =rect.left + m_pPropIconSize[0]->GetValue();
		m_pPropIcons[m_enumMenuItemState]->DrawImage(DrawBoard, rect);
	}

	//绘制中间的文言
	if (NULL != m_pPropFonts[m_enumMenuItemState])
	{
		RECT rect = GetClientRect();
		rect.top += m_pPropTextPadding[Top]->GetValue();
		rect.left += m_pPropTextPadding[Left]->GetValue();
		rect.bottom -= m_pPropTextPadding[Bottom]->GetValue();
		rect.right -= m_pPropTextPadding[Right]->GetValue();
		m_pPropFonts[m_enumMenuItemState]->DrawFontText(DrawBoard, m_pPropText->GetString(), rect);
	}

	//绘制右侧的箭头
	if(HaveSubMenu())
	{
		if (NULL != m_pPropArrows[m_enumMenuItemState])
		{
			RECT rect = GetClientRect();
			rect.top += m_pPropArrowPosition[0]->GetValue();
			rect.right -= m_pPropIconPosition[1]->GetValue();
			rect.bottom = rect.top + m_pPropArrowSize[1]->GetValue();
			rect.left =rect.right - m_pPropArrowSize[0]->GetValue();
			m_pPropArrows[m_enumMenuItemState]->DrawImage(DrawBoard, rect);
		}
	}
}

bool ICtrlContextMenuItemImpl::HaveSubMenu()
{
	if (m_pPropMenuPanelName == NULL)
		return false;

	if (m_pPropMenuPanelName->GetString() == NULL)
		return false;

	if (wcslen(m_pPropMenuPanelName->GetString()) <= 0)
		return false;

	return true;
}

// 鼠标进入
void ICtrlContextMenuItemImpl::OnMouseEnter(POINT pt)
{
	ChangeMenuItemStyle(MeunItemStateHover);
	if(NULL != m_pContextMenu && HaveSubMenu())
	{
		if (m_pSubContextMenuWnd == NULL)
			m_pSubContextMenuWnd = new CContextMenuWnd;

		m_pContextMenu->PopupSubMenu(this, m_pSubContextMenuWnd);
	}
}

// 鼠标移出
void ICtrlContextMenuItemImpl::OnMouseLeave(POINT pt)
{
	ChangeMenuItemStyle(MeunItemStateNormal);
	if(NULL != m_pContextMenu && HaveSubMenu())
		m_pContextMenu->DestroySubMenu(this);

	if (m_mouseFlag & MOUSE_FLAG_DOWN)
	{
		m_pContextMenu->DestroyMenu();
	}

	m_mouseFlag &= ~MOUSE_FLAG_DOWN;
}

// 鼠标左键点击
void ICtrlContextMenuItemImpl::OnLButtonDown(POINT pt)
{
	if(IsEnable() && !m_pPropIsSeparator->GetValue())
	{
		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_MENU_ITEM_DOWN, m_pPropIndex->GetValue(), NULL);
	}

	m_mouseFlag |= MOUSE_FLAG_DOWN;
}

// 鼠标左键抬起
void ICtrlContextMenuItemImpl::OnLButtonUp(POINT pt)
{
	if(IsEnable() 
		&& ::PtInRect(&GetClientRect(),pt)
		&& !m_pPropIsSeparator->GetValue())
	{
		if(::PtInRect(&GetClientRect(),pt))
		{
			m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_MENU_ITEM_UP, m_pPropIndex->GetValue(), NULL);
		}
	}

	if (m_mouseFlag & MOUSE_FLAG_DOWN)
	{
		m_pContextMenu->DestroyMenu();
	}

	m_mouseFlag &= ~MOUSE_FLAG_DOWN;
}

// 设置引导控件
void ICtrlContextMenuItemImpl::SetContextMenu(ICtrlContextMenu* pContextMenu)
{
	m_pContextMenu = pContextMenu;
}

// 变更MenuITem显示的风格
void ICtrlContextMenuItemImpl::ChangeMenuItemStyle(MENUITEMSTATE state)
{
	if(state != m_enumMenuItemState)
	{
		m_enumMenuItemState = state;
		RedrawControl(true);
	}
}

// 设置菜单的新字体
void ICtrlContextMenuItemImpl::SetMenuFont(IPropertyFontBase* pNormalFont, IPropertyFontBase* pHoverFont, IPropertyFontBase* pDisableFont)
{
	if (pNormalFont != NULL && m_pPropFonts[MeunItemStateNormal] != NULL)
		m_pPropFonts[MeunItemStateNormal]->SetRelevancyProp(dynamic_cast<IPropertyBase*>(pNormalFont));

	if (pHoverFont != NULL && m_pPropFonts[MeunItemStateHover] != NULL)
		m_pPropFonts[MeunItemStateHover]->SetRelevancyProp(dynamic_cast<IPropertyBase*>(pHoverFont));

	if (pDisableFont != NULL && m_pPropFonts[MeunItemStateDisable] != NULL)
		m_pPropFonts[MeunItemStateDisable]->SetRelevancyProp(dynamic_cast<IPropertyBase*>(pDisableFont));
}
