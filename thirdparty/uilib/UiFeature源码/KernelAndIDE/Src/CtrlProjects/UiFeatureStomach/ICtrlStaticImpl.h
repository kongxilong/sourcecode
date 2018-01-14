
#pragma once
#include "..\..\Inc\Control\ICtrlStatic.h"

class ICtrlStaticImpl : public ICtrlStatic
{
public:
	ICtrlStaticImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlStaticImpl();

	// 设置控件的文本
	virtual bool SetText(LPCWSTR pszText);
	virtual LPCWSTR GetText();

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
	void OnPaint(CDrawingBoard &DrawBoard);
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

private:
	IPropertyImage*				m_pPropImage;
	IPropertyFont*				m_pPropFont; 
	IPropertyString*			m_pPropText;
	IPropertyInt*				m_pPorpTextLeftGlow;			// 文字左边距
	IPropertyInt*				m_pPorpTextTopGlow;				// 文字上边距
	IPropertyInt*				m_pPorpTextRightGlow;			// 文字右边距
	IPropertyInt*				m_pPorpTextBottomGlow;			// 文字下边距
};
