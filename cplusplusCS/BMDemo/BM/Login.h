#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CLogin 对话框

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogin();

// 对话框数据
	enum { IDD = IDD_DLG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CSplitButton	m_split;
	CComboBox		m_addr;
	CComboBox		m_pos;
	CComboBox		m_username;
	CDateTimeCtrl	m_dateDealNew;
	loginInfo		m_loginInfo_Stru; 
	//BOOL m_bLoginSuccess;     //登录是否成功的标志
	BOOL   m_bWorking;    //正在进行登录
public:
	void    SetCloseButton(BOOL b);
	afx_msg void OnBnClickedButtonPwdedit();

	void GoToMainApp();		//登录成功后 进入主程序界面
	
	//afx_msg		void OnBnClickedSplitAddrAdd();
	virtual		BOOL OnInitDialog();
	//void		InitSplit();
	afx_msg		void OnBnClickedBtnLogin();
	afx_msg		LRESULT OnLoginFail(WPARAM wParam, LPARAM lParam);
protected:
	afx_msg		LRESULT OnLoginSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT OnCursorToNormal(WPARAM wParam, LPARAM lParam);
	afx_msg		void OnBnClickedBtnGoBack();


	void	WriteToText(CString& strToWtrite,CString& writeToPath);		//将str 写到path路径的文件中 覆盖原有的
	BOOL	ReadFromTxt(CString& path,CString& result);
	void	WriteCombToFile(void* comb, CString path);				//写到text文件中
	BOOL	LoadComb(void* ptr,CString& path);       //从文本文件中加载combox 的内容
	BOOL	WriteUserToIni();       //将用户名和密码写进配置文件
	void	EncryptCode(CString& result);     //加密
	void	DecryptCode(CString source,CString& result);    //将密码还原
	void    LoadUser();       //初始化用户名和密码
	void    RestoreCode(CString& midValue,char& strReturn);   //将每两个#号之间的字符串还原为原密码的一位字符
public:
	
	CStatic		m_status;
	CButton		m_remPsw;
	CString		m_password;
	afx_msg void OnCbnSelchangeComboLoginUsername();
	afx_msg void OnCbnEditchangeComboLoginUsername();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

//	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
