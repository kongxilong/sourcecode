#include "StdAfx.h"
#include <assert.h>
#include "..\..\Inc\IControlManager.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\ICommonFun.h"

#include "ICtrlScrollBarImpl.h"
#include "IScrollBarPainterImpl.h"

// 属性
#define 		SCROLLBAR_NORMAL_IMAGE						"Normal"
#define 		SCROLLBAR_MOUSEHOVER_IMAGE					"MouseHover"
#define 		SCROLLBAR_LBUTTONDOWN_IMAGE					"LButtonDown"
#define 		SCROLLBAR_PRESUDO_MOUSEHOVER_IMAGE			"PresudoMouseHover"

ICtrlScrollBar::ICtrlScrollBar(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlScrollBarImpl::ICtrlScrollBarImpl(IUiFeatureKernel *pUiKernel) : ICtrlScrollBar(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_SCROLLBAR);

	for(int i = 0;i < SBStateNum; i++)
	{
		m_pPropVSBBGImages[i] = NULL;
		m_pPropVSBPageUpImages[i] = NULL;
		m_pPropVSBPageDownImages[i] = NULL;
		m_pPropVSBLineUpImages[i] = NULL;
		m_pPropVSBLineDownImages[i] = NULL;
		m_pPropVSBGotoBottomImages[i] = NULL;
		m_pPropVSBGotoTopImages[i] = NULL;
		m_pPropVSBThumbImages[i] = NULL;
		m_pPropHSBBGImages[i] = NULL;
		m_pPropHSBPageLeftImages[i] = NULL;
		m_pPropHSBPageRightImages[i] = NULL;
		m_pPropHSBLineLeftImages[i] = NULL;
		m_pPropHSBLineRightImages[i] = NULL;
		m_pPropHSBGotoRightImages[i] = NULL;
		m_pPropHSBGotoLeftImages[i] = NULL;
		m_pPropHSBThumbImages[i] = NULL;
	}

	m_pPainter = NULL;
	m_PropType = NULL;
	m_PropOwnerCtrlName = NULL;
	m_pOwnerCtrl = NULL;
	m_pPainter = NULL;
}

ICtrlScrollBarImpl::~ICtrlScrollBarImpl(void)
{
	if(NULL != m_pPainter)
	{
		delete m_pPainter;
		m_pPainter = NULL;
	}
}

bool ICtrlScrollBarImpl::EnableScrollBar(UINT nArrowFlags)
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		return m_pPainter->EnableScrollBar(nArrowFlags);
	}
	else
	{
		return false;
	}
}

bool ICtrlScrollBarImpl::GetScrollBarInfo(PSCROLLBARINFO pScrollInfo) const
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		return m_pPainter->GetScrollBarInfo(pScrollInfo);
	}
	else
	{
		return false;
	}
}

bool ICtrlScrollBarImpl::GetScrollInfo(LPSCROLLINFO lpScrollInfo,UINT nMask)
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		return m_pPainter->GetScrollInfo(lpScrollInfo,nMask);
	}
	else
	{
		return false;
	}
}

int ICtrlScrollBarImpl::GetScrollLimit( )
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		return m_pPainter->GetScrollLimit();
	}
	else
	{
		return 0;
	}
}

int ICtrlScrollBarImpl::GetScrollPos( ) const
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		return m_pPainter->GetScrollPos();
	}
	else
	{
		return 0;
	}
}

void ICtrlScrollBarImpl::GetScrollRange(LPINT lpMinPos,LPINT lpMaxPos) const
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		m_pPainter->GetScrollRange(lpMinPos,lpMaxPos);
	}
}

bool ICtrlScrollBarImpl::SetScrollInfo(LPSCROLLINFO lpScrollInfo,bool bRedraw)
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		return m_pPainter->SetScrollInfo(lpScrollInfo,bRedraw);
	}
	else
	{
		return false;
	}
}

int ICtrlScrollBarImpl::SetScrollPos(int nPos,bool bRedraw)
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		return m_pPainter->SetScrollPos(nPos,bRedraw);
	}
	else
	{
		return 0;
	}
}

void ICtrlScrollBarImpl::SetScrollRange(int nMinPos,int nMaxPos,bool bRedraw)
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		return m_pPainter->SetScrollRange(nMinPos,nMaxPos,bRedraw);
	}
}

