
#pragma once
#include "..\..\Inc\Control\ICtrlShadowButton.h"
#include "..\..\Inc\IControlBase.h"

enum BUTTON_TYPE
{
	// 普通按钮
	BT_NORMAL_BUTTON = 0,
	// rgn 按钮
	BT_RGN_BUTTON,
	// tab用按钮
	BT_CHECK_BUTTON,
	// switch 按钮
	BT_SWITCH_BUTTON,
};

class ICtrlShadowButtonImpl : public ICtrlShadowButton
{
public:
	ICtrlShadowButtonImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlShadowButtonImpl();

	//设置Button文言
	virtual	void SetButtonText(LPCWSTR pszText);
	//获取Button文言
	virtual	LPCWSTR GetButtonText();

	// check button 模式下
	virtual void SetCheck(bool bIsCheck, bool bRedraw);
	virtual bool IsCheck();

	virtual IControlBase* GetCheckPanel();

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
	// 鼠标进入
	virtual void OnMouseEnter(POINT pt);
	// 鼠标移出
	virtual void OnMouseLeave(POINT pt);
	// 鼠标左键点击
	virtual void OnLButtonDown(POINT pt);
	// 鼠标左键双击
	virtual void OnLButtonDbClick(POINT pt);
	// 鼠标左键抬起
	virtual void OnLButtonUp(POINT pt);
	// 鼠标右键点击
	virtual void OnRButtonDown(POINT pt);
	// 控件取得焦点，通过Tab键跳转，激活控件
	virtual void OnSetFocus();
	// 控件失去焦点
	virtual void OnKillFocus();
	// 鼠标是否落在了控件指定的RGN区域，默认返回true，整个控件就是RGN区域
	virtual bool OnCheckMouseInRgn(POINT pt, int nMouseMsgId);
	// Builder刷新属性，此函数中只要重新加载控件属性即可
	virtual void OnBuilderRefreshProp(IPropertyBase* pPropBase);
	// 这个接口会在整个对话框初始化完成的时候调用
	// 控件通过派生这个接口，可以在这个接口函数里通过其他控件的名称取得当前对话框里的其他控件。
	virtual void OnWindowFinalCreate();
	// 设置控件光标
	virtual IPropertyCursor* OnSetCursor(POINT pt);
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

private:
	// 根据根据图片设置RGN
	bool SetRgn();
	int GetTextDownOffset();

private:
	typedef enum enumBtnState
	{
		BtnStateInvalid = -1,
		BtnStateNormal,
		BtnStateHover,
		BtnStateDown,
		BtnStateFocus,
		BtnStateDisable,
		//在此之前增加Button的状态
		BtnStateNum
	}BTNSTATE;

private:
	void ChangeButtonStyle(BTNSTATE state);

private:
	// check button 模式下，记录是否check了
	bool				m_bIsCheck;
	IPropertyComboBox*	m_pPropButtonTypeCombo;

	IPropertyImage*		m_pPropImages[BtnStateNum];
	IPropertyFont*		m_pPropFonts[BtnStateNum];
	IPropertyString*	m_pPropText; 
	IPropertyInt*		m_pDownTextOffset;

	IPropertyGroup*		m_pPropRgnGroup;
	IPropertyImage*		m_pPropRgnMaskImage;
	HRGN				m_hRgn;

	IPropertyGroup*		m_pPropCheckBtnGroup;
	IPropertyString*	m_pPropShowPanelName;
	IPropertyCursor*	m_pPropCursor;

	BTNSTATE			m_enumBtnState;
	ULONG				m_mouseFlag;

	IControlBase*		m_pCheckPanel;
};
