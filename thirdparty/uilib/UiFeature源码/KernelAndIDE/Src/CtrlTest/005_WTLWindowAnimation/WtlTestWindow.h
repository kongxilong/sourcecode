
#pragma once

#include "..\..\Inc\CWTLUiFeatureWindowT.h"
#include "..\..\Inc\Control\ICtrlShadowButton.h"
#include "..\..\Inc\Control\ICtrlLink.h"
#include "..\..\Inc\Control\ICtrlImageBox.h"

class CWtlTestWindow : public CWTLUiFeatureWindowT<CWtlTestWindow>
{
public:
	CWtlTestWindow();
	virtual ~CWtlTestWindow();

	bool ShowWtlTestWindow(CAppModule *_pModule, DWORD dwMainThreadId);
	void UnInitialized();

	DECLARE_WND_CLASS(_T("UiFeatureWtlWindow"))

	BEGIN_MSG_MAP(CWtlTestWindow)
		CHAIN_MSG_MAP(CWTLUiFeatureWindowT)
		MESSAGE_HANDLER(UI_FEATURE_KERNEL_MSG, OnUiFeatureMessage)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	END_MSG_MAP()

protected:
	// 控件发送给窗口的消息接口
	virtual LRESULT OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam);
	// WM_CREATE：对话框初始化	
	virtual VOID OnWTLWindowCreate();

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUiFeatureMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	void CloseTestWindow();
	void SetWindowAnimation();

private:
	DWORD m_dwMainThreadId;
	ICtrlShadowButton* m_pAnimationBtn;

	ICtrlImageBox* m_pWindowbkPanel;

	int m_nWndAnimationId;
	CAppModule *m_pModule;
	HICON m_DlgIcon;
};
