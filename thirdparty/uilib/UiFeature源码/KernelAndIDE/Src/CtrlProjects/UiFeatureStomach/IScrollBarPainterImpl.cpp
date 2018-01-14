#include "StdAfx.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\ICommonFun.h"

#include "IScrollBarPainterImpl.h"
#include "ICtrlScrollBarImpl.h"

#define 	MOUSE_FLAG_DOWN						0x0000001UL		//鼠标按下标志
#define 	MOUSE_FLAG_DRAG						0x0000002UL		//鼠标滑块标志
#define 	MOUSE_FLAG_ENTER_PAGE_INC			0x0000010UL		//进入Page增加标志
#define 	MOUSE_FLAG_ENTER_PAGE_DEC			0x0000020UL		//进入Page减少标志
#define 	MOUSE_FLAG_ENTER_THUMB				0x0000080UL		//进入滑块标志
#define 	TIMER_ELAPSE						150
#define 	PADDING								1

IScrollBarPainterImpl::IScrollBarPainterImpl(ICtrlScrollBarImpl* pScrollBar) : IScrollBarPainter(pScrollBar)
{	
	m_nTimerId = 0;
	m_nDistant = 0;
	m_nVThumbHeight = 0;
	m_nVThumbWidth = 0;
	m_nHThumbHeight = 0;
	m_nHThumbWidth = 0;

	INIT_RECT(m_rctPageLeft);
	INIT_RECT(m_rctPageRight);
	INIT_RECT(m_rctVThumb);
	INIT_RECT(m_rctPageUp);
	INIT_RECT(m_rctPageDown);
	INIT_RECT(m_rctHThumb);
	
}

IScrollBarPainterImpl::~IScrollBarPainterImpl(void)
{
	m_pScrollBar = NULL;
}

// 绘制控件
void IScrollBarPainterImpl::OnPaint(CDrawingBoard &DstDc)
{
	int nSelect = 0;

	assert(NULL != m_pScrollBar && NULL != m_pScrollBar->m_PropType);

	if(NULL == m_pScrollBar || NULL == m_pScrollBar->m_PropType)
		return;

	if(!m_bIsShow) 
		return;

	nSelect = m_pScrollBar->m_PropType->GetSelect();
	switch (nSelect)
	{
	case SBTypeHorizontal:
		DrawHorizontalScrollBar(DstDc);
		break;
	case SBTypeVertical:
	default:
		DrawVerticalScrollBar(DstDc);
		break;
	}
}

// 鼠标进入
void IScrollBarPainterImpl::OnMouseEnter(POINT pt)
{
	if(m_uMouseFlag & MOUSE_FLAG_DOWN)
	{
		ChangeScrollBarStyle(SBStateDown);
	}
	else
	{
		ChangeScrollBarStyle(SBStatePresudoHover);
	}
}

// 鼠标移出
void IScrollBarPainterImpl::OnMouseLeave(POINT pt)
{
	ChangeScrollBarStyle(SBStateNormal);
}

