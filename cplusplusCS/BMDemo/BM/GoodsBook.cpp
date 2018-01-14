// GoodsBook.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "GoodsBook.h"
#include "afxdialogex.h"


// CGoodsBook 对话框

IMPLEMENT_DYNAMIC(CGoodsBook, CPropertyPage)

CGoodsBook::CGoodsBook()
	: CPropertyPage(CGoodsBook::IDD)
{

}

CGoodsBook::~CGoodsBook()
{
}

void CGoodsBook::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MT_IN_GOODS_BOOK, m_list);
}


BEGIN_MESSAGE_MAP(CGoodsBook, CPropertyPage)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MT_IN_GOODS_BOOK, &CGoodsBook::OnLvnItemchangedListMtInGoodsBook)
	ON_BN_CLICKED(IDC_BTN_MT_ADD, &CGoodsBook::OnBnClickedBtnMtAdd)
END_MESSAGE_MAP()


// CGoodsBook 消息处理程序

BOOL CGoodsBook::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	InitList();
	return TRUE;
}

void CGoodsBook::InitList()
{
	m_list.SetExtendedStyle( m_list.GetExtendedStyle()| LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.SetTextColor(RGB(139, 90, 43));
	m_list.SetTextBkColor(RGB(255, 236, 139));

	CRect rect;
	m_list.GetClientRect(rect);
	
	m_list.InsertColumn(0, _T("材料类型"), LVCFMT_LEFT, rect.Width()/6);    
    m_list.InsertColumn(1, _T("材料名称"), LVCFMT_LEFT, rect.Width()/6);
	m_list.InsertColumn(2, _T("所需数量"), LVCFMT_LEFT, rect.Width()/6);
    m_list.InsertColumn(3, _T("供货厂家"), LVCFMT_LEFT, rect.Width()/6);

	m_list.InsertColumn(4, _T("备注"), LVCFMT_LEFT, rect.Width()/3);    
    

	m_list.InsertItem(0, _T("00000"));    
    m_list.SetItemText(0, 1, _T("0"));    
    m_list.SetItemText(0, 2, _T("0"));    
    m_list.SetItemText(0, 3, _T("0"));    
    m_list.SetItemText(0, 4, _T("0")); 
   
   
}




void CGoodsBook::OnBnClickedBtnMtAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}
