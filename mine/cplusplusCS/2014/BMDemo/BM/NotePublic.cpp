// NotePublic.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "NotePublic.h"
#include "afxdialogex.h"
#include "MainFormView.h"

// CNotePublic 对话框

IMPLEMENT_DYNAMIC(CNotePublic, CDialogEx)

CNotePublic::CNotePublic(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNotePublic::IDD, pParent)
	, m_title(_T(""))
	, m_personPublicName(_T(""))
	, m_content(_T(""))
{

}
CNotePublic::CNotePublic(CMainFormView* pView,CWnd* pParent /*=NULL*/)
	: CDialogEx(CNotePublic::IDD, pParent)
{
	m_pMainView = pView;
	timeTake = 0;
}
CNotePublic::~CNotePublic()
{
	m_pMainView = NULL;
}

void CNotePublic::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_title);
	DDX_Text(pDX, IDC_EDIT_PERSON_PUBLIC, m_personPublicName);
	DDX_Text(pDX, IDC_EDIT_CONTENT, m_content);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_status);
	DDX_Control(pDX, IDC_STATIC_TIME_TAKE, m_time_take);
}


BEGIN_MESSAGE_MAP(CNotePublic, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_PUBLIC, &CNotePublic::OnBnClickedBtnPublic)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CNotePublic::OnBnClickedBtnCancel)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_REQNOTEPUBLIC_NotePublic, &CNotePublic::OnReqNotePublic)
	ON_MESSAGE(WM_ENDNOTEPUBLIC_NotePublic, &CNotePublic::OnEndNotePublic)
	ON_WM_TIMER()
	ON_MESSAGE(WM_NOTEPUBLICSUCCESS_NotePublic, &CNotePublic::OnNotePublicSuccessMainView)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CNotePublic 消息处理程序


void CNotePublic::OnBnClickedBtnPublic()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if( "" == m_title)
	{
		AfxMessageBox(_T("公告题目不能为空！"));
		return;
	}
	if(m_title.GetLength()>40)
	{
		AfxMessageBox(_T("公告题目不能大于40个字符！"));
		return;
	}
	if( "" == m_content)
	{
		AfxMessageBox(_T("公告内容不能为空！"));
		return;
	}
	if( "" == m_personPublicName)
	{
		AfxMessageBox(_T("公告发布者不能为空！"));
		return;
	}
	if(m_content.GetLength()>300)
	{
		AfxMessageBox(_T("公告内容过长，不能超过300个字符！"));
		return;
	}
	if(m_personPublicName.GetLength()>15)
	{
		AfxMessageBox(_T("发布公告者的用户名不能长于15个字符！"));
		return;
	}
	GetCurTime(m_cur_time);
	m_pMainView->m_content_note = m_content;
	m_pMainView->m_title_note = m_title;
	m_pMainView->m_persion_public_note = m_personPublicName;
	m_pMainView->m_time_note_public = m_cur_time;
	
	CSock::StartReqNotePublic(this);     //开始添加发布公告到服务器
}


void CNotePublic::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


HBRUSH CNotePublic::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CNotePublic::EnableSysShutDownBtn(BOOL bEnable)   //真就是可以关闭窗口 假代表不可以
{
	CMenu *pSysMenu = (CMenu *)GetSystemMenu(FALSE);
    ASSERT(pSysMenu != NULL);
	if(bEnable){
		pSysMenu->EnableMenuItem(SC_CLOSE,MF_ENABLED);
	}else{
		pSysMenu->EnableMenuItem(SC_CLOSE,MF_DISABLED);
	}
}

void  CNotePublic::GetCurTime(CString &time)
{
	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);
	//strDate.Format(_T("%d"),5);
	strDate.Format(_T("%4d-%02d-%02d"),(int)st.wYear,(int)st.wMonth,(int)st.wDay);
	strTime.Format(_T("%02d:%02d:%02d"),st.wHour,st.wMinute,st.wSecond);
	time = strDate + _T(" ") + strTime;
}

afx_msg LRESULT CNotePublic::OnReqNotePublic(WPARAM wParam, LPARAM lParam)
{
	EnableSysShutDownBtn(FALSE);
	SetTimer(1,20*1000,NULL);  //设置20秒超时时间
	SetTimer(2,1000,NULL);
	m_time_take.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CNotePublic::OnEndNotePublic(WPARAM wParam, LPARAM lParam)
{
	EnableSysShutDownBtn(TRUE);
	KillTimer(1);
	KillTimer(2);
	m_time_take.SetWindowText(_T(""));
	//CSock::m_bReqAvailable = TRUE;  //使可以重新发起请求
	timeTake = 0;
	return 0;
}



void CNotePublic::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{
			m_status.SetWindowText(_T("请求超时，请检查与服务器连接状态"));
			OnEndNotePublic(0,0);
			CSock::m_pNotePublic = NULL;  //清除指针	 
			break;
		}
	case 2:
		{
			ShowTimeTakeStatus();
			break;
		}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void  CNotePublic::ShowTimeTakeStatus()
{
	timeTake++;
	CString time;
	time.Format(_T("用时：%d秒"),timeTake);
	m_time_take.SetWindowText(time);
}

afx_msg LRESULT CNotePublic::OnNotePublicSuccessMainView(WPARAM wParam, LPARAM lParam)
{
	//OnEndNotePublic(0,0);
	NotePublicSuccess();
	return 0;
}

void CNotePublic::NotePublicSuccess()
{
	m_pMainView->SendMessage(WM_SHOWNOTEINEDIT_MainView,0,0);
	int count = m_pMainView->m_list.GetItemCount();
	
	for(int i=0;i< count;i++)  //修改显示最近公告
	{
		CString index = NULL;
		index.Format(_T("%d"),(i+2));  //修改索引
		m_pMainView->m_list.SetItemText(i,0,index);
	}
	m_pMainView->m_list.InsertItem(0,_T("1"));
	m_pMainView->m_list.SetItemText(0,1,m_title);
	m_pMainView->m_list.SetItemText(0,3,m_personPublicName);
	m_pMainView->m_list.SetItemText(0,2,m_cur_time);
	CString tip;
	tip.Format(_T("最新的%d条通知"),count);
	m_pMainView->GetDlgItem(IDC_STATIC_TIP)->SetWindowText(tip);
	CSock::m_pNotePublic = NULL;
	CDialogEx::OnOK();
}


BOOL CNotePublic::OnInitDialog()
{
	//设置两个状态栏的字体
	CDialogEx::OnInitDialog();
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_status.SetFont(&font);
	m_time_take.SetFont(&font);

	font.Detach();

	return TRUE;
}

void CNotePublic::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSock::m_pNotePublic = NULL;
	CDialogEx::OnClose();
}
