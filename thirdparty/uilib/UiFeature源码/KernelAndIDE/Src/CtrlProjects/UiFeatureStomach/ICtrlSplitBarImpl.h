#pragma once
#include "..\..\Inc\Control\ICtrlSplitBar.h"

class ICtrlSplitBarImpl : public ICtrlSplitBar
{
public:
	ICtrlSplitBarImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlSplitBarImpl();

	BEGIN_CTRL_NOTIFY
		CTRL_NOTIFY_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
	END_CTRL_NOTIFY

	//设置分隔条控件的类型，bHorz为TRUE时表示水平分隔条，否则为垂直分隔条
	virtual void SetSplitterType(bool bHorz);

	//设置水平分隔条区域的最小值，nTopMin为顶部区域最小值,nBottomMin为底部区域的最小值
	virtual void SetSplitterHorzMin(int nTopMin, int nBottomMin);

	//设置垂直分隔条区域的最小值,nLeftMin为左侧区域最小值,nRightMin为右侧区域的最小值
	virtual void SetSplitterVertMin(int nLeftMin, int nRightMin);

	//获取左或者上最小值
	virtual int GetTopLeftMin();

	//获取底或者右最小值
	virtual int GetBottomRightMin();

	//获取图标的长度
	virtual int GetGraphLength();

	//设置图标的长度
	virtual void SetGraphLength(int nGraphLength, bool bRedraw);

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
	// 鼠标移动
	virtual void OnMouseMove(POINT pt);
	// 鼠标移出
	virtual void OnMouseLeave(POINT pt);
	// 鼠标左键点击
	virtual void OnLButtonDown(POINT pt);
	// 鼠标左键抬起
	virtual void OnLButtonUp(POINT pt);
	// 这个接口会在整个对话框初始化完成的时候调用
	// 控件通过派生这个接口，可以在这个接口函数里通过其他控件的名称取得当前对话框里的其他控件。
	virtual void OnWindowFinalCreate();
	// 设置控件光标
	virtual IPropertyCursor* OnSetCursor(POINT pt);
	// 移动、设置控件位置
	virtual void OnSize();

	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

	// 鼠标在控件内部拖动控件，这个消息即可以处理控件内部拖拽的动作，也可以处理不同控件之间拖拽实现数据传输
	virtual void OnMouseDragging(POINT pt);

	// 鼠标拖动操作结束，抬起鼠标。pDragCtrl：拖拽来源控件
	virtual void OnDragStop(IControlBase* pDragCtrl, POINT pt, WPARAM wParam, LPARAM lParam);

	int OnWindowPosChanged(WPARAM wParam, LPARAM lParam);

	static int FunComp(const void * p1,const void * p2);

private:
	typedef enum enumSplitBarState
	{
		SplitStateInvaild = -1,
		SplitStateNormal,
		SplitStateHot,
		SplitStatePressed,
		SplitStateDisabled,
		RadioStateNum
	}SPLITBARSTATE;

	struct PANEORDER
	{
		int idx;
		IControlBase *pPane;
		int pr;
		int min;
		int sz;
	};

private:
	void ChangeSplitState(SPLITBARSTATE state);
	char* w2c(char *pcstr,const wchar_t *pwstr, size_t len);
	void Relayout();

protected:

	SPLITBARSTATE m_enumSplitSate;
	ULONG				m_mouseFlag;
	IPropertyImage* m_pImage[RadioStateNum];		//背景图片
	IPropertyImage* m_pGraphic[RadioStateNum];		//图标

	//////////////////////////////////////////////////////////////////////////
	IPropertyBool* m_pIsHorz;				//是否是水平分隔条
	IPropertyInt*  m_pSplitSize;			//分隔条的宽度或者高度
	IPropertyInt*  m_pTopLeftMin;			//第一分割区域的最小值
	IPropertyInt*  m_pRightBottomMin;		//第二分割区域的最小值
	IPropertyString* m_pTopLeftName;		//第一分割区域的Panel名称
	IPropertyString* m_pRightBottomName;	//第二分割区域的Panel名称
	IPropertyInt*  m_pTopLeftSize;			//第一分割区域的宽度或者高度
	IPropertyInt*  m_pRightBottomSize;		//第二分割区域的宽度或者高度

	IPropertyInt*  m_pLTPriority;			//第一分隔区域优先级
	IPropertyInt*  m_pRBPriority;			//第二分隔区域优先级

	IPropertyInt*  m_pGraphLength;			//图标的长度

	//////////////////////////////////////////////////////////////////////////
	IPropertyCursor*  m_pCursor;		//拖拽时的光标,必须在m_bTracking的时候进行返回

protected:
	IControlBase* m_pTopLeftPanel;
	IControlBase* m_pRightBottomPanel;

private:
	INT m_nSplitPos;

	//是否在拖动中
	BOOL m_bTracking;

	//起始点击点
	POINT m_ptClick;

	int m_iDragBeam;

	BOOL m_bInSplit;
};