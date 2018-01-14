#pragma once
#include "..\..\Inc\CUiFeatureWindow.h"
#include "..\..\Inc\Control\ICtrlShadowButton.h"
#include "..\..\Inc\Control\ICtrlPanel.h"

class CCloneWindow : public CUiFeatureWindow
{
public:
	CCloneWindow();
	virtual ~CCloneWindow();

	bool ShowCloneWindow(IUiFeatureKernel* pUiKernel, HWND hParentWnd);
	void UnInitialized();

	// 控件发送给窗口的消息接口
	BEGIN_CTRL_COMMAND
	WIN32_WND_CTRL_COMMAND
		CTRL_COMMAND_HANDLER(m_pShowDirBtn, CM_BUTTON_UP, OnDirBtnClick)
	END_CTRL_COMMAND

	BEGIN_WNDPROC_MESSAGE
	END_WNDPROC_MESSAGE

protected:
	// WM_CREATE：对话框初始化	
	virtual void OnCreate();

	virtual void OnKeyDown(int nVirtKey, int nFlag);

	LRESULT OnDirBtnClick(WPARAM wParam, LPARAM lParam);

private:
	ICtrlShadowButton* m_pShowDirBtn;
	ICtrlPanel* m_pClonePanel;
	ICtrlPanel* m_pNewClonePanel;
};
