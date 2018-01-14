
#pragma once
#include "..\..\Inc\IWindowBase.h"
#include "..\..\Inc\IPropertyGroup.h"
#include "..\..\Inc\IPropertyString.h"
#include "..\..\Inc\IPropertyBool.h"
#include "..\..\Inc\IPropertyInt.h"
#include "..\..\Inc\IPropertySkinManager.h"
#include "..\..\Inc\CHighAccuracyTimer.h"
#include "..\..\Inc\IUIEffect.h"
#include "WindowResize.h"
#include "IPropertyImageBaseImpl.h"
#include "SystemTips.h"
#include <atlstr.h>
using namespace ATL;


// 控件注册需要取得的消息
typedef vector<IControlBase*>	REG_CTRL_VEC;
typedef map<int, REG_CTRL_VEC>	REG_MSG_MAP;

class IWindowBaseImpl : public IWindowBase, public IUIEffectCallBack
{
public:
	IWindowBaseImpl(IUiFeatureKernel * pUiKernel);
	virtual ~IWindowBaseImpl();

//////////////////////////////////////////////////////////////////////////
	// Builder 使用的函数
	virtual void BD_InitWindowBase(LPCWSTR pszSkinPath, IPropertyWindow *pWindowProp, bool bSetDftProp);
	virtual void BD_DrawWindowView(CDrawingBoard &ViewMemDc);
	// 得到在Builder中显示的四周8个方块的位置
	virtual FANGKUAI_8* BD_GetFangKuai8Rect();
	// 取得当前输入位置的控件
	virtual IControlBase *BD_GetMouseInControl(POINT pt);
	// 取得当前激活的控件
	virtual IControlBase *BD_GetFocusControl();
	// 设置当前激活的控件
	virtual void BD_SetFocusControl(IControlBase* pControl);
	// 设置控件在编辑模式下的位置和大小
	virtual void BD_SetControlRect(IControlBase* pControl, RECT RctInView);
	// 初始化所有控件在Builder中的显示位置
	virtual void BD_SetAllCtrlRectInView();
	// 设置指定控件在Builder中的显示位置
	virtual void BD_SetControlRectInView(CHILD_CTRLS_VEC *pChildCtrlsVec);
	// Builder有属性修改，重新刷新界面
	virtual void BD_RefreshWindowPropetry();
	// 重新计算子控件的位置和大小，参数：bMemToProp 是否记录到属性中，Builder时为true
	virtual void ResetChildCtrlPostion(CHILD_CTRLS_VEC* pChildVec, bool bMemToProp = false);
	// 是否为设计模式
	virtual bool IsDesignMode();
	// new child 在一个控件内部创建子控件，需要外部一个固定的Name传入pszNewCtrlObjectName参数
	// 在同一个 pParentCtrl 的控件下创建子控件，nSubCtrlId 不能重复，否则，在Clone控件的时候会发生异常
	virtual IControlBase* CreateSubControl(IControlMessage* pCtrlMsg, IControlBase* pParentCtrl, LPCWSTR pszNewCtrlTypeName, LPCWSTR pszNewCtrlObjectName, bool bIsCreate, int nSubCtrlId);
	// 删除一个子控件
	virtual bool DeleteSubControl(IControlBase* pDelCtrl);

//////////////////////////////////////////////////////////////////////////
	// 导入皮肤包使用的函数初始化
	virtual bool PG_InitWindowBase(HWND hWnd, LPCWSTR pszSkinPath, LPCWSTR pszWndName);

	// 窗口居中显示
	virtual void CenterWindow();
	// 取得窗口句柄
	virtual HWND GetSafeHandle();
	// 得到窗口屏幕坐标
	virtual RECT GetWindowRect();
	// 得到客户端坐标，[0,0]坐标系
	virtual RECT GetClientRect();
	// 停止绘制窗口
	virtual void StopDrawWindow(bool bStop);
	// 移动窗口
	virtual void MoveWindow(RECT toWndRct, bool bRedraw = true);

