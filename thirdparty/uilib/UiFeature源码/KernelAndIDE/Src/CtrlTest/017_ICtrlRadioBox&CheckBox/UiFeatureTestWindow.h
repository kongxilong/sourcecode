#pragma once
#include "..\..\Inc\CUiFeatureWindow.h"
#include "..\..\Inc\Control\ICtrlImageBox.h"
#include "..\..\Inc\Control\ICtrlCheckBox.h"
#include "..\..\Inc\Control\ICtrlRadioBox.h"
class CUiFeatureTestWindow : public CUiFeatureWindow
{
public:
	CUiFeatureTestWindow();
	virtual ~CUiFeatureTestWindow();

	bool ShowUiFeatureTestWindow(DWORD dwMainThreadId);
	void UnInitialized();

	// 控件发送给窗口的消息接口
	BEGIN_CTRL_COMMAND
	WIN32_WND_CTRL_COMMAND
		CTRL_COMMAND_HANDLER(m_pr1, CM_RADIO_BOX_UP, Onr1)
		CTRL_COMMAND_HANDLER(m_pr2, CM_RADIO_BOX_UP, Onr2)
		KERNEL_COMMAND_HANDLER(KCM_SYS_COMMAND_CLOSE, OnSysCommandClose)
	END_CTRL_COMMAND

	BEGIN_WNDPROC_MESSAGE
	END_WNDPROC_MESSAGE

protected:
	// WM_CREATE：对话框初始化	
	virtual void OnCreate();

	virtual void OnKeyDown(int nVirtKey, int nFlag);

	LRESULT Onr1(WPARAM wParam, LPARAM lParam);
	LRESULT Onr2(WPARAM wParam, LPARAM lParam);
	LRESULT OnSysCommandClose(WPARAM wParam, LPARAM lParam);

private:
	DWORD m_dwMainThreadId;
	ICtrlRadioBox* m_pr1, *m_pr2;
};
