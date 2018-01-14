// DataExportPropSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "DataExportPropSheet.h"


// CDataExportPropSheet

IMPLEMENT_DYNAMIC(CDataExportPropSheet, CPropertySheet)

CDataExportPropSheet::CDataExportPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CDataExportPropSheet::CDataExportPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_prodInfo);
	AddPage(&m_ctmInfo);
	AddPage(&m_empInfo);
}

CDataExportPropSheet::~CDataExportPropSheet()
{
}
BOOL CDataExportPropSheet::OnInitDialog() {
	CPropertySheet::OnInitDialog();
	(CWnd *) GetDlgItem( IDOK )->ShowWindow( FALSE );
	(CWnd *) GetDlgItem( IDCANCEL )->ShowWindow( FALSE );
	(CWnd *) GetDlgItem( ID_APPLY_NOW )->ShowWindow( FALSE );
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDataExportPropSheet, CPropertySheet)
END_MESSAGE_MAP()


// CDataExportPropSheet 消息处理程序


void CDataExportPropSheet::SetCloseBtnEable(BOOL bEnable)  //TRUE 关闭按钮可用 FALSE 关闭按钮不可用
{
	CMenu *pSysMenu = (CMenu *)GetSystemMenu(FALSE);
    ASSERT(pSysMenu != NULL);
	if(bEnable){
		pSysMenu->EnableMenuItem(SC_CLOSE,MF_ENABLED);
	}else{
		pSysMenu->EnableMenuItem(SC_CLOSE,MF_DISABLED);
	}
}