void ICtrlScrollBarImpl::ShowScrollBar(bool bShow)
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		return m_pPainter->ShowScrollBar(bShow);
	}
}

void ICtrlScrollBarImpl::OnVScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar)
{
	if(NULL != m_pOwnerCtrl)
	{
		m_pOwnerCtrl->OnVScroll(nSBCode,nPos,this);
	}
}

void ICtrlScrollBarImpl::OnHScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar)
{
	if(NULL != m_pOwnerCtrl)
	{
		m_pOwnerCtrl->OnHScroll(nSBCode,nPos,this);
	}
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlScrollBarImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	//垂直滚动条图片属性组                                                 
	IPropertyGroup* pVSBImagePropGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "VScrollBarImageGroup", "垂直ScrollBar的图片组");
	
	// 设置垂直滚动条背景图片
	m_pPropVSBBGImages[SBStateNormal] = (IPropertyImage*)CreatePropetry(pVSBImagePropGroup, OTID_IMAGE, "Background", "背景图片");
	m_pPropVSBBGImages[SBStatePresudoHover] = m_pPropVSBBGImages[SBStateDown] = m_pPropVSBBGImages[SBStateHover] = m_pPropVSBBGImages[SBStateNormal];
	
	// 设置垂直滚动条Thumb图片
	IPropertyGroup* pVSBThumbImagePropGroup = (IPropertyGroup*)CreatePropetry(pVSBImagePropGroup, OTID_GROUP, "ThumbImageGroup", "Thumb图片组");
	m_pPropVSBThumbImages[SBStateNormal] = (IPropertyImage*)CreatePropetry(pVSBThumbImagePropGroup, OTID_IMAGE, SCROLLBAR_NORMAL_IMAGE, "Normal状态时的Thumb图片");
	m_pPropVSBThumbImages[SBStatePresudoHover] = (IPropertyImage*)CreatePropetry(pVSBThumbImagePropGroup, OTID_IMAGE, SCROLLBAR_PRESUDO_MOUSEHOVER_IMAGE, "疑是MouseHover状态时的Thumb图片");
	m_pPropVSBThumbImages[SBStateHover] = (IPropertyImage*)CreatePropetry(pVSBThumbImagePropGroup, OTID_IMAGE, SCROLLBAR_MOUSEHOVER_IMAGE, "Hover状态时的Thumb图片");
	m_pPropVSBThumbImages[SBStateDown] = (IPropertyImage*)CreatePropetry(pVSBThumbImagePropGroup, OTID_IMAGE, SCROLLBAR_LBUTTONDOWN_IMAGE, "LButtonDown状态时的Thumb图片");

	// 设置垂直滚动条LineUp图片
	IPropertyGroup* pVSBLineUpImagePropGroup = (IPropertyGroup*)CreatePropetry(pVSBImagePropGroup, OTID_GROUP, "LineUpImageGroup", "LineUp图片组");
	m_pPropVSBLineUpImages[SBStateNormal] = (IPropertyImage*)CreatePropetry(pVSBLineUpImagePropGroup, OTID_IMAGE, SCROLLBAR_NORMAL_IMAGE, "Normal状态时的LineUp图片");
	m_pPropVSBLineUpImages[SBStatePresudoHover] = (IPropertyImage*)CreatePropetry(pVSBLineUpImagePropGroup, OTID_IMAGE, SCROLLBAR_PRESUDO_MOUSEHOVER_IMAGE, "疑是MouseHover状态时的LineUp图片");
	m_pPropVSBLineUpImages[SBStateHover] = (IPropertyImage*)CreatePropetry(pVSBLineUpImagePropGroup, OTID_IMAGE, SCROLLBAR_MOUSEHOVER_IMAGE, "MouseHover状态时的LineUp图片");
	m_pPropVSBLineUpImages[SBStateDown] = (IPropertyImage*)CreatePropetry(pVSBLineUpImagePropGroup, OTID_IMAGE, SCROLLBAR_LBUTTONDOWN_IMAGE, "LButtonDown状态时的LineUp图片");

	// 设置垂直滚动条LineDown图片
	IPropertyGroup* pVSBLineDwonImagePropGroup = (IPropertyGroup*)CreatePropetry(pVSBImagePropGroup, OTID_GROUP, "LineDownImageGroup", "LineDown图片组");
	m_pPropVSBLineDownImages[SBStateNormal] = (IPropertyImage*)CreatePropetry(pVSBLineDwonImagePropGroup, OTID_IMAGE, SCROLLBAR_NORMAL_IMAGE, "Normal状态时的LineDown图片");
	m_pPropVSBLineDownImages[SBStatePresudoHover] = (IPropertyImage*)CreatePropetry(pVSBLineDwonImagePropGroup, OTID_IMAGE, SCROLLBAR_PRESUDO_MOUSEHOVER_IMAGE, "疑是MouseHover状态时的LineDown图片");
	m_pPropVSBLineDownImages[SBStateHover] = (IPropertyImage*)CreatePropetry(pVSBLineDwonImagePropGroup, OTID_IMAGE, SCROLLBAR_MOUSEHOVER_IMAGE, "MouseHover状态时的LineDown图片");
	m_pPropVSBLineDownImages[SBStateDown] = (IPropertyImage*)CreatePropetry(pVSBLineDwonImagePropGroup, OTID_IMAGE, SCROLLBAR_LBUTTONDOWN_IMAGE, "LButtonDown状态时的LineDown图片");
		
	// 设置垂直滚动条PageUp区域的图片
	m_pPropVSBPageUpImages[SBStateNormal] = m_pPropVSBPageUpImages[SBStatePresudoHover] = m_pPropVSBPageUpImages[SBStateHover] = NULL;
	m_pPropVSBPageUpImages[SBStateDown] = (IPropertyImage*)CreatePropetry(pVSBImagePropGroup, OTID_IMAGE, "PageUp", "PageUp图片");

	// 设置垂直滚动条PageDown区域的图片
	m_pPropVSBPageDownImages[SBStateNormal] = m_pPropVSBPageDownImages[SBStatePresudoHover] = m_pPropVSBPageDownImages[SBStateHover] = NULL;
	m_pPropVSBPageDownImages[SBStateDown] = (IPropertyImage*)CreatePropetry(pVSBImagePropGroup, OTID_IMAGE, "PageDown", "PageDown图片");

	// 水平滚动条图片属性组                                     
	IPropertyGroup* pHSBImagePropGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "HScrollBarImageGroup", "水平ScrollBar的图片组");

	// 设置水平滚动条背景图片
	m_pPropHSBBGImages[SBStateNormal] = (IPropertyImage*)CreatePropetry(pHSBImagePropGroup, OTID_IMAGE, "Background", "背景图片");
	m_pPropHSBBGImages[SBStatePresudoHover] = m_pPropHSBBGImages[SBStatePresudoHover] = m_pPropHSBBGImages[SBStateDown] = m_pPropHSBBGImages[SBStateNormal];
	
	// 设置水平滚动条Thumb图片
	IPropertyGroup* pHSBThumbImagePropGroup = (IPropertyGroup*)CreatePropetry(pHSBImagePropGroup, OTID_GROUP, "ThumbImageGroup", "Thumb图片组");
	m_pPropHSBThumbImages[SBStateNormal] = (IPropertyImage*)CreatePropetry(pHSBThumbImagePropGroup, OTID_IMAGE, SCROLLBAR_NORMAL_IMAGE, "Normal状态时的Thumb图片");
	m_pPropHSBThumbImages[SBStatePresudoHover] = (IPropertyImage*)CreatePropetry(pHSBThumbImagePropGroup, OTID_IMAGE, SCROLLBAR_PRESUDO_MOUSEHOVER_IMAGE, "疑是MouseHover状态时的Thumb图片");
	m_pPropHSBThumbImages[SBStateHover] = (IPropertyImage*)CreatePropetry(pHSBThumbImagePropGroup, OTID_IMAGE, SCROLLBAR_MOUSEHOVER_IMAGE, "MouseHover状态时的Thumb图片");
	m_pPropHSBThumbImages[SBStateDown] = (IPropertyImage*)CreatePropetry(pHSBThumbImagePropGroup, OTID_IMAGE, SCROLLBAR_LBUTTONDOWN_IMAGE, "LButtonDown状态时的Thumb图片");

	// 设置水平滚动条LineLeft图片
	IPropertyGroup* pHSBLineLeftImagePropGroup = (IPropertyGroup*)CreatePropetry(pHSBImagePropGroup, OTID_GROUP, "LineLeftImageGroup", "LineLeft图片组");
	m_pPropHSBLineLeftImages[SBStateNormal] = (IPropertyImage*)CreatePropetry(pHSBLineLeftImagePropGroup, OTID_IMAGE, SCROLLBAR_NORMAL_IMAGE, "Normal状态时的LineLeft图片");
	m_pPropHSBLineLeftImages[SBStatePresudoHover] = (IPropertyImage*)CreatePropetry(pHSBLineLeftImagePropGroup, OTID_IMAGE, SCROLLBAR_PRESUDO_MOUSEHOVER_IMAGE, "疑是MouseHover状态时的LineLeft图片");
	m_pPropHSBLineLeftImages[SBStateHover] = (IPropertyImage*)CreatePropetry(pHSBLineLeftImagePropGroup, OTID_IMAGE, SCROLLBAR_MOUSEHOVER_IMAGE, "MouseHover状态时的LineLeft图片");
	m_pPropHSBLineLeftImages[SBStateDown] = (IPropertyImage*)CreatePropetry(pHSBLineLeftImagePropGroup, OTID_IMAGE, SCROLLBAR_LBUTTONDOWN_IMAGE, "LButtonDown状态时的LineLeft图片");
	
	// 设置水平滚动条LineRight图片
	IPropertyGroup* pHSBLineRightImagePropGroup = (IPropertyGroup*)CreatePropetry(pHSBImagePropGroup, OTID_GROUP, "LineRightImageGroup", "LineRight图片组");
	m_pPropHSBLineRightImages[SBStateNormal] = (IPropertyImage*)CreatePropetry(pHSBLineRightImagePropGroup, OTID_IMAGE, SCROLLBAR_NORMAL_IMAGE, "Normal状态时的LineRight图片");
	m_pPropHSBLineRightImages[SBStatePresudoHover] = (IPropertyImage*)CreatePropetry(pHSBLineRightImagePropGroup, OTID_IMAGE, SCROLLBAR_PRESUDO_MOUSEHOVER_IMAGE, "疑是MouseHover状态时的LineRight图片");
	m_pPropHSBLineRightImages[SBStateHover] = (IPropertyImage*)CreatePropetry(pHSBLineRightImagePropGroup, OTID_IMAGE, SCROLLBAR_MOUSEHOVER_IMAGE, "MouseHover状态时的LineRight图片");
	m_pPropHSBLineRightImages[SBStateDown] = (IPropertyImage*)CreatePropetry(pHSBLineRightImagePropGroup, OTID_IMAGE, SCROLLBAR_LBUTTONDOWN_IMAGE, "LButtonDown状态时的LineRight图片");
		
	// 设置水平滚动条PageLeft图片
	m_pPropHSBPageLeftImages[SBStateNormal] = m_pPropHSBPageLeftImages[SBStatePresudoHover] = m_pPropHSBPageLeftImages[SBStateNormal] = m_pPropHSBPageLeftImages[SBStateHover] = NULL;
	m_pPropHSBPageLeftImages[SBStateDown] = (IPropertyImage*)CreatePropetry(pHSBImagePropGroup, OTID_IMAGE, "PageLeft", "PageLeft图片");

	// 设置水平滚动条PageRight图片
	m_pPropHSBPageRightImages[SBStateNormal] = m_pPropHSBPageRightImages[SBStatePresudoHover] = m_pPropHSBPageRightImages[SBStateHover] = NULL;
	m_pPropHSBPageRightImages[SBStateDown] = (IPropertyImage*)CreatePropetry(pHSBImagePropGroup, OTID_IMAGE, "PageRight", "PageRight图片");

	// 类型：垂直、水平   
	m_PropType = (IPropertyComboBox*)CreatePropetry(NULL, OTID_COMBOBOX, "Type", "ScrollBar的类型");
	if(NULL !=m_PropType)
	{
		if (m_pWindowBase->IsDesignMode())
		{
			m_PropType->AppendString(L"Vertical");
			m_PropType->AppendString(L"Horizontal");

			if (m_PropType->GetSelect() < 0 || m_PropType->GetSelect() >= m_PropType->GetDataCounts())
				m_PropType->SetSelect(0);
		}
	}


	// 滚动条使用者的Name     
	m_PropOwnerCtrlName = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "OwnerCtrlName", "ScrollBar的使用者");
	
	return true;
}

