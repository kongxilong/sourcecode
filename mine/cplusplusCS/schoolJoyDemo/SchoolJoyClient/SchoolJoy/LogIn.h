#pragma once

#include "Register.h"
#include "ADOConn.h"
// CLogIn 对话框

#define LogInRight 35
#define LogInWrong 53


class CLogIn : public CDialogEx
{
	DECLARE_DYNAMIC(CLogIn)

public:
	CLogIn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogIn();

// 对话框数据
	enum { IDD = IDD_DIALOG_LogIn };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_edit_username;
	CString m_edit_password;
	afx_msg void OnBnClickedButtonLogin();
//	int m_login_status;
//	int GetLogInStatus(void);

	int GetLogInStatus(void);
	int m_loginstatus;
	afx_msg void OnBnClickedButtonRegister();
	
	CString GetUsername(void);
	static CString username;
	static CString password;
	afx_msg void OnBnClickedButtonAdvancedset();
	afx_msg void OnBnClickedButtonQuit();
};
