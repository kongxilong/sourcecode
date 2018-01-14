
#pragma once
#include "..\..\Inc\IUiFeatureKernel.h"
#include "IUiEngineImpl.h"
#include "IPropertySkinManagerImpl.h"
#include "IZipFileImpl.h"
#include "..\..\Inc\IControlManager.h"
#include "CriSec.h"
#include "ControlImpl.h"

class IWindowBaseImpl;
typedef map<HWND, IWindowBaseImpl*>		WINDOW_IMPL_MAP;

class IUiFeatureKernelImpl : public IUiFeatureKernel
{
public:
	IUiFeatureKernelImpl(void);
	virtual ~IUiFeatureKernelImpl(void);

	static IUiFeatureKernel* GetInstance();

	virtual IPropertySkinManager* GetSkinManager();
	virtual IUiEngine* GetUiEngine();

	// 创建一个含有未设置属性的控件
	virtual IControlBase* CreateControlEmptyPropetry(SKIN_FILE_ITEM* pSkinFileItem, IWindowBase *pParentWnd, IControlBase *pParentCtrl,
		LPCWSTR pszNewCtrlTypeName, LPCWSTR pszNewCtrlName, LPCWSTR pszDftSkinDir, bool bIsCloneCtrl);
	// 创建一个的控件，并配置上属性
	virtual IControlBase* CreateControlByPropetry(IWindowBase *pParentWnd, IControlBase *pParentCtrl, IPropertyControl *pPropCtrl);

	// 是否为设计模式
	virtual bool IsDesignMode();

//////////////////////////////////////////////////////////////////////////
	// 一个对话框从一个皮肤包里使用指定的对话框皮肤资源初始化自己
	virtual IWindowBase* PG_InitFeatureSkin(HWND hWnd, LPCWSTR pszSkinPath, LPCWSTR pszWndName, char *pszPassword);
	// 一个对话框释放皮肤资源，这个Kernel内核会调用，外部对话框不需要调用
	virtual void PG_ReleaseFeatureSkin(HWND hWnd);
	// 取得全局唯一定时器号
	virtual int GetTimerId();
	// 退出程序前，调用，卸载kernel的资源
	virtual void UnInitialized();

	// 内存函数
	virtual BYTE* KernelMalloc(int nLen);
	virtual void KernelFree(BYTE** ppbyFreeBuf);
	// 取得一个递增的序号
	virtual DWORD KernelGetId();
	virtual void KernelSetIdBegin(DWORD dwIdBegin);

protected:
//////////////////////////////////////////////////////////////////////////
	// Builder 使用函数
	// 设置当前为设计模式
	virtual void BD_SetDesignMode(bool bIsDesignMode);
	// 取得所有支持的控件
	virtual CONTROL_REG_TLIST *BD_GetRegisterControl();
	// 创建一个Builder使用的空的窗口
	virtual IWindowBase* BD_CreateWindowEmptyPropetry(SKIN_FILE_ITEM *pSkinFileItem);
	// 创建一个Builder使用的窗口，并配置上属性
	virtual IWindowBase* BD_CreateWindowByPropetry(SKIN_FILE_ITEM *pSkinFileItem, IPropertyWindow *pPropWnd);
	// 关闭一个工程
	virtual bool BD_CloseProject();
	// 设置可以保存的有效属性
	virtual void BD_SetWindowPropetryActiveProp(IWindowBase *pWndBase, bool bActive);
	// 删除一个windows
	virtual bool BD_DeleteWindow(IWindowBase *pWndBase);
	// 删除一个control
	virtual bool BD_DeleteControl(IControlBase *pCtrlBase);
	// 上下方向交换控件位置
	virtual bool BD_UpDownChangeControlPostion(IControlBase *pToUpCtrl, IControlBase *pToDownCtrl);
	// 转移控件到另外的控件队列中
	virtual bool BD_MoveControlToOtherParentControl(IControlBase *pFromCtrl, IFeatureObject *pToParentCtrl);
	// 设置builder正在读取皮肤包的状态
	virtual void BD_SetBuilderReadingSkin(bool bReading);
	virtual bool BD_IsBuilderReadingSkin();

private:
	void ReleaseKernelWindow();
	bool BD_DeleteControl_FromCtrlVec(CHILD_CTRLS_VEC* pCtrlVec, IControlBase *pCtrlBase);
	void ClearCtrlRegList();

	// 取得所有支持的动画列表
	virtual int GetAnimationList(const char* &strType);
	// 设置自控件的默认皮肤
	BOOL SetSubControlDftSkin(SKIN_FILE_ITEM* pSkinFileItem, IControlBase *pParentCtrl, LPCWSTR pszDftSkinDir);

private:
	static ULONG_PTR m_gdiplusToken;

	// 线程同步
	CCriSec m_KernelSec;
	// 是否为设计模式
	bool m_bIsDesignMode;

	CControlImpl* m_pCtrlDllMgr;
	IUiEngineImpl *m_pUiEngine;
	IPropertySkinManagerImpl *m_pSkinMgr;

	WINDOW_IMPL_MAP m_WndImplMap;
	// 所有支持的控件
	CONTROL_REG_TLIST m_CtrlRegMap;
	// Builder中使用的hwnd的索引值
	int m_nBuilderHwnd;
	// 全局唯一定时器号
	int m_nTimerId;
	// 取得一个递增的序号
	DWORD m_dwKernelId;
	// 是否Builder正在读取皮肤包
	bool m_bBuilderReadingSkin;
};
