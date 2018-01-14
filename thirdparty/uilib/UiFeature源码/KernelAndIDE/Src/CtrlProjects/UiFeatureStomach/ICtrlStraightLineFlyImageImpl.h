
#pragma once
#include "..\..\Inc\Control\ICtrlStraightLineFlyImage.h"
#include "FlyWindowManager.h"

class ICtrlStraightLineFlyImageImpl : public ICtrlStraightLineFlyImage
{
public:
	ICtrlStraightLineFlyImageImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlStraightLineFlyImageImpl();

	// 开始一个动画，设置动画开始和结束的位置，返回值是动画的一个KEY
	virtual VOID* StartFly(RECT rctBegin, RECT rctEnd);
	virtual void GetFlyPostion(RECT &rctBegin, RECT &rctEnd);

protected:
	// 派生控件用于创建属于自己的控件属性
	// bIsNewCtrl：true时说明是Builder调用，新创建一个控件，需要初始化属性的各个默认值
	// false：说明整个皮肤包已经初始化完毕，属性创建完毕的时候，从xml中读取的属性值已经赋值完毕了，不能再初始化，否则xml中的值会被冲掉
	virtual bool CreateControlPropetry(bool bIsNewCtrl);
	// 初始化控件
	virtual void OnCreate();
	// 控件初始化完毕
	virtual void OnFinalCreate();
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);
	// 销毁控件
	virtual void OnDestroy();
	// 绘制控件
	virtual void OnPaint(CDrawingBoard &DrawBoard);
	// 定时器
	virtual void OnTimer(UINT nTimerId);

private:
	CFlyWindowManager m_FlyWindowMgr;

	// 动画显示的移动图片
	IPropertyImage* m_pPropFlyImage;
	IPropertyInt* m_pPropFlyAlpha;
	IPropertyInt* m_pPropFlyTime;

	// 起点
	IPropertyInt* m_pPropBeginRctLeft;
	IPropertyInt* m_pPropBeginRctTop;
	IPropertyInt* m_pPropBeginWidth;
	IPropertyInt* m_pPropBeginHeight;

	// 重点
	IPropertyInt* m_pPropEndRctLeft;
	IPropertyInt* m_pPropEndRctTop;
	IPropertyInt* m_pPropEndWidth;
	IPropertyInt* m_pPropEndHeight;
};
