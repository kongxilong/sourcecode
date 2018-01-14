
#pragma once
#include "..\..\Inc\Control\ICtrlAndroidList.h"

class ICtrlAndroidListImpl : public ICtrlAndroidList
{
public:
	ICtrlAndroidListImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlAndroidListImpl();

	// 隐藏 ListItem 中的一个子控件，并腾出这个控件的地方
	virtual void Gone(IControlBase* pGoneCtrl);
	// 隐藏/显示 ListItem 中的一个子控件，被 gone 的控件也可以显示，并会重新占用原来的腾出来的空间
	virtual void Visible(IControlBase* pCtrl, bool bVisible);
	// 在 UiFeatureBuilder 中设计出来的 ListItem 的模板，在运行时态一行要显示多少个
	virtual int GetTempletColumnCount();
	// 整个列表是否为空
	virtual bool IsEmpty();

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
	// 当一个控件包含了其他子控件的时候，系统会发送创建或加载子控件的消息
	virtual void OnCreateSubControl(bool bIsCreate);
};