// 鼠标移动
void IScrollBarPainterImpl:: OnMouseMove(POINT pt)
{
	if(NULL == m_pScrollBar || NULL == m_pScrollBar->m_PropType)
		return;

	if(m_pScrollBar->m_PropType->GetSelect() == SBTypeVertical)
	{
		if(::PtInRect(&m_rctVThumb,pt))
		{
			if(m_uMouseFlag & MOUSE_FLAG_ENTER_THUMB)
			{
				return;
			}
			m_uMouseFlag |=  MOUSE_FLAG_ENTER_THUMB;	
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_INC;	
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_DEC;	
		}
		else if(::PtInRect(&m_rctPageUp,pt))
		{
			if(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_INC)
			{
				return;
			}
			m_uMouseFlag |=  MOUSE_FLAG_ENTER_PAGE_INC;
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_DEC;
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_THUMB;
		}
		else if(::PtInRect(&m_rctPageDown,pt))
		{
			if(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_DEC)
			{
				return;
			}
			m_uMouseFlag |=  MOUSE_FLAG_ENTER_PAGE_DEC;
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_INC;	
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_THUMB;
		}
		else
		{			
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_INC;	
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_DEC;
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_THUMB;
		}
	}
	else
	{
		if(::PtInRect(&m_rctHThumb,pt))
		{
			if(m_uMouseFlag & MOUSE_FLAG_ENTER_THUMB)
			{
				return;
			}
			m_uMouseFlag |=  MOUSE_FLAG_ENTER_THUMB;	
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_INC;	
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_DEC;	
		}
		else if(::PtInRect(&m_rctPageLeft,pt))
		{
			if(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_INC)
			{
				return;
			}
			m_uMouseFlag |=  MOUSE_FLAG_ENTER_PAGE_INC;
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_DEC;
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_THUMB;
		}
		else if(::PtInRect(&m_rctPageRight,pt))
		{
			if(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_DEC)
			{
				return;
			}
			m_uMouseFlag |=  MOUSE_FLAG_ENTER_PAGE_DEC;
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_INC;	
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_THUMB;
		}
		else
		{
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_INC;	
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_DEC;
			m_uMouseFlag &= ~MOUSE_FLAG_ENTER_THUMB;
		}

	}
	m_pScrollBar->RedrawControl(true);
}

// 鼠标左键点击
void IScrollBarPainterImpl::OnLButtonDown(POINT pt)
{
	if(NULL == m_pScrollBar || NULL == m_pScrollBar->m_pWindowBase || NULL == m_pScrollBar->m_PropType)
		return;

	
	m_uMouseFlag |= MOUSE_FLAG_DOWN;
	do 
	{	
		m_ptCurrent = pt;
		m_nStartPos = m_nPos;
		if(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_INC)
		{
			m_enumSBDragType = SBDragPageIncrease;
		}
		else if(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_DEC)
		{
			m_enumSBDragType = SBDragPageDecrease;
		}
		else if(m_uMouseFlag & MOUSE_FLAG_ENTER_THUMB)
		{
			m_enumSBDragType = SBDragThumb;
			ChangeScrollBarStyle(SBStateDown);
			break;
		}
		else
		{
			m_enumSBDragType = SBDragPageNone;
			ChangeScrollBarStyle(SBStateDown);
			break;
		}

		ChangeScrollBarStyle(SBStateDown);
		MoveThumb();

		if(TIMER_ELAPSE < m_nDoubleClickTime)
		{
			m_nTimerId = m_pScrollBar->m_pWindowBase->SetTimer(TIMER_ELAPSE);
		}
		else
		{
			m_nTimerId = m_pScrollBar->m_pWindowBase->SetTimer(m_nDoubleClickTime);
		}

	} while (0);

}

// 鼠标左键抬起
void IScrollBarPainterImpl::OnLButtonUp(POINT pt)
{
	if(NULL == m_pScrollBar || NULL == m_pScrollBar->m_pWindowBase)
		return;

	m_pScrollBar->m_pWindowBase->KillTimer(m_nTimerId);

	m_enumSBDragType = SBDragPageNone;
	m_uMouseFlag &= ~MOUSE_FLAG_DOWN;

	if(m_uMouseFlag & MOUSE_FLAG_DRAG)
		m_uMouseFlag &= ~MOUSE_FLAG_DRAG;

	if((m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_INC) || (m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_DEC) || (m_uMouseFlag & MOUSE_FLAG_ENTER_THUMB))
	{
		ChangeScrollBarStyle(SBStatePresudoHover);
	}
	else
	{
		ChangeScrollBarStyle(SBStateNormal);
	}
}

