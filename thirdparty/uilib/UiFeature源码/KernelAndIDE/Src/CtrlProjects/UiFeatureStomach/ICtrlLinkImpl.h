
#pragma once
#include "..\..\Inc\Control\ICtrlLink.h"
#include "..\..\Inc\IPropertyString.h"
#include "..\..\Inc\IPropertyGroup.h"
#include "..\..\Inc\IPropertyCursor.h"
#include "..\..\Inc\IPropertyFont.h"
#include "..\..\Inc\IPropertyFontBase.h"

class ICtrlLinkImpl : public ICtrlLink
{
public:
	ICtrlLinkImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlLinkImpl();

	// 设置Link文字
	virtual void SetLinkText(LPCWSTR pText, bool bRedraw);
	// 设置Link链接地址URL
	virtual void SetLinkUrl(LPCWSTR pUrl);
	// 取得Link链接地址URL
	virtual LPCWSTR GetLinkUrl();

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
	// 设置控件光标
	virtual IPropertyCursor* OnSetCursor(POINT pt);
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

private:
	bool m_bIsLButtonDown;

	IPropertyString* m_pPropLinkText;
	IPropertyString* m_pPropLinkUrl;
	IPropertyCursor* m_pPropCursor;
	IPropertyGroup* m_PropFontGroup;
	IPropertyFont* m_pPropFontNormal;
	IPropertyFont* m_pPropFontHover;
	IPropertyFont* m_pPropFontDown;
};
