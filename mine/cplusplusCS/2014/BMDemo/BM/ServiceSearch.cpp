// ServiceSearch.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "ServiceSearch.h"
#include "ServSearch.h"
#include "ServModify.h"
// CServiceSearch

IMPLEMENT_DYNCREATE(CServiceSearch, CFormView)

CServiceSearch::CServiceSearch()
	: CFormView(CServiceSearch::IDD)
	//, m_keyword(_T(""))
{

}

CServiceSearch::~CServiceSearch()
{
}

void CServiceSearch::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TIMECHOOSE, m_dateType);
	DDX_Control(pDX, IDC_DATETIME_DEADLINE, m_dateEnd);
	DDX_Control(pDX, IDC_DATETIME_START, m_dateStart);
	//DDX_Text(pDX, IDC_EDIT_KEYWORD, m_keyword);
	DDX_Control(pDX, IDC_LIST_SERVICE, m_list);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
}

BEGIN_MESSAGE_MAP(CServiceSearch, CFormView)
	//ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_EDIT, &CServiceSearch::OnBnClickedBtnEdit)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CServiceSearch::OnBnClickedBtnSearch)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTTASK_ServiceSearch, &CServiceSearch::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_ServiceSearch, &CServiceSearch::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CServiceSearch::OnBnClickedBtnDelete)
	ON_MESSAGE(WM_DELSERV_ServiceSearch, &CServiceSearch::OnDelServ)
	ON_BN_CLICKED(IDC_BTN_SEARCH_COMM, &CServiceSearch::OnBnClickedBtnSearchComm)
END_MESSAGE_MAP()



// CServiceSearch 诊断

#ifdef _DEBUG
void CServiceSearch::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CServiceSearch::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG




// CServiceSearch 消息处理程序
void CServiceSearch::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	InitList();
	InitCombDate();
	InitTipCtrl();
}

void CServiceSearch::InitTipCtrl()
{
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();
}

void CServiceSearch::InitCombDate()
{
	m_dateType.AddString(_T("要求服务日期"));
	m_dateType.AddString(_T("延期日期"));
	m_dateType.SetCurSel(0);

	m_dateEnd.SetFormat(_T("yyyy-MM-dd"));
	m_dateStart.SetFormat(_T("yyyy-MM-dd"));
}

void CServiceSearch::InitList()
{
	m_list.SetBkColor(CONCTRLBKCOLOR);
	m_list.SetExtendedStyle( m_list.GetExtendedStyle()| LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.SetTextColor(RGB(139, 90, 43));
	m_list.SetTextBkColor(RGB(255, 236, 139));
	
	CRect rect;
	m_list.GetClientRect(rect);
	m_list.InsertColumn(0, _T("服务单号"), LVCFMT_LEFT, rect.Width()/16);    
    m_list.InsertColumn(1, _T("进度"), LVCFMT_LEFT, rect.Width()/16);
	m_list.InsertColumn(2, _T("服务类型"), LVCFMT_LEFT, rect.Width()/16);
    m_list.InsertColumn(3, _T("要求服务日期"), LVCFMT_LEFT, rect.Width()/16);
	m_list.InsertColumn(4, _T("延期日期"), LVCFMT_LEFT, rect.Width()/16);
	m_list.InsertColumn(5, _T("客户类型"), LVCFMT_LEFT, rect.Width()/16);
	m_list.InsertColumn(6, _T("客户名称"), LVCFMT_LEFT, rect.Width()/16);
	m_list.InsertColumn(7, _T("顾客姓名"), LVCFMT_LEFT, rect.Width()/16);
	m_list.InsertColumn(8, _T("性别"), LVCFMT_LEFT, rect.Width()/16);
	m_list.InsertColumn(9, _T("顾客电话"), LVCFMT_LEFT, rect.Width()/16);
	m_list.InsertColumn(10, _T("地址"), LVCFMT_LEFT, rect.Width()/16);
	m_list.InsertColumn(11, _T("收款金额"), LVCFMT_LEFT, rect.Width()/16);
	m_list.InsertColumn(12, _T("服务人员姓名"), LVCFMT_LEFT, rect.Width()/16);
	m_list.InsertColumn(13, _T("服务人员电话"), LVCFMT_LEFT, rect.Width()/16);
	m_list.InsertColumn(14, _T("服务内容"), LVCFMT_LEFT, rect.Width()/16);
	m_list.InsertColumn(15, _T("备注"), LVCFMT_LEFT, rect.Width()/16);	   
}

void CServiceSearch::OnBnClickedBtnEdit()
{
	CServModify dlg;
	USES_CONVERSION;
	int count = 0;
	CString serv_no = NULL;
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		//if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ||m_list.GetCheck(i))
		if(m_list.GetCheck(i))
		{	
			count++ ;
			if(count > 1)  //表示有多个选中项
			{
				CString str;
				str.Format(_T("当前有多个选中项，默认将编辑第一个选中项，服务单号为：'%s'。确认继续吗"),serv_no);
				if(IDYES == MessageBox(str,_T("提示"),MB_YESNO)){
					break;
				}else
				{
					return;
				}
			}
			serv_no = m_list.GetItemText(i,0);
			dlg.m_servNO = serv_no;
			dlg.m_strProg = m_list.GetItemText(i,1);
			dlg.m_servType = m_list.GetItemText(i,2);
			dlg.m_strDateReq = m_list.GetItemText(i,3);
			dlg.m_strDateDelay = m_list.GetItemText(i,4);
			dlg.m_strCtmType = m_list.GetItemText(i,5);
			dlg.m_ctmNM = m_list.GetItemText(i,6);
			dlg.m_clientNM = m_list.GetItemText(i,7);
			dlg.m_strCtmSex = m_list.GetItemText(i,8);
			dlg.m_phone = m_list.GetItemText(i,9);
			dlg.m_addr = m_list.GetItemText(i,10);
			dlg.m_charge = atof(T2A(m_list.GetItemText(i,11)));
			dlg.m_servNM = m_list.GetItemText(i,12);
			dlg.m_servPhone = m_list.GetItemText(i,13);
			dlg.m_servContent = m_list.GetItemText(i,14);
			dlg.m_ps = m_list.GetItemText(i,15);
		}
	}
	if(serv_no.IsEmpty())
	{
		MessageBox(_T("当前没有选中项，请至少选中一项服务进行编辑"));
		return;
	}
	dlg.DoModal();
}

