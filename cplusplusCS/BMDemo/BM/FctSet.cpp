// FctSet.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "FctSet.h"
#include "afxdialogex.h"
#include "FactoryNew.h"
#include "FactoryEdit.h"
#include "MtAndFctSheet.h"
// CFctSet 对话框

IMPLEMENT_DYNAMIC(CFctSet, CPropertyPage)

CFctSet::CFctSet()
	: CPropertyPage(CFctSet::IDD)
{

}

CFctSet::~CFctSet()
{
}

void CFctSet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FCTORY_INFO, m_list);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
}


BEGIN_MESSAGE_MAP(CFctSet, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_FCTNEW, &CFctSet::OnBnClickedBtnFctNew)
	ON_BN_CLICKED(IDC_BTN_FCTEDIT, &CFctSet::OnBnClickedBtnFctEdit)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SEARCHALL, &CFctSet::OnBnClickedBtnSearchAll)
	ON_WM_TIMER()
	ON_MESSAGE(WM_STARTTASK_FctSet, &CFctSet::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_FctSet, &CFctSet::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_FCTDELETE, &CFctSet::OnBnClickedBtnFctdelete)
	ON_MESSAGE(WM_DEL_FCT_FctSet, &CFctSet::OnDelFct)
END_MESSAGE_MAP()


// CFctSet 消息处理程序

BOOL CFctSet::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();
	
	InitList();
	return TRUE;
}

void CFctSet::InitList()
{
	m_list.SetBkColor(RGB(223,223,223));
	m_list.SetExtendedStyle( m_list.GetExtendedStyle()| LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.SetTextColor(RGB(139, 90, 43));
	m_list.SetTextBkColor(RGB(255, 236, 139));
	
	CRect rect;
	m_list.GetClientRect(rect);
	m_list.InsertColumn(0, _T("厂家名称"), LVCFMT_LEFT, rect.Width()/4);    
    m_list.InsertColumn(1, _T("地址"), LVCFMT_LEFT, rect.Width()/4);
	m_list.InsertColumn(2, _T("联系人"), LVCFMT_LEFT, rect.Width()/4);
    m_list.InsertColumn(3, _T("主要产品"), LVCFMT_LEFT, rect.Width()/4);
	m_list.InsertColumn(4, _T("联系电话"), LVCFMT_LEFT, rect.Width()/4);
	m_list.InsertColumn(5, _T("备注"), LVCFMT_LEFT, rect.Width()/4);
	
	
	/*
	m_list.InsertItem(0, _T("五金厂"));    
    m_list.SetItemText(0, 1, _T("南京"));    
    m_list.SetItemText(0, 2, _T("张三"));    
    m_list.SetItemText(0, 3, _T("五金")); 
	m_list.SetItemText(0, 4, _T("12345678901"));
	m_list.SetItemText(0, 5, _T("无")); 
	*/
}

void CFctSet::OnBnClickedBtnFctNew()
{
	// TODO: 在此添加控件通知处理程序代码
	CFactoryNew ftNewDlg(this);
	ftNewDlg.DoModal();
}


void CFctSet::OnBnClickedBtnFctEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	CString	  name_fct = NULL;   //将要
	CString   addr_fct = NULL;
	CString   contacts_fct = NULL;
	CString   main_products_fct = NULL;
	CString   phone_fct = NULL;
	CString   ps_fct = NULL;
	int count = 0;
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		//if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ||m_list.GetCheck(i))
		if(m_list.GetCheck(i))
		{	
			count++ ;
			if(count > 1)  //表示有多个选中项
			{
				CString str;
				str.Format(_T("当前有多个选中项，默认将编辑第一个选中项，厂家名称为：'%s'。确认继续吗"),name_fct);
				if(IDYES == MessageBox(str,_T("提示"),MB_YESNO)){
					break;
				}else
				{
					return;
				}
			}
			name_fct = m_list.GetItemText(i,0);
			addr_fct = m_list.GetItemText(i,1);
			contacts_fct = m_list.GetItemText(i,2);
			main_products_fct = m_list.GetItemText(i,3);
			phone_fct = m_list.GetItemText(i,4);
			ps_fct = m_list.GetItemText(i,5);
		}
	}
	if(name_fct.IsEmpty())
	{
		MessageBox(_T("当前没有选中项，请至少选中一个厂家进行编辑"));
		return;
	}
	CFactoryEdit dlg(this);
	dlg.m_name_fctEdit = name_fct;
	dlg.m_addr_fctEdit = addr_fct;
	dlg.m_contact_person_fctEdit = contacts_fct;
	dlg.m_main_product_fctEdit = main_products_fct;
	dlg.m_phone_fctEdit = phone_fct;
	dlg.m_ps_fctEdit = ps_fct;
	dlg.DoModal();

}


