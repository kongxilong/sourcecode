#pragma once
#include "afxwin.h"


// CSet 对话框

class CSet : public CDialogEx
{
	DECLARE_DYNAMIC(CSet)

public:
	CSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_Set };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckBasicset2();
	afx_msg void OnBnClickedButtonSaveset();
	CButton m_check_basicSet1;
	CButton m_check_basicSet2;
	CButton m_check_basicSet3;
	CButton m_check_task1;
	CButton m_check_task2;
	CButton m_check_task3;
	CString m_edit_oldPassword;
//	CEdit m_edit_newPassword;
	CString m_edit_newPassword;
	CString m_edit_confirmPassword;
	CString m_edit_taskPath;
	void ReadSet(void);
	BOOL OnInitDialog(void);
};
