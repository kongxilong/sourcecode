
#pragma once
#include "..\..\Inc\Control\ICtrlRichEdit.h"
#include "RichEdit/IUiFeatureTextHost.h"

class ICtrlRichEditImpl : public ICtrlRichEdit, public CHostCallBack
{
public:
	ICtrlRichEditImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlRichEditImpl();

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
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);
	// 移动、设置控件位置
	virtual void OnSize();
	// 控件取得焦点，通过Tab键跳转，激活控件
	virtual void OnSetFocus();
	// 控件失去焦点
	virtual void OnKillFocus();
	// 控件需要处理的一些自定义的消息，消息基本上会由对话框或者其他控件传入
	virtual int OnCtrlNotify(int nMsgId, WPARAM wParam, LPARAM lParam);

	//@cmember Notify host of events
	virtual HRESULT TxNotify(DWORD iNotify, void *pv);

protected:
	IUiFeatureTextHost* m_pTextHost;
};
