// Download.cpp : 实现文件
//

#include "stdafx.h"
#include "SchoolJoy.h"
#include "Download.h"
#include "afxdialogex.h"
#include "SchoolJoyDlg.h"


// CDownload 对话框

IMPLEMENT_DYNAMIC(CDownload, CDialogEx)

CDownload::CDownload(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDownload::IDD, pParent)
{

}

CDownload::~CDownload()
{
}

void CDownload::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST_Download, m_List_Download);
	DDX_Control(pDX, IDC_TAB_Download, m_tab_download);
}


BEGIN_MESSAGE_MAP(CDownload, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_Download, &CDownload::OnSelchangeTabDownload)
	ON_BN_CLICKED(IDC_BUTTON_StartDownload, &CDownload::OnBnClickedButtonStartdownload)
END_MESSAGE_MAP()


// CDownload 消息处理程序


BOOL CDownload::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	InitDownload();
	return 0;
}


void CDownload::InitDownload(void)
{
	CRect tabRect;   // 标签控件客户区的位置和大小    
    m_tab_download.InsertItem(0, _T("正在下载"));         // 插入第一个标签“”  
	m_download_downloading.Create(IDD_DIALOG_Download_Downloading, &m_tab_download);    // 创建第一个标签页
    m_tab_download.InsertItem(1, _T("已完成下载"));  // 插入第二个标签“”
	m_download_downloaded.Create(IDD_DIALOG_Download_Downloaded, &m_tab_download); // 创建第二个标签页 
	m_tab_download.InsertItem(2, _T("所有下载"));   
	m_download_alldownloaded.Create(IDD_DIALOG_Download_AllDownloaded, &m_tab_download); 
 
    m_tab_download.GetClientRect(&tabRect);    // 获取标签控件客户区Rect    
    // 调整tabRect，使其覆盖范围适合放置标签页    
    tabRect.left += 1;                   
    tabRect.right -= 1;    
    tabRect.top += 25;    
    tabRect.bottom -= 1;    
    // 根据调整好的tabRect放置m_jzmDlg子对话框，并设置为显示    
    m_download_downloading.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);    
    // 根据调整好的tabRect放置m_androidDlg子对话框，并设置为隐藏    
    m_download_downloaded.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW); 
	m_download_alldownloaded.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW); 

	//Add by wqdnan 用于初始化下载Tab控件处的显示，并将Tab界面与Downloading，Downloaded，AllDownload三个对话框进行关联

}


void CDownload::OnSelchangeTabDownload(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CRect tabRect;    // 标签控件客户区的Rect    
 
    // 获取标签控件客户区Rect，并对其调整，以适合放置标签页    
    m_tab_download.GetClientRect(&tabRect);    
    tabRect.left += 1;    
    tabRect.right -= 1;    
    tabRect.top += 25;    
    tabRect.bottom -= 1;    
 
    switch (m_tab_download.GetCurSel())    
    {    
    // 如果标签控件当前选择标签为“正在下载”，则显示Downloading对话框，隐藏Downloaded和AllDownload对话框    
    case 0:    
        m_download_downloading.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		//m_download_downloading.InsertFile()
        m_download_downloaded.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_download_alldownloaded.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
        break;    
    // 如果标签控件当前选择标签为“已完成下载”，则显示Downloaded对话框，隐藏Downloading和AllDownload对话框     
    case 1:    
        m_download_downloading.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);    
        m_download_downloaded.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW); 
		m_download_alldownloaded.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
        break;   
	// 如果标签控件当前选择标签为“所有下载”，则显示AllDownload对话框，隐藏Downloaded和Downloading对话框 
	case 2:
		m_download_downloading.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);    
        m_download_downloaded.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW); 
		m_download_alldownloaded.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
    default:    
        break;    
    }    

}


void CDownload::OnBnClickedButtonStartdownload()
{
	this->m_download_downloading.StratToDownLoad();
}
