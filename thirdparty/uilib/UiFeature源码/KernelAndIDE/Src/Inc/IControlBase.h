
#pragma once
#include "UiFeatureDefs.h"
#include "CDrawingImage.h"
#include "IFeatureObject.h"
#include "IUiEngine.h"
#include "IPropertySkinManager.h"
#include "IPropertyString.h"
#include "IPropertyGroup.h"
#include "IPropertyBool.h"
#include "IPropertyInt.h"
#include "IPropertyColor.h"
#include "IPropertyComboBox.h"
#include "IPropertyCursor.h"
#include "IPropertyFont.h"
#include "IPropertyImage.h"
#include "CTList.h"


class IWindowBase;
class IWindowBaseImpl;
class IUiFeatureKernelImpl;
class IPropertyControl;
class IControlBase;
class IControlManager;

// 控件基础消息，这个消息会发给窗口或者创建它的父控件。
enum CTRL_MSG_BASE
{
	CMB_BASE = 0x000F0000,
	// 控件动画开始
	CMB_ANIMATION_BEGIN,
	// 控件动画结束
	CMB_ANIMATION_END,
	// 动画有异常
	CMB_ANIMATION_ERROR
};

// OnBeforeClone 函数的返回值如果是这个宏，那就不进行Clone
#define CAN_NOT_CLONE						CTRL_REG_MSG_NOT_PASS_ON
// 此控件并非通过CreateSubControl创建的
#define NOT_BY_CREATE_SUB_CONTROL			(0)


// 控件发送消息接口
class _declspec(novtable) IControlMessage
{
public:
	virtual LRESULT OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam) = 0;
};

// Builder 专用，显示控件的边框的8个方块
struct FANGKUAI_8
{
	// 所有坐标都是基于View的绝对坐标
	RECT EntityRct;
	RECT LeftTop;
	RECT LeftMid;
	RECT LeftBottom;
	RECT MidTop;
	RECT MidBottom;
	RECT RightTop;
	RECT RightMid;
	RECT RightBottom;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 当前控件相对于父控件的布局类型

// 固定大小：左上角定位
#define TN_CL_G_LEFT_TOP				"左上角定位"
// 固定大小：左下角定位
#define TN_CL_G_LEFT_BOTTOM				"左下角定位"
// 固定大小：右上角定位
#define TN_CL_G_RIGHT_TOP				"右上角定位"
// 固定大小：右下角定位
#define TN_CL_G_RIGHT_BOTTOM			"右下角定位"
// 拉伸变动大小：撑满左侧
#define TN_CL_L_LEFT					"撑满左侧"
// 拉伸变动大小：撑满右侧
#define TN_CL_L_RIGHT					"撑满右侧"
// 拉伸变动大小：撑满上方
#define TN_CL_L_TOP						"撑满上方"
// 拉伸变动大小：撑满下方
#define TN_CL_L_BOTTOM					"撑满下方"
// 拉伸变动大小：撑满全部
#define TN_CL_L_ALL						"撑满全部"
// 拉伸变动大小：居中
#define TN_CL_L_MIDDLE					"居中"
// 拉伸变动大小：用户自定义位置
#define TN_CL_L_USER_DEF				"用户自定义位置"
// 固定大小：靠上居中
#define TN_CL_G_TOP_MIDDLE				"固定大小：靠上居中"
// 固定大小：靠下居中
#define TN_CL_G_BOTTOM_MIDDLE			"固定大小：靠下居中"
// 固定大小：靠左居中
#define TN_CL_G_LEFT_MIDDLE				"固定大小：靠左居中"
// 固定大小：靠右居中
#define TN_CL_G_RIGHT_MIDDLE			"固定大小：靠右居中"
// 固定大小：居中
#define TN_CL_G_MID_MIDDLE				"固定大小：居中"

enum CONTROL_LAYOUT
{
	// 固定大小：左上角定位
	CL_G_LEFT_TOP		= 0,
	// 固定大小：左下角定位
	CL_G_LEFT_BOTTOM	= 1,
	// 固定大小：右上角定位
	CL_G_RIGHT_TOP		= 2,
	// 固定大小：右下角定位
	CL_G_RIGHT_BOTTOM	= 3,
	// 拉伸变动大小：撑满左侧
	CL_L_LEFT			= 4,
	// 拉伸变动大小：撑满右侧
	CL_L_RIGHT			= 5,
	// 拉伸变动大小：撑满上方
	CL_L_TOP			= 6,
	// 拉伸变动大小：撑满下方
	CL_L_BOTTOM			= 7,
	// 拉伸变动大小：撑满全部
	CL_L_ALL			= 8,
	// 拉伸变动大小：居中
	CL_L_MIDDLE			= 9,
	// 拉伸变动大小：用户自定义位置
	CL_L_USER_DEF		= 10,
	// 固定大小：靠上居中
	CL_G_TOP_MIDDLE		= 11,
	// 固定大小：靠下居中
	CL_G_BOTTOM_MIDDLE	= 12,
	// 固定大小：靠左居中
	CL_G_LEFT_MIDDLE	= 13,
	// 固定大小：靠右居中
	CL_G_RIGHT_MIDDLE	= 14,
	// 固定大小：居中
	CL_G_MID_MIDDLE		= 15,
};
// 当前控件相对于父控件的布局信息
struct CONTROL_LAYOUT_INFO
{
	// 布局类型
	CONTROL_LAYOUT clType;
	// 控件宽
	int nWidth;
	// 控件高
	int nHeight;
	// 距离父控件左侧距离
	int nLeftSpace;
	// 距离父控件右侧距离
	int nRightSpace;
	// 距离父控件上方距离
	int nTopSpace;
	// 距离父控件下方距离
	int nBottomSpace;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 一个控件对外提供的注册信息，这个信息会注册到UiFeatureBuilder提供Builder显示，
// 也会提供给UiFeatureKernel进行查询
class CONTROL_REG : public ListObj
{
public:
	CONTROL_REG();
	~CONTROL_REG();

