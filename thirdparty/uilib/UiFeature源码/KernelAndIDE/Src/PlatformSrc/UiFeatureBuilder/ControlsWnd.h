
#pragma once
#include "FeatureControlList.h"
#include "..\..\Inc\IControlBase.h"

struct CONTROL_LIST_GROUP
{
	// 控件所在的控件组名称
	CStringW strCtrlGroupName;
	CONTROL_REG_TLIST ItemMap;
};
// KEY:strCtrlGroupName
typedef map<CStringW, CONTROL_LIST_GROUP*>	CONTROL_LIST_GROUP_MAP;


class CControlsToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CControlsWnd : public CDockablePane
{
// 构造
public:
	CControlsWnd();
	virtual ~CControlsWnd();

	void AdjustLayout();

	// 设置控件列表
	void SetControlList(CONTROL_REG_TLIST* pRegControlMap);
	void SetProjectInitState(bool bInitOk);
	CFeatureControlList *GetControlList();

private:
	void SetCurrentSelectList(LPCWSTR strGroupName);
	void FreeControlRegInfoMap();

private:
	CComboBox m_wndObjectCombo;
	CFeatureControlList m_wndCtrlList;

	CONTROL_LIST_GROUP_MAP	m_ListGroupMap;

// 实现
public:

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGroupSelchangeCombo();

	DECLARE_MESSAGE_MAP()
};

