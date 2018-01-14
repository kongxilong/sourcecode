
#pragma once
#include "..\ICtrlInterface.h"
#include <atlstr.h>
using namespace ATL;

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_EDIT						(L"Edit")

// edit框发给附属控件的消息
enum CTRL_MSG_EDIT
{
	// EDIT消息Base，此消息值不允许被发送，只是作为动画消息值的Base
	CM_NORMAL_EDIT_BASE	= 0x10007000,
	// EDIT创建完毕
	CM_NORMAL_EDIT_CREATE,
	// EDIT销毁
	CM_NORMAL_EDIT_DESTROY,
	// EDIT显示
	CM_NORMAL_EDIT_SHOW,
	// EDIT隐藏
	CM_NORMAL_EDIT_HIDE,
	// EDIT得到焦点
	CM_NORMAL_EDIT_SET_FOCUS,
	// EDIT失去焦点
	CM_NORMAL_EDIT_KILL_FOCUS,
	// EDIT有文字输入
	CM_NORMAL_EDIT_CHAR,
	CM_NORMAL_EDIT_KEY_DOWN,
	CM_NORMAL_EDIT_KEY_UP,
	CM_NORMAL_EDIT_MOUSE_MOVE,
	// 按下功能键
	CM_NORMAL_EDIT_FUNCTION_KEY_DOWN,
	CM_NORMAL_EDIT_MESSAGE_END = 0x10005FFF
};

class _declspec(novtable) ICtrlEdit : public ICtrlInterface
{
public:
	ICtrlEdit(IUiFeatureKernel *pUiKernel);
	virtual LPCWSTR GetEditText() = 0;
	virtual void SetEditText(LPCWSTR pszText) = 0;
	virtual void SetEditFocus() = 0;
	// 判断当前edit是否得到输入焦点
	virtual bool IsForegroundWindow() = 0;
	// 取得Edit托管窗口的句柄
	virtual HWND GetWindowHwnd() = 0;
	// 取得Edit窗口的句柄
	virtual HWND GetEditHwnd() = 0;
	virtual void EnableEditWindow(BOOL bEnable) = 0;
	virtual void SetReadOnly(BOOL bReadOnly) = 0;
	virtual void GetEditRemindText(CStringW &strText) = 0;
	virtual void SetEditRemindText(WCHAR *pszText) = 0;
	virtual void SetEditToRemindText() = 0;
	virtual void SetEditAutoVScroll(bool bVScroll) = 0;
	virtual void SetEditAutoHScroll(bool bHScroll) = 0;
	virtual void SetEditMultiLine(bool bMultiline = false) = 0;
};