	IControlManager* pCtrlMgr;
	// 所处的DLL名称
	WCHAR strDllName[MAX_PATH + 1];
	// 控件所在的控件组名称
	WCHAR strCtrlGroupName[MAX_PATH + 1];
	// 控件名称
	WCHAR strCtrlName[MAX_PATH + 1];
	// 控件说明
	WCHAR strControlInfo[MAX_PATH + 1];
};
// 内部存放 CONTROL_REG*
typedef CTList CONTROL_REG_TLIST;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 子控件列表
// typedef vector<IControlBase*>			CHILD_CTRLS_VEC;
// IControlBase* 列表
typedef CTList			CHILD_CTRLS_VEC;

// 没有设置TabOrder属性的控件
#define NOT_SET_TAB_FOCUS					(0)

class IUiFeatureKernel;
class IUiEngine;
class IPropertySkinManager;

class IControlBase : public IFeatureObject, public ListObj
{
	friend class IWindowBaseImpl;
	friend class IUiFeatureKernelImpl;

public:
	IControlBase(IUiFeatureKernel *pUiKernel);
	virtual ~IControlBase();

	// 得到在Builder中显示的四周8个方块的位置	
	FANGKUAI_8* BD_GetFangKuai8Rect();
	// 得到绘制引擎
	IUiEngine* GetUiEngine();
	// 皮肤包管理类
	IPropertySkinManager* GetSkinManager();

	// 克隆一个和自己一模一样的控件
	// pParentCtrl：父控件
	// pstrCtrlName：克隆的控件名称
	// bCloneChild：是否克隆子控件
	// wParam：参数会传给OnBeforeClone消息函数和OnAfterClone函数
	IControlBase* Clone(IControlBase* pParentCtrl, LPCWSTR pstrCtrlName, BOOL bCloneChild, WPARAM wParam);

	// 取得父控件的指针，如果为NULL，说明父控件是对话框
	IControlBase* GetParentControl();
	IWindowBase* GetOwnerWindow();
	// 根据子控件的名字得到子控件
	IControlBase* GetControl(char *pszCtrlName);
	IControlBase* GetControl(WCHAR *pszCtrlName);
	IControlBase* GetControl(LPCWSTR pszCtrlName);
	IControlBase* GetControlByObjectId(DWORD dwObjId);

