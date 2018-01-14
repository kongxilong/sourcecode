#pragma once
#include "afxwin.h"


// CDBOperatDlg 对话框

class CDBOperatDlg : public CDialog
{
	DECLARE_DYNAMIC(CDBOperatDlg)

public:
	CDBOperatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDBOperatDlg();

// 对话框数据
	enum { IDD = IDD_DLG_DB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnNewCompany();
	afx_msg void OnBnClickedBtnNewDepartment();
	afx_msg void OnBnClickedBtnNewEmploy();
	afx_msg void OnBnClickedBtnDeleteCompany();
	afx_msg void OnBnClickedBtnDeleteDepartment();
	afx_msg void OnBnClickedBtnDeleteEmploy();
	CEdit m_companyName;
	CEdit m_departmentName;
	CEdit m_employName;
};
