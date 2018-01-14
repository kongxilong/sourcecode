
#pragma once
#include "..\..\Inc\IControlManager.h"

class IControlManagerImpl : public IControlManager
{
public:
	IControlManagerImpl(void);
	virtual ~IControlManagerImpl(void);

	static IControlManager* GetInstance();

	virtual void SetUiKernel(IUiFeatureKernel* pUiKernel);
	// 取得所有支持的控件
	virtual void SetRegControlMap(CONTROL_REG_TLIST *pCtrlMap);
	// 创建一个控件，参数为步骤1的宏
	virtual ICtrlInterface* CreateCtrl(LPCWSTR pCtrlType);
	// 销毁一个控件
	virtual bool ReleaseCtrl(ICtrlInterface **ppCtrl);
	// 删除一个控件的注册信息
	virtual void DeleteRegControlInfo(CONTROL_REG* pReg);

private:
	// 设置一个支持的控件的节点
	void SetRegControl(CONTROL_REG_TLIST *pCtrlMap, CStringW strCtrlGroupName, CStringW strCtrlName, CStringW strControlInfo);

private:
	IUiFeatureKernel* m_pUiKernel;
};
