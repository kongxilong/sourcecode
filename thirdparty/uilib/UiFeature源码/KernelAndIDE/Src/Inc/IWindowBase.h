
#pragma once
#include "IControlBase.h"
#include "IFeatureObject.h"
#include "IPropertyWindow.h"
#include "IPropertyCursor.h"


class IUiFeatureKernel;
class _declspec(novtable) IWindowBase : public IFeatureObject
{
	friend class IControlBase;
	friend class IUiFeatureKernelImpl;
	friend class IWindowBaseImpl;
	friend class CUiFeatureBuilderView;
	friend class CPropertyViewCtrl;
	friend class CWindowsViewTree;

public:
	IWindowBase(IUiFeatureKernel * pUiKernel);
	// 是否为设计模式
	virtual bool IsDesignMode() = 0;
	// new child 在一个控件内部创建子控件，需要外部一个固定的Name传入pszNewCtrlObjectName参数
	// 在同一个 pParentCtrl 的控件下创建子控件，nSubCtrlId 不能重复，否则，在Clone控件的时候会发生异常
	virtual IControlBase* CreateSubControl(IControlMessage* pCtrlMsg, IControlBase* pParentCtrl, LPCWSTR pszNewCtrlTypeName, LPCWSTR pszNewCtrlObjectName, bool bIsCreate, int nSubCtrlId) = 0;
	// 删除一个子控件
	virtual bool DeleteSubControl(IControlBase* pDelCtrl) = 0;

//////////////////////////////////////////////////////////////////////////
	// 窗口居中显示
	virtual void CenterWindow() = 0;
	// 取得窗口句柄
	virtual HWND GetSafeHandle() = 0;
	// 得到窗口屏幕坐标
	virtual RECT GetWindowRect() = 0;
	// 得到客户端坐标，[0,0]坐标系
	virtual RECT GetClientRect() = 0;
	// 停止绘制窗口
	virtual void StopDrawWindow(bool bStop) = 0;
	// 移动窗口
	virtual void MoveWindow(RECT toWndRct, bool bRedraw = true) = 0;

	// 根据控件布局信息设置控件的位置
	virtual void SetControlWindowLayoutPostion(IControlBase* pCtrl, RECT ParentRctInWnd) = 0;

//////////////////////////////////////////////////////////////////////////
	// 取得窗口控件指针
	virtual IControlBase* GetControl(char *pszCtrlName) = 0;
	virtual IControlBase* GetControl(LPCWSTR pszCtrlName) = 0;
	virtual IControlBase* GetControl(WCHAR *pszCtrlName) = 0;
	virtual IControlBase* GetControlByObjectId(DWORD dwObjId) = 0;
	// 向队列未插入一个控件
	virtual void AppendChildContrl(IControlBase *pCtrl) = 0;
	// 取得子控件列表
	virtual CHILD_CTRLS_VEC* GetChildControlsVec() = 0;
	// 取得子控件的属性列表：Layout.xml 中的布局
	virtual PROP_CONTROL_VEC* GetChildPropControlVec() = 0;

	virtual void InvalidateRect(RECT *lpRect) = 0;
	virtual void UpdateWindow() = 0;
	// 立即重绘窗口，只有需要重绘的控件才重绘
	virtual void RedrawWindow(RECT *pDrawRect = NULL) = 0;
	// 使用发消息的方式重绘窗口
	virtual void RedrawWindowInNextMessage() = 0;
	// 强迫立刻重绘窗口，强迫所有控件均重绘
	virtual void CompelRedrawWindow(RECT *pDrawRect = NULL) = 0;
	// 设置轻量级通用动画定时器
	virtual void SetAnimationTimer() = 0;
	// 设置普通定时器
	virtual int SetTimer(DWORD uElapse) = 0;
	virtual void KillTimer(int nId) = 0;
	// 设置高精度定时器
	virtual int SetHighAccuracyTimer(DWORD uElapse) = 0;
	virtual void KillHighAccuracyTimer(int nId) = 0;

