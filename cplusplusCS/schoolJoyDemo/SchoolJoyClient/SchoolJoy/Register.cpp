// Register.cpp : 实现文件
//

#include "stdafx.h"
#include "SchoolJoy.h"
#include "Register.h"
#include "afxdialogex.h"
#include "ADOConn.h"
#include "threadoperate.h"
// CRegister 对话框

IMPLEMENT_DYNAMIC(CRegister, CDialogEx)
CString CRegister::username=_T("");
CString CRegister::password=_T("");
//CString CRegister::m_edit_RPassword=_T("");
CRegister::CRegister(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegister::IDD, pParent)
	, m_edit_RUsername(_T(""))
	, m_edit_RConfirmPassword(_T(""))
	, m_edit_RPassword(_T(""))
{

}

CRegister::~CRegister()
{
}

void CRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT_ConfirmPassword, m_edit_ConfirmPassword);
	//  DDX_Text(pDX, IDC_EDIT_Register_Password, m_edit_Password);
	DDX_Text(pDX, IDC_EDIT_Register_Username, m_edit_RUsername);
	DDX_Text(pDX, IDC_EDIT_ConfirmPassword, m_edit_RConfirmPassword);
	DDX_Text(pDX, IDC_EDIT_Register_Password, m_edit_RPassword);
}


BEGIN_MESSAGE_MAP(CRegister, CDialogEx)
	ON_BN_CLICKED(ID_Button_ConfirmRegister, &CRegister::OnBnClickedButtonConfirmregister)
END_MESSAGE_MAP()


// CRegister 消息处理程序


void CRegister::OnBnClickedButtonConfirmregister()
{
	// TODO: 在此添加控件通知处理程序代码
	
	UpdateData(TRUE);
	if (""==m_edit_RUsername)
	{
		MessageBox(_T("用户名不能为空"),_T("警告"),MB_ICONWARNING|MB_OK);
		return;
	}
	
	if (""==m_edit_RPassword)
	{
		MessageBox(_T("密码不能为空"),_T("警告"),MB_ICONWARNING|MB_OK);
		return;	
	}
	if (m_edit_RConfirmPassword!=m_edit_RPassword)
	{
		MessageBox(_T("密码不一致，请重新输入"),_T("警告"),MB_ICONWARNING|MB_OK);
		//m_Password.SetFocus();
		m_edit_RPassword = "";
		m_edit_RConfirmPassword = "";
		UpdateData(FALSE);
	    return;	
	}
	username=m_edit_RUsername;
	password=m_edit_RPassword;
	ThreadOperate RegisterThread;//开启注册线程
	void* dialog=(void*)this;
	RegisterThread.startRegisterThread(dialog);	
	CDialogEx::OnOK();
}
