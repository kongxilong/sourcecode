#pragma once
#include "..\..\Inc\CUiFeatureWindow.h"

class CFeatureRunWindow : public CUiFeatureWindow
{
public:
	CFeatureRunWindow();
	virtual ~CFeatureRunWindow();

	bool ShowFeatureRunWindow(LPCWSTR pCmdLine, DWORD dwMainThreadId);
	void UnInitialized();

	BEGIN_WNDPROC_MESSAGE
		WNDPROC_MESSAGE_HANDLER(UI_FEATURE_KERNEL_MSG, OnUiFeatureKernelMsg)
	END_WNDPROC_MESSAGE

	// 控件发送给窗口的消息接口
	BEGIN_CTRL_COMMAND
	WIN32_WND_CTRL_COMMAND
		KERNEL_COMMAND_HANDLER(KCM_SYS_COMMAND_CLOSE, OnSysCommandClose)
	END_CTRL_COMMAND

protected:
	virtual void OnKeyDown(int nVirtKey, int nFlag);
	virtual void OnCreate();
	LRESULT OnUiFeatureKernelMsg(WPARAM wParam, LPARAM lParam, BOOL &bContinue);
	LRESULT OnSysCommandClose(WPARAM wParam, LPARAM lParam);
	void OnExitWindow();

private:
	DWORD m_dwMainThreadId;
};