	// 取得子控件列表
	CHILD_CTRLS_VEC* GetChildControlsVec();
	// 取得子控件的属性列表：Layout.xml 中的布局
	PROP_CONTROL_VEC* GetChildPropControlVec();

	// 重绘控件
	virtual void RedrawControl(bool bDrawImmediately = false);

	// 控件显示位置和大小，这个位置是相对于附着的窗口的
	// bRememberToProp:false 不会改变布局信息
	// bRememberToProp:true 会改变布局信息
	virtual void SetWindowRect(RECT CtrlInWndRct, bool bRememberToProp = false);
	virtual RECT GetWindowRect();
	virtual RECT GetClientRect();
	// 移动控件，会将最终的位置信息存为布局信息
	// 参数：CtrlInWndRct控件位于窗口的位置
	// bSetChild：是否递归设置子控件
	void SetWindowRectLayoutWithChild(RECT CtrlInWndRct, bool bSetChild = true);

	// 绘制控件到指定的DC上
	void PrintTo(CDrawingBoard &DstBoard, RECT ToRct);
	// 将当前控件，包括子控件，都打印到一个内存DC上
	// bPrintWhenInvisible：如果控件visiable属性为false是否也打印
	bool PrintToCoverChild(CDrawingBoard &DstBoard, bool bPrintWhenInvisible);

	// 判断参数中的控件是否就是自己
	bool CompareControl(IControlBase* pCtrl);

	// 取得父窗口的句柄
	HWND GetOwnerWindowHwnd();

	LPCWSTR PP_GetControlObjectName();

	CONTROL_LAYOUT_INFO GetLayout();

	// 设置是否删除克隆属性
	void SetDeleteCloneProp(bool bDelete);

	// 鼠标是否Hover
	bool IsMousehover();

	IPropertyGroup* PP_GetControlPropetryGroup();
	IPropertyControl *PP_GetControlPropetry();

//////////////////////////////////////////////////////////////////////////
	// 显示Tips
	void ShowSystemTips();
	// Tips属性信息
	// 系统 tips 显示的文字信息
	void PP_SetTipsInfomation(LPCWSTR pszTips);
	LPCWSTR PP_GetTipsInfomation();
	// 系统 tips 是否显示为气泡样式
	void PP_SetTipsBaloon(bool bBaloon);
	bool PP_GetTipsBaloon();
	// 系统 tips 显示时间：单位毫秒
	void PP_SetTipsShowTime(int nTime);
	int PP_GetTipsShowTime();
	// 是否在用户自定义的控件范围内显示
	void PP_SetTipsShowInUserDefPos(bool bBaloon);
	bool PP_GetTipsShowInUserDefPos();
	// 本控件是否含有系统tips
	bool IsNeedSystemTips();
//////////////////////////////////////////////////////////////////////////

	// 设置/取得发送控件消息的目标
	void SetControlMessage(IControlMessage* pCtrlMsg);
	IControlMessage* GetControlMessage();

	// 取得绘制控件的内存DC
	CDrawingBoard* GetMemoryDc();
	// 从属性更新数据到成员变量
	void PropetyValueToMemberValue(bool bCallRefresh, IPropertyBase* pPropBase);

//////////////////////////////////////////////////////////////////////////
	// 滚动条消息
	virtual void OnVScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar) = 0;
	virtual void OnHScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar) = 0;
	// 鼠标滚轮消息
	virtual void OnMouseWheel(WPARAM wParam, LPARAM lParam) = 0;

	// 控件需要处理的一些自定义的消息，消息基本上会由对话框或者其他控件传入
	virtual int OnCtrlNotify(int nMsgId, WPARAM wParam, LPARAM lParam) = 0;

