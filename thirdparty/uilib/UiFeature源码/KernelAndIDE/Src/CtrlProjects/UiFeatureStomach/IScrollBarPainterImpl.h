#pragma once
#include "IScrollBarPainter.h"


class IScrollBarPainterImpl : public IScrollBarPainter
{
public:
	IScrollBarPainterImpl(ICtrlScrollBarImpl* pScrollBar);
	virtual ~IScrollBarPainterImpl();
public:
	// 绘制控件
	virtual void OnPaint(CDrawingBoard &DstDc);
	// 鼠标进入
	virtual void OnMouseEnter(POINT pt);
	// 鼠标移出
	virtual void OnMouseLeave(POINT pt);
	// 鼠标移动
	virtual void OnMouseMove(POINT pt);
	// 鼠标左键点击
	virtual void OnLButtonDown(POINT pt);
	// 鼠标左键抬起
	virtual void OnLButtonUp(POINT pt);
	// 鼠标左键双击
	virtual void OnLButtonDbClick(POINT pt);
	// 鼠标滚轮消息
	virtual void OnMouseWheel(WPARAM wParam, LPARAM lParam);
	// 移动、设置控件位置
	virtual void OnSize();
	// 鼠标拖动控件
	virtual void OnMouseDragging(POINT pt);
	// 绘制控件的动画，如果需要重新刷新界面，返回true，否则返回false
	// 此函数由定时器触发，每100毫秒触发一次
	virtual bool OnDrawAnimation();
	// 定时器
	virtual void OnTimer(UINT nTimerId);

private:
	void DrawHorizontalScrollBar(CDrawingBoard &DstDc);
	void DrawVerticalScrollBar(CDrawingBoard &DstDc);
	void DrawPageLeftImage(CDrawingBoard &DstDc, SBSTATE state);
	void DrawPageRightImage(CDrawingBoard &DstDc, SBSTATE state);
	void DrawHThumbImage(CDrawingBoard &DstDc, SBSTATE state);
	void DrawPageUpImage(CDrawingBoard &DstDc, SBSTATE state);
	void DrawPageDownImage(CDrawingBoard &DstDc, SBSTATE state);
	void DrawVThumbImage(CDrawingBoard &DstDc, SBSTATE state);
	void SetVSBElementsPos(RECT parent);
	void SetHSBElementsPos(RECT parent);
	void MoveThumb();
	void DragThumb(POINT pt);
	void ChangeScrollBarStyle(SBSTATE state);

protected:
	void SetScrollBarElementsPos(bool bShow = TRUE);

private:

	int							m_nHThumbHeight;
	int							m_nHThumbWidth;	
	int							m_nVThumbHeight;
	int							m_nVThumbWidth;
	int							m_nTimerId;
	int							m_nDistant;

	RECT						m_rctHThumb;
	RECT						m_rctPageLeft;
	RECT						m_rctPageRight;
	RECT						m_rctVThumb;
	RECT						m_rctPageUp;
	RECT						m_rctPageDown;

	POINT						m_ptCurrent;
};