// 初始化控件
void ICtrlScrollBarImpl::OnCreate()
{
	PrepareScrollBarOwner();
	PrepareScrollBarPainter();
}

// 控件初始化完毕
void ICtrlScrollBarImpl::OnFinalCreate()
{

}

// 销毁控件
void ICtrlScrollBarImpl::OnDestroy()
{
	if(NULL != m_pPainter)
	{
		delete m_pPainter;
		m_pPainter = NULL;
	}
}

// 绘制控件
void ICtrlScrollBarImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		m_pPainter->OnPaint(DrawBoard);
	}
}

// 鼠标进入
void ICtrlScrollBarImpl::OnMouseEnter(POINT pt)
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		m_pPainter->OnMouseEnter(pt);
	}
}

// 鼠标移出
void ICtrlScrollBarImpl::OnMouseLeave(POINT pt)
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		m_pPainter->OnMouseLeave(pt);
	}
}
// 鼠标移动
void ICtrlScrollBarImpl::OnMouseMove(POINT pt)
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		m_pPainter->OnMouseMove(pt);
	}
}

// 鼠标左键点击
void ICtrlScrollBarImpl::OnLButtonDown(POINT pt)
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		m_pPainter->OnLButtonDown(pt);
	}
}

// 鼠标滚轮消息
void ICtrlScrollBarImpl::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	assert(NULL != m_pOwnerCtrl);

	if(NULL != m_pOwnerCtrl)
	{
		m_pOwnerCtrl->OnMouseWheel(wParam,lParam);
	}
}

