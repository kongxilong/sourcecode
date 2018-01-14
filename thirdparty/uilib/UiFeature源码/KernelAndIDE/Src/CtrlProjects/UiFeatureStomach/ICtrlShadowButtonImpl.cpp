
#include "StdAfx.h"
#include "ICtrlShadowButtonImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"

// 鼠标按下标志
#define MOUSE_FLAG_DOWN								(0x0000001UL)

ICtrlShadowButton::ICtrlShadowButton(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlShadowButtonImpl::ICtrlShadowButtonImpl(IUiFeatureKernel *pUiKernel) : ICtrlShadowButton(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_SHADOW_BUTTON);

	m_mouseFlag = 0;
	m_enumBtnState = BtnStateNormal;

	m_pPropButtonTypeCombo = NULL;
	m_pPropText = NULL;
	m_pPropRgnGroup = NULL;
	m_pPropRgnMaskImage = NULL;
	m_hRgn = NULL;

	for (int i = 0; i < BtnStateNum; i++)
	{
		m_pPropImages[i] = NULL;
		m_pPropFonts[i] = NULL;
	}

	m_bIsCheck = false;

	m_pPropCheckBtnGroup = NULL;
	m_pPropShowPanelName = NULL;
	m_pCheckPanel = NULL;
	m_pPropCursor = NULL;
}

ICtrlShadowButtonImpl::~ICtrlShadowButtonImpl()
{
	SAFE_DELETE_OBJECT(m_hRgn);
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlShadowButtonImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

void ICtrlShadowButtonImpl::SetButtonText(LPCWSTR pszText)
{
	if(NULL == pszText)
		return;

	if (NULL == m_pPropText)
		m_pPropText = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "Text", "", true);

	if (NULL == m_pPropText)
		return;

	m_pPropText->SetString(pszText);
}

LPCWSTR ICtrlShadowButtonImpl::GetButtonText()
{
	if(NULL == m_pPropText)
		return NULL;

	return m_pPropText->GetString();
}

// 初始化控件
void ICtrlShadowButtonImpl::OnCreate()
{
	if (NULL != m_pPropButtonTypeCombo && m_pPropButtonTypeCombo->GetSelect() == BT_RGN_BUTTON)
		SetRgn();
}

// 控件初始化完毕
void ICtrlShadowButtonImpl::OnFinalCreate()
{
}

// 销毁控件
void ICtrlShadowButtonImpl::OnDestroy()
{
}

