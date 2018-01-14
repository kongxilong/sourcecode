// Search.cpp : 实现文件
//

#include "stdafx.h"
#include "SchoolJoy.h"
#include "Search.h"
#include "afxdialogex.h"
#include "ThreadOperate.h"
#include "schooljoydlg.h"
#include "Download_Downloading.h"
#include <Windows.h>
//#include "ThreadOperate.cpp"



IMPLEMENT_DYNAMIC(CSearch, CDialogEx)

CSearch::CSearch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearch::IDD, pParent)
	, m_SearchFile(_T(""))
{
	//m_SelectedFileName=_T("");
	//m_SelectedFilePath=_T("");
	//m_SelectedFileSize=_T("");
}

CSearch::~CSearch()
{
}

void CSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Search, m_List_Search);
	DDX_Text(pDX, IDC_EDIT_Search, m_SearchFile);
}


BEGIN_MESSAGE_MAP(CSearch, CDialogEx)
//	ON_NOTIFY(NM_CLICK, IDC_LIST_Search, &CSearch::OnClickListSearch)
	ON_BN_CLICKED(IDC_BUTTON_SearchToFind, &CSearch::OnBnClickedButtonSearchtofind)
	ON_BN_CLICKED(IDC_BUTTON_AddToDownload, &CSearch::OnBnClickedButtonAddtodownload)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_Search, &CSearch::OnDblclkListSearch)
	ON_MESSAGE(WM_ONOKSearch, &CSearch::OnOnOKSearch)
END_MESSAGE_MAP()


// CSearch 消息处理程序

//Add by wqdnan 用于创建列表的格式及写入初始值
void CSearch::InitSearch(void)
{
	CRect rect;    
 
    // 获取编程语言列表视图控件的位置和大小    
    m_List_Search.GetClientRect(&rect);   

	m_List_Search.SetTextColor(RGB(139, 90, 43));

	m_List_Search.SetTextBkColor(RGB(255, 236, 139));
 
    // 为列表视图控件添加全行选中和栅格风格    
    m_List_Search.SetExtendedStyle(m_List_Search.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);    
 
    // 为列表视图控件添加四列    
    m_List_Search.InsertColumn(0, _T("文件名"), LVCFMT_CENTER, rect.Width()/4, 0);    
    m_List_Search.InsertColumn(1, _T("类型"), LVCFMT_CENTER, rect.Width()/4, 1);    
    m_List_Search.InsertColumn(2, _T("大小"), LVCFMT_CENTER, rect.Width()/4, 2);    
	m_List_Search.InsertColumn(3, _T("路径"), LVCFMT_CENTER, rect.Width()/4, 2);  
    // 在列表视图控件中插入列表项，并设置列表子项文本   

}

//Add by wqdnan 用于调用初始化列表内容值
BOOL CSearch::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	InitSearch();
	return 0;
}




void CSearch::OnBnClickedButtonSearchtofind()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_SearchFile=="")
	{
		AfxMessageBox(_T("请输入搜索关键字"));
		return ;
	}
	else
	{
		m_List_Search.DeleteAllItems();
		ThreadOperate SearchThread;//开启搜索线程
		//AfxMessageBox(m_SearchFile);
		void* dialog=(void*)this;
		SearchThread.startSearchThread(dialog);
	}
	
	return ;
}


void CSearch::OnBnClickedButtonAddtodownload()
{
	
	OnDblclkListSearch(NULL,NULL);
}
	 
	



void CSearch::OnDblclkListSearch(NMHDR *pNMHDR, LRESULT *pResult)
{
	CSchoolJoyDlg *dialog=(CSchoolJoyDlg *)GetParent()->GetParent();
	CRect tabRect;    // 标签控件客户区的Rect    
	// 获取标签控件客户区Rect，并对其调整，以适合放置标签页    
	dialog->m_tab_main.GetClientRect(&tabRect);    
	tabRect.left += 1;    
	tabRect.right -= 1;    
	tabRect.top += 25;    
	tabRect.bottom -= 1;

	// 如果标签控件当前选择标签为“下载”，隐藏“搜索”“上传”    
	dialog->m_download.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	dialog->m_search.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);    
	dialog->m_upload.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	int Label = dialog->m_tab_main.SetCurSel(1);
	CString filename,filetype,filepath,filesize;
	int j=0;
	for(int i=0; i<m_List_Search.GetItemCount(); i++)
	{
		if( m_List_Search.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{	
			filename=m_List_Search.GetItemText(i,0);
			filetype=m_List_Search.GetItemText(i,1);
			//filesize=(float)_ttoi(m_List_Search.GetItemText(i,2));
			filesize=(m_List_Search.GetItemText(i,2));
			filepath=m_List_Search.GetItemText(i,3);
			dialog->m_download.m_download_downloading.NewDownLoad(filename,filetype,filesize,filepath,j);
			j++;
		}	
	}
}


afx_msg LRESULT CSearch::OnOnOKSearch(WPARAM wParam, LPARAM lParam)   //回车键开始搜索
{
	OnBnClickedButtonSearchtofind();
	return 0;
}
//拦截发往窗口的消息    当鼠标在编辑框或者鼠标焦点在搜索按键上时，按下回车键开始搜索
BOOL CSearch::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
		
		if(GetFocus()->GetDlgCtrlID()==IDC_EDIT_Search ||GetFocus()->GetDlgCtrlID()==IDC_BUTTON_SearchToFind)
		{
			this->SendMessage(WM_ONOKSearch);
			return TRUE;
		}
		return CDialog::PreTranslateMessage(pMsg);
}
CString CSearch::FileSizeTransfer(double &NUM)  //用于将Byte转换为MB  方便显示
{
	CString _str;
	if(NUM/1024/1024/1024>=1)
	{
		_str.Format("%.2f GB",NUM/1024/1024/1024);
		return _str;
	}
	if(NUM/1024/1024>=1)
	{
		_str.Format("%.2f MB",NUM/1024/1024);
		return _str;
	}
	if(NUM/1024>=1)
	{
		_str.Format("%.2f KB",NUM/1024);
		return _str;
	}
	if(NUM>0)
	{
		_str.Format("%.2f B",NUM/1024);
		return _str;
	}
	_str=_T("");
	return _str;
	
} 