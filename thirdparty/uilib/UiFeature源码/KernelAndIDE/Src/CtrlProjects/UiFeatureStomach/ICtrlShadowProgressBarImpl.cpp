
#include "StdAfx.h"
#include "ICtrlShadowProgressBarImpl.h"
#include "..\..\Inc\IControlManager.h"
#include "..\..\Inc\UiFeatureDefs.h"

ICtrlShadowProgressBar::ICtrlShadowProgressBar(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlShadowProgressBarImpl::ICtrlShadowProgressBarImpl(IUiFeatureKernel *pUiKernel) : ICtrlShadowProgressBar(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_SHADOW_PROGRESS_BAR);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;

	m_bIsVisible = true;
	INIT_RECT(m_CtrlRectInOwner);

	m_nCurPos = 0;

	m_pPropProgressType = NULL;
	m_pPropBkImg = NULL;
	m_pPropMaskImg = NULL;
	m_pPropProgressImg = NULL;
	m_pPropFont = NULL;

	m_pPropSpaceGroup = NULL;
	m_pPropLeftSpace = NULL;
	m_pPropRightSpace = NULL;
	m_pPropTopSpace = NULL;
	m_pPropBottomSpace = NULL;
}

ICtrlShadowProgressBarImpl::~ICtrlShadowProgressBarImpl()
{
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlShadowProgressBarImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT ICtrlShadowProgressBarImpl::OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

bool ICtrlShadowProgressBarImpl::CreateControlPropetry( bool bIsNewCtrl)
{
	if (bIsNewCtrl)
	{
		// 设置默认不接受鼠标信息
		m_pPropBase_RcvMouseMsg->SetValue(false);
	}

	IPropertyGroup* pGroup = NULL;
	pGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "ProgressBar", "进度条");

	m_pPropProgressType = (IPropertyComboBox*)CreatePropetry(pGroup, OTID_COMBOBOX, "ProgressBarType", "进度条类型：水平进度条、垂直进度条");
	if(NULL != m_pPropProgressType)
	{
		if (m_pWindowBase->IsDesignMode())
		{
			m_pPropProgressType->AppendString(L"水平进度条");
			m_pPropProgressType->AppendString(L"垂直进度条");

			if (m_pPropProgressType->GetSelect() < 0 || m_pPropProgressType->GetSelect() >= m_pPropProgressType->GetDataCounts())
				m_pPropProgressType->SetSelect(0);
		}
	}

	m_pPropFont = (IPropertyFont*)CreatePropetry(pGroup, OTID_FONT, "TextFont", "进度条中间文字的字体");

	m_pPropBkImg = (IPropertyImage*)CreatePropetry(pGroup, OTID_IMAGE, "BackgroundImage", "进度条背景图片");
	m_pPropMaskImg = (IPropertyImage*)CreatePropetry(pGroup, OTID_IMAGE, "MaskImage", "盖在进度条上的蒙罩图片");
	m_pPropProgressImg = (IPropertyImage*)CreatePropetry(pGroup, OTID_IMAGE, "ProgressImage", "进度条走进度的图片");

	m_pPropSpaceGroup = (IPropertyGroup*)CreatePropetry(pGroup, OTID_GROUP, "ProgressRunSpace", "进度条开始走进度的边距");
	m_pPropLeftSpace = (IPropertyInt*)CreatePropetry(m_pPropSpaceGroup, OTID_INT, "LeftSpace", "进度条开始走进度距离控件左侧的偏移");
	m_pPropRightSpace = (IPropertyInt*)CreatePropetry(m_pPropSpaceGroup, OTID_INT, "RightSpace", "进度条开始走进度距离控件右侧的偏移");
	m_pPropTopSpace = (IPropertyInt*)CreatePropetry(m_pPropSpaceGroup, OTID_INT, "TopSpace", "进度条开始走进度距离控件上方的偏移");
	m_pPropBottomSpace = (IPropertyInt*)CreatePropetry(m_pPropSpaceGroup, OTID_INT, "BottomSpace", "进度条开始走进度距离控件下方的偏移");

	return true;
}

void ICtrlShadowProgressBarImpl::OnCreate()
{
}

void ICtrlShadowProgressBarImpl::OnFinalCreate()
{

}

void ICtrlShadowProgressBarImpl::OnDestroy()
{
	
}

void ICtrlShadowProgressBarImpl::SetVisible(bool bVisible)
{
	m_bIsVisible = bVisible;
}

bool ICtrlShadowProgressBarImpl::IsVisible()
{
	return m_bIsVisible;
}

