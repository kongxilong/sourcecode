#pragma once
#include "..\..\inc\CUiFeatureWindow.h"

#include <winuser.h>
#include <atlstr.h>
using namespace ATL;

class CRepairPC : public CUiFeatureWindow
{
public:
	CRepairPC();
	virtual ~CRepairPC();

	bool CreateUIWindow(HWND hParent);

protected:
	// 控件发送给窗口的消息接口
	virtual LRESULT OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam);
	// 本窗口的消息处理函数
	virtual LRESULT WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam);
	// WM_CREATE：对话框初始化	
	virtual void OnCreate();

	virtual void OnKeyDown(int nVirtKey, int nFlag);
};