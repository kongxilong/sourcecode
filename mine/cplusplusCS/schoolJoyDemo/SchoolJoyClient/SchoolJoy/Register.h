#pragma once
//#include "ADOConn.h"

// CRegister 对话框

class CRegister : public CDialogEx
{
	DECLARE_DYNAMIC(CRegister)

public:
	CRegister(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegister();

// 对话框数据
	enum { IDD = IDD_DIALOG_Register };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConfirmregister();
	//void UserInfoTransfer(CString &username,CString &password);
//	CString m_edit_ConfirmPassword;
//	CString m_edit_Password;
	CString m_edit_RUsername;//Add by wqdnan 用户名Edit对话框内容
	CString m_edit_RConfirmPassword;//Add by wqdnan 确认密码Edit对话框内容
	CString m_edit_RPassword;//Add by wqdnan 密码Edit对话框内容
	static CString username;
	static CString password;
};