	// 用于在派生控件的CreateControlPropetry函数中创建一个属性
	// 当参数：bNotExistCreate = false 的时候，此函数创建属性分为两种情况：
	// 编辑时态：无条件创建此属性
	// 运行时态：内部会判断皮肤包中是否有此属性，如果有的话就创建此属性，如果没有这个属性，就不创建，为了节省内存
	// 当参数：bNotExistCreate = true 的时候，无论何种时态，无条件创建属性
	// 所以在使用的时候请注意：一般在 CreateControlPropetry 函数中使用此函数创建属性的时候，请直接使用 bNotExistCreate = false 的默认值即可
	// 而在其他地方使用此函数的时候 bNotExistCreate 要设置为 true，如果：一个图片属性，也许在皮肤包中没有设置，初始化的时候就会为空，但在程序运行时可能会进行换肤操作，
	// 此时需要创建这个属性了，那就调用此函数进行创建，此时 bNotExistCreate 就需要设置为true，强制创建此属性。
	IPropertyBase* CreatePropetry(IPropertyGroup* pPropGroup, OBJECT_TYPE_ID propType, const char* pszPropName, const char *pszPropInfo, bool bNotExistCreate = false);

	// 当一个控件包含了其他子控件的时候，系统会发送创建或加载子控件的消息
	void CreateSubControl(bool bIsCreate);
	// 设置父控件
	void SetParentControl(IControlBase* pParentCtrl);
	// 设置控件的布局方式
	void SetControlLayoutType(CONTROL_LAYOUT CtrlLayout);
	// 设置隶属于的dll
	void SetControlDllManager(IControlManager* pOwnerCtrlDllMgr);
	IControlManager* GetControlDllManager();

	// 取得控件自己创建的属性
	IPropertyGroup* GetCtrlPrivatePropGroup();
	// 取得控件共有创建的属性
	IPropertyGroup* GetCtrlPublicPropGroup();

	// 销毁设计时态使用的属性，减少运行时态内存占用
	void ClearDesignModePropetry();
	// 当前控件是否为克隆控件
	bool IsCloneControl();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 属性接口
	// 可见属性
	virtual void SetVisible(bool bVisible, bool bSetChild = false);
	// 是否可以见，参数 bIncludeParent 代表是否递归判断父控件的可见属性，还是只判断自身的可见属性
	// true：递归判断父控件的可见属性
	// false：只判断自己的可见属性
	virtual bool IsVisible(bool bIncludeParent = true);

	// 可用属性
	virtual void SetEnable(bool bEnable, bool bSetChild = false);
	virtual bool IsEnable();

	// 是否接受鼠标消息
	void SetReceiveMouseMessage(bool bIsReceive);
	bool GetReceiveMouseMessage();

	// 是否锁定在Builder中鼠标改变控件大小和位置
	void SetLockControl(bool bLock);
	bool GetLockControl();

	// TabOrder属性
	void PP_SetControlTabOrder(int nTabOrder);
	int PP_GetControlTabOrder();

	// 是否为默认回车键接受函数
	void PP_SetDefaultEnterCtrl(bool bIsDftEnterCtrl);
	bool PP_IsDefaultEnterCtrl();

	// 是否为无绘制控件
	void PP_SetNoDrawControl(bool bIsNoDrawCtrl);
	bool PP_GetNoDrawControl();

	// 是否使用 AlphaBlend 函数直接将控件绘制到窗口上
	void PP_SetControlAlphaBlendDraw(bool bIsAlphaBlendDraw);
	bool PP_GetControlAlphaBlendDraw();

	// 整个控件绘制完成后，再绘制到父控件上的alpha值
	void PP_SetControlDrawAlpha(int nCtrlAlpha);
	int PP_GetControlDrawAlpha();

	// 是否使用临时内存绘制
	bool PP_IsUseTempDrawMem();
	void PP_SetUseTempDrawMem(bool bUseTempMem);

	// 设置动画类型
	void PP_SetCtrlAnimationType(DWORD animEffect);
	DWORD PP_GetCtrlAnimationType();

	// 设置动画时间：每一帧动画的时间间隔，单位：毫秒
	void PP_SetCtrlAnimationFrameTime(int nTime);
	int PP_GetCtrlAnimationFrameTime();

