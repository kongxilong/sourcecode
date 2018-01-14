
#pragma once
#include "IWindowBase.h"
#include "IControlBase.h"
#include "IPropertyFontBase.h"
#include "IUiEngine.h"

class _declspec(novtable) IUiFeatureKernel
{
	friend class CMainFrame;
	friend class CWindowsViewTree;
	friend class CLocalImageList;

public:
	virtual IPropertySkinManager* GetSkinManager() = 0;
	virtual IUiEngine* GetUiEngine() = 0;

	// 创建一个含有未设置属性的控件
	virtual IControlBase* CreateControlEmptyPropetry(SKIN_FILE_ITEM* pSkinFileItem, IWindowBase *pParentWnd, IControlBase *pParentCtrl,
		LPCWSTR pszNewCtrlTypeName, LPCWSTR pszNewCtrlName, LPCWSTR pszDftSkinDir, bool bIsCloneCtrl) = 0;

	// 退出程序前调用，卸载kernel的资源，外部函数必须在退出应用程序前进行调用
	// 也可不调用，但会引起GDI+的部分内存泄露
	virtual void UnInitialized() = 0;
	// 是否为设计模式
	virtual bool IsDesignMode() = 0;

//////////////////////////////////////////////////////////////////////////
	// 一个对话框从一个皮肤包里使用指定的对话框皮肤资源初始化自己
	// 窗口释放皮肤资源的释放在内部自动完成
	virtual IWindowBase* PG_InitFeatureSkin(HWND hWnd, LPCWSTR pszSkinPath, LPCWSTR pszWndName, char *pszPassword) = 0;
	// 一个对话框释放皮肤资源，这个Kernel内核会调用，外部对话框不需要调用
	virtual void PG_ReleaseFeatureSkin(HWND hWnd) = 0;

	// 创建一个Builder使用的控件，并配置上属性
	virtual IControlBase* CreateControlByPropetry(IWindowBase *pParentWnd, IControlBase *pParentCtrl, IPropertyControl *pPropCtrl) = 0;
	// 取得全局唯一定时器号
	virtual int GetTimerId() = 0;
	// 取得所有支持的动画列表
	virtual int GetAnimationList(const char* &strType) = 0;

//////////////////////////////////////////////////////////////////////////
	// 内存函数
	virtual BYTE* KernelMalloc(int nLen) = 0;
	virtual void KernelFree(BYTE** ppbyFreeBuf) = 0;
	// 取得一个递增的序号
	virtual DWORD KernelGetId() = 0;
	virtual void KernelSetIdBegin(DWORD dwIdBegin) = 0;

	// builder是否正在读取皮肤包
	virtual bool BD_IsBuilderReadingSkin() = 0;
protected:
//////////////////////////////////////////////////////////////////////////
	// Builder 使用函数
	// 设置当前为设计模式
	virtual void BD_SetDesignMode(bool bIsDesignMode) = 0;
	// 创建一个Builder使用的空的窗口
	virtual IWindowBase* BD_CreateWindowEmptyPropetry(SKIN_FILE_ITEM *pSkinFileItem) = 0;
	// 创建一个Builder使用的窗口，并配置上属性
	virtual IWindowBase* BD_CreateWindowByPropetry(SKIN_FILE_ITEM *pSkinFileItem, IPropertyWindow *pPropWnd) = 0;
	// 关闭一个工程
	virtual bool BD_CloseProject() = 0;
	// 设置可以保存的有效属性
	virtual void BD_SetWindowPropetryActiveProp(IWindowBase *pWndBase, bool bActive) = 0;
	// 删除一个windows
	virtual bool BD_DeleteWindow(IWindowBase *pWndBase) = 0;
	// 删除一个control
	virtual bool BD_DeleteControl(IControlBase *pCtrlBase) = 0;
	// 取得所有支持的控件
	virtual CONTROL_REG_TLIST *BD_GetRegisterControl() = 0;
	// 设置builder正在读取皮肤包的状态
	virtual void BD_SetBuilderReadingSkin(bool bReading) = 0;

//////////////////////////////////////////////////////////////////////////
	// 上下方向交换控件位置
	virtual bool BD_UpDownChangeControlPostion(IControlBase *pToUpCtrl, IControlBase *pToDownCtrl) = 0;
	// 转移控件到另外的控件队列中
	virtual bool BD_MoveControlToOtherParentControl(IControlBase *pFromCtrl, IFeatureObject *pToParentCtrl) = 0;
};

//////////////////////////////////////////////////////////////////////////
// 内核对【对话框】的接口
IUiFeatureKernel *GetUiKernelInterface();
typedef IUiFeatureKernel* (*GETUIKERNELINTERFACE)();
