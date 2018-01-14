// ThreadServerDlg.h : header file
//

#include "afxcmn.h"
#if !defined(AFX_THREADSERVERDLG_H__FF9F3EF2_DC1E_4E15_9B2A_25F06B85CA72__INCLUDED_)
#define AFX_THREADSERVERDLG_H__FF9F3EF2_DC1E_4E15_9B2A_25F06B85CA72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CThreadServerDlg dialog

class CThreadServerDlg : public CDialog
{
// Construction
public:
	CThreadServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CThreadServerDlg)
	enum { IDD = IDD_THREADSERVER_DIALOG };
//	CListCtrl	m_listView;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CThreadServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//public:
	//afx_msg void OnEnChangeEditLog();
public:
	afx_msg void OnBnClickedButtonStatrservice();
	afx_msg void OnBnClickedButtonClear();
	int Update(CString &_str);
	int GetLocalHostName(CString &sHostName);
	int GetIpAddress(const CString &sHostName, CString &sIpAddress);//获得本地IP
	int GetNamebyAddress(const CString &IpAddress,CString &sYouName);
	bool showUserListView(void);
	bool showResourcesListView(void);
	CString m_serverIP;
	int m_serverPORT;
	CListCtrl m_Userlist;
	CListCtrl m_Resourceslist;
//	afx_msg void OnLvnItemchangedListUserlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonUpdateuserlist();
	afx_msg void OnBnClickedButton4();
	CString FileSizeTransfer(double &NUM);
	void ShowTray();   //显示托盘，不是消息响应函数，不加afx_msg
	afx_msg LRESULT OnTray(WPARAM wParam, LPARAM lParam);//对托盘的处理函数
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREADSERVERDLG_H__FF9F3EF2_DC1E_4E15_9B2A_25F06B85CA72__INCLUDED_)
