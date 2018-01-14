
#pragma once
#include "..\..\Inc\CWin32Window.h"
#include "..\..\Inc\IControlBase.h"
#include "..\..\Inc\IPropertyImage.h"
#include "..\..\Inc\CDrawingImage.h"
#include <atlstr.h>
using namespace ATL;

// 创建EDIT消息
#define CREATE_EDIT_MSG							(WM_APP + 0x0100)
// 激活edit，得到输入焦点
#define CREATE_ACTIVE_EDIT_MSG					(CREATE_EDIT_MSG + 1)
#define	WM_CAL_SCROLL_BAR						(WM_USER + 0X32)

class CEditWindow : public CWin32Window
{
public:
	CEditWindow();
	virtual ~CEditWindow();

	// 创建Edit窗口的属性，在控件的 CreateControlPropetry 函数中调用
	bool CreateEditPropetry(IControlBase* pOwnerCtrl, bool bIsNewCtrl);
	// 创建EDIT窗口，在控件的 OnCreate 函数中调用
	bool CreateEditWindow(RECT ctrlRectInOwner, int nShow);
	// EDIT窗口自定义消息处理，在控件 OnCtrlNotify 函数中调用
	void OnEditNotify(int nMsgId, WPARAM wParam, LPARAM lParam);

	void GetEditText(CStringW &strText);
	void SetEditText(LPCWSTR pszText);
	void GetEditRemindText(CStringW &strText);
	void SetEditRemindText(WCHAR *pszText);
	void SetEditToRemindText();
	void SetEditAutoVScroll(bool bVScroll);
	void SetEditAutoHScroll(bool bHScroll);
	void SetEditMultiLine(bool bMultiline = false);
	// 激活edit，让edit得到输入焦点
	void PostActiveMessage();
	// 判断当前edit是否得到输入焦点
	bool IsForegroundWindow();

	virtual void SetRectInOwner(RECT ctrlRectInOwner);

	// 取得Edit托管窗口的句柄
	HWND GetWindowHwnd();
	// 取得Edit窗口的句柄
	HWND GetEditHwnd();
	void EnableEditWindow(BOOL bEnable);
	void SetReadOnly(BOOL bReadOnly);

protected:
	// 本窗口的消息处理函数
	virtual LRESULT WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam);
	virtual bool CreateWindowWithNewThread(HWND hParent, RECT WndRect, char *pszWndText, int nShow, int nStyle = WS_OVERLAPPED | WS_THICKFRAME, LPARAM lParam = NULL);
	virtual bool CreateWindowWithoutThread(HWND hParent, RECT WndRect, char *pszWndText, int nShow, int nStyle = WS_OVERLAPPED | WS_THICKFRAME, LPARAM lParam = NULL);
	virtual void OnCreate();
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnKeyDown(int nVirtKey, int nFlag);
	// 系统消息，返回true说明不需要返回给调用继续调用
	virtual bool OnEditSysCommand(WPARAM wParam, LPARAM lParam);

private:
	// 窗口上的EDIT控件的背景设置
	LRESULT OnCtlColorEdit(WPARAM wParam, LPARAM lParam);
	// 只读状态下的背景、字体、颜色设置
	LRESULT OnCtlColorStatic(WPARAM wParam, LPARAM lParam);
	void OnEditCreateMesage();
	bool OnEditEraseBkgnd(HDC hDc);
	void SetEditFocus();
	int GetColorValue(IPropertyInt* pPropColor);
	void SetEditFont(HFONT hFont);

public:
	LRESULT EditProc(UINT nMsgId, WPARAM wParam, LPARAM lParam);

private:
	IWindowBase* m_pWindowBase;
	CDrawingImage m_EditBkMemDc;
	IControlBase* m_pOwnerCtrl;
	HWND m_hEdit;
	WNDPROC m_defEditWndProc;
	RECT m_CtrlRectInOwner;
	int m_nShow;

	HBRUSH m_hReadOnlyColorBrush;
	HBRUSH m_hColorBrush;
	HFONT m_hEditRemindFont;

	HFONT m_hEditFont;
	IPropertyGroup* m_pPropEditGroup;
	IPropertyImage* m_pPropEditBkImg;
	IPropertyFont* m_pPropEditFont;
	IPropertyString* m_pPropEditRemindInfo;
	IPropertyFont* m_pPropEditRemindFont;
	IPropertyBool* m_pPropReadOnly;

	IPropertyGroup* m_pPropBkColorGroup;
	IPropertyBool* m_pPropShowBkColor;
	IPropertyInt* m_pPropR;
	IPropertyInt* m_pPropG;
	IPropertyInt* m_pPropB;


	IPropertyFont* m_pPropReadOnlyEditFont;
	IPropertyGroup* m_pPropReadOnlyBkColorGroup;
	IPropertyBool* m_pPropReadOnlyShowBkColor;
	IPropertyInt* m_pPropReadOnlyR;
	IPropertyInt* m_pPropReadOnlyG;
	IPropertyInt* m_pPropReadOnlyB;

	RECT m_OldEditRct;
	DWORD		 m_dwStyle;
	int  m_nMaxLineCount;
	BOOL m_isReminding;
};