// 绘制控件
void ICtrlShadowButtonImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	if (IsEnable())
	{
		if (m_enumBtnState == BtnStateDisable)
			m_enumBtnState = BtnStateNormal;
	}
	else
	{
//		// Disable且已经重绘过的状态下不在重绘
//		if (m_enumBtnState == BtnStateDisable)
//			return;

		m_enumBtnState = BtnStateDisable;
	}

	BTNSTATE BtnState = m_enumBtnState;
	if (m_bIsCheck && (BtnState == BtnStateNormal || BtnState == BtnStateHover))
		BtnState = BtnStateDown;

	if (NULL != m_pPropImages[BtnState])
		m_pPropImages[BtnState]->DrawImage(DrawBoard, GetClientRect());

	if (NULL != m_pPropFonts[BtnState])
	{
		RECT ctrlRct = GetClientRect();
		if (m_enumBtnState == BtnStateDown)
		{
			int nOffset = GetTextDownOffset();
			ctrlRct.top += nOffset;
			ctrlRct.bottom += nOffset;
		}

		m_pPropFonts[BtnState]->DrawFontText(DrawBoard, m_pPropText->GetString(), ctrlRct);
	}
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlShadowButtonImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	m_pPropButtonTypeCombo = (IPropertyComboBox*)CreatePropetry(NULL, OTID_COMBOBOX, "ButtonType", "按钮类型");
	if(NULL != m_pPropButtonTypeCombo)
	{
		if (m_pWindowBase->IsDesignMode())
		{
			m_pPropButtonTypeCombo->AppendString(L"普通按钮");
			m_pPropButtonTypeCombo->AppendString(L"Rgn按钮");
			m_pPropButtonTypeCombo->AppendString(L"Check按钮");
			m_pPropButtonTypeCombo->AppendString(L"switch按钮");

			if (m_pPropButtonTypeCombo->GetSelect() < 0 || m_pPropButtonTypeCombo->GetSelect() >= m_pPropButtonTypeCombo->GetDataCounts())
				m_pPropButtonTypeCombo->SetSelect(0);
		}
	}

	//设置背景图片
	IPropertyGroup* pImagePropGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "ButtonImage", "Button的背景图片Group");
	m_pPropImages[BtnStateNormal] = (IPropertyImage*)CreatePropetry(pImagePropGroup, OTID_IMAGE, "NormalImage", "Normal状态下Button的背景图片");
	m_pPropImages[BtnStateHover] = (IPropertyImage*)CreatePropetry(pImagePropGroup, OTID_IMAGE, "HoverImage", "Hover状态下Button的背景图片");
	m_pPropImages[BtnStateDown] = (IPropertyImage*)CreatePropetry(pImagePropGroup, OTID_IMAGE, "LButtonDownImage", "按下鼠标左键状态下Button的背景图片");
	m_pPropImages[BtnStateFocus] = (IPropertyImage*)CreatePropetry(pImagePropGroup, OTID_IMAGE, "FocusImage", "得到焦点时Button的背景图片");
	m_pPropImages[BtnStateDisable] = (IPropertyImage*)CreatePropetry(pImagePropGroup, OTID_IMAGE, "DisableImage", "禁用状态下Button的背景图片");

	//设置字体
	IPropertyGroup* pfontPropGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "ButtonFont", "Button的字体Group");
	m_pPropFonts[BtnStateNormal] = (IPropertyFont*)CreatePropetry(pfontPropGroup, OTID_FONT, "NormalFont", "Normal状态下Button的字体");
	m_pPropFonts[BtnStateHover] = (IPropertyFont*)CreatePropetry(pfontPropGroup, OTID_FONT, "HoverFont", "Hover状态下Button的字体");
	m_pPropFonts[BtnStateDown] = (IPropertyFont*)CreatePropetry(pfontPropGroup, OTID_FONT, "LButtonDownFont", "按下鼠标左键状态下Button的字体");
	m_pPropFonts[BtnStateFocus] = (IPropertyFont*)CreatePropetry(pfontPropGroup, OTID_FONT, "FocusFont", "得到焦点时Button的字体");
	m_pPropFonts[BtnStateDisable] = (IPropertyFont*)CreatePropetry(pfontPropGroup, OTID_FONT, "DisableFont", "禁用状态下Button的字体");

	//设置Button上显示的文言
	m_pPropText = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "Text", "Button上显示的文字");

	m_pDownTextOffset = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "DownTextOffset", "鼠标按下是文字往下的偏移量");

	// 设置Rgn按钮属性
	m_pPropRgnGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "RgnButton","Rgn按钮");
	// Rgn区域会根据蒙版图片产生，当蒙版图片像素为纯黑色，RGB值为0时，此像素不显示
	m_pPropRgnMaskImage = (IPropertyImage*)CreatePropetry(m_pPropRgnGroup, OTID_IMAGE, "MaskImage", "Rgn蒙版图片,像素为纯黑色，RGB值为0时，此像素不显示");

	// Check Button
	m_pPropCheckBtnGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "CheckButton","CheckButton按钮");
	m_pPropShowPanelName = (IPropertyString*)CreatePropetry(m_pPropCheckBtnGroup, OTID_STRING, "ShowPanelName", "用于控制显示、隐藏的Panel的ObjectName");

	m_pPropCursor = (IPropertyCursor*)CreatePropetry(NULL, OTID_CURSOR, "CursorStyle", "按钮的鼠标手势");

	return true;
}

int ICtrlShadowButtonImpl::GetTextDownOffset()
{
	if (m_pDownTextOffset == NULL)
		return 0;

	return m_pDownTextOffset->GetValue();
}


