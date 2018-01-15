#pragma once


// CConnectSet 对话框

class CConnectSet : public CDialogEx
{
	DECLARE_DYNAMIC(CConnectSet)

public:
	CConnectSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConnectSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_ConnectSet};


public:
	HWND m_hWnd;
	CString m_serverIp;
	int m_serverPort;
	static CString ServerIP;
	static int ServerPort;
	void OnOK();
	void OnCancel();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonOk();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
