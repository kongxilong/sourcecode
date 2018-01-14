
#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_IMAGE_BOX						(L"ImageBox")

// 替换图片的时候的动画
enum IMAGE_BOX_ANIMATION
{
	// 无动画
	IBA_NONE		= 0,
	// 渐隐渐现
	IBA_JYJX
};

class _declspec(novtable) ICtrlImageBox : public ICtrlInterface
{
public:
	ICtrlImageBox(IUiFeatureKernel *pUiKernel);
	virtual IPropertyImageBase* GetBackgroundImage() = 0;
	// 设置背景图
	virtual bool SetBackgroundImage(LPCWSTR pszImagePath) = 0;
	virtual bool SetBackgroundImage(IPropertyImageBase* pNewImageBase) = 0;
	// 设置动画类型
	virtual void SetImageAnimation(IMAGE_BOX_ANIMATION paType) = 0;
	// 取得当前换图的动画
	virtual IMAGE_BOX_ANIMATION GetImageAnimation() = 0;
};
