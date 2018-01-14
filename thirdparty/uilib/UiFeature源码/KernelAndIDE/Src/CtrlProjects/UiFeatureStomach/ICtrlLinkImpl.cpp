
#include "StdAfx.h"
#include "ICtrlLinkImpl.h"
#include "..\..\Inc\IControlManager.h"


ICtrlLink::ICtrlLink(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlLinkImpl::ICtrlLinkImpl(IUiFeatureKernel *pUiKernel) : ICtrlLink(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_LINK);

	m_bIsLButtonDown = false;

	m_pPropLinkText = NULL;
	m_pPropLinkUrl = NULL;
	m_pPropCursor = NULL;
	m_PropFontGroup = NULL;
	m_pPropFontNormal = NULL;
	m_pPropFontHover = NULL;
	m_pPropFontDown = NULL;
}

ICtrlLinkImpl::~ICtrlLinkImpl()
{
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlLinkImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 初始化控件
void ICtrlLinkImpl::OnCreate()
{
}

// 控件初始化完毕
void ICtrlLinkImpl::OnFinalCreate()
{
}

// 销毁控件
void ICtrlLinkImpl::OnDestroy()
{
}

// 绘制控件
void ICtrlLinkImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	if (m_pPropLinkText == NULL || m_pPropLinkText->GetLength() <= 0)
		return;

	IPropertyFont* pPropFont = m_pPropFontDown;
	if (!m_bIsLButtonDown)
	{
		if (IsMousehover())
			pPropFont = m_pPropFontHover;
		else
			pPropFont = m_pPropFontNormal;
	}

	if (pPropFont != NULL)
		pPropFont->DrawFontText(DrawBoard, m_pPropLinkText->GetString(), GetClientRect());
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlLinkImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	m_pPropLinkText = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "LinkText", "超链接显示的文字");
	m_pPropLinkUrl = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "LinkUrl", "超链接指向的地址");
	m_pPropCursor = (IPropertyCursor*)CreatePropetry(NULL, OTID_CURSOR, "CursorStyle", "超链接的鼠标手势");
	m_PropFontGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "FontGroup", "超链接显示的文字字体");
	m_pPropFontNormal = (IPropertyFont*)CreatePropetry(m_PropFontGroup, OTID_FONT, "Normal", "正常状态显示的文字字体");
	m_pPropFontHover = (IPropertyFont*)CreatePropetry(m_PropFontGroup, OTID_FONT, "MouseHover", "鼠标Hover状态显示的文字字体");
	m_pPropFontDown = (IPropertyFont*)CreatePropetry(m_PropFontGroup, OTID_FONT, "MouseDown", "鼠标按下状态显示的文字字体");

	return true;
}

// 鼠标左键抬起
void ICtrlLinkImpl::OnLButtonUp(POINT pt)
{
	m_bIsLButtonDown = false;
	this->RedrawControl(true);

	if (m_pWindowBase == NULL || m_pPropLinkUrl == NULL || m_pPropLinkUrl->GetLength() <= 0)
		return;

	m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_LINK_UP, (WPARAM)m_pPropLinkUrl->GetString(), NULL);
}

// 设置控件光标
IPropertyCursor* ICtrlLinkImpl::OnSetCursor(POINT pt)
{
	return m_pPropCursor;
}

// 鼠标左键点击
void ICtrlLinkImpl::OnLButtonDown(POINT pt)
{
	m_bIsLButtonDown = true;
	this->RedrawControl(true);

	if (m_pWindowBase == NULL || m_pPropLinkUrl == NULL || m_pPropLinkUrl->GetLength() <= 0)
		return;

	m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_LINK_DOWN, (WPARAM)m_pPropLinkUrl->GetString(), NULL);
}

// 设置Link文字
void ICtrlLinkImpl::SetLinkText(LPCWSTR pText, bool bRedraw)
{
	if (pText == NULL)
		return;

	if (m_pPropLinkText == NULL)
		m_pPropLinkText = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "LinkText", "超链接显示的文字", true);

	if (m_pPropLinkText == NULL)
		return;

	m_pPropLinkText->SetString(pText);
	this->RedrawControl(bRedraw);
}

// 设置Link链接地址URL
void ICtrlLinkImpl::SetLinkUrl(LPCWSTR pUrl)
{
	if (pUrl == NULL)
		return;

	if (m_pPropLinkUrl == NULL)
		m_pPropLinkUrl = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "LinkUrl", "超链接指向的地址", true);

	if (m_pPropLinkUrl == NULL)
		return;

	m_pPropLinkUrl->SetString(pUrl);
}

// 鼠标进入
void ICtrlLinkImpl::OnMouseEnter(POINT pt)
{
	this->RedrawControl(true);
}

// 鼠标移出
void ICtrlLinkImpl::OnMouseLeave(POINT pt)
{
	this->RedrawControl(true);
}

// 取得Link链接地址URL
LPCWSTR ICtrlLinkImpl::GetLinkUrl()
{
	if (m_pPropLinkUrl == NULL)
		return NULL;

	return m_pPropLinkUrl->GetString();
}