// 鼠标左键双击
void IScrollBarPainterImpl::OnLButtonDbClick(POINT pt)
{

	if(NULL == m_pScrollBar || NULL == m_pScrollBar->m_pWindowBase)
		return;

	do 
	{
		m_ptCurrent = pt;
		m_nStartPos = m_nPos;
		if(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_INC)
		{
			m_enumSBDragType = SBDragPageIncrease;
		}
		else if(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_DEC)
		{
			m_enumSBDragType = SBDragPageDecrease;
		}
		else if(m_uMouseFlag & MOUSE_FLAG_ENTER_THUMB)
		{
			m_enumSBDragType = SBDragThumb;
			break;
		}
		else
		{
			m_enumSBDragType = SBDragPageNone;
			break;
		}

		MoveThumb();

		if(TIMER_ELAPSE < m_nDoubleClickTime)
		{
			m_nTimerId = m_pScrollBar->m_pWindowBase->SetTimer(TIMER_ELAPSE);
		}
		else
		{
			m_nTimerId = m_pScrollBar->m_pWindowBase->SetTimer(m_nDoubleClickTime);
		}

	} while (0);
}

// 鼠标滚轮消息
void IScrollBarPainterImpl::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
}


// 移动、设置控件位置
void IScrollBarPainterImpl::OnSize()
{
	if(NULL == m_pScrollBar || NULL == m_pScrollBar->m_PropType)
		return;

	if(m_pScrollBar->m_PropType->GetSelect() == SBTypeVertical)
	{
		SetVSBElementsPos(m_pScrollBar->GetClientRect());
	}
	else
	{
		SetHSBElementsPos(m_pScrollBar->GetClientRect());
	}
}
// 鼠标拖动控件
void IScrollBarPainterImpl::OnMouseDragging(POINT pt)
{
	if(NULL == m_pScrollBar || NULL == m_pScrollBar->m_PropType)
		return;

	m_uMouseFlag |=MOUSE_FLAG_DRAG;
	
	do 
	{
		if(SBTypeVertical == m_pScrollBar->m_PropType->GetSelect())
		{
			if(m_enumSBDragType == SBDragPageIncrease)
			{
				if(!::PtInRect(&m_rctPageUp, pt))
				{
					m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_INC;
				}
				else
				{
					m_uMouseFlag |= MOUSE_FLAG_ENTER_PAGE_INC;
				}
			}
			else if(m_enumSBDragType == SBDragPageDecrease)
			{
				if(!::PtInRect(&m_rctPageDown,pt))
				{
					m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_DEC;
				}
				else
				{
					m_uMouseFlag |= MOUSE_FLAG_ENTER_PAGE_DEC;
				}
			}
			else if(m_enumSBDragType == SBDragThumb)
			{
				DragThumb(pt);
				break;
			}
			else
			{
				;
			}
		}
		else
		{
			if(m_enumSBDragType == SBDragPageIncrease)
			{
				if(!::PtInRect(&m_rctPageLeft,pt))
				{
					m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_INC;
				}
				else
				{
					m_uMouseFlag |= MOUSE_FLAG_ENTER_PAGE_INC;
				}
			}
			else if(m_enumSBDragType == SBDragPageDecrease)
			{
				if(!::PtInRect(&m_rctPageRight,pt))
				{
					m_uMouseFlag &= ~MOUSE_FLAG_ENTER_PAGE_DEC;
				}
				else
				{
					m_uMouseFlag |= MOUSE_FLAG_ENTER_PAGE_DEC;
				}
			}
			else if(m_enumSBDragType == SBDragThumb)
			{
				DragThumb(pt);
				break;
			}
			else
			{
				;
			}
		}

		m_ptCurrent = pt;

	} while (0);
}

// 绘制控件的动画，如果需要重新刷新界面，返回true，否则返回false
// 此函数由定时器触发，每100毫秒触发一次
bool IScrollBarPainterImpl::OnDrawAnimation()
{
	return false;
}

void IScrollBarPainterImpl::OnTimer(UINT nTimerId)
{
	if(nTimerId == m_nTimerId)
	{
		MoveThumb();
	}
}

