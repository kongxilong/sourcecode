
#include "StdAfx.h"
#include "ICtrlStaticImpl.h"
#include "..\..\Inc\IControlManager.h"
#include "..\..\Inc\ICommonFun.h"

ICtrlStatic::ICtrlStatic(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlStaticImpl::ICtrlStaticImpl(IUiFeatureKernel *pUiKernel) : ICtrlStatic(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_STATIC);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;

	m_pPropImage = NULL;
	m_pPropFont = NULL;
	m_pPropText = NULL;
	m_pPorpTextLeftGlow = NULL;	
	m_pPorpTextTopGlow = NULL;		
	m_pPorpTextRightGlow = NULL;
	m_pPorpTextBottomGlow = NULL;
}

ICtrlStaticImpl::~ICtrlStaticImpl()
{

}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlStaticImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 初始化控件
void ICtrlStaticImpl::OnCreate()
{

}

// 控件初始化完毕
void ICtrlStaticImpl::OnFinalCreate()
{

}

// 派生控件用于创建属于自己的控件属性
bool ICtrlStaticImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	// 设置背景图片、字体、显示文字属性
	m_pPropImage = (IPropertyImage*)CreatePropetry(NULL, OTID_IMAGE, "Image", "背景图片");
	m_pPropFont = (IPropertyFont*)CreatePropetry(NULL, OTID_FONT, "Font", "字体");
	m_pPropText = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "Text", "显示的文字");
	m_pPorpTextLeftGlow = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "TextLeftGlow", "文字左边距");	
	m_pPorpTextTopGlow = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "TextTopGlow", "文字上边距");	
	m_pPorpTextRightGlow = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "TextRightGlow", "文字右边距");
	m_pPorpTextBottomGlow = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "TextBottomGlow", "文字下边距");

	if (bIsNewCtrl && m_pWindowBase != NULL)
	{
		// 设置默认不接受鼠标信息
		m_pPropBase_RcvMouseMsg->SetValue(false);
	}
	return true;
}

// 设置控件文本
bool ICtrlStaticImpl::SetText(LPCWSTR pszText)
{
	if (NULL == pszText)
		return false;

	if (m_pPropText == NULL)
		m_pPropText = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "Text", "", true);

	if (m_pPropText == NULL)
		return false;

	m_pPropText->SetString(pszText);
	return true;
}

LPCWSTR ICtrlStaticImpl::GetText()
{
	if (m_pPropText == NULL)
		return NULL;

	return m_pPropText->GetString();
}

// 销毁控件
void ICtrlStaticImpl::OnDestroy()
{

}

// 绘制控件
void ICtrlStaticImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	if (NULL != m_pPropImage)
		m_pPropImage->DrawImage(DrawBoard, this->GetClientRect());

	if (NULL != m_pPropFont && m_pPropText != NULL && m_pPropText->GetLength() > 0)
	{
		RECT rcText = GetClientRect();
		if (m_pPorpTextLeftGlow)
			rcText.left += m_pPorpTextLeftGlow->GetValue();
		if (m_pPorpTextRightGlow)
			rcText.right -= m_pPorpTextRightGlow->GetValue();
		if (m_pPorpTextTopGlow)
			rcText.top += m_pPorpTextTopGlow->GetValue();
		if (m_pPorpTextBottomGlow)
			m_pPorpTextBottomGlow->GetValue();

		m_pPropFont->DrawFontText(DrawBoard, m_pPropText->GetString(), rcText);
	}
}
