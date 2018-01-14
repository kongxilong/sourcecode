//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "..\..\Inc\IPropertyFont.h"

class IPropertyFontImpl : public IPropertyFont
{
public:
	IPropertyFontImpl(IUiFeatureKernel* pUiKernel);
	virtual ~IPropertyFontImpl();

	// 是否为合法数据
	virtual bool IsRightData();
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode);
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode);

	virtual const WCHAR * GetFontBaseName();

	// 设置字体信息
	virtual void SetFontBaseProp(IPropertyFontBase *pFontProp);
	virtual IPropertyFontBase* GetFontBaseProp();

	// 绘制文字
	virtual bool DrawFontText(CDrawingBoard &DstDc, LPCWSTR pszOutText, RECT DstRct);

	// 设置图片文字重绘标志，设置完成后，将在下次绘制的时候重新创建图片文字
	virtual void SetPictureTextRedrawSign();

	// 取得绘制文字的矩形大小
	virtual bool GetTextDrawRect(CDrawingBoard* pDstBoard, LPCWSTR pszDrawText, RECT &DrawRect, RECT &RectInDc);
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);

private:
	void SetNeedRedraw();
};
