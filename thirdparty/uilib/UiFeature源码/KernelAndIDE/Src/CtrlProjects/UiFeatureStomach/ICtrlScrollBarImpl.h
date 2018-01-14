
#pragma once
#include "..\..\Inc\Control\ICtrlScrollBar.h"

#include "IScrollBarPainterImpl.h"

class ICtrlScrollBarImpl : public ICtrlScrollBar
{
friend class IScrollBarPainterImpl;

public:
	ICtrlScrollBarImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlScrollBarImpl();

	// Get  SCROLLBARINFO
	virtual bool GetScrollBarInfo(PSCROLLBARINFO pScrollInfo) const;
	// Get  SCROLLINFO
	virtual bool GetScrollInfo(LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
	// 设置 enable
	virtual bool EnableScrollBar(UINT nArrowFlags = ESB_ENABLE_BOTH);	
	// 得到滚动位置
	virtual int GetScrollPos() const;
	// 设置滚动限制
	virtual int GetScrollLimit();	
	// 是否显示滚动条
	virtual void ShowScrollBar(bool bShow = TRUE);
	// 设置滚动位置
	virtual int SetScrollPos(int nPos,bool bRedraw = TRUE);
	// 设置滚动范围
	virtual void SetScrollRange(int nMinPos, int nMaxPos,bool bRedraw = TRUE);
	// 得到滚动范围
	virtual void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const;
	// 设置滚动 信息
	virtual bool SetScrollInfo(LPSCROLLINFO lpScrollInfo, bool bRedraw = TRUE);

protected:
	// 派生控件用于创建属于自己的控件属性
	// bIsNewCtrl：true时说明是Builder调用，新创建一个控件，需要初始化属性的各个默认值
	// false：说明整个皮肤包已经初始化完毕，属性创建完毕的时候，从xml中读取的属性值已经赋值完毕了，不能再初始化，否则xml中的值会被冲掉
	virtual bool CreateControlPropetry(bool bIsNewCtrl);
	// 初始化控件
	virtual void OnCreate();
	// 控件初始化完毕
	virtual void OnFinalCreate();
	// 销毁控件
	virtual void OnDestroy();
	// 绘制控件
	virtual void OnPaint(CDrawingBoard &DrawBoard);
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
	// 控件取得焦点，通过Tab键跳转，激活控件
	virtual void OnSetFocus();
	// 控件失去焦点
	virtual void OnKillFocus();
	// 鼠标滚轮消息
	virtual void OnMouseWheel(WPARAM wParam, LPARAM lParam);
	// 移动、设置控件位置
	virtual void OnSize();
	// 鼠标拖动控件
	virtual void OnMouseDragging(POINT pt);
	// 绘制控件的动画，如果需要重新刷新界面，返回true，否则返回false
	// 此函数由定时器触发，每100毫秒触发一次
	virtual bool OnDrawAnimation();
	// Builder刷新属性，此函数中只要重新加载控件属性即可
	virtual void OnBuilderRefreshProp(IPropertyBase* pPropBase);
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);
	// 定时器
	virtual void OnTimer(UINT nTimerId);
	//垂直滚动条滚动
	virtual void OnVScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar);
	//水平滚动条滚动
	virtual void OnHScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar);

private:
	void PrepareScrollBarPainter();
	void PrepareScrollBarOwner();

private:

	//////////////////////////////////////////////////////////////////////////////
	// 水平滚动条属性
	IPropertyImage*		m_pPropHSBLineRightImages[SBStateNum]; 
	IPropertyImage*		m_pPropHSBGotoRightImages[SBStateNum]; 
	IPropertyImage*		m_pPropHSBGotoLeftImages[SBStateNum]; 
	IPropertyImage*		m_pPropHSBThumbImages[SBStateNum];
	IPropertyImage*		m_pPropHSBBGImages[SBStateNum];
	IPropertyImage*		m_pPropHSBPageLeftImages[SBStateNum];
	IPropertyImage*		m_pPropHSBPageRightImages[SBStateNum];
	IPropertyImage*		m_pPropHSBLineLeftImages[SBStateNum]; 

	//////////////////////////////////////////////////////////////////////////////
	// 垂直滚动条属性
	IPropertyImage*		m_pPropVSBLineDownImages[SBStateNum]; 
	IPropertyImage*		m_pPropVSBGotoBottomImages[SBStateNum]; 
	IPropertyImage*		m_pPropVSBGotoTopImages[SBStateNum]; 
	IPropertyImage*		m_pPropVSBThumbImages[SBStateNum];
	IPropertyImage*		m_pPropVSBBGImages[SBStateNum];
	IPropertyImage*		m_pPropVSBPageUpImages[SBStateNum];
	IPropertyImage*		m_pPropVSBPageDownImages[SBStateNum];
	IPropertyImage*		m_pPropVSBLineUpImages[SBStateNum]; 

	//////////////////////////////////////////////////////////////////////////////

	
	IPropertyComboBox*	m_PropType;				// 滚动条Type属性 垂直/水平
	IPropertyString*	m_PropOwnerCtrlName;	// 滚动条的使用者的名字
	IControlBase*		m_pOwnerCtrl;			// 滚动条的使用者的指针
	IScrollBarPainter*	m_pPainter;				// 滚动条绘制接口
};
