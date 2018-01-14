#pragma once
#include "EmpInfoProp.h"
#include "CustomInfoProp.h"
#include "ProdInfoProp.h"


// CDataImportPropSheet

class CDataImportPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CDataImportPropSheet)

public:
	CDataImportPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDataImportPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDataImportPropSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CCustomInfoProp m_customInfoProp;
	CProdInfoProp m_prodInfoProp;
	CEmpInfoProp m_empInfoProp;
public:
	virtual BOOL OnInitDialog();
	void         SetCloseBtnEable(BOOL bEnbale);
};