void ICtrlShadowProgressBarImpl::SetRectInOwner(RECT ctrlRectInOwner)
{
	m_CtrlRectInOwner = ctrlRectInOwner;
}

void ICtrlShadowProgressBarImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	if (m_pWindowBase == NULL)
		return;

	if (m_pWindowBase->IsDesignMode())
	{
		// 用于在编辑模式下的显示
		m_nCurPos = 70;
	}

	// 1. 绘制背景
	if (m_pPropBkImg != NULL)
		m_pPropBkImg->DrawImage(DrawBoard, m_CtrlRectInOwner);

	RECT TexgRect = m_CtrlRectInOwner;
	// 2. 绘制中间进度部分
	if (m_pPropProgressType != NULL && m_pPropProgressImg != NULL && m_pPropLeftSpace != NULL
		&& m_pPropRightSpace != NULL  && m_pPropTopSpace != NULL  && m_pPropBottomSpace != NULL)
	{
		RECT ProgressRect = m_CtrlRectInOwner;
		ProgressRect.left += m_pPropLeftSpace->GetValue();
		ProgressRect.right -= m_pPropRightSpace->GetValue();
		ProgressRect.top += m_pPropTopSpace->GetValue();
		ProgressRect.bottom -= m_pPropBottomSpace->GetValue();

		TexgRect = ProgressRect;

		if (PBT_H == (PROGRESS_BAR_TYPE)m_pPropProgressType->GetSelect())
		{
			// 水平
			double dbRight = ((double)(RECT_WIDTH(ProgressRect))) * (((double)m_nCurPos) / ((double)100.0));
			ProgressRect.right = ProgressRect.left + (int)dbRight;
		}
		else
		{
			// 垂直
			double dbBottom = ((double)(RECT_HEIGHT(ProgressRect))) * (((double)m_nCurPos) / ((double)100.0));
			ProgressRect.bottom = ProgressRect.top + (int)dbBottom;
		}

		if (ProgressRect.right < ProgressRect.left)
			ProgressRect.right = ProgressRect.left;

		if (ProgressRect.right > ProgressRect.left + RECT_WIDTH(ProgressRect))
			ProgressRect.right = ProgressRect.left + RECT_WIDTH(ProgressRect);

		if (ProgressRect.bottom < ProgressRect.top)
			ProgressRect.bottom = ProgressRect.top;

		if (ProgressRect.bottom > ProgressRect.top + RECT_HEIGHT(ProgressRect))
			ProgressRect.bottom = ProgressRect.top + RECT_HEIGHT(ProgressRect);

		m_pPropProgressImg->DrawImage(DrawBoard, ProgressRect);
	}

	// 3. 绘制蒙罩
	if (m_pPropMaskImg != NULL)
		m_pPropMaskImg->DrawImage(DrawBoard, m_CtrlRectInOwner);

	//// 4. 绘制文字
	//if (m_pPropFont != NULL)
	//{
	//	char szText[32];
	//	memset(szText, 0, 32);
	//	sprintf_s(szText, 31, "%d%%", m_nCurPos);
	//	m_pPropFont->DrawFontText(DrawBoard, szText, TexgRect);
	//}
}

void ICtrlShadowProgressBarImpl::SetProgressBarPos(int nPos, bool bRedraw)
{
	m_nCurPos = nPos;

	if (m_nCurPos > 100)
		m_nCurPos = 100;

	if (m_nCurPos < 0)
		m_nCurPos = 0;

	RedrawControl(bRedraw);
}

int ICtrlShadowProgressBarImpl::GetProgressBarPos()
{
	return m_nCurPos;
}

bool ICtrlShadowProgressBarImpl::OnDrawAnimation()
{
	if (!IsVisible())
		return false;

	return false;
}

void ICtrlShadowProgressBarImpl::OnTimer(UINT nTimerId)
{
	if (!IsVisible())
		return;
}

void ICtrlShadowProgressBarImpl::OnSize()
{
	m_CtrlRectInOwner = GetClientRect();
}

bool ICtrlShadowProgressBarImpl::MirrorDraw(CDrawingBoard &DrawBoard, RECT DstRect, int nPos)
{
	if (DrawBoard.GetSafeHdc() == NULL || IS_RECT_EMPTY(DstRect) || nPos < 0 || nPos > 100)
	return false;

	m_CtrlRectInOwner = DstRect;
	m_nCurPos = nPos;
	OnPaint(DrawBoard);

	return true;
}
