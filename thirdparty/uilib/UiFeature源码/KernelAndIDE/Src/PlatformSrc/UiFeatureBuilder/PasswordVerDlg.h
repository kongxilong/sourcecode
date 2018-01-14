#pragma once
#include "IUiFeatureKernel.h"
#include "resource.h"


// CPasswordVerDlg dialog

class CPasswordVerDlg : public CDialog
{
	DECLARE_DYNAMIC(CPasswordVerDlg)

public:
	CPasswordVerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPasswordVerDlg();

// Dialog Data
	enum { IDD = IDD_PASSWORD_VER_DLG };

	void GetPassword(CString &strOutPassword);

	void Init(IPropertySkinManager* pSkinMgr, CString &strUfd);

private:
	IPropertySkinManager* m_pSkinMgr;
	CString m_strSkinPath;
	CString m_strPassword;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
