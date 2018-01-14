
#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_SCROLLBAR						(L"ScrollBar")

class _declspec(novtable)ICtrlScrollBar : public ICtrlInterface
{
public:	
	ICtrlScrollBar(IUiFeatureKernel *pUiKernel);
	// Get  SCROLLBARINFO
	virtual bool GetScrollBarInfo(PSCROLLBARINFO pScrollInfo) const = 0;
	// Get  SCROLLINFO
	virtual bool GetScrollInfo(LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL) = 0;
	// 设置 enable
	virtual bool EnableScrollBar(UINT nArrowFlags = ESB_ENABLE_BOTH) = 0;	
	// 得到滚动位置
	virtual int GetScrollPos() const = 0;
	// 设置滚动限制
	virtual int GetScrollLimit() = 0;	
	// 是否显示滚动条
	virtual void ShowScrollBar(bool bShow = TRUE) = 0;
	// 设置滚动位置
	virtual int SetScrollPos(int nPos, bool bRedraw = TRUE) = 0;
	// 设置滚动范围
	virtual void SetScrollRange(int nMinPos, int nMaxPos, bool bRedraw = TRUE) = 0;
	// 得到滚动范围
	virtual void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const = 0;
	// 设置滚动 信息
	virtual bool SetScrollInfo(LPSCROLLINFO lpScrollInfo, bool bRedraw = TRUE) = 0;
};