// 鼠标左键抬起
void ICtrlScrollBarImpl::OnLButtonUp(POINT pt)
{
	if(NULL != m_pPainter)
	{
		m_pPainter->OnLButtonUp(pt);
	}
}

void ICtrlScrollBarImpl::OnLButtonDbClick(POINT pt)
{
	if(NULL != m_pPainter)
	{
		m_pPainter->OnLButtonDbClick(pt);
	}
}

// 控件取得焦点，通过Tab键跳转，激活控件
void ICtrlScrollBarImpl::OnSetFocus()
{

}

// 控件失去焦点
void ICtrlScrollBarImpl::OnKillFocus()
{
	this->RedrawControl(true);
}

// 移动、设置控件位置
void ICtrlScrollBarImpl::OnSize()
{
	if(NULL != m_pPainter)
	{
		m_pPainter->OnSize();
	}
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlScrollBarImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 鼠标拖动控件
void ICtrlScrollBarImpl::OnMouseDragging(POINT pt)
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		m_pPainter->OnMouseDragging(pt);
	}
}

// 绘制控件的动画，如果需要重新刷新界面，返回true，否则返回false
// 此函数由定时器触发，每100毫秒触发一次
bool ICtrlScrollBarImpl::OnDrawAnimation()
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		return m_pPainter->OnDrawAnimation();
	}
	else
	{
		return false;
	}
}

