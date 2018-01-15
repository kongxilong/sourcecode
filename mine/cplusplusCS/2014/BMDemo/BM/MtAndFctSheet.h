#pragma once

#include "MtSetProp.h"
#include "FctSet.h"

// CMtAndFctSheet

class CMtAndFctSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMtAndFctSheet)

public:
	CMtAndFctSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMtAndFctSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMtAndFctSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CMtSetProp m_mtSet;
	CFctSet m_fctSet;
	//CString m_test;
public:
	virtual BOOL OnInitDialog();
	//afx_msg void OnDestroy();
	afx_msg void OnClose();
	void  SetCloseBtnEable(BOOL bEnable);
};


