// Download_AllDownloaded.cpp : 实现文件
//

#include "stdafx.h"
#include "SchoolJoy.h"
#include "Download_AllDownloaded.h"
#include "afxdialogex.h"


// CDownload_AllDownloaded 对话框

IMPLEMENT_DYNAMIC(CDownload_AllDownloaded, CDialogEx)

CDownload_AllDownloaded::CDownload_AllDownloaded(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDownload_AllDownloaded::IDD, pParent)
{

}

CDownload_AllDownloaded::~CDownload_AllDownloaded()
{
}

void CDownload_AllDownloaded::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Download_AllDownloaded, m_List_Download_AllDownloaded);
}


BEGIN_MESSAGE_MAP(CDownload_AllDownloaded, CDialogEx)
END_MESSAGE_MAP()


// CDownload_AllDownloaded 消息处理程序

//Add by wqdnan 用于创建列表的格式及写入初始值
void CDownload_AllDownloaded::InitDownload_AllDownloaded(void)
{
	CRect rect;    
 
    // 获取编程语言列表视图控件的位置和大小    
    m_List_Download_AllDownloaded.GetClientRect(&rect);    
 
    // 为列表视图控件添加全行选中和栅格风格    
    m_List_Download_AllDownloaded.SetExtendedStyle(m_List_Download_AllDownloaded.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	m_List_Download_AllDownloaded.SetTextColor(RGB(126, 192, 238));

	m_List_Download_AllDownloaded.SetTextBkColor(RGB(47, 79, 79));
 
    // 为列表视图控件添加四列    
    m_List_Download_AllDownloaded.InsertColumn(0, _T("下载文件"), LVCFMT_CENTER, rect.Width()/4, 0);    
    m_List_Download_AllDownloaded.InsertColumn(1, _T("文件大小"), LVCFMT_CENTER, rect.Width()/4, 1);    
    m_List_Download_AllDownloaded.InsertColumn(2, _T("文件类型"), LVCFMT_CENTER, rect.Width()/4, 2);    
	m_List_Download_AllDownloaded.InsertColumn(1, _T("完成时间"), LVCFMT_CENTER, rect.Width()/4, 3);    
 

    // 在列表视图控件中插入列表项，并设置列表子项文本    
    m_List_Download_AllDownloaded.InsertItem(0, _T("文件一"));    
    m_List_Download_AllDownloaded.SetItemText(0, 1, _T("0"));    
    m_List_Download_AllDownloaded.SetItemText(0, 2, _T("0"));    
    m_List_Download_AllDownloaded.SetItemText(0, 3, _T("0"));  
	m_List_Download_AllDownloaded.InsertItem(1, _T("文件二"));    
    m_List_Download_AllDownloaded.SetItemText(1, 1, _T("0"));    
    m_List_Download_AllDownloaded.SetItemText(1, 2, _T("0"));    
    m_List_Download_AllDownloaded.SetItemText(1, 3, _T("0"));  
}

//Add by wqdnan 用于调用初始化列表内容值
BOOL CDownload_AllDownloaded::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	InitDownload_AllDownloaded();
	return 0;
}
