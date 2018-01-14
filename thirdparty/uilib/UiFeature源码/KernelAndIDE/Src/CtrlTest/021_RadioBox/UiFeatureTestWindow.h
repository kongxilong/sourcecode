#pragma once
#include "..\..\Inc\CUiFeatureWindow.h"
#include "..\..\Inc\Control\ICtrlImageBox.h"
#include "..\..\Inc\Control\ICtrlShadowButton.h"
#include "..\..\Inc\Control\ICtrlRadioBox.h"
#include "..\..\Inc\Control\ICtrlStatic.h"

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
		CTRL_COMMAND_HANDLER(m_pCheckRadio, CM_RADIO_BOX_UP, OnCheckRadioClick)
		CTRL_COMMAND_HANDLER(m_pCockRadio, CM_RADIO_BOX_UP, OnCockRadioClick)
		CTRL_COMMAND_HANDLER(m_pClearRadio, CM_RADIO_BOX_UP, OnClearRadioClick)
		CTRL_COMMAND_HANDLER(m_pRehabRadio, CM_RADIO_BOX_UP, OnRehabRadioClick)
		CTRL_COMMAND_HANDLER(m_pRepairRadio, CM_RADIO_BOX_UP, OnRepairRadioClick)
		CTRL_COMMAND_HANDLER(m_pPcClear, CM_RADIO_BOX_UP, OnPcClearClick)
		CTRL_COMMAND_HANDLER(m_pFunctionRadio, CM_RADIO_BOX_UP, OnFunctionRadioClick)
		CTRL_COMMAND_HANDLER(m_pSoftwareRadio, CM_RADIO_BOX_UP, OnSoftwareRadioClick)

		CTRL_COMMAND_HANDLER(m_pRadio3, CM_RADIO_BOX_UP, OnRadio3Click)
		CTRL_COMMAND_HANDLER(m_pRadio2, CM_RADIO_BOX_UP, OnRadio2Click)
		KERNEL_COMMAND_HANDLER(KCM_SYS_COMMAND_CLOSE, OnSysCommandClose)
	END_CTRL_COMMAND

protected:
	// 本窗口的消息处理函数
	virtual LRESULT WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam);
	// WM_CREATE：对话框初始化	
	virtual void OnCreate();

	virtual void OnKeyDown(int nVirtKey, int nFlag);

	LRESULT OnCheckRadioClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnCockRadioClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnClearRadioClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnRehabRadioClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnRepairRadioClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnPcClearClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnFunctionRadioClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnSoftwareRadioClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnRadio3Click(WPARAM wParam, LPARAM lParam);
	LRESULT OnRadio2Click(WPARAM wParam, LPARAM lParam);
	LRESULT OnSysCommandClose(WPARAM wParam, LPARAM lParam);

private:
	DWORD m_dwMainThreadId;
	ICtrlRadioBox* m_pCheckRadio;
	ICtrlRadioBox* m_pCockRadio;
	ICtrlRadioBox* m_pClearRadio;
	ICtrlRadioBox* m_pRehabRadio;
	ICtrlRadioBox* m_pRepairRadio;
	ICtrlRadioBox* m_pPcClear;
	ICtrlRadioBox* m_pFunctionRadio;
	ICtrlRadioBox* m_pSoftwareRadio;
	ICtrlStatic* m_pshowTip;
	ICtrlStatic* m_pshowTip2;

	ICtrlRadioBox* m_pRadio3;
	ICtrlRadioBox* m_pRadio2;
};
