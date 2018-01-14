
#pragma once
#include "..\..\Inc\Control\ICtrlImageBox.h"

class ICtrlImageBoxImpl : public ICtrlImageBox
{
public:
	ICtrlImageBoxImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlImageBoxImpl();

	virtual IPropertyImageBase* GetBackgroundImage();
	// 设置背景图
	virtual bool SetBackgroundImage(LPCWSTR pszImagePath);
	virtual bool SetBackgroundImage(IPropertyImageBase* pNewImageBase);
	// 设置动画类型
	virtual void SetImageAnimation(IMAGE_BOX_ANIMATION paType);
	virtual IMAGE_BOX_ANIMATION GetImageAnimation();

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
	// 绘制控件的动画，如果需要重新刷新界面，返回true，否则返回false
	// 此函数由定时器触发，每100毫秒触发一次
	virtual bool OnDrawAnimation();
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

private:
	IPropertyImage* m_pPropImage;
	IPropertyComboBox* m_pChangeImgAnimation;

	IPropertyImageBase* m_pOldImgBase;
	IPropertyImageBase* m_pNewImgBase;

	int m_nNewImgAlpha;
};
