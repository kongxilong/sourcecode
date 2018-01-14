// PasswordVerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UiFeatureBuilder.h"
#include "PasswordVerDlg.h"
#include "resource.h"
#include "CA2W_W2A.h"
#include "..\..\Inc\UiFeatureDefs.h"


// CPasswordVerDlg dialog

IMPLEMENT_DYNAMIC(CPasswordVerDlg, CDialog)

CPasswordVerDlg::CPasswordVerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswordVerDlg::IDD, pParent)
{
	m_pSkinMgr = NULL;
	m_strSkinPath = _T("");
	m_strPassword = _T("");
}

CPasswordVerDlg::~CPasswordVerDlg()
{
}

void CPasswordVerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPasswordVerDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CPasswordVerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CPasswordVerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPasswordVerDlg message handlers

void CPasswordVerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CPasswordVerDlg::OnBnClickedOk()
{
	if (m_pSkinMgr == NULL)
		return;

	m_strPassword = _T("");
	this->GetDlgItemText(IDE_VER_PASSWORD, m_strPassword);
	if (m_strPassword.IsEmpty())
	{
		AfxMessageBox(_T("ÇëÊäÈëÆ¤·ô°üÑéÖ¤ÃÜÂë£¡"), MB_OK | MB_ICONERROR);
		this->GetDlgItem(IDE_VER_PASSWORD)->SetFocus();
		return;
	}

	if (m_strPassword.GetLength() > USER_PASSWORD_KEY_LEN)
	{
		AfxMessageBox(_T("ÊäÈë´íÎó£¬ÃÜÂë³¤¶È²»ÄÜ³¬¹ý128×Ö½Ú£¡"), MB_OK | MB_ICONERROR);
		this->GetDlgItem(IDE_VER_PASSWORD)->SetFocus();
		return;
	}

	USES_CONVERSION_F;
	if (!m_pSkinMgr->ChekUserPassword(W2A_F(m_strPassword.LockBuffer()), m_strSkinPath))
	{
		m_strPassword.UnlockBuffer();
		AfxMessageBox(_T("±§Ç¸£¬ÊäÈëµÄÃÜÂë´íÎó£¡"), MB_OK | MB_ICONERROR);
		this->GetDlgItem(IDE_VER_PASSWORD)->SetFocus();
		return;
	}
	m_strPassword.UnlockBuffer();

	OnOK();
}

void CPasswordVerDlg::GetPassword(CString &strOutPassword)
{
	strOutPassword = m_strPassword;
}

void CPasswordVerDlg::Init(IPropertySkinManager* pSkinMgr, CString &strUfd)
{
	m_pSkinMgr = pSkinMgr;
	m_strSkinPath = strUfd;
}
