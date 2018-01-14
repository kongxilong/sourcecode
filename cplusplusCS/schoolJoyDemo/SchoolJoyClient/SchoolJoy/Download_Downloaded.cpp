// Download_Downloaded.cpp : 实现文件
//

#include "stdafx.h"
#include "SchoolJoy.h"
#include "Download_Downloaded.h"
#include "afxdialogex.h"


// CDownload_Downloaded 对话框

IMPLEMENT_DYNAMIC(CDownload_Downloaded, CDialogEx)

CDownload_Downloaded::CDownload_Downloaded(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDownload_Downloaded::IDD, pParent)
{

}

CDownload_Downloaded::~CDownload_Downloaded()
{
}

void CDownload_Downloaded::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Download_Downloaded, m_Download_Downloaded);
}


BEGIN_MESSAGE_MAP(CDownload_Downloaded, CDialogEx)
END_MESSAGE_MAP()


// CDownload_Downloaded 消息处理程序

//Add by wqdnan 用于创建列表的格式及写入初始值
void CDownload_Downloaded::InitDownload_Downloaded(void)
{
	CRect rect;    
 
    // 获取编程语言列表视图控件的位置和大小    
    m_Download_Downloaded.GetClientRect(&rect);    
 
    // 为列表视图控件添加全行选中和栅格风格    
    m_Download_Downloaded.SetExtendedStyle(m_Download_Downloaded.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);    
 
    // 为列表视图控件添加三列    
    m_Download_Downloaded.InsertColumn(0, _T("下载文件"), LVCFMT_CENTER, rect.Width()/4, 0);    
    m_Download_Downloaded.InsertColumn(1, _T("文件大小"), LVCFMT_CENTER, rect.Width()/4, 1);    
    m_Download_Downloaded.InsertColumn(2, _T("文件类型"), LVCFMT_CENTER, rect.Width()/4, 2);    
	m_Download_Downloaded.InsertColumn(1, _T("完成时间"), LVCFMT_CENTER, rect.Width()/4, 3);    
 

    // 在列表视图控件中插入列表项，并设置列表子项文本    
    m_Download_Downloaded.InsertItem(0, _T("文件一"));    
    m_Download_Downloaded.SetItemText(0, 1, _T("0"));    
    m_Download_Downloaded.SetItemText(0, 2, _T("0"));    
    m_Download_Downloaded.SetItemText(0, 3, _T("0"));   
}


BOOL CDownload_Downloaded::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	InitDownload_Downloaded();
	return 0;
}