void IScrollBarPainterImpl::DrawHorizontalScrollBar(CDrawingBoard &DstDc)
{
	if(NULL == m_pScrollBar || m_nMax == m_nMin)
		return;

	//绘制背景
	if(NULL != m_pScrollBar->m_pPropHSBBGImages[m_enumSBState])
	{
		m_pScrollBar->m_pPropHSBBGImages[m_enumSBState]->DrawImage(DstDc,m_pScrollBar->GetClientRect());
	}

	//绘制Thumb
	DrawHThumbImage(DstDc,m_enumSBState);

	//绘制PageLeft
	DrawPageLeftImage(DstDc,m_enumSBState);

	//绘制PageRight
	DrawPageRightImage(DstDc,m_enumSBState);
}

void IScrollBarPainterImpl::DrawVerticalScrollBar(CDrawingBoard &DstDc)
{
	if(NULL == m_pScrollBar || m_nMax == m_nMin)
		return;

	//绘制背景
	if(NULL != m_pScrollBar->m_pPropVSBBGImages[m_enumSBState])
	{
		m_pScrollBar->m_pPropVSBBGImages[m_enumSBState]->DrawImage(DstDc,m_pScrollBar->GetClientRect());
	}

	//绘制Thumb
	DrawVThumbImage(DstDc,m_enumSBState);

	//绘制PageUp
	DrawPageUpImage(DstDc,m_enumSBState);

	//绘制PageDown
	DrawPageDownImage(DstDc,m_enumSBState);
}

void IScrollBarPainterImpl::DrawPageUpImage(CDrawingBoard &DstDc, SBSTATE state)
{
	if(NULL == m_pScrollBar || m_nMax == m_nMin)
		return;

	if(SBStatePresudoHover == state)
	{
		if(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_INC)
		{
			state = SBStateHover;
		}
	}
	else if(SBStateDown == state)
	{
		if(!(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_INC))
		{
			state = SBStatePresudoHover;
		}

	}

	if(NULL != m_pScrollBar->m_pPropVSBPageUpImages[state])
	{
		m_pScrollBar->m_pPropVSBPageUpImages[state]->DrawImage(DstDc,m_rctPageUp);
	}
}

void IScrollBarPainterImpl::DrawPageDownImage(CDrawingBoard &DstDc, SBSTATE state)
{
	if(NULL == m_pScrollBar || m_nMax == m_nMin)
		return;

	if(SBStatePresudoHover == state)
	{
		if(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_DEC)
		{
			state = SBStateHover;
		}
	}
	else if(SBStateDown == state)
	{
		if(!(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_DEC))
		{
			state = SBStatePresudoHover;
		}
	}

	if(NULL != m_pScrollBar->m_pPropVSBPageDownImages[state])
	{
		m_pScrollBar->m_pPropVSBPageDownImages[state]->DrawImage(DstDc,m_rctPageDown);
	}
}

void IScrollBarPainterImpl::DrawVThumbImage(CDrawingBoard &DstDc, SBSTATE state)
{
	if(NULL == m_pScrollBar || m_nMax == m_nMin)
		return;

	if(SBStatePresudoHover == state)
	{
		if(m_uMouseFlag & MOUSE_FLAG_ENTER_THUMB)
		{
			state = SBStateHover;
		}
	}
	else if(SBStateDown == state)
	{
		if(!(m_uMouseFlag & MOUSE_FLAG_ENTER_THUMB))
		{
			state = SBStatePresudoHover;
		}
	}

	if(NULL != m_pScrollBar->m_pPropVSBThumbImages[state])
	{
		m_pScrollBar->m_pPropVSBThumbImages[state]->DrawImage(DstDc,m_rctVThumb);
	}
}

