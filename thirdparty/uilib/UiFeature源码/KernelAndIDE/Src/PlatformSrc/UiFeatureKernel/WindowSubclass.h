
#pragma once
#include "..\..\Inc\IWindowBase.h"
#include "..\..\Inc\CDWORDMap.h"

// 被子类化的窗口map
class SUBCLASS_FUNCTION
{
public:
	SUBCLASS_FUNCTION();
	~SUBCLASS_FUNCTION();

	IWindowBase* pWndBase;
	WNDPROC pOldWndProc;
};
// key:HWND
// VOID*:SUBCLASS_FUNCTION*
typedef CDWORDMap		SUB_CLASS_WINDOW_MAP;

class CWindowSubclass
{
public:
	CWindowSubclass();
	virtual ~CWindowSubclass();

	static CWindowSubclass* GetInstance();

	SUBCLASS_FUNCTION* FindSubWindow(HWND hWnd);
	void EraseSubWindow(HWND hWnd);
	bool SubWindow(HWND hWnd, IWindowBase* pWndBase);

protected:
	static LRESULT CALLBACK WindowSubProc(HWND hWnd, UINT nMsgId, WPARAM wParam, LPARAM lParam);

private:
	// 窗口队列
	SUB_CLASS_WINDOW_MAP m_SubClsWndMap;
};