	// 根据控件布局信息设置控件的位置
	void SetControlWindowLayoutPostion(IControlBase* pCtrl, RECT ParentRctInWnd);
//////////////////////////////////////////////////////////////////////////
	// 取得窗口控件指针
	virtual IControlBase* GetControl(char *pszCtrlName);
	virtual IControlBase* GetControl(LPCWSTR pszCtrlName);
	virtual IControlBase* GetControl(WCHAR *pszCtrlName);
	virtual IControlBase* GetControlByObjectId(DWORD dwObjId);
	// 向队列未插入一个控件
	virtual void AppendChildContrl(IControlBase *pCtrl);
	// 取得子控件列表
	virtual CHILD_CTRLS_VEC* GetChildControlsVec();
	// 取得子控件的属性列表：Layout.xml 中的布局
	virtual PROP_CONTROL_VEC* GetChildPropControlVec();

	// 得到当前窗口的皮肤包数据
	virtual SKIN_FILE_ITEM* GetSkinFileItem();

	// 设置轻量级通用动画定时器
	virtual void SetAnimationTimer();
	// 设置普通定时器
	virtual int SetTimer(DWORD uElapse);
	virtual void KillTimer(int nId);
	// 设置高精度定时器
	virtual int SetHighAccuracyTimer(DWORD uElapse);
	virtual void KillHighAccuracyTimer(int nId);

	void InvalidateRect(RECT *lpRect);
	void UpdateWindow();
	// 立即重绘窗口
	virtual void RedrawWindow(RECT *pDrawRect = NULL);
	// 使用发消息的方式重绘窗口
	virtual void RedrawWindowInNextMessage();
	// 强迫立刻重绘窗口，强迫所有控件均重绘
	virtual void CompelRedrawWindow(RECT *pDrawRect = NULL);

	// 显示自定义光标
	void SetWindowCursor(int nCursor);

	// 取得整个程序默认的字体信息
	virtual IPropertyFontBase* GetDefaultFontBase();

	// 设置控件回调函数
	virtual void SetCtrlMsgCallBack(IControlMessage* pCtrlMsg);
	// 控件消息回调函数
	virtual LRESULT OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam);

	// 向内核注册一个想要取到的消息：异步处理方式，一般都采用这个处理方式，特殊情况采用同步处理方式
	virtual void RegisterControlMessage(IControlBase* pCtrl, int nMsgId);
	virtual void UnRegisterControlMessage(IControlBase* pCtrl, int nMsgId);

	// 向内核注册一个想要取到的消息：同步处理方式，控件请不要再自己注册了的消息的处理过程中执行同步处理
	virtual void RegisterControlMessageSync(IControlBase* pCtrl, int nMsgId);
	virtual void UnRegisterControlMessageSync(IControlBase* pCtrl, int nMsgId);

	// 发送消息:Send方式
	virtual LRESULT SendMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam);
	// 发送消息:Post方式
	virtual bool PostMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam);

	// 滚动条消息
	virtual void OnVScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar);
	virtual void OnHScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar);
	// 添加一个控件动画
	virtual bool AppendAnimationControl(IControlBase* pCtrl);
	// 开始控件动画
	virtual void DoControlAnimation();
	// 清空动画资源
	virtual void ClearAnimation();
	// 清空绘制缓存
	virtual void ClearDrawMemoryDc();
	// 将窗口图片输出到一个目标画板上
	virtual void PrintWindow(CDrawingImage &ToDrawDc);

	// 显示tips
	// pszTips：tip文字
	// ShowPt：开始显示的位置
	// bBaloon：是否为气泡样式
	virtual bool ShowSystemTips(LPCWSTR pszTips, bool bBaloon, int nShowTime);
	virtual bool DestroySystemTips();

	// 注册焦点控件，用于绘制用户自定义的焦点状态，当一个可以取得焦点的控件取得焦点时
	// 被注册的焦点控件就会收到焦点改变的消息
	virtual void RegisterFocusControl(IControlBase* pFocusCtrl);

	// 取得当前的焦点控件
	virtual IControlBase* GetFocusControl();
	// 取得当前tab停留的控件
	virtual IControlBase* GetCurrentTabControl();

	// 窗口是否设置了最大、最小尺寸
	virtual bool PP_IsSetMinMaxInfo();
	// 窗口是否可以拖拽四边、四角进行拉伸
	virtual bool PP_IsCanStretchWindow();
	// 最大化的时候窗口是否可以自由拖动
	virtual void SetMaximizeWndDragMove(bool bMove);
	virtual bool GetMaximizeWndDragMove();
	// 取得默认的回车键接受控件
	virtual IControlBase* GetDefaultEnterCtrl(CHILD_CTRLS_VEC* pChildVec);

	// 注册一个需要得到窗口所有消息的控件，这个控件在取得焦点之后会收到所有窗口消息
	virtual void RegisterWindowMessageControl(IControlBase* pCtrl);