void IScrollBarPainterImpl::DrawPageLeftImage(CDrawingBoard &DstDc, SBSTATE state)
{
	if(NULL == m_pScrollBar || m_nMax == m_nMin)
		return;

	if(SBStatePresudoHover == state)
	{
		if(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_INC)
		{
			state = SBStateHover;
		}
	}
	else if(SBStateDown == state)
	{
		if(!(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_INC))
		{
			state = SBStatePresudoHover;
		}

	}

	if(NULL != m_pScrollBar->m_pPropHSBPageLeftImages[state])
	{
		m_pScrollBar->m_pPropHSBPageLeftImages[state]->DrawImage(DstDc,m_rctPageLeft);
	}
}

void IScrollBarPainterImpl::DrawPageRightImage(CDrawingBoard &DstDc, SBSTATE state)
{
	if(NULL == m_pScrollBar || m_nMax == m_nMin)
		return;

	if(SBStatePresudoHover == state)
	{
		if(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_DEC)
		{
			state = SBStateHover;
		}
	}
	else if(SBStateDown == state)
	{
		if(!(m_uMouseFlag & MOUSE_FLAG_ENTER_PAGE_DEC))
		{
			state = SBStatePresudoHover;
		}
	}

	if(NULL != m_pScrollBar->m_pPropHSBPageRightImages[state])
	{
		m_pScrollBar->m_pPropHSBPageRightImages[state]->DrawImage(DstDc,m_rctPageRight);
	}
}

void IScrollBarPainterImpl::DrawHThumbImage(CDrawingBoard &DstDc, SBSTATE state)
{

	if(NULL == m_pScrollBar || m_nMax == m_nMin)
		return;

	if(SBStatePresudoHover == state)
	{
		if(m_uMouseFlag & MOUSE_FLAG_ENTER_THUMB)
		{
			state = SBStateHover;
		}
	}
	else if(SBStateDown == state)
	{
		if(!(m_uMouseFlag & MOUSE_FLAG_ENTER_THUMB))
		{
			state = SBStatePresudoHover;
		}
	}

	if(NULL != m_pScrollBar->m_pPropHSBThumbImages[state])
	{
		m_pScrollBar->m_pPropHSBThumbImages[state]->DrawImage(DstDc,m_rctHThumb);
	}
}

void IScrollBarPainterImpl::MoveThumb()
{
	bool bNeedDraw = false;

	if(NULL == m_pScrollBar || NULL == m_pScrollBar->m_PropType)
		return;

	for (int i=0 ; i< 1; i++)
	{
		if(m_pScrollBar->m_PropType->GetSelect() == SBTypeVertical)
		{
			switch (m_enumSBDragType)
			{
			case SBDragPageIncrease:
				if(!::PtInRect(&m_rctPageUp,m_ptCurrent))
					continue;

				if(m_nPos == m_nMin)
					continue;

				bNeedDraw = true;
				m_nPos -=m_nPage;
				m_nPos = (m_nPos >= m_nMin) ? m_nPos : m_nMin;
				break;
			case SBDragPageDecrease:
				if(!::PtInRect(&m_rctPageDown,m_ptCurrent))
					continue;

				if(m_nPos == m_nMax - (int)m_nPage + 1)
					continue;

				bNeedDraw = true;
				m_nPos = m_nPos + (int)m_nPage;
				m_nPos = m_nPos <= (m_nMax - (int)m_nPage + 1) ? m_nPos : (m_nMax - (int)m_nPage + 1);
				break;
			case SBDragThumb:
			default:
				continue;

			}

			SetVSBElementsPos(m_pScrollBar->GetClientRect());
		}
		else
		{
			switch (m_enumSBDragType)
			{
			case SBDragPageIncrease:
				if(!::PtInRect(&m_rctPageLeft,m_ptCurrent))
					continue;

				if(m_nPos == m_nMin)
					continue;

				bNeedDraw = true;
				m_nPos -=m_nPage;
				m_nPos = (m_nPos >= m_nMin) ? m_nPos : m_nMin;
				break;
			case SBDragPageDecrease:
				if(!::PtInRect(&m_rctPageRight,m_ptCurrent))
					continue;

				if(m_nPos == m_nMax - (int)m_nPage + 1)
					continue;

				bNeedDraw = true;
				m_nPos = m_nPos + (int)m_nPage;
				m_nPos = m_nPos <= (m_nMax - (int)m_nPage + 1) ? m_nPos : (m_nMax - (int)m_nPage + 1);
				break;
			case SBDragThumb:
			default:
				continue;

			}

			SetHSBElementsPos(m_pScrollBar->GetClientRect());
		}
	}

	if(bNeedDraw)
		m_pScrollBar->RedrawControl(true);

}