	// 设置动画方向：0：正向动画；1：反向动画
	void PP_SetCtrlAnimationDirection(int nDirection);
	int PP_GetCtrlAnimationDirection();

protected:
	// 设置子控件都必须自绘
	void SetChildCtrlToRedraw();
	// 删除克隆属性
	void ReleaseCloneProp();
	// 设置当前控件为克隆出来的控件
	void SetCloneControl(bool bIsCloneCtrl);

	// 派生控件用于创建属于自己的控件属性
	// bIsNewCtrl：true时说明是Builder调用，新创建一个控件，需要初始化属性的各个默认值
	// false：说明整个皮肤包已经初始化完毕，属性创建完毕的时候，从xml中读取的属性值已经赋值完毕了，不能再初始化，否则xml中的值会被冲掉
	virtual bool CreateControlPropetry(bool bIsNewCtrl) = 0;
	// 绘制控件的动画，如果需要重新刷新界面，返回true，否则返回false
	// 此函数由定时器触发，每100毫秒触发一次
	virtual bool OnDrawAnimation() = 0;
	// 定时器
	virtual void OnTimer(UINT nTimerId) = 0;
	// 初始化控件
	virtual void OnCreate() = 0;
	// 控件初始化完毕
	virtual void OnFinalCreate() = 0;
	// 当一个控件包含了其他子控件的时候，系统会发送创建或加载子控件的消息
	virtual void OnCreateSubControl(bool bIsCreate) = 0;
	// 整个对话框包括对话框上的控件都初始化完毕
	virtual void OnWindowFinalCreate() = 0;
	// 附属对话框即将关闭
	virtual void OnWindowClose() = 0;
	// Builder刷新属性，此函数中只要重新加载控件属性即可
	virtual void OnBuilderRefreshProp(IPropertyBase* pPropBase) = 0;
	// 销毁控件
	virtual void OnDestroy() = 0;
	// 绘制控件
	virtual void OnPaint(CDrawingBoard &DrawBoard) = 0;
	// 鼠标进入
	virtual void OnMouseEnter(POINT pt) = 0;
	// 鼠标移出
	virtual void OnMouseLeave(POINT pt) = 0;
	// 鼠标移动
	virtual void OnMouseMove(POINT pt) = 0;
	// 鼠标左键点击
	virtual void OnLButtonDown(POINT pt) = 0;
	// 鼠标左键抬起
	virtual void OnLButtonUp(POINT pt) = 0;
	// 鼠标左键双击
	virtual void OnLButtonDbClick(POINT pt) = 0;
	// 鼠标在控件内部拖动
	virtual void OnMouseDragging(POINT pt) = 0;
	// 鼠标拖动操作结束，抬起鼠标。pDragCtrl：拖拽来源控件
	virtual void OnDragStop(IControlBase* pDragCtrl, POINT pt, WPARAM wParam, LPARAM lParam) = 0;
	// 取得控件之间拖拽的数据参数，在触发了 OnDragStop 消息后会被调用
	virtual void OnGetDragParam(WPARAM& wParam, LPARAM& lParam) = 0;
	// 鼠标右键点击
	virtual void OnRButtonDown(POINT pt) = 0;
	// 移动、设置控件位置
	virtual void OnSize() = 0;
	// 移动窗口开始
	virtual void OnEnterSizeMove() = 0;
	// 移动窗口结束
	virtual void OnExitSizeMove() = 0;
	// 控件取得焦点，通过Tab键跳转，激活控件
	virtual void OnSetFocus() = 0;
	// 控件失去焦点
	virtual void OnKillFocus() = 0;
	// 接受到默认回车键，执行控件操作
	virtual void OnDefaultEnterCtrl() = 0;
	// 控件取得键盘输入消息
	virtual void OnChar(WPARAM wParam, LPARAM lParam) = 0;
	// 按键按下
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam) = 0;
	virtual void OnKeyUp(WPARAM wParam, LPARAM lParam) = 0;
	// 鼠标是否落在了控件指定的RGN区域，默认返回true，整个控件就是RGN区域
	virtual bool OnCheckMouseInRgn(POINT pt, int nMouseMsgId) = 0;
	// 设置控件光标
	virtual IPropertyCursor* OnSetCursor(POINT pt) = 0;
	// 收到需要清空绘制缓存的消息
	virtual void OnClearDrawMemoryDc() = 0;

