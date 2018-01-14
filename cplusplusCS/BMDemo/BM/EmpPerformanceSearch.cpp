// EmpPerformanceSearch.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "EmpPerformanceSearch.h"
#include "afxdialogex.h"


// CEmpPerformanceSearch 对话框

IMPLEMENT_DYNAMIC(CEmpPerformanceSearch, CDialogEx)

CEmpPerformanceSearch::CEmpPerformanceSearch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEmpPerformanceSearch::IDD, pParent)
{

}

CEmpPerformanceSearch::~CEmpPerformanceSearch()
{
}

void CEmpPerformanceSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEmpPerformanceSearch, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SEACH, &CEmpPerformanceSearch::OnBnClickedBtnSeach)
END_MESSAGE_MAP()


// CEmpPerformanceSearch 消息处理程序


HBRUSH CEmpPerformanceSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		pDC->SetBkMode(TRANSPARENT);  
		//hbr=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH b_static=CreateSolidBrush(RGB(185,211,255));	
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


void CEmpPerformanceSearch::OnBnClickedBtnSeach()
{
	// TODO: 在此添加控件通知处理程序代码
}