protected:
	// 本窗口的消息处理函数，bPassOn参数为true是，消息会继续传递处理；false时，处理完毕，不再下传
	virtual LRESULT WindowProc(UINT nMsgId, WPARAM wParam, LPARAM lParam, bool &bPassOn);

	void OnCreate();
	void OnTimer(UINT nTimerId);
	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	void OnKeyUp(WPARAM wParam, LPARAM lParam);
	void OnChar(WPARAM wParam, LPARAM lParam);
	// 系统消息，返回true说明不需要返回给调用继续调用
	void OnSysCommand(WPARAM wParam, LPARAM lParam);
	void OnKillFocus(WPARAM wParam, LPARAM lParam);
	void OnSetFocus(WPARAM wParam, LPARAM lParam);
	void OnDestroy();
	void OnClose();
	void OnSize(UINT nType, int cx, int cy);
	void OnSizing(UINT nSide, LPRECT lpRect);
	// WM_ENTERSIZEMOVE：进入移动、拉伸窗口操作
	void OnEnterSizeMove();
	// WM_EXITSIZEMOVE：退出移动、拉伸窗口操作
	void OnExitSizeMove();
	void OnPaint(HDC hWndDc, RECT *pLayeredRect = NULL);
	// 设置鼠标样式，返回true表示设置了自定义的鼠标手势，false为需要使用系统默认的手势
	bool OnSetCursor(HWND hWnd, int nHitTest, int nMsgId);
	LRESULT OnNcHitTest(int nX, int nY);
	// 窗口尺寸最大值最小值设置
	bool OnGetMinMaxInfo(MINMAXINFO *pMinMaxInfo);

	void OnMouseWheel(WPARAM wParam, LPARAM lParam);
	void OnMouseLeave(WPARAM wParam, LPARAM lParam);
	void OnMouseMove(int nVirtKey, POINT pt);
	void OnLButtonDown(int nVirtKey, POINT pt);
	void OnLButtonUp(int nVirtKey, POINT pt);
	void OnLButtonDbClick(int nVirtKey, POINT pt);
	void OnRButtonDown(int nVirtKey, POINT pt);

	// 显示窗口
	void OnShowWindow(bool bIsShow, int nShowStatus);
	// 窗口需要tab切换
	void OnTabChange();

private:
	// 内部接受到初始化消息，开始初始化窗口，加载皮肤
	bool OnInitWindowBase();
	// 初始化对话框、加载窗口皮肤完成，在分层窗口模式下，初始化窗口后，显示窗口
	void OnInitWindowBaseEnd();
	bool IsInit();
	// 创建控件
	bool CreateChildCtrlVec(IControlBase *pParentCtrl, PROP_CONTROL_VEC* pPropCtrlVec, CHILD_CTRLS_VEC* pCtrlVec);

	// 在Builder中绘制用的边框色块
	void BD_NewFrameImage();
	void BD_NewFrameImageBase(IPropertyImageBase *pImgBase, LPCWSTR strImgPath);
	bool BD_CheckMouseInControl(CHILD_CTRLS_VEC *pCtrlVec, POINT pt, IControlBase** ppControl);

	// 从相对窗口的鼠标相对坐标得到相对控件的鼠标坐标
	POINT GetCtrlMouseOffset(POINT WndPt, IControlBase* pCtrl);
	// 取得桌面工作区域大小
	RECT GetWorkAreaRect();
	// 强迫子控件重绘
	void CompelRedrawControl(CHILD_CTRLS_VEC *pCtrlVec);
	// 清空子控件绘制缓存
	void ClearCtrlDrawMemoryDc(CHILD_CTRLS_VEC *pCtrlVec);