//////////////////////////////////////////////////////////////////////////
	// 用户自定义强动画开始
	virtual void OnUserDefHighAnimationBegin() = 0;
	// 用户自定义强动画绘制接口，hWndDc：控件所在的窗口DC
	virtual void OnUserDefHighAnimationDraw(CDrawingImage &CtrlAnimationMemDc, HDC &hWndDc) = 0;
	// 用户自定义强动画结束
	virtual void OnUserDefHighAnimationEnd() = 0;
//////////////////////////////////////////////////////////////////////////
	// 开始克隆，参数由Clone函数传入，如果函数返回 CAN_NOT_CLONE ，则不进行克隆操作
	virtual int OnBeforeClone(WPARAM wParam) = 0;
	// pCloneCtrl 是最终克隆出来的控件，可以在这个消息响应函数中再克隆一些控件私有的属性
	virtual void OnAfterClone(IControlBase* pCloneCtrl, WPARAM lParam) = 0;
	// 一个控件如果是另一个控件的子控件，并且此控件是通过CreateSubContrl函数创建的，那么这个控件的父控件被Clone的时候，当这个子控件也被Clone完成后，会受到此消息函数
	virtual void OnCreateSubControlCloneEnd() = 0;

	// 向内核注册一个想要取到的消息：异步处理方式，一般都采用这个处理方式，特殊情况采用同步处理方式
	void RegisterControlMessage(int nMsgId);
	void UnRegisterControlMessage(int nMsgId);

	// 向内核注册一个想要取到的消息：同步处理方式，控件请不要再自己注册了的消息的处理过程中执行同步处理
	void RegisterControlMessageSync(int nMsgId);
	void UnRegisterControlMessageSync(int nMsgId);

	// 发送消息:Send方式
	LRESULT SendMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam);
	// 发送消息:Post方式
	bool PostMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam);
	// 播放动画消息
	void OnCtrlAnimation(WPARAM wParam, LPARAM lParam);
	// 准备控件动画
	bool ControlAnimationReady();

//////////////////////////////////////////////////////////////////////////
	// 当一个控件的TabOrder属性被设置了之后，同时此控件得到焦点，窗口会将此消息
	// 通过这个函数发送给注册了需要取得焦点消息的控件
	virtual void OnContrlFocusChange(IControlBase* pFocusCtrl) = 0;

