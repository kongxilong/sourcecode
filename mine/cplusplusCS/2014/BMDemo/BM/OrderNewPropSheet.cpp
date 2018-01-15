// OrderNewPropSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "OrderNewPropSheet.h"


// COrderNewPropSheet

IMPLEMENT_DYNAMIC(COrderNewPropSheet, CPropertySheet)

COrderNewPropSheet::COrderNewPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

COrderNewPropSheet::COrderNewPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	//AddPage(&m_orderContent);
	//AddPage(&m_goodsBook);
}

COrderNewPropSheet::~COrderNewPropSheet()
{
}


BEGIN_MESSAGE_MAP(COrderNewPropSheet, CPropertySheet)
END_MESSAGE_MAP()

BOOL COrderNewPropSheet::OnInitDialog()
{
	CPropertySheet::OnInitDialog();
	//(CWnd *) GetDlgItem( IDOK )->ShowWindow( FALSE );
	//(CWnd *) GetDlgItem( IDCANCEL )->ShowWindow( FALSE );
	((CButton *) GetDlgItem( IDOK ))->SetWindowText(_T("新建订单"));
	((CButton *) GetDlgItem( IDCANCEL ))->SetWindowText(_T("放弃"));
	CRect rectOk;
	((CButton *) GetDlgItem( IDOK ))->GetClientRect(rectOk);
	CRect rectClient;
	this->GetClientRect(rectClient);
	int rectOkWidth = rectOk.right - rectOk.left;
	int rectOkHeight = rectOk.bottom - rectOk.top;
	rectOk.left = (rectClient.right - rectClient.left)/2 - rectOkWidth - 20;
	rectOk.right = rectOk.left + rectOkWidth ;
	rectOk.bottom = rectClient.bottom - 10 ;
	rectOk.top = rectOk.bottom - rectOkHeight;

	//调整OK按钮的位置
	((CButton *) GetDlgItem( IDOK ))->MoveWindow(rectOk,0);
	
	CRect rectCancel;
	rectCancel.top = rectOk.top;
	rectCancel.bottom = rectOk.bottom;
	rectCancel.left = (rectClient.right - rectClient.left)/2  + 20;
	rectCancel.right = rectCancel.left + rectOkWidth; 
	
	((CButton *) GetDlgItem( IDCANCEL ))->MoveWindow(rectCancel);
	(CWnd *) GetDlgItem( ID_APPLY_NOW )->ShowWindow( FALSE );
	return TRUE;
}
// COrderNewPropSheet 消息处理程序
