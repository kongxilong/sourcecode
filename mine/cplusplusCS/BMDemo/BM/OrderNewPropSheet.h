#pragma once

//#include "OrderContentProp.h"
//#include "GoodsBook.h"

// COrderNewPropSheet

class COrderNewPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COrderNewPropSheet)

public:
	COrderNewPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COrderNewPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~COrderNewPropSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//CGoodsBook m_goodsBook;
	//COrderContentProp m_orderContent;
public:
	virtual BOOL OnInitDialog();
		
};