private:
	// 创建控件属性
	void CreateCtrlAllPropetry(bool bIsNewCtrl);
	IPropertyBase* CreateCtrlPublicPropetry(SKIN_FILE_ITEM* pSkinFileItem, IPropertyGroup* pGroup, OBJECT_TYPE_ID propType, const char* pszPropName, const char *pszPropInfo);

	// 初始化在builder中的属性
	bool InitControlPropetry(IPropertyControl *pCtrlProp, bool bIsNewCtrl);
	void SetPropertySkinManager(IPropertySkinManager *pMgr);
	// 向队列未插入一个控件
	void AppendChildContrl(IControlBase *pCtrl);

	void SetUiEngine(IUiEngine *pUiEngine);
	// 附着对话框
	void SetOwnerWindow(IWindowBase* pWindowsBase);

	// 鼠标是否Hover
	void SetMouseHover(bool bHover);

	// 此函数由窗口调用，绘制当前控件，参数为父窗口/父控件的内存DC
	virtual void OnPaintControl(CDrawingBoard &WndMemDc, RECT ActiveDrawRct);

	// 从成员变量更新数据到属性
	void MemberValueToPropetyValue();
	// 重新计算子控件的位置和大小
	void ResetChildPropetyValueToMemberValue(IControlBase* pParentCtrl, bool bCallRefresh, IPropertyBase* pPropBase);
	// 控件显示位置和大小，会根据布局信息连带修改子控件的位置
	void SetChildControlLayoutPostion(CHILD_CTRLS_VEC* pVec);
	// 递归打印子控件到一个内存DC上
	bool ChildPrint(CHILD_CTRLS_VEC *pChildCtrlsVec, CDrawingBoard& DstBoard, POINT offsetPt, bool bPrintWhenInvisible);

	// 清空属性的隶属于哪个控件的信息
	void ClearPropertyOwner();
	// 清空绘制缓存
	void ClearDrawMemoryDc();
	// 用于内核显示全控件范围的tips
	void ShowDefaultSystemTips();
	// 调用Clone函数时，一个一个Group复制控件的所有属性，包括共有的属性和私有的属性
	BOOL CloneGroupPropetry(IPropertyGroup* pFromGroup, IPropertyGroup* pToGroup);
	// 实施Clone一个控件操作
	IControlBase* Cloneing(IControlBase* pParentCtrl, LPCWSTR pstrCtrlName, BOOL bCloneChild, WPARAM wParam, BOOL bBase);
	IControlBase* DoClone(IControlBase* pParentCtrl, LPCWSTR pstrCtrlName, BOOL bCloneChild, WPARAM wParam, BOOL bBase);
	BOOL ClonePropetry(IControlBase* pFromCtrl, IControlBase* pToCtrl);
	// 递归给通过CreateSubControl函数创建的子控件的属性克隆
	BOOL ClonePropetry_ByCSC(IControlBase* pCloneCtrl, CHILD_CTRLS_VEC *pFromChildCtrlsVec, CHILD_CTRLS_VEC *pToChildCtrlsVec);
	bool IsParentVisible(IControlBase* pParentCtrl, bool bSelfVisible);

protected:
	// 控件是否有系统的Tips属性
	bool m_bHaveSysTipsProp;
	// 发送消息的目标
	IControlMessage* m_pSendMsgCtrl;
	// 通过CreateSubControl函数创建的子控件的ID，为了Clone是能给这些子控件克隆属性使用
	int m_nSubCtrlId;

	// 子控件列表
	CHILD_CTRLS_VEC m_ChildCtrlsVec;

	// 是否需重绘控件
	bool m_bNeedRedraw;
	// 鼠标Hover状态
	bool m_bMouseHover;
	// 控件位置，这个位置是相对于窗口的位置
	RECT m_RectInWindow;

	// 整个控件的内存DC
	CDrawingImage *m_pCtrlMemDc;
	// 动画内存DC
	CDrawingImage *m_pCtrlAnimationMemDc;
	// 动画是否开始
	bool m_bIsCtrlAnimationBegin;

	// 附着的窗口
	IWindowBase* m_pWindowBase;
	// 父控件
	IControlBase* m_pParentCtrl;
	// 绘图引擎
	IUiEngine *m_pUiEngine;

	// 皮肤包管理类
	IPropertySkinManager *m_pSkinPropMgr;
	// 记录到xml中的属性
	IPropertyControl *m_pXmlPropCtrl;

	// Group:base
	IPropertyGroup* m_pPropGroupBase;
	// base-类型名称：使用 GetObjectName 的对外接口访问
	IPropertyString *m_pPropBase_TypeName;
	// base-objectid：使用 GetObjectId 的对外接口访问
	IPropertyInt *m_pPropBase_ObjectId;
	// base-name：使用 GetObjectName 的对外接口访问
	IPropertyString *m_pPropBase_Name;
	// base-lock：锁定控件，在builder中不允许鼠标拖动控件
	IPropertyBool *m_pPropBase_Lock;
	// base-visible：控件是否可见
	IPropertyBool *m_pPropBase_Visible;
	// base-ReceiveMouseMessage：控件是否接受鼠标消息
	IPropertyBool *m_pPropBase_RcvMouseMsg;
	// base-enable：控件是否可用
	IPropertyBool *m_pPropBase_Enable;
	// base-taborder：tab顺序
	IPropertyInt *m_pPropBase_TabOrder;
	// base-defaultenterctrl：是否为默认的回车键接受控件
	IPropertyBool *m_pPropBase_DefaultEnterCtrl;

	// Group:Tips
	IPropertyGroup* m_pPropGroupTips;
	// Tips-ShowInUserDefPos
	IPropertyBool *m_pPropTips_ShowInUserDefPos;
	// Tips-Baloon
	IPropertyBool *m_pPropTips_Baloon;
	// Tips-tips
	IPropertyString *m_pPropTips_TipsData;
	// Tips-times
	IPropertyInt *m_pPropTips_ShowTimes;

	// base-Draw
	IPropertyGroup *m_pPropBase_DrawGroup;
	// base-Draw-NoDrawControl
	IPropertyBool *m_pPropBase_NoDrawCtrl;
	// base-Draw-AlphaBlendDraw
	IPropertyBool *m_pPropBase_Draw_AlphaBlendDraw;
	// base-Draw-DrawAlpha
	IPropertyInt *m_pPropBase_Draw_DrawAlpha;
	// base-Draw-TempDrawMem
	IPropertyBool *m_pPropBase_Draw_TempDrawMem;

	// base-layout
	IPropertyGroup *m_pPropBase_LayoutGroup;
	// layout-width
	IPropertyInt *m_pPropBase_Layout_Width;
	// layout-height
	IPropertyInt *m_pPropBase_Layout_Height;

	// layout-
	IPropertyComboBox *m_pPropBase_Layout_Layout;
	// layout-leftspace
	IPropertyInt *m_pPropBase_Layout_LeftSpace;
	// layout-rightspace
	IPropertyInt *m_pPropBase_Layout_RightSpace;
	// layout-topspace
	IPropertyInt *m_pPropBase_Layout_TopSpace;
	// layout-bottomspace
	IPropertyInt *m_pPropBase_Layout_BottomSpace;

	// base-Animation
	IPropertyGroup *m_pPropBase_AnimationGroup;
	// Animation-AnimationType
	IPropertyComboBox *m_pPropBase_AnimationType;
	// Animation-FrameTimeSpin
	IPropertyInt *m_pPropBase_Animation_FrameTime;
	// Animation-Front
	IPropertyComboBox *m_pPropBase_Animation_Direction;

	// Group:CtrlDefs
	IPropertyGroup* m_pPropGroupCtrlDefs;

