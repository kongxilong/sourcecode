#include "StdAfx.h"
#include "ICtrlCheckBoxImpl.h"
#include "..\..\Inc\IControlManager.h"


ICtrlCheckBox::ICtrlCheckBox(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlCheckBoxImpl::ICtrlCheckBoxImpl(IUiFeatureKernel *pUiKernel) : ICtrlCheckBox(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_CHECK_BOX);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;

	m_bIsLButtonDown = false;
	m_bCheck = false;

	m_pImageLeft = NULL;
	m_pImageTop = NULL;
	m_pImageNormal = NULL;
	m_pImageHover = NULL;
	m_pImageDown = NULL;
	m_pImageDisbale = NULL;
	m_pImageCheckNormal = NULL;
	m_pImageCheckHover = NULL;
	m_pImageCheckDown = NULL;
	m_pImageCheckDisbale = NULL;
	m_pPropFontGroup = NULL;
	m_pPropText = NULL;
	m_pTextLeft = NULL;
	m_pTextTop = NULL;
	m_pPropFontNormal = NULL;
	m_pPropFontHover = NULL;
	m_pPropFontDown = NULL;
	m_pPropFontDisable = NULL;
}

ICtrlCheckBoxImpl::~ICtrlCheckBoxImpl()
{
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlCheckBoxImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 初始化控件
void ICtrlCheckBoxImpl::OnCreate()
{
}

// 控件初始化完毕
void ICtrlCheckBoxImpl::OnFinalCreate()
{
}

// 销毁控件
void ICtrlCheckBoxImpl::OnDestroy()
{
}

// 绘制控件
void ICtrlCheckBoxImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	if(!(m_pImageNormal && m_pImageLeft && m_pImageTop && m_pTextLeft && m_pTextTop))
	{
		return;
	}

	RECT rcClient = GetClientRect();
	// 所有状态图片大小相等
	SIZE imgSize = m_pImageNormal->GetImageSize();
	// 图片区域
	RECT rcImage = { rcClient.left + m_pImageLeft->GetValue(), rcClient.top + m_pImageTop->GetValue(), 0, 0 };
	int drawSize = min(imgSize.cy, imgSize.cx); 
	rcImage.right = rcImage.left + drawSize;
	rcImage.bottom = rcImage.top + drawSize;
	// rcClient 为文字的区域
	rcClient.left = m_pTextLeft->GetValue();
	rcClient.top = m_pTextTop->GetValue();

	if(!this->IsEnable())
	{
		// 禁止
		if(m_bCheck)
		{
			if (m_pImageCheckDisbale != NULL)
				m_pImageCheckDisbale->DrawImage(DrawBoard, rcImage);
		}
		else
		{
			if (m_pImageDisbale != NULL)
				m_pImageDisbale->DrawImage(DrawBoard, rcImage);
		}

		if (m_pPropFontDisable != NULL && m_pPropText != NULL && m_pPropText->GetLength() > 0)
			m_pPropFontDisable->DrawFontText(DrawBoard, m_pPropText->GetString(), rcClient);
		return;
	}

	if(IsMousehover())
	{
		if(m_bIsLButtonDown)
		{
			// 按下
			if(m_bCheck)
			{
				// checked
				if (m_pImageCheckDown != NULL)
					m_pImageCheckDown->DrawImage(DrawBoard, rcImage);
			}
			else
			{
				// uncheck
				if (m_pImageDown != NULL)
					m_pImageDown->DrawImage(DrawBoard, rcImage);
			}

			if (m_pPropFontDown != NULL && m_pPropText != NULL && m_pPropText->GetLength() > 0)
				m_pPropFontDown->DrawFontText(DrawBoard, m_pPropText->GetString(), rcClient);	
		}
		else
		{
			// hover
			if(m_bCheck)
			{
				// checked
				if (m_pImageCheckHover != NULL)
					m_pImageCheckHover->DrawImage(DrawBoard, rcImage);
			}
			else
			{
				// uncheck
				if (m_pImageHover != NULL)
					m_pImageHover->DrawImage(DrawBoard, rcImage);
			}
			
			if (m_pPropFontHover != NULL && m_pPropText != NULL && m_pPropText->GetLength() > 0)
				m_pPropFontHover->DrawFontText(DrawBoard, m_pPropText->GetString(), rcClient);
		}
	}
	else
	{
		// normal
		if(m_bCheck)
		{
			// checked
			if (m_pImageCheckNormal != NULL)
				m_pImageCheckNormal->DrawImage(DrawBoard, rcImage);	
		}
		else
		{
			// uncheck
			m_pImageNormal->DrawImage(DrawBoard, rcImage);
		}

		if (m_pPropFontNormal != NULL && m_pPropText != NULL && m_pPropText->GetLength() > 0)
			m_pPropFontNormal->DrawFontText(DrawBoard, m_pPropText->GetString(), rcClient);
	}
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlCheckBoxImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	IPropertyGroup *pGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "ImageGroup", "图片组");

	m_pImageNormal =		 (IPropertyImage*)CreatePropetry(pGroup, OTID_IMAGE, "NormalImage", "Normal图片");
	m_pImageHover =			 (IPropertyImage*)CreatePropetry(pGroup, OTID_IMAGE, "HoverImage", "Hover图片");
	m_pImageDown =			 (IPropertyImage*)CreatePropetry(pGroup, OTID_IMAGE, "DownImage", "Down图片");
	m_pImageDisbale =		 (IPropertyImage*)CreatePropetry(pGroup, OTID_IMAGE, "DisableImage", "Disable图片");

	m_pImageCheckNormal =	 (IPropertyImage*)CreatePropetry(pGroup, OTID_IMAGE, "CheckNormalImage", "CheckNormal图片");
	m_pImageCheckHover =	 (IPropertyImage*)CreatePropetry(pGroup, OTID_IMAGE, "CheckHoverImage", "CheckHover图片");
	m_pImageCheckDown =		 (IPropertyImage*)CreatePropetry(pGroup, OTID_IMAGE, "CheckDownImage", "CheckDown图片");
	m_pImageCheckDisbale =	 (IPropertyImage*)CreatePropetry(pGroup, OTID_IMAGE, "CheckDisableImage", "CheckDisable图片");
	
	m_pPropFontGroup =		 (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "TextGroup", "文字组");
	m_pPropText =			 (IPropertyString*)CreatePropetry(m_pPropFontGroup, OTID_STRING, "Text", "文字内容");
	m_pPropFontNormal =		 (IPropertyFont*)CreatePropetry(m_pPropFontGroup, OTID_FONT, "NormalFont", "正常状态显示的文字字体");
	m_pPropFontHover =		 (IPropertyFont*)CreatePropetry(m_pPropFontGroup, OTID_FONT, "HoverFont", "悬浮状态显示的文字字体");
	m_pPropFontDown =		 (IPropertyFont*)CreatePropetry(m_pPropFontGroup, OTID_FONT, "DownFont", "按下状态显示的文字字体");;
	m_pPropFontDisable =	 (IPropertyFont*)CreatePropetry(m_pPropFontGroup, OTID_FONT, "DisableFont", "禁止状态显示的文字字体");;
	
	pGroup =				 (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "AnchGroup", "定位组");
	m_pImageLeft =			 (IPropertyInt*)CreatePropetry(pGroup, OTID_INT, "ImageLeft", "图片左边距");;
	m_pImageTop =			 (IPropertyInt*)CreatePropetry(pGroup, OTID_INT, "ImageTop", "图片上边距");;
	m_pTextLeft =			 (IPropertyInt*)CreatePropetry(pGroup, OTID_INT, "TextLeft", "文字左边距");;
	m_pTextTop =			 (IPropertyInt*)CreatePropetry(pGroup, OTID_INT, "TextTop", "文字上边距");;
	
	return true;
}

