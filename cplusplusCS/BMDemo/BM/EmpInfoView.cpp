// EmpInfoView.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "EmpInfoView.h"
#include "EmpInfoSearch.h"
#include "EmpNew.h"
#include "EmpEdit.h"
// CEmpInfoView

IMPLEMENT_DYNCREATE(CEmpInfoView, CFormView)

CEmpInfoView::CEmpInfoView()
	: CFormView(CEmpInfoView::IDD)
{

}

CEmpInfoView::~CEmpInfoView()
{
}

void CEmpInfoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EMP, m_list);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
}

BEGIN_MESSAGE_MAP(CEmpInfoView, CFormView)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CURSORBACKNORMAL,&CEmpInfoView::OnCursorToNormal)
	ON_BN_CLICKED(IDC_BTN_EMP_NEW, &CEmpInfoView::OnBnClickedBtnEmpNew)
	ON_BN_CLICKED(IDC_BTN_EMP_EDIT, &CEmpInfoView::OnBnClickedBtnEmpEdit)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CEmpInfoView::OnBnClickedBtnSearch)
	ON_WM_TIMER()
	ON_MESSAGE(WM_STARTTASK_EmpInfoView, &CEmpInfoView::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_EmpInfoView, &CEmpInfoView::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_EMP_DEL, &CEmpInfoView::OnBnClickedBtnEmpDel)
	ON_MESSAGE(WM_DELSELEMPINFO, &CEmpInfoView::OnDelSelEmpInfo)
END_MESSAGE_MAP()


// CEmpInfoView 诊断

#ifdef _DEBUG
void CEmpInfoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEmpInfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEmpInfoView 消息处理程序
void CEmpInfoView::InitList()
{
	m_list.SetBkColor(RGB(223, 223, 223));
	m_list.SetExtendedStyle( m_list.GetExtendedStyle()| LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.SetTextColor(RGB(139, 90, 43));
	m_list.SetTextBkColor(RGB(255, 236, 139));
	
	CRect rect;
	m_list.GetClientRect(rect);
	m_list.InsertColumn(0, _T("员工工号"), LVCFMT_LEFT, rect.Width()/4);    
    m_list.InsertColumn(1, _T("员工姓名"), LVCFMT_LEFT, rect.Width()/4);
	m_list.InsertColumn(2, _T("所属地点"), LVCFMT_LEFT, rect.Width()/4);
    m_list.InsertColumn(3, _T("职务"), LVCFMT_LEFT, rect.Width()/4);
	m_list.InsertColumn(4, _T("联系电话"), LVCFMT_LEFT, rect.Width()/4);
	m_list.InsertColumn(5, _T("备注"), LVCFMT_LEFT, rect.Width()/4);
	
	/*
	m_list.InsertItem(0, _T("0001"));    
    m_list.SetItemText(0, 1, _T("张三"));    
    m_list.SetItemText(0, 2, _T("南京"));    
    m_list.SetItemText(0, 3, _T("总经理")); 
	m_list.SetItemText(0, 4, _T("12345678901"));
	m_list.SetItemText(0, 5, _T("无")); 
	*/
}


void CEmpInfoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	CFont font;
	LOGFONT m_tempfont={22,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();

	InitList();
}

HBRUSH CEmpInfoView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		pDC->SetBkMode(TRANSPARENT);  
		//hbr=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH b_static=CreateSolidBrush(RGB(185,211,255));	
		return b_static;	
	}
	if(nCtlColor == CTLCOLOR_DLG)   //更改对话框背景色 
	{ 
		pDC->SetTextColor(RGB(0,0,0)); 
		pDC->SetBkColor(RGB(185,211,255)); 
		HBRUSH b=CreateSolidBrush(RGB(185,211,255)); 
		return b; 
	} 
	return hbr;
}


void CEmpInfoView::OnBnClickedBtnEmpNew()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_empNewDlg.SetBackgroundColor(RGB(185,211,255));
	//m_empNewDlg.DoModal();
	CEmpNew dlg(this);
	dlg.DoModal();
}