// 鼠标左键点击
void ICtrlShadowButtonImpl::OnLButtonDown(POINT pt)
{
	m_mouseFlag |= MOUSE_FLAG_DOWN;
	ChangeButtonStyle(BtnStateDown);

	if (m_pWindowBase != NULL && this->IsEnable())
		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_BUTTON_DOWN, NULL, NULL);
}

// 鼠标左键双击
void ICtrlShadowButtonImpl::OnLButtonDbClick(POINT pt)
{
	OnLButtonDown(pt);
}

// 鼠标左键抬起
void ICtrlShadowButtonImpl::OnLButtonUp(POINT pt)
{
	RECT ctrlRct = this->GetClientRect();
	if (!::PtInRect(&ctrlRct, pt))
	{
		ChangeButtonStyle(BtnStateNormal);
		return;
	}

	if (m_pPropButtonTypeCombo != NULL && m_pPropButtonTypeCombo->GetSelect() == BT_SWITCH_BUTTON)
	{
		//	if (!IsCheck())
		{
			m_mouseFlag &= ~MOUSE_FLAG_DOWN;
			ChangeButtonStyle(IsMousehover() ? BtnStateHover : BtnStateNormal);
		}
	}
	else
	{
		m_mouseFlag &= ~MOUSE_FLAG_DOWN;
		ChangeButtonStyle(IsMousehover() ? BtnStateHover : BtnStateNormal);
	}

	if (m_pWindowBase != NULL && this->IsEnable())
		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_BUTTON_UP, NULL, NULL);	

	if (m_pPropButtonTypeCombo != NULL && m_pPropButtonTypeCombo->GetSelect() == BT_CHECK_BUTTON)
	{
		SetCheck(true, false);
	}
}

// 鼠标右键点击
void ICtrlShadowButtonImpl::OnRButtonDown(POINT pt)
{
	if (m_pWindowBase != NULL && this->IsEnable())
		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_BUTTON_R_BTN_DOWN, NULL, NULL);
}

// 鼠标移出
void ICtrlShadowButtonImpl::OnMouseLeave(POINT pt)
{
	ChangeButtonStyle(BtnStateNormal);

	if (m_pWindowBase != NULL && this->IsEnable())
		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_BUTTON_LEAVE, NULL, NULL);
}

void ICtrlShadowButtonImpl::OnMouseEnter(POINT pt)
{
	if (m_mouseFlag & MOUSE_FLAG_DOWN)
		ChangeButtonStyle(BtnStateDown);
	else
		ChangeButtonStyle(BtnStateHover);

	if (m_pWindowBase != NULL && this->IsEnable())
		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_BUTTON_ENTER, NULL, NULL);
}

void ICtrlShadowButtonImpl::ChangeButtonStyle(BTNSTATE state)
{
	if(state != m_enumBtnState)
	{
		m_enumBtnState = state;
		RedrawControl(true);
	}
}

// 控件取得焦点，通过Tab键跳转，激活控件
void ICtrlShadowButtonImpl::OnSetFocus()
{
	if(!IsMousehover())
		ChangeButtonStyle(BtnStateFocus);
}

// 控件失去焦点
void ICtrlShadowButtonImpl::OnKillFocus()
{
	if(!IsMousehover())
		ChangeButtonStyle(BtnStateNormal);
}

// 鼠标是否落在了控件指定的RGN区域，默认返回true，整个控件就是RGN区域
bool ICtrlShadowButtonImpl::OnCheckMouseInRgn(POINT pt, int nMouseMsgId)
{
	if (NULL != m_pPropButtonTypeCombo && m_pPropButtonTypeCombo->GetSelect() == BT_RGN_BUTTON)
		return (::PtInRegion(m_hRgn, pt.x, pt.y) == TRUE);

	return true;
}

