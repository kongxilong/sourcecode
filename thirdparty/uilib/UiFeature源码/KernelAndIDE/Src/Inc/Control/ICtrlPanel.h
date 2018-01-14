
#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_BASE_PANEL						(L"Panel")

// 控件向窗口发送的消息
enum CTRL_MSG_PANEL
{
	// 消息Base，此消息值不允许被发送，只是作为控件消息值的Base
	CM_PANEL_BASE	= 0x10001000,
	// Down消息
	CM_PANEL_DOWN,
	// Up消息
	CM_PANEL_UP,
	// 进入 
	CM_PANEL_ENTER,
	// 离开
	CM_PANEL_LEAVE,
	// 双击
	CM_PANEL_DBCLICK,
	// Button 右键按下消息
	CM_PANEL_R_BTN_DOWN
};


// 替换图片的动画类型
enum PANEL_ANIMATION
{
	// 无动画
	PA_NONE		= 0,
	// 渐隐渐现
	PA_JYJX
};

class _declspec(novtable) ICtrlPanel : public ICtrlInterface
{
public:
	ICtrlPanel(IUiFeatureKernel *pUiKernel);
	// 设置背景图
	virtual bool SetBackgroundImage(LPCWSTR pszImagePath, bool bReload) = 0;
	// 设置动画类型
	virtual void SetImageAnimation(PANEL_ANIMATION paType) = 0;
	virtual PANEL_ANIMATION GetImageAnimation() = 0;
	// 回复到皮肤包中的图片
	virtual void SetToDefault(bool bRedraw, bool bDeleteOld = true) = 0;
	// 是否显示手型指针开关
	virtual void SetIsShowCursor(bool bIsShow) = 0;
	// 设置背景图
	virtual bool SetBackgroundImage(HBITMAP hbmp, bool bReload) = 0;
	// get背景图
	virtual IPropertyImageBase* GetBackgroundImage() = 0;
	// 设置背景图
	virtual bool SetBackgroundImage(IPropertyImageBase* pNewImageBase) = 0;
	// 设置关键字，方便做容器的时候排序用
	virtual void SetKey(LPCWSTR key) = 0;
	// getkey
	virtual LPCWSTR GetKey() = 0;
};