private:
	// 循环遍历每个控件的绘制
	void DrawControl(CDrawingImage &ToDrawDc);
	// 当窗口的属性发生变化时需要通知窗口进行刷新时调用
	void RefreshWindowStyle();
	// 取得当前鼠标坐标落在了哪个控件上
	bool CheckMouseInControl(CHILD_CTRLS_VEC *pCtrlVec, POINT pt, IControlBase **ppControl, int nMouseMsgId);
	// 设置窗口中所有控件的位置和大小
	void SetControlPostion(CHILD_CTRLS_VEC *pCtrlVec, SIZE NewSize);
	// 设置当前得到焦点的控件，如果参数为NULL，说明当前没有控件得到焦点，所有控件都失去焦点
	void SetFocusCtrl(IControlBase *pControl);
	// 在分层窗口模式下拉伸窗口大小
	void ResizeInLayeredWindow(RECT NewWndRect, bool bRedraw = true);
	// 根据窗口基本属性，设置窗口基本样式
	void SetWindowStyleByProp();
	// 设置窗口支持透明模式
	void SetWindowTransparence(bool bIsTrans);
	// 在任务栏上隐藏主窗口按钮
	void HideInTaskbar();
	// 向内核注册一个想要取到的消息
	void DoRegisterControlMessage(IControlBase* pCtrl, int nMsgId);
	void DoUnRegisterControlMessage(IControlBase* pCtrl, int nMsgId);
	// 派发控件注册的消息
	int DispatchRegisterMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam);
	// 向所有控件发送关闭对话框消息
	void SendCloseWndMsgToCtrl(CHILD_CTRLS_VEC *pChildCtrlsVec);
	bool OnTimerToChild(CHILD_CTRLS_VEC &ChildCtrlsVec, UINT nTimerId);
	// 删除控件后需要清空相关的控件指针
	void ResetActiveControl();
	// 取得下一个taborder的控件
	IControlBase* GetNextTabOrderControl(int nFromTabOrder);
	// 取得指定taborder的控件
	IControlBase* IWindowBaseImpl::GetTabOrderCtrl(CHILD_CTRLS_VEC* pChildVec, int nOrder);