void CServiceSearch::FindAllServ()
{
	CSock::StartFindAllServ(this);
}

void CServiceSearch::OnBnClickedBtnSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CServSearch servSearchDlg(this);
	servSearchDlg.DoModal();
}


HBRUSH CServiceSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
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


void CServiceSearch::OnTimer(UINT_PTR nIDEvent)
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

void CServiceSearch::ShowTip()
{
	m_time_take++;
	CString time = NULL;
	time.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowText(time);
	if(m_time_take == 120)
	{
		OnEndTask(0,0);
		m_tip.SetWindowText(_T("请求超时，请重试或者重新连接服务器"));
	}
}


void CServiceSearch::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFormView::OnClose();
}


afx_msg LRESULT CServiceSearch::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CServiceSearch::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


void CServiceSearch::OnBnClickedBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int count = 0;
	CString num;
	CString  lineToDelete = _T("将要被删除服务的单号是：");
	m_strDelServ.Empty();
	int total = m_list.GetItemCount();
	for(int i=0; i< total; i++)
	{
		//if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ||m_list.GetCheck(i))
		if(m_list.GetCheck(i))
		{	
			count++;
			m_strDelServ += m_list.GetItemText(i,0);
			m_strDelServ += ",";   //两个时间段的分隔符
		}
	}
	if(count == 0)
	{
		MessageBox(_T("请先至少勾选一个服务，然后再删除"));
		return;
	}else{   //有选中项
		CString total;
		total.Format(_T("共有%d个选中项，"),count);
		lineToDelete +=  m_strDelServ + _T("确认删除吗？");
		if(IDYES == MessageBox(total+lineToDelete,_T("警告"),MB_YESNO))
		{
			CSock::StartDelServ(this);
		}else{
			return;
		}
	}
}


afx_msg LRESULT CServiceSearch::OnDelServ(WPARAM wParam, LPARAM lParam)
{
	CString _strToDelete = m_strDelServ;
	m_strDelServ.Empty();
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


void CServiceSearch::OnBnClickedBtnSearchComm()     //普通搜索
{
	// TODO: 在此添加控件通知处理程序代码
	CString str = NULL;
	m_dateType.GetWindowText(str);
	m_searchByKeyWord.Empty();
	m_searchByKeyWord = _T("普通搜索,");
	if(_T("要求服务日期") == str)
	{
		m_searchByKeyWord += _T("要求服务日期,");
	}else{
		m_searchByKeyWord += _T("延期日期,");
	}
	CTime timeTime;
	USES_CONVERSION;
	DWORD dwResult = m_dateStart.GetTime(timeTime);
	if (dwResult == GDT_VALID)
	{
		// the user checked the box and specified data
		
		// is it a time-only control, or a date-only control?
		if ((m_dateStart.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		m_searchByKeyWord += T2A(str) ;
		m_searchByKeyWord += _T(",");
	}else
	{
		MessageBox(_T("请选择开始日期"));
		return;
	}
	dwResult = m_dateEnd.GetTime(timeTime);
	if (dwResult == GDT_VALID)
	{
		if ((m_dateEnd.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		m_searchByKeyWord += T2A(str) ;
	}else
	{
		MessageBox(_T("请选择截止日期"));
		return;
	}
	CSock::StartFindServByKeyWord(this);
}
