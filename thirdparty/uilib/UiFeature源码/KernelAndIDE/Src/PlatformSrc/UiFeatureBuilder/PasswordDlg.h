#pragma once
#include "IUiFeatureKernel.h"
#include "resource.h"


// CPasswordDlg dialog

class CPasswordDlg : public CDialog
{
	DECLARE_DYNAMIC(CPasswordDlg)

public:
	CPasswordDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPasswordDlg();

// Dialog Data
	enum { IDD = IDD_PASSWORD_SET_DLG };

	void GetPassword(CString &strOutPassword);

	void Init(CString &strOldPassword);

private:
	CString m_strOldPassword;
	CString m_strNewPassword;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDelPassword();
};