protected:
	// 鼠标左键按下的位置
	POINT m_bLButtonDownPoint;
	// 上一次设置的鼠标手势
	HCURSOR m_hPreviousCursor;
	// 鼠标是否正在拖拽中
	bool m_bMouseDragging;
	// 鼠标是否离开窗口
	bool m_bMouseLeave;
	// 是否停止进行绘制窗口
	bool m_bStopDrawWindow;
	// 非分层对话框拉伸窗口边缘的hittype
	int m_nHitType;
	// 控件注册需要取得的消息
	REG_MSG_MAP m_CtrlRegMsgMap;
	// 控件的消息回调接口
	IControlMessage* m_pCtrlMsgCallBack;
	// 高进度定时器
	CHighAccuracyTimer *m_pHighTimer;
	// 皮肤路径
	CStringW m_strSkinPath;
	// 皮肤的object名称
	CStringW m_strWindowObjectName;
	// Builder 专用，显示控件的边框的8个方块
	FANGKUAI_8 m_BD_FangKuai8;

	// 在Builder中绘制用的边框色块
	IPropertyImageBaseImpl m_BuilderWndFrameImageBase;
	IPropertyImageBaseImpl m_BuilderCtrlFrameImageBase;

	BLENDFUNCTION m_Blend;
	// 父窗口句柄
	HWND m_hParent;
	// 窗口句柄
	HWND m_hWnd;
	// 整个对话框的内存DC
	CDrawingImage m_WndMemDc;
	// 鼠标左键是否按下
	bool m_bIsLButtonDown;
	// 鼠标Hover的控件
	IControlBase* m_pMouseHoverCtrl;
	// 鼠标左键按下的控件
	IControlBase* m_pLButtonDownCtrl;
	// 取得焦点的控件
	IControlBase* m_pFocusCtrl;
	// 注册的用于绘制焦点的控件
	IControlBase* m_pRegFocusCtrl;
	// tab停留的焦点控件
	IControlBase* m_pTabCtrl;
	// 子控件列表
	CHILD_CTRLS_VEC m_ChildCtrlsVec;
	// 一个需要得到窗口所有消息的控件，这个控件在取得焦点之后会收到所有窗口消息
	IControlBase* m_pWindowMessageCtrl;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 窗口属性
public:
	// 将xml中的属性设置到manager中
	virtual IPropertyGroup *PP_GetWindowPropetryGroup();
	virtual IPropertyWindow *PP_GetWindowPropetry();

	virtual void PP_SetWindowPropetry(IPropertyWindow *pWndPropInXml, bool bSetDftProp);

	// 是否支持全窗口点击移动
	virtual void PP_SetDragWindow(bool bDrag);
	virtual bool PP_GetDragWindow();

	// 是否最大化
	virtual void SetFullScreen(bool bFull);
	virtual bool IsFullScreen();

	// 是否窗口置顶
	virtual bool IsTopmost();

	// 是否使用临时内存绘制
	virtual bool PP_IsUseTempDrawMem();

	// 设置窗体的透明特性、设置窗口是否支持分层
	virtual void PP_SetLayeredWindow(bool bIsLayered);
	virtual bool PP_GetLayeredWindow();

	// 窗口Object名称
	virtual void PP_SetWindowObjectName(LPCWSTR pszWndName);
	virtual LPCWSTR PP_GetWindowObjectName();

	// 窗口名称
	virtual void PP_SetWindowText(LPCWSTR pszWndName);
	virtual LPCWSTR PP_GetWindowText();

	// 取得窗口中设置属性的窗口大小
	virtual SIZE PP_GetWindowPropSize();

private:
	// 创建空的属性队列
	void CreateWindowPropetry(bool bSetDftProp);
	IPropertyBase* CreateWindowPublicPropetry(SKIN_FILE_ITEM* pSkinFileItem, IPropertyGroup* pGroup, OBJECT_TYPE_ID propType, const char* pszPropName, const char *pszPropInfo);
	// 释放子控件列表
	void ReleaseChildVec(CHILD_CTRLS_VEC *pChildCtrlsVec);
	// 绘制窗口和被选中的控件的边框的8个方块
	void BD_DrawSelectRect(CDrawingBoard &MemDc, FANGKUAI_8 &FangKuai8, bool bIsWndFrame);
	// 向所有控件发送对话框初始化完成的消息
	void SendWindowInitOkMsgToCtrl(CHILD_CTRLS_VEC *pChildCtrlsVec);
	// 向所有控件发送进入/离开对话框移动/拉伸消息
	void SendEnterOrExitSizeMsgToCtrl(bool bIsEnter, CHILD_CTRLS_VEC *pChildCtrlsVec);
	// 从队列中删除一个控件
	bool DeleteSubControlFromVec(CHILD_CTRLS_VEC *pCtrlVec, IControlBase* pDelCtrl);

