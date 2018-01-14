// DataImportPropSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "DataImportPropSheet.h"


// CDataImportPropSheet

IMPLEMENT_DYNAMIC(CDataImportPropSheet, CPropertySheet)

CDataImportPropSheet::CDataImportPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_prodInfoProp);
	AddPage(&m_customInfoProp);
	AddPage(&m_empInfoProp);

}

CDataImportPropSheet::CDataImportPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_prodInfoProp);
	AddPage(&m_customInfoProp);
	AddPage(&m_empInfoProp);
}

CDataImportPropSheet::~CDataImportPropSheet()
{
}


BEGIN_MESSAGE_MAP(CDataImportPropSheet, CPropertySheet)
END_MESSAGE_MAP()

BOOL CDataImportPropSheet::OnInitDialog() {
	CPropertySheet::OnInitDialog();
	(CWnd *) GetDlgItem( IDOK )->ShowWindow( FALSE );
	(CWnd *) GetDlgItem( IDCANCEL )->ShowWindow( FALSE );
	(CWnd *) GetDlgItem( ID_APPLY_NOW )->ShowWindow( FALSE );
	return TRUE;
}

// CDataImportPropSheet 消息处理程序
void CDataImportPropSheet::SetCloseBtnEable(BOOL bEnable)
{
	CMenu *pSysMenu = (CMenu *)GetSystemMenu(FALSE);
    ASSERT(pSysMenu != NULL);
	if(bEnable){
		pSysMenu->EnableMenuItem(SC_CLOSE,MF_ENABLED);
	}else{
		pSysMenu->EnableMenuItem(SC_CLOSE,MF_DISABLED);
	}
}