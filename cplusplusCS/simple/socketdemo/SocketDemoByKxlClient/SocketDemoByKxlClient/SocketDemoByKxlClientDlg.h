
// SocketDemoByKxlClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "socket/SocketDefines.h"

// CSocketDemoByKxlClientDlg 对话框
class CSocketDemoByKxlClientDlg : public CDialog
{
// 构造
public:
	CSocketDemoByKxlClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SOCKETDEMOBYKXLCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    BOOL    InitSocket();
    void    GetLocalIp();
	BOOL	SendReq();			//向服务器发送请求  
	BOOL	RecvData();			//接收从服务器返回的数据


	
public:
	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL SendTextMessage();	
	void UnInitSocke();
	BOOL RecvTextMessage(int len);
	BOOL RecvMessageFromServer(int len);	
	BOOL SendReplyMessageToServer();
public:
	enumREQTYPE   m_nCurReq;    //当前请求类型
    SOCKET   m_sockWhole;
    BOOL     m_bIsRunning;

	CString  m_sSendMessage;    //发送的消息缓存  暂时这么写而已 实际应用使用map缓存
	CString  m_sRecvMessage;    //接收的消息   暂时这么存储而已 实际应用可以用map
	CEdit m_chat;
	CEdit m_message;
	CEdit m_statusSend;
	CEdit m_statusLink;
	CEdit m_port;
	CIPAddressCtrl m_ip;
public:
	afx_msg void OnBnClickedBtnLink();
	afx_msg void OnBnClickedBtnSend();
	void   UpdateChatEdit(CString &newMessage);    //更新窗口信息
	void   GetCurTime(CString& time);
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnStop();
};
