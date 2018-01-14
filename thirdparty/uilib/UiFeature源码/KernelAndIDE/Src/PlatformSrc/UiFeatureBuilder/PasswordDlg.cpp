// PasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UiFeatureBuilder.h"
#include "PasswordDlg.h"
#include "CA2W_W2A.h"
#include "..\..\Inc\UiFeatureDefs.h"


// CPasswordDlg dialog

IMPLEMENT_DYNAMIC(CPasswordDlg, CDialog)

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswordDlg::IDD, pParent)
{
	m_strOldPassword = _T("");
	m_strNewPassword = _T("");
}

CPasswordDlg::~CPasswordDlg()
{
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPasswordDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CPasswordDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CPasswordDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDB_DEL_PASSWORD, &CPasswordDlg::OnBnClickedDelPassword)
END_MESSAGE_MAP()


// CPasswordDlg message handlers

void CPasswordDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CPasswordDlg::Init(CString &strOldPassword)
{
	m_strOldPassword = strOldPassword;
}

BOOL CPasswordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetDlgItemText(IDE_F_PASSWORD, m_strOldPassword);

	this->GetDlgItem(IDB_DEL_PASSWORD)->EnableWindow(!m_strOldPassword.IsEmpty());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPasswordDlg::OnBnClickedOk()
{
	m_strNewPassword = _T("");
	this->GetDlgItemText(IDE_F_PASSWORD, m_strNewPassword);
	if (m_strNewPassword.IsEmpty())
	{
		AfxMessageBox(_T("请输入密码！"), MB_OK | MB_ICONERROR);
		this->GetDlgItem(IDE_F_PASSWORD)->SetFocus();
		return;
	}
	if (m_strNewPassword.GetLength() > USER_PASSWORD_KEY_LEN)
	{
		AfxMessageBox(_T("输入错误，密码长度不能超过128字节！"), MB_OK | MB_ICONERROR);
		this->GetDlgItem(IDE_F_PASSWORD)->SetFocus();
		return;
	}

	CString strSPassword = _T("");
	this->GetDlgItemText(IDE_S_PASSWORD, strSPassword);
	if (strSPassword.Compare(m_strNewPassword) != 0)
	{
		AfxMessageBox(_T("两次输入的密码不一致！"), MB_OK | MB_ICONERROR);
		this->GetDlgItem(IDE_S_PASSWORD)->SetFocus();
		return;
	}

	OnOK();
}

void CPasswordDlg::GetPassword(CString &strOutPassword)
{
	strOutPassword = m_strNewPassword;
}

void CPasswordDlg::OnBnClickedDelPassword()
{
	m_strNewPassword = _T("");
	OnOK();
}
