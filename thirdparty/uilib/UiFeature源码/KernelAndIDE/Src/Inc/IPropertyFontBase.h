//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "IPropertyBase.h"
#include "CDrawingBoard.h"

// 文字显示样式
enum FONT_SHOW_MODE
{
	// 单行显示，超过显示范围末尾不显示...
	FSM_ONE_ROW_NO_POINT	= 0,
	// 单行显示，超过显示范围显示...
	FSM_ONE_ROW_POINT		= 1,
	// 折行显示
	FSM_MULTI_ROW			= 2
};

// 对齐模式
enum FONT_ALIGNING
{
	// 水平靠左/垂直靠上
	FAL_LEFT_TOP		= 0,
	// 水平居中/垂直居中
	FAL_MIDDLE			= 1,
	// 水平靠右/垂直靠下
	FAL_RIGHT_BOTTOM	= 2
};

// 文字特效
enum FONT_EFFECT
{
	FE_NONE			= 0x00000000,
	// 阴影文字
	FE_SHADOW		= 0x00000001,
	// 模糊文字
	FE_OBSCURE		= 0x00000002
};

// 字体属性
struct FONT_PROP
{
	// 是否为图片文字
	bool bIsPicText;
	// 是否为直立文字
	bool bIsZhiLiText;
	// 字体信息
	LOGFONTW Font;
	// 字体颜色
	COLORREF FontColor;
	// 垂直停靠信息
	FONT_ALIGNING VAligning;
	// 水平停靠信息
	FONT_ALIGNING HAligning;
	// 显示方式
	FONT_SHOW_MODE ShowMode;
	// 文字特效
	UINT FontEffect;
};

class _declspec(novtable) IPropertyFontBase : public IPropertyBase
{
public:
	IPropertyFontBase(IUiFeatureKernel* pUiKernel);
	// 是否为合法数据
	virtual bool IsRightData() = 0;
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode) = 0;
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode) = 0;

	// 设置字体信息
	virtual void SetFontProp(FONT_PROP *pFontProp) = 0;
	virtual FONT_PROP* GetFontProp() = 0;

	// 是否含有指定特效
	virtual bool GetEffectState(FONT_EFFECT FontEffect) = 0;
	virtual void SetEffect(FONT_EFFECT FontEffect, bool bSet) = 0;

	// 绘制文字
	virtual bool DrawFontText(CDrawingBoard &DstDc, LPCWSTR pszOutText, RECT DstRct) = 0;

	// 设置图片文字重绘标志，设置完成后，将在下次绘制的时候重新创建图片文字
	virtual void SetPictureTextRedrawSign() = 0;

	// 取得绘制文字的矩形大小
	// DrawRect：返回的实际的文字输出的矩形大小，可能会超过DC的大小
	// RectInDc：返回在DC中绘制的位置，这个位置并非100%精确，不同字体会稍有偏差
	virtual bool GetTextDrawRect(CDrawingBoard* pDstBoard, LPCWSTR pszDrawText, RECT &DrawRect, RECT &RectInDc) = 0;
};
