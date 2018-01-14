
#pragma once
#include <assert.h>
#include "..\..\Inc\Control\ICtrlScrollBar.h"

class ICtrlScrollBarImpl;

typedef enum enumSbMode
{
	SBModeWindows = 0,
	SBModeMac,
	SBModeNum
}SBMODE;

typedef enum enumSbState
{
	SBStateInvalid = -1,
	SBStateNormal,
	SBStatePresudoHover,
	SBStateHover,
	SBStateDown,
	//在此之前增加ScrollBar的状态
	SBStateNum
}SBSTATE;

typedef enum enumDragType
{
	SBDragPageNone = 0,
	SBDragPageIncrease,
	SBDragPageDecrease,
	SBDragLineIncrease,
	SBDragLineDecrease,
	SBDragThumb,
	SBDragTypeNum
}SBDRAGTYPE;

typedef enum enumType
{
	SBTypeVertical = 0,
	SBTypeHorizontal,
	SBTypeNum
}SBTYPE;

class IScrollBarPainter
{
public:
	// con
	IScrollBarPainter(ICtrlScrollBarImpl* pScrollBar);
	// des
	virtual ~IScrollBarPainter();
public:	 
	virtual bool SetScrollInfo(LPSCROLLINFO lpScrollInfo, bool bRedraw = TRUE);
	virtual int  SetScrollPos(int nPos, bool bRedraw = TRUE);
	virtual void SetScrollRange(int nMinPos, int nMaxPos, bool bRedraw = TRUE);
	virtual bool EnableScrollBar(UINT nArrowFlags = ESB_ENABLE_BOTH);
	virtual bool GetScrollBarInfo(PSCROLLBARINFO pScrollInfo) const;
	virtual bool GetScrollInfo(LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
	virtual int  GetScrollPos() const;
	virtual int  GetScrollLimit();
	virtual void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const;
	virtual void ShowScrollBar(bool bShow = TRUE);
	// 绘制控件
	virtual void OnPaint(CDrawingBoard &DstDc) = 0;
	// 鼠标进入
	virtual void OnMouseEnter(POINT pt) = 0;
	// 鼠标移出
	virtual void OnMouseLeave(POINT pt) = 0;
	// 鼠标移动
	virtual void OnMouseMove(POINT pt) = 0;
	// 鼠标左键点击
	virtual void OnLButtonDown(POINT pt) = 0;
	// 鼠标左键抬起
	virtual void OnLButtonUp(POINT pt) = 0;
	// 鼠标左键双击
	virtual void OnLButtonDbClick(POINT pt) = 0;
	// 鼠标滚轮消息
	virtual void OnMouseWheel(WPARAM wParam, LPARAM lParam) = 0;
	// 移动、设置控件位置
	virtual void OnSize() = 0;
	// 鼠标拖动控件
	virtual void OnMouseDragging(POINT pt) = 0;
	// 绘制控件的动画，如果需要重新刷新界面，返回true，否则返回false
	// 此函数由定时器触发，每100毫秒触发一次
	virtual bool OnDrawAnimation() = 0;
	// 定时器
	virtual void OnTimer(UINT nTimerId) = 0;

protected:
	virtual void SetScrollBarElementsPos(bool bShow = TRUE) = 0;

protected:
	
	int									m_nMin; 
	int									m_nMax; 
	int									m_nPos; 
	int									m_nTrackPos; 
	int									m_nStartPos;
	UINT								m_nPage;
	UINT								m_nDoubleClickTime;
	ULONG								m_uMouseFlag;
	bool								m_bIsShow;
	SBSTATE								m_enumSBState;
	SBDRAGTYPE							m_enumSBDragType;
	ICtrlScrollBarImpl* 				m_pScrollBar;
};