//////////////////////////////////////////////////////////////////////////
	// 控件动画
	static DWORD WINAPI ControlAnimationThread(LPVOID lpParam);
	void DrawControlAnimation();
	virtual bool AppendAnimation(IControlBase* pCtrl, CDrawingImage& CtrlAnimationMemDc,
		DWORD animType, int nFrameTime, int nAnimationDirection);
	// 当一个动画开始画第一帧之前，先发送动画开始的消息
	virtual void OnUiEffectBegin(WPARAM effectKey, DWORD animaType);
	// 当一个动画的最后一帧绘制完毕，发送动画结束的消息
	virtual void OnUiEffectEnd(WPARAM effectKey, DWORD animaType);
	// 所有动画资源都计算完毕，开始绘制动画
	virtual void OnUiEffectDraw();

private:
//////////////////////////////////////////////////////////////////////////
	// 动画
	// 动画状态
	HDC m_hAnimationWndDc;
	HANDLE m_hCtrlAnimateThread;
	IUIEffect* m_pUiEffect;
	// 用户自定义强动画控件
	IControlBase* m_pUserDefAnimationCtrl;
//////////////////////////////////////////////////////////////////////////

	bool m_bMaximizeWndDragMove;
	CSystemTips *m_pSysTips;
	IUiEngine *m_pUiEngine;
	IPropertySkinManager* m_pSkinPropMgr;
	// 控制拉伸窗口操作的类
	CWindowResize m_WndResize;
	// 当前显示的鼠标手势
	IPropertyCursorBase *m_pCurrentCursorBase;

	bool m_bIsFullScreen;
	// 当前鼠标样式宏
	int m_nCurMouseStyle;

	// 从xml中读入并需要写入xml中的属性窗口属性列表
	IPropertyWindow *m_pXmlPropWindow;

	// Group:base
	IPropertyGroup* m_pPropGroupBase;
	// base-类型名称
	IPropertyString *m_pPropBase_TypeName;
	// base-objectid
	IPropertyInt *m_pPropBase_ObjectId;
	// base-name
	IPropertyString *m_pPropBase_Name;
	// base-windowtitle
	IPropertyString *m_pPropBase_WindowText;
	// base-在任务栏显示按钮
	IPropertyBool *m_pPropBase_ShowInTaskbar;
	// base-支持分层窗口
	IPropertyBool *m_pPropBase_Layered;
	// base-topmost
	IPropertyBool *m_pPropBase_TopMost;
	// base-TempDrawMem
	IPropertyBool *m_pPropBase_TempDrawMem;
	// base-BigTabOrder 对话框中最大TabOrder值
	IPropertyInt *m_pPropBase_BigTabOrder;

	// Group-size
	IPropertyGroup* m_pPropGroupWindowSize;
	// size-width
	IPropertyInt *m_pPropSize_WindowWidth;
	// size-height
	IPropertyInt *m_pPropSize_WindowHeight;

	// Group-drag(拖拽窗口)
	IPropertyGroup* m_pPropGroupDrag;
	// drag-enable
	IPropertyBool *m_pPropDrag_Enable;

	// Group-Size(窗口的大小)
	IPropertyGroup* m_pPropGroupSize;
	// Size-enable
	IPropertyBool *m_pPropSize_Enable;
	// Size-MaxWidth
	IPropertyInt *m_pPropSize_MaxWidth;
	// Size-MaxHeight
	IPropertyInt *m_pPropSize_MaxHeight;
	// Size-MinWidth
	IPropertyInt *m_pPropSize_MinWidth;
	// Size-MinHeight
	IPropertyInt *m_pPropSize_MinHeight;

	// Group-stretching(拉伸窗口)
	IPropertyGroup* m_pPropGroupStretching;
	// stretching-enable
	IPropertyBool *m_pPropStretching_Enable;
	// stretching-leftspace
	IPropertyInt *m_pPropStretching_LeftSpace;
	// stretching-rightspace
	IPropertyInt *m_pPropStretching_RightSpace;
	// stretching-topspace
	IPropertyInt *m_pPropStretching_TopSpace;
	// stretching-bottomspace
	IPropertyInt *m_pPropStretching_BottomSpace;
};
