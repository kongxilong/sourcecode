#include "StdAfx.h"
#include "IScrollBarPainter.h"
#include "ICtrlScrollBarImpl.h"
#include "..\..\Inc\IControlManager.h"
#include "..\..\Inc\ICommonFun.h"


#define	MAX										180 * 84
#define MIN										0
#define POS										1
#define PAGE									540

IScrollBarPainter::IScrollBarPainter(ICtrlScrollBarImpl* pScrollBar)
{
	m_pScrollBar = pScrollBar;

	m_nTrackPos = POS;
	m_nStartPos = POS;
	m_nPage = PAGE;
	m_uMouseFlag = 0;	

	m_nMin = MIN;
	m_nMax = MAX;
	m_nPos = POS;

	m_enumSBState = SBStateNormal;
	m_bIsShow = false;
	m_nDoubleClickTime = GetDoubleClickTime();
}

IScrollBarPainter::~IScrollBarPainter(void)
{
	m_pScrollBar = NULL;
}

bool IScrollBarPainter::EnableScrollBar(UINT nArrowFlags)
{
	return false;
}

bool IScrollBarPainter::GetScrollBarInfo(PSCROLLBARINFO pScrollInfo) const
{
	return false;
}

bool IScrollBarPainter::GetScrollInfo(LPSCROLLINFO lpScrollInfo,UINT nMask)
{
	if(NULL == lpScrollInfo)
		return false;

	switch (nMask)
	{
	case SIF_ALL:
		lpScrollInfo->nMax = m_nMax;
		lpScrollInfo->nMin = m_nMin;
		lpScrollInfo->nPage = m_nPage;
		lpScrollInfo->nPos = m_nPos;
		lpScrollInfo->nTrackPos	= m_nPos;
		break;
	case SIF_RANGE:
		lpScrollInfo->nMax = m_nMax;
		lpScrollInfo->nMin = m_nMin;
		break;
	case SIF_PAGE:
		lpScrollInfo->nPage = m_nPage;
		break;
	case SIF_POS:
		lpScrollInfo->nPos = m_nPos;
		break;
	case SIF_TRACKPOS:
		lpScrollInfo->nPos = m_nTrackPos;
		break;
	default:
		break;
	}
	return true;
}

int IScrollBarPainter::GetScrollLimit( )
{
	return m_nMax;
}

int IScrollBarPainter::GetScrollPos( ) const
{
	return m_nPos;
}

void IScrollBarPainter::GetScrollRange(LPINT lpMinPos,LPINT lpMaxPos) const
{
	if(NULL != lpMinPos && NULL != lpMaxPos)
	{
		*lpMinPos = m_nMin;
		*lpMaxPos = m_nMax;
	}
}

bool IScrollBarPainter::SetScrollInfo(LPSCROLLINFO lpScrollInfo,bool bRedraw)
{
	if(NULL != lpScrollInfo )
	{
		switch (lpScrollInfo->fMask)
		{
			case SIF_ALL:
				m_nMax = lpScrollInfo->nMax;
				m_nMin = lpScrollInfo->nMin;
				m_nPage = lpScrollInfo->nPage;
				m_nPos = lpScrollInfo->nPos;
				break;
			case SIF_RANGE:
				m_nMax = lpScrollInfo->nMax;
				m_nMin = lpScrollInfo->nMin;
				break;
			case SIF_PAGE:
				m_nPage = lpScrollInfo->nPage;
				break;
			case SIF_POS:
				m_nPos = lpScrollInfo->nPos;
				break;
			default:
				break;
		}
		
		m_nMax = m_nMin > m_nMax ? m_nMin:m_nMax;

		if((0 != m_nPage) && ((int)m_nPage > abs(m_nMax - m_nMin)))
			m_nPage = m_nMax - m_nMin + 1;

		m_nPos = (m_nPos >= m_nMin) ? m_nPos : m_nMin;
		m_nPos = m_nPos <= (m_nMax - (int)m_nPage + 1) ? m_nPos : (m_nMax - (int)m_nPage + 1);
		
		SetScrollBarElementsPos(bRedraw);

		return true;
	}
	return false;
}

int IScrollBarPainter::SetScrollPos(int nPos,bool bRedraw)
{
	int nPrePos = m_nPos;

	m_nPos = nPos;
	m_nPos = (m_nPos >= m_nMin) ? m_nPos : m_nMin;
	m_nPos = m_nPos <= (m_nMax - (int)m_nPage + 1) ? m_nPos : (m_nMax - (int)m_nPage + 1);
	
	SetScrollBarElementsPos(bRedraw);

	return nPrePos;
}

void IScrollBarPainter::SetScrollRange(int nMinPos,int nMaxPos,bool bRedraw)
{
	m_nMax = nMaxPos;
	m_nMin = nMinPos;

	m_nMax = m_nMin > m_nMax ? m_nMin:m_nMax;

	if(0 !=m_nPage)
		if((int)m_nPage > abs(m_nMax - m_nMin))
			m_nPage = m_nMax - m_nMin + 1;

	SetScrollBarElementsPos(bRedraw);
}

void IScrollBarPainter::ShowScrollBar(bool bShow)
{
	if(NULL == m_pScrollBar)
		return;

	m_bIsShow =bShow;
	m_pScrollBar->RedrawControl(true);
		
}
