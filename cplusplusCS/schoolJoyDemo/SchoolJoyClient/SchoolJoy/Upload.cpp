// Upload.cpp : 实现文件
//

#include "stdafx.h"
#include "SchoolJoy.h"
#include "Upload.h"
#include "afxdialogex.h"
#include "LogIn.h"
#include "ThreadOperate.h"
extern CString m_usernameShow;

// CUpload 对话框

IMPLEMENT_DYNAMIC(CUpload, CDialogEx)

CUpload::CUpload(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpload::IDD, pParent)
	, m_uploadNum(0)
{

}

CUpload::~CUpload()
{
}

void CUpload::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Upload, m_List_Upload);
}


BEGIN_MESSAGE_MAP(CUpload, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_UploadBrowse, &CUpload::OnBnClickedButtonUploadbrowse)
END_MESSAGE_MAP()


// CUpload 消息处理程序

//Add by wqdnan 用于创建列表的格式及写入初始值
void CUpload::InitUpload(void)
{
	CRect rect;    
 
    // 获取编程语言列表视图控件的位置和大小    
    m_List_Upload.GetClientRect(&rect);    
 
    // 为列表视图控件添加全行选中和栅格风格    
    m_List_Upload.SetExtendedStyle(m_List_Upload.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);    
 
	m_List_Upload.SetTextColor(RGB(139, 90, 43));

	m_List_Upload.SetTextBkColor(RGB(255, 236, 139));

    // 为列表视图控件添加三列    
    m_List_Upload.InsertColumn(0, _T("编号"), LVCFMT_CENTER, rect.Width()/6, 0);    
    m_List_Upload.InsertColumn(1, _T("文件名"), LVCFMT_CENTER, rect.Width()/2, 1);    
    m_List_Upload.InsertColumn(2, _T("用户列表"), LVCFMT_CENTER, rect.Width()/6, 2);   
	m_List_Upload.InsertColumn(3, _T("文件大小"), LVCFMT_CENTER, rect.Width()/6, 3);  
	m_uploadNum = 0;
 
	/*
    // 在列表视图控件中插入列表项，并设置列表子项文本    
    m_List_Upload.InsertItem(0, _T("文件一"));    
    m_List_Upload.SetItemText(0, 1, _T("0"));    
    m_List_Upload.SetItemText(0, 2, _T("0"));    
    m_List_Upload.InsertItem(1, _T("文件二"));    
    m_List_Upload.SetItemText(1, 1, _T("0"));    
    m_List_Upload.SetItemText(1, 2, _T("0"));    
    m_List_Upload.InsertItem(2, _T("文件三"));    
    m_List_Upload.SetItemText(2, 1, _T("0"));    
    m_List_Upload.SetItemText(2, 2, _T("0"));    
    m_List_Upload.InsertItem(3, _T("文件四"));    
    m_List_Upload.SetItemText(3, 1, _T("0"));    
    m_List_Upload.SetItemText(3, 2, _T("0")); 
	*/
}

//Add by wqdnan 用于调用初始化列表内容值
BOOL CUpload::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	InitUpload();
	return 0;
}


void CUpload::OnBnClickedButtonUploadbrowse()
{
	TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");      
    CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);    
	CString strNum;      
    if (IDOK == fileDlg.DoModal())    
    {  
		//CLogIn logIn;  
        // 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里    
        strFilePath = fileDlg.GetPathName();//获得选定文件的路径全名   
		strFileName = fileDlg.GetFileTitle();//获得选定文件的名称，不包括后缀名。
		strFileUserList = CLogIn::username;//logIn.GetUsername();
		CFileStatus rStatus;                      
		CFile::GetStatus(strFilePath,rStatus);      
		fFileTotalSize = (float)rStatus.m_size; 
		strFileTotalSize = CalculateFileSize(rStatus.m_size);      //获取文件大小;
		strFileType = fileDlg.GetFileExt();//获取选定文件的后缀名
		ThreadOperate UploadThread;//开启上传线程
		void* dialog=(void*)this;
		UploadThread.startUploadThread(dialog);
    }     
}


CString CUpload::intToStr(int n)
{
	
	return 0;
}
CString CUpload::CalculateFileSize(ULONGLONG ulong)
{
	//获取文件大小单位表示;
	CString filesize;
	double f= (double)ulong;
	if (f/1024>1)
	{
		if (f/1024/1024>1)
		{
			if (f/1024/1024/1024>1)
			{
				filesize.Format(_T("%.2fGB"),f/1024/1024/1024);   //文件大小GB显示;
				return filesize;
			}
			filesize.Format(_T("%.2fMB"),f/1024/1024);     //文件大小MB显示;
			return filesize;
		}
		filesize.Format(_T("%.2fKB"),f/1024);		 //文件大小KB显示;
	}
	else
	{
		filesize.Format(_T("%I64uB"),ulong);        //文件大小B显示;
	}
	return filesize;
}
