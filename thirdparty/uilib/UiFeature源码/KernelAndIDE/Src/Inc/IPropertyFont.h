//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "IPropertyBase.h"
#include "IPropertyFontBase.h"

class _declspec(novtable) IPropertyFont : public IPropertyBase
{
public:
	IPropertyFont(IUiFeatureKernel* pUiKernel);
	// 是否为合法数据
	virtual bool IsRightData() = 0;
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode) = 0;
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode) = 0;

	virtual const WCHAR * GetFontBaseName() = 0;

	// 设置字体信息
	virtual void SetFontBaseProp(IPropertyFontBase *pFontProp) = 0;
	virtual IPropertyFontBase* GetFontBaseProp() = 0;

	// 绘制文字
	virtual bool DrawFontText(CDrawingBoard &DstDc, LPCWSTR pszOutText, RECT DstRct) = 0;

	// 设置图片文字重绘标志，设置完成后，将在下次绘制的时候重新创建图片文字
	virtual void SetPictureTextRedrawSign() = 0;

	// 取得绘制文字的矩形大小
	// DrawRect：返回的实际的文字输出的矩形大小，可能会超过DC的大小
	// RectInDc：返回在DC中绘制的位置，这个位置并非100%精确，不同字体会稍有偏差
	virtual bool GetTextDrawRect(CDrawingBoard* pDstBoard, LPCWSTR pszDrawText, RECT &DrawRect, RECT &RectInDc) = 0;
};
