// CtmInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "CtmInfo.h"
#include "afxdialogex.h"
#include "CtmNew.h"
#include "CtmEdit.h"



// CCtmInfo 对话框

IMPLEMENT_DYNAMIC(CCtmInfo, CDialogEx)

CCtmInfo::CCtmInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCtmInfo::IDD, pParent)
{

}



CCtmInfo::~CCtmInfo()
{
}

void CCtmInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CTMINFO, m_list);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
}


BOOL CCtmInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

void CCtmInfo::InitList()
{
	m_list.SetBkColor(CONCTRLBKCOLOR);
	m_list.SetExtendedStyle( m_list.GetExtendedStyle()| LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.SetTextColor(RGB(139, 90, 43));
	m_list.SetTextBkColor(RGB(255, 236, 139));
	
	m_list.InsertColumn(0, _T("客户编号"), LVCFMT_LEFT, 150);    
    m_list.InsertColumn(1, _T("客户名称"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, _T("顾客姓名"), LVCFMT_LEFT, 100);
    m_list.InsertColumn(3, _T("性别"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(4, _T("电话"), LVCFMT_LEFT, 150);
	m_list.InsertColumn(5, _T("销售次数"), LVCFMT_LEFT, 100);   
	m_list.InsertColumn(6, _T("金额"), LVCFMT_LEFT, 100);    
	m_list.InsertColumn(7, _T("首次销售日期"), LVCFMT_LEFT, 100);    
	m_list.InsertColumn(8, _T("最近消费日期"), LVCFMT_LEFT, 100);    
	m_list.InsertColumn(9, _T("备注"), LVCFMT_LEFT, 100);    

	//m_list.InsertItem(0, _T("00000"));    
   // m_list.SetItemText(0, 1, _T("张三"));    
    //m_list.SetItemText(0, 2, _T("南京"));    
    //m_list.SetItemText(0, 3, _T("总经理"));    
    //m_list.SetItemText(0, 4, _T("12345678901")); 
	//m_list.SetItemText(0, 5, _T("无"));
}

BEGIN_MESSAGE_MAP(CCtmInfo, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_NEW, &CCtmInfo::OnBnClickedBtnNew)
	ON_BN_CLICKED(IDC_BTN_EDIT, &CCtmInfo::OnBnClickedBtnEdit)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_STARTTASK_CtmInfo, &CCtmInfo::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_CtmInfo, &CCtmInfo::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_SEARCHALL, &CCtmInfo::OnBnClickedBtnSearchall)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CCtmInfo::OnBnClickedBtnDelete)
	ON_MESSAGE(WM_DELCTM_CtmInfo, &CCtmInfo::OnDelCtm)
END_MESSAGE_MAP()


// CCtmInfo 消息处理程序


void CCtmInfo::OnBnClickedBtnNew()
{
	// TODO: 在此添加控件通知处理程序代码
	CCtmNew dlg(this);
	dlg.DoModal();
}


void CCtmInfo::OnBnClickedBtnEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	CString	  NO = NULL;   //将要
	CString   call = NULL;
	CString   name = NULL;
	CString   sex = NULL;
	CString   phone = NULL;
	CString   times = NULL;
	CString   paytotal = NULL;
	CString   firsttime = NULL;
	CString   latetime = NULL;
	CString   ps = NULL;

	int count = 0;
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		if(m_list.GetCheck(i))
		{	
			count++ ;
			if(count > 1)  //表示有多个选中项
			{
				CString str;
				str.Format(_T("当前有多个选中项，默认将编辑第一个选中项，客户编号为：%s。确认继续吗"),NO);
				if(IDYES == MessageBox(str,_T("提示"),MB_YESNO)){
					break;
				}else
				{
					return;
				}
			}
			NO = m_list.GetItemText(i,0);
			call = m_list.GetItemText(i,1);
			name = m_list.GetItemText(i,2);
			sex = m_list.GetItemText(i,3);
			phone = m_list.GetItemText(i,4);
			times = m_list.GetItemText(i,5);
			paytotal = m_list.GetItemText(i,6);
			firsttime = m_list.GetItemText(i,7);
			latetime = m_list.GetItemText(i,8);
			ps = m_list.GetItemText(i,9);
		}
	}
	if(NO.IsEmpty())
	{
		MessageBox(_T("当前没有选中项，请至少选中一个客户进行编辑"));
		return;
	}
	USES_CONVERSION;
	char *p = NULL;
	CCtmEdit dlg(this);
	dlg.m_ctmNM = name;
	p = T2A(NO);
	dlg.m_ctmNum = atoi(p);
	dlg.m_ctmCall = call;
	//dlg.LoadCombOfSex(sex);
	dlg.m_phone = phone;
	dlg.m_sexflag = sex;
	dlg.m_times = atoi(T2A(times));
	dlg.m_price = atoi(T2A(paytotal));
	dlg.m_timeFirst = firsttime;
	dlg.m_timeLate = latetime;
	dlg.m_ps = ps;
	dlg.DoModal();
}




HBRUSH CCtmInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CCtmInfo::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSock::m_pCtmInfo = NULL;
	CDialogEx::OnClose();
}


void CCtmInfo::OnTimer(UINT_PTR nIDEvent)
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
	CDialogEx::OnTimer(nIDEvent);
}


afx_msg LRESULT CCtmInfo::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CCtmInfo::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T("就绪"));
	return 0;
}

void CCtmInfo::ShowTip()
{
	m_time_take++;
	CString time =NULL;
	time.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowTextW(time);
	if(m_time_take == 120)
	{
		OnEndTask(0,0);
		m_tip.SetWindowTextW(_T("请求超时，请重试或者重新连接服务器"));
	}
}

void CCtmInfo::OnBnClickedBtnSearchall()
{
	// TODO: 在此添加控件通知处理程序代码
	CSock::StartFindAllCtm(this);
}


void CCtmInfo::OnBnClickedBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int count = 0;
	CString num;
	CString  lineToDelete = _T("将要被删除的客户的编号是：");
	m_strToDelCtm.Empty();
	int total = m_list.GetItemCount();
	for(int i=0; i< total; i++)
	{
		if(m_list.GetCheck(i))
		{	
			count++;
			m_strToDelCtm += m_list.GetItemText(i,0);
			m_strToDelCtm += ",";   //两个时间段的分隔符
		}
	}
	if(count == 0)
	{
		MessageBox(_T("请先至少勾选一个员工，然后再删除"));
		return;
	}else{   //有选中项
		CString total;
		total.Format(_T("共有%d个选中项，"),count);
		lineToDelete +=  m_strToDelCtm + _T("确认删除吗？");
		if(IDYES == MessageBox(total+lineToDelete,_T("警告"),MB_YESNO))
		{
			CSock::StartDelCtm(this);
		}else{
			return;
		}
	}
}


afx_msg LRESULT CCtmInfo::OnDelCtm(WPARAM wParam, LPARAM lParam)
{
	CString _strToDelete = m_strToDelCtm;
	CString _strToFind = _T(",");    //找到这样的标志位 进行字符段的截取
	while(TRUE){
		int _strEnd = _strToDelete.Find(_strToFind);
		if(-1 == _strEnd)   //没有找到
		{
			break;
		}else{
			CString temp = _strToDelete.Mid(0,_strEnd);
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
