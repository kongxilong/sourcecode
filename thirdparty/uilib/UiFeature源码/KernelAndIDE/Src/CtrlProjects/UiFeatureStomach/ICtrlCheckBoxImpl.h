#pragma once
#include "..\..\Inc\Control\ICtrlCheckBox.h"

class ICtrlCheckBoxImpl : public ICtrlCheckBox
{
public:
	ICtrlCheckBoxImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlCheckBoxImpl();

	// interface

	// 设置文字
	void SetText(LPCWSTR pText, bool bRedraw);

	// 设置选中属性
	void SetCheck(bool bCheck);

	// 取得选中属性
	bool GetCheck();

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
	// 鼠标左键点击
	virtual void OnLButtonDown(POINT pt);
	// 鼠标左键抬起
	virtual void OnLButtonUp(POINT pt);
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

private:
	// 左键是否按下
	bool				m_bIsLButtonDown;
	// 是否check
	bool				m_bCheck;
	// 图片左上角定位点
	IPropertyInt*		m_pImageLeft;
	IPropertyInt*		m_pImageTop;
	// 非选中状态图片
	IPropertyImage*		m_pImageNormal;
	IPropertyImage*		m_pImageHover;
	IPropertyImage*		m_pImageDown;
	IPropertyImage*		m_pImageDisbale;
	// 选中状态图片
	IPropertyImage*		m_pImageCheckNormal;
	IPropertyImage*		m_pImageCheckHover;
	IPropertyImage*		m_pImageCheckDown;
	IPropertyImage*		m_pImageCheckDisbale;
	// 文字
	IPropertyString*	m_pPropText;
	// 文字左上角定位点
	IPropertyInt*		m_pTextLeft;
	IPropertyInt*		m_pTextTop;
	// 字体
	IPropertyGroup*		m_pPropFontGroup;
	IPropertyFont*		m_pPropFontNormal;
	IPropertyFont*		m_pPropFontHover;
	IPropertyFont*		m_pPropFontDown;
	IPropertyFont*		m_pPropFontDisable;
};