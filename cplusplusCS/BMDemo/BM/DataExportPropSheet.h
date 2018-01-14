#pragma once
#include "EmpInfoExProp.h"
#include "CustomInfoExProp.h"
#include "ProdInfoExProp.h"


// CDataExportPropSheet

class CDataExportPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CDataExportPropSheet)

public:
	CDataExportPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDataExportPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDataExportPropSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CEmpInfoExProp m_empInfo;
	CProdInfoExProp m_prodInfo;
	CCustomInfoExProp m_ctmInfo;
public:
	virtual BOOL OnInitDialog();
	void         SetCloseBtnEable(BOOL bEnable);
};