// 鼠标左键抬起
void ICtrlCheckBoxImpl::OnLButtonUp(POINT pt)
{
	if(!this->IsEnable())
	{
		return;
	}

	if (m_pWindowBase == NULL)
		return;
	
	if(m_bIsLButtonDown)
	{
		m_bCheck = !m_bCheck;
		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_CHECK_BOX_UP, NULL, NULL);
	}

	m_bIsLButtonDown = false;
	this->RedrawControl(true);
}

// 鼠标左键点击
void ICtrlCheckBoxImpl::OnLButtonDown(POINT pt)
{	
	if(!this->IsEnable())
	{
		return;
	}

	if (m_pWindowBase == NULL)
		return;

	m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_CHECK_BOX_DOWN, NULL, NULL);

	m_bIsLButtonDown = true;
	this->RedrawControl(true);
}

// 设置Link文字
void ICtrlCheckBoxImpl::SetText(LPCWSTR pText, bool bRedraw)
{
	if (pText == NULL)
		return;

	if (m_pPropText == NULL)
		m_pPropText = (IPropertyString*)CreatePropetry(m_pPropFontGroup, OTID_STRING, "Text", "文字内容", true);

	if (m_pPropText == NULL)
		return;

	m_pPropText->SetString(pText);
	this->RedrawControl(bRedraw);
}

// 鼠标进入
void ICtrlCheckBoxImpl::OnMouseEnter(POINT pt)
{
	if(!this->IsEnable())
	{
		return;
	}

	this->RedrawControl(true);
}

// 鼠标移出
void ICtrlCheckBoxImpl::OnMouseLeave(POINT pt)
{
	if(!this->IsEnable())
	{
		return;
	}

	this->RedrawControl(true);
}

void ICtrlCheckBoxImpl::SetCheck(bool bCheck)
{
	m_bCheck = bCheck;
	this->RedrawControl(true);
}

bool ICtrlCheckBoxImpl::GetCheck()
{
	return m_bCheck;
}
