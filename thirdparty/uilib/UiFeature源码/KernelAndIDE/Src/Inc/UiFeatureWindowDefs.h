
#pragma once

// 取得控件的指针
#define GET_CONTROL(ctrlType,ctrlName)								dynamic_cast<ctrlType*>(m_pWindowBase->GetControl(ctrlName))
#define GET_CONTROL_EX(outCtrl,ctrlType,ctrlName)					outCtrl=GET_CONTROL(ctrlType,ctrlName)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 配合 BEGIN_CTRL_COMMAND 使用的宏
#define	WIN32_WND_CTRL_COMMAND			CUiFeatureWindow::OnCtrlMessage(pCtrl, nMsgId, wParam, lParam);

#define BEGIN_WNDPROC_MESSAGE																	\
public:																							\
	virtual LRESULT WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam){							\

// LRESULT OnBtnClick(WPARAM wParam, LPARAM lParam, BOOL &bContinue);
#define WNDPROC_MESSAGE_HANDLER(Msg_Id, Func_name)												\
	if (Msg_Id == nMsgId){BOOL bContinue = TRUE; LRESULT lRst = Func_name(wParam, lParam, bContinue); if(!bContinue) return lRst;}

#define END_WNDPROC_MESSAGE																		\
	return CUiFeatureWindow::WndProc(nMsgId, wParam, lParam);}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
