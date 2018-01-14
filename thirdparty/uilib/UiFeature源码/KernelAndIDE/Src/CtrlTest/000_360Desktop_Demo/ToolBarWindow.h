#pragma once
#include "..\..\Inc\CUiFeatureWindow.h"
#include "..\..\Inc\Control\ICtrlImageBox.h"
#include "..\..\Inc\Control\ICtrlShadowButton.h"

class CToolBarWindow : public CUiFeatureWindow
{
public:
	CToolBarWindow();
	virtual ~CToolBarWindow();

	bool ShowToolBarWindow(DWORD dwMainThreadId);
	void UnInitialized();

	// 控件发送给窗口的消息接口
	BEGIN_CTRL_COMMAND
	WIN32_WND_CTRL_COMMAND
		CTRL_COMMAND_HANDLER(m_pShowDirBtn, CM_BUTTON_UP, OnDirBtnClick)
	END_CTRL_COMMAND

protected:
	// 本窗口的消息处理函数
	virtual LRESULT WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam);
	// WM_CREATE：对话框初始化	
	virtual void OnCreate();
	LRESULT OnDirBtnClick(WPARAM wParam, LPARAM lParam);

private:
	DWORD m_dwMainThreadId;
	ICtrlImageBox* m_pDialogBkImgBox;
	ICtrlShadowButton* m_pShowDirBtn;
};
