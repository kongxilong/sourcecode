
#pragma once
#include "..\..\Inc\Control\ICtrlShadowProgressBar.h"

// 进度条方向
enum PROGRESS_BAR_TYPE
{
	// 水平
	PBT_H = 0,
	// 垂直
	PBT_V,
};

class ICtrlShadowProgressBarImpl : public ICtrlShadowProgressBar, public IControlMessage
{
public:
	ICtrlShadowProgressBarImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlShadowProgressBarImpl();

	// 使用镜像的方式绘制滚动条，结果会绘制到指定的dc的指定区域
	virtual bool MirrorDraw(CDrawingBoard &DrawBoard, RECT DstRect, int nPos);

	// 设置当前进度，值从0-100
	virtual void SetProgressBarPos(int nPos, bool bRedraw = true);

	virtual int GetProgressBarPos();

protected:
	virtual LRESULT OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam);

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

	// 绘制控件的动画，如果需要重新刷新界面，返回true，否则返回false
	// 此函数由定时器触发，每100毫秒触发一次
	virtual bool OnDrawAnimation();

	// 定时器
	virtual void OnTimer(UINT nTimerId);

	// 移动、设置控件位置
	virtual void OnSize();
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

	// 可见属性
	void SetVisible(bool bVisible);

	bool IsVisible();

	virtual void SetRectInOwner(RECT ctrlRectInOwner);

private:
	IPropertyComboBox* m_pPropProgressType;
	// 中间显示的文字字体
	IPropertyFont* m_pPropFont;

	// 背景图片
	IPropertyImage* m_pPropBkImg;
	// 盖在进度条上的蒙罩图片
	IPropertyImage* m_pPropMaskImg;
	// 进度条走进度的图片
	IPropertyImage* m_pPropProgressImg;

	IPropertyGroup* m_pPropSpaceGroup;
	// 进度条开始走进度距离控件左侧的偏移
	IPropertyInt* m_pPropLeftSpace;
	// 进度条开始走进度距离控件右侧的偏移
	IPropertyInt* m_pPropRightSpace;
	// 进度条开始走进度距离控件上方的偏移
	IPropertyInt* m_pPropTopSpace;
	// 进度条开始走进度距离控件下方的偏移
	IPropertyInt* m_pPropBottomSpace;
	// 可见
	bool m_bIsVisible;
	// rect
	RECT m_CtrlRectInOwner;
	// 当前位置
	int m_nCurPos;
};
