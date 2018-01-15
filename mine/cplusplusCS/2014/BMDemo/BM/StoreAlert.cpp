// StoreAlert.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "StoreAlert.h"
#include "afxdialogex.h"


// CStoreAlert 对话框

IMPLEMENT_DYNAMIC(CStoreAlert, CDialogEx)

CStoreAlert::CStoreAlert(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStoreAlert::IDD, pParent)
{

}

CStoreAlert::~CStoreAlert()
{
}

void CStoreAlert::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STOREINFO, m_list);
}


BEGIN_MESSAGE_MAP(CStoreAlert, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CStoreAlert 消息处理程序


HBRUSH CStoreAlert::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		pDC->SetBkMode(TRANSPARENT);  
		//hbr=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH b_static=CreateSolidBrush(BACKGROUNDCOLOR);	
		return b_static;	
	}
	if(nCtlColor == CTLCOLOR_DLG){
		pDC->SetTextColor(RGB(0,0,0)); 
		pDC->SetBkColor(RGB(185,211,255)); 
		HBRUSH b=CreateSolidBrush(RGB(185,211,255)); 
		return b;
	}
	return hbr;
}