HBRUSH CFctSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		pDC->SetBkMode(TRANSPARENT);  
		//hbr=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH b_static=CreateSolidBrush(BACKGROUNDCOLOR);	
		return b_static;	
	}
	if(nCtlColor == CTLCOLOR_DLG){
		pDC->SetTextColor(RGB(0,0,0)); 
		pDC->SetBkColor(BACKGROUNDCOLOR); 
		HBRUSH b=CreateSolidBrush(BACKGROUNDCOLOR); 
		return b;
	}
	return hbr;
}

void CFctSet::EnableParentDlgShutDownBtn(BOOL nEnable)   //禁用属性页关闭窗口按钮
{
	CMtAndFctSheet * p_parentDlg = (CMtAndFctSheet *)GetParent();
	//TRUE表示可用 FALSE表示不可用   
	p_parentDlg->SetCloseBtnEable(nEnable);
}

void CFctSet::OnBnClickedBtnSearchAll()
{
	// TODO: 在此添加控件通知处理程序代码
	CSock::StartFindALLFct(this);
}


void CFctSet::OnTimer(UINT_PTR nIDEvent)
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
	CPropertyPage::OnTimer(nIDEvent);
}

void CFctSet::ShowTip()
{
	m_time_take++;
	CString tip;
	tip.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowTextW(tip);
	if(m_time_take == 60)
	{
		OnEndTask(0,0);
		m_tip.SetWindowText(_T("请求超时，请重新检查服务器连接状态，并重试"));
	}
}

afx_msg LRESULT CFctSet::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	EnableParentDlgShutDownBtn(FALSE);
	//SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CFctSet::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	EnableParentDlgShutDownBtn(TRUE);
	//KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


void CFctSet::OnBnClickedBtnFctdelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int count = 0;
	CString num;
	CString  lineToDelete = _T("将要被删除的厂家的名字是：");
	m_strfctToDel.Empty();
	int total = m_list.GetItemCount();
	for(int i=0; i< total; i++)
	{
		if(m_list.GetCheck(i))
		{	
			count++;
			m_strfctToDel += m_list.GetItemText(i,0);
			m_strfctToDel += ",";   //两个名字段的分隔符
		}
	}
	if(count == 0)
	{
		MessageBox(_T("请先至少勾选一个厂家，然后再删除"));
		return;
	}else{   //有选中项
		lineToDelete += m_strfctToDel + _T("确认删除吗？");
		if(IDYES == MessageBox(lineToDelete,_T("警告"),MB_YESNO))
		{
			CSock::StartDelFct(this);
		}else{
			return;
		}
	}
}


afx_msg LRESULT CFctSet::OnDelFct(WPARAM wParam, LPARAM lParam)
{
	CString _strToDelete = m_strfctToDel;
	CString temp; 
	CString _strToFind = _T(",");    //找到这样的标志位 进行字符段的截取
	while(TRUE)
	{
		int _strEnd = _strToDelete.Find(_strToFind);
		if(-1 == _strEnd)   //没有找到
		{
			break;
		}else{
			temp = _strToDelete.Mid(0,_strEnd);
			int num =m_list.GetItemCount();
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