	// 取得整个程序默认的字体信息
	virtual IPropertyFontBase* GetDefaultFontBase() = 0;
	// 设置控件回调函数
	virtual void SetCtrlMsgCallBack(IControlMessage* pCtrlMsg) = 0;
	// 控件消息回调函数
	virtual LRESULT OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam) = 0;

	// 向内核注册一个想要取到的消息：异步处理方式，一般都采用这个处理方式，特殊情况采用同步处理方式
	virtual void RegisterControlMessage(IControlBase* pCtrl, int nMsgId) = 0;
	virtual void UnRegisterControlMessage(IControlBase* pCtrl, int nMsgId) = 0;

	// 向内核注册一个想要取到的消息：同步处理方式，控件请不要再自己注册了的消息的处理过程中执行同步处理
	virtual void RegisterControlMessageSync(IControlBase* pCtrl, int nMsgId) = 0;
	virtual void UnRegisterControlMessageSync(IControlBase* pCtrl, int nMsgId) = 0;

	// 发送消息:Send方式
	virtual LRESULT SendMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam) = 0;
	// 发送消息:Post方式
	virtual bool PostMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam) = 0;

	// 得到当前窗口的皮肤包数据
	virtual SKIN_FILE_ITEM* GetSkinFileItem() = 0;
	// 显示tips
	// pszTips：tip文字
	// ShowPt：开始显示的位置
	// bBaloon：是否为气泡样式
	// nShowTime：显示的时间，单位：秒
	virtual bool ShowSystemTips(LPCWSTR pszTips, bool bBaloon, int nShowTime) = 0;
	virtual bool DestroySystemTips() = 0;
	// 取得默认的回车键接受控件
	virtual IControlBase* GetDefaultEnterCtrl(CHILD_CTRLS_VEC* pChildVec) = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 窗口属性相关
	virtual IPropertyGroup *PP_GetWindowPropetryGroup() = 0;
	virtual IPropertyWindow *PP_GetWindowPropetry() = 0;

	// 是否支持全窗口点击移动
	virtual void PP_SetDragWindow(bool bDrag) = 0;
	virtual bool PP_GetDragWindow() = 0;
	// 窗口是否设置了最大、最小尺寸
	virtual bool PP_IsSetMinMaxInfo() = 0;
	// 窗口是否可以拖拽四边、四角进行拉伸
	virtual bool PP_IsCanStretchWindow() = 0;
	// 最大化的时候窗口是否可以自由拖动
	virtual void SetMaximizeWndDragMove(bool bMove) = 0;
	virtual bool GetMaximizeWndDragMove() = 0;

	// 设置窗体的透明特性、设置窗口是否支持分层
	virtual void PP_SetLayeredWindow(bool bIsLayered) = 0;
	virtual bool PP_GetLayeredWindow() = 0;

	// 是否最大化
	virtual void SetFullScreen(bool bFull) = 0;
	virtual bool IsFullScreen() = 0;

	// 是否窗口置顶
	virtual bool IsTopmost() = 0;

	// 窗口Object名称
	virtual void PP_SetWindowObjectName(LPCWSTR pszWndName) = 0;
	virtual LPCWSTR PP_GetWindowObjectName() = 0;

	// 窗口名称
	virtual void PP_SetWindowText(LPCWSTR pszWndName) = 0;
	virtual LPCWSTR PP_GetWindowText() = 0;

	// 取得窗口中设置属性的窗口大小
	virtual SIZE PP_GetWindowPropSize() = 0;

	// 是否使用临时内存绘制
	virtual bool PP_IsUseTempDrawMem() = 0;

	// 滚动条消息
	virtual void OnVScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar) = 0;
	virtual void OnHScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar) = 0;
	// 添加一个控件动画
	virtual bool AppendAnimationControl(IControlBase* pCtrl) = 0;
	// 开始控件动画
	virtual void DoControlAnimation() = 0;
	// 清空动画资源
	virtual void ClearAnimation() = 0;
	// 将窗口图片输出到一个目标画板上
	virtual void PrintWindow(CDrawingImage &ToDrawDc) = 0;

////// 以下函数非外部窗口调用 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 非外部窗口调用：本窗口的消息处理函数，bPassOn参数为true是，消息会继续传递处理；false时，处理完毕，不再下传
	virtual LRESULT WindowProc(UINT nMsgId, WPARAM wParam, LPARAM lParam, bool &bPassOn) = 0;

	// 清空绘制缓存
	virtual void ClearDrawMemoryDc() = 0;

	// 注册焦点控件，用于绘制用户自定义的焦点状态，当一个可以取得焦点的控件取得焦点时
	// 被注册的焦点控件就会收到焦点改变的消息
	virtual void RegisterFocusControl(IControlBase* pFocusCtrl) = 0;

	// 取得当前的焦点控件
	virtual IControlBase* GetFocusControl() = 0;
	// 取得当前tab停留的控件
	virtual IControlBase* GetCurrentTabControl() = 0;

	// 注册一个需要得到窗口所有消息的控件，这个控件在取得焦点之后会收到所有窗口消息
	virtual void RegisterWindowMessageControl(IControlBase* pCtrl) = 0;

protected:
	// 非外部窗口调用：初始化子类化窗口
	virtual bool PG_InitWindowBase(HWND hWnd, LPCWSTR pszSkinPath, LPCWSTR pszWndName) = 0;
	
//////////////////////////////////////////////////////////////////////////
	// Builder 使用的函数
	virtual void BD_InitWindowBase(LPCWSTR pszSkinPath, IPropertyWindow *pWindowProp, bool bSetDftProp) = 0;
	virtual void BD_DrawWindowView(CDrawingBoard &ViewMemDc) = 0;
	// 得到在Builder中显示的四周8个方块的位置
	virtual FANGKUAI_8* BD_GetFangKuai8Rect() = 0;
	// 取得当前输入位置的控件
	virtual IControlBase *BD_GetMouseInControl(POINT pt) = 0;
	// 取得当前激活的控件
	virtual IControlBase *BD_GetFocusControl() = 0;
	// 设置当前激活的控件
	virtual void BD_SetFocusControl(IControlBase* pControl) = 0;
	// 设置控件在编辑模式下的位置和大小
	virtual void BD_SetControlRect(IControlBase* pControl, RECT RctInView) = 0;
	// 初始化所有控件在Builder中的显示位置
	virtual void BD_SetAllCtrlRectInView() = 0;
	// 设置指定控件在Builder中的显示位置
	virtual void BD_SetControlRectInView(CHILD_CTRLS_VEC *pChildCtrlsVec) = 0;
	// Builder有属性修改，重新刷新界面
	virtual void BD_RefreshWindowPropetry() = 0;
	// 重新计算子控件的位置和大小
	virtual void ResetChildCtrlPostion(CHILD_CTRLS_VEC* pChildVec, bool bMemToProp = false) = 0;

	// 添加一个控件动画
	virtual bool AppendAnimation(IControlBase* pCtrl, CDrawingImage& CtrlAnimationMemDc,
		DWORD animType, int nFrameTime, int nAnimationDirection) = 0;
};