private:
	// 是否为Clone创建的控件
	bool m_bIsCloneCtrl;
	// 是否删除克隆属性
	bool m_bDeleteCloneProp;
	// Builder 专用，显示控件的边框的8个方块
	FANGKUAI_8 m_BD_FangKuai8;
	IControlManager* m_pOwnerCtrlDllMgr;
	SKIN_FILE_ITEM* m_pSkinFileItem;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 对话框或者控件需要处理子控件的消息的时候的宏
#define BEGIN_CTRL_COMMAND																		\
public:																							\
virtual LRESULT OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam){	\

// LRESULT OnBtnClick(WPARAM wParam, LPARAM lParam);
#define CTRL_COMMAND_HANDLER(pCtrlIn, CmdId, Func_name)											\
if (pCtrlIn != NULL && pCtrlIn->CompareControl(pCtrl) && CmdId == nMsgId){return Func_name(wParam, lParam);}

// 对话框需要处理从Ui内核发从的消息的宏，需要配合 BEGIN_CTRL_COMMAND 宏使用
// LRESULT OnBtnClick(WPARAM wParam, LPARAM lParam);
#define KERNEL_COMMAND_HANDLER(CmdId, Func_name)												\
	if (pCtrl == NULL && CmdId == nMsgId){return Func_name(wParam, lParam);}

#define END_CTRL_COMMAND																		\
return S_OK;}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 控件需要处理向内核注册的消息的时候的宏
#define BEGIN_CTRL_NOTIFY																		\
public:																							\
virtual int OnCtrlNotify(int nMsgId, WPARAM wParam, LPARAM lParam){								\

// int OnSysCommand(WPARAM wParam, LPARAM lParam);
#define CTRL_NOTIFY_HANDLER(CmdId, Func_name)													\
if (CmdId == nMsgId){return Func_name(wParam, lParam);}

#define END_CTRL_NOTIFY																			\
return S_OK;}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
