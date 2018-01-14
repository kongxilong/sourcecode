// Set.cpp : 实现文件
//

#include "stdafx.h"
#include "SchoolJoy.h"
#include "Set.h"
#include "afxdialogex.h"


// CSet 对话框

IMPLEMENT_DYNAMIC(CSet, CDialogEx)

CSet::CSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSet::IDD, pParent)
	, m_edit_oldPassword(_T(""))
	, m_edit_newPassword(_T(""))
	, m_edit_confirmPassword(_T(""))
	, m_edit_taskPath(_T(""))
{

}

CSet::~CSet()
{
}

void CSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_BasicSet1, m_check_basicSet1);
	DDX_Control(pDX, IDC_CHECK_BasicSet2, m_check_basicSet2);
	DDX_Control(pDX, IDC_CHECK_BasicSet3, m_check_basicSet3);
	DDX_Control(pDX, IDC_CHECK_Task1, m_check_task1);
	DDX_Control(pDX, IDC_CHECK_Task2, m_check_task2);
	DDX_Control(pDX, IDC_CHECK_Task3, m_check_task3);
	DDX_Text(pDX, IDC_EDIT_OldPassword, m_edit_oldPassword);
	//  DDX_Control(pDX, IDC_EDIT_NewPassword, m_edit_newPassword);
	DDX_Text(pDX, IDC_EDIT_NewPassword, m_edit_newPassword);
	DDX_Text(pDX, IDC_EDIT_ConfirmPassword, m_edit_confirmPassword);
	DDX_Text(pDX, IDC_EDIT_TaskPath, m_edit_taskPath);
}


BEGIN_MESSAGE_MAP(CSet, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_BasicSet2, &CSet::OnBnClickedCheckBasicset2)
	ON_BN_CLICKED(ID_Button_SaveSet, &CSet::OnBnClickedButtonSaveset)
END_MESSAGE_MAP()


// CSet 消息处理程序


void CSet::OnBnClickedCheckBasicset2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSet::OnBnClickedButtonSaveset()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFileSel = "";
	/*记录的形式为：
	“  #*#*#*        #*       #*#*#*#*”
	  |基本设置|  |用户设置|  |任务设置|
							*/				
    UpdateData(TRUE);// 将各控件中的数据保存到相应的变量  
	if (1 == m_check_basicSet1.GetCheck())    //记录基本设置
    {    
		strFileSel += _T("#1");    
    } 
	else
	{
		strFileSel += _T("#0");
	}
	if (1 == m_check_basicSet2.GetCheck())    
    {    
		strFileSel += _T("#1");    
    } 
	else
	{
		strFileSel += _T("#0");
	}
	if (1 == m_check_basicSet3.GetCheck())    
    {    
		strFileSel += _T("#1");    
    } 
	else
	{
		strFileSel += _T("#0");
	}
	if(m_edit_newPassword == m_edit_confirmPassword)//还需判断原密码是否正确 //记录密码
	{
		strFileSel += _T("#");
		strFileSel += m_edit_newPassword;
	}
	else
	{
		strFileSel += _T("#");
		strFileSel += m_edit_oldPassword;
	}
	strFileSel += _T("#");//记录默认路径
	strFileSel += m_edit_taskPath;
	if (1 == m_check_task1.GetCheck())    //记录任务设置
    {    
		strFileSel += _T("#1");    
    } 
	else
	{
		strFileSel += _T("#0");
	}
	if (1 == m_check_task2.GetCheck())    //记录任务设置
    {    
		strFileSel += _T("#1");    
    } 
	else
	{
		strFileSel += _T("#0");
	}
	if (1 == m_check_task3.GetCheck())    //记录任务设置
    {    
		strFileSel += _T("#1");    
    } 
	else
	{
		strFileSel += _T("#0");
	}
	UpdateData(FALSE);  
	//现在进行写文件操作
	char* pszFileName="myfile.txt";
	CStdioFile myFile;
	CFileException fileException;
	if(myFile.Open(pszFileName,CFile::typeText|CFile::modeCreate|CFile::modeReadWrite),&fileException)
	{
		myFile.WriteString(strFileSel);
	}
	else
	{
		TRACE("Can't open file %s,error=%u\n",pszFileName,fileException.m_cause);
	}
	myFile.Close();
	CDialogEx::OnOK(); 
}


void CSet::ReadSet(void)
{
	char* pszFileName="myfile.txt";
	CStdioFile myFile;
	CFileException fileException;
	if(myFile.Open(pszFileName,CFile::typeText|CFile::modeReadWrite),&fileException)
	{
		//myFile.SeekToBegin();
		CString strFileSel;
		CString password = "";
		CString path = "";
		myFile.ReadString(strFileSel);
		char * p = (LPSTR)(LPCTSTR)strFileSel;
		int flag = 0;
		while(1)
		{
			if(*p == '#')
			{
				flag ++;
				p ++;
				if(flag == 1)//恢复基本设置
				{
					if(*p == '1')
					{
						m_check_basicSet1.SetCheck(1);
					}
					else
					{
						m_check_basicSet1.SetCheck(0);
					}
				}
				else if(flag == 2)
				{
					if(*p == '1')
					{
						m_check_basicSet2.SetCheck(1);
					}
					else
					{
						m_check_basicSet2.SetCheck(0);
					}
				}
				else if(flag == 3)
				{
					if(*p == '1')
					{
						m_check_basicSet3.SetCheck(1);
					}
					else
					{
						m_check_basicSet3.SetCheck(0);
					}
				}
				else if(flag == 4)//记录密码
				{
					while(*p != '#')
					{
						password += *p;
						p ++;
					}
					p --;
					SetDlgItemText(IDC_EDIT_OldPassword, password);
				}
				else if(flag == 5)//记录路径
				{
					while(*p != '#')
					{
						path += *p;
						p ++;
					}
					p --;
					SetDlgItemText(IDC_EDIT_TaskPath, path);
				}
				else if(flag == 6)
				{
					if(*p == '1')
					{
						m_check_task1.SetCheck(1);
					}
					else
					{
						m_check_task1.SetCheck(0);
					}
				}
				else if(flag == 7)
				{
					if(*p == '1')
					{
						m_check_task2.SetCheck(1);
					}
					else
					{
						m_check_task2.SetCheck(0);
					}
				}
				else if(flag == 8)
				{
					if(*p == '1')
					{
						m_check_task3.SetCheck(1);
					}
					else
					{
						m_check_task3.SetCheck(0);
					}
					break;
				}

			}
			p ++;
		}
		
	}
	else
	{
		TRACE("Can't open file %s,error=%u\n",pszFileName,fileException.m_cause);
	}
	myFile.Close();

}


BOOL CSet::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	ReadSet();
	return 0;
}