// 根据根据图片设置RGN
bool ICtrlShadowButtonImpl::SetRgn()
{
	if (m_pPropRgnMaskImage == NULL)
		return false;

	HRGN rgnTemp = ::CreateRectRgn(0, 0, 0, 0);
	SIZE szMask = {0, 0};
	m_pPropRgnMaskImage->InitImage();

	if (NULL == m_pPropRgnMaskImage->GetImageBaseProp() || NULL == m_pPropRgnMaskImage->GetImageBaseProp()->GetDrawingImage())
		return false;

	BYTE* pImageData = m_pPropRgnMaskImage->GetImageBaseProp()->GetDrawingImage()->GetBits();
	if (NULL == pImageData)
		return false;

	SAFE_DELETE_OBJECT(m_hRgn);
	m_hRgn = ::CreateRectRgn(0, 0, 0, 0);

	szMask = m_pPropRgnMaskImage->GetImageSize();
	for (int nHeight = szMask.cy - 1; nHeight >= 0; nHeight--)
	{
		for (int nWidth = 0; nWidth < szMask.cx; nWidth++)
		{
			unsigned int nPos = (nHeight * szMask.cx + nWidth) * 4;
			unsigned int nColor = pImageData[nPos] + pImageData[nPos + 1] + pImageData[nPos + 2];

			if (0 < nColor)	// 非黑色区域
			{
				rgnTemp = ::CreateRectRgn(nWidth, (szMask.cy - nHeight), nWidth+1, (szMask.cy - nHeight) + 1);
				::CombineRgn(m_hRgn, m_hRgn, rgnTemp, RGN_OR);
			}
		}
	}
	SAFE_DELETE_OBJECT(rgnTemp);
	return true;
}

// check button 模式下
void ICtrlShadowButtonImpl::SetCheck(bool bIsCheck, bool bRedraw)
{
	if (m_pPropButtonTypeCombo == NULL || !(m_pPropButtonTypeCombo->GetSelect() == BT_CHECK_BUTTON || m_pPropButtonTypeCombo->GetSelect() == BT_SWITCH_BUTTON))
		return;

	m_bIsCheck = bIsCheck;

	if (GetCheckPanel() != NULL)
	{
		m_pCheckPanel->SetVisible(m_bIsCheck);
		m_pCheckPanel->RedrawControl(bRedraw);
	}

	this->RedrawControl(bRedraw);
}

bool ICtrlShadowButtonImpl::IsCheck()
{
	if (m_pPropButtonTypeCombo == NULL || !(m_pPropButtonTypeCombo->GetSelect() == BT_CHECK_BUTTON || m_pPropButtonTypeCombo->GetSelect() == BT_SWITCH_BUTTON))
		return false;

	return m_bIsCheck;
}

// 这个接口会在整个对话框初始化完成的时候调用
// 控件通过派生这个接口，可以在这个接口函数里通过其他控件的名称取得当前对话框里的其他控件。
void ICtrlShadowButtonImpl::OnWindowFinalCreate()
{
	OnBuilderRefreshProp(NULL);

	if (m_pWindowBase != NULL && !m_pWindowBase->IsDesignMode() && GetCheckPanel() != NULL)
	{
		m_pCheckPanel->SetVisible(m_bIsCheck);
		m_pCheckPanel->RedrawControl(false);
	}
}

IControlBase* ICtrlShadowButtonImpl::GetCheckPanel()
{
	if (m_pPropButtonTypeCombo == NULL || !(m_pPropButtonTypeCombo->GetSelect() == BT_CHECK_BUTTON || m_pPropButtonTypeCombo->GetSelect() == BT_SWITCH_BUTTON))
	{
		if (m_pCheckPanel == NULL && m_pWindowBase != NULL && m_pPropShowPanelName != NULL && m_pPropShowPanelName->GetLength() > 0)
			m_pCheckPanel = m_pWindowBase->GetControl(m_pPropShowPanelName->GetString());
	}
	else
	{
		m_pCheckPanel = NULL;
	}

	return m_pCheckPanel;
}

// Builder刷新属性，此函数中只要重新加载控件属性即可
void ICtrlShadowButtonImpl::OnBuilderRefreshProp(IPropertyBase* pPropBase)
{
	m_pCheckPanel = NULL;
	GetCheckPanel();
}


// 设置控件光标
IPropertyCursor* ICtrlShadowButtonImpl::OnSetCursor(POINT pt)
{
	if (!IsEnable())
		return NULL;

	return m_pPropCursor;
}