// Builder刷新属性，此函数中只要重新加载控件属性即可
void ICtrlScrollBarImpl::OnBuilderRefreshProp(IPropertyBase* pPropBase)
{
	PrepareScrollBarOwner();
	PrepareScrollBarPainter();
}

void ICtrlScrollBarImpl::OnTimer(UINT nTimerId)
{
	assert(NULL != m_pPainter);

	if(NULL != m_pPainter)
	{
		return m_pPainter->OnTimer(nTimerId);
	}
}
void ICtrlScrollBarImpl::PrepareScrollBarPainter()
{
	if(NULL == m_pPainter)
	{
		m_pPainter =  new IScrollBarPainterImpl(this);
	}
	else
	{
		if(typeid(IScrollBarPainterImpl) != typeid(*m_pPainter))
		{
			delete m_pPainter;
			m_pPainter = new IScrollBarPainterImpl(this);
		}
	}

	if(NULL != m_pPainter)
	{
		m_pPainter->OnSize();
	}

}

void ICtrlScrollBarImpl::PrepareScrollBarOwner()
{	
	assert(NULL !=m_pWindowBase);
	if(NULL == m_pWindowBase)
		return;

	m_pOwnerCtrl = m_pWindowBase->GetControl(m_PropOwnerCtrlName->GetString());
}
