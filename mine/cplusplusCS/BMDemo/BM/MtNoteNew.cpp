// MtNoteNew.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "MtNoteNew.h"
#include "afxdialogex.h"
#include "MtSetProp.h"

// CMtNoteNew 对话框

IMPLEMENT_DYNAMIC(CMtNoteNew, CDialogEx)

CMtNoteNew::CMtNoteNew(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMtNoteNew::IDD, pParent)
	, m_parentNode(_T(""))
	, m_nodeNew(_T(""))
{

}

CMtNoteNew::CMtNoteNew(CMtSetProp* pMtSetProp,CWnd* pParent /*=NULL*/)
	: CDialogEx(CMtNoteNew::IDD, pParent)
{
	m_pMtSetProp = pMtSetProp;
}

CMtNoteNew::~CMtNoteNew()
{
	m_pMtSetProp = NULL;
}

void CMtNoteNew::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PARENT_NODE, m_parentNode);
	DDX_Text(pDX, IDC_EDIT_NODE_NEW, m_nodeNew);
}


BEGIN_MESSAGE_MAP(CMtNoteNew, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_NOTE_NEW, &CMtNoteNew::OnBnClickedBtnNoteNew)
	//ON_EN_CHANGE(IDC_EDIT1, &CMtNoteNew::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BTN_QUIT, &CMtNoteNew::OnBnClickedBtnQuit)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMtNoteNew 消息处理程序


void CMtNoteNew::OnBnClickedBtnNoteNew()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_parentNode.IsEmpty())
	{
		MessageBox(_T("上级节点不能为空！"));
		return;
	}
	if(m_nodeNew.IsEmpty())
	{
		MessageBox(_T("新建节点不能为空！"));
		return;
	}
	if(m_nodeNew == m_parentNode)
	{
		MessageBox(_T("新建节点不能和上级节点名字相同！"));
		return;
	}
	if(!((-1 == m_parentNode.Find(_T("\\")))
		&&(-1 == m_parentNode.Find(_T("|")))
		&&(-1 == m_parentNode.Find(_T("<")))
		&&(-1 == m_parentNode.Find(_T(">")))
		))
	{
		MessageBox(_T("上级节点不能包含'\\','|','<','>'等特殊字符！"));
		return;
	}
	if(!((-1 == m_nodeNew.Find(_T("\\")))
		&&(-1 == m_nodeNew.Find(_T("|")))
		&&(-1 == m_nodeNew.Find(_T("<")))
		&&(-1 == m_nodeNew.Find(_T(">")))
		))
	{
		MessageBox(_T("新建节点不能包含'\\','|','<','>'等特殊字符！"));
		return;
	}
	if(m_parentNode.GetLength()>25 ||m_nodeNew.GetLength()>25)
	{
		MessageBox(_T("节点名称不能大于25个字符！"));
		return;
	}
	m_pMtSetProp->m_parentNodeName = m_parentNode;
	m_pMtSetProp->m_nodeNameNew = m_nodeNew;
	//void* dlg = m_pMtSetProp;
	m_pMtSetProp->SendMessage(WM_REQNOTENEW_MtSetProp,0,0);
	CDialogEx::OnOK();
}





void CMtNoteNew::OnBnClickedBtnQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


HBRUSH CMtNoteNew::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
