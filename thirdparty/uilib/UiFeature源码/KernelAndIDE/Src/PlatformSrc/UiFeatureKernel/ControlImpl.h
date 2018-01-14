
#pragma once
#include "..\..\Inc\IUiFeatureKernel.h"
#include "..\..\Inc\ICtrlInterface.h"

#include <atlstr.h>
using namespace ATL;

struct CONTRL_DLL_INFO 
{
	HMODULE hDll;
	IControlManager* pCtrlMgr;
	CStringW strPath;
};

typedef vector<CONTRL_DLL_INFO>		CONTROL_DLL_VEC;

class IUiFeatureKernel;
class CControlImpl
{
public:
	CControlImpl(IUiFeatureKernel* pUiKernel);
	virtual ~CControlImpl(void);

	// 取得所有支持的控件
	void SetRegControlMap(CONTROL_REG_TLIST *pCtrlMap);
	// 创建一个控件，参数为步骤1的宏
	ICtrlInterface* CreateCtrl(LPCWSTR pCtrlType);
	// 销毁一个控件
	bool ReleaseCtrl(ICtrlInterface **ppCtrl);

private:
	bool LoadControlDll();

private:
	CONTROL_DLL_VEC m_CtrlDllVec;
	IUiFeatureKernel* m_pUiKernel;
};
