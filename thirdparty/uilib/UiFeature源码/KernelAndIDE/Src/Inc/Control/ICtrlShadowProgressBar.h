
#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_SHADOW_PROGRESS_BAR				(L"ShadowProgressBar")

class _declspec(novtable) ICtrlShadowProgressBar : public ICtrlInterface
{
public:
	ICtrlShadowProgressBar(IUiFeatureKernel *pUiKernel);
	// 设置当前进度，值从0-100
	virtual void SetProgressBarPos(int nPos, bool bRedraw = true) = 0;
	// 获取当前值
	virtual int GetProgressBarPos() = 0;
	// 使用镜像的方式绘制滚动条，结果会绘制到指定的dc的指定区域
	virtual bool MirrorDraw(CDrawingBoard &DrawBoard, RECT DstRect, int nPos) = 0;
};
