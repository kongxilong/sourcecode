
#pragma once
#include "..\..\Inc\Control\ICtrlFocus.h"

class ICtrlFocusImpl : public ICtrlFocus
{
public:
	ICtrlFocusImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlFocusImpl();

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
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);
	// 当一个控件的TabOrder属性被设置了之后，同时此控件得到焦点，窗口会将此消息
	// 通过这个函数发送给注册了需要取得焦点消息的控件
	virtual void OnContrlFocusChange(IControlBase* pFocusCtrl);
	virtual void OnMoveAnimationDraw();

private:
	bool IsUseAnimation();
	bool SetMoveRect(int nAnimationTime, int nPropMoveTime);
	void DoAnimation(CDrawingImage &WndMemDc, HDC &hWndDc, BLENDFUNCTION &Blend);

private:
	RECT m_BeginRct;
	RECT m_EndRct;
	RECT m_MoveRct;
	bool m_bIsInAnimation;

	IPropertyInt* m_pPropFrameSpace;
	IControlBase* m_pFocusCtrl;
	IPropertyImage *m_pPropFrameImage;
	IPropertyBool* m_pPropUseAnimation;
	IPropertyInt* m_pPropAnimationTime;
};