void CEmpInfoView::OnBnClickedBtnEmpEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	CString	  num_emp_ToEdit = NULL;   //将要
	CString   name_emp_ToEdit = NULL;
	CString   pos_emp_ToEdit = NULL;
	CString   phone_emp_ToEdit = NULL;
	CString   ps_emp_ToEdit = NULL;
	CString   addr_emp_ToEdit = NULL;
	int count = 0;
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		if(m_list.GetCheck(i))
		{	
			count++ ;
			if(count > 1)  //表示有多个选中项
			{
				CString str;
				str.Format(_T("当前有多个选中项，默认将编辑第一个选中项，员工号为：%s。确认继续吗"),num_emp_ToEdit);
				if(IDYES == MessageBox(str,_T("提示"),MB_YESNO)){
					break;
				}else
				{
					return;
				}
			}
			num_emp_ToEdit = m_list.GetItemText(i,0);
			name_emp_ToEdit = m_list.GetItemText(i,1);
			addr_emp_ToEdit = m_list.GetItemText(i,2);
			pos_emp_ToEdit = m_list.GetItemText(i,3);
			phone_emp_ToEdit = m_list.GetItemText(i,4);
			ps_emp_ToEdit = m_list.GetItemText(i,5);
		}
	}
	if(num_emp_ToEdit.IsEmpty())
	{
		MessageBox(_T("当前没有选中项，请至少选中一个员工进行编辑"));
		return;
	}
	CEmpEdit dlg(this);
	dlg.m_num_emp = num_emp_ToEdit;
	dlg.m_name_emp = name_emp_ToEdit;
	dlg.m_pos_emp = pos_emp_ToEdit;
	dlg.m_addr_emp = addr_emp_ToEdit;
	dlg.m_ps_emp = ps_emp_ToEdit;
	dlg.m_phone_emp = phone_emp_ToEdit;
	dlg.DoModal();
}


void CEmpInfoView::OnBnClickedBtnSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CEmpInfoSearch empSearchDlg(this);
	empSearchDlg.DoModal();
}

afx_msg LRESULT CEmpInfoView::OnCursorToNormal(WPARAM wParam, LPARAM lParam)
{
	SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR ,(LONG)LoadCursor(NULL , IDC_ARROW));
	return 0;
}

void CEmpInfoView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{
			ShowTip();
			break;
		}
	default:
		break;
	}
	CFormView::OnTimer(nIDEvent);
}

void CEmpInfoView::ShowTip()
{
	m_time_take++;
	CString tip;
	tip.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowText(tip);
	if(m_time_take == 60)
	{
		m_time_take = 0;
		KillTimer(1);
		m_tip.SetWindowText(_T("连接超时，请检查服务器连接状态并重试"));
	}
}


afx_msg LRESULT CEmpInfoView::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CEmpInfoView::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T("就绪"));
	return 0;
}


void CEmpInfoView::OnBnClickedBtnEmpDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int count = 0;
	CString num;
	CString  lineToDelete = _T("将要被删除的员工的工号是：");
	m_empToDelete.Empty();
	int total = m_list.GetItemCount();
	for(int i=0; i< total; i++)
	{
		//if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ||m_list.GetCheck(i))
		if(m_list.GetCheck(i))
		{	
			count++;
			m_empToDelete += m_list.GetItemText(i,0);
			m_empToDelete += ",";   //两个员工之间的分隔符
		}
	}
	if(count == 0)
	{
		MessageBox(_T("请先至少勾选一个员工，然后再删除"));
		return;
	}else{   //有选中项
		CString total;
		total.Format(_T("共有%d个选中项，"),count);
		lineToDelete += m_empToDelete + _T("确认删除吗？");
		if(IDYES == MessageBox(total+lineToDelete,_T("警告"),MB_YESNO))
		{
			CSock::StartDelEmpInfo(this);
		}else{
			return;
		}
	}
}




afx_msg LRESULT CEmpInfoView::OnDelSelEmpInfo(WPARAM wParam, LPARAM lParam)
{
	CString _strToDelete = m_empToDelete;
	CString _strToFind = _T(",");    //找到这样的标志位 进行字符段的截取
	while(TRUE)
	{
		int _strEnd = _strToDelete.Find(_strToFind);
		if(-1 == _strEnd)   //没有找到
		{
			break;
		}else{
			CString temp = _strToDelete.Mid(0,_strEnd);
			int num = m_list.GetItemCount();
			for(int i = 0;i< num;i++){
				if(temp == m_list.GetItemText(i,0))
				{
					m_list.DeleteItem(i);
				}
			}
			_strToDelete = _strToDelete.Mid(_strEnd+1);
		}
	}
	return 0;
}

