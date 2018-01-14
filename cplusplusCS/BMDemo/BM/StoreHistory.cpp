// StoreHistory.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "StoreHistory.h"
#include "afxdialogex.h"


// CStoreHistory 对话框

IMPLEMENT_DYNAMIC(CStoreHistory, CDialogEx)

CStoreHistory::CStoreHistory(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStoreHistory::IDD, pParent)
{

}

CStoreHistory::~CStoreHistory()
{
}

void CStoreHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STORE_HISTORY, m_list);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
}


BEGIN_MESSAGE_MAP(CStoreHistory, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_FIND_ALL, &CStoreHistory::OnBnClickedBtnFindAll)
	
	ON_BN_CLICKED(IDC_BTN_DEL_HISTORY, &CStoreHistory::OnBnClickedBtnDelHistory)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTTASK_StoreHistory, &CStoreHistory::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_StoreHistory, &CStoreHistory::OnEndTask)
	ON_MESSAGE(WM_DELHISTORY_StoreHistory, &CStoreHistory::OnDelHistory)
END_MESSAGE_MAP()


// CStoreHistory 消息处理程序

void CStoreHistory::InitList()
{
	m_list.SetBkColor(CONCTRLBKCOLOR);
	m_list.SetExtendedStyle( m_list.GetExtendedStyle()| LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.SetTextColor(RGB(139, 90, 43));
	m_list.SetTextBkColor(RGB(255, 236, 139));
	
	CRect rect;
	m_list.GetClientRect(rect);
	m_list.InsertColumn(0, _T("名称"), LVCFMT_LEFT, rect.Width()/4);    
    m_list.InsertColumn(1, _T("变动数量"), LVCFMT_LEFT, rect.Width()/4);
	m_list.InsertColumn(2, _T("状态"), LVCFMT_LEFT, rect.Width()/4);
    m_list.InsertColumn(3, _T("时间"), LVCFMT_LEFT, rect.Width()/4);
	
}

BOOL CStoreHistory::OnInitDialog()
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

HBRUSH CStoreHistory::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CStoreHistory::OnBnClickedBtnFindAll()
{
	// TODO: 在此添加控件通知处理程序代码
	CSock::StartFindAllStoreHistory(this);
}

void CStoreHistory::OnBnClickedBtnDelHistory()
{
	// TODO: 在此添加控件通知处理程序代码
	int count = 0;
	CString num;
	CString  lineToDelete = _T("将要被删除记录的存档时间是：");
	m_strToDelHistory.Empty();
	int total = m_list.GetItemCount();
	for(int i=0; i< total; i++)
	{
		//if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ||m_list.GetCheck(i))
		if(m_list.GetCheck(i))
		{	
			count++;
			m_strToDelHistory += m_list.GetItemText(i,3);
			m_strToDelHistory += ",";   //两个时间段的分隔符
		}
	}
	if(count == 0)
	{
		MessageBox(_T("请先至少勾选一个记录，然后再删除"));
		return;
	}else{   //有选中项
		CString total;
		total.Format(_T("共有%d个选中项，"),count);
		lineToDelete +=  m_strToDelHistory + _T("确认删除吗？");
		if(IDYES == MessageBox(total+lineToDelete,_T("警告"),MB_YESNO))
		{
			CSock::StartDelHistory(this);
		}else{
			return;
		}
	}
}


void CStoreHistory::OnTimer(UINT_PTR nIDEvent)
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

void CStoreHistory::ShowTip()
{
	m_time_take++;
	CString time =NULL;
	time.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowText(time);
	if(m_time_take == 120)
	{
		OnEndTask(0,0);
		m_tip.SetWindowText(_T("请求超时，请重试或者重新连接服务器"));
	}
}

void CStoreHistory::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSock::m_pStoreHistory = NULL;
	CDialogEx::OnClose();
}


afx_msg LRESULT CStoreHistory::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CStoreHistory::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T("就绪"));
	return 0;
}


afx_msg LRESULT CStoreHistory::OnDelHistory(WPARAM wParam, LPARAM lParam)
{
	CString _strToDelete = m_strToDelHistory;
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
				if(temp == m_list.GetItemText(i,3))
				{
					m_list.DeleteItem(i);
				}
			}
			_strToDelete = _strToDelete.Mid(_strEnd+1);
		}
	}
	return 0;
}