void IScrollBarPainterImpl::DragThumb(POINT pt)
{
	int nDistant = 0;
	RECT parent;

	if(NULL == m_pScrollBar || NULL == m_pScrollBar->m_PropType)
		return;

	parent = m_pScrollBar->GetClientRect();
	if(SBTypeVertical == m_pScrollBar->m_PropType->GetSelect())
	{
		nDistant = pt.y - m_ptCurrent.y;
		m_nPos = m_nStartPos + (nDistant * (m_nMax - m_nMin )) / (RECT_HEIGHT(parent)/* - m_nVThumbHeight*/);
		m_nPos = (m_nPos >= m_nMin) ? m_nPos : m_nMin;
		m_nPos = m_nPos <= (m_nMax - (int)m_nPage + 1) ? m_nPos : (m_nMax - (int)m_nPage + 1);
		SetVSBElementsPos(m_pScrollBar->GetClientRect());
	}
	else
	{
		nDistant = pt.x - m_ptCurrent.x;
		m_nPos = m_nStartPos + (nDistant * (m_nMax - m_nMin)) / (RECT_WIDTH(parent)/* - m_nHThumbWidth*/);
		m_nPos = (m_nPos >= m_nMin) ? m_nPos : m_nMin;
		m_nPos = m_nPos <= (m_nMax - (int)m_nPage + 1) ? m_nPos : (m_nMax - (int)m_nPage + 1);
		SetHSBElementsPos(parent);
	}

	m_pScrollBar->RedrawControl(true);
}

void IScrollBarPainterImpl::SetVSBElementsPos(RECT parent)
{
	UINT uSBCode = SB_TOP;
	if(NULL == m_pScrollBar)
		return;

	m_nVThumbHeight = RECT_HEIGHT(parent) * m_nPage / (m_nMax - m_nMin/* + m_nPage*/);
	if(m_nVThumbHeight < 8)
		m_nVThumbHeight = 8;
	m_nVThumbWidth = RECT_WIDTH(parent);

	//设置Thumb的位置
	m_rctVThumb.left = parent.left;
	m_rctVThumb.right = parent.right;
	m_rctVThumb.top = parent.top + RECT_HEIGHT(parent) * (m_nPos - m_nMin)/ (m_nMax - m_nMin);
	m_rctVThumb.bottom = m_rctVThumb.top + m_nVThumbHeight;

	if(m_rctVThumb.bottom >=  parent.bottom - PADDING)
	{
		m_rctVThumb.bottom = parent.bottom - PADDING;
		m_rctVThumb.top = m_rctVThumb.bottom - m_nVThumbHeight;
	}
	if(m_rctVThumb.top <=  parent.top + PADDING)
	{
		m_rctVThumb.top = parent.top + PADDING;
		m_rctVThumb.bottom = m_rctVThumb.top + m_nVThumbHeight;
	}

	//设置PageDown的位置
	m_rctPageDown.top = m_rctVThumb.bottom;
	m_rctPageDown.bottom =  parent.bottom;
	m_rctPageDown.left = m_rctVThumb.left;
	m_rctPageDown.right = m_rctVThumb.right;

	//设置PageUp的位置
	m_rctPageUp.top = parent.top;
	m_rctPageUp.bottom =  m_rctVThumb.top;
	m_rctPageUp.left = m_rctVThumb.left;
	m_rctPageUp.right = m_rctVThumb.right;
	
	// SB_THUMBPOSITION   Scroll to the absolute position. The current position is provided in nPos.

	switch (m_enumSBDragType)
	{
	case SBDragPageIncrease:
		uSBCode = SB_PAGEUP;
		break;
	case SBDragPageDecrease:
		uSBCode = SB_PAGEDOWN;
		break;
	case SBDragLineIncrease:
		uSBCode = SB_LINEUP;
	case SBDragLineDecrease:
		uSBCode = SB_LINEDOWN;
		break;
	case SBDragThumb:
		uSBCode = SB_THUMBTRACK;
		break;
	}

	if(0 == m_nPos)
	{
		uSBCode = SB_TOP;
	}
	else if(m_nPos == m_nMax - (int)m_nPage + 1)
	{
		uSBCode = SB_BOTTOM;
	}
	else
	{
	}

	m_pScrollBar->OnVScroll(uSBCode,m_nPos,m_pScrollBar);
}

