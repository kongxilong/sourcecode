
// SocketDemoByKxlDlg.h : 头文件
//
#include "Resource.h"
#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "socketclient/SyncLocker.h"
#include "socketclient/CommucationProtocol.h"
class CClient;

// CSocketDemoByKxlDlg 对话框
class CSocketDemoByKxlDlg : public CDialog
{
// 构造
public:
	CSocketDemoByKxlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SOCKETDEMOBYKXL_DIALOG };

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
	LRESULT OnRecordLog(WPARAM wParam, LPARAM lParam);
	LRESULT OnRecvTextMessage(WPARAM wParam, LPARAM lParam);
	LRESULT OnRecvReplyFromClient(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()


public:
   void			InitSocket(void);			//初始化socket
   void         UnInitSocke();
   void         GetLocalIp();
   void         GetLogTime(CString &time);    //获得记录日志的时间
   void         WriteInfoToSysLog(CString log); 
   void         RecordLog(CString log);
   void         ShowClientNumberInfor(int nNum);
   CClient*     GetClient(SOCKET s);
   void         AddClient(CClient *pClient);   //添加客户端节点
   void         DeleteAllClient(void);
   void         DeleteClient(SOCKET s);
   inline SOCKET GetSocket(void)				//获得套接字
   {
	   return m_sockServer;
   }
   inline BOOL   IsRunning()
   {
	   return m_bRunning;
   }
   void  GetCurTime(CString& time);
 
public:
	SOCKET	    m_sockServer;					//服务器监听套接结字
	BOOL		m_bRunning;					//服务器连接状态
	static	    FD_SET	m_allSockfd;             //服务器所有套接字集合
public:
	CEdit       m_message;
	CEdit       m_status;
	CEdit       m_port;
	CIPAddressCtrl m_ip;
	HANDLE		    m_hAcceptAndHandle;				//接收和处理客户端请求线程句柄
	CSyncLocker 	m_listLocker;				//保护链表的临界区对象	
	std::list<CClient*>		m_listClient;		//管理客户端的链表
    
    CEdit      m_statusSend;
    CEdit      m_text;
public:
	BOOL         PreTranslateMessage(MSG*   pMsg);
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnClearmessage();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnClearLog();
	afx_msg void OnBnClickedBtnShowlog();
};
