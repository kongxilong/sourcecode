//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "..\..\Inc\IPropertyFontBase.h"

class IPropertyFontBaseImpl : public IPropertyFontBase
{
public:
	IPropertyFontBaseImpl(IUiFeatureKernel* pUiKernel);
	virtual ~IPropertyFontBaseImpl();

	// 是否为合法数据
	virtual bool IsRightData();
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode);
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode);

	// 设置字体信息
	virtual void SetFontProp(FONT_PROP *pFontProp);
	virtual FONT_PROP* GetFontProp();

	// 是否含有指定特效
	virtual bool GetEffectState(FONT_EFFECT FontEffect);
	virtual void SetEffect(FONT_EFFECT FontEffect, bool bSet);

	// 绘制文字
	virtual bool DrawFontText(CDrawingBoard &DstDc, LPCWSTR pszOutText, RECT DstRct);

	// 设置图片文字重绘标志，设置完成后，将在下次绘制的时候重新创建图片文字
	virtual void SetPictureTextRedrawSign();

	// 取得绘制文字的矩形大小
	virtual bool GetTextDrawRect(CDrawingBoard* pDstBoard, LPCWSTR pszDrawText, RECT &DrawRect, RECT &RectInDc);
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);

private:
	// 绘制到指定内存DC上
	bool DrawToBoard(CDrawingBoard &DstDc, LPCWSTR pszOutText, RECT DstRct);
	// 绘制阴影文字到指定内存DC上
	bool DrawShadowText(CDrawingBoard &DstDc, LPCWSTR pszOutText, RECT DstRct);
	// 绘制图片文字
	bool DrawPictureText(CDrawingBoard &DstDc, LPCWSTR pszOutText, RECT DstRct);
	// 绘制文字或者计算文字输出矩形
	bool DrawToBoardBranch(CDrawingBoard &DstDc, LPCWSTR pszOutText, RECT &DstRct, bool bIsMeasureString);

private:
	bool m_bRecreatePicText;
	FONT_PROP m_FontProp;
	// 图片文字内存DC
	CDrawingBoard m_PicTextBorad;
};