void IScrollBarPainterImpl::SetHSBElementsPos(RECT parent)
{
	if(NULL == m_pScrollBar)
		return;

	m_nHThumbWidth = RECT_WIDTH(parent) * m_nPage / (m_nMax - m_nMin/* + m_nPage*/);
	if(m_nHThumbWidth < 8)
		m_nHThumbWidth = 8;
	m_nHThumbHeight = RECT_HEIGHT(parent);

	//设置Thumb的位置
	m_rctHThumb.top = parent.top;
	m_rctHThumb.bottom = parent.bottom;
	m_rctHThumb.left = parent.left + RECT_WIDTH(parent) *  (m_nPos - m_nMin)/ (m_nMax - m_nMin);
	m_rctHThumb.right = m_rctHThumb.left + m_nHThumbWidth;

	if(m_rctHThumb.right >=  parent.right - PADDING)
	{
		m_rctVThumb.right = parent.right - PADDING;
		m_rctVThumb.left = m_rctVThumb.right - m_nHThumbWidth;
	}
	if(m_rctHThumb.left <=  parent.left + PADDING)
	{
		m_rctHThumb.left = parent.left + PADDING;
		m_rctHThumb.right = m_rctHThumb.left + m_nHThumbWidth;
	}

	//设置PageRight的位置
	m_rctPageRight.left = m_rctHThumb.right;
	m_rctPageRight.right =  parent.right;
	m_rctPageRight.top = m_rctHThumb.top;
	m_rctPageRight.bottom = m_rctHThumb.bottom;

	//设置PageLeft的位置
	m_rctPageLeft.left = parent.left;
	m_rctPageLeft.right =  m_rctHThumb.left;
	m_rctPageLeft.top = m_rctHThumb.top;
	m_rctPageLeft.bottom = m_rctHThumb.bottom;
	m_pScrollBar->OnHScroll(0,m_nPos,m_pScrollBar);
}

void IScrollBarPainterImpl::ChangeScrollBarStyle(SBSTATE state)
{
	if(NULL == m_pScrollBar)
		return;

	if(state != m_enumSBState)
	{
		m_enumSBState = state;
		if(NULL != m_pScrollBar)
			m_pScrollBar->RedrawControl(true);
	}
}

void IScrollBarPainterImpl::SetScrollBarElementsPos(bool bShow)
{
	if(NULL == m_pScrollBar || NULL == m_pScrollBar->m_PropType)
		return;

	if(SBTypeVertical == m_pScrollBar->m_PropType->GetSelect())
	{
		SetVSBElementsPos(m_pScrollBar->GetClientRect());
	}
	else
	{
		SetHSBElementsPos(m_pScrollBar->GetClientRect());
	}

	m_pScrollBar->RedrawControl(bShow);
}