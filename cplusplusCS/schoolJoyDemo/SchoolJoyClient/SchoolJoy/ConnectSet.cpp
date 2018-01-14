// ConnectSet.cpp : 实现文件
//

#include "stdafx.h"
#include "SchoolJoy.h"
#include "ConnectSet.h"
#include "afxdialogex.h"


// CConnectSet 对话框
#define SERVERPORT 12344
#define SERVERIP   "10.6.8.16" 
CString CConnectSet::ServerIP=SERVERIP;
int CConnectSet::ServerPort=SERVERPORT;


IMPLEMENT_DYNAMIC(CConnectSet, CDialogEx)

CConnectSet::CConnectSet(CWnd* pParent /*=NULL*/): CDialogEx(CConnectSet::IDD, pParent)

{
	m_serverIp=SERVERIP;
	m_serverPort=SERVERPORT;
}

CConnectSet::~CConnectSet()
{
}

void CConnectSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ServerIp, m_serverIp);
	DDX_Text(pDX, IDC_EDIT_ServerPort, m_serverPort);
}


BEGIN_MESSAGE_MAP(CConnectSet, CDialogEx)
//	ON_BN_CLICKED(IDC_BUTTON1, &CConnectSet::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_Cancel, &CConnectSet::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CConnectSet::OnBnClickedButtonOk)
END_MESSAGE_MAP()


// CConnectSet 消息处理程序


//void CConnectSet::OnBnClickedButton1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}
void CConnectSet::OnOK()
{
}
void CConnectSet::OnCancel()
{
}

void CConnectSet::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}



void CConnectSet::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	ServerIP=m_serverIp;
	ServerPort=m_serverPort;
	CDialog::OnOK();
}
