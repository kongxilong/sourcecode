
#pragma once
#include "..\..\Inc\Control\ICtrlEdit.h"
#include "..\..\Inc\IPropertyImage.h"
#include "EditWindow.h"

class ICtrlEditImpl : public ICtrlEdit
{
public:
	ICtrlEditImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlEditImpl();

	// 可见属性
	virtual void SetVisible(bool bVisible, bool bSetChild = false);

	virtual LPCWSTR GetEditText();
	virtual void SetEditText(LPCWSTR pszText);
	virtual void SetEditFocus();

	// 取得Edit托管窗口的句柄
	virtual HWND GetWindowHwnd();
	// 取得Edit窗口的句柄
	virtual HWND GetEditHwnd();
	virtual void EnableEditWindow(BOOL bEnable);
	virtual void SetReadOnly(BOOL bReadOnly);

	// 判断当前edit是否得到输入焦点
	virtual bool IsForegroundWindow();

	virtual void GetEditRemindText(CStringW &strText);
	virtual void SetEditRemindText(WCHAR *pszText);
	virtual void SetEditToRemindText();

	virtual void SetEditAutoVScroll(bool bVScroll);
	virtual void SetEditAutoHScroll(bool bHScroll);
	virtual void SetEditMultiLine(bool bMultiline = false);

protected:
	// 派生控件用于创建属于自己的控件属性
	// bIsNewCtrl：true时说明是Builder调用，新创建一个控件，需要初始化属性的各个默认值
	// false：说明整个皮肤包已经初始化完毕，属性创建完毕的时候，从xml中读取的属性值已经赋值完毕了，不能再初始化，否则xml中的值会被冲掉
	virtual bool CreateControlPropetry(bool bIsNewCtrl);
	// 初始化控件
	virtual void OnCreate();
	// 控件初始化完毕
	virtual void OnFinalCreate();
	// 销毁控件
	virtual void OnDestroy();
	// 绘制控件
	virtual void OnPaint(CDrawingBoard &DrawBoard);
	// 控件需要处理的一些自定义的消息，消息基本上会由对话框或者其他控件传入
	virtual int OnCtrlNotify(int nMsgId, WPARAM wParam, LPARAM lParam);
	// 移动、设置控件位置
	virtual void OnSize();
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

private:
	RECT GetEditWindowRect();

private:
	CEditWindow m_EditWnd;
	IPropertyImage* m_pPropEditBackgroundImg;
	IPropertyGroup* m_pPropMargins;
	IPropertyInt*	m_pPropLeft;
	IPropertyInt*	m_pPropTop;
	IPropertyInt*	m_pPropRight;
	IPropertyInt*	m_pPropBottom;
};
