// Download_Downloading.cpp : 实现文件
//

#include "stdafx.h"
#include "SchoolJoy.h"
#include "Download_Downloading.h"
#include "afxdialogex.h"
#include "ThreadOperate.h"


// CDownload_Downloading 对话框

IMPLEMENT_DYNAMIC(CDownload_Downloading, CDialogEx)

CDownload_Downloading::CDownload_Downloading(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDownload_Downloading::IDD, pParent)
{
	m_filename=_T("");
	m_filepath=_T("");
}

CDownload_Downloading::~CDownload_Downloading()
{
}

void CDownload_Downloading::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Download_Downloading, m_Download_Downloading);
}


BEGIN_MESSAGE_MAP(CDownload_Downloading, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON1, &CDownload_Downloading::OnBnClickedButton1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_Download_Downloading, &CDownload_Downloading::OnDblclkListDownloadDownloading)
	ON_WM_TIMER()
//	ON_MESSAGE(WM_OKSEARCH, &CDownload_Downloading::OnOkSearch)
END_MESSAGE_MAP()


// CDownload_Downloading 消息处理程序

//Add by wqdnan 用于创建列表的格式及写入初始值
void CDownload_Downloading::InitDownload_Downloading(void)
{
	CRect rect;    
 
    // 获取编程语言列表视图控件的位置和大小    
    m_Download_Downloading.GetClientRect(&rect);    
 
    // 为列表视图控件添加全行选中和栅格风格    
    m_Download_Downloading.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);   

	m_Download_Downloading.SetTextColor(RGB(139, 90, 43));

	m_Download_Downloading.SetTextBkColor(RGB(255, 236, 139));
 

	m_Download_Downloading.InsertColumn(0, _T("文件名"), LVCFMT_LEFT, 100);    
    m_Download_Downloading.InsertColumn(1, _T("文件类型"), LVCFMT_LEFT, 100);
	m_Download_Downloading.InsertColumn(2, _T("文件大小"), LVCFMT_LEFT, 100);
    m_Download_Downloading.InsertColumn(3, _T("原文件路径"), LVCFMT_LEFT, 150);
	m_Download_Downloading.InsertColumn(4, _T("下载进度"), LVCFMT_LEFT, 100);
	m_Download_Downloading.InsertColumn(5, _T("下载速度"), LVCFMT_LEFT, 100);    
    m_Download_Downloading.InsertColumn(6, _T("下载状态"), LVCFMT_LEFT, 100);


}

//Add by wqdnan 用于调用初始化列表内容值
BOOL CDownload_Downloading::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	InitDownload_Downloading();
	return 0;
}

void CDownload_Downloading::NewDownLoad(CString &filename,CString &filetype,CString &filesize,CString &filepath,int &j)
{
	CDownload_Downloading *dlg=(CDownload_Downloading *)this;
	dlg->m_Download_Downloading.InsertItem(j,NULL);
	dlg->m_Download_Downloading.SetItemText(j,0,filename);
	dlg->m_Download_Downloading.SetItemText(j,1,filetype);
	dlg->m_Download_Downloading.SetItemText(j,2,filesize);
	dlg->m_Download_Downloading.SetItemText(j,3,filepath);
	dlg->m_Download_Downloading.SetItemText(j,4,_T(""));
	dlg->m_Download_Downloading.SetItemText(j,5,_T(""));
	dlg->m_Download_Downloading.SetItemText(j,6,_T("等待下载"));
}




void CDownload_Downloading::OnDblclkListDownloadDownloading(NMHDR *pNMHDR, LRESULT *pResult)
{
	 //双击listctrl中的选中行开始下载
	StratToDownLoad();
}
void CDownload_Downloading::StratToDownLoad(void)    //单击开始下载按钮的响应函数，两种下载方式
{
	for(int i=0; i<m_Download_Downloading.GetItemCount(); i++)
	{
		if( m_Download_Downloading.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{	
			m_filename=m_Download_Downloading.GetItemText(i,0);
			m_filepath=m_Download_Downloading.GetItemText(i,3);
			this->m_Download_Downloading.SetItemText(i,6,_T("正在下载中"));
			this->m_NUMDownloading=i;//记录正在下载的第几行
			ThreadOperate downloadThread;//转到下载线程  下载后作提示用
			void* dialog=(void*)this;////////////////
			downloadThread.startRequireInfoThread(dialog);//////
			Sleep(1000);//程序挂起一秒，当选中多个下载项的时候，防止程序执行太快而造成值传递丢失
		}
	}
}

void CDownload_Downloading::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)
	{
	case TIMER_SPEED:
		_pthreadoperate->UpdateSpeed(this);
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

