#pragma once
#include "afxwin.h"


// COrderContentProp 对话框

class COrderContentProp : public CPropertyPage
{
	DECLARE_DYNAMIC(COrderContentProp)

public:
	COrderContentProp();
	virtual ~COrderContentProp();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_ORDER_CONTENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ctmFrom;
//	CString m_intrNM;
//	int m_intrPhone;
	CStatic m_staticIntrName;
	CStatic m_staticIntrPhone;
	CStatic m_staticServPersonNM;
//	CString m_servPersonName;
	afx_msg void OnCbnSelchangeComboCtmFrom();
public:
	
	CEdit m_intrName;
	CEdit m_intrPhone;
	CEdit m_servPerson;
	CStatic m_static_emp_done;
	CEdit m_empName;
	//afx_msg void OnEnChangeEditAccessoryFilePath();
	CComboBox m_businessProg;
	CComboBox m_payDealChoose;
	CComboBox m_orderDiscussType;
	CComboBox m_earlyStageType;
public:
	virtual BOOL OnInitDialog();
	void InitCtmFromComb();
	void InitBusinessProgComb();
	void InitPayDealChooseComb();
	void InitOrderDiscussTypeComb();
	void InitEarlyStageTypeComb();
};
